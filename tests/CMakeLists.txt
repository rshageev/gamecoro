add_executable(GameCoro-Tests
	catch_amalgamated.cpp
	GameCoroTests.cpp
)
target_link_libraries(GameCoro-Tests GameCoro)