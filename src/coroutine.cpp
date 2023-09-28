#include "detail/coroutine.h"

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
	};


	void Coroutine::Update(float dt)
	{
		if (!Done()) {
			auto& wait_state = handle.promise().wait_state;
			const bool need_resume = std::visit(NeedResume{ dt }, wait_state);
			if (need_resume) {
				handle.resume();
			}
		}
	}
}
