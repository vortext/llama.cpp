set(TARGET grammar)
add_library(${TARGET} SHARED grammar.cpp)

# Ensuring that the rpath is strictly set to $ORIGIN
set_target_properties(${TARGET} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN"
)

target_link_libraries(${TARGET} PRIVATE common llama ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(${TARGET} PRIVATE LLAMA_SHARED LLAMA_BUILD)
