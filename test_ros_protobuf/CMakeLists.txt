
add_subdirectory(proto_data)

# 创建四个可执行文件
add_executable(pb_talker pb_talker.cpp)
add_executable(pb_listener pb_listener.cpp)
add_executable(talker talker.cpp)
add_executable(listener listener.cpp)

# 添加自定义依赖
add_dependencies(pb_talker copy_traits_files copy_serialization_files)
add_dependencies(pb_listener copy_traits_files copy_serialization_files)

# 添加自定义依赖
add_dependencies(pb_talker copy_traits_files copy_serialization_files)
add_dependencies(pb_listener copy_traits_files copy_serialization_files)

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