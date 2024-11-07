#include "jigsaw_trigger.h"

void JigsawTrigger::_bind_methods() {
}

void JigsawTriggerEffect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerEffect::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

void JigsawTriggerModifier::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerModifier::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

void JigsawTriggerNPC::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerNPC::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

void JigsawTriggerVariant::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerVariant::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}
