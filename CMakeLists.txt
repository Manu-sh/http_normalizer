project(state)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
	# set(CMAKE_BUILD_TYPE Release)
	set(CMAKE_BUILD_TYPE Debug) # keep assertion() on
endif()

# equivalent of: strip --strip-all
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(EXECUTABLE_OUTPUT_PATH, ${PROJECT_SOURCE_DIR}/bin)
set(INC_PATH  ${PROJECT_SOURCE_DIR}/include)
set(SRC_PATH  ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(lib/http_normalizer_parts/http_parts)
add_executable(${PROJECT_NAME} 
	${SRC_PATH}/http_normalizer.cpp
	${PROJECT_NAME}.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_PATH})
target_link_libraries(${PROJECT_NAME} PRIVATE httparts)

target_compile_options(${PROJECT_NAME}
	PRIVATE
		-Wall -Wextra -Wno-unused-function -pipe -pedantic
		-fstack-protector-all -fstack-protector-strong
		-O3 -ffast-math -funroll-loops -march=native
)
