#include "protocol/data_container.h"

#include "util/format_helper.h"
#include "util/why_isnt_this_in_godot.h"

void DataContainer::_bind_methods() {
	BIND_CONSTANT(MAGIC);

	BIND_ENUM_CONSTANT(FORMAT_0);

	BIND_ENUM_CONSTANT(GAME_MODE);
	BIND_ENUM_CONSTANT(GAME_MODE_SUMMARY);
	BIND_ENUM_CONSTANT(RECORDING);
	BIND_ENUM_CONSTANT(ERROR_REPORT);

	BIND_PROPERTY_ENUM(DataContainer::FormatRevision, format_revision);
	BIND_PROPERTY_ENUM(DataContainer::ContainerType, container_type);
	BIND_PROPERTY(Variant::VECTOR3I, game_version);

	BIND_PROPERTY(Variant::INT, timestamp);

	ClassDB::bind_static_method("DataContainer", D_METHOD("from_byte_array", "buf", "debug_name"), &DataContainer::from_byte_array, DEFVAL("data container"));
	ClassDB::bind_method(D_METHOD("to_byte_array", "debug_name"), &DataContainer::to_byte_array, DEFVAL("data container"));
}

IMPLEMENT_PROPERTY(DataContainer, DataContainer::FormatRevision, format_revision);
IMPLEMENT_PROPERTY(DataContainer, DataContainer::ContainerType, container_type);
IMPLEMENT_PROPERTY(DataContainer, Vector3i, game_version);

IMPLEMENT_PROPERTY(DataContainer, uint64_t, timestamp);

Ref<DataContainer> DataContainer::from_byte_array(const PackedByteArray &buf, const String &debug_name) {
	Ref<FormatHelper> fh = FormatHelper::read(debug_name, buf);

	uint64_t magic = fh->read_uint64();
	ERR_FAIL_COND_V(magic != MAGIC, Ref<DataContainer>());

	Ref<DataContainer> data;
	data.instantiate();

	data->set_format_revision(static_cast<DataContainer::FormatRevision>(fh->read_uvarint()));
	ERR_FAIL_COND_V(data->get_format_revision() > FORMAT_0, Ref<DataContainer>());

	data->set_container_type(static_cast<DataContainer::ContainerType>(fh->read_uvarint()));
	data->set_game_version(Vector3i(fh->read_uvarint(), fh->read_uvarint(), fh->read_uvarint()));

	switch (data->get_container_type()) {
	case GAME_MODE:
		ERR_FAIL_V_MSG(Ref<DataContainer>(), "DataContainer cannot encode data: not yet implemented");
		break;
	case GAME_MODE_SUMMARY:
		ERR_FAIL_V_MSG(Ref<DataContainer>(), "DataContainer cannot encode data: not yet implemented");
		break;
	case RECORDING:
		data->set_timestamp(fh->read_uvarint());
		ERR_FAIL_V_MSG(Ref<DataContainer>(), "DataContainer cannot encode data: not yet implemented");
		break;
	case ERROR_REPORT:
		data->set_timestamp(fh->read_uvarint());
		ERR_FAIL_V_MSG(Ref<DataContainer>(), "DataContainer cannot encode data: not yet implemented");
		break;
	default:
		ERR_FAIL_V_MSG(Ref<DataContainer>(), vformat("DataContainer cannot encode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", data->get_container_type())));
		break;
	}

	ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());
	return data;
}

PackedByteArray DataContainer::to_byte_array(const String &debug_name) const {
	Ref<FormatHelper> fh = FormatHelper::write(debug_name);

	fh->write_uint64(MAGIC);
	fh->write_uvarint(_format_revision);
	fh->write_uvarint(_container_type);
	fh->write_uvarint(_game_version.x);
	fh->write_uvarint(_game_version.y);
	fh->write_uvarint(_game_version.z);

	switch (_container_type) {
	case GAME_MODE:
		ERR_FAIL_V_MSG(PackedByteArray(), "DataContainer cannot encode data: not yet implemented");
		break;
	case GAME_MODE_SUMMARY:
		ERR_FAIL_V_MSG(PackedByteArray(), "DataContainer cannot encode data: not yet implemented");
		break;
	case RECORDING:
		fh->write_uvarint(_timestamp);
		ERR_FAIL_V_MSG(PackedByteArray(), "DataContainer cannot encode data: not yet implemented");
		break;
	case ERROR_REPORT:
		fh->write_uvarint(_timestamp);
		ERR_FAIL_V_MSG(PackedByteArray(), "DataContainer cannot encode data: not yet implemented");
		break;
	default:
		ERR_FAIL_V_MSG(PackedByteArray(), vformat("DataContainer cannot encode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", _container_type)));
		break;
	}

	ERR_FAIL_COND_V(!fh->is_valid(), PackedByteArray());
	return fh->get_buffer();
}
