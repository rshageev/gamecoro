#pragma once

#include "wait_state.h"

#include <coroutine>

namespace gamecoro
{
	struct NextFrame {};

	template<class P>
	class NextFrameAwaitable
	{
	public:
		bool await_ready() const noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<P> h)
		{
			handle = h;
			handle.promise().wait_state = WaitNextFrame{};
		}

		float await_resume()
		{
			const auto& wait_state = std::get<WaitNextFrame>(handle.promise().wait_state);
			return wait_state.dt;
		}

	private:
		std::coroutine_handle<P> handle;
	};

	template<class P>
	struct TimerAwaitable
	{
		float time;

		bool await_ready() const noexcept
		{
			return time <= 0.0f;
		}

		void await_suspend(std::coroutine_handle<P> h)
		{
			h.promise().wait_state = WaitTimer{ time };
		}

		void await_resume() const noexcept {}
	};

	template<class P>
	struct CoroutineAwaitable
	{
		std::coroutine_handle<> handle;

		bool await_ready() const noexcept
		{
			return !handle || handle.done();
		}

		void await_suspend(std::coroutine_handle<P> h)
		{
			h.promise().wait_state = WaitCoroutine{ handle };
		}

		void await_resume() const noexcept {}
	};
}