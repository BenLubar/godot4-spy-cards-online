#ifndef JIGSAW_PRESENTATION_DATA_H
#define JIGSAW_PRESENTATION_DATA_H

#include "dry.h"

class JigsawVisual;

class JigsawPresentationData : public Resource {
	GDCLASS(JigsawPresentationData, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, created_frame, = -1);
	DECLARE_PROPERTY(int64_t, base_frame, = -1);
	DECLARE_PROPERTY(int64_t, current_frame, = -1);

	virtual void init_node(JigsawVisual *visual) = 0;
	virtual void kill_node() = 0;
	virtual void discard_rollback_data(int64_t new_base_frame = -1) = 0;
	virtual void rollback_to_frame(int64_t frame) = 0;
};

#endif // JIGSAW_PRESENTATION_DATA_H
