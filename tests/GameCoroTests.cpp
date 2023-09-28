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

TEST_CASE("Await coroutine")
{
	static constexpr int frames_count = 4;

	bool finished = false;

	auto wait_frames = [](int frames) -> gc::Coroutine {
		for (int i = 0; i < frames; ++i) {
			co_await gc::NextFrame{};
		}
	};

	SECTION("Await single")
	{
		auto wait_coro = [&]() -> gc::Coroutine {
			co_await wait_frames(frames_count);
			finished = true;
		};

		gc::Updater updater{ wait_coro() };

		for (int i = 0; i < frames_count - 1; ++i) {
			updater.Update(0.0f);
		}
		CHECK(!finished);
		updater.Update(0.0f);
		CHECK(finished);
	}

	SECTION("Await sequence")
	{
		auto wait_coro = [&]() -> gc::Coroutine {
			co_await wait_frames(frames_count);
			co_await wait_frames(frames_count);
			finished = true;
		};

		gc::Updater updater{ wait_coro() };

		// next coroutine is started only on the next frame after previous
		// has finished, and because of that we have +1 frame here
		for (int i = 0; i < ((frames_count * 2) + 1) - 1; ++i) {
			updater.Update(0.0f);
		}
		CHECK(!finished);
		updater.Update(0.0f);
		CHECK(finished);
	}
}

TEST_CASE("Spawning parallel coroutines")
{
	static constexpr int frame_count1 = 3;
	static constexpr int frame_count2 = 5;

	int counter = 0;

	auto inc_counter = [&](int cnt) -> gc::Coroutine {
		for (int i = 0; i < cnt; ++i) {
			++counter;
			co_await gc::NextFrame{};
		}
	};

	auto test = [&]() -> gc::Coroutine {
		co_yield inc_counter(frame_count1);
		co_yield inc_counter(frame_count2);
	};

	gc::Updater updater{ test() };
	for (int i = 0; i < std::max(frame_count1, frame_count2); ++i) {
		updater.Update(0.1f);
	}

	CHECK(counter == (frame_count1 + frame_count2));
}