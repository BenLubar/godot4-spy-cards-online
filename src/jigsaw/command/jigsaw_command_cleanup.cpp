#include "jigsaw/command/jigsaw_command_cleanup.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_card_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_modifier_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"

#include "defs/card_filter_and.h"
#include "defs/card_filter_or.h"
#include "defs/card_filter_param.h"

void JigsawCommandCleanup::_bind_methods() {
	BIND_ENUM_CONSTANT(GC_CARD_INSTANCES);

	BIND_PROPERTY_ENUM(JigsawCommandCleanup::Operation, operation);
}

IMPLEMENT_PROPERTY(JigsawCommandCleanup, JigsawCommandCleanup::Operation, operation);

JigsawCommand::Type JigsawCommandCleanup::get_type() const {
	return CLEANUP;
}

bool JigsawCommandCleanup::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		return false;
	case INIT:
	case MAIN:
		return true;
	case SELECT:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		return false;
	}

	ERR_FAIL_V(false);
}
template<typename TArray, typename TParam>
static void collect_jigsaw_parameter_refs(const TypedArray<TArray> &array, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	for (int64_t i = 0; i < array.size(); i++) {
		Ref<TArray> ref = array[i];
		if (ref.is_valid()) {
			collect_jigsaw_parameter_refs(ref, ref_count, get_id);
		}
	}
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<JigsawParameter> &param, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	Ref<TParam> goal = param;
	if (goal.is_valid()) {
		int64_t id = get_id(goal);
		if (id != -1) {
			ref_count[id]++;
		}
	}

	Ref<JigsawParameterCardFilter> filter = param;
	if (filter.is_valid()) {
		collect_jigsaw_parameter_refs(filter->get_filter(), ref_count, get_id);
	}

	Ref<JigsawParameterOrderedList> list = param;
	if (list.is_valid()) {
		collect_jigsaw_parameter_refs(list->get_list(), ref_count, get_id);
	}
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<CardFilter> &filter, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	switch (filter->get_type()) {
	case CardFilter::AND:
		collect_jigsaw_parameter_refs(Ref<CardFilterAnd>(filter)->get_list(), ref_count, get_id);
		break;
	case CardFilter::OR:
		collect_jigsaw_parameter_refs(Ref<CardFilterOr>(filter)->get_list(), ref_count, get_id);
		break;
	case CardFilter::PARAM:
		if (Ref<CardFilterParam>(filter)->get_param().is_valid() && Ref<CardFilterParam>(filter)->get_param()->get_specifier().is_valid()) {
			collect_jigsaw_parameter_refs(Ref<CardFilterParam>(filter)->get_param()->get_specifier(), ref_count, get_id);
		}
		break;
	default:
		break;
	}
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const VariableParameterDict &dict, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(TypedArray<JigsawParameter>(dict.values()), ref_count, get_id);
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<QueuedEffect> &queue, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(queue->get_variables(), ref_count, get_id);
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<CardInstance> &card, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	// TODO
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<JigsawState> &state, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(state->get_cards(), ref_count, get_id);
	collect_jigsaw_parameter_refs(state->get_sides(), ref_count, get_id);
	collect_jigsaw_parameter_refs(state->get_variables(), ref_count, get_id);
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<JigsawSide> &side, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(side->get_variables(), ref_count, get_id);
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<JigsawStackFrame> &frame, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	// TODO
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(const Ref<JigsawContext> &ctx, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(ctx->get_arguments(), ref_count, get_id);
	collect_jigsaw_parameter_refs(ctx->get_results(), ref_count, get_id);
	collect_jigsaw_parameter_refs(ctx->get_stack(), ref_count, get_id);
}
template<typename TParam>
static void collect_jigsaw_parameter_refs(JigsawGlobal *global, PackedInt32Array &ref_count, int64_t (*get_id)(const Ref<TParam> &param)) {
	collect_jigsaw_parameter_refs(global->get_queue(), ref_count, get_id);
	collect_jigsaw_parameter_refs(global->get_state(), ref_count, get_id);
	collect_jigsaw_parameter_refs(global->get_context_stack(), ref_count, get_id);
}
JigsawExecutionState JigsawCommandCleanup::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	JigsawGlobal *global = context->get_global();

	PackedInt32Array ref_count;
	switch (_operation) {
	case GC_CARD_INSTANCES:
	{
		Ref<JigsawState> state = global->get_state();
		TypedArray<CardInstance> cards = state->get_cards();
		ref_count.resize(cards.size());

		for (int64_t i = 0; i < cards.size(); i++) {
			Ref<CardInstance> card = cards[i];
			if (card.is_null()) {
				continue;
			}

			int64_t linked_parent = card->get_linked_parent();
			if (linked_parent != -1) {
				ref_count[linked_parent]++;
			}
		}

		TypedArray<JigsawSide> sides = state->get_sides();
		for (int64_t i = 0; i < sides.size(); i++) {
			Ref<JigsawSide> side = sides[i];
			TypedArray<PackedInt32Array> locations = side->get_location_card_instances();
			for (int64_t j = 0; j < locations.size(); j++) {
				PackedInt32Array location = locations[j];
				for (int64_t k = 0; k < location.size(); k++) {
					ref_count[location[k]]++;
				}
			}
		}

		TypedArray<QueuedEffect> effects = global->get_queue();
		for (int64_t i = 0; i < effects.size(); i++) {
			Ref<QueuedEffect> effect = effects[i];
			if (effect->get_card_instance() != -1) {
				ref_count[effect->get_card_instance()]++;
			}
		}

		collect_jigsaw_parameter_refs<JigsawParameterCardInstance>(global, ref_count, [](const Ref<JigsawParameterCardInstance> &param) -> int64_t {
			return param->get_instance();
		});
		collect_jigsaw_parameter_refs<JigsawParameterModifierInstance>(global, ref_count, [](const Ref<JigsawParameterModifierInstance> &param) -> int64_t {
			return param->get_card_instance();
		});

		for (int64_t i = 0; i < cards.size(); i++) {
			Ref<CardInstance> card = cards[i];
			if (card.is_null()) {
				CRASH_COND_MSG(ref_count[i] != 0, "referenced card instance was destroyed on a previous GC iteration (or corrupted reference)");
				continue;
			}

			if (ref_count[i] == 0) {
				cards[i] = Ref<CardInstance>();

				int64_t linked_parent = card->get_linked_parent();
				while (linked_parent != -1 && ref_count[linked_parent] == 1) {
					// special case: allow linked parent deref to cascade
					ref_count[linked_parent]--;

					Ref<CardInstance> parent_card = cards[linked_parent];
					cards[linked_parent] = Ref<CardInstance>();
					linked_parent = parent_card->get_linked_parent();
				}
			}
		}

		break;
	}
	}

	return JigsawExecutionState::CONTINUE;
}

int64_t JigsawCommandCleanup::get_num_configs() const {
	return 1;
}
String JigsawCommandCleanup::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, String());

	return "Type";
}
String JigsawCommandCleanup::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, String());

	switch (_operation) {
	case GC_CARD_INSTANCES:
		return ""; // TODO
	}

	ERR_FAIL_V(String());
}
int64_t JigsawCommandCleanup::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandCleanup::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	set_operation(static_cast<Operation>(value));
}
PackedStringArray JigsawCommandCleanup::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	static_assert(GC_CARD_INSTANCES == 0);

	return PackedStringArray{
		"Card Instances",
	};
}
