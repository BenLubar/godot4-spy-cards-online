#ifndef CARD_FILTER_H
#define CARD_FILTER_H

#include "dry.h"

class CardFilter : public Resource {
	GDCLASS(CardFilter, Resource);

protected:
	static void _bind_methods();

public:
	CardFilter();
	~CardFilter();
};

#endif // CARD_FILTER_H
