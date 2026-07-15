# Docker镜像无法下载解决方案

由于**乳包**问题访问国内的镜像源经常无法下载，所以我这里将本项目中我本地下载过的镜像源推送到阿里云镜像仓库

如果大家下载不了`DockerHub`或某些镜像网站的镜像时，可以考虑拉取我推送到阿里云的镜像源

## 镜像列表

下面是我目前推送的镜像如下：

| 仓库名称             | 镜像版本             | 备注                   |
| -------------------- | -------------------- | ---------------------- |
| php                  | 5.4-fpm              |                        |
| alpine               | 3.16                 |                        |
| amazoncorretto       | 8<br>17              | JDK                    |
| nacos-server         | v2.1.0<br>v3.1.1     |                        |
| elasticsearch        | 7.6.2<br>8.19.14     |                        |
| logstash             | 7.6.2<br>8.19.14     |                        |
| kibana               | 7.6.2<br>8.19.14     |                        |
| rocketmq-dashboard   | 1.0.0<br>2.1.0       |                        |
| rocketmq             | 4.9.3<br>5.3.1       |                        |
| nginx                | latest               |                        |
| nginx-proxy-manager  | latest               |                        |
| redis                | 6.2.7<br>7.4.8       |                        |
| redis-stack          | 7.4.0-v8             |                        |
| mongo                | 4.4.17<br>6.0.26     |                        |
| mysql                | 5.6.51<br>8.4.9      |                        |
| shardingsphere-proxy | latest               |                        |
| minio                | latest               |                        |
| dfs                  | 6.09                 | `FastDFS`本地构建镜像  |
| sentinel-dashboard   | 1.8.4<br>1.8.9       | `Sentinel`本地构建镜像 |
| seata-server         | 1.5.1<br>2.5.0.jdk17 |                        |
| seata-naming-server  | 2.5.0.jdk17          |                        |
| jenkins              | lts<br>lts-jdk17     |                        |

## 镜像使用

要拉取镜像前需要先需要登录一下私库，才能访问私库的镜像，执行下面命令行登录

```sh
docker login --username=01xqdocker@1079857282388264 crpi-5yw4kk2bxbk3nj6k.cn-hangzhou.personal.cr.aliyuncs.com
```

登录密码是：01xqdocker

完整镜像名组成规则如下：

```shell
crpi-5yw4kk2bxbk3nj6k.cn-hangzhou.personal.cr.aliyuncs.com/01xq/仓库名称:镜像版本
```

镜像使用，只需要替换掉安装文档中的对应镜像即可。

比如`MySQL`安装文档中的某些步骤

```shell
# 拉取镜像
docker pull mysql:8.4.9
# 创建容器
docker run -id ...... mysql:8.4.9
```

替换成我的镜像

```shell
# 拉取镜像
docker pull crpi-5yw4kk2bxbk3nj6k.cn-hangzhou.personal.cr.aliyuncs.com/01xq/mysql:8.4.9
# 创建容器
docker run -id ...... crpi-5yw4kk2bxbk3nj6k.cn-hangzhou.personal.cr.aliyuncs.com/01xq/mysql:8.4.9
```

再比如`Nacos`安装文档中`standalone-derby.yaml`中配置镜像

```yaml
services:
  nacos:
    image: nacos/nacos-server:v3.1.1
    container_name: nacos-standalone-v31
# ......
```

替换成我的镜像

```yaml
services:
  nacos:
    image: crpi-5yw4kk2bxbk3nj6k.cn-hangzhou.personal.cr.aliyuncs.com/01xq/nacos-server:v3.1.1
    container_name: nacos-standalone-v31
# ......
```

## 其他参考

国外的Docker镜像转存到阿里云私有仓库工具：https://github.com/tech-shrimp/docker_image_pusher
