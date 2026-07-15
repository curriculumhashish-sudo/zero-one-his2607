#!/bin/bash

# ========== 配置区 ==========
MYSQL_ROOT_PASSWORD="YourStrongPassword123!"
REDIS_PASSWORD="YourRedisPassword123!"
SERVER_IP=$(hostname -I | awk '{print $1}')
DATA_DIR="/data"

# ========== 基础准备 ==========
echo ">>> 安装基础工具..."
sudo yum update -y
sudo yum install -y wget curl vim net-tools telnet unzip tar git
sudo systemctl stop firewalld
sudo systemctl disable firewalld
sudo setenforce 0

# ========== 安装 JDK 17 ==========
echo ">>> 安装 JDK 17..."
sudo yum install -y java-17-openjdk java-17-openjdk-devel
echo 'export JAVA_HOME=/usr/lib/jvm/java-17-openjdk' | sudo tee /etc/profile.d/java.sh
echo 'export PATH=$JAVA_HOME/bin:$PATH' | sudo tee -a /etc/profile.d/java.sh
source /etc/profile.d/java.sh

# ========== 安装 Docker ==========
echo ">>> 安装 Docker..."
sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/anolis/docker-ce.repo
sudo yum install -y docker-ce docker-ce-cli containerd.io
sudo systemctl start docker
sudo systemctl enable docker

# ========== 创建数据目录 ==========
mkdir -p ${DATA_DIR}/{mysql/{data,logs,conf},redis/{data,conf},nacos/{logs,conf},seata/{logs,conf},rocketmq/{namesrv,broker,logs},nginx/{conf,html,logs}}

# ========== 部署 MySQL ==========
echo ">>> 部署 MySQL 8.4.9..."
docker run -d --name mysql-8.4 --restart always -p 3306:3306 \
  -v ${DATA_DIR}/mysql/data:/var/lib/mysql \
  -e MYSQL_ROOT_PASSWORD=${MYSQL_ROOT_PASSWORD} \
  -e TZ=Asia/Shanghai \
  mysql:8.4.9

# 等待 MySQL 启动
sleep 30
docker exec mysql-8.4 mysql -uroot -p${MYSQL_ROOT_PASSWORD} -e "
CREATE DATABASE IF NOT EXISTS zero_one_his DEFAULT CHARACTER SET utf8mb4;
CREATE DATABASE IF NOT EXISTS nacos DEFAULT CHARACTER SET utf8mb4;
CREATE USER IF NOT EXISTS 'his_user'@'%' IDENTIFIED BY 'HisUser@2024';
GRANT ALL PRIVILEGES ON *.* TO 'his_user'@'%';
FLUSH PRIVILEGES;"

# ========== 部署 Redis ==========
echo ">>> 部署 Redis 7.4.8..."
cat > ${DATA_DIR}/redis/conf/redis.conf <<EOF
bind 0.0.0.0
port 6379
requirepass ${REDIS_PASSWORD}
maxmemory 2gb
maxmemory-policy allkeys-lru
appendonly yes
EOF

docker run -d --name redis-7.4 --restart always -p 6379:6379 \
  -v ${DATA_DIR}/redis/data:/data \
  -v ${DATA_DIR}/redis/conf/redis.conf:/usr/local/etc/redis/redis.conf \
  redis:7.4.8 redis-server /usr/local/etc/redis/redis.conf

# ========== 部署 Nacos ==========
echo ">>> 部署 Nacos 3.1.1..."
docker run -d --name nacos-3.1 --restart always -p 8848:8848 -p 9848:9848 \
  -e MODE=standalone \
  -e SPRING_DATASOURCE_PLATFORM=mysql \
  -e MYSQL_SERVICE_HOST=${SERVER_IP} \
  -e MYSQL_SERVICE_PORT=3306 \
  -e MYSQL_SERVICE_DB_NAME=nacos \
  -e MYSQL_SERVICE_USER=his_user \
  -e MYSQL_SERVICE_PASSWORD=HisUser@2024 \
  nacos/nacos-server:v3.1.1

# ========== 部署 Sentinel ==========
echo ">>> 部署 Sentinel 1.8.9..."
docker run -d --name sentinel-1.8.9 --restart always -p 8858:8858 \
  bladex/sentinel-dashboard:1.8.9

# ========== 部署 RocketMQ NameServer ==========
echo ">>> 部署 RocketMQ 5.3.1..."
docker run -d --name rmqnamesrv --restart always -p 9876:9876 \
  apache/rocketmq:5.3.1 sh mqnamesrv

# ========== 部署 Nginx ==========
echo ">>> 部署 Nginx..."
docker run -d --name nginx --restart always -p 80:80 \
  nginx:latest

echo "========================================"
echo "✅ 基础环境部署完成！"
echo "========================================"
echo "MySQL:     ${SERVER_IP}:3306  (root/${MYSQL_ROOT_PASSWORD})"
echo "Redis:     ${SERVER_IP}:6379  (auth: ${REDIS_PASSWORD})"
echo "Nacos:     http://${SERVER_IP}:8848/nacos"
echo "Sentinel:  http://${SERVER_IP}:8858"
echo "RocketMQ:  ${SERVER_IP}:9876"
echo "========================================"
echo "⚠️  请手动部署 Seata（需配置 Nacos 注册）"
echo "⚠️  请根据项目需求配置 Nacos 数据库初始化 SQL"
echo "⚠️  生产环境请修改默认密码并配置 SSL/HTTPS"