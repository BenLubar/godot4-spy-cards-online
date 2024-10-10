#ifndef JIGSAW_TRIGGER_H
#define JIGSAW_TRIGGER_H

#include "dry.h"

class JigsawTrigger;

#include "jigsaw_command_list.h"

class JigsawTrigger : public Resource {
	GDCLASS(JigsawTrigger, Resource);

public:
	enum Type {
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawCommandList>, commands);

	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawTrigger::Type);

#endif // JIGSAW_TRIGGER_H
