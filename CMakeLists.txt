# --------------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------------

find_package(yat CONFIG REQUIRED)

# --------------------------------------------------------------------------
# Collect sources and includes
# --------------------------------------------------------------------------

file(GLOB_RECURSE sources src/*.cpp)
set(includedirs
    include/
    lib/
)

# --------------------------------------------------------------------------
# Target
# --------------------------------------------------------------------------

add_library(limauview ${sources})

target_include_directories(limauview
    PUBLIC 
        ${includedirs}
)

target_link_libraries(limauview 
    PUBLIC
        limacore
        yat::yat
)

limatools_set_library_soversion(limauview "VERSION")

# --------------------------------------------------------------------------
# Compile definitions
# --------------------------------------------------------------------------

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" UVIEW_VERSION_RAW)
string(STRIP "${UVIEW_VERSION_RAW}" UVIEW_VERSION)

target_compile_definitions(limauview
    PUBLIC
        UVIEW_NAME=Uview
        UVIEW_VERSION=${UVIEW_VERSION}
        UVIEW_ENABLED
)

message(STATUS "Camera enabled: Uview ${UVIEW_VERSION}")

# Windows DLL export
if (WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(limauview
        PRIVATE
            LIBUVIEW_EXPORTS
    )
endif()

# #import incompatibility with /MP option
# With msvc 140 is not possible to disable multiprocessor compilation with /MP-
if(MSVC)
    string(REGEX REPLACE "/MP[0-9]*" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/MP[0-9]*" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
endif()

# --------------------------------------------------------------------------
# Install
# --------------------------------------------------------------------------

foreach(includedir ${includedirs})
    install (
        DIRECTORY ${includedir} 
        DESTINATION include 
        FILES_MATCHING 
        PATTERN "*.h"
        PATTERN "*.tlb"
    )
endforeach()

install(
    TARGETS limauview 
    LIBRARY DESTINATION lib
)
