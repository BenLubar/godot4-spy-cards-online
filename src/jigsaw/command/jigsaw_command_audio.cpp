#include "jigsaw_command_audio.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/jigsaw_visual.h"

void JigsawCommandAudio::_bind_methods() {
	BIND_ENUM_CONSTANT(SET_MUSIC);

	BIND_PROPERTY_ENUM(JigsawCommandAudio::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, sound);
}

IMPLEMENT_PROPERTY(JigsawCommandAudio, JigsawCommandAudio::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, sound);

bool JigsawCommandAudio::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
	case LOGIC:
		return false;
	case VISUAL:
	case INIT:
	case MAIN:
		return true;
	case CHOICE_SELECT:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
		return false;
	case REALTIME_VISUAL:
		return true;
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandAudio::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case SET_MUSIC:
	{
		Ref<JigsawParameterAudio> audio;
		err = context->resolve_variable(_sound, audio, "sound");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		JigsawGlobal *global = context->get_global();
		JigsawVisual *visual = likely(global) ? global->get_visual() : nullptr;
		if (likely(visual)) {
			visual->set_music(audio);
		}

		return JigsawExecutionState::CONTINUE;
	}
	}

	return JigsawExecutionState::CONTINUE;
}

int64_t JigsawCommandAudio::get_num_configs() const {
	return 1;
}
String JigsawCommandAudio::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandAudio::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandAudio::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandAudio::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandAudio::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(SET_MUSIC == 0);

	return PackedStringArray{
		"Set music",
	};
}

int64_t JigsawCommandAudio::get_num_arguments() const {
	switch (_operation) {
	case SET_MUSIC:
		return 1;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandAudio::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	if (i == 0) {
		return _sound;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	if (i == 0) {
		return Array::make(
			JigsawParameterCIDOpus::make(PackedByteArray(), 0.0, 0.0),
			JigsawParameterFileIDOpus::make(PackedByteArray(), 0.0, 0.0)
		);
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandAudio::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	if (i == 0) {
		_sound = arg;
		emit_changed();
	}
}
String JigsawCommandAudio::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	if (i == 0) {
		return "sound";
	}

	return "";
}

int64_t JigsawCommandAudio::get_num_results() const {
	switch (_operation) {
	case SET_MUSIC:
		return 0;
	}

	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandAudio::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return Ref<JigsawParameterLocalVariable>();
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	return TypedArray<JigsawParameter>();
}
void JigsawCommandAudio::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());
}
String JigsawCommandAudio::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	return "";
}
