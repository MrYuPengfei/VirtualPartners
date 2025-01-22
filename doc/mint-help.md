以下是一个完整的脚本，用于在 Linux Mint 22.1 上安装 Docker：bash# 1. 安装必要的依赖
sudo apt update
sudo apt install -y apt-transport-https ca-certificates curl gnupg

# 2. 导入 Docker 的 GPG 密钥
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker.gpg

# 3. 添加 Docker 的 APT 仓库
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu jammy stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# 4. 更新包列表
sudo apt update

# 5. 安装 Docker
sudo apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# 6. 验证 Docker 是否安装成功
sudo docker run hello-world

# 7. 将用户添加到 Docker 组
sudo usermod -aG docker ${USER}
注意事项1. 版本代号匹配：确保使用正确的 Ubuntu 版本代号（如   jammy  ）。2. 重新登录：在将用户添加到   docker   组后，需要重新登录或重启系统。3. 网络问题：如果你在中国大陆地区，可能需要配置 Docker 的镜像加速器。