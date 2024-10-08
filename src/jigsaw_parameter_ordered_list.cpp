#include "jigsaw_parameter_ordered_list.h"

void JigsawParameterOrderedList::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, list);

	ClassDB::bind_method(D_METHOD("is_homogenous", "type"), &JigsawParameterOrderedList::is_homogenous);

	ClassDB::bind_static_method("JigsawParameterOrderedList", D_METHOD("make", "list"), &JigsawParameterOrderedList::make);
}

IMPLEMENT_PROPERTY(JigsawParameterOrderedList, TypedArray<JigsawParameter>, list);

bool JigsawParameterOrderedList::is_homogenous(Type type) const {
	for (int64_t i = 0; i < list.size(); i++) {
		JigsawParameter *param = Object::cast_to<JigsawParameter>(list[i]);
		if (!param || param->get_type() != type) {
			return false;
		}
	}

	return true;
}

Ref<JigsawParameterOrderedList> JigsawParameterOrderedList::make(const TypedArray<JigsawParameter> &list) {
	Ref<JigsawParameterOrderedList> param;
	param.instantiate();
	param->set_list(list);
	return param;
}
