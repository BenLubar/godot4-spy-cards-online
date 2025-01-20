#ifndef ENUMS_JIGSAW_PROCEDURE_MODE_H
#define ENUMS_JIGSAW_PROCEDURE_MODE_H

namespace enums {
namespace JigsawProcedure {
enum Mode {
	FUNCTIONAL = 0,
	INIT = 1,
	MAIN = 2,
	SELECT = 3,
	CHOICE_PREVIEW = 4,
	REALTIME_LOGIC = 5,
	REALTIME_VISUAL = 6,
};
}
}
DECLARE_ENUM(enums::JigsawProcedure::Mode);

#endif // ENUMS_JIGSAW_PROCEDURE_MODE_H
