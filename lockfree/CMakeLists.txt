# Catkin User Guide: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/user_guide.html
# Catkin CMake Standard: http://www.ros.org/doc/groovy/api/catkin/html/user_guide/standards.html
cmake_minimum_required(VERSION 2.8.3)
project(lockfree)
# Load catkin and all dependencies required for this package
# TODO: remove all from COMPONENTS that are not catkin packages.
find_package(catkin REQUIRED COMPONENTS rosatomic rosconsole roslib allocators)

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

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include)
link_directories(${catkin_INCLUDE_DIRS})
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


# CATKIN_MIGRATION: removed during catkin migration
# rosbuild_add_boost_directories()

add_library(${PROJECT_NAME} src/free_list.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${Boost_LIBRARIES})


# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS rosatomic rosconsole roslib allocators
    CATKIN_DEPENDS rosatomic allocators# TODO
    INCLUDE_DIRS include ${catkin_INCLUDE_DIRS} # TODO include
    LIBRARIES # TODO
)
catkin_add_gtest(test_freelist test/test_freelist.cpp)
target_link_libraries(test_freelist ${PROJECT_NAME})## Generate added messages and services with any dependencies listed here
catkin_add_gtest(test_object_pool test/test_object_pool.cpp)
target_link_libraries(test_object_pool ${PROJECT_NAME})

