cmake_minimum_required(VERSION 3.10)
project(example)

find_package(catkin REQUIRED COMPONENTS
  roscpp_traits rostime
  roscpp
  roslib
  std_msgs
  )

catkin_package()

add_subdirectory(sample_protos)

include_directories(${roscpp_INCLUDE_DIRS})
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)


# 创建四个可执行文件
add_executable(pb_talker pb_talker.cpp)
add_executable(pb_listener pb_listener.cpp)
add_executable(talker talker.cpp)
add_executable(listener listener.cpp)

# 设置链接库
target_link_libraries(pb_talker 
  ${roscpp_LIBRARIES} 
  pb_proto
  # roscpp_proto_serialization
)

target_link_libraries(pb_listener 
  ${roscpp_LIBRARIES} 
  pb_proto
  # roscpp_proto_serialization
)

target_link_libraries(talker 
  ${roscpp_LIBRARIES} 
  ${std_msgs_LIBRARIES}
)

target_link_libraries(listener 
  ${roscpp_LIBRARIES} 
  ${std_msgs_LIBRARIES}
)