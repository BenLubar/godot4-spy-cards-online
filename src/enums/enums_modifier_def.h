#ifndef ENUMS_MODIFIER_DEF_H
#define ENUMS_MODIFIER_DEF_H

namespace enums {
namespace ModifierDef {
enum Modifier {
	NONE = -1,

	EMPOWER_ATK = 0,
	EMPOWER_DEF = 1,
	NUMB = 2,
	PREVENT_NUMB = 3,

	FIRST_CUSTOM = 4,
};
}
}
DECLARE_ENUM(enums::ModifierDef::Modifier);

#endif // ENUMS_MODIFIER_DEF_H
