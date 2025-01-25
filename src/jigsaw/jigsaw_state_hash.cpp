#include "jigsaw/jigsaw_state.h"

#include "active/card_instance.h"
#include "protocol/data_container.h"
#include "util/format_helper.h"

static void write_variant_hash(const Ref<HashingContext> &hash, const Variant &v) {
	hash->update(UtilityFunctions::var_to_bytes(v));
}

template<typename T>
static void write_array_hash(const Ref<HashingContext> &hash, const TypedArray<T> &array) {
	write_variant_hash(hash, array.size());

	for (int64_t i = 0; i < array.size(); i++) {
		const Ref<T> item = array[i];
		if (likely(item.is_valid())) {
			hash->update(PackedByteArray{1});
			item->write_hash(hash);
		} else {
			hash->update(PackedByteArray{0});
		}
	}
}

static void write_variables_hash(const Ref<HashingContext> &hash, const VariableParameterDict &variables) {
	Ref<FormatHelper> fh = FormatHelper::write("hash");

	PackedInt64Array keys = variables.keys();
	write_variant_hash(hash, keys);

	Ref<DataContainer> container;
	container.instantiate();
	TypedArray<JigsawParameter> values = variables.values();
	for (int64_t i = 0; i < values.size(); i++) {
		container->_encode_jigsaw_parameter(fh, values[i]);
	}

	hash->update(fh->get_buffer());
}

void JigsawState::write_hash(const Ref<HashingContext> &hash) const {
	write_array_hash(hash, _cards);
	write_array_hash(hash, _sides);
	write_variables_hash(hash, _variables);
	_audience->write_hash(hash);
}

void Audience::write_hash(const Ref<HashingContext> &hash) const {
	PackedByteArray member_data;
	member_data.resize(35);

	write_variant_hash(hash, _members.size());
	for (int64_t i = 0; i < _members.size(); i++) {
		const Vector<AudienceMember_t> &group = _members[i];
		write_variant_hash(hash, group.size());

		for (int64_t j = 0; j < group.size(); j++) {
			const AudienceMember_t &member = group[j];

			member_data.encode_float(0, member.base_position.x);
			member_data.encode_float(4, member.base_position.y);
			member_data.encode_float(8, member.base_position.z);
			member_data.encode_float(12, member.color.r);
			member_data.encode_float(16, member.color.g);
			member_data.encode_float(20, member.color.b);
			member_data.encode_float(24, member.color.a);
			member_data.encode_float(28, member.excitement);
			member_data[32] = member.left ? 1 : 0;
			member_data[33] = member.flip ? 1 : 0;
			member_data[34] = member.deleted ? 1 : 0;

			hash->update(member_data);
		}
	}
}

void CardInstance::write_hash(const Ref<HashingContext> &hash) const {
	write_variant_hash(hash, _index);
	write_variant_hash(hash, _def.is_null() ? enums::CardDef::NONE : _def->get_id());
	// TODO: name?
	write_variant_hash(hash, _rank);
	write_variant_hash(hash, _back);
	// TODO: costs
	write_variant_hash(hash, _portrait);
	// TODO: effects?
	write_variant_hash(hash, _tribes);
	// TODO: description?
	// TODO: simple_description?
	// TODO: modifiers
	write_variant_hash(hash, _face_down_for_side);
	write_variant_hash(hash, _linked_parent);
}

void JigsawSide::write_hash(const Ref<HashingContext> &hash) const {
	write_variant_hash(hash, _player_name);
	write_variant_hash(hash, _character);
	write_variant_hash(hash, _initial_deck);

	write_variant_hash(hash, _location_card_instances);

	write_variant_hash(hash, _stats.size());

	PackedByteArray stat_buffer;
	stat_buffer.resize(17);
	for (int64_t i = 0; i < _stats.size(); i++) {
		Ref<NumericValue> stat_value = _stats[i];

		stat_buffer.encode_u64(0, stat_value->get_amount());
		stat_buffer.encode_u64(8, stat_value->get_amount_inf());
		stat_buffer[16] = stat_value->is_nan() ? 1 : 0;

		hash->update(stat_buffer);
	}

	write_variables_hash(hash, _variables);
}
