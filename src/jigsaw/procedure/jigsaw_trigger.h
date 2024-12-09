#ifndef JIGSAW_TRIGGER_H
#define JIGSAW_TRIGGER_H

#include "jigsaw/procedure/jigsaw_procedure.h"
#include "jigsaw/parameter/jigsaw_parameter_choice.h"

class JigsawTrigger : public JigsawProcedure {
	GDCLASS(JigsawTrigger, JigsawProcedure);

protected:
	static void _bind_methods();

public:
};

class JigsawTriggerEffect : public JigsawTrigger {
	GDCLASS(JigsawTriggerEffect, JigsawTrigger);

public:
	enum Type {
		CUSTOM = 0,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTriggerEffect::Type);

class JigsawTriggerModifier : public JigsawTrigger {
	GDCLASS(JigsawTriggerModifier, JigsawTrigger);

public:
	enum Type {
		CUSTOM = 0,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTriggerModifier::Type);

class JigsawTriggerNPC : public JigsawTrigger {
	GDCLASS(JigsawTriggerNPC, JigsawTrigger);

public:
	enum Type {
		CUSTOM = 0,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTriggerNPC::Type);

class JigsawTriggerVariant : public JigsawTrigger {
	GDCLASS(JigsawTriggerVariant, JigsawTrigger);

public:
	enum Type {
		CUSTOM = 0,
		COSMETIC_INIT = 1,
		CHARACTER_INIT = 2,
		DECK_INIT = 3,
		MATCH_START = 4,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTriggerVariant::Type);

#define DECLARE_JIGSAW_TRIGGER(m_type, m_subtype, m_subtype_enum, ...) \
	class JigsawTrigger##m_type##m_subtype : public JigsawTrigger##m_type { \
		GDCLASS(JigsawTrigger##m_type##m_subtype, JigsawTrigger##m_type); \
	protected: \
		static void _bind_methods(); \
	public: \
		Type get_type() const override { return m_subtype_enum; } \
		String get_editor_name() const override; \
		String get_editor_description() const override; \
		TypedArray<JigsawParameter> get_arguments() const override; \
		PackedStringArray get_argument_names() const override; \
		TypedArray<JigsawParameter> get_results() const override; \
		PackedStringArray get_result_names() const override; \
		__VA_ARGS__ \
	}

DECLARE_JIGSAW_TRIGGER(Effect, Custom, CUSTOM, DECLARE_PROPERTY(Ref<JigsawParameterChoice>, custom_tag););

DECLARE_JIGSAW_TRIGGER(Modifier, Custom, CUSTOM, DECLARE_PROPERTY(Ref<JigsawParameterChoice>, custom_tag););

DECLARE_JIGSAW_TRIGGER(NPC, Custom, CUSTOM, DECLARE_PROPERTY(Ref<JigsawParameterChoice>, custom_tag););

DECLARE_JIGSAW_TRIGGER(Variant, Custom, CUSTOM, DECLARE_PROPERTY(Ref<JigsawParameterChoice>, custom_tag););
DECLARE_JIGSAW_TRIGGER(Variant, CosmeticInit, COSMETIC_INIT);
DECLARE_JIGSAW_TRIGGER(Variant, CharacterInit, CHARACTER_INIT);
DECLARE_JIGSAW_TRIGGER(Variant, DeckInit, DECK_INIT);
DECLARE_JIGSAW_TRIGGER(Variant, MatchStart, MATCH_START);

#endif // JIGSAW_TRIGGER_H
