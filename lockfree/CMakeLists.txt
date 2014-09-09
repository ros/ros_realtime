cmake_minimum_required(VERSION 2.8.3)
project(lockfree)
find_package(catkin REQUIRED COMPONENTS rosatomic rosconsole roslib allocators)

include_directories(include ${catkin_INCLUDE_DIRS})


catkin_package(
    DEPENDS
    CATKIN_DEPENDS rosatomic allocators allocators rosconsole roslib rosatomic # TODO
    INCLUDE_DIRS include # TODO include
    LIBRARIES ${PROJECT_NAME}# TODO
)

add_library(${PROJECT_NAME} src/free_list.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${Boost_LIBRARIES})
add_dependencies(${PROJECT_NAME} rosatomic_gencpp)

catkin_add_gtest(test_freelist test/test_freelist.cpp)
catkin_add_gtest(test_object_pool test/test_object_pool.cpp)
target_link_libraries(test_object_pool ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
   ARCHIVE DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


install(DIRECTORY test/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
