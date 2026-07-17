# 参考模板

## pom.xml 模板

### Business 模块（带数据库）

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.zeroone.star</groupId>
        <artifactId>{prefix}-java</artifactId>
        <version>${revision}</version>
        <relativePath>../pom.xml</relativePath>
    </parent>
    <artifactId>{prefix}-{name}</artifactId>
    <dependencies>
        <!--spring web mvc-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!--导入系统内置模块-->
        <dependency>
            <groupId>com.zeroone.star</groupId>
            <artifactId>{prefix}-apis</artifactId>
        </dependency>
        <dependency>
            <groupId>com.zeroone.star</groupId>
            <artifactId>{prefix}-common</artifactId>
        </dependency>

        <!--nacos相关-->
        <!--nacos discovery-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
        </dependency>
        <!--nacos config-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
        </dependency>

        <!--数据库-->
        <!--mp-->
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus-spring-boot3-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus-jsqlparser</artifactId>
        </dependency>
        <!--druid-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid-spring-boot-starter</artifactId>
        </dependency>
        <!--jdbc driver-->
        <dependency>
            <groupId>com.mysql</groupId>
            <artifactId>mysql-connector-j</artifactId>
        </dependency>

        <!-- mapstruct -->
        <dependency>
            <groupId>org.mapstruct</groupId>
            <artifactId>mapstruct</artifactId>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <mainClass>com.zeroone.star.{name}.{Name}Application</mainClass>
                </configuration>
            </plugin>
            <plugin>
                <groupId>com.zeroone.star</groupId>
                <artifactId>mp-code-generator-plugin</artifactId>
                <configuration>
                    <dbConfig>
                        <host>192.168.220.128</host>
                        <port>3306</port>
                        <dbname>{dbName}</dbname>
                        <username>{dbUser}</username>
                        <password>{dbPass}</password>
                    </dbConfig>
                    <outDir>${project.basedir}/src/main/java</outDir>
                </configuration>
            </plugin>
            <plugin>
                <groupId>io.fabric8</groupId>
                <artifactId>docker-maven-plugin</artifactId>
                <configuration>
                    <!-- Docker 远程管理地址-->
                    <dockerHost>https://192.168.220.128:2375</dockerHost>
                    <!-- CA 证书位置 -->
                    <certPath>/home/docker-ca</certPath>
                    <!-- 镜像设置 -->
                    <images>
                        <image>
                            <!-- Docker 镜像名称定义 -->
                            <name>01star/${project.artifactId}:${project.version}</name>
                            <!-- 指定Dockerfile所在目录 -->
                            <build>
                                <contextDir>${project.basedir}</contextDir>
                            </build>
                            <!-- 别名用于容器命名 -->
                            <alias>${project.artifactId}</alias>
                            <!-- 容器run相关配置 -->
                            <run>
                                <!-- 配置运行时容器命名策略为:别名,如果不指定则默认为none,即使用随机分配名称 -->
                                <namingStrategy>alias</namingStrategy>
                                <!-- 端口映射 -->
                                <ports>
                                    <port>{port}:{port}</port>
                                </ports>
                                <!-- 数据卷 -->
                                <volumes>
                                    <bind>
                                        <volume>/etc/localtime:/etc/localtime</volume>
                                        <volume>/home/app/${project.artifactId}/logs:/tmp/logs</volume>
                                    </bind>
                                </volumes>
                                <!-- 设置环境变量 -->
                                <env>
                                    <!-- JVM参数 -->
                                    <JAVA_OPTS>-Xms256m -Xmx256m</JAVA_OPTS>
                                    <!-- 启动替换参数 -->
                                    <SPRING_ARGS>
                                        --spring.profiles.active=test --spring.cloud.nacos.discovery.ip=192.168.220.128
                                    </SPRING_ARGS>
                                </env>
                            </run>
                        </image>
                    </images>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

### Simple 模块（无数据库）

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.zeroone.star</groupId>
        <artifactId>{prefix}-java</artifactId>
        <version>${revision}</version>
        <relativePath>../pom.xml</relativePath>
    </parent>
    <artifactId>{prefix}-{name}</artifactId>
    <dependencies>
        <!--spring web mvc-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!--导入系统内置模块-->
        <dependency>
            <groupId>com.zeroone.star</groupId>
            <artifactId>{prefix}-apis</artifactId>
        </dependency>
        <dependency>
            <groupId>com.zeroone.star</groupId>
            <artifactId>{prefix}-common</artifactId>
        </dependency>

        <!--nacos discovery-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
        </dependency>
        <!--nacos config-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <mainClass>com.zeroone.star.{name}.{Name}Application</mainClass>
                </configuration>
            </plugin>
            <plugin>
                <groupId>io.fabric8</groupId>
                <artifactId>docker-maven-plugin</artifactId>
                <configuration>
                    <!-- Docker 远程管理地址-->
                    <dockerHost>https://192.168.220.128:2375</dockerHost>
                    <!-- CA 证书位置 -->
                    <certPath>/home/docker-ca</certPath>
                    <!-- 镜像设置 -->
                    <images>
                        <image>
                            <!-- Docker 镜像名称定义 -->
                            <name>01star/${project.artifactId}:${project.version}</name>
                            <!-- 指定Dockerfile所在目录 -->
                            <build>
                                <contextDir>${project.basedir}</contextDir>
                            </build>
                            <!-- 别名用于容器命名 -->
                            <alias>${project.artifactId}</alias>
                            <!-- 容器run相关配置 -->
                            <run>
                                <!-- 配置运行时容器命名策略为:别名,如果不指定则默认为none,即使用随机分配名称 -->
                                <namingStrategy>alias</namingStrategy>
                                <!-- 端口映射 -->
                                <ports>
                                    <port>{port}:{port}</port>
                                </ports>
                                <!-- 数据卷 -->
                                <volumes>
                                    <bind>
                                        <volume>/etc/localtime:/etc/localtime</volume>
                                        <volume>/home/app/${project.artifactId}/logs:/tmp/logs</volume>
                                    </bind>
                                </volumes>
                                <!-- 设置环境变量 -->
                                <env>
                                    <!-- JVM参数 -->
                                    <JAVA_OPTS>-Xms256m -Xmx256m</JAVA_OPTS>
                                    <!-- 启动替换参数 -->
                                    <SPRING_ARGS>
                                        --spring.profiles.active=test --spring.cloud.nacos.discovery.ip=192.168.220.128
                                    </SPRING_ARGS>
                                </env>
                            </run>
                        </image>
                    </images>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

### Feign 模块（调用其他服务）

依赖配置在 **Simple 模块** 基础上额外添加：

```xml
        <!--openfeign-->
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-openfeign</artifactId>
        </dependency>
        <!--sentinel-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
        </dependency>
        <!--load balancer-->
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-starter-loadbalancer</artifactId>
        </dependency>
        <!--load balancer cache manager need-->
        <dependency>
            <groupId>com.github.ben-manes.caffeine</groupId>
            <artifactId>caffeine</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-cache</artifactId>
        </dependency>
```

同时在 Application 类上添加 `@EnableFeignClients` 注解。

### AI 模块（集成 AI 能力）

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>com.zeroone.star</groupId>
        <artifactId>{prefix}-java</artifactId>
        <version>${revision}</version>
        <relativePath>../pom.xml</relativePath>
    </parent>
    <artifactId>{prefix}-{name}</artifactId>
    <dependencies>
        <!--spring web mvc-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!--导入系统内置模块-->
        <dependency>
            <groupId>com.zeroone.star</groupId>
            <artifactId>{prefix}-common</artifactId>
        </dependency>

        <!--nacos config-->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
        </dependency>

        <!-- DashScope ChatModel -->
        <dependency>
            <groupId>com.alibaba.cloud.ai</groupId>
            <artifactId>spring-ai-alibaba-starter-dashscope</artifactId>
        </dependency>
        <!-- MCP Distributed -->
        <dependency>
            <groupId>com.alibaba.cloud.ai</groupId>
            <artifactId>spring-ai-alibaba-starter-mcp-distributed</artifactId>
        </dependency>
        <!-- MCP Client -->
        <dependency>
            <groupId>org.springframework.ai</groupId>
            <artifactId>spring-ai-starter-mcp-client</artifactId>
        </dependency>
        <!-- JSON Schema Validator -->
        <dependency>
            <groupId>com.networknt</groupId>
            <artifactId>json-schema-validator</artifactId>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <mainClass>com.zeroone.star.{name}.{Name}Application</mainClass>
                </configuration>
            </plugin>
            <plugin>
                <groupId>io.fabric8</groupId>
                <artifactId>docker-maven-plugin</artifactId>
                <configuration>
                    <!-- Docker 远程管理地址-->
                    <dockerHost>https://192.168.220.128:2375</dockerHost>
                    <!-- CA 证书位置 -->
                    <certPath>/home/docker-ca</certPath>
                    <!-- 镜像设置 -->
                    <images>
                        <image>
                            <!-- Docker 镜像名称定义 -->
                            <name>01star/${project.artifactId}:${project.version}</name>
                            <!-- 指定Dockerfile所在目录 -->
                            <build>
                                <contextDir>${project.basedir}</contextDir>
                            </build>
                            <!-- 别名用于容器命名 -->
                            <alias>${project.artifactId}</alias>
                            <!-- 容器run相关配置 -->
                            <run>
                                <!-- 配置运行时容器命名策略为:别名,如果不指定则默认为none,即使用随机分配名称 -->
                                <namingStrategy>alias</namingStrategy>
                                <!-- 端口映射 -->
                                <ports>
                                    <port>{port}:{port}</port>
                                </ports>
                                <!-- 数据卷 -->
                                <volumes>
                                    <bind>
                                        <volume>/etc/localtime:/etc/localtime</volume>
                                        <volume>/home/app/${project.artifactId}/logs:/tmp/logs</volume>
                                    </bind>
                                </volumes>
                                <!-- 设置环境变量 -->
                                <env>
                                    <!-- JVM参数 -->
                                    <JAVA_OPTS>-Xms256m -Xmx256m</JAVA_OPTS>
                                    <!-- 启动替换参数 -->
                                    <SPRING_ARGS>
                                        --spring.profiles.active=test --spring.cloud.nacos.discovery.ip=192.168.220.128
                                    </SPRING_ARGS>
                                </env>
                            </run>
                        </image>
                    </images>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

### 公共模块（apis/domain/common 类型）

仅用于 `{prefix}-apis`、`{prefix}-domain`、`{prefix}-common` 这类被其他模块依赖的共享模块。新建此类模块的情况较少，如有需求请确认后再操作。

## application.yaml 模板

### Business 模块

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

> 数据库相关配置（datasource、mybatis-plus 等）通过 Nacos 配置中心统一下发，无需在本地 yaml 中配置。

### Simple 模块

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

### AI 模块

```yaml
server:
  port: ${sp.{name}}
  servlet:
    # 设置应用上下文路径
    context-path: /{name}
spring:
  application:
    name: ${sn.{name}}
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

### Feign + Sentinel 配置

当模块使用了 Feign 客户端时，需在上述基础配置上追加 Sentinel 配置：

```yaml
  cloud:
    loadbalancer:
      retry:
        enabled: true
        # 对同一个服务实例的最大重试次数（不包括首次调用）
        max-retries-on-same-service-instance: 0
        # 切换新实例的最大重试次数（不包括首次调用的实例）
        max-retries-on-next-service-instance: 1
    sentinel:
      transport:
        # 控制台地址
        dashboard: ${sentinel.dashboard}
      eager: true

# 开启sentinel声明式服务熔断
feign:
  sentinel:
    enabled: true
```

> Sentinel datasource（流控、降级规则）按业务需求后期自行补充，无需在模块创建时预置。

## MyBatisInit.java 模板

```java
package com.zeroone.star.{name}.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.zeroone.star.project.components.user.UserHolder;
import jakarta.annotation.Resource;
import org.apache.ibatis.reflection.MetaObject;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：MP初始化
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author {author}
 * @version 1.0.0
 */
@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.{name}.mapper")
public class MyBatisInit {
    @Resource
    UserHolder userHolder;

    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                this.strictInsertFill(metaObject, "createTime", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictInsertFill(metaObject, "createBy", String.class, userHolder.getCurrentUser().getUsername());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updateTime", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictUpdateFill(metaObject, "updateBy", String.class, userHolder.getCurrentUser().getUsername());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }
        };
    }
}
```

## FeignConfiguration.java 模板

```java
package com.zeroone.star.{name}.config;

import feign.RequestInterceptor;
import feign.RequestTemplate;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.context.annotation.Configuration;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

/**
 * <p>
 * 描述：声明式服务配置，主要解决Header传参问题
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author {author}
 * @version 1.0.0
 */
@Configuration
public class FeignConfiguration implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate requestTemplate) {
        if (RequestContextHolder.getRequestAttributes() instanceof ServletRequestAttributes attributes) {
            HttpServletRequest request = attributes.getRequest();
            // 1. 转发 Authorization 头（用于身份认证）
            String authorization = request.getHeader("Authorization");
            if (StringUtils.hasText(authorization)) {
                requestTemplate.header("Authorization", authorization);
            }

            // 2. 转发自定义的 X-Request-Id 头（用于全链路追踪）
            String requestId = request.getHeader("X-Request-Id");
            if (StringUtils.hasText(requestId)) {
                requestTemplate.header("X-Request-Id", requestId);
            }

            // 3. 根据需要补充其他头
        }
    }
}
```

## Dockerfile 模板

```dockerfile
FROM amazoncorretto:17
MAINTAINER 01star
ADD target/{prefix}-{name}-1.0.0-SNAPSHOT.jar app.jar
ENV JAVA_OPTS="-Xms256m -Xmx256m"
ENV SPRING_ARGS=""
RUN echo "JAVA_OPTS=" $JAVA_OPTS
CMD java $JAVA_OPTS -jar app.jar --logging.file.path=/tmp/logs/spring-boot $SPRING_ARGS
```
