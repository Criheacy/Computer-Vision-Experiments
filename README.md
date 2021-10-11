<h1 align="center">
  Criheacy's Computer Vision Experiments
</h1>

## 如何使用 How to Use

首先需要说明，整个实验是在 Linux 环境下使用 CMake 编译的；而我使用的是 `WSL2 (Windows Subsystem for Linux)`，因此只提供了源代码、资源文件（比如实验用的图片）和 CMake 编译需要的 `CMakeLists.txt`。

### 对于 Linux 环境

0. 确保已经安装了 CMake 和 OpenCV

	> 安装指令：
	> 
	> CMake:
	> ```
	> sudo apt install cmake
	> ```
	> 
	> OpenCV:
	> 
	> ```
	> sudo apt install libopencv-dev python3-opencv
	> ```

1. 切换到对应的实验根目录
	> 比如切换到实验一 `1-contrast-and-saturation`:
	> 
	> ```
	> cd 1-contrast-and-saturation
	> ```

2. `mkdir build && cd build`

3. `cmake ..`

4. `make`

5. 执行对应的可执行文件
	> 比如实验一 `1-contrast-and-saturation`:
	> 
	> ```
	> ./1-contrast-and-saturation
	> ```

对于 `WSL` 用户，运行程序之后可能会出现无法显示图片的问题。参考[这篇回答](https://stackoverflow.com/a/43399827/13876457)和[这篇回答](https://stackoverflow.com/a/68341165/13876457)，大致的解决步骤如下：

1. 在 WSL 的 linux 系统中设置 `export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0.0`。这条指令可以直接在命令行中输入，也可以存入 `.bashrc` 文件中（永久设置）。它的意思是将窗口显示的信息转换为上传到 `localhost:0.0` 这个端口。

2. 在 windows 系统中安装 [VcXsrv](https://sourceforge.net/projects/vcxsrv/) 。这个软件的功能是接受来自 `localhost:0.0` 这个端口的信息，并创建一个 GUI 页面来显示它。如果显示连接失败之类的报错，可以考虑将 `XLaunch`（`VcXsrv`的启动器）中 `Disable access control` 选项打开，用于接受来自所有连接的信息。

### 对于 Windows 环境

自行设置 `Visual Studio` 来编译给出的源代码；或者使用 `CLion` 之类的 `CMake` 工具来读取 `CMakeLists.txt` 进行编译。注意可能需要更改 `CMakeLists.txt` 文件中的环境变量位置内容，具体请自行查阅教程（我就是因为 `Visual Studio` 太笨重以及 windows 下的 `CMake` 环境不会配才用的 linux ）
