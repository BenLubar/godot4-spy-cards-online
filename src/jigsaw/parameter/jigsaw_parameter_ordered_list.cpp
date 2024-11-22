#include "jigsaw_parameter_ordered_list.h"

void JigsawParameterOrderedList::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, list);
	BIND_PROPERTY_IS(Variant::BOOL, template);

	ClassDB::bind_method(D_METHOD("is_homogenous", "type"), &JigsawParameterOrderedList::is_homogenous);

	ClassDB::bind_static_method("JigsawParameterOrderedList", D_METHOD("make", "list"), &JigsawParameterOrderedList::make);
	ClassDB::bind_static_method("JigsawParameterOrderedList", D_METHOD("make_template", "list"), &JigsawParameterOrderedList::make_template);
}

IMPLEMENT_PROPERTY(JigsawParameterOrderedList, TypedArray<JigsawParameter>, list);
IMPLEMENT_PROPERTY_IS(JigsawParameterOrderedList, bool, template);

bool JigsawParameterOrderedList::is_homogenous(Type type) const {
	for (int64_t i = 0; i < _list.size(); i++) {
		Ref<JigsawParameter> param = _list[i];
		if (param.is_null() || param->get_type() != type) {
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

Ref<JigsawParameterOrderedList> JigsawParameterOrderedList::make_template(const TypedArray<JigsawParameter> &list) {
	Ref<JigsawParameterOrderedList> param;
	param.instantiate();
	param->set_list(list);
	param->set_template(true);
	return param;
}
