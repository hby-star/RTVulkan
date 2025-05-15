# RTVulkan

# 1. 编译

* 使用VS2022打开RTVulkan.sln文件。
* 检查并更改以下项目设置（和Vulkan Turorial中的环境配置步骤相同）

<img src="C:\Users\Admin\Desktop\Learn\RayTracing\RTVulkan\image\image-20250515135005880.png" alt="image-20250515135005880" style="zoom: 50%;" />

<img src="C:\Users\Admin\Desktop\Learn\RayTracing\RTVulkan\image\image-20250515135022419.png" alt="image-20250515135022419" style="zoom:50%;" />

<img src="C:\Users\Admin\Desktop\Learn\RayTracing\RTVulkan\image\image-20250515135039981.png" alt="image-20250515135039981" style="zoom:50%;" />

* 在VS2022中运行项目。

## 2. 运行

* 可自行编译，也可直接使用x64文件夹中构建好的exe文件。
* 可使用WASD移动相机。
* 观察控制台获得每秒的帧数信息。
* 优先选择独立显卡，可在main.cpp中更改preferHighPerformanceGPU的值切换使用集成显卡
* 默认分辨率为1024x768。可通过修改main.cpp和shader.comp中的Width和Height的值改变分辨率。
