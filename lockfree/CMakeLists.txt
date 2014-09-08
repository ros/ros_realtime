cmake_minimum_required(VERSION 2.8.3)
project(lockfree)
find_package(catkin REQUIRED COMPONENTS rosatomic rosconsole roslib allocators)

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include)
link_directories(${catkin_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} src/free_list.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${Boost_LIBRARIES})

catkin_package(
    DEPENDS rosatomic rosconsole roslib allocators
    CATKIN_DEPENDS rosatomic allocators# TODO
    INCLUDE_DIRS include ${catkin_INCLUDE_DIRS} # TODO include
    LIBRARIES # TODO
)
catkin_add_gtest(test_freelist test/test_freelist.cpp)
catkin_add_gtest(test_object_pool test/test_object_pool.cpp)
target_link_libraries(test_object_pool ${PROJECT_NAME})


install(DIRECTORY test/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
