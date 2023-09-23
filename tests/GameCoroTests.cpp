#include "catch_amalgamated.hpp"

#include "gamecoro.h"

namespace gc = gamecoro;

class TestUpdater : public gc::Updater
{
public:
	size_t RunningCount() const { return running.size(); }
};


TEST_CASE("Await next frame")
{
	float last_delta = 0.0f;

	auto wait_frames = [&last_delta](int count) -> gc::Coroutine {
		for (int i = 0; i < count; ++i) {
			last_delta = co_await gc::NextFrame{};
		}
	};

	TestUpdater updater;

	updater.Start(wait_frames(3));

	for (int i = 0; i < 2; ++i) {
		updater.Update(0.1f);
	}
	CHECK(updater.RunningCount() == 1);

	updater.Update(0.1f);
	CHECK(updater.RunningCount() == 0);
	CHECK(last_delta == 0.1f);
}