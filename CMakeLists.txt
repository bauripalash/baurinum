cmake_minimum_required(VERSION 3.24...3.30)
project(baurinum)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE 
		"Debug" 
		CACHE STRING 
		"Set Build Type (Debug, Release, RelWithDebInfo, MinSizeRel)" 
		FORCE)
endif()

set_property(CACHE 
	CMAKE_BUILD_TYPE 
	PROPERTY STRINGS 
	"Debug" "Release" "RelWithDebInfo" "MinSizeRel"
)
add_executable(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED ON)
#set_property(TARGET ${PROJECT_NAME} PROPERTY C_EXTENSIONS OFF)



set(BAURINUM_DEBUG_CFLAGS
	"-g3" 
	"-pedantic"
	"-Wall" 
	"-Wextra"
	"-Wconversion"
	"-fsanitize=address"
	"-fsanitize=undefined"
	"-fsanitize=float-cast-overflow" 
)

set(BAURINUM_RELEASE_CFLAGS
	"-pedantic" 
	"-Wall"
	"-O2"
	"-flto" 
	"-funroll-loops" 
)

set (BAURINUM_REL_WITH_DINFO_CFLAGS
	"-g3"
	${PANKTI_RELEASE_CFLAGS}
)

set(BAURINUM_DEBUG_LINKER_FLAGS
	"-lm"
	"-fsanitize=address"
	"-fsanitize=undefined"
	"-fsanitize=float-cast-overflow"
	"-flto"
)

set(BAURINUM_RELEASE_LINKER_FLAGS
	"-lm"
	"-flto"
	"-funroll-loops"
)

set(BAURINUM_REL_WITH_DINFO_LINKER_FLAGS 
	${BAURINUM_RELEASE_LINKER_FLAGS}
)

set(BAURINUM_SRC_FILES 
	"${CMAKE_CURRENT_LIST_DIR}/baurinum.c"
)

set(BAURINUM_HEADER_FILES 
	"${CMAKE_CURRENT_LIST_DIR}/baurinum.h"
)

target_sources(${PROJECT_NAME} 
	PRIVATE 
	${BAURINUM_SRC_FILES}
	${BAURINUM_HEADER_FILES}
)

target_compile_options(${PROJECT_NAME} PRIVATE 
	$<$<CONFIG:Debug>:${BAURINUM_DEBUG_CFLAGS}>
	$<$<CONFIG:Release>:${BAURINUM_RELEASE_CFLAGS}>
	$<$<CONFIG:RelWithDebInfo>:${BAURINUM_REL_WITH_DINFO_CFLAGS}>
	$<$<CONFIG:MinSizeRel>:${BAURINUM_RELEASE_CFLAGS}>
)

target_link_options(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:${BAURINUM_DEBUG_LINKER_FLAGS}>
	$<$<CONFIG:Release>:${BAURINUM_RELEASE_LINKER_FLAGS}>
	$<$<CONFIG:RelWithDebInfo>:${BAURINUM_REL_WITH_DINFO_LINKER_FLAGS}>
	$<$<CONFIG:MinSizeRel>:${BAURINUM_RELEASE_LINKER_FLAGS}>
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PRIVATE MI_TRACK_ASAN=1)
	add_compile_definitions(BAURINUM_BUILD_DEBUG)
else()
	add_compile_definitions(BAURINUM_BUILD_RELEASE)
endif()
