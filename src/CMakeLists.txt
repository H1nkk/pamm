set(target ${MP2_LIBRARY})

file(GLOB_RECURSE hdrs "*.h*" "${MP2_INCLUDE}/*.h*")
file(GLOB_RECURSE srcs "*.cpp")

add_library(${target} STATIC ${srcs} ${hdrs})
target_link_libraries(${target} ${LIBRARY_DEPS})
