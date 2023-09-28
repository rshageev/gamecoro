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

		Coroutine(Coroutine&& rhs);
		Coroutine& operator=(Coroutine&& rhs);

		~Coroutine();

		Handle Run(Updater* updater) &&;
		Handle GetHandle() { return handle; }

		bool Done() const;

	private:
		Handle handle;
	};
}