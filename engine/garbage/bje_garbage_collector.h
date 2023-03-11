#pragma once
#include <vector>

#include "common.h"


class BJE_Garbage
{
public:

	BJE_Garbage() {}
	~BJE_Garbage() {}

	void add(void* ptr)
	{
		garbage_collector_.push_back(ptr);
	}

	void clear()
	{
		for (const auto& t : garbage_collector_)
		{
			CHECK(rprObjectDelete(t));
		}
	}

	std::vector<void*> garbage_collector_;

};

