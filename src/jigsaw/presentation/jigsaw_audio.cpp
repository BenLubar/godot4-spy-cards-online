#include "jigsaw/presentation/jigsaw_audio.h"

#include "jigsaw/jigsaw_visual.h"

#include <godot_cpp/classes/audio_stream_wav.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

void JigsawSoundCommandHistory::_bind_methods() {
	BIND_ENUM_CONSTANT(PLAY);
	BIND_ENUM_CONSTANT(POSITION);
	BIND_ENUM_CONSTANT(VOLUME);
	BIND_ENUM_CONSTANT(PITCH);
	BIND_ENUM_CONSTANT(DESTROY);

	BIND_PROPERTY(Variant::INT, rollback_frame);
	BIND_PROPERTY_ENUM(JigsawSoundCommandHistory::Command, command);
	BIND_PROPERTY(Variant::FLOAT, from_value);
	BIND_PROPERTY(Variant::FLOAT, to_value);
	BIND_PROPERTY(Variant::VECTOR3, from_pos);
	BIND_PROPERTY(Variant::VECTOR3, to_pos);
	BIND_PROPERTY_IS(Variant::BOOL, paused);
	BIND_PROPERTY(Variant::FLOAT, duration);

	ClassDB::bind_method(D_METHOD("get_duration_in_frames"), &JigsawSoundCommandHistory::get_duration_in_frames);
}

IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, int64_t, rollback_frame);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, JigsawSoundCommandHistory::Command, command);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, float, from_value);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, float, to_value);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, Vector3, from_pos);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, Vector3, to_pos);
IMPLEMENT_PROPERTY_IS(JigsawSoundCommandHistory, bool, paused);
IMPLEMENT_PROPERTY(JigsawSoundCommandHistory, float, duration);

int JigsawSoundCommandHistory::get_duration_in_frames() const {
	return Math::fast_ftoi(get_duration() * Engine::get_singleton()->get_physics_ticks_per_second());
}

void JigsawSound::_bind_methods() {
	BIND_ENUM_CONSTANT(GLOBAL);
	BIND_ENUM_CONSTANT(STAGE_3D);
	BIND_ENUM_CONSTANT(HUD_3D);
	BIND_ENUM_CONSTANT(STAGE_2D);
	BIND_ENUM_CONSTANT(HUD_2D);

	BIND_ENUM_CONSTANT(OTHER);
	BIND_ENUM_CONSTANT(CARD_SOUND);
	BIND_ENUM_CONSTANT(MUSIC);

	BIND_PROPERTY_ENUM(JigsawSound::Space, space);
	BIND_PROPERTY_ENUM(JigsawSound::Type, type);
	BIND_PROPERTY_RESOURCE(JigsawParameterAudio, track);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSoundCommandHistory, history);
	BIND_PROPERTY_OBJECTID_NOT_SAVED(Node, node);
	BIND_PROPERTY_IS(Variant::BOOL, destroyed);

	ClassDB::bind_method(D_METHOD("init_node", "visual"), &JigsawSound::init_node);

	ClassDB::bind_method(D_METHOD("play", "seek", "frame"), &JigsawSound::play, DEFVAL(0.0f), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("pause", "frame"), &JigsawSound::pause, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("resume", "frame"), &JigsawSound::resume, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("destroy", "frame"), &JigsawSound::destroy, DEFVAL(-1));

	ClassDB::bind_method(D_METHOD("set_position", "position", "duration", "frame"), &JigsawSound::set_position, DEFVAL(0.0f), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("set_volume", "volume", "duration", "frame"), &JigsawSound::set_volume, DEFVAL(0.0f), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("set_pitch", "pitch", "duration", "frame"), &JigsawSound::set_pitch, DEFVAL(0.0f), DEFVAL(-1));
}

IMPLEMENT_PROPERTY(JigsawSound, JigsawSound::Space, space);
IMPLEMENT_PROPERTY(JigsawSound, JigsawSound::Type, type);
IMPLEMENT_PROPERTY(JigsawSound, Ref<JigsawParameterAudio>, track);
IMPLEMENT_PROPERTY(JigsawSound, TypedArray<JigsawSoundCommandHistory>, history);
IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(JigsawSound, Node, node);
IMPLEMENT_PROPERTY_IS(JigsawSound, bool, destroyed);

static StringName get_bus_name(JigsawSound::Type type) {
	switch (type) {
	case JigsawSound::MUSIC:
		return "Music";
	case JigsawSound::CARD_SOUND:
		return "Card Effects";
	case JigsawSound::OTHER:
		return "Other Game Sounds";
	}

	ERR_FAIL_V("Other Game Sounds");
}
void JigsawSound::init_node(JigsawVisual *visual) {
	ERR_FAIL_NULL(visual);
	ERR_FAIL_COND_MSG(_track.is_null(), "JigsawSound must have space, type, and track set before use!");
	ERR_FAIL_COND(get_node());

	switch (_space) {
	case GLOBAL:
	{
		JigsawAudio *audio = memnew(JigsawAudio);

		set_node(audio);
		audio->set_bus(get_bus_name(_type));
		audio->set_sound(this);
		audio->set_stream(_track->get_audio());
		visual->add_child(audio);

		break;
	}
	case STAGE_3D:
	{
		if (visual->get_force_simple_background()) {
			break;
		}

		JigsawAudio3D *audio = memnew(JigsawAudio3D);

		set_node(audio);
		audio->set_bus(get_bus_name(_type));
		audio->set_sound(this);
		audio->set_stream(_track->get_audio());
		visual->get_stage_viewport()->add_child(audio);

		break;
	}
	case HUD_3D:
	{
		JigsawAudio3D *audio = memnew(JigsawAudio3D);

		set_node(audio);
		audio->set_bus(get_bus_name(_type));
		audio->set_sound(this);
		audio->set_stream(_track->get_audio());
		visual->add_child(audio);

		break;
	}
	case STAGE_2D:
	{
		if (visual->get_force_simple_background()) {
			break;
		}

		JigsawAudio2D *audio = memnew(JigsawAudio2D);

		set_node(audio);
		audio->set_bus(get_bus_name(_type));
		audio->set_sound(this);
		audio->set_stream(_track->get_audio());
		visual->get_stage_viewport()->add_child(audio);

		break;
	}
	case HUD_2D:
	{
		JigsawAudio2D *audio = memnew(JigsawAudio2D);

		set_node(audio);
		audio->set_bus(get_bus_name(_type));
		audio->set_sound(this);
		audio->set_stream(_track->get_audio());
		visual->add_child(audio);

		break;
	}
	}
}
void JigsawSound::kill_node() {
	Node *node = get_node();
	if (node) {
		node->call("stop");
		node->queue_free();
		set_node(nullptr);
	}
}
void JigsawSound::discard_rollback_data(int64_t new_base_frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(new_base_frame < -1);

	// special case: moving out of rollback
	if (new_base_frame == -1) {
		ERR_FAIL_COND(get_base_frame() == -1); // we weren't in rollback

		int64_t last_play_entry = -1;
		int64_t last_position_entry = -1;
		int64_t last_volume_entry = -1;
		int64_t last_pitch_entry = -1;
		for (int64_t i = 0; i < _history.size(); i++) {
			Ref<JigsawSoundCommandHistory> history = _history[i];
			history->set_rollback_frame(-1);

			switch (history->get_command()) {
			case JigsawSoundCommandHistory::PLAY:
				if (last_play_entry != -1) {
					_history.remove_at(last_play_entry);
					i--;
				}
				last_play_entry = i;
				break;
			case JigsawSoundCommandHistory::POSITION:
				if (last_position_entry != -1) {
					_history.remove_at(last_position_entry);
					i--;
				}
				last_position_entry = i;
				break;
			case JigsawSoundCommandHistory::VOLUME:
				if (last_volume_entry != -1) {
					_history.remove_at(last_volume_entry);
					i--;
				}
				last_volume_entry = i;
				break;
			case JigsawSoundCommandHistory::PITCH:
				if (last_pitch_entry != -1) {
					_history.remove_at(last_pitch_entry);
					i--;
				}
				last_pitch_entry = i;
				break;
			case JigsawSoundCommandHistory::DESTROY:
				// we can't have more than one destroy entry.
				// however, exiting rollback means we need to apply it for real.
				kill_node();
				break;
			}
		}

		set_base_frame(-1);
		set_current_frame(-1);

		return;
	}

	// special case: moving into rollback
	if (get_base_frame() == -1) {
		ERR_FAIL_COND_MSG(new_base_frame != 0, "JigsawSound::discard_rollback_data did not properly enter rollback mode");

		Node *audio = get_node();
		if (!audio) {
			// we're not actually playing (due to preferences or whatever) so we don't need to have the correct data; just set everything to frame 0 to keep the invariant.

			for (int64_t i = 0; i < _history.size(); i++) {
				Ref<JigsawSoundCommandHistory> history = _history[i];
				history->set_rollback_frame(0);
			}
		} else {
			float current_seek = 0.0f;
			bool currently_paused = false;
			Vector3 current_position;
			float current_volume = 0.0f;
			float current_pitch = 1.0f;

			if (_space == GLOBAL) {
				JigsawAudio *a = Object::cast_to<JigsawAudio>(audio);
				current_seek = a->get_playback_position();
				currently_paused = a->get_stream_paused();
				current_volume = a->get_volume_db();
				current_pitch = a->get_pitch_scale();
			} else if (_space == STAGE_3D || _space == HUD_3D) {
				JigsawAudio3D *a = Object::cast_to<JigsawAudio3D>(audio);
				current_seek = a->get_playback_position();
				currently_paused = a->get_stream_paused();
				current_position = a->get_position();
				current_volume = a->get_volume_db();
				current_pitch = a->get_pitch_scale();
			} else if (_space == STAGE_2D || _space == HUD_2D) {
				JigsawAudio2D *a = Object::cast_to<JigsawAudio2D>(audio);
				current_seek = a->get_playback_position();
				currently_paused = a->get_stream_paused();
				Vector2 pos = a->get_position();
				current_position = Vector3(pos.x, pos.y, 0.0f);
				current_volume = a->get_volume_db();
				current_pitch = a->get_pitch_scale();
			}

			// we should have at most one copy of each command, so don't bother checking for duplicates.
			for (int64_t i = 0; i < _history.size(); i++) {
				Ref<JigsawSoundCommandHistory> history = _history[i];
				history->set_rollback_frame(0);

				switch (history->get_command()) {
				case JigsawSoundCommandHistory::PLAY:
					history->set_from_value(current_seek);
					history->set_paused(currently_paused);
					break;
				case JigsawSoundCommandHistory::POSITION:
					if (Math::is_zero_approx(history->get_duration())) {
						history->set_from_pos(current_position);
					} else {
						float remaining = Math::clamp(current_position.distance_to(history->get_to_pos()) / history->get_from_pos().distance_to(history->get_to_pos()), 0.0f, 1.0f);
						history->set_from_pos(current_position);
						history->set_duration(remaining * history->get_duration());
					}
					break;
				case JigsawSoundCommandHistory::VOLUME:
					if (Math::is_zero_approx(history->get_duration())) {
						history->set_from_value(current_volume);
					} else {
						float remaining = Math::clamp((current_volume - history->get_to_value()) / (history->get_from_value() - history->get_to_value()), 0.0f, 1.0f);
						history->set_from_value(current_volume);
						history->set_duration(remaining * history->get_duration());
					}
					break;
				case JigsawSoundCommandHistory::PITCH:
					if (Math::is_zero_approx(history->get_duration())) {
						history->set_from_value(current_pitch);
					} else {
						float remaining = Math::clamp((current_pitch - history->get_to_value()) / (history->get_from_value() - history->get_to_value()), 0.0f, 1.0f);
						history->set_from_value(current_pitch);
						history->set_duration(remaining * history->get_duration());
					}
					break;
				case JigsawSoundCommandHistory::DESTROY:
					// don't need to fix anything up.
					break;
				}
			}
		}

		set_base_frame(0);
		set_current_frame(0);

		return;
	}

	ERR_FAIL_COND(new_base_frame >= get_current_frame());

	int64_t last_play_entry = -1;
	int64_t last_position_entry = -1;
	int64_t last_volume_entry = -1;
	int64_t last_pitch_entry = -1;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> history = _history[i];
		if (history->get_rollback_frame() >= new_base_frame) {
			break;
		}

		switch (history->get_command()) {
		case JigsawSoundCommandHistory::PLAY:
			if (last_play_entry != -1) {
				_history.remove_at(last_play_entry);
				i--;
			}
			last_play_entry = i;
			break;
		case JigsawSoundCommandHistory::POSITION:
			if (last_position_entry != -1) {
				_history.remove_at(last_position_entry);
				i--;
			}
			last_position_entry = i;
			break;
		case JigsawSoundCommandHistory::VOLUME:
			if (last_volume_entry != -1) {
				_history.remove_at(last_volume_entry);
				i--;
			}
			last_volume_entry = i;
			break;
		case JigsawSoundCommandHistory::PITCH:
			if (last_pitch_entry != -1) {
				_history.remove_at(last_pitch_entry);
				i--;
			}
			last_pitch_entry = i;
			break;
		case JigsawSoundCommandHistory::DESTROY:
			kill_node();
			break;
		}
	}

	set_base_frame(new_base_frame);
}

static bool audio_loop_modulo(const Ref<AudioStreamWAV> &track, float &seek) {
	ERR_FAIL_COND_V(track->get_format() != AudioStreamWAV::FORMAT_8_BITS && track->get_format() != AudioStreamWAV::FORMAT_16_BITS, false);
	ERR_FAIL_COND_V(track->get_loop_mode() != AudioStreamWAV::LOOP_DISABLED && track->get_loop_mode() != AudioStreamWAV::LOOP_FORWARD, false);

	int64_t total_samples = track->get_data().size();
	if (track->get_format() == AudioStreamWAV::FORMAT_16_BITS) {
		total_samples /= 2;
	}
	if (track->is_stereo()) {
		total_samples /= 2;
	}

	float mix_rate_float = track->get_mix_rate();
	int64_t seek_samples = Math::fast_ftoi(seek * mix_rate_float);
	if (seek_samples < 0) {
		return false;
	}

	if (seek_samples <= (track->get_loop_mode() == AudioStreamWAV::LOOP_FORWARD ? track->get_loop_end() : total_samples - 1)) {
		return true;
	}

	if (track->get_loop_mode() == AudioStreamWAV::LOOP_DISABLED) {
		return false;
	}

	int64_t start_loop = track->get_loop_begin();
	int64_t loop_length = track->get_loop_end() - start_loop + 1;
	ERR_FAIL_COND_V(loop_length <= 0, false);
	seek_samples = ((seek_samples - start_loop) % loop_length) + start_loop;

	seek = float(seek_samples) / mix_rate_float;

	return true;
}

void JigsawSound::rollback_to_frame(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND(get_base_frame() == -1);
	ERR_FAIL_COND(frame < get_base_frame());
	ERR_FAIL_COND(frame > get_current_frame());

	// we're not moving in time; we're only undoing changes that happened "in the future" of this past frame.
	int64_t last_play_entry = -1;
	int64_t last_position_entry = -1;
	int64_t last_volume_entry = -1;
	int64_t last_pitch_entry = -1;
	bool need_to_revert_play = false;
	bool need_to_revert_position = false;
	bool need_to_revert_volume = false;
	bool need_to_revert_pitch = false;
	bool need_to_revert_destroy = false;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> history = _history[i];
		if (history->get_rollback_frame() > frame) {
			switch (history->get_command()) {
			case JigsawSoundCommandHistory::PLAY:
				need_to_revert_play = true;
				break;
			case JigsawSoundCommandHistory::POSITION:
				need_to_revert_position = true;
				break;
			case JigsawSoundCommandHistory::VOLUME:
				need_to_revert_volume = true;
				break;
			case JigsawSoundCommandHistory::PITCH:
				need_to_revert_pitch = true;
				break;
			case JigsawSoundCommandHistory::DESTROY:
				need_to_revert_destroy = true;
				set_destroyed(false);
				break;
			}
			_history.remove_at(i);
			i--;
		} else {
			switch (history->get_command()) {
			case JigsawSoundCommandHistory::PLAY:
				last_play_entry = i;
				break;
			case JigsawSoundCommandHistory::POSITION:
				last_position_entry = i;
				break;
			case JigsawSoundCommandHistory::VOLUME:
				last_volume_entry = i;
				break;
			case JigsawSoundCommandHistory::PITCH:
				last_pitch_entry = i;
				break;
			case JigsawSoundCommandHistory::DESTROY:
				break;
			}
		}
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	if (need_to_revert_play || need_to_revert_destroy) {
		audio->call("stop");
		audio->call("set_stream_paused", false);
		if (last_play_entry != -1) {
			Ref<JigsawSoundCommandHistory> last_play = _history[last_play_entry];
			float seek = last_play->get_from_value() + float(get_current_frame() - last_play->get_rollback_frame()) / float(Engine::get_singleton()->get_physics_ticks_per_second());
			if (!last_play->is_paused() && audio_loop_modulo(get_track()->get_audio(), seek)) {
				audio->call("play", seek);
			}
		}
	}

	if (need_to_revert_position) {
		JigsawAudio2D *a2d = Object::cast_to<JigsawAudio2D>(audio);
		JigsawAudio3D *a3d = Object::cast_to<JigsawAudio3D>(audio);
		Ref<Tween> tween = a2d ? a2d->get_position_tween() : a3d->get_position_tween();
		if (tween.is_valid()) {
			tween->kill();
			tween = Ref<Tween>();
			if (a2d) {
				a2d->set_position_tween(tween);
			} else {
				a3d->set_position_tween(tween);
			}
		}
		if (last_position_entry == -1) {
			if (a2d) {
				a2d->set_position(Vector2());
			} else {
				a3d->set_position(Vector3());
			}
		} else {
			Ref<JigsawSoundCommandHistory> last_position = _history[last_position_entry];
			int64_t start_frame = last_position->get_rollback_frame();
			int64_t frame_length = last_position->get_duration_in_frames();
			Vector3 to = last_position->get_to_pos();
			if (start_frame + frame_length <= get_current_frame()) {
				if (a2d) {
					a2d->set_position(Vector2(to.x, to.y));
				} else {
					a3d->set_position(to);
				}
			} else {
				float progress = float(get_current_frame() - start_frame) / float(frame_length);
				Vector3 from = last_position->get_from_pos().lerp(to, progress);

				tween = audio->create_tween();
				tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);

				if (a2d) {
					a2d->set_position(Vector2(from.x, from.y));
					a2d->set_position_tween(tween);
					tween->tween_property(a2d, "position", Vector2(to.x, to.y), last_position->get_duration() * (1.0f - progress))->set_trans(Tween::TRANS_LINEAR);
				} else {
					a3d->set_position(from);
					a3d->set_position_tween(tween);
					tween->tween_property(a3d, "position", to, last_position->get_duration() * (1.0f - progress))->set_trans(Tween::TRANS_LINEAR);
				}
			}
		}
	}

	if (need_to_revert_volume) {
		Ref<Tween> tween = audio->get("volume_tween");
		if (tween.is_valid()) {
			tween->kill();
			tween = Ref<Tween>();
			audio->set("volume_tween", tween);
		}
		if (last_volume_entry == -1) {
			audio->set("volume_linear", 1.0f);
		} else {
			Ref<JigsawSoundCommandHistory> last_volume = _history[last_volume_entry];
			int64_t start_frame = last_volume->get_rollback_frame();
			int64_t frame_length = last_volume->get_duration_in_frames();
			float to = last_volume->get_to_value();
			if (start_frame + frame_length <= get_current_frame()) {
				audio->set("volume_linear", to);
			} else {
				float progress = float(get_current_frame() - start_frame) / float(frame_length);
				float from = Math::lerp(last_volume->get_from_value(), to, progress);

				tween = audio->create_tween();
				audio->set("volume_tween", tween);
				tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
				tween->tween_property(audio, "volume_linear", to, last_volume->get_duration() * (1.0f - progress))->set_trans(Tween::TRANS_LINEAR);
			}
		}
	}

	if (need_to_revert_pitch) {
		Ref<Tween> tween = audio->get("pitch_tween");
		if (tween.is_valid()) {
			tween->kill();
			tween = Ref<Tween>();
			audio->set("pitch_tween", tween);
		}
		if (last_pitch_entry == -1) {
			audio->set("pitch_scale", 1.0f);
		} else {
			Ref<JigsawSoundCommandHistory> last_pitch = _history[last_pitch_entry];
			int64_t start_frame = last_pitch->get_rollback_frame();
			int64_t frame_length = last_pitch->get_duration_in_frames();
			float to = last_pitch->get_to_value();
			if (start_frame + frame_length <= get_current_frame()) {
				audio->set("pitch_scale", to);
			} else {
				float progress = float(get_current_frame() - start_frame) / float(frame_length);
				float from = Math::lerp(last_pitch->get_from_value(), to, progress);

				tween = audio->create_tween();
				audio->set("pitch_tween", tween);
				tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
				tween->tween_property(audio, "pitch_scale", to, last_pitch->get_duration() * (1.0f - progress))->set_trans(Tween::TRANS_LINEAR);
			}
		}
	}
}

void JigsawSound::advance_frames(int64_t frames) {
	// JigsawSound handles time travel at command time, so no fast forward is needed.
}

void JigsawSound::play(float seek, int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::PLAY && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::PLAY);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	history->set_from_value(seek);
	history->set_paused(false);

	// if we're playing in the past, adjust the seek time
	if (frame != get_current_frame()) {
		seek += float(get_current_frame() - frame) / float(Engine::get_singleton()->get_physics_ticks_per_second());
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	audio->call("stop");
	if (audio_loop_modulo(get_track()->get_audio(), seek)) {
		audio->call("play", seek);
	}
	audio->call("set_stream_paused", false);
}
void JigsawSound::pause(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> previous;
	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::PLAY && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_command() == JigsawSoundCommandHistory::PLAY && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::PLAY);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	history->set_paused(true);
	if (previous.is_null()) {
		// we don't have a previous value, which means the audio hasn't been played yet.
		history->set_from_value(0.0f);
	} else if (previous->is_paused()) {
		// we were paused, and we still are
		history->set_from_value(previous->get_from_value());
	} else if (frame != -1) {
		// we weren't paused; add the number of frames we advanced
		// if we're not in rollback, we'll grab the time next
		history->set_from_value(previous->get_from_value() + float(frame - previous->get_rollback_frame()) / float(Engine::get_singleton()->get_physics_ticks_per_second()));
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	audio->call("set_stream_paused", true);
	if (frame == -1) {
		history->set_from_value(audio->call("get_playback_position"));
	}
}
void JigsawSound::resume(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> previous;
	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::PLAY && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_command() == JigsawSoundCommandHistory::PLAY && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::PLAY);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	history->set_paused(false);
	if (previous.is_null()) {
		// we don't have a previous value, which means the audio hasn't been played yet.
		history->set_from_value(0.0f);
	} else if (previous->is_paused()) {
		// we were paused, and we just unpaused
		history->set_from_value(previous->get_from_value());
	} else if (frame != -1) {
		// we weren't paused; add the number of frames we advanced
		// if we're not in rollback, we'll grab the time next
		history->set_from_value(previous->get_from_value() + float(frame - previous->get_rollback_frame()) / float(Engine::get_singleton()->get_physics_ticks_per_second()));
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	if (frame == -1) {
		history->set_from_value(audio->call("get_playback_position"));
	} else {
		audio->call("stop");
		float seek = history->get_from_value();
		if (audio_loop_modulo(get_track()->get_audio(), seek)) {
			audio->call("play", seek);
		}
	}
	audio->call("set_stream_paused", false);
}
void JigsawSound::destroy(int64_t frame) {
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> history;
	history.instantiate();
	history->set_command(JigsawSoundCommandHistory::DESTROY);
	history->set_rollback_frame(frame);
	_history.append(history);

	set_destroyed(true);

	if (frame == -1) {
		kill_node();
	} else {
		Node *audio = get_node();
		if (audio) {
			audio->call("stop");
		}
	}
}

void JigsawSound::set_position(Vector3 position, float duration, int64_t frame) {
	ERR_FAIL_COND(get_space() == GLOBAL);
	ERR_FAIL_COND((get_space() == STAGE_2D || get_space() == HUD_2D) && !Math::is_zero_approx(position.z));
	ERR_FAIL_COND(!position.is_finite());
	ERR_FAIL_COND(duration < 0.0f);
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> previous;
	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::POSITION && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_command() == JigsawSoundCommandHistory::POSITION && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::POSITION);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	Node *audio = get_node();
	JigsawAudio2D *a2d = Object::cast_to<JigsawAudio2D>(audio);
	JigsawAudio3D *a3d = Object::cast_to<JigsawAudio3D>(audio);
	if (!a2d && !a3d) {
		return;
	}

	// kill the old tween, if any
	if (a2d) {
		Ref<Tween> old_tween = a2d->get_position_tween();
		if (old_tween.is_valid()) {
			old_tween->kill();
			a2d->set_position_tween(Ref<Tween>());
		}
	} else {
		Ref<Tween> old_tween = a3d->get_position_tween();
		if (old_tween.is_valid()) {
			old_tween->kill();
			a3d->set_position_tween(Ref<Tween>());
		}
	}

	Vector3 start_position;
	if (frame == -1) {
		// we're not in rollback mode; the current position of the node is our starting position.
		if (a2d) {
			Vector2 pos = a2d->get_position();
			start_position = Vector3(pos.x, pos.y, 0.0f);
		} else {
			start_position = a3d->get_position();
		}
	} else if (previous.is_valid()) {
		int64_t prev_start = previous->get_rollback_frame();
		int64_t prev_duration = previous->get_duration_in_frames();
		if (prev_start + prev_duration <= frame) {
			// easy case: previous position change ended before we start
			start_position = previous->get_to_pos();
		} else {
			// slightly harder case: interpolate based on remaining frame count
			start_position = previous->get_to_pos().lerp(previous->get_from_pos(), float(frame - prev_start) / float(prev_duration));
		}
	}

	history->set_from_pos(start_position);
	history->set_to_pos(position);
	history->set_duration(duration);

	// how far in the past is this tween being started?
	float skip_duration = Math::min(float(get_current_frame() - frame) / float(Engine::get_singleton()->get_physics_ticks_per_second()), duration);

	// only tween if we're actually animating for a nonzero duration
	Ref<Tween> tween = duration > skip_duration ? (a2d ? a2d->create_tween() : a3d->create_tween())->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS) : Ref<Tween>();
	if (a2d) {
		a2d->set_position_tween(tween);
	} else {
		a3d->set_position_tween(tween);
	}

	if (a2d) {
		if (tween.is_valid()) {
			if (frame != -1) {
				Vector3 from = history->get_from_pos().lerp(history->get_to_pos(), float(get_current_frame() - frame) / float(history->get_duration_in_frames()));
				a2d->set_position(Vector2(from.x, from.y));
			}
			tween->tween_property(a2d, "position", Vector2(position.x, position.y), duration - skip_duration)->set_trans(Tween::TRANS_LINEAR);
		} else {
			a2d->set_position(Vector2(position.x, position.y));
		}
	} else {
		if (tween.is_valid()) {
			if (frame != -1) {
				Vector3 from = history->get_from_pos().lerp(history->get_to_pos(), float(get_current_frame() - frame) / float(history->get_duration_in_frames()));
				a3d->set_position(from);
			}
			tween->tween_property(a3d, "position", position, duration - skip_duration)->set_trans(Tween::TRANS_LINEAR);
		} else {
			a3d->set_position(position);
		}
	}
}
void JigsawSound::set_volume(float volume, float duration, int64_t frame) {
	ERR_FAIL_COND(Math::is_nan(volume));
	ERR_FAIL_COND(volume < 0.0f);
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> previous;
	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::VOLUME && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_command() == JigsawSoundCommandHistory::VOLUME && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::VOLUME);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	// kill the old tween, if any
	Ref<Tween> tween = audio->get("volume_tween");
	if (tween.is_valid()) {
		tween->kill();
		tween = Ref<Tween>();
		audio->set("volume_tween", tween);
	}

	float starting_volume = 1.0f;
	if (frame == -1) {
		// in non-rollback mode, use current volume
		starting_volume = audio->get("volume_linear");
	} else if (previous.is_valid()) {
		int64_t prev_start = previous->get_rollback_frame();
		int64_t prev_duration = previous->get_duration_in_frames();
		if (prev_start + prev_duration <= frame) {
			// easy case: previous volume change ended before we start
			starting_volume = previous->get_to_value();
		} else {
			// slightly harder case: interpolate based on remaining frame count
			starting_volume = Math::lerp(previous->get_to_value(), previous->get_from_value(), float(frame - prev_start) / float(prev_duration));
		}
	}

	history->set_from_value(starting_volume);
	history->set_to_value(volume);
	history->set_duration(duration);

	// how far in the past is this tween being started?
	float skip_duration = Math::min(float(get_current_frame() - frame) / float(Engine::get_singleton()->get_physics_ticks_per_second()), duration);

	// only tween if we're actually animating for a nonzero duration
	if (duration > skip_duration) {
		tween = audio->create_tween();
		tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
		audio->set("volume_tween", tween);

		if (frame != -1) {
			float from = Math::lerp(starting_volume, volume, float(get_current_frame() - frame) / float(history->get_duration_in_frames()));
			audio->set("volume_linear", from);
		}
		tween->tween_property(audio, "volume_linear", volume, duration - skip_duration)->set_trans(Tween::TRANS_LINEAR);
	} else {
		audio->set("volume_linear", volume);
	}
}
void JigsawSound::set_pitch(float pitch, float duration, int64_t frame) {
	ERR_FAIL_COND(Math::is_nan(pitch));
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(get_base_frame() > get_current_frame()); // invariant
	ERR_FAIL_COND((get_base_frame() == -1) != (frame == -1));
	ERR_FAIL_COND(frame > get_current_frame());
	ERR_FAIL_COND(!_history.is_empty() && Object::cast_to<JigsawSoundCommandHistory>(_history.back())->get_rollback_frame() > frame);
	ERR_FAIL_COND(is_destroyed());

	Ref<JigsawSoundCommandHistory> previous;
	Ref<JigsawSoundCommandHistory> history;
	for (int64_t i = 0; i < _history.size(); i++) {
		Ref<JigsawSoundCommandHistory> h = _history[i];
		if (h->get_command() == JigsawSoundCommandHistory::PITCH && h->get_rollback_frame() <= frame) {
			previous = h;
			// keep going
		}
		if (h->get_command() == JigsawSoundCommandHistory::PITCH && h->get_rollback_frame() == frame) {
			history = h;
			break;
		}
	}

	if (history.is_null()) {
		history.instantiate();
		history->set_command(JigsawSoundCommandHistory::PITCH);
		history->set_rollback_frame(frame);
		_history.append(history);
	}

	Node *audio = get_node();
	if (!audio) {
		return;
	}

	// kill the old tween, if any
	Ref<Tween> tween = audio->get("pitch_tween");
	if (tween.is_valid()) {
		tween->kill();
		tween = Ref<Tween>();
		audio->set("pitch_tween", tween);
	}

	float starting_pitch = 0.0f;
	if (frame == -1) {
		// in non-rollback mode, use current pitch
		starting_pitch = audio->get("pitch_scale");
	} else if (previous.is_valid()) {
		int64_t prev_start = previous->get_rollback_frame();
		int64_t prev_duration = previous->get_duration_in_frames();
		if (prev_start + prev_duration <= frame) {
			// easy case: previous pitch change ended before we start
			starting_pitch = previous->get_to_value();
		} else {
			// slightly harder case: interpolate based on remaining frame count
			starting_pitch = Math::lerp(previous->get_to_value(), previous->get_from_value(), float(frame - prev_start) / float(prev_duration));
		}
	}

	history->set_from_value(starting_pitch);
	history->set_to_value(pitch);
	history->set_duration(duration);

	// how far in the past is this tween being started?
	float skip_duration = Math::min(float(get_current_frame() - frame) / float(Engine::get_singleton()->get_physics_ticks_per_second()), duration);

	// only tween if we're actually animating for a nonzero duration
	if (duration > skip_duration) {
		tween = audio->create_tween();
		tween->set_process_mode(Tween::TWEEN_PROCESS_PHYSICS);
		audio->set("pitch_tween", tween);

		if (frame != -1) {
			float from = Math::lerp(starting_pitch, pitch, float(get_current_frame() - frame) / float(history->get_duration_in_frames()));
			audio->set("pitch_scale", from);
		}
		tween->tween_property(audio, "pitch_scale", pitch, duration - skip_duration)->set_trans(Tween::TRANS_LINEAR);
	} else {
		audio->set("pitch_scale", pitch);
	}
}

void JigsawAudio::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawSound, sound);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, volume_tween);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, pitch_tween);
	BIND_PROPERTY_NOT_SAVED(Variant::FLOAT, volume_linear);
}
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio, Ref<JigsawSound>, sound);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio, Ref<Tween>, volume_tween);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio, Ref<Tween>, pitch_tween);

void JigsawAudio2D::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawSound, sound);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, position_tween);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, volume_tween);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, pitch_tween);
	BIND_PROPERTY_NOT_SAVED(Variant::FLOAT, volume_linear);
}
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio2D, Ref<JigsawSound>, sound);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio2D, Ref<Tween>, position_tween);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio2D, Ref<Tween>, volume_tween);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio2D, Ref<Tween>, pitch_tween);

void JigsawAudio3D::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawSound, sound);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, position_tween);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, volume_tween);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Tween, pitch_tween);
	BIND_PROPERTY_NOT_SAVED(Variant::FLOAT, volume_linear);
}
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio3D, Ref<JigsawSound>, sound);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio3D, Ref<Tween>, position_tween);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio3D, Ref<Tween>, volume_tween);
IMPLEMENT_PROPERTY_SIMPLE(JigsawAudio3D, Ref<Tween>, pitch_tween);
