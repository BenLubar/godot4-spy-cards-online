#ifndef JIGSAW_COMMAND_AUDIO_H
#define JIGSAW_COMMAND_AUDIO_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandAudio;

#include "jigsaw/presentation/jigsaw_audio.h"

class JigsawCommandAudio : public JigsawCommand {
	GDCLASS(JigsawCommandAudio, JigsawCommand);

public:
	enum Operation {
		CREATE = 0,
		DESTROY = 1,
		PLAY = 2,
		SET_PAUSED = 3,
		SET_POSITION = 4,
		SET_VOLUME = 5,
		SET_PITCH = 6,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Operation, operation, = CREATE);
	DECLARE_PROPERTY(JigsawSound::Type, mixer, = JigsawSound::OTHER);
	DECLARE_PROPERTY(JigsawSound::Space, space, = JigsawSound::GLOBAL);

	DECLARE_PROPERTY(Ref<JigsawParameter>, sound);
	DECLARE_PROPERTY(Ref<JigsawParameter>, paused);
	DECLARE_PROPERTY(Ref<JigsawParameter>, x);
	DECLARE_PROPERTY(Ref<JigsawParameter>, y);
	DECLARE_PROPERTY(Ref<JigsawParameter>, z);
	DECLARE_PROPERTY(Ref<JigsawParameter>, value);
	DECLARE_PROPERTY(Ref<JigsawParameter>, duration);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, created_sound);

	Type get_type() const override { return AUDIO; }
	bool allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const override;
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	int64_t get_num_results() const override;
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override;
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};
DECLARE_ENUM(JigsawCommandAudio::Operation);

#endif // JIGSAW_COMMAND_AUDIO_H
