#include "jigsaw/command/jigsaw_command_audio.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_sound_instance.h"

void JigsawCommandAudio::_bind_methods() {
	BIND_ENUM_CONSTANT(CREATE);
	BIND_ENUM_CONSTANT(DESTROY);
	BIND_ENUM_CONSTANT(PLAY);
	BIND_ENUM_CONSTANT(SET_PAUSED);
	BIND_ENUM_CONSTANT(SET_POSITION);
	BIND_ENUM_CONSTANT(SET_VOLUME);
	BIND_ENUM_CONSTANT(SET_PITCH);

	BIND_PROPERTY_ENUM(JigsawCommandAudio::Operation, operation);
	BIND_PROPERTY_ENUM(JigsawSound::Type, mixer);
	BIND_PROPERTY_ENUM(JigsawSound::Space, space);

	BIND_PROPERTY_RESOURCE(JigsawParameter, sound);
	BIND_PROPERTY_RESOURCE(JigsawParameter, paused);
	BIND_PROPERTY_RESOURCE(JigsawParameter, x);
	BIND_PROPERTY_RESOURCE(JigsawParameter, y);
	BIND_PROPERTY_RESOURCE(JigsawParameter, z);
	BIND_PROPERTY_RESOURCE(JigsawParameter, value);
	BIND_PROPERTY_RESOURCE(JigsawParameter, duration);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, created_sound);
}

IMPLEMENT_PROPERTY(JigsawCommandAudio, JigsawCommandAudio::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandAudio, JigsawSound::Type, mixer);
IMPLEMENT_PROPERTY(JigsawCommandAudio, JigsawSound::Space, space);

IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, sound);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, paused);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, x);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, y);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, z);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, value);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameter>, duration);
IMPLEMENT_PROPERTY(JigsawCommandAudio, Ref<JigsawParameterLocalVariable>, created_sound);

bool JigsawCommandAudio::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		return false;
	case INIT:
	case MAIN:
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		return true;
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandAudio::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	JigsawGlobal *global = context->get_global();

	switch (_operation) {
	case CREATE:
	{
		Ref<JigsawParameterAudio> audio;
		err = context->resolve_variable(_sound, audio, "audio");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterBoolean> start_paused;
		err = context->resolve_variable(_paused, start_paused, "start_paused");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterFloat> x, y, z;
		switch (_space) {
		case JigsawSound::GLOBAL:
			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			err = context->resolve_variable(_x, x, "x");
			if (unlikely(err.is_valid())) {
				return JigsawExecutionState::ERROR;
			}
			if (unlikely(!Math::is_finite(x->get_value()))) {
				err = context->create_error("x coordinate is not a number", Array::make(x));
				return JigsawExecutionState::ERROR;
			}

			err = context->resolve_variable(_y, y, "y");
			if (unlikely(err.is_valid())) {
				return JigsawExecutionState::ERROR;
			}
			if (unlikely(!Math::is_finite(y->get_value()))) {
				err = context->create_error("y coordinate is not a number", Array::make(y));
				return JigsawExecutionState::ERROR;
			}

			break;
		}
		switch (_space) {
		case JigsawSound::GLOBAL:
			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			err = context->resolve_variable(_z, z, "z");
			if (unlikely(err.is_valid())) {
				return JigsawExecutionState::ERROR;
			}
			if (unlikely(!Math::is_finite(z->get_value()))) {
				err = context->create_error("z coordinate is not a number", Array::make(z));
				return JigsawExecutionState::ERROR;
			}

			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			z = JigsawParameterFloat::make(0.0);

			break;
		}

		Ref<JigsawSound> sound;
		sound.instantiate();

		int64_t frame = global->get_current_frame();
		if (frame != -1) {
			sound->discard_rollback_data(0);
			sound->set_created_frame(frame);
			sound->set_base_frame(frame);
			sound->set_current_frame(frame);
		}

		sound->set_space(_space);
		sound->set_type(_mixer);
		sound->set_track(audio);
		sound->init_node(global->get_visual());

		switch (_space) {
		case JigsawSound::GLOBAL:
			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			sound->set_position(Vector3(x->get_value(), y->get_value(), z->get_value()), 0.0f, frame);
			break;
		}

		sound->play(0.0f, frame);
		if (start_paused->get_boolean()) {
			sound->pause(frame);
		}

		TypedArray<JigsawSound> sounds = global->get_sounds();
		int64_t sound_index = 0;
		while (sounds.size() > sound_index && sounds[sound_index].get_type() != Variant::NIL) {
			sound_index++;
		}

		if (sound_index == sounds.size()) {
			sounds.append(sound);
		} else {
			sounds[sound_index] = sound;
		}
		global->set_sounds(sounds);

		return set_command_result(context, err, 0, JigsawParameterSoundInstance::make(sound_index));
	}
#define GET_SOUND_INSTANCE \
		Ref<JigsawParameterSoundInstance> sound_index; \
		err = context->resolve_variable(_sound, sound_index, "sound"); \
		if (unlikely(err.is_valid())) { \
			return JigsawExecutionState::ERROR; \
		} \
		if (unlikely(sound_index->get_sound() == -1)) { \
			err = context->create_error("invalid sound instance"); \
			return JigsawExecutionState::ERROR; \
		} \
		Ref<JigsawSound> sound = global->get_sounds()[sound_index->get_sound()]; \
		if (unlikely(sound.is_null() || sound->is_destroyed())) { \
			err = context->create_error("invalid sound instance"); \
			return JigsawExecutionState::ERROR; \
		}
	case DESTROY:
	{
		GET_SOUND_INSTANCE;

		sound->destroy(global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	case PLAY:
	{
		GET_SOUND_INSTANCE;

		Ref<JigsawParameterFloat> seek;
		err = context->resolve_variable(_value, seek, "seek");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (unlikely(!Math::is_finite(seek->get_value()))) {
			err = context->create_error("audio seek position is not a number");
			return JigsawExecutionState::ERROR;
		}

		if (unlikely(seek->get_value() < 0.0)) {
			err = context->create_error("audio seek position is negative");
			return JigsawExecutionState::ERROR;
		}

		sound->play(seek->get_value(), global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	case SET_PAUSED:
	{
		GET_SOUND_INSTANCE;

		Ref<JigsawParameterBoolean> paused;
		err = context->resolve_variable(_paused, paused, "paused");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (paused->get_boolean()) {
			sound->pause(global->get_current_frame());
		} else {
			sound->resume(global->get_current_frame());
		}

		return JigsawExecutionState::CONTINUE;
	}
#define GET_DURATION \
		Ref<JigsawParameterFloat> duration; \
		err = context->resolve_variable(_duration, duration, "duration"); \
		if (unlikely(err.is_valid())) { \
			return JigsawExecutionState::ERROR; \
		} \
		if (unlikely(!Math::is_finite(duration->get_value()))) { \
			err = context->create_error("duration is not a number"); \
			return JigsawExecutionState::ERROR; \
		} \
		if (unlikely(duration->get_value() < 0.0)) { \
			err = context->create_error("duration is negative"); \
			return JigsawExecutionState::ERROR; \
		}
	case SET_POSITION:
	{
		GET_SOUND_INSTANCE;
		GET_DURATION;

		Ref<JigsawParameterFloat> x, y, z;
		err = context->resolve_variable(_x, x, "x");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(x->get_value()))) {
			err = context->create_error("x coordinate is not a number", Array::make(x));
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_y, y, "y");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(y->get_value()))) {
			err = context->create_error("y coordinate is not a number", Array::make(y));
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_z, z, "z");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(z->get_value()))) {
			err = context->create_error("z coordinate is not a number", Array::make(z));
			return JigsawExecutionState::ERROR;
		}

		switch (sound->get_space()) {
		case JigsawSound::GLOBAL:
			err = context->create_error("cannot move a positionless sound");
			return JigsawExecutionState::ERROR;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			if (unlikely(z->get_value() != 0.0)) {
				err = context->create_error("2D sounds must have a z coordinate of 0", Array::make(z));
				return JigsawExecutionState::ERROR;
			}
			break;
		}

		sound->set_position(Vector3(x->get_value(), y->get_value(), z->get_value()), duration->get_value(), global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	case SET_VOLUME:
	{
		GET_SOUND_INSTANCE;
		GET_DURATION;

		Ref<JigsawParameterFloat> volume;
		err = context->resolve_variable(_value, volume, "volume");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(volume->get_value()))) {
			err = context->create_error("volume is not a number", Array::make(volume));
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(volume->get_value() < 0.0 || volume->get_value() > 1.0)) {
			err = context->create_error("volume must be between 0.0 and 1.0", Array::make(volume));
			return JigsawExecutionState::ERROR;
		}

		sound->set_volume(volume->get_value(), duration->get_value(), global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	case SET_PITCH:
	{
		GET_SOUND_INSTANCE;
		GET_DURATION;

		Ref<JigsawParameterFloat> pitch;
		err = context->resolve_variable(_value, pitch, "pitch");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(pitch->get_value()))) {
			err = context->create_error("pitch is not a number", Array::make(pitch));
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(pitch->get_value() < 0.0)) {
			err = context->create_error("pitch is negative", Array::make(pitch));
			return JigsawExecutionState::ERROR;
		}

		sound->set_pitch(pitch->get_value(), duration->get_value(), global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	}

	err = context->create_error(vformat("internal error: unhandled audio operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandAudio", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandAudio::get_num_configs() const {
	switch (_operation) {
	case CREATE:
		return 3;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		return 1;
	}

	ERR_FAIL_V(1);
}
String JigsawCommandAudio::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_configs(), "");

	if (i == 0) {
		return "Operation";
	}

	switch (_operation) {
	case CREATE:
		if (i == 1) {
			return "Mixer";
		} else if (i == 2) {
			return "Space";
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V("");
}
String JigsawCommandAudio::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_configs(), "");

	if (i == 0) {
		switch (_operation) {
		case CREATE:
			return "";
		case DESTROY:
			return "";
		case PLAY:
			return "";
		case SET_PAUSED:
			return "";
		case SET_POSITION:
			return "";
		case SET_VOLUME:
			return "";
		case SET_PITCH:
			return "";
		}
	}

	switch (_operation) {
	case CREATE:
		if (i == 1) {
			switch (_mixer) {
			case JigsawSound::OTHER:
				return "";
			case JigsawSound::CARD_SOUND:
				return "";
			case JigsawSound::MUSIC:
				return "";
			}
		} else if (i == 2) {
			switch (_space) {
			case JigsawSound::GLOBAL:
				return "";
			case JigsawSound::STAGE_3D:
				return "";
			case JigsawSound::HUD_3D:
				return "";
			case JigsawSound::STAGE_2D:
				return "";
			case JigsawSound::HUD_2D:
				return "";
			}
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V("");
}
int64_t JigsawCommandAudio::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_configs(), 0);

	if (i == 0) {
		return _operation;
	}

	switch (_operation) {
	case CREATE:
		if (i == 1) {
			return _mixer;
		} else if (i == 2) {
			return _space;
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V(0);
}
void JigsawCommandAudio::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, get_num_configs());

	if (i == 0) {
		_operation = static_cast<Operation>(value);
		emit_changed();
		return;
	}

	switch (_operation) {
	case CREATE:
		if (i == 1) {
			_mixer = static_cast<JigsawSound::Type>(value);
			emit_changed();
			return;
		} else if (i == 2) {
			_space = static_cast<JigsawSound::Space>(value);
			emit_changed();
			return;
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL();
}
PackedStringArray JigsawCommandAudio::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_configs(), PackedStringArray());

	if (i == 0) {
		// If these asserts fail, you have broken compatibility with existing game modes.

		static_assert(CREATE == 0);
		static_assert(DESTROY == 1);
		static_assert(PLAY == 2);
		static_assert(SET_PAUSED == 3);
		static_assert(SET_POSITION == 4);
		static_assert(SET_VOLUME == 5);
		static_assert(SET_PITCH == 6);

		return PackedStringArray{
			"Create",
			"Destroy",
			"Play",
			"Pause",
			"Move to",
			"Set volume",
			"Set pitch",
		};
	}

	switch (_operation) {
	case CREATE:
		if (i == 1) {
			// If these asserts fail, you have broken compatibility with existing game modes.

			static_assert(JigsawSound::OTHER == 0);
			static_assert(JigsawSound::CARD_SOUND == 1);
			static_assert(JigsawSound::MUSIC == 2);

			return PackedStringArray{
				"Uncategorized",
				"Card sound",
				"Music",
			};
		} else if (i == 2) {
			// If these asserts fail, you have broken compatibility with existing game modes.

			static_assert(JigsawSound::GLOBAL == 0);
			static_assert(JigsawSound::STAGE_3D == 1);
			static_assert(JigsawSound::HUD_3D == 2);
			static_assert(JigsawSound::STAGE_2D == 3);
			static_assert(JigsawSound::HUD_2D == 4);

			return PackedStringArray{
				"Positionless",
				"Stage (3D)",
				"HUD (3D)",
				"Stage (2D)",
				"HUD (2D)",
			};
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V(PackedStringArray());
}

int64_t JigsawCommandAudio::get_num_arguments() const {
	switch (_operation) {
	case CREATE:
		switch (_space) {
		case JigsawSound::GLOBAL:
			return 2;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			return 5;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			return 4;
		}

		break;
	case DESTROY:
		return 1;
	case PLAY:
	case SET_PAUSED:
		return 2;
	case SET_POSITION:
		return 5;
	case SET_VOLUME:
	case SET_PITCH:
		return 3;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameter> JigsawCommandAudio::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		switch (_space) {
		case JigsawSound::GLOBAL:
			if (i == 0) {
				return _sound;
			} else if (i == 1) {
				return _paused;
			}

			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			if (i == 0) {
				return _sound;
			} else if (i == 1) {
				return _paused;
			} else if (i == 2) {
				return _x;
			} else if (i == 3) {
				return _y;
			} else if (i == 4) {
				return _z;
			}

			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			if (i == 0) {
				return _sound;
			} else if (i == 1) {
				return _paused;
			} else if (i == 2) {
				return _x;
			} else if (i == 3) {
				return _y;
			}

			break;
		}

		break;
	case DESTROY:
		if (i == 0) {
			return _sound;
		}

		break;
	case PLAY:
		if (i == 0) {
			return _sound;
		} else if (i == 1) {
			return _value;
		}

		break;
	case SET_PAUSED:
		if (i == 0) {
			return _sound;
		} else if (i == 1) {
			return _paused;
		}

		break;
	case SET_POSITION:
		if (i == 0) {
			return _sound;
		} else if (i == 1) {
			return _x;
		} else if (i == 2) {
			return _y;
		} else if (i == 3) {
			return _z;
		} else if (i == 4) {
			return _duration;
		}

		break;
	case SET_VOLUME:
	case SET_PITCH:
		if (i == 0) {
			return _sound;
		} else if (i == 1) {
			return _value;
		} else if (i == 2) {
			return _duration;
		}

		break;
	}

	ERR_FAIL_V(Ref<JigsawParameter>());
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		switch (_space) {
		case JigsawSound::GLOBAL:
			if (i == 0) {
				return Array::make(
					JigsawParameterFileIDOpus::make(PackedByteArray(), 0.0f, 0.0f),
					JigsawParameterCIDOpus::make(PackedByteArray(), 0.0f, 0.0f)
				);
			} else if (i == 1) {
				return Array::make(JigsawParameterBoolean::make(false));
			}

			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			if (i == 0) {
				return Array::make(
					JigsawParameterFileIDOpus::make(PackedByteArray(), 0.0f, 0.0f),
					JigsawParameterCIDOpus::make(PackedByteArray(), 0.0f, 0.0f)
				);
			} else if (i == 1) {
				return Array::make(JigsawParameterBoolean::make(false));
			} else if (i == 2) {
				return Array::make(JigsawParameterFloat::make(0.0));
			} else if (i == 3) {
				return Array::make(JigsawParameterFloat::make(0.0));
			} else if (i == 4) {
				return Array::make(JigsawParameterFloat::make(0.0));
			}

			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			if (i == 0) {
				return Array::make(
					JigsawParameterFileIDOpus::make(PackedByteArray(), 0.0f, 0.0f),
					JigsawParameterCIDOpus::make(PackedByteArray(), 0.0f, 0.0f)
				);
			} else if (i == 1) {
				return Array::make(JigsawParameterBoolean::make(false));
			} else if (i == 2) {
				return Array::make(JigsawParameterFloat::make(0.0));
			} else if (i == 3) {
				return Array::make(JigsawParameterFloat::make(0.0));
			}

			break;
		}

		break;
	case DESTROY:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		}

		break;
	case PLAY:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	case SET_PAUSED:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		} else if (i == 1) {
			return Array::make(JigsawParameterBoolean::make(true));
		}

		break;
	case SET_POSITION:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 3) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 4) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	case SET_VOLUME:
	case SET_PITCH:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(1.0));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandAudio::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	switch (_operation) {
	case CREATE:
		switch (_space) {
		case JigsawSound::GLOBAL:
			if (i == 0) {
				_sound = arg;
				emit_changed();
				return;
			} else if (i == 1) {
				_paused = arg;
				emit_changed();
				return;
			}

			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			if (i == 0) {
				_sound = arg;
				emit_changed();
				return;
			} else if (i == 1) {
				_paused = arg;
				emit_changed();
				return;
			} else if (i == 2) {
				_x = arg;
				emit_changed();
				return;
			} else if (i == 3) {
				_y = arg;
				emit_changed();
				return;
			} else if (i == 4) {
				_z = arg;
				emit_changed();
				return;
			}

			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			if (i == 0) {
				_sound = arg;
				emit_changed();
				return;
			} else if (i == 1) {
				_paused = arg;
				emit_changed();
				return;
			} else if (i == 2) {
				_x = arg;
				emit_changed();
				return;
			} else if (i == 3) {
				_y = arg;
				emit_changed();
				return;
			}

			break;
		}

		break;
	case DESTROY:
		if (i == 0) {
			_sound = arg;
			emit_changed();
			return;
		}

		break;
	case PLAY:
		if (i == 0) {
			_sound = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_value = arg;
			emit_changed();
			return;
		}

		break;
	case SET_PAUSED:
		if (i == 0) {
			_sound = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_paused = arg;
			emit_changed();
			return;
		}

		break;
	case SET_POSITION:
		if (i == 0) {
			_sound = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_x = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_y = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_z = arg;
			emit_changed();
			return;
		} else if (i == 4) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	case SET_VOLUME:
	case SET_PITCH:
		if (i == 0) {
			_sound = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_value = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	}

	ERR_FAIL();
}
String JigsawCommandAudio::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	switch (_operation) {
	case CREATE:
		switch (_space) {
		case JigsawSound::GLOBAL:
			if (i == 0) {
				return "audio";
			} else if (i == 1) {
				return "start_paused";
			}

			break;
		case JigsawSound::STAGE_3D:
		case JigsawSound::HUD_3D:
			if (i == 0) {
				return "audio";
			} else if (i == 1) {
				return "start_paused";
			} else if (i == 2) {
				return "x";
			} else if (i == 3) {
				return "y";
			} else if (i == 4) {
				return "z";
			}

			break;
		case JigsawSound::STAGE_2D:
		case JigsawSound::HUD_2D:
			if (i == 0) {
				return "audio";
			} else if (i == 1) {
				return "start_paused";
			} else if (i == 2) {
				return "x";
			} else if (i == 3) {
				return "y";
			}

			break;
		}

		break;
	case DESTROY:
		if (i == 0) {
			return "sound";
		}

		break;
	case PLAY:
		if (i == 0) {
			return "sound";
		} else if (i == 1) {
			return "seek";
		}

		break;
	case SET_PAUSED:
		if (i == 0) {
			return "sound";
		} else if (i == 1) {
			return "paused";
		}

		break;
	case SET_POSITION:
		if (i == 0) {
			return "sound";
		} else if (i == 1) {
			return "x";
		} else if (i == 2) {
			return "y";
		} else if (i == 3) {
			return "z";
		} else if (i == 4) {
			return "duration";
		}

		break;
	case SET_VOLUME:
		if (i == 0) {
			return "sound";
		} else if (i == 1) {
			return "volume";
		} else if (i == 2) {
			return "duration";
		}

		break;
	case SET_PITCH:
		if (i == 0) {
			return "sound";
		} else if (i == 1) {
			return "pitch";
		} else if (i == 2) {
			return "duration";
		}

		break;
	}

	ERR_FAIL_V("");
}

int64_t JigsawCommandAudio::get_num_results() const {
	switch (_operation) {
	case CREATE:
		return 1;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		return 0;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameterLocalVariable> JigsawCommandAudio::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return _created_sound;
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V(Ref<JigsawParameterLocalVariable>());
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return Array::make(JigsawParameterSoundInstance::make(-1));
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandAudio::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			_created_sound = result;
			emit_changed();
			return;
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL();
}
String JigsawCommandAudio::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return "sound";
		}

		break;
	case DESTROY:
	case PLAY:
	case SET_PAUSED:
	case SET_POSITION:
	case SET_VOLUME:
	case SET_PITCH:
		break;
	}

	ERR_FAIL_V("");
}
