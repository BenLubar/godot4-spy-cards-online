#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "game_mode_summary.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GameMode : public GameModeSummary {
	GDCLASS(GameMode, GameModeSummary);

protected:
	static void _bind_methods();

private:

public:
	GameMode();
	~GameMode();
};

#endif // GAME_MODE_H
