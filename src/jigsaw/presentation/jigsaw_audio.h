#ifndef JIGSAW_AUDIO_H
#define JIGSAW_AUDIO_H

#include "dry.h"

#include "jigsaw/parameter/jigsaw_parameter_audio.h"
#include "jigsaw/presentation/jigsaw_presentation_data.h"

class JigsawVisual;

#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/tween.hpp>

class JigsawSoundCommandHistory : public Resource {
	GDCLASS(JigsawSoundCommandHistory, Resource);

protected:
	static void _bind_methods();

public:
	enum Command {
		PLAY, // no duration; from_value (seek position), paused
		POSITION, // from_pos, to_pos
		VOLUME, // from_value, to_value (linear scale)
		PITCH, // from_value, to_value (linear scale)
		DESTROY, // no duration; no values
	};

	DECLARE_PROPERTY(int64_t, rollback_frame, = -1);
	DECLARE_PROPERTY(Command, command, = PLAY);
	DECLARE_PROPERTY(float, from_value, = 0.0f);
	DECLARE_PROPERTY(float, to_value, = 0.0f);
	DECLARE_PROPERTY(Vector3, from_pos);
	DECLARE_PROPERTY(Vector3, to_pos);
	DECLARE_PROPERTY_IS(bool, paused, = false);
	DECLARE_PROPERTY(float, duration, = 0.0f);
	int get_duration_in_frames() const;
};
DECLARE_ENUM(JigsawSoundCommandHistory::Command);

class JigsawSound : public JigsawPresentationData {
	GDCLASS(JigsawSound, JigsawPresentationData);

protected:
	static void _bind_methods();

public:
	enum Space {
		GLOBAL = 0,
		STAGE_3D = 1,
		HUD_3D = 2,
		STAGE_2D = 3,
		HUD_2D = 4,
	};
	enum Type {
		OTHER = 0,
		CARD_SOUND = 1,
		MUSIC = 2,
	};

	DECLARE_PROPERTY(Space, space, = GLOBAL);
	DECLARE_PROPERTY(Type, type, = OTHER);
	DECLARE_PROPERTY(Ref<JigsawParameterAudio>, track);
	DECLARE_PROPERTY(TypedArray<JigsawSoundCommandHistory>, history);
	DECLARE_PROPERTY_OBJECTID(Node, node);
	DECLARE_PROPERTY_IS(bool, destroyed, = false);

	void init_node(JigsawVisual *visual);
	void kill_node() override;
	void discard_rollback_data(int64_t new_base_frame = -1) override;
	void rollback_to_frame(int64_t frame) override;
	void advance_frames(int64_t frames = 1) override;

	void play(float seek = 0.0f, int64_t frame = -1);
	void pause(int64_t frame = -1);
	void resume(int64_t frame = -1);
	void destroy(int64_t frame = -1);

	void set_position(Vector3 position, float duration = 0.0f, int64_t frame = -1);
	void set_volume(float volume, float duration = 0.0f, int64_t frame = -1);
	void set_pitch(float pitch, float duration = 0.0f, int64_t frame = -1);
};
DECLARE_ENUM(JigsawSound::Space);
DECLARE_ENUM(JigsawSound::Type);

class JigsawAudio : public AudioStreamPlayer {
	GDCLASS(JigsawAudio, AudioStreamPlayer);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawSound>, sound);
	DECLARE_PROPERTY(Ref<Tween>, volume_tween);
	DECLARE_PROPERTY(Ref<Tween>, pitch_tween);
	inline float get_volume_linear() const {
		return Math::db2linear(get_volume_db());
	}
	inline void set_volume_linear(float new_volume_linear) {
		set_volume_db(Math::linear2db(new_volume_linear));
	}
};

class JigsawAudio2D : public AudioStreamPlayer2D {
	GDCLASS(JigsawAudio2D, AudioStreamPlayer2D);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawSound>, sound);
	DECLARE_PROPERTY(Ref<Tween>, position_tween);
	DECLARE_PROPERTY(Ref<Tween>, volume_tween);
	DECLARE_PROPERTY(Ref<Tween>, pitch_tween);
	inline float get_volume_linear() const {
		return Math::db2linear(get_volume_db());
	}
	inline void set_volume_linear(float new_volume_linear) {
		set_volume_db(Math::linear2db(new_volume_linear));
	}
};

class JigsawAudio3D : public AudioStreamPlayer3D {
	GDCLASS(JigsawAudio3D, AudioStreamPlayer3D);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawSound>, sound);
	DECLARE_PROPERTY(Ref<Tween>, position_tween);
	DECLARE_PROPERTY(Ref<Tween>, volume_tween);
	DECLARE_PROPERTY(Ref<Tween>, pitch_tween);
	inline float get_volume_linear() const {
		return Math::db2linear(get_volume_db());
	}
	inline void set_volume_linear(float new_volume_linear) {
		set_volume_db(Math::linear2db(new_volume_linear));
	}
};

#endif // JIGSAW_AUDIO_H
