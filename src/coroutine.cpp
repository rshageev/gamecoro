#include "detail/coroutine.h"

namespace gamecoro
{
	void Coroutine::Update(TimeDuration dt)
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
