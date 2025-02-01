#ifndef AUDIENCE_H
#define AUDIENCE_H

#include "dry.h"

#include <godot_cpp/classes/hashing_context.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>

struct AudienceMember_t {
	Vector3 base_position{};
	Color color{};
	float excitement{};
	uint8_t want_cheer{};
	uint8_t cheering{};
	uint8_t hop_time{};
	uint8_t hop{};
	bool left{};
	bool flip{};
	bool deleted{};
	bool reusable{};
};

class Audience : public RefCounted {
	GDCLASS(Audience, RefCounted);

protected:
	static void _bind_methods();

private:
	Vector<Vector<AudienceMember_t>> _members;
	uint8_t _hop_offset = 0;
	friend class JigsawCommandAudience;

public:
	DECLARE_PROPERTY(TypedArray<MultiMesh>, meshes);

	void update(Vector2i dir);

	void cheer(uint8_t repeats);
	void cheer_left(uint8_t repeats);
	void cheer_right(uint8_t repeats);

	void assign(const Ref<Audience> &audience);
	void write_hash(const Ref<HashingContext> &hash) const;

	DEFAULT_TO_STRING();
};

#endif // AUDIENCE_H
