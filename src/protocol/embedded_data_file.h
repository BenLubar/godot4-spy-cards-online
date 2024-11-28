#ifndef EMBEDDED_DATA_FILE_H
#define EMBEDDED_DATA_FILE_H

#include "dry.h"

class EmbeddedDataFile : public Resource {
	GDCLASS(EmbeddedDataFile, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(PackedByteArray, file_id);
	DECLARE_PROPERTY(PackedByteArray, data);

	bool validate_id() const;
};

#endif // EMBEDDED_DATA_FILE_H
