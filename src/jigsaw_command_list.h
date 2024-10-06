#ifndef JIGSAW_COMMAND_LIST_H
#define JIGSAW_COMMAND_LIST_H

#include "dry.h"

class JigsawCommandList : public Resource {
	GDCLASS(JigsawCommandList, Resource);

protected:
	static void _bind_methods();

public:
	JigsawCommandList();
	~JigsawCommandList();
};

#endif // JIGSAW_COMMAND_LIST_H
