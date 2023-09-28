#include "detail/coroutine.h"
#include "detail/updater.h"

namespace gamecoro
{
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
}
