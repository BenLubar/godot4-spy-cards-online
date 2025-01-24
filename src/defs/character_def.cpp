#include "character_def.h"

#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

void CharacterDef::_bind_methods() {
	using namespace enums::CharacterDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, id);
	BIND_PROPERTY(Variant::STRING, display_name);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, tags);
	BIND_PROPERTY_IS(Variant::BOOL, hidden);

	BIND_PROPERTY_ENUM(enums::IconDef::Icon, idle0);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, idle1);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, angry);

	BIND_PROPERTY(Variant::FLOAT, idle0_center_px);
	BIND_PROPERTY(Variant::FLOAT, idle1_center_px);
	BIND_PROPERTY(Variant::FLOAT, angry_center_px);

	BIND_PROPERTY(Variant::FLOAT, pixel_size);
	BIND_PROPERTY(Variant::VECTOR2, portrait_offset);
	BIND_PROPERTY(Variant::FLOAT, portrait_scale);
	BIND_PROPERTY(Variant::BOOL, portrait_crop_top);
	BIND_PROPERTY(Variant::BOOL, portrait_crop_front);

	ClassDB::bind_method(D_METHOD("create_portrait", "mode"), &CharacterDef::create_portrait);
	ClassDB::bind_method(D_METHOD("set_portrait_2d", "mode", "inst"), &CharacterDef::set_portrait_2d);
}

IMPLEMENT_PROPERTY(CharacterDef, String, id);
IMPLEMENT_PROPERTY(CharacterDef, String, display_name);
IMPLEMENT_PROPERTY(CharacterDef, PackedStringArray, tags);
IMPLEMENT_PROPERTY_IS(CharacterDef, bool, hidden);

IMPLEMENT_PROPERTY(CharacterDef, enums::IconDef::Icon, idle0);
IMPLEMENT_PROPERTY(CharacterDef, enums::IconDef::Icon, idle1);
IMPLEMENT_PROPERTY(CharacterDef, enums::IconDef::Icon, angry);

IMPLEMENT_PROPERTY(CharacterDef, float, idle0_center_px);
IMPLEMENT_PROPERTY(CharacterDef, float, idle1_center_px);
IMPLEMENT_PROPERTY(CharacterDef, float, angry_center_px);

IMPLEMENT_PROPERTY(CharacterDef, float, pixel_size);
IMPLEMENT_PROPERTY(CharacterDef, Vector2, portrait_offset);
IMPLEMENT_PROPERTY(CharacterDef, float, portrait_scale);
IMPLEMENT_PROPERTY(CharacterDef, bool, portrait_crop_top);
IMPLEMENT_PROPERTY(CharacterDef, bool, portrait_crop_front);

Ref<Mesh> CharacterDef::create_portrait(const Ref<GameMode> &mode) const {
	ERR_FAIL_COND_V(mode.is_null(), Ref<Mesh>());

	Ref<Texture2D> tex = mode->get_icon_texture(_idle0);

	Ref<StandardMaterial3D> mat;
	mat.instantiate();
	mat->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA_SCISSOR);
	mat->set_alpha_antialiasing(BaseMaterial3D::ALPHA_ANTIALIASING_ALPHA_TO_COVERAGE);
	mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, tex);

	// center and min radius in texel-space
	float half_square = _portrait_scale / _pixel_size * 0.5f;
	Vector2 center{tex->get_width() * 0.5f, half_square};
	center += _portrait_offset;
	Vector2 quarter{half_square, half_square};

	// rect corners in position space
	Vector2 tlp{-0.5f, -0.5f}, brp{0.5f, 0.5f};
	// in uv space
	Vector2 tlt = (center - quarter) / tex->get_size(), brt = (center + quarter) / tex->get_size();

	// front edge
	if (!_portrait_crop_front || tlt.x < 0.0f) {
		tlp.x = -center.x / half_square * 0.5f;
		tlt.x = 0.0f;
	}

	// top edge
	if (!_portrait_crop_top || tlt.y < 0.0f) {
		tlp.y = -center.y / half_square * 0.5f;
		tlt.y = 0.0f;
	}

	// back edge
	if (brt.x > 1.0f) {
		brp.x = (tex->get_width() - center.x) / half_square * 0.5f;
		brt.x = 1.0f;
	}

	// bottom edge
	if (brt.y > 1.0f) {
		brp.y = (tex->get_height() - center.y) / half_square * 0.5f;
		brt.y = 1.0f;
	}

	BitField<Mesh::ArrayFormat> format = Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_INDEX | Mesh::ARRAY_FLAG_USE_2D_VERTICES;

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);

	arrays[Mesh::ARRAY_VERTEX] = PackedVector2Array{{tlp.x, tlp.y}, {brp.x, tlp.y}, {brp.x, brp.y}, {tlp.x, brp.y}};
	arrays[Mesh::ARRAY_TEX_UV] = PackedVector2Array{{tlt.x, tlt.y}, {brt.x, tlt.y}, {brt.x, brt.y}, {tlt.x, brt.y}};
	arrays[Mesh::ARRAY_INDEX] = PackedInt32Array{0, 1, 2, 2, 3, 0};

	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(), format);
	mesh->surface_set_name(0, _id);
	mesh->surface_set_material(0, mat);

	return mesh;
}

void CharacterDef::set_portrait_2d(const Ref<GameMode> &mode, MeshInstance2D *inst) const {
	ERR_FAIL_NULL(inst);

	Ref<Mesh> mesh = create_portrait(mode);
	ERR_FAIL_COND(mesh.is_null());
	inst->set_mesh(mesh);

	Ref<StandardMaterial3D> mat = mesh->surface_get_material(0);
	inst->set_texture(mat->get_texture(BaseMaterial3D::TEXTURE_ALBEDO));
}
