#ifndef MODIFIER_INSTANCE_H
#define MODIFIER_INSTANCE_H

#include "dry.h"

class ModifierInstance;

#include "modifier_def.h"

class ModifierInstance : public RefCounted {
	GDCLASS(ModifierInstance, RefCounted);

protected:
	static void _bind_methods();

public:
	ModifierInstance() = default;
	~ModifierInstance() = default;

	DECLARE_PROPERTY(ModifierDef::Modifier, modifier, = ModifierDef::Modifier::NONE);
	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);
};

#endif // MODIFIER_INSTANCE_H
