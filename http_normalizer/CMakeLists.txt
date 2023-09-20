project(http_normalizer)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
	# set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_BUILD_TYPE Debug) # keep assertion() on
endif()

# equivalent of: strip --strip-all
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# set(CTEST_OUTPUT_ON_FAILURE ON)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(EXECUTABLE_OUTPUT_PATH, ${PROJECT_SOURCE_DIR}/bin)

set(INC_PATH  ${PROJECT_SOURCE_DIR}/include)
set(SRC_PATH  ${PROJECT_SOURCE_DIR}/src)
set(LIB_PATH  ${PROJECT_SOURCE_DIR}/lib)

# self
add_library(${PROJECT_NAME} STATIC ${SRC_PATH}/http_normalizer.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_PATH})

# http_parts
add_subdirectory(${LIB_PATH}/http_normalizer_parts/http_parts)
target_link_libraries(${PROJECT_NAME} PRIVATE httparts)

target_compile_options(${PROJECT_NAME}
	PRIVATE
		-Wall -Wextra -Wno-unused-function -pipe -pedantic
		-fstack-protector-all -fstack-protector-strong
		-O3 -ffast-math -funroll-loops -march=native -mtune=native
)

# yay -S --noconfirm doctest
include(CTest)
if(BUILD_TESTING)
	message("Config build type: ${CMAKE_BUILD_TYPE}")
	add_subdirectory(tests)
endif()
