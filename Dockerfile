# 基础镜像
FROM ubuntu:20.04

# 设置时区
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 安装必要的依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt5-default \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    libssl-dev \
    wget \
    git \
    && rm -rf /var/lib/apt/lists/*

# 创建工作目录
WORKDIR /app

# 复制项目文件
COPY . /app

# 创建构建目录
RUN mkdir -p build
WORKDIR /app/build

# 配置CMake
RUN cmake ..

# 编译项目
RUN cmake --build . --config Release

# 设置环境变量
ENV QT_QPA_PLATFORM=xcb

# 暴露端口（如果需要）
# EXPOSE 8080

# 运行程序
CMD ["../build/gomoku_game"]
