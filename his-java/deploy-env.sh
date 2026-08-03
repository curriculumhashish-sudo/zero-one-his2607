#!/bin/bash
#=============================================================================
# 服务器环境初始化脚本
# 目标服务器: 121.199.54.234 (全新阿里云服务器)
# 功能: 安装 Docker、Docker Compose、Jenkins、JDK 17、Maven、Git
# 使用: sudo bash deploy-env.sh
#=============================================================================

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log()     { echo -e "${GREEN}[$(date +'%H:%M:%S')]${NC} $1"; }
log_warn(){ echo -e "${YELLOW}[WARN]${NC} $1"; }
log_err() { echo -e "${RED}[ERROR]${NC} $1"; }

# 检查 root
if [[ $EUID -ne 0 ]]; then
    log_err "请以 root 用户运行: sudo bash deploy-env.sh"
    exit 1
fi

log "===== 开始服务器环境初始化 ====="

#------------- 1. 系统更新 + 基础工具 -------------
log "安装基础工具..."
yum update -y
yum install -y curl wget git vim tar unzip yum-utils

#------------- 2. 安装 Docker + Docker Compose -------------
log "安装 Docker..."
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
yum install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin

systemctl start docker
systemctl enable docker

DOCKER_VER=$(docker --version)
log "Docker 安装完成: ${DOCKER_VER}"
log "Docker Compose 版本: $(docker compose version)"

#------------- 3. 安装 JDK 17 -------------
log "安装 JDK 17..."
yum install -y java-17-openjdk java-17-openjdk-devel

JAVA_HOME_17=$(dirname $(dirname $(readlink -f $(which java))))
log "JDK 17 安装完成: ${JAVA_HOME_17}"

#------------- 4. 安装 Maven -------------
log "安装 Maven..."
MAVEN_VERSION="3.9.9"
cd /tmp
wget -q "https://dlcdn.apache.org/maven/maven-3/${MAVEN_VERSION}/binaries/apache-maven-${MAVEN_VERSION}-bin.tar.gz"
tar xf "apache-maven-${MAVEN_VERSION}-bin.tar.gz" -C /opt
ln -sf "/opt/apache-maven-${MAVEN_VERSION}" /opt/maven

cat > /etc/profile.d/maven.sh << 'MAVEN_ENV'
export MAVEN_HOME=/opt/maven
export PATH=$MAVEN_HOME/bin:$PATH
MAVEN_ENV
source /etc/profile.d/maven.sh

log "Maven 安装完成: $(mvn --version | head -1)"

#------------- 5. 安装 Jenkins -------------
log "安装 Jenkins..."
wget -O /etc/yum.repos.d/jenkins.repo https://pkg.jenkins.io/redhat-stable/jenkins.repo
rpm --import https://pkg.jenkins.io/redhat-stable/jenkins.io-2023.key
yum install -y jenkins

# 配置 Jenkins 使用 JDK 17
sed -i 's|/usr/bin/java|/usr/lib/jvm/java-17-openjdk/bin/java|g' /etc/sysconfig/jenkins 2>/dev/null || true

# 将 jenkins 用户加入 docker 组（让 Jenkins 能执行 docker 命令）
usermod -aG docker jenkins

systemctl start jenkins
systemctl enable jenkins

log "等待 Jenkins 启动..."
sleep 15

JENKINS_PASS=$(cat /var/lib/jenkins/secrets/initialAdminPassword 2>/dev/null || echo "未找到")
log "Jenkins 安装完成"
log "  访问地址: http://121.199.54.234:8080"
log "  初始密码: ${JENKINS_PASS}"

#------------- 6. 配置 Jenkins 的 Maven settings.xml -------------
log "配置 Jenkins Maven settings.xml..."
JENKINS_M2_DIR="/var/lib/jenkins/.m2"
mkdir -p "${JENKINS_M2_DIR}"

cat > "${JENKINS_M2_DIR}/settings.xml" << 'SETTINGS_XML'
<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0
          http://maven.apache.org/xsd/settings-1.0.0.xsd">
    <servers>
        <server>
            <id>2383166-snapshot-6TSW5l</id>
            <username>64794d2073120cb7e7ef6f1a</username>
            <password>hnC])Je1TEBu</password>
        </server>
    </servers>

    <mirrors>
        <mirror>
            <id>mirror</id>
            <mirrorOf>central,jcenter,!2383166-snapshot-6TSW5l</mirrorOf>
            <name>aliyun mirror</name>
            <url>https://maven.aliyun.com/nexus/content/groups/public</url>
        </mirror>
    </mirrors>

    <profiles>
        <profile>
            <id>java17</id>
            <activation>
                <activeByDefault>true</activeByDefault>
                <jdk>17</jdk>
            </activation>
            <properties>
                <maven.compiler.source>17</maven.compiler.source>
                <maven.compiler.target>17</maven.compiler.target>
            </properties>
        </profile>
        <profile>
            <id>rdc</id>
            <properties>
                <altReleaseDeploymentRepository>
                    2383166-snapshot-6TSW5l::default::https://packages.aliyun.com/maven/repository/2383166-snapshot-6TSW5l
                </altReleaseDeploymentRepository>
                <altSnapshotDeploymentRepository>
                    2383166-snapshot-6TSW5l::default::https://packages.aliyun.com/maven/repository/2383166-snapshot-6TSW5l
                </altSnapshotDeploymentRepository>
            </properties>
            <repositories>
                <repository>
                    <id>central</id>
                    <url>https://maven.aliyun.com/nexus/content/groups/public</url>
                    <releases><enabled>true</enabled></releases>
                    <snapshots><enabled>false</enabled></snapshots>
                </repository>
                <repository>
                    <id>2383166-snapshot-6TSW5l</id>
                    <url>https://packages.aliyun.com/maven/repository/2383166-snapshot-6TSW5l</url>
                    <releases><enabled>true</enabled></releases>
                    <snapshots><enabled>true</enabled></snapshots>
                </repository>
            </repositories>
            <pluginRepositories>
                <pluginRepository>
                    <id>2383166-snapshot-6TSW5l</id>
                    <url>https://packages.aliyun.com/maven/repository/2383166-snapshot-6TSW5l</url>
                    <releases><enabled>true</enabled></releases>
                    <snapshots><enabled>true</enabled></snapshots>
                </pluginRepository>
            </pluginRepositories>
        </profile>
    </profiles>

    <activeProfiles>
        <activeProfile>java17</activeProfile>
        <activeProfile>rdc</activeProfile>
    </activeProfiles>
</settings>
SETTINGS_XML

chown -R jenkins:jenkins "${JENKINS_M2_DIR}"
log "Jenkins Maven settings.xml 配置完成"

#------------- 7. 配置防火墙 -------------
log "配置防火墙..."
if command -v firewall-cmd &> /dev/null; then
    firewall-cmd --permanent --add-port=8080/tcp
    firewall-cmd --permanent --add-port=8848/tcp
    firewall-cmd --permanent --add-port=9848/tcp
    firewall-cmd --permanent --add-port=10500/tcp
    firewall-cmd --permanent --add-port=10501/tcp
    firewall-cmd --permanent --add-port=3306/tcp
    firewall-cmd --reload
    log "防火墙配置完成"
else
    log_warn "firewall-cmd 未安装，请手动配置安全组规则"
fi

#------------- 8. 阿里云安全组提醒 -------------
log_warn "============================================"
log_warn "  重要：请在阿里云控制台安全组中开放以下端口"
log_warn "============================================"
log_warn "  8080  - Jenkins"
log_warn "  8848  - Nacos 控制台"
log_warn "  9848  - Nacos gRPC (Nacos 2.x 客户端用)"
log_warn "  10500 - his-j5-ons"
log_warn "  10501 - his-j5-configuration"
log_warn "  3306  - MySQL (可选，调试用)"
log_warn "  22    - SSH (通常已开放)"
log_warn "============================================"

#------------- 9. 创建部署目录 -------------
log "创建部署目录..."
mkdir -p /opt/his-deploy

#------------- 完成汇总 -------------
log "===== 服务器环境初始化完成 ====="
log ""
log "已安装组件:"
log "  Docker:       ${DOCKER_VER}"
log "  JDK:          $(java -version 2>&1 | head -1)"
log "  Maven:        $(mvn --version | head -1)"
log "  Git:          $(git --version)"
log "  Jenkins:      http://121.199.54.234:8080 (初始密码: ${JENKINS_PASS})"
log ""
log "后续步骤:"
log "  1. 访问 http://121.199.54.234:8080，使用初始密码解锁 Jenkins"
log "  2. 安装建议插件"
log "  3. 配置 JDK17 和 Maven3 (Global Tool Configuration)"
log "  4. 添加 codeup-credentials 凭据 (阿里云云效 HTTPS 账号密码)"
log "  5. 创建 Pipeline 任务，指向项目 Jenkinsfile (分支 j5)"
log "  6. 将 init-sql/ 目录放入 SQL 初始化文件后执行 Pipeline"
