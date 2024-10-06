#include "modifier_instance.h"

void ModifierInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, modifier);
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY_SIMPLE(ModifierInstance, ModifierDef::Modifier, modifier);
IMPLEMENT_PROPERTY_SIMPLE(ModifierInstance, int64_t, amount);
IMPLEMENT_PROPERTY_SIMPLE(ModifierInstance, int64_t, amount_inf);
IMPLEMENT_PROPERTY_SIMPLE_IS(ModifierInstance, bool, nan);
