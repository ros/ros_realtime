cmake_minimum_required(VERSION 2.8.3)
project(allocators)
find_package(catkin REQUIRED )

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(include ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
catkin_package(
    DEPENDS include # TODO add dependencies
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)

catkin_add_gtest(test_aligned_alloc test/test_aligned_alloc.cpp)## Generate added messages and services with any dependencies listed here
target_link_libraries(test_aligned_alloc ${Boost_LIBRARIES} ${catkin_LIBRARIES})
add_dependencies(test_aligned_alloc allocators_gencpp)

install(DIRECTORY test/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
