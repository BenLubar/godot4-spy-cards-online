#ifndef JIGSAW_PROCEDURE_H
#define JIGSAW_PROCEDURE_H

#include "dry.h"

class JigsawProcedure;
// TODO
using JigsawProcedureEffectCurve = JigsawProcedure;
using JigsawProcedureEffectDescribe = JigsawProcedure;
using JigsawProcedureEffectSimpleDescribe = JigsawProcedure;
using JigsawProcedureModifierDescribe = JigsawProcedure;
using JigsawProcedureModifierSimpleDescribe = JigsawProcedure;
using JigsawProcedureStickerShouldShow = JigsawProcedure;
using JigsawProcedureStatFormatQuantity = JigsawProcedure;

#include "jigsaw_command_list.h"

class JigsawProcedure : public Resource {
	GDCLASS(JigsawProcedure, Resource);

protected:
	static void _bind_methods();

public:
	JigsawProcedure() = default;
	~JigsawProcedure() = default;

	DECLARE_PROPERTY(Ref<JigsawCommandList>, commands);
};

#endif // JIGSAW_PROCEDURE_H
