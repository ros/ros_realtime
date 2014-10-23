cmake_minimum_required(VERSION 2.8.3)
project(rosrt)
find_package(catkin REQUIRED COMPONENTS 
roscpp 
roslib 
rosunit 
rosatomic 
allocators 
lockfree 
std_msgs)

include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

catkin_package(
    DEPENDS roscpp roslib rosunit rosatomic allocators lockfree std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES ${PROJECT_NAME} # TODO
)

add_library(${PROJECT_NAME} src/malloc.cpp src/simple_gc.cpp src/publisher.cpp src/subscriber.cpp src/init.cpp)

find_package(Boost REQUIRED COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${catkin_LIBRARIES})

add_executable(test_publisher EXCLUDE_FROM_ALL test/test_publisher.cpp)
target_link_libraries(test_publisher ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(test_subscriber EXCLUDE_FROM_ALL  test/test_subscriber.cpp)
target_link_libraries(test_subscriber ${PROJECT_NAME})

add_library(test_malloc_wrappers_so EXCLUDE_FROM_ALL test/test_malloc_wrappers_so.cpp)
target_link_libraries(test_malloc_wrappers_so ${catkin_LIBRARIES} ${Boost_LIBRARIES})
catkin_add_gtest(test_malloc_wrappers test/test_malloc_wrappers.cpp)
target_link_libraries(test_malloc_wrappers ${PROJECT_NAME} ${catkin_LIBRARIES})

catkin_add_gtest(test_publisher_no_initialize test/test_publisher_no_initialize.cpp)
target_link_libraries(test_publisher_no_initialize ${PROJECT_NAME} ${catkin_LIBRARIES} ${lockfree_LIBRARIES})
add_dependencies(test_publisher_no_initialize ${catkin_EXPORTED_TARGETS})

install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(DIRECTORY test/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

