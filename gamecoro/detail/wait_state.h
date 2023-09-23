#pragma once

#include <variant>


namespace gamecoro
{
	using TimeDuration = float;
	inline constexpr TimeDuration ZeroTime = 0.0f;



	struct WaitNextFrame
	{
		TimeDuration dt = ZeroTime;
	};

	struct WaitTimer
	{
		TimeDuration remaining = ZeroTime;
	};

	using WaitState = std::variant<WaitNextFrame, WaitTimer>;



	struct NeedResume
	{
		TimeDuration dt = ZeroTime;

		bool operator() (WaitNextFrame& wait_state) const
		{
			wait_state.dt = dt;
			return true;
		}

		bool operator() (WaitTimer& wait_state) const
		{
			wait_state.remaining -= dt;
			return wait_state.remaining <= ZeroTime;
		}
	};
}
