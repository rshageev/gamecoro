#pragma once

#include <variant>

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

	using WaitState = std::variant<WaitNextFrame, WaitTimer>;
}
