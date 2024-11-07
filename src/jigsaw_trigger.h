#ifndef JIGSAW_TRIGGER_H
#define JIGSAW_TRIGGER_H

#include "jigsaw_procedure.h"

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
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTriggerVariant::Type);

#endif // JIGSAW_TRIGGER_H
