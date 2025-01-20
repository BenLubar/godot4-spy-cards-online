#ifndef JIGSAW_GLOBAL_H
#define JIGSAW_GLOBAL_H

#include "dry.h"

#include <godot_cpp/classes/node.hpp>

class JigsawGlobal;
class JigsawVisual;

#include "active/card_instance.h"
#include "active/queued_effect.h"
#include "defs/effect_instance.h"
#include "defs/game_mode.h"
#include "defs/variant_def.h"
#include "jigsaw/jigsaw_context.h"
#include "jigsaw/jigsaw_input_source.h"
#include "jigsaw/jigsaw_state.h"
#include "jigsaw/presentation/jigsaw_audio.h"
#include "util/rng.h"

class JigsawGlobal : public Node {
	GDCLASS(JigsawGlobal, Node);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(JigsawInputSource *, input_source, = nullptr);
	DECLARE_PROPERTY(JigsawVisual *, visual, = nullptr);
	DECLARE_PROPERTY(Ref<GameMode>, mode);
	DECLARE_PROPERTY(Ref<VariantDef>, selected_variant);

	DECLARE_PROPERTY(Ref<CardInstance>, current_card_instance);
	DECLARE_PROPERTY(Ref<EffectInstance>, current_effect_instance);
	DECLARE_PROPERTY(Ref<QueuedEffect>, current_queued_effect);
	DECLARE_PROPERTY(int32_t, current_side, = 0);

	DECLARE_PROPERTY(Ref<JigsawState>, state);

	DECLARE_PROPERTY(double, time_scale, = 1.0);
	DECLARE_PROPERTY(uint32_t, pause_frames, = 0);
	DECLARE_PROPERTY(TypedArray<JigsawContext>, context_stack);

	DECLARE_PROPERTY(int64_t, queue_reset_count, = 0);
	DECLARE_PROPERTY(TypedArray<QueuedEffect>, queue);

	DECLARE_PROPERTY(TypedArray<JigsawSound>, sounds);

	~JigsawGlobal();

	void init_sides();
	Ref<JigsawError> run_variant_triggers(JigsawTriggerVariant::Type type, const TypedArray<JigsawParameter> &args, const Ref<RNG> &rng, bool copy_rng);

	DEFAULT_TO_STRING();
};

#endif // JIGSAW_GLOBAL_H
