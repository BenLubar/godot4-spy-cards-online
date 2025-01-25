#ifndef ENUMS_JIGSAW_PROCEDURE_MODE_H
#define ENUMS_JIGSAW_PROCEDURE_MODE_H

namespace enums {
namespace JigsawProcedure {
enum Mode {
	FUNCTIONAL = 0,
	INIT = 1,
	MAIN = 2,
	SELECT = 3,
	VISUAL = 4,
	CHOICE_PREVIEW = 5,
	REALTIME_LOGIC = 6,
	REALTIME_VISUAL = 7,
};
}
}
DECLARE_ENUM(enums::JigsawProcedure::Mode);

#endif // ENUMS_JIGSAW_PROCEDURE_MODE_H
