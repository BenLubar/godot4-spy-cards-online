#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include "dry.h"

class DataContainer;

#include "defs/game_mode_summary.h"
#include "defs/game_mode.h"
#include "protocol/embedded_data_file.h"
#include "protocol/recording_player_data.h"
#include "protocol/recording_round_data.h"
#include "util/format_helper.h"
#include "defs/card_filter.h"

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
		GAME_MODE_SUMMARY = 0,
		GAME_MODE = 1,
		RECORDING = 2,
		ERROR_REPORT = 3,
	};

	// header
	DECLARE_PROPERTY(FormatRevision, format_revision, = FORMAT_0);
	DECLARE_PROPERTY(ContainerType, container_type, = GAME_MODE);
	DECLARE_PROPERTY(Vector3i, game_version);

	// shared
	DECLARE_PROPERTY(uint64_t, timestamp, = 0);
	DECLARE_PROPERTY(TypedArray<EmbeddedDataFile>, data_files);

	// game mode
	DECLARE_PROPERTY(Ref<GameModeSummary>, mode_summary);
	DECLARE_PROPERTY(Ref<GameMode>, mode);

	// recording
	DECLARE_PROPERTY(String, mode_public_name);
	DECLARE_PROPERTY(int64_t, mode_public_revision, = 0);
	DECLARE_PROPERTY(int64_t, selected_variant, = 0);
	DECLARE_PROPERTY(int64_t, rematches, = 0);
	DECLARE_PROPERTY(PackedInt64Array, previous_wins);
	DECLARE_PROPERTY(PackedByteArray, resumed_from_recording);
	DECLARE_PROPERTY(int64_t, resumed_from_round, = -1);
	DECLARE_PROPERTY(PackedArray<enums::NPCDef::NPC>, player_npc);
	DECLARE_PROPERTY(TypedArray<RecordingPlayerData>, player_data);
	DECLARE_PROPERTY(PackedByteArray, shared_seed);
	DECLARE_PROPERTY(TypedArray<RecordingRoundData>, rounds);
	DECLARE_PROPERTY(PackedByteArray, final_checksum);

private:
	bool _validate_file_id(const PackedByteArray &file_id, uint64_t remote_type, uint64_t local_type) const;
	bool _decode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary);
	bool _encode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) const;
	bool _decode_game_mode(const Ref<FormatHelper> &fh);
	bool _encode_game_mode(const Ref<FormatHelper> &fh) const;
	bool _decode_recording(const Ref<FormatHelper> &fh);
	bool _encode_recording(const Ref<FormatHelper> &fh) const;
	bool _decode_error_report(const Ref<FormatHelper> &fh);
	bool _encode_error_report(const Ref<FormatHelper> &fh) const;

	// data_container_defs.cpp
	bool _decode_audience_def(const Ref<FormatHelper> &fh, const Ref<AudienceDef> &audience_def);
	bool _encode_audience_def(const Ref<FormatHelper> &fh, const Ref<AudienceDef> &audience_def) const;
	bool _decode_card_def(const Ref<FormatHelper> &fh, const Ref<CardDef> &card_def);
	bool _encode_card_def(const Ref<FormatHelper> &fh, const Ref<CardDef> &card_def) const;
	bool _decode_card_design(const Ref<FormatHelper> &fh, const Ref<CardDesign> &card_design);
	bool _encode_card_design(const Ref<FormatHelper> &fh, const Ref<CardDesign> &card_design) const;
	bool _decode_card_filter(const Ref<FormatHelper> &fh, Ref<CardFilter> &card_filter);
	bool _encode_card_filter(const Ref<FormatHelper> &fh, const Ref<CardFilter> &card_filter) const;
	bool _decode_character_def(const Ref<FormatHelper> &fh, const Ref<CharacterDef> &character_def);
	bool _encode_character_def(const Ref<FormatHelper> &fh, const Ref<CharacterDef> &character_def) const;
	bool _decode_choices_def(const Ref<FormatHelper> &fh, const Ref<ChoicesDef> &choices_def);
	bool _encode_choices_def(const Ref<FormatHelper> &fh, const Ref<ChoicesDef> &choices_def) const;
	bool _decode_effect_def(const Ref<FormatHelper> &fh, const Ref<EffectDef> &effect_def);
	bool _encode_effect_def(const Ref<FormatHelper> &fh, const Ref<EffectDef> &effect_def) const;
	bool _decode_effect_instance(const Ref<FormatHelper> &fh, const Ref<EffectInstance> &effect_instance);
	bool _encode_effect_instance(const Ref<FormatHelper> &fh, const Ref<EffectInstance> &effect_instance) const;
	bool _decode_location_def(const Ref<FormatHelper> &fh, const Ref<LocationDef> &location_def);
	bool _encode_location_def(const Ref<FormatHelper> &fh, const Ref<LocationDef> &location_def) const;
	bool _decode_modifier_def(const Ref<FormatHelper> &fh, const Ref<ModifierDef> &modifier_def);
	bool _encode_modifier_def(const Ref<FormatHelper> &fh, const Ref<ModifierDef> &modifier_def) const;
	bool _decode_npc_def(const Ref<FormatHelper> &fh, const Ref<NPCDef> &npc_def);
	bool _encode_npc_def(const Ref<FormatHelper> &fh, const Ref<NPCDef> &npc_def) const;
	bool _decode_rank_def(const Ref<FormatHelper> &fh, const Ref<RankDef> &rank_def);
	bool _encode_rank_def(const Ref<FormatHelper> &fh, const Ref<RankDef> &rank_def) const;
	bool _decode_stat_def(const Ref<FormatHelper> &fh, const Ref<StatDef> &stat_def);
	bool _encode_stat_def(const Ref<FormatHelper> &fh, const Ref<StatDef> &stat_def) const;
	bool _decode_sticker_def(const Ref<FormatHelper> &fh, const Ref<StickerDef> &sticker_def);
	bool _encode_sticker_def(const Ref<FormatHelper> &fh, const Ref<StickerDef> &sticker_def) const;
	bool _decode_tribe_def(const Ref<FormatHelper> &fh, const Ref<TribeDef> &tribe_def);
	bool _encode_tribe_def(const Ref<FormatHelper> &fh, const Ref<TribeDef> &tribe_def) const;
	bool _decode_variable_def(const Ref<FormatHelper> &fh, const Ref<VariableDef> &variable_def);
	bool _encode_variable_def(const Ref<FormatHelper> &fh, const Ref<VariableDef> &variable_def) const;
	bool _decode_variant_def(const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant_def);
	bool _encode_variant_def(const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant_def) const;

	// data_container_variant.cpp
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, bool &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const bool &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, PackedByteArray &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const PackedByteArray &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, String &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const String &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, PackedStringArray &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const PackedStringArray &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, double &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const double &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, float &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const float &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Vector2 &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Vector2 &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Vector3 &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Vector3 &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Vector4 &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Vector4 &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Color &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Color &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Transform2D &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Transform2D &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Transform3D &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Transform3D &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Basis &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Basis &value) const;
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, Rect2 &value);
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const Rect2 &value) const;
	template<typename E>
	std::enable_if_t<std::is_enum_v<E> && E::NONE == -1, bool> _decode_godot_variant(const Ref<FormatHelper> &fh, E &value);
	template<typename E>
	std::enable_if_t<std::is_enum_v<E> && E::NONE == -1, bool> _encode_godot_variant(const Ref<FormatHelper> &fh, const E &value) const;
	template<typename T>
	bool _decode_godot_variant(const Ref<FormatHelper> &fh, BitField<T> &value);
	template<typename T>
	bool _encode_godot_variant(const Ref<FormatHelper> &fh, const BitField<T> &value) const;

	// data_container_jigsaw.cpp
	bool _decode_jigsaw_function(const Ref<FormatHelper> &fh, const Ref<JigsawFunction> &function);
	bool _encode_jigsaw_function(const Ref<FormatHelper> &fh, const Ref<JigsawFunction> &function) const;
	bool _decode_jigsaw_trigger_effect(const Ref<FormatHelper> &fh, Ref<JigsawTriggerEffect> &trigger);
	bool _encode_jigsaw_trigger_effect(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerEffect> &trigger) const;
	bool _decode_jigsaw_trigger_modifier(const Ref<FormatHelper> &fh, Ref<JigsawTriggerModifier> &trigger);
	bool _encode_jigsaw_trigger_modifier(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerModifier> &trigger) const;
	bool _decode_jigsaw_trigger_npc(const Ref<FormatHelper> &fh, Ref<JigsawTriggerNPC> &trigger);
	bool _encode_jigsaw_trigger_npc(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerNPC> &trigger) const;
	bool _decode_jigsaw_trigger_variant(const Ref<FormatHelper> &fh, Ref<JigsawTriggerVariant> &trigger);
	bool _encode_jigsaw_trigger_variant(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerVariant> &trigger) const;
	bool _decode_jigsaw_procedure(const Ref<FormatHelper> &fh, const Ref<JigsawProcedure> &procedure);
	bool _encode_jigsaw_procedure(const Ref<FormatHelper> &fh, const Ref<JigsawProcedure> &procedure) const;
	bool _decode_jigsaw_command_list(const Ref<FormatHelper> &fh, Ref<JigsawCommandList> &commands);
	bool _encode_jigsaw_command_list(const Ref<FormatHelper> &fh, const Ref<JigsawCommandList> &commands) const;
	bool _decode_jigsaw_command(const Ref<FormatHelper> &fh, Ref<JigsawCommand> &command);
	bool _encode_jigsaw_command(const Ref<FormatHelper> &fh, const Ref<JigsawCommand> &command) const;
	bool _decode_jigsaw_parameter(const Ref<FormatHelper> &fh, Ref<JigsawParameter> &parameter);
	bool _encode_jigsaw_parameter(const Ref<FormatHelper> &fh, const Ref<JigsawParameter> &parameter) const;

public:
	static Ref<DataContainer> from_byte_array(const PackedByteArray &buf, const String &debug_name = "data container");
	PackedByteArray to_byte_array(const String &debug_name = "data container") const;
	TypedArray<JigsawError> validate() const;

	DEFAULT_TO_STRING();
};
DECLARE_ENUM(DataContainer::FormatRevision);
DECLARE_ENUM(DataContainer::ContainerType);

// used in multiple data_container_*.cpp files:
#define DECODE_VARIANT(m_object, m_property) \
	{ \
		decltype(m_object->get_##m_property()) value; \
		ERR_FAIL_COND_V(!_decode_godot_variant(fh, value), false); \
		m_object->set_##m_property(value); \
	}
#define DECODE_VARIANT_IS(m_object, m_property) \
	{ \
		decltype(m_object->is_##m_property()) value; \
		ERR_FAIL_COND_V(!_decode_godot_variant(fh, value), false); \
		m_object->set_##m_property(value); \
	}
#define ENCODE_VARIANT(m_object, m_property) \
	ERR_FAIL_COND_V(!_encode_godot_variant(fh, m_object->get_##m_property()), false)
#define ENCODE_VARIANT_IS(m_object, m_property) \
	ERR_FAIL_COND_V(!_encode_godot_variant(fh, m_object->is_##m_property()), false)
template<typename T>
static inline bool _decode_amount(const Ref<FormatHelper> &fh, const Ref<T> &value) {
	value->set_amount(fh->read_svarint());
	value->set_amount_inf(fh->read_svarint());
	value->set_nan(unlikely(value->get_amount() == 0 && value->get_amount_inf() == 0) && fh->read_bool());

	return true;
}
template<typename T>
static inline bool _encode_amount(const Ref<FormatHelper> &fh, const Ref<T> &value) {
	if (unlikely(value->is_nan())) {
		fh->write_svarint(0);
		fh->write_svarint(0);
		fh->write_bool(true);
		return true;
	}

	fh->write_svarint(value->get_amount());
	fh->write_svarint(value->get_amount_inf());
	if (unlikely(value->get_amount() == 0 && value->get_amount_inf() == 0)) {
		fh->write_bool(false);
	}

	return true;
}

// template, so it needs to be in a header
template<typename E>
inline std::enable_if_t<std::is_enum_v<E> && E::NONE == -1, bool> DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, E &value) {
	value = fh->read_id<E>();
	return fh->is_valid();
}
template<typename E>
inline std::enable_if_t<std::is_enum_v<E> && E::NONE == -1, bool> DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const E &value) const {
	fh->write_id(value);
	return fh->is_valid();
}
template<typename T>
inline bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, BitField<T> &value) {
	value = fh->read_uvarint();
	return fh->is_valid();
}
template<typename T>
inline bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const BitField<T> &value) const {
	fh->write_uvarint(value);
	return fh->is_valid();
}

#endif // DATA_CONTAINER_H
