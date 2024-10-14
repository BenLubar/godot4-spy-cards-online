#ifndef JIGSAW_PARAMETER_H
#define JIGSAW_PARAMETER_H

#include "dry.h"

class JigsawParameter : public Resource {
	GDCLASS(JigsawParameter, Resource);

public:
	enum Type {
		UI_LABEL_INSTANCE = -11,
		UI_ICON_INSTANCE = -10,
		AUDIENCE_INSTANCE = -9,
		CHARACTER_INSTANCE = -8,
		SPRITE_INSTANCE = -7,
		SCENE_INSTANCE = -6,
		QUEUED_EFFECT = -5,
		MODIFIER_INSTANCE = -4,
		CARD_INSTANCE = -3,
		FORMATTED_TEXT = -2,
		NONE = -1,
		LOCAL_VARIABLE = 0,
		VARIABLE = 1,
		ORDERED_LIST = 2,
		BOOLEAN = 3,
		AMOUNT = 4,
		FLOAT = 5,
		STRING = 6,
		EFFECT_INSTANCE = 7,
		CARD_FILTER = 8,
		CARD = 9,
		STAT = 10,
		RANK = 11,
		TRIBE = 12,
		MODIFIER = 13,
		LOCATION = 14,
		ICON = 15,
		CID_OPUS = 16,
		FILE_ID_OPUS = 17,
		FILE_ID_GLTF = 18,
		COLOR = 19,
		CHOICE = 20,
		CHARACTER = 21,
		AUDIENCE = 22,
	};

protected:
	static void _bind_methods();

public:
	JigsawParameter() = default;
	~JigsawParameter() = default;

	virtual Type get_type() const = 0;

	static bool is_concrete_type(Type type) { return type != NONE && type != VARIABLE && type != LOCAL_VARIABLE; }
	static bool is_saved_type(Type type) { return type > NONE; }
};
DECLARE_ENUM(JigsawParameter::Type);

#endif // JIGSAW_PARAMETER_H
