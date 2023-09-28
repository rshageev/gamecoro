#include "detail/updater.h"

namespace gamecoro
{
	Updater::Updater(Coroutine&& coro)
	{
		Start(std::move(coro));
	}

	void Updater::Start(Coroutine&& coro)
	{
		coro.SetUpdater(this);
		coro.Resume();

		// adding fresh coroutines into separate vector, because they could be added
		// by running coroutines, while we are iterating over `running` vector
		started.push_back(std::move(coro));
	}

	void Updater::Update(TimeDuration dt)
	{
		RunStartedCoroutines();

		for (auto& coro : running) {
			coro.Update(dt);
		}

		RemoveFinishedCoroutines();
	}

	void Updater::RunStartedCoroutines()
	{
		running.insert(running.end(), std::make_move_iterator(started.begin()), std::make_move_iterator(started.end()));
		started.clear();
	}

	void Updater::RemoveFinishedCoroutines()
	{
		// instead of deleting finished coroutines, we are letting them live one more frame in a separate vector
		// because we want to be able to check them from inside other coroutines, which are possibly awaiting them

		auto is_running = [](const Coroutine& coro) { return !coro.Done(); };
		auto itr = std::partition(running.begin(), running.end(), is_running);

		finished.assign(std::make_move_iterator(itr), std::make_move_iterator(running.end()));

		running.erase(itr, running.end());
	}
}