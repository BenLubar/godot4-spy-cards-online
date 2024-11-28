#include "protocol/data_container.h"

#include "util/format_helper.h"
#include "util/why_isnt_this_in_godot.h"
#include "protocol/button_input_history.h"

#include <godot_cpp/classes/hashing_context.hpp>
#include <godot_cpp/templates/local_vector.hpp>

void DataContainer::_bind_methods() {
	BIND_CONSTANT(MAGIC);

	BIND_ENUM_CONSTANT(FORMAT_0);

	BIND_ENUM_CONSTANT(GAME_MODE_SUMMARY);
	BIND_ENUM_CONSTANT(GAME_MODE);
	BIND_ENUM_CONSTANT(RECORDING);
	BIND_ENUM_CONSTANT(ERROR_REPORT);

	BIND_PROPERTY_ENUM(DataContainer::FormatRevision, format_revision);
	BIND_PROPERTY_ENUM(DataContainer::ContainerType, container_type);
	BIND_PROPERTY(Variant::VECTOR3I, game_version);

	BIND_PROPERTY(Variant::INT, timestamp);
	BIND_PROPERTY_RESOURCE_ARRAY(EmbeddedDataFile, data_files);

	BIND_PROPERTY_RESOURCE(GameModeSummary, mode_summary);
	BIND_PROPERTY_RESOURCE(GameMode, mode);

	BIND_PROPERTY(Variant::STRING, mode_public_name);
	BIND_PROPERTY(Variant::INT, mode_public_revision);
	BIND_PROPERTY(Variant::INT, selected_variant);
	BIND_PROPERTY(Variant::INT, rematches);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingPlayerData, player_data);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, shared_seed);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingRoundData, rounds);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, final_checksum);

	ClassDB::bind_static_method("DataContainer", D_METHOD("from_byte_array", "buf", "debug_name"), &DataContainer::from_byte_array, DEFVAL("data container"));
	ClassDB::bind_method(D_METHOD("to_byte_array", "debug_name"), &DataContainer::to_byte_array, DEFVAL("data container"));
}

IMPLEMENT_PROPERTY(DataContainer, DataContainer::FormatRevision, format_revision);
IMPLEMENT_PROPERTY(DataContainer, DataContainer::ContainerType, container_type);
IMPLEMENT_PROPERTY(DataContainer, Vector3i, game_version);

IMPLEMENT_PROPERTY(DataContainer, uint64_t, timestamp);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<EmbeddedDataFile>, data_files);

IMPLEMENT_PROPERTY(DataContainer, Ref<GameModeSummary>, mode_summary);
IMPLEMENT_PROPERTY(DataContainer, Ref<GameMode>, mode);

IMPLEMENT_PROPERTY(DataContainer, String, mode_public_name);
IMPLEMENT_PROPERTY(DataContainer, int64_t, mode_public_revision);
IMPLEMENT_PROPERTY(DataContainer, int64_t, selected_variant);
IMPLEMENT_PROPERTY(DataContainer, int64_t, rematches);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<RecordingPlayerData>, player_data);
IMPLEMENT_PROPERTY(DataContainer, PackedByteArray, shared_seed);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<RecordingRoundData>, rounds);
IMPLEMENT_PROPERTY(DataContainer, PackedByteArray, final_checksum);

bool DataContainer::_decode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) {
	ERR_FAIL_V_MSG(false, "DataContainer cannot decode data: not yet implemented");
}
bool DataContainer::_encode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) const {
	ERR_FAIL_V_MSG(false, "DataContainer cannot encode data: not yet implemented");
}
bool DataContainer::_decode_game_mode(const Ref<FormatHelper> &fh) {
	_mode.instantiate();
	_mode_summary = _mode;
	ERR_FAIL_COND_V(!_decode_game_mode_summary(fh, _mode), false);

	ERR_FAIL_V_MSG(false, "DataContainer cannot decode data: not yet implemented");
}
bool DataContainer::_encode_game_mode(const Ref<FormatHelper> &fh) const {
	ERR_FAIL_COND_V(!_encode_game_mode_summary(fh, _mode), false);

	ERR_FAIL_V_MSG(false, "DataContainer cannot encode data: not yet implemented");
}
bool DataContainer::_decode_recording(const Ref<FormatHelper> &fh) {
	ERR_FAIL_COND_V(!_decode_game_mode(fh), false);

	_mode_public_name = fh->read_stringvar();
	_mode_public_revision = fh->read_uvarint();
	_selected_variant = fh->read_uvarint();
	ERR_FAIL_INDEX_V(_selected_variant, _mode->get_variants().size(), false);
	_rematches = fh->read_uvarint();

	Ref<VariantDef> selected_variant = _mode->get_variants()[_selected_variant];
	_player_data.resize(selected_variant->get_player_count());
	for (int64_t i = 0; i < selected_variant->get_player_count(); i++) {
		Ref<RecordingPlayerData> player;
		player.instantiate();
		player->set_display_name(fh->read_stringvar());
		player->set_character(static_cast<enums::CharacterDef::Character>(fh->read_uvarint()));
		ERR_FAIL_INDEX_V(player->get_character(), _mode->get_characters().size(), false);

		TypedArray<enums::CardDef::Card> initial_deck;
		initial_deck.resize(fh->read_uvarint());
		for (int64_t j = 0; j < initial_deck.size(); j++) {
			enums::CardDef::Card card = static_cast<enums::CardDef::Card>(fh->read_uvarint());
			ERR_FAIL_COND_V(_mode->get_card(card).is_null(), false);
			initial_deck[j] = card;
		}
		player->set_initial_deck(initial_deck);
		_player_data[i] = player;
	}

	_shared_seed = fh->read_bytesvar();

	_rounds.resize(fh->read_uvarint());
	for (int64_t i = 0; i < _rounds.size(); i++) {
		Ref<RecordingRoundData> round;
		round.instantiate();
		round->set_type(static_cast<RecordingRoundData::RoundType>(fh->read_uvarint()));
		ERR_FAIL_COND_V(round->get_type() != RecordingRoundData::CHOICE && round->get_type() != RecordingRoundData::REALTIME, false);

		round->set_initial_checksum(fh->read_bytesvar());

		TypedArray<RecordingRoundPlayerData> player_data;
		player_data.resize(selected_variant->get_player_count());
		for (int64_t j = 0; j < round->get_player_data().size(); j++) {
			Ref<RecordingRoundPlayerData> player;
			player.instantiate();

			player->set_personal_seed(fh->read_bytesvar());
			switch (round->get_type()) {
			case RecordingRoundData::CHOICE:
			{
				PackedInt64Array chosen_cards;
				chosen_cards.resize(fh->read_uvarint());
				for (int64_t k = 0; k < chosen_cards.size(); k++) {
					chosen_cards[k] = fh->read_uvarint();
				}
				player->set_chosen_cards(chosen_cards);
				break;
			}
			case RecordingRoundData::REALTIME:
			{
				PackedByteArray packed_inputs = fh->read_bytesvar();
				PackedByteArray unpacked_inputs = ButtonInputHistory::unpack_inputs(packed_inputs);
				ERR_FAIL_COND_V(unpacked_inputs.is_empty() && !packed_inputs.is_empty(), false);
				player->set_realtime_inputs(unpacked_inputs);
				player->set_personal_seed_after(fh->read_bytesvar());
				break;
			}
			}

			player_data[j] = player;
		}
		round->set_player_data(player_data);

		switch (round->get_type()) {
		case RecordingRoundData::CHOICE:
			break;
		case RecordingRoundData::REALTIME:
			round->set_final_checksum(fh->read_bytesvar());
			break;
		}

		_rounds[i] = round;
	}

	_final_checksum = fh->read_bytesvar();

	return true;
}
bool DataContainer::_encode_recording(const Ref<FormatHelper> &fh) const {
	ERR_FAIL_COND_V(!_encode_game_mode(fh), false);

	fh->write_stringvar(_mode_public_name);
	fh->write_uvarint(_mode_public_revision);
	ERR_FAIL_INDEX_V(_selected_variant, _mode->get_variants().size(), false);
	fh->write_uvarint(_selected_variant);
	fh->write_uvarint(_rematches);

	Ref<VariantDef> selected_variant = _mode->get_variants()[_selected_variant];
	ERR_FAIL_COND_V(selected_variant->get_player_count() != _player_data.size(), false);
	for (int64_t i = 0; i < _player_data.size(); i++) {
		Ref<RecordingPlayerData> player = _player_data[i];
		fh->write_stringvar(player->get_display_name());
		fh->write_uvarint(player->get_character());
		fh->write_uvarint(player->get_initial_deck().size());
		for (int64_t j = 0; j < player->get_initial_deck().size(); j++) {
			fh->write_uvarint(player->get_initial_deck()[j]);
		}
	}

	fh->write_bytesvar(_shared_seed);

	fh->write_uvarint(_rounds.size());
	for (int64_t i = 0; i < _rounds.size(); i++) {
		Ref<RecordingRoundData> round = _rounds[i];
		fh->write_uvarint(round->get_type());
		fh->write_bytesvar(round->get_initial_checksum());

		ERR_FAIL_COND_V(selected_variant->get_player_count() != round->get_player_data().size(), false);
		for (int64_t j = 0; j < round->get_player_data().size(); j++) {
			Ref<RecordingRoundPlayerData> player = round->get_player_data()[j];
			fh->write_bytesvar(player->get_personal_seed());
			switch (round->get_type()) {
			case RecordingRoundData::CHOICE:
			{
				fh->write_uvarint(player->get_chosen_cards().size());
				for (int64_t k = 0; k < player->get_chosen_cards().size(); k++) {
					fh->write_uvarint(player->get_chosen_cards()[k]);
				}
				break;
			}
			case RecordingRoundData::REALTIME:
			{
				PackedByteArray unpacked_inputs = player->get_realtime_inputs();
				PackedByteArray packed_inputs = ButtonInputHistory::pack_inputs(unpacked_inputs);
				ERR_FAIL_COND_V(packed_inputs.is_empty() && !unpacked_inputs.is_empty(), false);
				fh->write_bytesvar(packed_inputs);
				fh->write_bytesvar(player->get_personal_seed_after());
				break;
			}
			}
		}

		switch (round->get_type()) {
		case RecordingRoundData::CHOICE:
			break;
		case RecordingRoundData::REALTIME:
			fh->write_bytesvar(round->get_final_checksum());
			break;
		}
	}

	fh->write_bytesvar(_final_checksum);

	return true;
}
bool DataContainer::_decode_error_report(const Ref<FormatHelper> &fh) {
	ERR_FAIL_V_MSG(false, "DataContainer cannot decode data: not yet implemented");
}
bool DataContainer::_encode_error_report(const Ref<FormatHelper> &fh) const {
	ERR_FAIL_V_MSG(false, "DataContainer cannot encode data: not yet implemented");
}

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

	data->set_timestamp(fh->read_uvarint());

	int64_t data_file_count = fh->read_uvarint();
	TypedArray<EmbeddedDataFile> data_files;
	data_files.resize(data_file_count);
	LocalVector<PackedByteArray> seen_file_ids;
	for (int64_t i = 0; i < data_file_count; i++) {
		Ref<EmbeddedDataFile> data_file;
		data_file.instantiate();
		data_file->set_file_id(fh->read_bytesvar());
		data_file->set_data(fh->read_bytesvar());
		data_files[i] = data_file;

		ERR_FAIL_COND_V_MSG(seen_file_ids.has(data_file->get_file_id()), Ref<DataContainer>(), "duplicate embedded file ID");
		seen_file_ids.push_back(data_file->get_file_id());

		if (unlikely(!data_file->validate_id())) {
			return Ref<DataContainer>();
		}
	}
	data->set_data_files(data_files);

	switch (data->get_container_type()) {
	case GAME_MODE_SUMMARY:
		data->_mode_summary.instantiate();
		ERR_FAIL_COND_V(!data->_decode_game_mode_summary(fh, data->_mode_summary), Ref<DataContainer>());
		break;
	case GAME_MODE:
		ERR_FAIL_COND_V(!data->_decode_game_mode(fh), Ref<DataContainer>());
		break;
	case RECORDING:
		ERR_FAIL_COND_V(!data->_decode_recording(fh), Ref<DataContainer>());
		break;
	case ERROR_REPORT:
		ERR_FAIL_COND_V(!data->_decode_error_report(fh), Ref<DataContainer>());
		break;
	default:
		ERR_FAIL_V_MSG(Ref<DataContainer>(), vformat("DataContainer cannot decode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", data->get_container_type())));
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

	fh->write_uvarint(_timestamp);

	fh->write_uvarint(_data_files.size());
	LocalVector<PackedByteArray> seen_file_ids;
	for (int64_t i = 0; i < _data_files.size(); i++) {
		Ref<EmbeddedDataFile> data_file = _data_files[i];
		fh->write_bytesvar(data_file->get_file_id());
		fh->write_bytesvar(data_file->get_data());

		ERR_FAIL_COND_V_MSG(seen_file_ids.has(data_file->get_file_id()), PackedByteArray(), "duplicate embedded file ID");
		seen_file_ids.push_back(data_file->get_file_id());

		if (unlikely(!data_file->validate_id())) {
			return PackedByteArray();
		}
	}

	switch (_container_type) {
	case GAME_MODE_SUMMARY:
		ERR_FAIL_COND_V(!_encode_game_mode_summary(fh, _mode_summary), PackedByteArray());
		break;
	case GAME_MODE:
		ERR_FAIL_COND_V(!_encode_game_mode(fh), PackedByteArray());
		break;
	case RECORDING:
		ERR_FAIL_COND_V(!_encode_recording(fh), PackedByteArray());
		break;
	case ERROR_REPORT:
		ERR_FAIL_COND_V(!_encode_error_report(fh), PackedByteArray());
		break;
	default:
		ERR_FAIL_V_MSG(PackedByteArray(), vformat("DataContainer cannot encode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", _container_type)));
		break;
	}

	ERR_FAIL_COND_V(!fh->is_valid(), PackedByteArray());
	return fh->get_buffer();
}
