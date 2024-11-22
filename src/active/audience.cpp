#include "audience.h"
#include "player_preferences_helper.h"

void Audience::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(MultiMesh, meshes);

	ClassDB::bind_method(D_METHOD("update", "dir"), &Audience::update);

	ClassDB::bind_method(D_METHOD("cheer", "repeats"), &Audience::cheer);
	ClassDB::bind_method(D_METHOD("cheer_left", "repeats"), &Audience::cheer_left);
	ClassDB::bind_method(D_METHOD("cheer_right", "repeats"), &Audience::cheer_right);
}

IMPLEMENT_PROPERTY_ONCHANGE(Audience, TypedArray<MultiMesh>, meshes, _members.resize(new_meshes.size()));

void Audience::update(Vector2i dir) {
	for (int64_t i = 0; i < _members.size(); i++) {
		Ref<MultiMesh> mesh = _meshes[i];
		ERR_CONTINUE(mesh.is_null());

		int64_t mesh_instances = next_power_of_2(_members[i].size());
		if (unlikely(mesh->get_instance_count() != next_power_of_2(_members[i].size()))) {
			mesh->set_instance_count(mesh_instances);
		}

		int64_t k = 0;
		for (int64_t j = 0; j < _members[i].size(); j++) {
			AudienceMember_t &member = _members.write[i].write[j];
			if (member.deleted) {
				continue;
			}

			int8_t d = member.left ? dir.x : dir.y;
			if (d == 1 && member.hop < member.hop_time) {
				member.hop += 3;
			} else if (d == -1 && member.hop >= member.hop_time) {
				member.hop += 5;
			} else {
				member.hop += 2;
			}

			if (member.hop >= member.hop_time * 2) {
				member.hop = 0;
				if (!member.cheering) {
					member.cheering = member.want_cheer;
					member.want_cheer = 0;
				} else {
					member.cheering--;
				}
			}

			float e2 = member.cheering ? 0.25f : 1.0f;
			float arc = 1.0f - float(member.hop) / float(member.hop_time);
			float dy = (member.excitement / e2 * 0.5f + ((1.0f - e2) * 0.4f)) * (1.0f - arc * arc);
			if (PlayerPreferences::prefers_reduced_motion()) {
				dy = 0.0f;
			}

			Vector3 pos = member.base_position;
			pos.y += dy;

			mesh->set_instance_transform(k, Transform3D(Basis(), pos));
			mesh->set_instance_color(k, member.color);
			mesh->set_instance_custom_data(k, Color(member.cheering ? 1.0f : 0.0f, member.flip ? 1.0f : 0.0f, 0.0f, 0.0f));
			k++;
		}

		mesh->set_visible_instance_count(k);
	}
}

void Audience::cheer(uint8_t repeats) {
	for (Vector<AudienceMember_t> &group : _members) {
		for (AudienceMember_t &member : group) {
			member.want_cheer = repeats;
		}
	}
}
void Audience::cheer_left(uint8_t repeats) {
	for (Vector<AudienceMember_t> &group : _members) {
		for (AudienceMember_t &member : group) {
			if (member.left) {
				member.want_cheer = repeats;
			}
		}
	}
}
void Audience::cheer_right(uint8_t repeats) {
	for (Vector<AudienceMember_t> &group : _members) {
		for (AudienceMember_t &member : group) {
			if (!member.left) {
				member.want_cheer = repeats;
			}
		}
	}
}
