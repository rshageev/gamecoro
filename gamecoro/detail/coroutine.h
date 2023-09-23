#pragma once

#include "promise.h"
#include "awaitables.h"

#include <coroutine>
#include <utility>

namespace gamecoro
{
	class Updater;

	class Coroutine
	{
	public:
		using promise_type = Promise<Coroutine>;
		using Handle = std::coroutine_handle<promise_type>;

		Coroutine() = delete;
		Coroutine(Handle h) : handle(h) {}

		Coroutine(const Coroutine&) = delete;
		Coroutine operator=(const Coroutine&) = delete;

		Coroutine(Coroutine&& rhs)
			: handle(std::exchange(rhs.handle, nullptr))
		{}

		Coroutine& operator=(Coroutine&& rhs)
		{
			if (handle) {
				handle.destroy();
			}
			handle = std::exchange(rhs.handle, nullptr);
			return *this;
		}

		~Coroutine()
		{
			if (handle) {
				handle.destroy();
			}
		}

		void SetUpdater(Updater* updater)
		{
			handle.promise().updater = updater;
		}

		void Resume()
		{
			handle.resume();
		}

		bool Done() const
		{
			return !handle || handle.done();
		}

		void Update(TimeDuration dt)
		{
			if (!Done()) {
				auto& wait_state = handle.promise().wait_state;
				const bool need_resume = std::visit(NeedResume{ dt }, wait_state);
				if (need_resume) {
					handle.resume();
				}
			}
		}

	private:
		Handle handle;
	};


	using NextFrame = NextFrameAwaitable<Coroutine::promise_type>;
}