#ifndef LEGACY_PARSE_H
#define LEGACY_PARSE_H

#include "dry.h"

#include "protocol/data_container.h"

class LegacyParse : public Object {
	GDCLASS(LegacyParse, Object);

protected:
	static void _bind_methods();

public:
	static Ref<DataContainer> game_mode(const PackedStringArray &buf, const String &name, int64_t revision);
	static Ref<DataContainer> card_recording(const PackedByteArray &buf);
	static Ref<DataContainer> arcade_recording(const PackedByteArray &buf);
};

#endif // LEGACY_PARSE_H
