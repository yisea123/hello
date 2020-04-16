项目代码：
Project -- 替换成自己项目立项名
code -- 存放代码
  ||||
LIB 存放不用更改的库文件（公司库，图像库，st官方库等）   
Src 存放所有用户源码（方便代码版本管理提交）
MDK-Proj存放Keil的工程文件（方便后期不同IDE平台的切换，保证代码安全）

Src目录下
根据项目需求定，通常包括
tool：项目使用工具
drv：项目接口
config：项目配置
...其他根据需求
公共文件

项目结构：
在main中，调用systeminit()，appinit()进行初始化操作
在程序的大while(1)中，放系统轮询systemexec和用户轮询MainTask()


用户代码管理：MainTask() --
包括项目需要的状态机管理，错误管理，逻辑管理，ui逻辑管理，其他接口


文件夹：drv  接口驱动
提供设备运行的各种接口驱动接口。


tool: 
常用api -- 轴，io接口

