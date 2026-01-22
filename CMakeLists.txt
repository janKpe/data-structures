add_library(data-structures hashmap/hashmap.c vector/vector.c)

target_include_directories(data-structures
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
