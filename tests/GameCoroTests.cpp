#include "catch_amalgamated.hpp"

#include "gamecoro/gamecoro.h"


namespace gc = gamecoro;


TEST_CASE("Await next frame")
{
	static constexpr int frame_count = 3;
	static constexpr float delta_time = 0.1f;

	float last_delta = 0.0f;
	bool finished = false;

	auto wait_frames = [&](int count) -> gc::Coroutine {
		for (int i = 0; i < count; ++i) {
			last_delta = co_await gc::NextFrame{};
		}
		finished = true;
	};

	gc::Updater updater{ wait_frames(frame_count) };

	for (int i = 0; i < frame_count - 1; ++i) {
		updater.Update(delta_time);
	}
	CHECK(!finished);

	updater.Update(delta_time);
	CHECK(finished);
	CHECK(last_delta == delta_time);
}

TEST_CASE("Await timer")
{
	static constexpr float wait_time = 1.0f;
	static constexpr float delta_time = 0.015f;

	bool finished = false;
	auto wait_timer = [&](float time) -> gc::Coroutine {
		co_await time;
		finished = true;
	};

	gc::Updater updater{ wait_timer(wait_time) };
	
	float time = wait_time;
	while (time > 0.0f) {
		CHECK(!finished);
		updater.Update(delta_time);
		time -= delta_time;
	}
	CHECK(finished);
}