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

		Coroutine(Coroutine&& rhs, Updater* updater);

		~Coroutine();

		void Run(Updater* updater) &&;

		void Update(float dt);
		bool Done() const;

	private:
		Handle handle;
	};
}