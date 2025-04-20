#include "jigsaw/presentation/jigsaw_generic_node.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/hash_set.hpp>

static LazyVariant<NodePath, const char *> QUEUE_FREE{"queue_free"};
static LazyVariant<NodePath, const char *> VISIBLE{"visible"};

void JigsawGenericNodeCommandHistory::_bind_methods() {
	BIND_PROPERTY(Variant::INT, rollback_frame);
	BIND_PROPERTY(Variant::NODE_PATH, property_name);
	BIND_PROPERTY_RESOURCE(Tween, tween);
	BIND_PROPERTY(Variant::NIL, from_value);
	BIND_PROPERTY(Variant::NIL, to_value);
	BIND_PROPERTY(Variant::FLOAT, duration);
	BIND_PROPERTY(Variant::FLOAT, skip_duration);

	ClassDB::bind_method(D_METHOD("get_duration_in_frames"), &JigsawGenericNodeCommandHistory::get_duration_in_frames);
}

IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, int64_t, rollback_frame);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, NodePath, property_name);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, Ref<Tween>, tween);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, Variant, from_value);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, Variant, to_value);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, float, duration);
IMPLEMENT_PROPERTY(JigsawGenericNodeCommandHistory, float, skip_duration);

int JigsawGenericNodeCommandHistory::get_duration_in_frames() const {
	return Math::fast_ftoi((get_duration() - get_skip_duration()) * Engine::get_singleton()->get_physics_ticks_per_second());
}

void JigsawGenericNode::_bind_methods() {
	BIND_PROPERTY_OBJECTID_NOT_SAVED(Node, node);
	BIND_PROPERTY(Variant::BOOL, owns_node);
	BIND_PROPERTY_IS(Variant::BOOL, destroyed);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawGenericNodeCommandHistory, history);

	ClassDB::bind_method(D_METHOD("set_property", "name", "variant", "duration", "current_frame"), &JigsawGenericNode::set_property, DEFVAL(0.0f), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("stop_property", "name", "current_frame"), &JigsawGenericNode::stop_property, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("destroy", "current_frame"), &JigsawGenericNode::destroy, DEFVAL(-1));
}

IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(JigsawGenericNode, Node, node);
IMPLEMENT_PROPERTY(JigsawGenericNode, bool, owns_node);
IMPLEMENT_PROPERTY_IS(JigsawGenericNode, bool, destroyed);
IMPLEMENT_PROPERTY(JigsawGenericNode, TypedArray<JigsawGenericNodeCommandHistory>, history);

void JigsawGenericNode::kill_node() {
	ERR_FAIL_COND(!_owns_node);

	Node *node = get_node();
	if (node) {
		node->set_indexed(VISIBLE, false);
		node->queue_free();
		set_node(nullptr);
	}
}

void JigsawGenericNode::discard_rollback_data(int64_t new_base_frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(new_base_frame < -1);

	// special case: moving out of rollback
	if (new_base_frame == -1) {
		ERR_FAIL_COND(get_base_frame() == -1); // we weren't in rollback

		HashMap<NodePath, int64_t> last_entry;
		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawGenericNodeCommandHistory> history = _history[i];
			history->set_rollback_frame(-1);

			NodePath prop = history->get_property_name();
			if (prop == QUEUE_FREE) {
				kill_node();
			} else {
				if (last_entry.has(prop)) {
					_history.remove_at(last_entry.get(prop));
					i--;
				}
				last_entry[prop] = i;
			}
		}

		set_base_frame(-1);
		set_current_frame(-1);

		return;
	}

	// special case: moving into rollback
	if (get_base_frame() == -1) {
		ERR_FAIL_COND_MSG(new_base_frame != 0, "JigsawGenericNode::discard_rollback_data did not properly enter rollback mode");

		Node *node = get_node();
		if (!node) {
			// if we don't have a node, just set the rollback frame to keep the invariant.
			for (int64_t i = 0; i < _history.size(); i++) {
				Ref<JigsawGenericNodeCommandHistory> history = _history[i];
				history->set_rollback_frame(0);
			}
		} else {
			// we should have at most one copy of each command, so don't bother checking for duplicates.
			for (int64_t i = 0; i < _history.size(); i++) {
				Ref<JigsawGenericNodeCommandHistory> history = _history[i];
				history->set_rollback_frame(0);

				NodePath prop = history->get_property_name();
				if (prop != QUEUE_FREE) {
					history->set_from_value(node->get_indexed(prop));
					if (history->get_tween().is_valid()) {
						history->set_skip_duration(history->get_tween()->get_total_elapsed_time());
					}
					break;
				}
			}
		}

		set_base_frame(0);
		set_current_frame(0);

		return;
	}

	ERR_FAIL_COND(new_base_frame >= get_current_frame());

	HashMap<NodePath, int64_t> last_entry;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawGenericNodeCommandHistory> history = _history[i];
		if (history->get_rollback_frame() >= new_base_frame) {
			break;
		}

		NodePath prop = history->get_property_name();
		if (prop == QUEUE_FREE) {
			kill_node();
		} else {
			if (last_entry.has(prop)) {
				_history.remove_at(last_entry.get(prop));
				i--;
			}
			last_entry[prop] = i;
		}
	}

	set_base_frame(new_base_frame);
}

void JigsawGenericNode::rollback_to_frame(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(get_base_frame() == -1);
	ERR_FAIL_COND(frame < get_base_frame());
	ERR_FAIL_COND(frame > get_current_frame());

	// we're not moving in time; we're only undoing changes that happened "in the future" of this past frame.
	HashMap<NodePath, int64_t> last_entry;
	HashSet<NodePath> need_to_revert;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawGenericNodeCommandHistory> history = _history[i];
		NodePath prop = history->get_property_name();
		if (history->get_rollback_frame() > frame) {
			if (history->get_tween().is_valid()) {
				history->get_tween()->kill();
			}
			need_to_revert.insert(prop);
			_history.remove_at(i);
			i--;
		} else {
			if (prop != QUEUE_FREE) {
				last_entry[prop] = i;
			}
		}
	}

	Node *node = get_node();
	if (!node) {
		return;
	}

	for (const NodePath &to_revert : need_to_revert) {
		if (to_revert == QUEUE_FREE) {
			set_destroyed(false);
		} else if (last_entry.has(to_revert)) {
			Ref<JigsawGenericNodeCommandHistory> history = _history[last_entry.get(to_revert)];
			int64_t start_frame = history->get_rollback_frame();
			int64_t frame_length = history->get_duration_in_frames();
			if (start_frame + frame_length <= get_current_frame()) {
				node->set_indexed(to_revert, history->get_to_value());
			} else {
				// TODO: skip_duration

				float progress = float(get_current_frame() - start_frame) / float(frame_length);
				Variant delta;
				bool valid = false;
				Variant::evaluate(Variant::OP_SUBTRACT, history->get_to_value(), history->get_from_value(), delta, valid);
				ERR_CONTINUE(!valid);
				Variant from = Tween::interpolate_value(history->get_from_value(), delta, progress, 1.0, Tween::TRANS_LINEAR, Tween::EASE_IN_OUT);

				Ref<Tween> tween = node->create_tween();
				tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
				node->set_indexed(to_revert, from);
				history->set_tween(tween);

				Ref<PropertyTweener> ptween = tween->tween_property(node, to_revert, history->get_to_value(), history->get_duration() * (1.0f - progress));
				ptween->set_trans(Tween::TRANS_LINEAR);
			}
		} else {
			ERR_PRINT(vformat("internal error in rollback of %s: we don't have a previous snapshot of %s!", node->get_class(), to_revert));
		}
	}
}

void JigsawGenericNode::advance_frames(int64_t frames) {
	// JigsawGenericNode handles time travel at command time, so no fast forward is needed.
}

void JigsawGenericNode::set_property(const NodePath &name, const Variant &value, float duration, int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawGenericNodeCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());


	Ref<JigsawGenericNodeCommandHistory> previous;
	Ref<JigsawGenericNodeCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawGenericNodeCommandHistory> h = _history[i];
		if (h->get_property_name() == name && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_property_name() == name && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_property_name(name);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	Node *node = get_node();
	if (!node) {
		return;
	}

	// kill the old tween, if any
	if (previous.is_valid() && previous->get_tween().is_valid()) {
		previous->get_tween()->kill();
	}

	Variant starting_value;
	if (frame == -1 || previous.is_null()) {
		starting_value = node->get_indexed(name);
	} else {
		int64_t prev_start = previous->get_rollback_frame();
		int64_t prev_duration = previous->get_duration_in_frames();
		if (prev_start + prev_duration <= frame) {
			// easy case: previous change ended before we start
			starting_value = previous->get_to_value();
		} else {
			// slightly harder case: interpolate based on remaining frame count
			bool valid = false;
			Variant delta;
			Variant::evaluate(Variant::OP_SUBTRACT, previous->get_from_value(), previous->get_to_value(), delta, valid);
			starting_value = Tween::interpolate_value(previous->get_to_value(), delta, float(frame - prev_start) / float(prev_duration), 1.0, Tween::TRANS_LINEAR, Tween::EASE_IN_OUT);
		}
	}

	if (history->get_tween().is_valid()) {
		history->get_tween()->kill();
	}

	history->set_from_value(starting_value);
	history->set_to_value(value);
	history->set_duration(duration);

	// how far in the past is this tween being started?
	float skip_duration = Math::min(float(get_current_frame() - frame) / float(Engine::get_singleton()->get_physics_ticks_per_second()), duration);

	// only tween if we're actually animating for a nonzero duration
	if (duration > skip_duration) {
		Ref<Tween> tween = node->create_tween();
		tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
		history->set_tween(tween);

		if (frame != -1) {
			bool valid = false;
			Variant delta;
			Variant::evaluate(Variant::OP_SUBTRACT, value, starting_value, delta, valid);
			Variant from = Tween::interpolate_value(starting_value, delta, float(get_current_frame() - frame) / float(history->get_duration_in_frames()), 1.0, Tween::TRANS_LINEAR, Tween::EASE_IN_OUT);
			node->set_indexed(name, from);
		}
		tween->tween_property(node, name, value, duration - skip_duration)->set_trans(Tween::TRANS_LINEAR);
	} else {
		node->set_indexed(name, value);
	}
}

void JigsawGenericNode::stop_property(const NodePath &name, int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawGenericNodeCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	CRASH_COND(frame != get_current_frame()); // TODO: handle rollback properly

	if (!get_node()) {
		return;
	}

	Variant value = get_node()->get_indexed(name);

	set_property(name, value, 0.0f, frame);
}

void JigsawGenericNode::destroy(int64_t frame) {

}
