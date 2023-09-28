#pragma once

#include "wait_state.h"
#include "awaitables.h"

#include <coroutine>

namespace gamecoro
{
	class Updater;

	template<class CoroInterface>
	struct Promise
	{
		WaitState wait_state;
		Updater* updater = nullptr;

		CoroInterface get_return_object() {
			return CoroInterface{ std::coroutine_handle<Promise>::from_promise(*this) };
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() {}

		TimerAwaitable<Promise> await_transform(float time) { return { time }; }
		auto await_transform(auto&& v) { return v; }
	};
}