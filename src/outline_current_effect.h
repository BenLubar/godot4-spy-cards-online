#ifndef OUTLINE_CURRENT_EFFECT_H
#define OUTLINE_CURRENT_EFFECT_H

#include "dry.h"

#include <godot_cpp/classes/char_fx_transform.hpp>
#include <godot_cpp/classes/rich_text_effect.hpp>

class OutlineCurrentEffect : public RichTextEffect {
	GDCLASS(OutlineCurrentEffect, RichTextEffect);

protected:
	static void _bind_methods();

public:
	OutlineCurrentEffect() = default;
	~OutlineCurrentEffect() = default;

	bool _process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const override;
};

#endif // OUTLINE_CURRENT_EFFECT_H
