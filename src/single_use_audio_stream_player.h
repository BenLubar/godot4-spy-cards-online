#ifndef SINGLE_USE_AUDIO_STREAM_PLAYER_H
#define SINGLE_USE_AUDIO_STREAM_PLAYER_H

#include "dry.h"

#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

class SingleUseAudioStreamPlayer : public AudioStreamPlayer {
	GDCLASS(SingleUseAudioStreamPlayer, AudioStreamPlayer);

protected:
	static void _bind_methods();

public:
	SingleUseAudioStreamPlayer();
};

class SingleUseAudioStreamPlayer2D : public AudioStreamPlayer2D {
	GDCLASS(SingleUseAudioStreamPlayer2D, AudioStreamPlayer2D);

protected:
	static void _bind_methods();

public:
	SingleUseAudioStreamPlayer2D();
};

class SingleUseAudioStreamPlayer3D : public AudioStreamPlayer3D {
	GDCLASS(SingleUseAudioStreamPlayer3D, AudioStreamPlayer3D);

protected:
	static void _bind_methods();

public:
	SingleUseAudioStreamPlayer3D();
};

#endif // SINGLE_USE_AUDIO_STREAM_PLAYER_H
