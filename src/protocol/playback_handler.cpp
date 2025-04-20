#include "protocol/playback_handler.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"
#include "active/audience.h"

static LazyGlobalFile<GDScript> AUDIENCE_MESH{"res://stage/audience_mesh.gd"};

void PlaybackHandler::_bind_methods() {
	BIND_PROPERTY_RESOURCE(DataContainer, recording);
	BIND_PROPERTY_RESOURCE(JigsawGlobal, global);

	ClassDB::bind_method(D_METHOD("init_playback"), &PlaybackHandler::init_playback);
}

IMPLEMENT_PROPERTY_SIMPLE(PlaybackHandler, Ref<DataContainer>, recording);
IMPLEMENT_PROPERTY_SIMPLE(PlaybackHandler, JigsawGlobal *, global);

static bool _sound_history_is_seek_play_helper(const Ref<JigsawSoundCommandHistory> &history) {
	return history->get_command() == JigsawSoundCommandHistory::PLAY && !Math::is_zero_approx(history->get_from_value());
}

void PlaybackHandler::init_playback() {
	ERR_FAIL_COND(_recording.is_null());

	JigsawGlobal *prev_global = _global;

	_global = memnew(JigsawGlobal);
	_global->set_input_source(this);

	Ref<GameMode> mode = _recording->get_mode();
	_global->set_mode(mode);
	TypedArray<VariantDef> variants = mode->get_variants();
	_global->set_selected_variant(variants[_recording->get_selected_variant()]);
	add_child(_global, true);

	JigsawVisual *visual = memnew(JigsawVisual);
	visual->set_simple_background(mode->get_icon_texture(mode->get_default_simple_background()));
	add_child(visual, true);
	_global->set_visual(visual);

	Ref<JigsawState> state;
	state.instantiate();
	_global->set_state(state);

	Ref<Audience> audience;
	audience.instantiate();
	audience->set_meshes(AUDIENCE_MESH->call("create_multi_meshes", _recording->get_mode()));
	state->set_audience(audience);
	visual->set_audience(audience);

	_global->init_sides();
	_global->run_mode_init(_recording->get_timestamp(), _recording->get_shared_seed());

	// TODO: init NPCs

	TypedArray<RecordingPlayerData> player_data = _recording->get_player_data();
	for (int64_t i = 0; i < player_data.size(); i++) {
		Ref<RecordingPlayerData> player_datum = player_data[i];

		_global->run_mode_trigger(JigsawTriggerVariant::CHARACTER_INIT, Array::make(
			JigsawParameterAmount::make(i + 1),
			JigsawParameterString::make(player_datum->get_display_name()),
			JigsawParameterCharacter::make(player_datum->get_character())
		), Ref<RNG>());

		_global->run_mode_trigger(JigsawTriggerVariant::DECK_INIT, Array::make(
			JigsawParameterAmount::make(i + 1),
			JigsawParameterOrderedList::make(Array(player_datum->get_initial_deck()).map(callable_mp_static(&JigsawParameterCard::make)))
		), Ref<RNG>());
	}

	if (prev_global) {
		TypedArray<JigsawSound> prev_sounds = prev_global->get_sounds();
		TypedArray<JigsawSound> new_sounds = _global->get_sounds();
		for (int64_t i = 0; i < prev_sounds.size(); i++) {
			Ref<JigsawSound> prev_sound = prev_sounds[i];
			if (prev_sound.is_null()) {
				continue;
			}
			Node *prev_sound_node = prev_sound->get_node();
			if (!prev_sound_node || !prev_sound_node->call("is_playing").booleanize()) {
				continue;
			}

			Ref<JigsawParameterAudio> prev_track = prev_sound->get_track();
			if (prev_track.is_null() || prev_track->get_audio().is_null() || prev_track->get_audio()->get_loop_mode() == AudioStreamWAV::LOOP_DISABLED) {
				continue;
			}

			// assume that any looping sound that was playing at the end of the previous recording will
			// make the most sense and be the least jarring if we seek the new sound to where it was
			for (int64_t j = 0; j < new_sounds.size(); j++) {
				Ref<JigsawSound> new_sound = new_sounds[j];
				if (new_sound.is_null()) {
					continue;
				}

				if (prev_sound->get_space() != new_sound->get_space() ||
					prev_sound->get_type() != new_sound->get_type()) {
					continue;
				}

				Ref<JigsawParameterAudio> new_track = new_sound->get_track();
				if (new_track.is_null() || new_track->get_type() != prev_track->get_type() ||
					new_track->get_loop_start() != prev_track->get_loop_start() ||
					new_track->get_loop_end() != prev_track->get_loop_end()) {
					continue;
				}

				if (likely(new_track->get_type() == JigsawParameter::FILE_ID_OPUS)) {
					Ref<JigsawParameterFileIDOpus> new_file_id = new_track;
					Ref<JigsawParameterFileIDOpus> prev_file_id = prev_track;
					if (new_file_id->get_file_id() != prev_file_id->get_file_id()) {
						continue;
					}
				} else if (likely(new_track->get_type() == JigsawParameter::CID_OPUS)) {
					Ref<JigsawParameterCIDOpus> new_cid = new_track;
					Ref<JigsawParameterCIDOpus> prev_cid = prev_track;
					if (new_cid->get_cid() != prev_cid->get_cid()) {
						continue;
					}
				} else {
					ERR_PRINT_ONCE(vformat("unhandled looping audio parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", new_track->get_type())));
					continue;
				}

				Node *new_sound_node = new_sound->get_node();
				if (!new_sound_node || !new_sound_node->call("is_playing").booleanize()) {
					continue;
				}

				// only do this if we're at the start of the track (assume there are no rollback shenanigans)
				if (new_sound->get_history().any(callable_mp_static(&_sound_history_is_seek_play_helper))) {
					continue;
				}

				// also require them to be in the same physical place if applicable
				switch (new_sound->get_space()) {
				case JigsawSound::GLOBAL:
					break;
				case JigsawSound::STAGE_3D:
				case JigsawSound::HUD_3D:
					if (!new_sound_node->get("position").operator Vector3().is_equal_approx(prev_sound_node->get("position"))) {
						continue;
					}

					break;
				case JigsawSound::STAGE_2D:
				case JigsawSound::HUD_2D:
					if (!new_sound_node->get("position").operator Vector2().is_equal_approx(prev_sound_node->get("position"))) {
						continue;
					}

					break;
				}

				new_sound_node->call("seek", prev_sound_node->call("get_playback_position"));
			}
		}

		JigsawVisual *prev_visual = prev_global->get_visual();
		if (prev_visual) {
			visual->set_camera_time(prev_visual->get_camera_time());
			visual->force_camera_update();

			remove_child(prev_visual);
			prev_visual->queue_free();
		}

		remove_child(prev_global);
		prev_global->queue_free();
	}
}

BitField<ButtonInputHistory::InputButton> PlaybackHandler::get_player_realtime_inputs(int32_t side, int64_t frame) const {
	ERR_FAIL_V_MSG(0, "TODO: PlaybackHandler::get_player_realtime_inputs"); // TODO
}

void PlaybackHandler::on_jigsaw_error(const Ref<JigsawError> &err) {
	ERR_FAIL_MSG("TODO: PlaybackHandler::on_jigsaw_error"); // TODO
}
