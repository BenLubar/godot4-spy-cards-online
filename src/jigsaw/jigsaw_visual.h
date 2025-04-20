#ifndef JIGSAW_VISUAL_H
#define JIGSAW_VISUAL_H

#include "dry.h"

class JigsawVisual;

#include "active/audience.h"
#include "jigsaw/parameter/jigsaw_parameter_expression.h"

#include <godot_cpp/classes/audio_effect_spectrum_analyzer_instance.hpp>
#include <godot_cpp/classes/camera_attributes_practical.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/viewport_texture.hpp>
#include <godot_cpp/classes/world3d.hpp>

namespace godot {
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
	ColorRect *_stage_compatibility_background_color = nullptr;
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

	Vector2i _last_size;
	Ref<AudioEffectSpectrumAnalyzerInstance> _fft;
	Vector2 _fft_average;
	Vector<MultiMeshInstance3D *> _audience_meshes;

	void _set_use_simple_background(bool force_2d);

public:
	DECLARE_PROPERTY(Ref<Audience>, audience);
	DECLARE_PROPERTY(bool, force_simple_background, = false);

	DECLARE_PROPERTY(double, camera_time, = 0.0);
	DECLARE_PROPERTY(Ref<JigsawParameterExpression>, stage_camera_target_position_expr);
	DECLARE_PROPERTY(PackedFloat64Array, stage_camera_target_position_expr_args);
	DECLARE_PROPERTY(Ref<JigsawParameterExpression>, stage_camera_target_rotation_expr);
	DECLARE_PROPERTY(PackedFloat64Array, stage_camera_target_rotation_expr_args);
	DECLARE_PROPERTY(Ref<JigsawParameterExpression>, stage_camera_offset_expr);
	DECLARE_PROPERTY(PackedFloat64Array, stage_camera_offset_expr_args);
	DECLARE_PROPERTY(Ref<JigsawParameterExpression>, stage_camera_rotation_offset_expr);
	DECLARE_PROPERTY(PackedFloat64Array, stage_camera_rotation_offset_expr_args);

	JigsawVisual();

	void _ready() override;
	void _process(double p_delta) override;
	void _physics_process(double p_delta) override;
	void _init_audience();

	DEFAULT_TO_STRING();

	void force_camera_update();
	Node3D *get_picked_object() const;
	SubViewport *get_stage_viewport() const;

	Ref<Texture2D> get_simple_background() const;
	void set_simple_background(const Ref<Texture2D> &texture);
	bool get_simple_background_stretch() const;
	void set_simple_background_stretch(bool stretch);
	Color get_scene_background_color() const;
	void set_scene_background_color(Color new_color);
	float get_ambient_light_intensity() const;
	void set_ambient_light_intensity(float new_intensity);
	Vector3 get_stage_camera_target_position() const;
	void set_stage_camera_target_position(Vector3 new_position);
	Vector3 get_stage_camera_target_rotation() const;
	void set_stage_camera_target_rotation(Vector3 new_rotation);
	Vector3 get_stage_camera_target_rotation_degrees() const;
	void set_stage_camera_target_rotation_degrees(Vector3 new_rotation);
	Vector3 get_stage_camera_offset() const;
	void set_stage_camera_offset(Vector3 new_position);
	Vector3 get_stage_camera_rotation_offset() const;
	void set_stage_camera_rotation_offset(Vector3 new_rotation);
	Vector3 get_stage_camera_rotation_offset_degrees() const;
	void set_stage_camera_rotation_offset_degrees(Vector3 new_rotation);
};

#endif // JIGSAW_VISUAL_H
