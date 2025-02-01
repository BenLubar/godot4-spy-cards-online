#ifndef JIGSAW_SCENE_H
#define JIGSAW_SCENE_H

#include "jigsaw/presentation/jigsaw_presentation_data.h"

#include "jigsaw/parameter/jigsaw_parameter_scene.h"

#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/node3d.hpp>

class JigsawSceneCommandHistory : public Resource {
	GDCLASS(JigsawSceneCommandHistory, Resource);

protected:
	static void _bind_methods();

public:
	enum Command {
		DESTROY_SCENE, // no data or path; hides scene root node and prevents further commands; when removed from history (forwards), deletes the scene.
		TOGGLE_VISIBLE, // no data; visibility flips boolean value when this command is reached.
	};

	DECLARE_PROPERTY(int64_t, rollback_frame, = -1);
	DECLARE_PROPERTY(NodePath, node_path);
	DECLARE_PROPERTY(Command, command, = DESTROY_SCENE);
};
DECLARE_ENUM(JigsawSceneCommandHistory::Command);

class JigsawSceneManipulator : public JigsawPresentationData {
	GDCLASS(JigsawSceneManipulator, JigsawPresentationData);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<JigsawSceneCommandHistory>, history);
	DECLARE_PROPERTY(Ref<JigsawParameterScene>, scene_id);
	DECLARE_PROPERTY_OBJECTID(Node, root);
	Node *get_root_rollback() const;
	DECLARE_PROPERTY_OBJECTID(AnimationTree, animation_tree);

	void kill_node() override;
	void discard_rollback_data(int64_t new_base_frame = -1) override;
	void rollback_to_frame(int64_t frame) override;
	void advance_frames(int64_t frames = 1) override;

	void destroy(int64_t frame = -1);
	void set_visible(const NodePath &path, bool visible, int64_t frame = -1);

private:
	void _apply_command(const Ref<JigsawSceneCommandHistory> &history, bool reverse = false) const;
	void _apply_animation_command(const Ref<JigsawSceneCommandHistory> &history, AnimationTree *tree) const;
	void _take_animation_snapshot();
	void _restore_animation_snapshot();
};

#endif // JIGSAW_SCENE_MANIPULATOR_H
