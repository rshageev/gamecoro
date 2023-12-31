#include "detail/updater.h"

namespace gamecoro
{
	struct NeedResume
	{
		float dt = 0.0f;

		bool operator() (WaitNextFrame& wait_state) const noexcept
		{
			wait_state.dt = dt;
			return true;
		}

		bool operator() (WaitTimer& wait_state) const noexcept
		{
			wait_state.remaining -= dt;
			return wait_state.remaining <= 0.0f;
		}

		bool operator() (const WaitCoroutine& wait_state) const noexcept
		{
			return !wait_state.handle || wait_state.handle.done();
		}
	};


	Updater::Updater(Coroutine&& coro)
	{
		Start(std::move(coro));
	}

	void Updater::Start(Coroutine&& coro)
	{
		auto handle = coro.GetHandle();
		handle.promise().updater = this;
		handle.resume();

		// adding fresh coroutines into separate vector, because they could be added
		// by running coroutines, while we are iterating over `running` vector
		started.push_back(std::move(coro));
	}

	void Updater::Update(float dt)
	{
		RunStartedCoroutines();

		for (auto& coro : running)
		{
			if (!coro.Done())
			{
				auto handle = coro.GetHandle();
				auto& wait_state = handle.promise().wait_state;
				const bool need_resume = std::visit(NeedResume{ dt }, wait_state);
				if (need_resume) {
					handle.resume();
				}
			}
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