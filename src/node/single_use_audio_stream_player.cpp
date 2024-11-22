#include "single_use_audio_stream_player.h"

void SingleUseAudioStreamPlayer::_bind_methods() {}

SingleUseAudioStreamPlayer::SingleUseAudioStreamPlayer() {
	connect("finished", callable_mp(static_cast<Node *>(this), &Node::queue_free));
}

void SingleUseAudioStreamPlayer2D::_bind_methods() {}

SingleUseAudioStreamPlayer2D::SingleUseAudioStreamPlayer2D() {
	connect("finished", callable_mp(static_cast<Node *>(this), &Node::queue_free));
}

void SingleUseAudioStreamPlayer3D::_bind_methods() {}

SingleUseAudioStreamPlayer3D::SingleUseAudioStreamPlayer3D() {
	connect("finished", callable_mp(static_cast<Node *>(this), &Node::queue_free));
}
