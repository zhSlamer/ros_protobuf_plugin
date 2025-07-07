# ros-protobuf-plugin使用方式

## 源码下载

```
git clone https://github.com/zhSlamer/ros_protobuf_plugin.git
```

## 1 插件编译与安装

**对于 ros_protobuf_msg 代码首先编译插件并安装**

```
mkdir build
cd build
cmake ..
make
```

**插件安装的目的是**

1 将 proto 偏特化的序列化和反序列化代码放入到 ros 底层文件夹中

    message_serialization/protobuffer_serialization.h

2 将 proto 偏特化的特征数据反射代码放入到 ros 底层文件中

    message_traits/protobuf_traits.h

## 2 plugin 的使用

1. **创建自定义的 proto 文件**publish_info.proto
2. **源程序中引入头文件即可**

   ```
   #include <ros/protobuffer_traits.h>
   #include <ros/serialization_protobuffer.h>
   #include "ros/ros.h"
   #include "publish_info.pb.h"
   ```
3. 直接发布proto数据

   ```
   superbai::sample::PublishInfo proto_msg_info;
   proto_msg_info.set_name("zhaoheng");
   proto_msg_info.set_message_type("test_message");
   // proto_msg_info.set_publish_msg(std::string(1024*1024, '#'));
   proto_msg_info.set_publish_msg("This is new publish");

   pub.publish(proto_msg_info);
   ```

**具体样例可以见 test_ros_protobuf 以及 example/plugin_use_example**

## 3 plugin_use_example 样例使用

**首先进入到样例代码**

```
cd example/plugin_use_example/test_ros_protobuf
mkdir build
cmake ..
make
```

生成 pb_talk 和 pb_listener 可执行程序

```
source devel/setup.bash
rosrun example pb_talker
rosrun example pb_listener
```

# docker内使用

## 1.通过项目中dockerfile文件，构建项目镜像

```bash
cd ~/work/ros_protobuf_msg/docker/build
docker build --network host -t ros_protobuf:noetic  -f ros_x86.dockerfile .
```

## 2.进入docker容器

```bash
cd ~/work/ros_protobuf_msg/docker/scripts
#启动容器
./ros_docker_run.sh
#进入容器
./ros_docker_into.sh
```

## 3.编译代码

```bash
#创建build目录
mkdir build
cd build
cmake ..
make -j6
```

## 4.启动程序

```bash
#先启动roscore，并且启动pb_talker节点
cd /work
source devel/setup.bash
roscore &
rosrun ros_protobuf pb_talker
```

```bash
#打开新终端，再次进入容器，启动pb_listener节点
#进入容器中
cd ~/work/ros_protobuf_msg/docker/scripts
#进入容器
./ros_docker_into.sh
rosrun ros_protobuf pb_listener
```
