# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(rosrt)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS roscpp roslib rosunit rosatomic allocators lockfree std_msgs)

# include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})
# CATKIN_MIGRATION: removed during catkin migration
# cmake_minimum_required(VERSION 2.4.6)

# CATKIN_MIGRATION: removed during catkin migration
# include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)


# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#add_message_files(
#uncomment if you have defined services
#add_service_files(
include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})
link_directories(${catkin_INCLUDE_DIRS})

#common commands for building c++ executables and libraries
add_library(${PROJECT_NAME} src/malloc.cpp src/simple_gc.cpp src/publisher.cpp src/subscriber.cpp src/init.cpp)

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_add_boost_directories()
find_package(Boost REQUIRED COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${catkin_LIBRARIES})

add_executable(test_publisher EXCLUDE_FROM_ALL test/test_publisher.cpp)
target_link_libraries(test_publisher ${catkin_LIBRARIES} ${Boost_LIBRARIES})

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_add_gtest_build_flags(test_publisher)
target_link_libraries(test_publisher ${PROJECT_NAME})
#add_rostest(test/test_publisher.xml)

#
add_executable(test_subscriber EXCLUDE_FROM_ALL  test/test_subscriber.cpp)
target_link_libraries(test_subscriber ${PROJECT_NAME})

# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_add_gtest_build_flags(test_subscriber)
#add_rostest(test/test_subscriber.xml)


add_library(test_malloc_wrappers_so EXCLUDE_FROM_ALL test/test_malloc_wrappers_so.cpp)
target_link_libraries(test_malloc_wrappers_so ${catkin_LIBRARIES} ${Boost_LIBRARIES})

#rosbuild_declare_test(test_malloc_wrappers_so)

#add_rostest(test/test_publisher_no_initialize.xml)
## Generate added messages and services with any dependencies listed here

# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS roscpp roslib rosunit rosatomic allocators lockfree std_msgs
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES # TODO
)
catkin_add_gtest(test_malloc_wrappers test/test_malloc_wrappers.cpp)
target_link_libraries(test_malloc_wrappers ${PROJECT_NAME})

catkin_add_gtest(test_publisher_no_initialize test/test_publisher_no_initialize.cpp)
#rosbuild_declare_test(test_publisher_no_initialize)
target_link_libraries(test_publisher_no_initialize ${PROJECT_NAME} ${catkin_LIBRARIES} ${lockfree_LIBRARIES})
add_dependencies(test_publisher_no_initialize ${catkin_EXPORTED_TARGETS})

