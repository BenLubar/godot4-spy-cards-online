#ifndef EFFECT_INSTANCE_H
#define EFFECT_INSTANCE_H

#include "effect_def.h"
#include "jigsaw_parameter.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class EffectInstance : public Resource {
	GDCLASS(EffectInstance, Resource);

protected:
	static void _bind_methods();

private:
	EffectDef::Effect effect;
	int64_t priority;
	TypedArray<JigsawParameter> params;

public:
	EffectInstance();
	~EffectInstance();
};

#endif // EFFECT_INSTANCE_H
