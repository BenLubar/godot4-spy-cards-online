#ifndef JIGSAW_FUNCTION_H
#define JIGSAW_FUNCTION_H

#include "jigsaw_procedure.h"

class JigsawFunction : public JigsawProcedure {
	GDCLASS(JigsawFunction, JigsawProcedure);

protected:
	static void _bind_methods();

public:
	JigsawFunction() = default;
	~JigsawFunction() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY(PackedStringArray, argument_names);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, results);
	DECLARE_PROPERTY(PackedStringArray, result_names);
};

#endif // JIGSAW_FUNCTION_H
