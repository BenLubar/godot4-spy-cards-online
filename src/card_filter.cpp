#include "card_filter.h"

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

	GDVIRTUAL_BIND(_matches_def, "global", "card");
	GDVIRTUAL_BIND(_matches_instance, "global", "instance");

	ClassDB::bind_static_method("CardFilter", D_METHOD("make_and", "list"), &CardFilter::make_and);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_or", "list"), &CardFilter::make_or);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_card", "card"), &CardFilter::make_card);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_rank", "rank"), &CardFilter::make_rank);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_tribe", "tribe"), &CardFilter::make_tribe);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_cost", "stat", "amount"), &CardFilter::make_cost);
	ClassDB::bind_static_method("CardFilter", D_METHOD("make_param", "param"), &CardFilter::make_param);
}

IMPLEMENT_PROPERTY(CardFilter, bool, negate);
