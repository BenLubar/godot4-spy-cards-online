#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H

#include "game_mode.h"
#include "card_def.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class CardInstance : public RefCounted {
	GDCLASS(CardInstance, RefCounted);

protected:
	static void _bind_methods();

private:

public:
	CardInstance();
	~CardInstance();

	static CardInstance *make(GameMode *game_mode, CardDef *card);
};

#endif // CARD_INSTANCE_H
