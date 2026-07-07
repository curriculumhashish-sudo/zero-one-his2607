
---
name: create-module
description: 在 Java 微服务项目中按照既定规范创建新的 Maven 模块。当用户要求创建新模块、添加新服务模块或搭建新业务模块时使用此技能。
---

# 创建模块

## 操作指南

按照以下步骤在项目中创建一个新模块。

### 第 1 步：收集需求

向用户询问以下信息：

1. **模块前缀**：如 `project`、`biz`、`svc`（模块 artifactId 为 `{prefix}-{name}`）
2. **模块名称**：如 `user`、`order`、`payment`
3. **模块类型**：
   - **business** — 完整 CRUD 服务，带数据库、Nacos、MyBatis-Plus
   - **simple** — 轻量服务，无数据库
   - **feign** — 通过 OpenFeign 调用其他服务的模块
   - **ai** — AI 服务模块，集成 DashScope ChatModel 与 MCP 分布式能力
4. **基础包名**：如 `com.zeroone.star.{name}`
5. **端口占位变量**：如 `sp.{name}`
6. **服务名占位变量**：如 `sn.{name}`（Nacos 注册用的服务名）
7. **上下文路径**：如 `/{name}`
8. **Docker 端口号**：如 `10200`（对应应用运行端口，docker-maven-plugin 端口映射为 `{port}:{port}`）
9. **是否需要数据库？**：（是/否，仅 business 类型）
10. **是否需要 Feign 客户端？**：（是/否）

### 第 2 步：创建目录结构

在项目根目录下创建如下目录结构：

```
{prefix}-{name}/
├── src/
│   ├── main/
│   │   ├── java/com/zeroone/star/{name}/
│   │   │   ├── {Name}Application.java
│   │   │   └── config/
│   │   │       └── ComponentInit.java
│   │   └── resources/
│   │       └── application.yaml
├── Dockerfile
├── README.md
└── pom.xml
```

**business** 类型额外添加：
```
│   │   │   └── config/
│   │   │       └── MyBatisInit.java
```

> 注意：`controller`、`entity`、`mapper`、`service`、`test`、`resources/mapper` 等子包目录无需预先创建，后续编写代码时按需添加即可。

### 第 3 步：创建 pom.xml

根据模块类型选择对应的模板，详见 [reference.md](reference.md#pomxml-模板) 中的模板。

核心规则：
- 父 POM：`com.zeroone.star:{prefix}-java:${revision}`
- ArtifactId：`{prefix}-{name}`
- 始终依赖 `{prefix}-apis` 和 `{prefix}-common`
- 服务模块需引入 Nacos discovery + config
- 数据库模块需引入 MyBatis-Plus + Druid + MySQL
- Feign 模块需引入 OpenFeign + Sentinel + Loadbalancer + Caffeine + Cache
- AI 模块需引入 Nacos Config + DashScope ChatModel + MCP Distributed
- 配置 `spring-boot-maven-plugin` 并指定正确的 mainClass
- 配置 `docker-maven-plugin`（io.fabric8），设置 Docker 远程地址、CA 证书路径、镜像名称、端口映射、数据卷及环境变量（端口使用 `{port}` 占位）

### 第 4 步：创建 Application.java

```java
package com.zeroone.star.{name};

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：{模块描述}
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author {author}
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class {Name}Application {
    public static void main(String[] args) {
        SpringApplication.run({Name}Application.class, args);
    }
}
```

如果模块使用 Feign，需额外添加 `@EnableFeignClients` 注解。

### 第 5 步：创建 application.yaml

**基础配置**（所有模块通用）：

```yaml
server:
  port: ${sp.{name}}
  servlet:
    # 设置应用上下文路径
    context-path: /{name}
spring:
  application:
    name: ${sn.{name}}
```

**如果使用 Feign**，需追加 Sentinel 配置（参考 [reference.md](reference.md#feign--sentinel-配置)）：

```yaml
  cloud:
    loadbalancer:
      retry:
        enabled: true
        max-retries-on-same-service-instance: 0
        max-retries-on-next-service-instance: 1
    sentinel:
      transport:
        dashboard: ${sentinel.dashboard}
      eager: true

feign:
  sentinel:
    enabled: true
```

**如果是 AI 模块**，需追加 AI 相关配置（参考 [reference.md](reference.md#ai-模块-1)）：

```yaml
  ai:
    # 配置DashScope模型
    dashscope:
      # 模型的apikey
      # 可以在系统环境变量中添加此变量
      # 也可以配置到nacos配置中心
      api-key: ${AI_DASHSCOPE_API_KEY}
      chat:
        options:
          # 模型类型，如：qwen-max、qwen-turbo、qwen-plus
          model: glm-5
          # 其他配置自行补充
    # 配置MCP客户端
    mcp:
      client:
        enabled: true
        name: my-mcp-client  # 客户端名称，需根据业务自行修改
        version: 1.0.0
        type: SYNC
    alibaba:
      mcp:
        nacos:
          # 配置MCP Nacos客户端
          client:
            enabled: true  # 是否启用，必须配置否则找不到工具
            # sse、streamable、configs 等配置需根据实际业务需求补充和完善
```

> 数据库相关配置（datasource、mybatis-plus 等）通过 Nacos 配置中心下发，无需本地配置。Sentinel datasource 规则配置按业务需求后期自行补充。

### 第 6 步：创建 ComponentInit.java

**business 类型**：需扫描全部基础组件并注册 `EhResultWrapper` Bean。

```java
package com.zeroone.star.{name}.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 描述：初始化自定义组件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author {author}
 * @version 1.0.0
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.exception",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user"
})
public class ComponentInit {
    @Bean
    public EhResultWrapper ehResultWrapper() {
        return JsonVO::create;
    }
}
```

**非 business 类型**（simple、feign、ai）：只扫描 swagger，无需注册 `EhResultWrapper`。

```java
package com.zeroone.star.{name}.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 描述：初始化自定义组件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author {author}
 * @version 1.0.0
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger"
})
public class ComponentInit {

}
```

> 其他组件扫描（如 fastdfs、excel 等）按业务需求在编码时自行补充。

### 第 7 步：创建 MyBatisInit.java（仅 business 类型）

模板详见 [reference.md](reference.md#mybatisinitjava-模板)。

### 第 8 步：创建 FeignConfiguration.java（仅 feign 类型）

如果模块类型为 **feign**，需要创建 `src/main/java/com/zeroone/star/{name}/config/FeignConfiguration.java`，用于 OpenFeign 请求头转发（如 Authorization、X-Request-Id）。

模板详见 [reference.md](reference.md#feignconfigurationjava)。

### 第 9 步：在父 pom.xml 中注册模块

在根 `pom.xml` 的 `<modules>` 节点中添加 `<module>{prefix}-{name}</module>`。

### 第 9 步：创建 Dockerfile

```dockerfile
FROM amazoncorretto:17
MAINTAINER 01star
ADD target/{prefix}-{name}-1.0.0-SNAPSHOT.jar app.jar
ENV JAVA_OPTS="-Xms256m -Xmx256m"
ENV SPRING_ARGS=""
RUN echo "JAVA_OPTS=" $JAVA_OPTS
CMD java $JAVA_OPTS -jar app.jar --logging.file.path=/tmp/logs/spring-boot $SPRING_ARGS
```

### 第 10 步：创建 README.md

包含模块名称、描述和主启动类信息。

## 命名约定

| 项目 | 命名规则 | 示例（name=user） |
|------|---------|-------------------|
| 模块 artifactId | `{prefix}-{name}` | `project-user` |
| 包名 | `com.zeroone.star.{name}` | `com.zeroone.star.user` |
| 启动类 | `{Name}Application` | `UserApplication` |
| Controller 类 | `{Name}Controller` | `UserController` |
| Service 接口 | `I{Name}Service` | `IUserService` |
| Service 实现 | `{Name}ServiceImpl` | `UserServiceImpl` |
| Entity 类 | `{Name}` | `User` |
| Mapper 接口 | `{Name}Mapper` | `UserMapper` |
| API 接口 | `{Name}Apis` | `UserApis` |
| 上下文路径 | `/{name}` | `/user` |
| 端口变量 | `sp.{name}` | `sp.user` |
| 服务名变量 | `sn.{name}` | `sn.user` |
| Docker 端口 | `{port}` | `10200` |

{prefix} = 用户指定的模块前缀，{Name} = 大驼峰命名，{name} = 全小写。

## 扩展参考

- 完整的 pom.xml 模板和配置文件参见 [reference.md](reference.md)
