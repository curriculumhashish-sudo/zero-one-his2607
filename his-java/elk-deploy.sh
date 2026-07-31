#!/bin/bash

# ELK 一键部署脚本
# 适用系统: Alibaba Cloud Linux 3.2104 LTS 64位
# 端口配置:
#   - Logstash: 4560-4563
#   - Kibana: 5601
#   - Elasticsearch: 9200, 9300

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 配置变量
SERVER_IP="114.55.58.120"
ELASTICSEARCH_VERSION="8.11.0"
LOGSTASH_VERSION="8.11.0"
KIBANA_VERSION="8.11.0"

INSTALL_DIR="/opt/elk"
ELASTICSEARCH_PORT="9200"
ELASTICSEARCH_TRANSPORT_PORT="9300"
LOGSTASH_PORT="4560-4563"
KIBANA_PORT="5601"

LOG_FILE="/var/log/elk-deploy-$(date +%Y%m%d_%H%M%S).log"

# 日志函数
log() {
    echo -e "${GREEN}[$(date +'%Y-%m-%d %H:%M:%S')]${NC} $1" | tee -a "$LOG_FILE"
}

log_error() {
    echo -e "${RED}[ERROR]$(date +'%Y-%m-%d %H:%M:%S')${NC} $1" | tee -a "$LOG_FILE"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]$(date +'%Y-%m-%d %H:%M:%S')${NC} $1" | tee -a "$LOG_FILE"
}

# 检查 root 权限
check_root() {
    if [[ $EUID -ne 0 ]]; then
        log_error "此脚本必须以 root 用户运行"
        exit 1
    fi
}

# 检查系统环境
check_system() {
    log "检查系统环境..."
    
    # 检查操作系统
    if ! cat /etc/os-release | grep -qi "Alibaba Cloud Linux"; then
        log_warning "此脚本专为 Alibaba Cloud Linux 设计，当前系统可能不兼容"
    fi
    
    # 检查内存（ELK 至少需要 4GB）
    MEMORY=$(free -g | awk '/^Mem:/{print $2}')
    if [[ $MEMORY -lt 4 ]]; then
        log_warning "系统内存不足 4GB（当前: ${MEMORY}GB），ELK 可能无法正常运行"
    fi
    
    # 检查磁盘空间（至少需要 20GB）
    DISK_SPACE=$(df -BG / | awk 'NR==2 {print $4}' | sed 's/G//')
    if [[ $DISK_SPACE -lt 20 ]]; then
        log_warning "根分区磁盘空间不足 20GB（当前: ${DISK_SPACE}GB）"
    fi
    
    log "系统检查完成"
}

# 安装依赖
install_dependencies() {
    log "安装系统依赖..."
    
    yum update -y
    yum install -y java-17-openjdk java-17-openjdk-devel curl wget tar
    
    # 验证 Java 安装
    if ! command -v java &> /dev/null; then
        log_error "Java 安装失败"
        exit 1
    fi
    
    JAVA_VERSION=$(java -version 2>&1 | head -1 | cut -d '"' -f 2)
    log "Java 版本: $JAVA_VERSION"
    
    log "依赖安装完成"
}

# 创建 ELK 用户和目录
setup_user() {
    log "创建 ELK 用户和目录..."
    
    # 创建 elk 用户
    if ! id elk &> /dev/null; then
        useradd -r -s /bin/false elk
    fi
    
    # 创建安装目录
    mkdir -p "$INSTALL_DIR"
    
    # 创建数据目录
    mkdir -p /var/data/elasticsearch
    mkdir -p /var/log/elasticsearch
    mkdir -p /var/log/logstash
    mkdir -p /var/log/kibana
    
    # 设置权限
    chown -R elk:elk /var/data/elasticsearch
    chown -R elk:elk /var/log/elasticsearch
    chown -R elk:elk /var/log/logstash
    chown -R elk:elk /var/log/kibana
    
    log "用户和目录创建完成"
}

# 下载并安装 Elasticsearch
install_elasticsearch() {
    log "下载并安装 Elasticsearch ${ELASTICSEARCH_VERSION}..."
    
    cd /tmp
    wget -q "https://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-${ELASTICSEARCH_VERSION}-linux-x86_64.tar.gz"
    
    tar -xzf "elasticsearch-${ELASTICSEARCH_VERSION}-linux-x86_64.tar.gz"
    mv "elasticsearch-${ELASTICSEARCH_VERSION}" "$INSTALL_DIR/"
    
    # 配置 Elasticsearch
    cat > "$INSTALL_DIR/elasticsearch-${ELASTICSEARCH_VERSION}/config/elasticsearch.yml" << EOF
cluster.name: elk-cluster
node.name: node-1
node.roles: [master, data, ingest]
path.data: /var/data/elasticsearch
path.logs: /var/log/elasticsearch
network.host: ${SERVER_IP}
http.port: ${ELASTICSEARCH_PORT}
transport.port: ${ELASTICSEARCH_TRANSPORT_PORT}
discovery.seed_hosts: ["127.0.0.1"]
cluster.initial_master_nodes: ["node-1"]
xpack.security.enabled: false
bootstrap.memory_lock: false
EOF
    
    # 设置 JVM 内存
    cat > "$INSTALL_DIR/elasticsearch-${ELASTICSEARCH_VERSION}/config/jvm.options" << EOF
-Xms2g
-Xmx2g
EOF
    
    # 设置权限
    chown -R elk:elk "$INSTALL_DIR/elasticsearch-${ELASTICSEARCH_VERSION}"
    
    # 创建软链接
    ln -sf "$INSTALL_DIR/elasticsearch-${ELASTICSEARCH_VERSION}" "$INSTALL_DIR/elasticsearch"
    
    log "Elasticsearch 安装完成"
}

# 下载并安装 Logstash
install_logstash() {
    log "下载并安装 Logstash ${LOGSTASH_VERSION}..."
    
    cd /tmp
    wget -q "https://artifacts.elastic.co/downloads/logstash/logstash-${LOGSTASH_VERSION}.tar.gz"
    
    tar -xzf "logstash-${LOGSTASH_VERSION}.tar.gz"
    mv "logstash-${LOGSTASH_VERSION}" "$INSTALL_DIR/"
    
    # 配置 Logstash
    cat > "$INSTALL_DIR/logstash-${LOGSTASH_VERSION}/config/logstash.yml" << EOF
path.data: /var/lib/logstash
path.config: ${INSTALL_DIR}/logstash-${LOGSTASH_VERSION}/pipeline
path.logs: /var/log/logstash
http.host: ${SERVER_IP}
http.port: ${LOGSTASH_PORT}
EOF
    
    # 创建示例管道配置
    mkdir -p "$INSTALL_DIR/logstash-${LOGSTASH_VERSION}/pipeline"
    cat > "$INSTALL_DIR/logstash-${LOGSTASH_VERSION}/pipeline/logstash.conf" << EOF
input {
  beats {
    port => 5044
  }
  http {
    host => "${SERVER_IP}"
    port => 4560
  }
  tcp {
    host => "${SERVER_IP}"
    port => 4561
    codec => json_lines
  }
  syslog {
    host => "${SERVER_IP}"
    port => 4562
  }
  udp {
    host => "${SERVER_IP}"
    port => 4563
  }
}

filter {
  # 添加过滤逻辑
  if [type] == "nginx" {
    grok {
      match => { "message" => "%{NGINXACCESS}" }
    }
  }
}

output {
  elasticsearch {
    hosts => ["http://${SERVER_IP}:${ELASTICSEARCH_PORT}"]
    index => "%{[@metadata][beat]}-%{+YYYY.MM.dd}"
  }
  
  stdout {
    codec => rubydebug
  }
}
EOF
    
    # 设置权限
    chown -R elk:elk "$INSTALL_DIR/logstash-${LOGSTASH_VERSION}"
    
    # 创建软链接
    ln -sf "$INSTALL_DIR/logstash-${LOGSTASH_VERSION}" "$INSTALL_DIR/logstash"
    
    log "Logstash 安装完成"
}

# 下载并安装 Kibana
install_kibana() {
    log "下载并安装 Kibana ${KIBANA_VERSION}..."
    
    cd /tmp
    wget -q "https://artifacts.elastic.co/downloads/kibana/kibana-${KIBANA_VERSION}-linux-x86_64.tar.gz"
    
    tar -xzf "kibana-${KIBANA_VERSION}-linux-x86_64.tar.gz"
    mv "kibana-${KIBANA_VERSION}" "$INSTALL_DIR/"
    
    # 配置 Kibana
    cat > "$INSTALL_DIR/kibana-${KIBANA_VERSION}/config/kibana.yml" << EOF
server.port: ${KIBANA_PORT}
server.host: "${SERVER_IP}"
server.name: "elk-kibana"
elasticsearch.hosts: ["http://${SERVER_IP}:${ELASTICSEARCH_PORT}"]
elasticsearch.username: ""
elasticsearch.password: ""
logging.root.level: info
logging.dest: /var/log/kibana/kibana.log
EOF
    
    # 设置权限
    chown -R elk:elk "$INSTALL_DIR/kibana-${KIBANA_VERSION}"
    
    # 创建软链接
    ln -sf "$INSTALL_DIR/kibana-${KIBANA_VERSION}" "$INSTALL_DIR/kibana"
    
    log "Kibana 安装完成"
}

# 创建 systemd 服务
create_services() {
    log "创建 systemd 服务..."
    
    # Elasticsearch 服务
    cat > /etc/systemd/system/elasticsearch.service << EOF
[Unit]
Description=Elasticsearch
After=network.target

[Service]
Type=simple
User=elk
Group=elk
WorkingDirectory=${INSTALL_DIR}/elasticsearch
ExecStart=${INSTALL_DIR}/elasticsearch/bin/elasticsearch
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF
    
    # Logstash 服务
    cat > /etc/systemd/system/logstash.service << EOF
[Unit]
Description=Logstash
After=network.target elasticsearch.service
Requires=elasticsearch.service

[Service]
Type=simple
User=elk
Group=elk
WorkingDirectory=${INSTALL_DIR}/logstash
ExecStart=${INSTALL_DIR}/logstash/bin/logstash
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF
    
    # Kibana 服务
    cat > /etc/systemd/system/kibana.service << EOF
[Unit]
Description=Kibana
After=network.target elasticsearch.service logstash.service
Requires=elasticsearch.service logstash.service

[Service]
Type=simple
User=elk
Group=elk
WorkingDirectory=${INSTALL_DIR}/kibana
ExecStart=${INSTALL_DIR}/kibana/bin/kibana
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF
    
    # 重载 systemd
    systemctl daemon-reload
    
    # 设置开机自启
    systemctl enable elasticsearch
    systemctl enable logstash
    systemctl enable kibana
    
    log "服务创建完成"
}

# 配置防火墙
configure_firewall() {
    log "配置防火墙..."
    
    # 开放端口
    firewall-cmd --permanent --add-port=${ELASTICSEARCH_PORT}/tcp
    firewall-cmd --permanent --add-port=${ELASTICSEARCH_TRANSPORT_PORT}/tcp
    firewall-cmd --permanent --add-port=4560/tcp
    firewall-cmd --permanent --add-port=4561/tcp
    firewall-cmd --permanent --add-port=4562/tcp
    firewall-cmd --permanent --add-port=4563/tcp
    firewall-cmd --permanent --add-port=${KIBANA_PORT}/tcp
    firewall-cmd --reload
    
    log "防火墙配置完成"
}

# 启动服务
start_services() {
    log "启动 ELK 服务..."
    
    # 启动 Elasticsearch
    log "启动 Elasticsearch..."
    systemctl start elasticsearch
    sleep 30
    
    # 检查 Elasticsearch 是否启动成功
    if curl -s http://localhost:${ELASTICSEARCH_PORT} > /dev/null; then
        log "Elasticsearch 启动成功"
    else
        log_error "Elasticsearch 启动失败"
        journalctl -u elasticsearch -n 50 --no-pager
        exit 1
    fi
    
    # 启动 Logstash
    log "启动 Logstash..."
    systemctl start logstash
    sleep 15
    
    # 启动 Kibana
    log "启动 Kibana..."
    systemctl start kibana
    sleep 15
    
    log "所有服务启动完成"
}

# 显示状态
show_status() {
    log "=========================================="
    log "ELK 部署完成！"
    log "=========================================="
    log "服务器 IP: ${SERVER_IP}"
    log "Elasticsearch: http://${SERVER_IP}:${ELASTICSEARCH_PORT}"
    log "Logstash: http://${SERVER_IP}:4560-${LOGSTASH_PORT}"
    log "Kibana: http://${SERVER_IP}:${KIBANA_PORT}"
    log "=========================================="
    log "服务状态:"
    systemctl status elasticsearch --no-pager | grep -E "Active:"
    systemctl status logstash --no-pager | grep -E "Active:"
    systemctl status kibana --no-pager | grep -E "Active:"
    log "=========================================="
    log "日志文件:"
    log "  Elasticsearch: /var/log/elasticsearch/"
    log "  Logstash: /var/log/logstash/"
    log "  Kibana: /var/log/kibana/"
    log "=========================================="
}

# 主函数
main() {
    log "开始部署 ELK 堆栈..."
    log "版本: ES=${ELASTICSEARCH_VERSION}, Logstash=${LOGSTASH_VERSION}, Kibana=${KIBANA_VERSION}"
    
    check_root
    check_system
    install_dependencies
    setup_user
    install_elasticsearch
    install_logstash
    install_kibana
    create_services
    configure_firewall
    start_services
    show_status
    
    log "ELK 部署完成！"
}

# 执行主函数
main
