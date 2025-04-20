#ifndef JIGSAW_GENERIC_NODE_H
#define JIGSAW_GENERIC_NODE_H

#include "jigsaw/presentation/jigsaw_presentation_data.h"

#include <godot_cpp/classes/tween.hpp>

class JigsawGenericNodeCommandHistory : public Resource {
	GDCLASS(JigsawGenericNodeCommandHistory, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, rollback_frame, = -1);
	DECLARE_PROPERTY(NodePath, property_name);
	DECLARE_PROPERTY(Ref<Tween>, tween);
	DECLARE_PROPERTY(Variant, from_value);
	DECLARE_PROPERTY(Variant, to_value);
	DECLARE_PROPERTY(float, duration, = 0.0f);
	DECLARE_PROPERTY(float, skip_duration, = 0.0f);
	int get_duration_in_frames() const;
};

class JigsawGenericNode : public JigsawPresentationData {
	GDCLASS(JigsawGenericNode, JigsawPresentationData);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY_OBJECTID(Node, node);
	DECLARE_PROPERTY(bool, owns_node, = true);
	DECLARE_PROPERTY_IS(bool, destroyed, = false);
	DECLARE_PROPERTY(TypedArray<JigsawGenericNodeCommandHistory>, history);

	void kill_node() override;
	void discard_rollback_data(int64_t new_base_frame = -1) override;
	void rollback_to_frame(int64_t frame) override;
	void advance_frames(int64_t frames = 1) override;

	void set_property(const NodePath &name, const Variant &value, float duration = 0.0f, int64_t frame = -1);
	void stop_property(const NodePath &name, int64_t frame = -1);
	void destroy(int64_t frame = -1);
};

#endif // JIGSAW_GENERIC_NODE_H
