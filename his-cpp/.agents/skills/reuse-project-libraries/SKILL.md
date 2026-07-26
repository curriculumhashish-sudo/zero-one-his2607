---
name: reuse-project-libraries
description: 在 zero-one-his2607/his-cpp 中开发或修改 API、Controller、DTO/VO/Query、Service、DAO、数据库、鉴权、配置、中间件、文件、Excel、PDF、消息、缓存、对象存储等功能时，先审计并复用 lib-oatpp、lib-mysql、lib-common、arch-demo 和 arch-tests 的既有封装、示例与测试，再决定是否新增代码。
---

# 复用 his-cpp 项目库

## 目标

在写代码前找到项目已有能力，优先复用稳定的二次封装，减少重复代码，同时避免把第三方源码、演示代码或运行时 DLL 误当成成熟公共 API。

## 必读资料

开始任务后：

1. 完整读取项目根目录 AGENTS.md。
2. 读取 references/capability-catalog.md 中与任务有关的章节。
3. 任务只要涉及 DO、Mapper、DAO、Service、CRUD 或数据库表，就读取 ../crud-code-generator/SKILL.md。
4. 任务只要涉及 HTTP API、Controller、DTO/Query/VO、Router 或 Swagger，就读取 ../api-interface-generator/SKILL.md。
5. 跨 API、数据库、缓存等多类别任务要读取所有相关技能，不能因“不属于完整 CRUD”而跳过。

## 强制工作流

### 1. 先分类

先确定目标业务模块；未指定时搜索最接近的模块并给出候选。多个候选会改变 CMake、配置或源码落点时，先请求确认，不直接修改。

再把需求归入一个或多个类别：

- HTTP API、Swagger、鉴权、文件上传、WebSocket、HTTP Client。
- MySQL 查询、增删改、批量操作、事务。
- YAML、Redis、MongoDB、RocketMQ、FastDFS、Nacos、邮件、短信。
- Excel、PDF、二维码、字符集、字符串、时间、ID、树结构。
- 新模块、CMake、运行资源或动态库。

### 2. 先搜索，后设计

至少搜索以下位置：

    rg -n "能力名|类名|宏名" lib-oatpp/include lib-mysql/include lib-common/include
    rg -n "能力名|类名|宏名" arch-demo arch-tests
    rg -n "能力名|类名|宏名" <目标模块> <目标模块>/Macros.h
    rg -n -g "CMakeLists.txt" "USE_[A-Z_]+|include_directories|link_directories|target_link_libraries|target_include_directories" .

优先查项目入口头文件和业务示例，不要先钻入第三方头文件目录。

### 3. 形成复用证据

实现前在工作说明中列出：

- 要复用的头文件。
- 类、函数、宏或 CMake target。
- 最接近的 arch-demo 或 arch-tests 示例。
- 所需配置、资源、动态库和平台限制。
- 仍需新增的最小代码及其原因。

如果没有找到可复用能力，要写明搜索词和检查过的目录。

### 4. 按固定优先级选型

使用以下顺序：

1. 项目二次封装。
2. arch-demo 中可改造的真实业务范例。
3. arch-tests 中仍需在隔离环境验证的集成示例。
4. 已随附的第三方 API。
5. 新建项目封装。
6. 新增外部依赖。

上一级能满足需求时，不进入下一级。

### 5. 实现时守住边界

- Controller 只做协议适配、校验、调用 Service 和包装响应。
- Service 负责业务规则、对象转换和事务边界。
- DAO 负责 SQL、参数绑定与 DO 映射。
- 认证基础设施负责签名、时效、issuer、audience、环境公钥、撤销状态和启动 fail-fast；Service 或授权策略负责角色、租户和资源归属。
- 禁止手拼 JSON、用户输入 SQL、JWT 解析、multipart boundary 或通用分页代码。
- 禁止把 arch-tests 示例直接复制到生产业务。
- 禁止修改 lib-oatpp/include/oatpp、oatpp-swagger、oatpp-websocket、jwt-cpp、openssl、picojson，以及 lib64、lib-dy 中的二进制。
- 配置、密码、服务地址和文件路径必须外置，不复制示例硬编码值。

### 6. 验证依赖与行为

提交前检查：

- 对应 CMake 选项是否打开，target 是否链接。
- Windows/Linux 是否都支持；仅一端支持时明确说明。
- 配置、证书、Swagger、模板、字体、DLL/SO 是否进入实际运行目录；public.pem 加载与内容校验由 main、应用组件或 CustomerAuthorizeHandler 在启动阶段 fail-fast。
- API 描述鉴权与运行时鉴权是否成对。
- 认证基础设施是否校验签名、时效、issuer、audience、密钥和撤销状态；Service/授权策略是否校验角色、租户和资源归属。
- 分页是否限制 pageIndex/pageSize 的下限与最大 pageSize，并使用稳定排序。
- 数据库参数是否绑定，批量输入是否非空，事务是否显式提交或回滚。
- 缓存键是否隔离用户/租户/权限域与查询条件，是否定义 TTL、版本、失效、故障降级和击穿保护。
- 客户端/连接池是否为长生命周期；禁止在每次请求内重新创建 Redis、MongoDB 或 RocketMQ 客户端。
- 对应 USE_* 关闭时，是明确停止构建还是提供经过验证的降级路径。
- 至少编译受影响目标；需要外部服务的测试只能使用隔离测试环境。

## 快速决策

### API

- 普通 Controller：ApiHelper.h、API_ACCESS_DECLARE、API_HANDLER_*、API_DEF_*。
- JSON：DTO/VO、JsonVO、ObjectMapper。
- 鉴权：带 AUTH 的 Endpoint 宏、CustomerAuthorizeHandler、PayloadDTO。
- 路由与 Swagger：跨文件复用公共 BIND_CONTROLLER；ROUTER_SIMPLE_BIND 只能参考并使用目标模块 Router.cpp 中的局部定义。
- 上传、WebSocket、声明式客户端：先查 FileController、WSController、SampleApiClient 示例。

### MySQL

- 普通新增、修改、删除：BaseDAO。
- 批量增改：BaseDAO 批量方法。
- 自定义查询：SqlParams + SqlSession + Mapper。
- 多 DAO 原子操作：TransactionManager，并显式提交或回滚。
- 特殊 DDL/SQL：最后才直接使用 SqlSession。

### 缓存与中间件

- 缓存任务由 reuse-project-libraries 统一编排：应用组件创建长生命周期客户端，专用缓存适配器负责键、序列化、TTL、版本、失效、降级、击穿和指标，Service 负责调用；API/CRUD 生成技能不会自动交付这部分。
- 需求包含缓存时，不能只完成 Controller、Service 和数据库查询；必须同时交付组件注册、缓存适配器、配置、写后失效和隔离测试。
- RedisClient::execute 已吞掉异常并返回默认值，上层无法恢复未命中、合法空值和故障的区别。若需求需要可靠故障语义，必须扩展 lib-common 为状态承载型结果 API 并补测试，不能声称只靠业务适配器解决。
- 敏感缓存键必须包含版本、租户/用户或权限域和全部规范化查询条件；禁止用阻塞式 KEYS 做批量失效。

### 公共组件

- 先从 lib-common/include 根目录的项目门面头文件选择。
- 再去 arch-demo 确认业务用法，或去 arch-tests 确认隔离集成示例。
- lib-dy 只解决运行时动态库装载，不能被 include，也不代表功能已经启用。
- RedisClient 只提供访问门面，不提供缓存键、TTL、失效、隔离或击穿策略；敏感数据缓存必须把身份域放入键，并让 MySQL 保持事实源。
- RedisClient::execute 的默认值无法区分未命中和异常；需要可靠错误语义时按上一节扩展项目门面。

## 交付格式

最终回复至少包含：

1. 复用了哪些项目能力。
2. 新增了哪些最小代码。
3. 验证了什么。
4. 哪些外部服务、平台或安全边界仍需用户确认。
