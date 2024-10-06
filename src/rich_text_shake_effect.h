#ifndef RICH_TEXT_SHAKE_EFFECT_H
#define RICH_TEXT_SHAKE_EFFECT_H

#include "dry.h"

#include <godot_cpp/classes/char_fx_transform.hpp>
#include <godot_cpp/classes/rich_text_effect.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

class RichTextShakeEffect : public RichTextEffect {
	GDCLASS(RichTextShakeEffect, RichTextEffect);

protected:
	static void _bind_methods();

private:
	Ref<RandomNumberGenerator> _rng;

public:
	RichTextShakeEffect();
	~RichTextShakeEffect() = default;

	constexpr static double DEFAULT_RATE = 20.0;
	constexpr static double DEFAULT_STRENGTH = 5.0;

	DECLARE_PROPERTY(String, bbcode, = "sc_shake");

	bool _process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const override;
};

#endif // RICH_TEXT_SHAKE_EFFECT_H
