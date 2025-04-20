#include "jigsaw/jigsaw_visual.h"

#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world_environment.hpp>

#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "util/player_preferences_helper.h"

static LazyStringName MUSIC_BUS{ "Music" };

const double JigsawVisual::STAGE_FOV_VERTICAL = Math::rad_to_deg(2 * Math::atan(Math::tan(Math::deg_to_rad(JigsawVisual::STAGE_FOV_HORIZONTAL) / 2) * JigsawVisual::FORCE_ASPECT));
const double JigsawVisual::HUD_FOV_HORIZONTAL = Math::rad_to_deg(2 * Math::atan(Math::tan(Math::deg_to_rad(JigsawVisual::HUD_FOV_VERTICAL) / 2) / JigsawVisual::FORCE_ASPECT));

void JigsawVisual::_bind_methods() {
	BIND_PROPERTY_RESOURCE(Audience, audience);
	BIND_PROPERTY(Variant::BOOL, force_simple_background);

	BIND_PROPERTY_RESOURCE(Texture2D, simple_background);
	BIND_PROPERTY(Variant::BOOL, simple_background_stretch);

	BIND_PROPERTY(Variant::COLOR, scene_background_color);
	BIND_PROPERTY(Variant::FLOAT, ambient_light_intensity);

	BIND_PROPERTY(Variant::VECTOR3, stage_camera_target_position);
	BIND_PROPERTY(Variant::VECTOR3, stage_camera_target_rotation);
	BIND_PROPERTY_NOT_SAVED(Variant::VECTOR3, stage_camera_target_rotation_degrees);
	BIND_PROPERTY(Variant::VECTOR3, stage_camera_offset);
	BIND_PROPERTY(Variant::VECTOR3, stage_camera_rotation_offset);
	BIND_PROPERTY_NOT_SAVED(Variant::VECTOR3, stage_camera_rotation_offset_degrees);

	BIND_PROPERTY(Variant::FLOAT, camera_time);
	BIND_PROPERTY_RESOURCE(JigsawParameterExpression, stage_camera_target_position_expr);
	BIND_PROPERTY(Variant::PACKED_FLOAT64_ARRAY, stage_camera_target_position_expr_args);
	BIND_PROPERTY_RESOURCE(JigsawParameterExpression, stage_camera_target_rotation_expr);
	BIND_PROPERTY(Variant::PACKED_FLOAT64_ARRAY, stage_camera_target_rotation_expr_args);
	BIND_PROPERTY_RESOURCE(JigsawParameterExpression, stage_camera_offset_expr);
	BIND_PROPERTY(Variant::PACKED_FLOAT64_ARRAY, stage_camera_offset_expr_args);
	BIND_PROPERTY_RESOURCE(JigsawParameterExpression, stage_camera_rotation_offset_expr);
	BIND_PROPERTY(Variant::PACKED_FLOAT64_ARRAY, stage_camera_rotation_offset_expr_args);

	ClassDB::bind_method(D_METHOD("force_camera_update"), &JigsawVisual::force_camera_update);
	ClassDB::bind_method(D_METHOD("get_picked_object"), &JigsawVisual::get_picked_object);
}

void JigsawVisual::_set_use_simple_background(bool force_2d) {
	_stage_simple_background->set_visible(force_2d);
	_stage_viewport->set_disable_3d(force_2d);
}

IMPLEMENT_PROPERTY_ONCHANGE(JigsawVisual, Ref<Audience>, audience, _init_audience());
IMPLEMENT_PROPERTY_ONCHANGE(JigsawVisual, bool, force_simple_background, _set_use_simple_background(PlayerPreferences::disable_3d() || new_force_simple_background));

IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, double, camera_time);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, Ref<JigsawParameterExpression>, stage_camera_target_position_expr);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, PackedFloat64Array, stage_camera_target_position_expr_args);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, Ref<JigsawParameterExpression>, stage_camera_target_rotation_expr);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, PackedFloat64Array, stage_camera_target_rotation_expr_args);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, Ref<JigsawParameterExpression>, stage_camera_offset_expr);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, PackedFloat64Array, stage_camera_offset_expr_args);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, Ref<JigsawParameterExpression>, stage_camera_rotation_offset_expr);
IMPLEMENT_PROPERTY_SIMPLE(JigsawVisual, PackedFloat64Array, stage_camera_rotation_offset_expr_args);

JigsawVisual::JigsawVisual() {
	_stage_layer = memnew(CanvasLayer);
	_stage_layer->set_name("StageLayer");
	_stage_layer->set_layer(-1);
	add_child(_stage_layer);

	if (RenderingServer::get_singleton()->get_current_rendering_method() == "gl_compatibility") {
		_stage_compatibility_background_color = memnew(ColorRect);
		_stage_compatibility_background_color->set_name("StageCompatibilityBackgroundColor");
		_stage_compatibility_background_color->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		_stage_compatibility_background_color->set_color(Color(0.0f, 0.0f, 0.0f, 0.0f));
		_stage_layer->add_child(_stage_compatibility_background_color);
	}

	_stage_viewport_container = memnew(TextureRect);
	_stage_viewport_container->set_name("StageViewportContainer");
	_stage_viewport_container->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	_stage_viewport_container->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	_stage_layer->add_child(_stage_viewport_container);

	_stage_environment.instantiate();
	_stage_environment->set_background(Environment::BG_COLOR);
	_stage_environment->set_bg_color(Color(0.3, 0.3, 0.3));
	_stage_environment->set_glow_enabled(true);

	_stage_camera_attributes.instantiate();
	_stage_camera_attributes->set_dof_blur_near_distance(4.25);
	_stage_camera_attributes->set_dof_blur_near_transition(2.0);
	_stage_camera_attributes->set_dof_blur_amount(0.15);

	// certain features are only available on the Vulkan/WebGPU backend
	if (RenderingServer::get_singleton()->get_current_rendering_method() == "forward_plus") {
		_stage_environment->set_ssr_enabled(true);
		_stage_camera_attributes->set_dof_blur_near_enabled(true);
	}

	_stage_world_3d.instantiate();
	_stage_world_3d->set_environment(_stage_environment);
	_stage_world_3d->set_camera_attributes(_stage_camera_attributes);

	_stage_viewport = memnew(SubViewport);
	_stage_viewport->set_name("StageViewport");
	_stage_viewport->set_update_mode(SubViewport::UPDATE_WHEN_PARENT_VISIBLE);
	_stage_viewport->set_msaa_3d(Viewport::MSAA_2X);
	_stage_viewport->set_screen_space_aa(Viewport::SCREEN_SPACE_AA_FXAA);
	_stage_viewport->set_as_audio_listener_3d(true);
	_stage_viewport->set_handle_input_locally(false);
	_stage_viewport->set_world_3d(_stage_world_3d);
	_stage_viewport_container->add_child(_stage_viewport);

	if (_stage_compatibility_background_color) {
		_stage_viewport->set_transparent_background(true);
	}

	_stage_camera_target = memnew(Node3D);
	_stage_camera_target->set_name("CameraTarget");
	_stage_camera_target->set_position(Vector3(5, 0, 0));
	_stage_camera_target->set_rotation_degrees(Vector3(-16, 0, 0));
	_stage_viewport->add_child(_stage_camera_target);

	_stage_camera = memnew(Camera3D);
	_stage_camera->set_name("Camera3D");
	_stage_camera->set_keep_aspect_mode(Camera3D::KEEP_WIDTH);
	_stage_camera->set_current(true);
	_stage_camera->set_fov(STAGE_FOV_HORIZONTAL);
	_stage_camera->set_near(0.3);
	_stage_camera->set_far(50);
	_stage_camera->set_position(Vector3(0, 3.5, 5.5));
	_stage_camera_target->add_child(_stage_camera);

	_stage_simple_background = memnew(TextureRect);
	_stage_simple_background->set_name("SimpleBackground");
	_stage_simple_background->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	_stage_simple_background->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	_stage_viewport->add_child(_stage_simple_background);

	_hud_environment.instantiate();
	_hud_environment->set_background(Environment::BG_CANVAS);
	_hud_environment->set_canvas_max_layer(-1);
	_hud_environment->set_ambient_source(Environment::AMBIENT_SOURCE_COLOR);
	_hud_environment->set_ambient_light_color(Color("#444444"));

	_hud_camera_attributes.instantiate();

	_hud_world_environment = memnew(WorldEnvironment);
	_hud_world_environment->set_name("HUDEnvironment");
	_hud_world_environment->set_environment(_hud_environment);
	_hud_world_environment->set_camera_attributes(_hud_camera_attributes);
	add_child(_hud_world_environment);

	_hud_camera = memnew(Camera3D);
	_hud_camera->set_name("HUDCamera");
	_hud_camera->set_keep_aspect_mode(Camera3D::KEEP_WIDTH);
	_hud_camera->set_current(true);
	_hud_camera->set_fov(HUD_FOV_HORIZONTAL);
	_hud_camera->set_near(1);
	_hud_camera->set_far(20);
	_hud_camera->set_position(Vector3(
		HUD_EMULATED_SIZE_X * HUD_3D_SCALE / 2.0f,
		HUD_EMULATED_SIZE_Y * HUD_3D_SCALE / -2.0f,
		HUD_EMULATED_SIZE_X * HUD_3D_SCALE * Math::sin(Math::deg_to_rad(HUD_FOV_VERTICAL))
	));
	add_child(_hud_camera);

	_hud_ray_cast = memnew(RayCast3D);
	_hud_ray_cast->set_name("HUDRayCast");
	_hud_camera->add_child(_hud_ray_cast);

	_hud_light = memnew(DirectionalLight3D);
	_hud_light->set_name("HUDLight");
	_hud_light->set_bake_mode(Light3D::BAKE_STATIC);
	add_child(_hud_light);

	_set_use_simple_background(PlayerPreferences::disable_3d());
}

void JigsawVisual::_ready() {
	AudioServer *audio_server = AudioServer::get_singleton();
	_fft = audio_server->get_bus_effect_instance(audio_server->get_bus_index(MUSIC_BUS), 0);

	_stage_viewport_container->set_texture(_stage_viewport->get_texture());

	force_camera_update();
}

void JigsawVisual::_process(double p_delta) {
	_camera_time += p_delta;

	if (_stage_camera_target_position_expr.is_valid()) {
		Ref<Expression> expr = _stage_camera_target_position_expr->get_parsed_expression();
		if (likely(expr.is_valid())) {
			Variant camera_target_position = expr->execute(Array::make(_camera_time, _stage_camera_target_position_expr_args), nullptr, true, true);
			if (likely(!expr->has_execute_failed())) {
				set_stage_camera_target_position(camera_target_position);
			}
		}
	}

	if (_stage_camera_target_rotation_expr.is_valid()) {
		Ref<Expression> expr = _stage_camera_target_rotation_expr->get_parsed_expression();
		if (likely(expr.is_valid())) {
			Variant camera_target_rotation = expr->execute(Array::make(_camera_time, _stage_camera_target_rotation_expr_args), nullptr, true, true);
			if (likely(!expr->has_execute_failed())) {
				set_stage_camera_target_rotation_degrees(camera_target_rotation);
			}
		}
	}

	if (_stage_camera_offset_expr.is_valid()) {
		Ref<Expression> expr = _stage_camera_offset_expr->get_parsed_expression();
		if (likely(expr.is_valid())) {
			Variant camera_offset = expr->execute(Array::make(_camera_time, _stage_camera_offset_expr_args), nullptr, true, true);
			if (likely(!expr->has_execute_failed())) {
				set_stage_camera_offset(camera_offset);
			}
		}
	}

	if (_stage_camera_rotation_offset_expr.is_valid()) {
		Ref<Expression> expr = _stage_camera_rotation_offset_expr->get_parsed_expression();
		if (likely(expr.is_valid())) {
			Variant camera_rotation_offset = expr->execute(Array::make(_camera_time, _stage_camera_rotation_offset_expr_args), nullptr, true, true);
			if (likely(!expr->has_execute_failed())) {
				set_stage_camera_rotation_offset_degrees(camera_rotation_offset);
			}
		}
	}

	Window *window = get_window();
	Vector2i size = window->get_size();
	if (_last_size != size) {
		_last_size = size;

		double aspect = double(size.x) / double(size.y);
		bool horizontal = aspect <= FORCE_ASPECT;

		_stage_viewport->set_size(size);

		// cover stage
		_stage_camera->set_keep_aspect_mode(horizontal ? Camera3D::KEEP_HEIGHT : Camera3D::KEEP_WIDTH);
		_stage_camera->set_fov(horizontal ? STAGE_FOV_HORIZONTAL : STAGE_FOV_VERTICAL);

		// contain HUD
		_hud_camera->set_keep_aspect_mode(horizontal ? Camera3D::KEEP_WIDTH : Camera3D::KEEP_HEIGHT);
		_hud_camera->set_fov(horizontal ? HUD_FOV_VERTICAL : HUD_FOV_HORIZONTAL);
	}
}

void JigsawVisual::_physics_process(double p_delta) {
	Viewport *hud_viewport = _hud_camera->get_viewport();
	if (hud_viewport) {
		Vector2 mouse_pos = hud_viewport->get_mouse_position();
		Vector3 mouse_origin = _hud_camera->project_ray_origin(mouse_pos);
		Vector3 mouse_normal = _hud_camera->project_ray_normal(mouse_pos);
		_hud_ray_cast->set_target_position(mouse_origin + mouse_normal * _hud_camera->get_far() - _hud_ray_cast->get_global_position());
	}

	Vector2 magnitude = _fft->get_magnitude_for_frequency_range(16, 8000, AudioEffectSpectrumAnalyzerInstance::MAGNITUDE_AVERAGE);

	Vector2i dir;
	if (magnitude.x == 0.0) {
		dir.x = 0;
	} else if (magnitude.x < _fft_average.x) {
		dir.x = -1;
	} else {
		dir.x = 1;
	}

	if (magnitude.y == 0.0) {
		dir.y = 0;
	} else if (magnitude.y < _fft_average.y) {
		dir.y = -1;
	} else {
		dir.y = 1;
	}

	if (_fft_average == Vector2()) {
		_fft_average = magnitude;
	} else {
		_fft_average = (magnitude + _fft_average) / 2.0;
	}

	if (_audience.is_valid()) {
		_audience->update(dir);
	}
}

void JigsawVisual::_init_audience() {
	for (int64_t i = 0; i < _audience_meshes.size(); i++) {
		_audience_meshes[i]->queue_free();
	}

	if (PlayerPreferences::disable_3d()) {
		_audience_meshes.clear();
		return;
	}

	TypedArray<MultiMesh> meshes = _audience.is_valid() ? _audience->get_meshes() : TypedArray<MultiMesh>();
	_audience_meshes.resize(meshes.size());

	for (int64_t i = 0; i < meshes.size(); i++) {
		MultiMeshInstance3D *mmi = memnew(MultiMeshInstance3D);
		mmi->set_multimesh(meshes[i]);
		_stage_viewport->add_child(mmi);
		_audience_meshes.write[i] = mmi;
	}
}

void JigsawVisual::force_camera_update() {
	_process(0.0);
}

Node3D *JigsawVisual::get_picked_object() const {
	// only allow picking objects if the window has focus so we don't do weird things in the background
	if (!get_window()->has_focus()) {
		return nullptr;
	}

	Object *collider = _hud_ray_cast->get_collider();
	if (PhysicsBody3D *body = Object::cast_to<PhysicsBody3D>(collider)) {
		return body->get_parent_node_3d();
	}

	return Object::cast_to<Node3D>(collider);
}

SubViewport *JigsawVisual::get_stage_viewport() const {
	return _stage_viewport;
}

Ref<Texture2D> JigsawVisual::get_simple_background() const {
	return _stage_simple_background->get_texture();
}
void JigsawVisual::set_simple_background(const Ref<Texture2D> &texture) {
	_stage_simple_background->set_texture(texture);
}
bool JigsawVisual::get_simple_background_stretch() const {
	return _stage_simple_background->get_stretch_mode() == TextureRect::STRETCH_SCALE;
}
void JigsawVisual::set_simple_background_stretch(bool stretch) {
	_stage_simple_background->set_stretch_mode(stretch ? TextureRect::STRETCH_SCALE : TextureRect::STRETCH_KEEP_ASPECT_COVERED);
}
Color JigsawVisual::get_scene_background_color() const {
	if (_stage_environment->get_background() == Environment::BG_CLEAR_COLOR) {
		return Color(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return _stage_environment->get_bg_color();
}
void JigsawVisual::set_scene_background_color(Color new_color) {
	if (new_color.a <= 0.0f) {
		_stage_environment->set_background(Environment::BG_CLEAR_COLOR);
	} else {
		_stage_environment->set_background(Environment::BG_COLOR);
		_stage_environment->set_bg_color(new_color);
	}

	if (_stage_compatibility_background_color) {
		// we need to make glow not affect the background color, just like in the higher fidelity render modes
		_stage_compatibility_background_color->set_color(new_color);

		// we do need the default clear color to affect the ambient light, though, for consistency
		_stage_environment->set_ambient_source(new_color.a <= 0.0f ? Environment::AMBIENT_SOURCE_COLOR : Environment::AMBIENT_SOURCE_BG);
		_stage_environment->set_ambient_light_color(RenderingServer::get_singleton()->get_default_clear_color());
	}
}
float JigsawVisual::get_ambient_light_intensity() const {
	return _stage_environment->get_ambient_light_sky_contribution();
}
void JigsawVisual::set_ambient_light_intensity(float new_intensity) {
	_stage_environment->set_ambient_light_sky_contribution(new_intensity);
}

Vector3 JigsawVisual::get_stage_camera_target_position() const {
	return _stage_camera_target->get_position();
}
void JigsawVisual::set_stage_camera_target_position(Vector3 new_position) {
	_stage_camera_target->set_position(new_position);
}
Vector3 JigsawVisual::get_stage_camera_target_rotation() const {
	return _stage_camera_target->get_rotation();
}
void JigsawVisual::set_stage_camera_target_rotation(Vector3 new_rotation) {
	_stage_camera_target->set_rotation(new_rotation);
}
Vector3 JigsawVisual::get_stage_camera_target_rotation_degrees() const {
	return _stage_camera_target->get_rotation_degrees();
}
void JigsawVisual::set_stage_camera_target_rotation_degrees(Vector3 new_rotation) {
	_stage_camera_target->set_rotation_degrees(new_rotation);
}
Vector3 JigsawVisual::get_stage_camera_offset() const {
	return _stage_camera->get_position();
}
void JigsawVisual::set_stage_camera_offset(Vector3 new_position) {
	_stage_camera->set_position(new_position);
}
Vector3 JigsawVisual::get_stage_camera_rotation_offset() const {
	return _stage_camera->get_rotation();
}
void JigsawVisual::set_stage_camera_rotation_offset(Vector3 new_rotation) {
	_stage_camera->set_rotation(new_rotation);
}
Vector3 JigsawVisual::get_stage_camera_rotation_offset_degrees() const {
	return _stage_camera->get_rotation_degrees();
}
void JigsawVisual::set_stage_camera_rotation_offset_degrees(Vector3 new_rotation) {
	_stage_camera->set_rotation_degrees(new_rotation);
}
