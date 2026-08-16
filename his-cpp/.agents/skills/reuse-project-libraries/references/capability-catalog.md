# his-cpp 项目库能力目录

## 目录

1. 证据等级与选型原则
2. API 运行架构
3. lib-oatpp
4. lib-mysql
5. lib-common
6. lib-dy 与第三方目录
7. CMake、平台与运行资源
8. 任务到能力的选择表
9. 已知边界和风险

## 1. 证据等级与选型原则

目录存在不等于业务已经启用。记录能力时使用下列等级：

| 等级 | 含义 | 可以怎样表述 |
| --- | --- | --- |
| A | 有项目封装，且 arch-demo 有真实调用 | 项目已接入，可优先复用 |
| B | 有项目封装，主要在 arch-tests 有示例 | 项目已随附封装，接入前需在隔离环境验证 |
| C | 只有第三方头文件或预编译库 | 依赖已随附，不代表项目已完成封装或业务验证 |
| D | 只有 DLL/SO | 仅为运行时依赖，不能作为 C++ API 调用 |

固定优先级：

    项目二次封装
      → arch-demo 可改造范例
      → arch-tests 隔离集成示例
      → 随附第三方 API
      → 新建封装
      → 新增依赖

## 2. API 运行架构

### 2.1 服务启动

    main.cpp
      → 读取启动参数、YAML 或 Nacos 配置
      → 初始化 DbInit
      → HttpServer::startServer
        → oatpp Environment
        → AppComponent
          → HttpRouter
          → ObjectMapper
          → ErrorHandler
          → CORS/Token 拦截器
        → OtherComponent
          → WebSocket handler
          → HTTP Client RequestExecutor
        → Router::initRouter
        → Swagger Controller
        → server.run

主要证据：

- arch-demo/main.cpp
- lib-oatpp/src/HttpServer.cpp
- lib-oatpp/include/AppComponent.hpp
- arch-demo/controller/Router.cpp

### 2.2 一次请求

    客户端
      → HttpConnectionHandler
      → 请求拦截器
      → HttpRouter
      → Endpoint 参数解析
      → 可选的 RSA JWT 鉴权
      → Controller
      → Service
      → DAO / 外部组件
      → JsonVO
      → ObjectMapper
      → 响应拦截器
      → 客户端

API_DEF_ADD_AUTH 只描述 Swagger 安全要求；带 AUTH 的 Endpoint 宏才触发运行时授权处理器，两者必须成对。

## 3. lib-oatpp

lib-oatpp 是项目在 oat++ 上的二次封装。优先入口是 lib-oatpp/include 根目录与 domain 子目录；oatpp、jwt-cpp、openssl 等子目录是第三方代码。

| 任务 | 项目能力 | 参考位置 | 证据 |
| --- | --- | --- | --- |
| 启动 HTTP 服务 | HttpServer::startServer | arch-demo/main.cpp | A |
| Router 与 Swagger 同步注册 | 公共 BIND_CONTROLLER；模块 Router.cpp 中的局部 ROUTER_SIMPLE_BIND | arch-demo/controller/Router.cpp | A |
| Controller 构造和 ObjectMapper 注入 | API_ACCESS_DECLARE | arch-demo/controller/sample/SampleController.h | A |
| HTTP 方法 | API_M_GET、API_M_POST、API_M_PUT、API_M_DEL | lib-oatpp/include/ApiHelper.h | A |
| 普通与鉴权 Endpoint | API_HANDLER_ENDPOINT、API_HANDLER_ENDPOINT_AUTH | SampleController.h | A |
| 无参接口 | API_HANDLER_ENDPOINT_NOPARAM 及 AUTH 版本 | UserController.h | A |
| Query 自动转换 | API_HANDLER_ENDPOINT_QUERY、API_HANDLER_QUERY_PARAM | SampleController.h | A |
| Swagger 描述 | API_DEF_ENDPOINT_INFO 及 AUTH/QUERY 版本 | SampleController.h | A |
| DTO 字段与文档 | API_DTO_FIELD 系列 | domain DTO 与 SampleDTO.h | A |
| 分页请求 | PageQuery | lib-oatpp/include/domain/query | A |
| 分页响应 | PageDTO、calcPages | lib-oatpp/include/domain/dto | A |
| 统一响应 | JsonVO、NoDataJsonVO、StringJsonVO、ListJsonVO | lib-oatpp/include/domain/vo | A |
| 业务状态 | RS_SUCCESS、RS_FAIL、RS_PARAMS_INVALID 等 | Message.h、ResultStatus.h | A |
| JSON 映射 | ObjectMapper、createDtoResponse | AppComponent.hpp、Controller 示例 | A |
| RSA JWT 验签接入 | CustomerAuthorizeHandler、JWTUtil、PayloadDTO | lib-oatpp/src 与 include | A；不等于完整发证与授权体系 |
| 读取已验签的 claims | authObject->getPayload | SampleController.h | A；不是权威用户查询 |
| CORS 与 Token 是否存在的前置检查 | SystemInterceptor 接口和模块实现 | arch-demo/controller/SystemInterceptor.cpp | A；不负责 JWT 验签 |
| 全局异常响应 | ErrorHandler | lib-oatpp/src/ErrorHandler.cpp | A |
| Multipart | API_MULTIPART 系列 | arch-demo/controller/file/FileController.cpp | A |
| 流式请求体 | BODY_STRING | FileController.h | A |
| URL 编解码 | URIUtil | FileController.cpp | A |
| 本机 IP | IpUtil | HttpServer.cpp | A |
| WebSocket | OtherComponent、WSController、WSListener | arch-demo/controller/ws | A，教学范例 |
| 声明式 HTTP Client | API_CLIENT_CREATE、ApiClient::API_CALL | SampleApiClient、SampleController.cpp | A，地址需配置化 |

ApiHelper.h 已集中导入 Controller、ObjectMapper、multipart、Swagger、JsonVO、PageDTO、授权和 URI 工具。业务 Controller 应显式包含它，不依赖 GlobalInclude.h 的偶然传递包含。

## 4. lib-mysql

lib-mysql 是 MySQL Connector/C++ JDBC 的轻量封装，不是完整 ORM。推荐入口：

- DbInit.h
- SqlSession.h
- Mapper.h
- BaseDO.h / SqlHelper.h
- BaseDAO.h
- TransactionManager.h

| 任务 | 项目能力 | 参考位置 | 证据 |
| --- | --- | --- | --- |
| 连接参数和全局池 | DBConfig、DbInit | arch-demo/main.cpp | A |
| 特殊无结果 SQL | SqlSession::execute | arch-tests/tests/mysql | B |
| 参数化增改删 | executeUpdate | SqlSession | A |
| 自增主键 | executeInsert | SqlSession、BaseDAO | A |
| 单值查询 | executeQueryNumerical | SampleDAO.cpp | A |
| 单行查询 | executeQueryOne | SampleDAO.cpp | A |
| 多行查询 | executeQuery | SampleDAO.cpp | A |
| 参数绑定 | SqlParams、SQLPARAMS_PUSH | SampleDAO.cpp | A |
| ResultSet 到 DO | Mapper | SampleMapper.h | A |
| 表与字段元数据 | BaseDO、MYSQL_* 宏 | SampleDO.h | A |
| 普通插入 | BaseDAO::insert | SampleService.cpp | A |
| 自增插入 | BaseDAO::insertAutoPk | lib-mysql/src | B |
| 更新 | BaseDAO::update | SampleService.cpp | A |
| 按主键删除 | deleteById、deleteByIds | SampleService.cpp | A |
| 批量增改 | insertBatch、insertAutoPkBatch、updateBatch | arch-tests/tests/mysql | B |
| 单会话事务 | begin、commit、rollbackTransaction | arch-tests/tests/mysql | B |
| 多 DAO 事务 | TransactionManager | arch-tests/tests/mysql | B |

数据库配置链：

    启动参数 / YAML / Nacos
      → YamlHelper
      → DBConfig
      → DbInit::initDbPool
      → BaseDAO / SqlSession

数据库决策：

- 普通新增、修改、删除：BaseDAO。
- 批量增改：BaseDAO 批量方法。
- 自定义查询：SqlParams + SqlSession + Mapper。
- 跨 DAO 原子操作：TransactionManager。
- DDL 或封装未覆盖的特殊 SQL：最后才直接使用 SqlSession。

## 5. lib-common

lib-common 的 include 根目录是项目门面层；其内部还随附大量第三方头文件。使用前要同时确认封装、示例、CMake 选项、平台和运行时库。

| 类别 | 项目入口 | 能力 | 主要证据 |
| --- | --- | --- | --- |
| YAML | YamlHelper.h | 解析 YAML 文件或文本 | arch-demo/main.cpp |
| 字符集 | CharsetConvertHepler.h | UTF-8、ANSI、GBK、Unicode 转换 | lib-common 源码 |
| 字符串 | StringUtil.h | split、trim、replace 等 | arch-demo / 源码 |
| 日期时间 | SimpleDateTimeFormat.h | 时间解析和格式化 | arch-demo |
| UUID | id/UuidFacade.h | UUID 生成 | arch-demo |
| 雪花 ID | id/SnowFlake.h | 分布式整数 ID | arch-tests/id |
| 树结构 | tree 目录 | 扁平数据与树形结构转换 | arch-demo |
| FastDFS | FastDfsClient.h | 上传、下载、删除文件 | arch-demo FileController |
| Excel | ExcelComponent.h | 文件/内存表格读写 | arch-demo SampleController |
| PDF | PdfComponent.h | 文本、图片、模板、内存/文件输出、条码/二维码 | arch-demo SampleController |
| Redis | RedisClient.h | 基于 redis++ 的连接池和回调式访问 | B：arch-tests/redis；只提供访问门面，不含缓存策略 |
| MongoDB | MongoClient.h | 连接池、执行和插入 | arch-tests/mongo |
| RocketMQ | RocketClient.h | 同步/异步发布、订阅和监听 | arch-tests/rocketmq |
| Nacos | NacosClient.h | 读取配置、注册和注销服务 | arch-demo/main.cpp；Windows 有退化 |
| 邮件 | EmailSender.h | SMTP/SSL、收件人、抄送、附件和发送 | arch-tests/mail；Windows 受限 |
| 阿里短信/PNS | sms/aliyun | 短信与号码认证接口 | arch-tests/sms；Linux |

可直接使用但属于第三方的常见能力：

- bcrypt：密码哈希生成和校验。
- zxing-cpp：条码和二维码。
- xlnt：Excel。
- libharu：PDF。
- yaml-cpp：YAML。
- redis++ / hiredis：Redis。
- mongocxx / bsoncxx：MongoDB。

只在项目门面不覆盖需求时才直接调用第三方 API，并将重复样板沉淀到 lib-common，而不是散落到业务 Controller。

当前没有统一日志门面或通用可逆加密工具类。bcrypt 是密码哈希，JWT 是签名凭证；通用 HTTP Client 位于 lib-oatpp 的声明式 ApiClient，不在 lib-common。

模块 Macros.h 已提供部分客户端创建、Rocket DTO/JSON、DO/DTO 转换宏。创建客户端的宏会声明固定局部变量名；同一作用域多次使用时应放进独立花括号作用域，并先核对展开结果。

## 6. lib-dy 与第三方目录

lib-dy 只保存 Windows DLL 或 Linux SO，用于程序启动时的动态装载。它不能被 include，也不提供编译期 API。

常见运行库覆盖 MySQL、OpenSSL、MongoDB/BSON、RocketMQ、FastDFS、yaml-cpp、xlnt、PDF/png/zlib、测试库和阿里云 SDK。Redis 要分平台理解：Windows 的 DLL 位于 lib-dy 的 debug/release 目录，Linux 当前使用 lib-common/lib64 中的静态库，不要笼统写成“lib-dy 提供所有 Redis 运行库”。现有依赖主要按 x64 组织。

下列目录通常不要由业务任务修改：

- lib-oatpp/include/oatpp
- lib-oatpp/include/oatpp-swagger
- lib-oatpp/include/oatpp-websocket
- lib-oatpp/include/jwt-cpp
- lib-oatpp/include/openssl
- lib-oatpp/include/picojson
- lib-oatpp/lib64
- lib-common 的第三方 include 子树与 lib64
- lib-mysql/include/jdbc、mysqlx
- lib-dy

## 7. CMake、平台与运行资源

根 CMake 的常见可选项：

- USE_XLNT
- USE_DFS
- USE_YML
- USE_REDIS
- USE_ROCKETMQ
- USE_MONGO
- USE_PDF
- USE_NACOS
- USE_ALISMS

USE_NACOS 与 USE_ALISMS 在当前项目具有平台限制；不要只因头文件存在就假定 Windows 可用。

Redis 还要核对组合依赖：配置解析通常依赖 USE_YML，Linux 的现有客户端创建宏还可能依赖 USE_NACOS。USE_REDIS 关闭时，业务必须明确选择“功能不可构建”或“经过验证的数据库直查降级”，不能留下半编译路径。

lib-common 没有把 redis++/hiredis 作为可传递 CMake 依赖，新可执行 target 仍需像现有模块一样显式链接。RedisClient 头文件未受 USE_REDIS 保护而构造器实现受保护；关闭开关后继续引用该类会产生链接问题。

接入新能力时同时检查：

1. target_include_directories 或项目现有的 include_directories 是否包含公共头目录。
2. target_link_libraries 以及项目现有的 link_directories 是否覆盖项目 target 和第三方库。
3. 根 CMake 是否 add_subdirectory 对应库。
4. DLL/SO 是否进入实际可执行文件目录或加载路径。
5. conf、public.pem、swagger、fonts、tpl 等资源是否复制到运行目录。
6. file(GLOB) 和 file(COPY) 属于配置阶段行为；新增文件或修改复制源后，Visual Studio 可能需要重新配置 CMake 缓存。

## 8. 任务到能力的选择表

| 需求 | 首选 | 不要先做 |
| --- | --- | --- |
| 新增 REST API | ApiHelper + DTO/Query/VO + Router | 手写 oat++ 构造样板 |
| 统一响应 | JsonVO / BaseJsonVO | 手拼 JSON 字符串 |
| 登录接口保护 | AUTH Endpoint + CustomerAuthorizeHandler | Controller 内手动解析 JWT |
| 分页 | PageQuery + PageDTO | 每个模块重写 pageIndex/pageSize |
| 普通 CRUD | BaseDAO | 重复写 INSERT/UPDATE/DELETE |
| 条件查询 | SqlParams + SqlSession + Mapper | 拼接用户输入 SQL |
| 跨 DAO 事务 | TransactionManager | 每个 DAO 各拿一条连接 |
| 文件上传 | FileController 与 multipart 封装 | 自己解析 boundary |
| 内部 HTTP 调用 | 声明式 ApiClient | 手工拼 URL 和 JSON |
| 配置读取 | YamlHelper / NacosClient | 把地址密码写进代码 |
| ID | UuidFacade / SnowFlake | 自建随机 ID 方案 |
| Excel/PDF/二维码 | ExcelComponent / PdfComponent | Controller 直接操作底层库 |
| 缓存/消息/文档库 | RedisClient / RocketClient / MongoClient | 未验证就复制测试代码 |

## 9. 已知边界和风险

### API

- Query 宏主要处理基础类型；列表和嵌套对象应使用 DTO body 或定制解析。
- PageQuery 当前没有强制 pageIndex/pageSize 为正数，也没有最大 pageSize；使用前必须校验，否则可能出现除零、无符号下溢或超大查询。
- ObjectMapper 当前拒绝未知字段并忽略空字段，这是实际 JSON 契约。
- multipart 示例的无限内存模式不能用于生产大文件。
- FileController 路径拼接缺少完整规范化校验。
- 声明式 HTTP Client 示例地址硬编码，必须配置化。
- Swagger 鉴权声明不等于运行时鉴权。
- 当前 RSA 验签没有自动完成 issuer、audience、撤销和密钥轮换校验；这些属于认证基础设施。角色、租户和资源归属属于 Service 或授权策略，二者不能混为一层。
- public.pem 从进程工作目录的固定相对路径读取，失败不会让服务启动立即失败；新模块要由 main、应用组件或 CustomerAuthorizeHandler 使用环境专属公钥、校验加载结果、设计轮换并在启动时 fail-fast。
- WebSocket 是教学范例，不是经过并发与生命周期审计的成熟公共组件。

### MySQL

- 现有连接池初始化日志可能输出数据库密码，生产前必须修复。
- BaseDAO 自定义 where 是直接拼接，禁止放用户输入。
- 多数 SQL 异常被转换为空值或 0，业务层必须识别歧义。
- TransactionManager 不是 RAII，必须显式提交或回滚。
- 批量方法和 deleteByIds 在调用前必须检查非空和合法 batchSize。
- 连接池耗尽时不会等待；初始化顺序也必须保证。

### 公共组件

- arch-tests 需要外部服务，只能在隔离环境运行。
- Redis、MongoDB、RocketMQ、邮件、短信等“有示例”不等于当前业务已经验证。
- Redis 示例只是独立可执行程序，未注册为 CTest，也没有 TTL、清理和唯一隔离键；直接运行可能污染共享 Redis。
- RedisClient::execute 捕获异常后返回默认值，错误信息已经丢失，上层不能恢复缓存未命中、合法空值和 Redis 故障的区别；需要可靠语义时必须扩展 lib-common 为状态承载型结果 API 并补测试。
- 当前连接池等待被固定为约 12 ms，封装没有开放连接/Socket 超时、ACL 用户名、TLS、哨兵、集群、健康检查和指标配置；不能跨不可信网络传输敏感明文。
- ZO_CREATE_REDIS_CLIENT 只读取 host/port/password，端口使用 atoi 且缺少校验，宏也不是自包含接口；不要在请求内展开创建连接池。
- 项目没有内置缓存键、TTL、序列化版本、失效、身份域隔离、击穿保护或可观测性策略；敏感分页缓存必须自行补齐，并长期复用客户端/连接池。
- Windows 下 Nacos 存在本地 YAML 退化行为，邮件和阿里短信也有平台限制。
- 示例里的地址、账号、密钥和路径不能复制到新业务。
