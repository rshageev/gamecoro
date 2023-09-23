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

		explicit Updater(Coroutine&& coro)
		{
			Start(std::move(coro));
		}

		void Start(Coroutine&& coro)
		{
			coro.SetUpdater(this);
			coro.Resume();
			started.push_back(std::move(coro));
		}

		void Update(TimeDuration dt)
		{
			RunStartedCoroutines();

			for (auto& coro : running) {
				coro.Update(dt);
			}

			RemoveFinishedCoroutines();
		}

	protected:
		std::vector<Coroutine> started;
		std::vector<Coroutine> running;
		std::vector<Coroutine> finished;

		void RunStartedCoroutines()
		{
			running.insert(running.end(), std::make_move_iterator(started.begin()), std::make_move_iterator(started.end()));
			started.clear();
		}

		void RemoveFinishedCoroutines()
		{
			auto is_running = [](const Coroutine& coro) { return !coro.Done(); };
			auto itr = std::partition(running.begin(), running.end(), is_running);

			finished.assign(std::make_move_iterator(itr), std::make_move_iterator(running.end()));

			running.erase(itr, running.end());
		}
	};
}