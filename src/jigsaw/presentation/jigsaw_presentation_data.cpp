#include "jigsaw/presentation/jigsaw_presentation_data.h"

#include "jigsaw/jigsaw_visual.h"

void JigsawPresentationData::_bind_methods() {
	BIND_PROPERTY(Variant::INT, created_frame);
	BIND_PROPERTY(Variant::INT, base_frame);
	BIND_PROPERTY(Variant::INT, current_frame);

	ClassDB::bind_method(D_METHOD("kill_node"), &JigsawPresentationData::kill_node);
	ClassDB::bind_method(D_METHOD("discard_rollback_data", "new_base_frame"), &JigsawPresentationData::discard_rollback_data, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("rollback_to_frame", "frame"), &JigsawPresentationData::rollback_to_frame);
	ClassDB::bind_method(D_METHOD("advance_frames", "frames"), &JigsawPresentationData::advance_frames, DEFVAL(1));
}

IMPLEMENT_PROPERTY(JigsawPresentationData, int64_t, created_frame);
IMPLEMENT_PROPERTY(JigsawPresentationData, int64_t, base_frame);
IMPLEMENT_PROPERTY(JigsawPresentationData, int64_t, current_frame);
