#ifndef JIGSAW_FUNCTION_H
#define JIGSAW_FUNCTION_H

#include "jigsaw_procedure.h"

class JigsawFunction : public JigsawProcedure {
	GDCLASS(JigsawFunction, JigsawProcedure);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY_IMPLGET(String, editor_name);
	DECLARE_PROPERTY_IMPLGET(String, editor_description);
	DECLARE_PROPERTY_IMPLGET(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY_IMPLGET(PackedStringArray, argument_names);
	DECLARE_PROPERTY_IMPLGET(TypedArray<JigsawParameter>, results);
	DECLARE_PROPERTY_IMPLGET(PackedStringArray, result_names);

	DECLARE_PROPERTY(bool, cached_modifies_game_state, = false);
	DECLARE_PROPERTY(bool, cached_can_pause_execution, = false);
};

#endif // JIGSAW_FUNCTION_H
