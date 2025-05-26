set(target ${MP2_COMPILER_APP})

add_executable(${target} main.cpp)
target_link_libraries(${target} PUBLIC ${MP2_LIBRARY})
target_sources(${target} PRIVATE ${RESOURCES})