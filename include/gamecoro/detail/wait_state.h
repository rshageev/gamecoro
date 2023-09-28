#pragma once

#include <variant>
#include <coroutine>

namespace gamecoro
{
	struct WaitNextFrame
	{
		float dt = 0.0f;
	};

	struct WaitTimer
	{
		float remaining = 0.0f;
	};

	struct WaitCoroutine
	{
		std::coroutine_handle<> handle;
	};

	using WaitState = std::variant<WaitNextFrame, WaitTimer, WaitCoroutine>;
}
