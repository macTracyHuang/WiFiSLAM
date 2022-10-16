cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
rosbuild_init()

# set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/install)
set(CMAKE_CXX_FLAGS "-std=c++14 -march=native -pipe -O2 -Wall -Wextra -fopenmp -fPIC -pthread")

IF(NOT ROS_BUILD_TYPE)
  SET(ROS_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${ROS_BUILD_TYPE})

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../cmake_modules)

rosbuild_genmsg()
# find_package(PCL 1.12 REQUIRED)

# include_directories(${PCL_INCLUDE_DIRS})
# link_directories(${PCL_LIBRARY_DIRS})
# add_definitions(${PCL_DEFINITIONS})


# rosbuild_add_executable(cloud_viewer_rviz cloud_viewer_rviz.cpp)
# add_executable (cloud_viewer_rviz cloud_viewer_rviz.cpp)
# target_link_libraries (cloud_viewer_rviz ${PCL_LIBRARIES} ${PROJECT_SOURCE_DIR}/../../lib/libORB_SLAM3.so)

# install(TARGETS cloud_viewer_rviz
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})