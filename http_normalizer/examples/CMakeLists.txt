project(example)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug) # keep assertion() on
endif()

# equivalent of: strip --strip-all
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# http_normalizer
add_subdirectory(../ build)

set(examples example)
foreach(example ${examples})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} PRIVATE http_normalizer)
    target_compile_options(${example}
        PRIVATE
            -Wall -Wextra -Wno-unused-function -pipe -pedantic
            -fstack-protector-all -fstack-protector-strong
            -O3 -ffast-math -funroll-loops -march=native
    )
endforeach(example)

