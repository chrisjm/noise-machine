cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NoiseMachine VERSION 0.1)

# --- Project Configuration ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(noise_machine ${SOURCES})
target_include_directories(noise_machine PUBLIC include)

target_link_libraries(noise_machine PRIVATE
  ${CMAKE_THREAD_LIBS_INIT}
)

if(APPLE)
  target_link_libraries(noise_machine PRIVATE "-framework CoreAudio")
endif()
