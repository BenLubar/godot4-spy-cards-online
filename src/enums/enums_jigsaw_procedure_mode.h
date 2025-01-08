#ifndef ENUMS_JIGSAW_PROCEDURE_MODE_H
#define ENUMS_JIGSAW_PROCEDURE_MODE_H

namespace enums {
namespace JigsawProcedure {
enum Mode {
	FUNCTIONAL = 0,
	LOGIC = 1,
	VISUAL = 2,
	INIT = 3,
	MAIN = 4,
	CHOICE_SELECT = 5,
	CHOICE_PREVIEW = 6,
	REALTIME_LOGIC = 7,
	REALTIME_VISUAL = 8,
};
}
}
DECLARE_ENUM(enums::JigsawProcedure::Mode);

#endif // ENUMS_JIGSAW_PROCEDURE_MODE_H
