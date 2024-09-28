Retro Centipede Game 
Welcome to the Retro Centipede Game, a simplified version of the classic Atari game Centipede, created as part of ECE6122 coursework. In this game, players navigate a ship to shoot at the centipede while avoiding enemies. The game offers a nostalgic arcade experience with modern implementation techniques.欢迎来到 Retro Centipede Game，这是经典 Atari 游戏 Centipede 的简化版本，作为ECE6122课程的一部分创建。在这个游戏中，玩家驾驶一艘船向蜈蚣射击，同时避开敌人。该游戏通过现代实现技术提供怀旧的街机体验。

Project Overview 项目概况
This project is developed in C++ using the SFML (Simple and Fast Multimedia Library) for graphics and audio. The game features basic movement, collision detection, and enemy behavior, all closely modeled after the original Centipede gameplay.该项目使用 SFML（简单快速多媒体库）在 C++ 中开发图形和音频。该游戏具有基本的移动、碰撞检测和敌人行为，所有这些都与原始的 Centipede 游戏玩法密切相关。

How to Build the Game如何构建游戏
To build and run the game, you'll need to use CMake. Follow the steps below:要构建和运行游戏，您需要使用 CMake。请按照以下步骤操作：

Clone the Repository:克隆存储库：

git clone --recurse-submodules https://github.com/KaiLyee/RetroCentipedeGame.git
Configure and Build with CMake: You'll need to specify the paths for your libraries and dependencies. Run the following commands in the project directory:使用 CMake 进行配置和构建：您需要指定库和依赖项的路径。在项目目录中执行以下命令：

mkdir build
cd build
cmake ..
make
Run the Game: After building the project, you can run the executable file generated in the build directory.Run the Game：构建完项目后，可以运行 build 目录下生成的可执行文件。

Dependencies依赖
SFML: This project uses the SFML library for multimedia functionalities. Make sure SFML is correctly set up in your environment.SFML：该项目使用 SFML 库来实现多媒体功能。确保在您的环境中正确设置 SFML。
For more details on configuring CMake for your system, consult the CMake Documentation.有关为您的系统配置 CMake 的更多详细信息，请参阅 CMake 文档。

Feel free to update this README as the project evolves.:) 随着项目的发展，请随时更新此 README。
