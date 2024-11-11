#ifndef JIGSAW_VISUAL_H
#define JIGSAW_VISUAL_H

#include "dry.h"

class JigsawVisual;

#include "audience.h"
#include "jigsaw_parameter_audio.h"

#include <godot_cpp/classes/audio_effect_spectrum_analyzer_instance.hpp>
#include <godot_cpp/classes/camera_attributes_practical.hpp>
#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/viewport_texture.hpp>
#include <godot_cpp/classes/world3d.hpp>

namespace godot {
	class AudioStreamPlayer;
	class Camera3D;
	class CanvasLayer;
	class DirectionalLight3D;
	class MultiMeshInstance3D;
	class RayCast3D;
	class SubViewport;
	class TextureRect;
	class WorldEnvironment;
}

class JigsawVisual : public Node3D {
	GDCLASS(JigsawVisual, Node3D);

protected:
	static void _bind_methods();

public:
	static constexpr int64_t HUD_EMULATED_SIZE_X = 1280;
	static constexpr int64_t HUD_EMULATED_SIZE_Y = 720;
	static constexpr double FORCE_ASPECT = 16.0 / 9.0;
	static_assert(HUD_EMULATED_SIZE_X == HUD_EMULATED_SIZE_Y * FORCE_ASPECT);
	static constexpr double HUD_3D_SCALE = 0.01;
	static constexpr double STAGE_FOV_HORIZONTAL = 75;
	static const double STAGE_FOV_VERTICAL;
	static const double HUD_FOV_HORIZONTAL;
	static constexpr double HUD_FOV_VERTICAL = 60;

private:
	CanvasLayer *_stage_layer = nullptr;
	TextureRect *_stage_viewport_container = nullptr;
	Ref<Environment> _stage_environment;
	Ref<CameraAttributesPractical> _stage_camera_attributes;
	Ref<World3D> _stage_world_3d;
	SubViewport *_stage_viewport = nullptr;
	Node3D *_stage_camera_target = nullptr;
	Camera3D *_stage_camera = nullptr;
	TextureRect *_stage_simple_background = nullptr;

	Ref<Environment> _hud_environment;
	Ref<CameraAttributesPractical> _hud_camera_attributes;
	WorldEnvironment *_hud_world_environment = nullptr;
	Camera3D *_hud_camera = nullptr;
	RayCast3D *_hud_ray_cast = nullptr;
	DirectionalLight3D *_hud_light = nullptr;
	AudioStreamPlayer *_music_player = nullptr;

	Vector2i _last_size;
	Ref<AudioEffectSpectrumAnalyzerInstance> _fft;
	Vector2 _fft_average;
	Vector<MultiMeshInstance3D *> _audience_meshes;

public:
	DECLARE_PROPERTY(Ref<Audience>, audience);
	DECLARE_PROPERTY(Ref<JigsawParameterAudio>, music);
	DECLARE_PROPERTY(bool, force_simple_background, = false);

	JigsawVisual();

	void _ready() override;
	void _process(double p_delta) override;
	void _physics_process(double p_delta) override;
	void _init_audience();

	Node3D *get_picked_object() const;

	Ref<Texture2D> get_simple_background() const;
	void set_simple_background(const Ref<Texture2D> &texture);
	bool get_simple_background_stretch() const;
	void set_simple_background_stretch(bool stretch);
	Color get_scene_background_color() const;
	void set_scene_background_color(Color new_color);
	float get_ambient_light_intensity() const;
	void set_ambient_light_intensity(float new_intensity);
};

#endif // JIGSAW_VISUAL_H
