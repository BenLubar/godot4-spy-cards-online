#ifndef JIGSAW_SIDE_LOCATION_H
#define JIGSAW_SIDE_LOCATION_H

#include "dry.h"

class JigsawSideLocation;

#include "card_instance.h"

class JigsawSideLocation : public RefCounted {
	GDCLASS(JigsawSideLocation, RefCounted);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<CardInstance>, cards);
};

#endif // JIGSAW_SIDE_LOCATION_H
