#include "detail/coroutine.h"
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

	Coroutine::Coroutine(Coroutine&& rhs)
		: handle(std::exchange(rhs.handle, nullptr))
	{}

	Coroutine& Coroutine::operator=(Coroutine&& rhs)
	{
		if (handle) {
			handle.destroy();
		}
		handle = std::exchange(rhs.handle, nullptr);
		return *this;
	}

	Coroutine::Handle Coroutine::Run(Updater* updater) &&
	{
		auto h = handle;
		updater->Start(std::move(*this));
		return h;
	}

	Coroutine::~Coroutine()
	{
		if (handle) {
			handle.destroy();
		}
	}

	bool Coroutine::Done() const
	{
		return !handle || handle.done();
	}

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
