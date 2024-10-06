#include "stat_def.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void StatDef::_bind_methods() {
	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(ATK);
	BIND_ENUM_CONSTANT(DEF);
	BIND_ENUM_CONSTANT(HP);
	BIND_ENUM_CONSTANT(TP);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, name);
	BIND_PROPERTY(Variant::INT, icon);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, format_quantity);
}
StatDef::StatDef() {}
StatDef::~StatDef() {}

IMPLEMENT_PROPERTY(StatDef, String, name);
IMPLEMENT_PROPERTY(StatDef, IconDef::Icon, icon);
IMPLEMENT_PROPERTY(StatDef, Ref<JigsawCommandList>, format_quantity);
