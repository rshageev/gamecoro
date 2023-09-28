#pragma once

#include "coroutine.h"

#include <vector>
#include <algorithm>

namespace gamecoro
{
	class Updater
	{
	public:
		Updater() = default;

		Updater(const Updater&) = delete;
		Updater& operator=(const Updater&) = delete;

		explicit Updater(Coroutine&& coro);

		void Start(Coroutine&& coro);
		void Update(TimeDuration dt);

	protected:
		std::vector<Coroutine> started;
		std::vector<Coroutine> running;
		std::vector<Coroutine> finished;

		void RunStartedCoroutines();
		void RemoveFinishedCoroutines();
	};
}