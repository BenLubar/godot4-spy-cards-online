#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include "dry.h"

class DataContainer : public Resource {
	GDCLASS(DataContainer, Resource);

protected:
	static void _bind_methods();

public:
	// first 8 bytes of sha256("Spy Cards Online")
	static constexpr uint64_t MAGIC = 0x300e7eb7871ca461LLU;

	enum FormatRevision {
		FORMAT_0 = 0,
	};

	enum ContainerType {
		GAME_MODE = 0,
		GAME_MODE_SUMMARY = 1,
		RECORDING = 2,
		ERROR_REPORT = 3,
	};

	DECLARE_PROPERTY(FormatRevision, format_revision, = FORMAT_0);
	DECLARE_PROPERTY(ContainerType, container_type, = GAME_MODE);
	DECLARE_PROPERTY(Vector3i, game_version);

	DECLARE_PROPERTY(uint64_t, timestamp, = 0);

	static Ref<DataContainer> from_byte_array(const PackedByteArray &buf, const String &debug_name = "data container");
	PackedByteArray to_byte_array(const String &debug_name = "data container") const;
};
DECLARE_ENUM(DataContainer::FormatRevision);
DECLARE_ENUM(DataContainer::ContainerType);

#endif // DATA_CONTAINER_H
