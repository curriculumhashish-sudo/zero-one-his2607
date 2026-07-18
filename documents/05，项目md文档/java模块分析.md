# his-java 模块架构说明

## 1. 模块总览

`his-java` 项目基于 Spring Boot & Spring Cloud 微服务架构，采用 Maven 多模块管理，共包含 13 个子模块。分为公共层、网关鉴权层和业务层。

## 2. 模块分类与职责

### 2.1 公共模块
提供全局通用的基础能力、数据模型和接口定义，被其他业务模块依赖。

- **his-common**: 基础公共模块。提供全局配置（MyBatis、Redis、Swagger等）、通用组件（Excel、JWT、邮件、SMS等）、全局异常处理及工具类。
- **his-domain**: 领域模型模块。存放跨模块共用的 DTO（数据传输对象）、VO（视图对象）和 Query（查询对象）。
- **his-apis**: API 接口定义模块。定义各微服务对外暴露的 Feign/API 接口，依赖 `his-domain`。

### 2.2 网关与鉴权模块
负责系统的请求路由、身份认证与权限校验。

- **his-gateway**: 主网关服务。负责请求路由转发、Token校验及资源权限加载。
- **his-mcp-gateway**: MCP网关服务。处理特定的MCP请求头过滤与路由转发。
- **his-oauth2**: OAuth2 认证中心。处理用户登录认证、JWT令牌颁发、权限缓存及资源加载。
- **his-login**: 登录聚合服务。处理登录相关的业务逻辑及菜单树获取，依赖 OAuth2 服务。

### 2.3 J4 诊疗业务模块
实现具体的医疗业务功能，模块间相互独立，均依赖 `his-apis`、`his-common` 和 `his-domain`。

- **his-j4-treatment**: 诊疗管理服务。提供诊疗项目的增删改查及启停用功能。
- **his-j4-diagnosis**: 诊断管理服务。
- **his-j4-medicine**: 药品管理服务。
- **his-j4-equipment**: 设备管理服务。
- **his-j4-frequency**: 频次管理服务。
- **his-j4-sample**: 标本管理服务。

## 3. 模块依赖关系

业务模块的依赖遵循以下标准层级：
`业务模块` -> `his-apis` -> `his-domain` -> `his-common`

```text
his-j4-treatment (业务实现)
 ├── his-apis (接口定义)
 │    └── his-domain (DTO/VO)
 ├── his-common (公共组件与配置)
 └── 数据库驱动/MyBatis Plus/Druid
