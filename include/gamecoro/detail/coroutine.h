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

		void Update(float dt);

	private:
		Handle handle;
	};


	using NextFrame = NextFrameAwaitable<Coroutine::promise_type>;
}