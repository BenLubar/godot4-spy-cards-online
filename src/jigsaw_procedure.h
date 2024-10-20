#ifndef JIGSAW_PROCEDURE_H
#define JIGSAW_PROCEDURE_H

#include "dry.h"

class JigsawProcedure;
class JigsawProcedureEffectCurve;
class JigsawProcedureEffectDescribe;
class JigsawProcedureEffectSimpleDescribe;
class JigsawProcedureEffectExtendedDescribe;
class JigsawProcedureModifierDescribe;
class JigsawProcedureModifierSimpleDescribe;
class JigsawProcedureModifierExtendedDescribe;
class JigsawProcedureStickerShouldShow;
class JigsawProcedureStatFormatCost;

#include "jigsaw_command_list.h"
#include "jigsaw_parameter.h"

class JigsawProcedure : public Resource {
	GDCLASS(JigsawProcedure, Resource);

protected:
	static void _bind_methods();

public:
	JigsawProcedure() = default;
	~JigsawProcedure() = default;

	DECLARE_PROPERTY(Ref<JigsawCommandList>, commands);
	virtual String get_editor_name() const = 0;
	virtual String get_editor_description() const = 0;
	virtual TypedArray<JigsawParameter> get_arguments() const = 0;
	virtual PackedStringArray get_argument_names() const = 0;
	virtual TypedArray<JigsawParameter> get_results() const = 0;
	virtual PackedStringArray get_result_names() const = 0;
};

#define DECLARE_JIGSAW_PROCEDURE(m_type) \
	class m_type : public JigsawProcedure { \
		GDCLASS(m_type, JigsawProcedure); \
	protected: \
		static void _bind_methods(); \
	public: \
		m_type() = default; \
		~m_type() = default; \
		String get_editor_name() const override; \
		String get_editor_description() const override; \
		TypedArray<JigsawParameter> get_arguments() const override; \
		PackedStringArray get_argument_names() const override; \
		TypedArray<JigsawParameter> get_results() const override; \
		PackedStringArray get_result_names() const override; \
	}

DECLARE_JIGSAW_PROCEDURE(JigsawProcedureEffectCurve);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureEffectDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureEffectSimpleDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureEffectExtendedDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureModifierDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureModifierSimpleDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureModifierExtendedDescribe);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureStickerShouldShow);
DECLARE_JIGSAW_PROCEDURE(JigsawProcedureStatFormatCost);

#endif // JIGSAW_PROCEDURE_H
