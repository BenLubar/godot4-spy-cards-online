#include "card_filter.h"

#include "card_instance.h"
#include "jigsaw_context.h"
#include "jigsaw_parameter.h"

void CardFilter::_bind_methods() {
	BIND_ENUM_CONSTANT(AND);
	BIND_ENUM_CONSTANT(OR);

	BIND_ENUM_CONSTANT(CARD);
	BIND_ENUM_CONSTANT(RANK);
	BIND_ENUM_CONSTANT(TRIBE);
	BIND_ENUM_CONSTANT(COST);
	BIND_ENUM_CONSTANT(PARAM);

	ClassDB::bind_method(D_METHOD("get_type"), &CardFilter::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");

	BIND_PROPERTY(Variant::BOOL, negate);

	ClassDB::bind_method(D_METHOD("matches_def", "ctx", "card"), &CardFilter::matches_def);
	ClassDB::bind_method(D_METHOD("matches_instance", "ctx", "inst"), &CardFilter::matches_instance);

	ClassDB::bind_static_method("CardFilter", D_METHOD("make_and", "list"), &CardFilter::make_and);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_or", "list"), &CardFilter::make_or);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_card", "card"), &CardFilter::make_card);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_rank", "rank"), &CardFilter::make_rank);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_tribe", "tribe"), &CardFilter::make_tribe);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_cost", "stat", "amount", "amount_inf"), &CardFilter::make_cost, DEFVAL(int64_t(0)));
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_cost_nan", "stat"), &CardFilter::make_cost_nan);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_param", "param"), &CardFilter::make_param);
}

IMPLEMENT_PROPERTY(CardFilter, bool, negate);

bool CardFilter::matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	bool match = _matches_def(ctx, card);
	if (_negate) {
		match = !match;
	}

	return match;
}

bool CardFilter::matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	bool match = _matches_instance(ctx, inst);
	if (_negate) {
		match = !match;
	}

	return match;
}
