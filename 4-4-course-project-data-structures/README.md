# Course Project -- Data Structures

北邮计算机类本科二年级数据结构课程设计课程作业。

:warning:不能保证正确性，请不要作为作业提交。

仅包含后端部分，采用 dijkstra 算法，暴露RESTful API。

## build

安装vcpkg，安装jsoncpp和restbed包，修改CMakeLists.txt中INC\_DIR和LINK\_DIR变量的前缀为vcpkg的安装路径

```shell
mkdir build
cd build
cmake ..
make
```