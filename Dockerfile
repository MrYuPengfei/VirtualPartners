# 基于 Ubuntu 22.04
FROM ubuntu:22.04

# 安装必要的依赖
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    qt6-multimedia-dev \
    qt6-tools-dev \
    libvulkan-dev \
    libxkbcommon-dev \
    build-essential \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# 安装 Linux Mint 的特定软件包（可选）
# 例如，安装 Cinnamon 桌面环境
RUN apt-get update && apt-get install -y \
    cinnamon \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 将项目文件复制到工作目录
COPY . /app

# 构建项目
RUN mkdir build && cd build \
    && qmake .. \
    && make

# 设置环境变量
ENV QT_PLUGIN_PATH=/app/build/plugins
ENV LD_LIBRARY_PATH=/app/build/lib:$LD_LIBRARY_PATH

# # 暴露运行端口（如果有需要）
# EXPOSE 8000

# 运行应用程序
CMD ["./build/VirtualPartners"]
