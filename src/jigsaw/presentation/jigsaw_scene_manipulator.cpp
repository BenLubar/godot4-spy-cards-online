#include "jigsaw/presentation/jigsaw_scene_manipulator.h"

#include <godot_cpp/classes/engine.hpp>

static double get_time_per_physics_frame() {
	return 1.0 / Engine::get_singleton()->get_physics_ticks_per_second();
}

void JigsawSceneCommandHistory::_bind_methods() {
	BIND_ENUM_CONSTANT(DESTROY_SCENE);
	BIND_ENUM_CONSTANT(TOGGLE_VISIBLE);

	BIND_PROPERTY(Variant::INT, rollback_frame);
	BIND_PROPERTY(Variant::NODE_PATH, node_path);
	BIND_PROPERTY_ENUM(JigsawSceneCommandHistory::Command, command);
}

IMPLEMENT_PROPERTY(JigsawSceneCommandHistory, int64_t, rollback_frame);
IMPLEMENT_PROPERTY(JigsawSceneCommandHistory, NodePath, node_path);
IMPLEMENT_PROPERTY(JigsawSceneCommandHistory, JigsawSceneCommandHistory::Command, command);

void JigsawSceneManipulator::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSceneCommandHistory, history);
	BIND_PROPERTY_RESOURCE(JigsawParameterScene, scene_id);
	BIND_PROPERTY_OBJECTID_NOT_SAVED(Node3D, root);
	BIND_PROPERTY_OBJECTID_NOT_SAVED(AnimationTree, animation_tree);

	ClassDB::bind_method(D_METHOD("get_root_rollback"), &JigsawSceneManipulator::get_root_rollback);
	ClassDB::bind_method(D_METHOD("destroy", "frame"), &JigsawSceneManipulator::destroy, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("set_visible", "path", "visible", "frame"), &JigsawSceneManipulator::set_visible, DEFVAL(-1));
}

IMPLEMENT_PROPERTY(JigsawSceneManipulator, TypedArray<JigsawSceneCommandHistory>, history);
IMPLEMENT_PROPERTY(JigsawSceneManipulator, Ref<JigsawParameterScene>, scene_id);
IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(JigsawSceneManipulator, Node, root);
Node *JigsawSceneManipulator::get_root_rollback() const {
	Node *root = get_root();
	if (likely(root)) {
		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawSceneCommandHistory> history = _history[i];
			if (history->get_command() == JigsawSceneCommandHistory::DESTROY_SCENE) {
				return nullptr;
			}
		}
	}

	return root;
}
IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(JigsawSceneManipulator, AnimationTree, animation_tree);

void JigsawSceneManipulator::kill_node() {
	AnimationTree *tree = get_animation_tree();
	if (tree) {
		tree->queue_free();
	}
	set_animation_tree(nullptr);

	Node *root = get_root();
	if (root) {
		Node *parent = root->get_parent();
		if (parent) {
			parent->remove_child(root);
		}

		root->queue_free();
	}

	set_root(nullptr);
}

void JigsawSceneManipulator::discard_rollback_data(int64_t new_base_frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(new_base_frame < -1);

	// special case: moving out of rollback
	if (new_base_frame == -1) {
		ERR_FAIL_COND(get_base_frame() == -1); // we weren't in rollback

		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawSceneCommandHistory> history = _history[i];
			if (history->get_command() == JigsawSceneCommandHistory::DESTROY_SCENE) {
				kill_node();
				break;
			}
		}
		_history.clear();

		// TODO: clear animation snapshot

		set_base_frame(-1);
		set_current_frame(-1);

		return;
	}

	// special case: moving into rollback
	if (get_base_frame() == -1) {
		ERR_FAIL_COND_MSG(new_base_frame != 0, "JigsawSceneManipulator::discard_rollback_data did not properly enter rollback mode");
		ERR_FAIL_COND_MSG(!_history.is_empty(), "JigsawSceneManipulator should have had empty history outside of rollback");

		if (get_animation_tree()) {
			_take_animation_snapshot();
		}

		set_base_frame(0);
		set_current_frame(0);

		return;
	}

	ERR_FAIL_COND(new_base_frame >= get_current_frame());
	int64_t animation_frame = get_base_frame();
	AnimationTree *tree = get_animation_tree();
	if (tree) {
		_restore_animation_snapshot();
	}

	while (!_history.is_empty()) {
		Ref<JigsawSceneCommandHistory> history = _history.front();
		int64_t rollback_frame = history->get_rollback_frame();
		if (rollback_frame > new_base_frame) {
			break;
		}

		if (history->get_command() == JigsawSceneCommandHistory::DESTROY_SCENE) {
			// RIP
			kill_node();
			_history.clear();
			set_base_frame(new_base_frame);
			return;
		}

		if (tree) {
			if (rollback_frame > animation_frame) {
				tree->advance((rollback_frame - animation_frame) * get_time_per_physics_frame());
				animation_frame = rollback_frame;
			}

			_apply_animation_command(history, tree);
		}

		_history.pop_front();
	}

	if (tree) {
		if (animation_frame < new_base_frame) {
			tree->advance((new_base_frame - animation_frame) * get_time_per_physics_frame());
			animation_frame = new_base_frame;
		}

		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawSceneCommandHistory> history = _history[i];
			int64_t rollback_frame = history->get_rollback_frame();

			if (rollback_frame > animation_frame) {
				tree->advance((rollback_frame - animation_frame) * get_time_per_physics_frame());
				animation_frame = rollback_frame;
			}

			_apply_animation_command(history, tree);
		}

		if (get_current_frame() > animation_frame) {
			tree->advance((get_current_frame() - animation_frame) * get_time_per_physics_frame());
		}
	}

	set_base_frame(new_base_frame);
}
void JigsawSceneManipulator::rollback_to_frame(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(get_base_frame() == -1);
	ERR_FAIL_COND(frame < get_base_frame());
	ERR_FAIL_COND(frame > get_current_frame());

	while (!_history.is_empty()) {
		Ref<JigsawSceneCommandHistory> history = _history.back();
		if (history->get_rollback_frame() <= frame) {
			break;
		}

		_apply_command(history, true);
		_history.pop_back();
	}

	AnimationTree *tree = get_animation_tree();
	if (tree) {
		_restore_animation_snapshot();

		int64_t animation_frame = get_base_frame();
		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawSceneCommandHistory> history = _history[i];
			int64_t rollback_frame = history->get_rollback_frame();
			if (rollback_frame > animation_frame) {
				tree->advance((rollback_frame - animation_frame) * get_time_per_physics_frame());
				animation_frame = rollback_frame;
			}

			_apply_animation_command(history, tree);
		}

		if (animation_frame < frame) {
			tree->advance((frame - animation_frame) * get_time_per_physics_frame());
		}
	}
}
void JigsawSceneManipulator::advance_frames(int64_t frames) {
	AnimationTree *tree = get_animation_tree();
	if (tree) {
		tree->advance(frames * get_time_per_physics_frame());
	}
}

void JigsawSceneManipulator::destroy(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSceneCommandHistory>(_history.back())->get_rollback_frame() > frame);

	Node *root = get_root_rollback();
	ERR_FAIL_NULL(root);
	Node3D *root3d = Object::cast_to<Node3D>(root);
	ERR_FAIL_NULL(root3d);

	if (frame == -1) {
		kill_node();
		return;
	}

	if (!root3d->is_visible()) {
		Ref<JigsawSceneCommandHistory> make_visible;
		make_visible.instantiate();
		make_visible->set_rollback_frame(frame);
		make_visible->set_node_path(".");
		make_visible->set_command(JigsawSceneCommandHistory::TOGGLE_VISIBLE);

		_apply_command(make_visible);
		_history.append(make_visible);
	}

	Ref<JigsawSceneCommandHistory> future_destroy;
	future_destroy.instantiate();
	future_destroy->set_rollback_frame(frame);
	future_destroy->set_node_path(".");
	future_destroy->set_command(JigsawSceneCommandHistory::DESTROY_SCENE);

	_apply_command(future_destroy);
	_history.append(future_destroy);
}

void JigsawSceneManipulator::set_visible(const NodePath &path, bool visible, int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSceneCommandHistory>(_history.back())->get_rollback_frame() > frame);

	Node *root = get_root_rollback();
	ERR_FAIL_NULL(root);

	Node3D *node = Object::cast_to<Node3D>(root->get_node_or_null(path));
	if (unlikely(!node)) {
		print_verbose(vformat("No such node in scene %s: %s", get_scene_id(), path));
		return;
	}

	if (node->is_visible() == visible) {
		// setting visible to current value; nothing needs to be done
		return;
	}

	Ref<JigsawSceneCommandHistory> toggle;
	toggle.instantiate();
	toggle->set_rollback_frame(frame);
	toggle->set_node_path(path);
	toggle->set_command(JigsawSceneCommandHistory::TOGGLE_VISIBLE);

	_apply_command(toggle);
	if (frame != -1) {
		_history.append(toggle);
	}
}

void JigsawSceneManipulator::_apply_command(const Ref<JigsawSceneCommandHistory> &history, bool reverse) const {
	Node *root = get_root();
	ERR_FAIL_NULL(root);
	Node3D *node = root->get_node<Node3D>(history->get_node_path());
	ERR_FAIL_NULL(node);
	ERR_FAIL_COND(!root->is_ancestor_of(node));

	switch (history->get_command()) {
	case JigsawSceneCommandHistory::DESTROY_SCENE:
		ERR_FAIL_COND(node != root);
		node->set_visible(reverse);
		break;
	case JigsawSceneCommandHistory::TOGGLE_VISIBLE:
		node->set_visible(!node->is_visible()); // toggle is the same forwards and backwards
		break;
	}
}
void JigsawSceneManipulator::_apply_animation_command(const Ref<JigsawSceneCommandHistory> &history, AnimationTree *tree) const {
	switch (history->get_command()) {
	case JigsawSceneCommandHistory::DESTROY_SCENE:
	case JigsawSceneCommandHistory::TOGGLE_VISIBLE:
		break;
	}
}

void JigsawSceneManipulator::_take_animation_snapshot() {
	AnimationTree *tree = get_animation_tree();
	ERR_FAIL_NULL(tree);

	CRASH_NOW_MSG("TODO: take animation snapshot"); // TODO
}
void JigsawSceneManipulator::_restore_animation_snapshot() {
	AnimationTree *tree = get_animation_tree();
	ERR_FAIL_NULL(tree);

	CRASH_NOW_MSG("TODO: restore animation snapshot"); // TODO
}
