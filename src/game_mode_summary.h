#ifndef GAME_MODE_SUMMARY_H
#define GAME_MODE_SUMMARY_H

#include "icon_def.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class GameModeSummary : public Resource {
	GDCLASS(GameModeSummary, Resource);

protected:
	static void _bind_methods();

private:
	String author;
	IconDef::Icon mode_thumbnail = IconDef::Icon::NONE;
	String description;
	String latest_changes;
	TypedArray<IconDef> custom_icons;

public:
	GameModeSummary();
	~GameModeSummary();

	String get_author() const;
	void set_author(String new_author);
	IconDef::Icon get_mode_thumbnail() const;
	void set_mode_thumbnail(IconDef::Icon new_mode_thumbnail);
	String get_description() const;
	void set_description(String new_description);
	String get_latest_changes() const;
	void set_latest_changes(String new_latest_changes);
	TypedArray<IconDef> get_custom_icons() const;
	void set_custom_icons(TypedArray<IconDef> new_custom_icons);
	Ref<Texture2D> get_icon_texture(IconDef::Icon icon) const;
};

#endif // GAME_MODE_SUMMARY_H
