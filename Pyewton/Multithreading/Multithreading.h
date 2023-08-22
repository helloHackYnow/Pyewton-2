#pragma once
#include <shared_mutex>
#include <vector>
#include "../Body/Body.h"

namespace Pyewton
{
	template <typename T>
	class SharedData
	{
	public:
		T* GetPtr();
		T Get();

	private:
		std::shared_mutex mutex;
		T data;
	};


}