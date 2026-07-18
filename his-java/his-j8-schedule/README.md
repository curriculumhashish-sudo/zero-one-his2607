# his-j8-schedule

## 模块说明

定时任务服务模块，提供定时任务调度功能。

## 技术栈

- Java 21
- Spring Boot 3.2.x
- Spring Cloud Alibaba
- Nacos Discovery & Config
- MyBatis-Plus
- MySQL

## 启动方式

### 开发环境

```bash
mvn spring-boot:run
```

### 生产环境

```bash
mvn clean package
java -jar target/his-j8-schedule-1.0.0-SNAPSHOT.jar
```

## 配置说明

| 配置项 | 说明 | 默认值 |
| :--- | :--- | :--- |
| `sp.schedule` | 服务端口 | 10222 |
| `sn.schedule` | 服务名称 | his-j8-schedule |
| `nacos.server-addr` | Nacos 地址 | - |

## Docker 部署

```bash
mvn clean package docker:build
docker run -d -p 10222:10222 --name his-j8-schedule 01star/his-j8-schedule:1.0.0-SNAPSHOT
```
