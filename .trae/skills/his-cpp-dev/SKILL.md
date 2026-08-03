---
name: "his-cpp-dev"
description: "HIS C++项目开发助手。了解项目结构、命名规范、已有可复用接口（lib-oatpp宏、lib-common工具、BaseDAO方法、.skeleton一键创建宏）、各层代码模板、常见模式（MongoDB、Redis、事务、枚举字典、JWT鉴权）。在zero-one-his2607/his-cpp项目中编写或分析代码时调用。"
---

# HIS C++ 项目开发助手

本 skill 提供 `zero-one-his2607/his-cpp` 项目的完整知识——基于 oat++ 框架的 C++ 微服务 HIS（医院信息系统）。

**项目根目录**: `e:\git仓库\zero-one-his2607\his-cpp`

---

## 硬约束（必须遵守）

| 规则 | 说明 |
|------|------|
| 文件编码 | 所有 `.h`、`.cpp`、`.yaml` 源文件必须使用 **GB2312（代码页936）** 编码 |
| 注释语言 | 所有代码注释必须使用 **英文** |
| 禁止重复造轮子 | 写新代码前必须先看下方的"已有能力清单" |
| 命名规范 | 类名大驼峰(PascalCase)，方法/变量小驼峰(camelCase)，宏/常量全大写下划线(UPPER_SNAKE_CASE) |
| 文件操作 | 优先修改已有文件，避免新建文件 |
| 文档 | 不要主动创建 `.md` 或 `README` 文件，除非用户明确要求 |

---

## 项目目录结构

```
his-cpp/
├── .skeleton/              # 新模块模板骨架
│   ├── Macros.h            # 一键创建宏（ZO_CREATE_*系列）
│   └── stdafx.h            # 预编译头
├── lib-oatpp/              # oat++ 框架封装（DTO/VO/JWT/拦截器/响应）
├── lib-common/             # 通用基础设施（MongoClient、RedisClient、SnowFlake等）
├── lib-mysql/              # MySQL ORM层（BaseDAO、BaseDO、SqlSession、Mapper）
├── his-c2-opdoctor/        # 门诊医生工作站（业务模块）
│   ├── conf/data-source.yaml
│   ├── controller/         # 路由+端点定义
│   ├── service/            # 业务逻辑
│   ├── dao/                # 数据访问层（继承BaseDAO）
│   ├── domain/
│   │   ├── do/             # 数据库对象（继承BaseDO）
│   │   ├── dto/            # 请求DTO
│   │   ├── vo/             # 响应VO（继承JsonVO<T>）
│   │   └── query/          # 查询过滤条件（继承PageQuery）
│   ├── Macros.h
│   ├── zh-dict.yaml        # 中文显示名字典
│   └── CMakeLists.txt
├── his-c2-opnurse/         # 门诊护士工作站
├── CMakeLists.txt
├── build.sh
└── build-nodoc.sh
```

### 命名规范

| 类型 | 规范 | 示例 |
|------|------|------|
| DO类 | `{表名}DO` | `EmrDocument3DO` |
| DTO类 | `{业务名}DTO` | `chinapres3IssueDTO` |
| VO类 | `{业务名}ResultVO` | `chinapres3RevokeResultVO` |
| Query类 | `{业务名}Query` | `chinapres3ChargeItemQuery` |
| Controller | `{模块}Controller` | `chinapres3Controller` |
| Service | `{模块}Service` | `chinapres3Service` |
| DAO | `{模块}DAO` | `chinapres3ChargeItemDAO` |
| Mapper | `{模块}Mapper` | `chinapres3ChargeItemMapper` |
| Controller方法 | `exec小驼峰` | `execIssue()`、`execQueryAll()` |

---

## 已有能力清单（写代码前必读）

写任何新功能前，先检查此清单。已有封装就直接复用，不要自己重写。

### lib-oatpp：API接口定义宏

| 宏 | 使用场景 |
|-------|-------------|
| `API_ACCESS_DECLARE(类名)` | Controller的JWT鉴权声明——每个Controller都要写 |
| `API_DEF_ENDPOINT_INFO_AUTH(...)` | 需要认证的端点的Swagger文档定义 |
| `API_DEF_ENDPOINT_INFO_QUERY_AUTH(...)` | 带认证+查询参数的端点的Swagger文档定义 |
| `API_HANDLER_ENDPOINT_AUTH(...)` | 绑定路由+认证，请求体通过`BODY_DTO`自动解析 |
| `API_HANDLER_ENDPOINT_QUERY_AUTH(...)` | 绑定路由+认证，自动解析Query查询参数 |
| `API_HANDLER_ENDPOINT_NOPARAM_AUTH(...)` | 绑定路由+认证，无请求参数的端点 |
| `API_HANDLER_PAGE_PARAME` | 注入`pageIndex`+`pageSize`分页参数 |
| `API_HANDLER_AUTH_PARAME` | 注入JWT鉴权对象`authObject`（通过`authObject->getPayload()`获取用户信息） |
| `BODY_DTO(类型, 变量名)` | 自动将JSON请求体反序列化为DTO对象 |
| `API_DTO_FIELD_REQUIRE(类型, 名称, 描述, 是否必填)` | 定义DTO字段（含Swagger文档） |
| `API_DTO_FIELD_DEFAULT(类型, 名称, 描述)` | 定义DTO字段（仅描述） |
| `DTO_FIELD(类型, 名称)` + `DTO_FIELD_INFO(名称){...}` | 定义可选DTO字段 |
| `ZH_WORDS_GETTER("键")` | 从`zh-dict.yaml`获取中文显示文本 |
| `API_M_GET` / `API_M_POST` / `API_M_PUT` / `API_M_DEL` | HTTP方法常量 |

### lib-oatpp：响应帮助类

| 类/宏 | 使用场景 |
|-------------|-------------|
| `JsonVO<T>::success(data)` | 返回成功+数据（code=10000） |
| `JsonVO<T>::fail(data)` | 返回失败+数据（code=9999） |
| `NoDataJsonVO` + `init(ResultStatus)` | 只返回code+message，无data |
| `StringJsonVO` / `Int32JsonVO` / `BooleanJsonVO` | 预定义类型的JsonVO别名 |
| `PageDTO<T>` | 包装分页列表结果（含pageIndex、pageSize、total、pages、rows） |
| `RS_SUCCESS` / `RS_FAIL` / `RS_UNAUTHORIZED` / `RS_PARAMS_INVALID` / `RS_SERVER_ERROR` | 标准响应状态码 |

### lib-oatpp：JWT与鉴权

| 类/方法 | 使用场景 |
|--------------|-------------|
| `PayloadDTO`字段 | `getId()`、`getUsername()`、`getName()`、`getPractitionerId()`、`getOrganizationUnits()`、`getAuthorities()`——从JWT获取当前用户信息 |
| `authObject->getPayload()` | 在Controller/Service中获取PayloadDTO（通过`API_HANDLER_AUTH_PARAME`传入） |

### lib-common：基础设施工具

| 类 | 核心方法 | 使用场景 |
|-------|------------|-------------|
| `MongoClient` | `execute(集合名, lambda)`, `addOne(集合名, 文档)`, `addMultiple(集合名, 文档列表)` | MongoDB操作。推荐使用`execute`配合lambda灵活查询 |
| `RedisClient` | `execute<T>(lambda)` | Redis操作。lambda接收`Redis*`指针，可调用redis++全部API（get/set/hset/hget/del等） |
| `SnowFlake` | `getInstance()->nextId()` | 生成分布式唯一ID（uint64_t） |
| `UuidFacade` | `genUuid()` | 生成UUID字符串 |
| `TransactionManager` | 构造函数传入`(BaseDAO* 主DAO, 其他DAO...)`，然后调用`commit()`或`rollback()` | 多DAO事务。传第一个DAO引用+其余所有DAO，最后提交或回滚 |
| `NacosClient` | `getConfig(dataId)` | 从Nacos（或本地文件）读取YAML配置，返回`YAML::Node` |
| `SimpleDateTimeFormat` | `format()` | 获取当前时间格式化字符串（默认格式`"%Y-%m-%d %H:%M:%S"`） |
| `StringUtil` | `split()`、`trim()`、`replace()` | 字符串处理 |
| `ExcelComponent` | `writeVectorToFile()`、`readIntoVector()` | Excel导入导出（仅支持.xlsx） |
| `PdfComponent` | `drawText()`、`drawWithTemplate()`、`saveDocToFile()` | PDF生成 |
| `EmailSender` | `setEmailContent()`、`addRecvEmailAddr()`、`send()` | SMTP发送邮件 |
| `FastDfsClient` | `uploadFile()`、`downloadFile()`、`deleteFile()` | 分布式文件存储 |
| `RocketClient` | `productMsgAsync()`、`productMsgSync()`、`subscribe()` | RocketMQ消息队列 |
| `CharsetConvertHepler` | `utf8ToAnsi()`、`ansiToUtf8()`、`utf8ToGbk()`、`gbkToUtf8()` | 字符集转换 |

### lib-mysql：ORM层

| 类 | 核心方法 | 使用场景 |
|-------|------------|-------------|
| `BaseDAO::insert(DO)` | 将DO写入数据库（非自增主键） | 预生成ID后插入 |
| `BaseDAO::insertAutoPk(DO)` | 写入DO，返回自增ID | 主键自增的插入 |
| `BaseDAO::update(DO)` | 按主键更新 | 修改已有记录 |
| `BaseDAO::update(DO, where条件)` | 按自定义条件更新 | 条件更新 |
| `BaseDAO::deleteById<T>(id)` | 按主键删除 | 软删除或硬删除 |
| `SqlSession::executeQuery<T>(sql, mapper, params)` | 用Mapper查询 | DAO中自定义SELECT查询 |
| `SqlSession::executeQueryNumerical(sql, params)` | 查询count/max等数值 | 聚合查询 |
| `SqlSession::executeUpdate(sql, params)` | 执行DML语句 | 自定义UPDATE/DELETE |
| `Mapper<T>` | `mapper(ResultSet*) -> T` | 将ResultSet行数据转换为DO对象 |
| `SqlParam("s", shared_ptr<string>)` | | 预编译语句参数绑定 |
| `SQLPARAMS_PUSH(params, 类型码, C++类型, 值)` | | 向SqlParams向量追加参数 |

### .skeleton：一键创建宏

| 宏 | 功能 |
|-------|-------------|
| `ZO_CREATE_REDIS_CLIENT(变量名)` | 自动从Nacos配置创建RedisClient |
| `ZO_CREATE_MONGO_CLIENT(变量名)` | 自动从Nacos配置创建MongoClient |
| `ZO_CREATE_DFS_CLIENT(变量名)` | 自动从配置创建FastDfsClient |
| `ZO_CREATE_MAIL_SENDER(变量名)` | 自动从Nacos配置创建EmailSender |
| `ZO_STAR_DOMAIN_DO_TO_DTO(目标, 来源, 字段列表...)` | 将DO的值拷贝到DTO对象 |
| `ZO_STAR_DOMAIN_DTO_TO_DO(目标, 来源, 字段列表...)` | 将DTO的值拷贝到DO对象 |

### DO类宏（在DO类中使用的宏）

| 宏 | 用途 |
|-------|---------|
| `MYSQL_SYNTHESIZE(类型, 变量名, 函数名)` | 声明字段及getter/setter |
| `MYSQL_ADD_FIELD_PK("列名", "类型码", 变量)` | 注册主键字段 |
| `MYSQL_ADD_FIELD("列名", "类型码", 变量)` | 注册非空普通字段 |
| `MYSQL_ADD_FIELD_NULLABLE("列名", "类型码", 变量, 是否可空)` | 注册可空字段 |

字段类型码：`"s"`=string（字符串）, `"d"`=double（双精度浮点）, `"i"`=int32（32位整数）, `"ui"`=uint32（32位无符号整数）, `"ll"`=int64（64位整数）, `"ull"`=uint64（64位无符号整数）, `"b"`=bool（布尔）, `"dt"`=datetime（日期时间）

---

## 代码模板

### DO（数据库对象）

```cpp
#ifndef _XxxDO_H_
#define _XxxDO_H_
#include "../DoInclude.h"

/**
 * 对应数据库表: table_name
 */
class XxxDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, fieldName, FieldName);
    MYSQL_SYNTHESIZE(string, isDeleted, IsDeleted);

public:
    XxxDO() : BaseDO("table_name")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("field_name", "s", fieldName);
        MYSQL_ADD_FIELD("is_deleted", "s", isDeleted);
    }
};
typedef std::shared_ptr<XxxDO> PtrXxxDO;
#endif
```

### Mapper（结果集映射）

```cpp
#ifndef _XxxMapper_H_
#define _XxxMapper_H_
#include "Mapper.h"
#include "../../domain/do/模块名/XxxDO.h"

class XxxMapper : public Mapper<PtrXxxDO>
{
public:
    PtrXxxDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<XxxDO>();
        data->setId(resultSet->getString("id"));
        data->setFieldName(resultSet->getString("field_name"));
        return data;
    }
};
#endif
```

### DTO（请求体）

```cpp
#ifndef _XxxDTO_H_
#define _XxxDTO_H_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class XxxDTO : public oatpp::DTO
{
    DTO_INIT(XxxDTO, DTO);
    API_DTO_FIELD_REQUIRE(String, requiredField, ZH_WORDS_GETTER("模块.field.requiredField"), true);
    DTO_FIELD(String, optionalField);
    DTO_FIELD_INFO(optionalField) {
        info->description = ZH_WORDS_GETTER("模块.field.optionalField");
    }
};
#include OATPP_CODEGEN_END(DTO)
#endif
```

### VO（响应体）

```cpp
#ifndef _XxxResultVO_H_
#define _XxxResultVO_H_
#include "../../GlobalInclude.h"
#include "domain/vo/JsonVO.h"
#include OATPP_CODEGEN_BEGIN(DTO)

// 内层数据DTO——实际返回的业务数据
class XxxResultDTO : public oatpp::DTO
{
    DTO_INIT(XxxResultDTO, DTO);
    API_DTO_FIELD_REQUIRE(Int32, count, ZH_WORDS_GETTER("模块.field.count"), true);
    DTO_FIELD(String, message);
    DTO_FIELD_INFO(message) { info->description = ZH_WORDS_GETTER("模块.field.message"); }
};

// 外层VO——标准JSON包装体 {code, message, data}
class XxxResultVO : public JsonVO<XxxResultDTO::Wrapper>
{
    DTO_INIT(XxxResultVO, JsonVO<XxxResultDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif
```

### Query（查询过滤+分页）

```cpp
#pragma once
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class XxxQuery : public PageQuery
{
    DTO_INIT(XxxQuery, PageQuery);
    DTO_FIELD(String, filterField);
    DTO_FIELD_INFO(filterField) {
        info->description = ZH_WORDS_GETTER("模块.field.filterField");
    }
};
#include OATPP_CODEGEN_END(DTO)
```

### DAO（数据访问层）

**头文件 .h：**
```cpp
#ifndef _XxxDAO_H_
#define _XxxDAO_H_
#include "BaseDAO.h"
#include "XxxMapper.h"
#include "../../domain/do/模块名/XxxDO.h"
#include "../../domain/query/模块名/XxxQuery.h"

class XxxDAO : public BaseDAO
{
    std::string queryConditionBuilder(const XxxQuery::Wrapper& query, SqlParams& params);
public:
    uint64_t count(const XxxQuery::Wrapper& query);
    std::list<PtrXxxDO> selectWithPage(const XxxQuery::Wrapper& query);
    PtrXxxDO selectById(const std::string& id);
};
#endif
```

**实现文件 .cpp：**
```cpp
#include "stdafx.h"
#include "XxxDAO.h"
#include "XxxMapper.h"
#include "SqlSession.h"
#include <sstream>

uint64_t XxxDAO::count(const XxxQuery::Wrapper& query)
{
    std::ostringstream sql;
    SqlParams params;
    sql << "SELECT COUNT(*) FROM table_name WHERE 1=1";
    sql << queryConditionBuilder(query, params);
    return sqlSession->executeQueryNumerical(sql.str(), params);
}

std::list<PtrXxxDO> XxxDAO::selectWithPage(const XxxQuery::Wrapper& query)
{
    std::ostringstream sql;
    SqlParams params;
    sql << "SELECT id, field_name FROM table_name WHERE 1=1";
    sql << queryConditionBuilder(query, params);
    sql << " ORDER BY id DESC ";
    sql << "LIMIT " << ((query->pageIndex - 1) * query->pageSize) << "," << query->pageSize;
    XxxMapper mapper;
    return sqlSession->executeQuery<PtrXxxDO>(sql.str(), mapper, params);
}

PtrXxxDO XxxDAO::selectById(const std::string& id)
{
    std::ostringstream sql;
    sql << "SELECT id, field_name FROM table_name WHERE id=?";
    XxxMapper mapper;
    SqlParams params = { SqlParam("s", std::make_shared<std::string>(id)) };
    auto list = sqlSession->executeQuery<PtrXxxDO>(sql.str(), mapper, params);
    return list.empty() ? nullptr : list.front();
}

std::string XxxDAO::queryConditionBuilder(const XxxQuery::Wrapper& query, SqlParams& params)
{
    std::ostringstream sql;
    if (query->filterField) {
        auto val = query->filterField.getValue("");
        if (!val.empty()) {
            params.push_back(SqlParam("s", std::make_shared<std::string>(val)));
            sql << " AND field_name=?";
        }
    }
    return sql.str();
}
```

### Service（业务逻辑层）

**头文件 .h：**
```cpp
#ifndef _XxxService_H_
#define _XxxService_H_
#include "domain/dto/模块名/XxxDTO.h"
#include "domain/vo/模块名/XxxResultVO.h"
#include "../../dao/模块名/XxxDAO.h"

class XxxService
{
    XxxDAO xxxDAO;
public:
    XxxResultVO::Wrapper execMethod(const XxxDTO::Wrapper& dto, const PayloadDTO& payload);
};
#endif
```

**实现文件 .cpp：**
```cpp
#include "stdafx.h"
#include "XxxService.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"

XxxResultVO::Wrapper XxxService::execMethod(const XxxDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto resultVo = XxxResultVO::createShared();

    // 1. 参数校验
    auto field = dto->requiredField;
    if (!field || field.getValue("") == "") {
        resultVo->code = 400;
        resultVo->message = "Parameter requiredField cannot be empty";
        return resultVo;
    }

    // 2. 业务逻辑处理
    auto now = SimpleDateTimeFormat::format();
    auto currentUser = payload.getUsername();

    XxxDO data;
    data.setId(std::to_string(SnowFlake::getInstance()->nextId()));
    data.setFieldName(field.getValue(""));
    xxxDAO.insert(data);

    // 3. 组装返回结果
    auto resultData = XxxResultDTO::createShared();
    resultData->count = 1;
    resultData->message = "Success";
    resultVo->success(resultData);
    return resultVo;
}
```

### Controller（控制器）

```cpp
#ifndef _XxxController_H_
#define _XxxController_H_
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/模块名/XxxDTO.h"
#include "../../domain/vo/模块名/XxxResultVO.h"
#include "../../service/模块名/XxxService.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("模块.tags.tag")

class XxxController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(XxxController);
public:
    // Swagger文档定义
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("模块.端点.summary"),
        execMethod,
        XxxResultVO::Wrapper,
        API_TAG);
    // 路由绑定
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/模块/端点",
        execMethod,
        BODY_DTO(XxxDTO::Wrapper, dto),
        execMethod(dto, authObject->getPayload()));

private:
    XxxService service;
    XxxResultVO::Wrapper execMethod(const XxxDTO::Wrapper& dto, const PayloadDTO& payload) {
        return service.execMethod(dto, payload);
    }
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)
#endif
```

---

## 常见模式

### 1. MongoDB 操作

```cpp
// 创建客户端（一行代码，自动从Nacos/YAML读取配置）
ZO_CREATE_MONGO_CLIENT(mongoClient);

// 插入单条文档
auto doc = bsoncxx::builder::basic::document{};
doc.append(kvp("field", "value"));
auto insertedId = mongoClient.addOne("集合名", doc.extract());

// 通过execute做自定义查询
bool success = mongoClient.execute("集合名", [](mongocxx::collection* col) {
    auto cursor = col->find(document{} << "field" << "value" << finalize);
    for (auto&& doc : cursor) {
        // 处理文档
    }
    return true;
});
```

### 2. Redis 操作

```cpp
// 创建客户端（一行代码）
ZO_CREATE_REDIS_CLIENT(redisClient);

// 所有操作通过execute lambda执行
redisClient.execute<void>([](sw::redis::Redis* r) {
    r->set("key", "value");
    r->expire("key", 7200);  // 过期时间（秒）
});

auto val = redisClient.execute<std::string>([](sw::redis::Redis* r) {
    return r->get("key");
});

// Hash操作
redisClient.execute<void>([](sw::redis::Redis* r) {
    r->hset("hash_key", "field1", "value1");
    r->hset("hash_key", "field2", "value2");
});

auto fieldVal = redisClient.execute<std::string>([](sw::redis::Redis* r) {
    return r->hget("hash_key", "field1");
});
```

### 3. 多DAO事务

```cpp
try {
    TransactionManager tm(&dao1, &dao2, &dao3);
    // 此时所有DAO共享同一个数据库连接
    dao1.insert(do1);
    dao2.update(do2);
    dao3.insert(do3);
    tm.commit();
} catch (const std::exception& e) {
    // 如果在tm.commit()之前抛异常，事务自动回滚
}
```

### 4. 枚举字典（ter_code_system + ter_concept）

项目中的枚举值以纯字符串形式存储在业务表中（如`status_enum = "DRAFT"`）。码值到显示名的映射通过MySQL两张表完成：

- `ter_code_system` 定义枚举类型（如`name='category_enum'`）
- `ter_concept` 定义枚举值（`code='DRAFT'` → `display='草稿'`）

**用法**：C++后端只读写**码值**（字符串），显示名翻译交给前端处理。如果后端必须返回显示名，需要新建一个查`ter_concept`的DAO，按`code_system_id + code`查询display。

```cpp
// 后端只做码值的存储和比较，不做码值→显示名翻译
reqQuery->statusEnum = "ACTIVE";
if (status != "DRAFT") { /* 业务判断逻辑 */ }
```

### 5. JWT 获取用户信息

```cpp
// 在Controller中（通过API_HANDLER_AUTH_PARAME获得authObject参数）：
auto& payload = authObject->getPayload();
payload.getUsername();        // 登录账号
payload.getId();              // 用户唯一ID
payload.getName();            // 用户真实姓名
payload.getPractitionerId();  // 医护人员ID

// 在Service中（payload由Controller传入）：
auto currentUser = payload.getUsername();
auto orgId = payload.getId();
```

### 6. ID 生成

```cpp
// 雪花算法（uint64_t，分布式唯一）
auto id = std::to_string(SnowFlake::getInstance()->nextId());

// UUID（字符串）
UuidFacade uf;
auto uuid = uf.genUuid();  // 返回无分隔符的UUID字符串
```

### 7. 响应模式

```cpp
// 成功+数据
resultVo->success(resultData);  // 自动设置code=10000, message="success", data=resultData

// 成功+自定义消息
resultVo->message = "处方签发成功";
resultVo->success(resultData);

// 业务失败+自定义code和message
resultVo->code = 400;
resultVo->message = "参数不能为空";

// 无数据响应
auto vo = NoDataJsonVO::createShared();
vo->init(RS_SUCCESS);
return vo;
```

---

## 数据源配置

配置文件位于 `his-c2-opdoctor/conf/data-source.yaml`（其他模块同理）：

```yaml
spring:
  datasource:
    url: jdbc:mysql://host:port/zo_his?useUnicode=true&useSSL=false&characterEncoding=utf-8&serverTimezone=Asia/Shanghai
    username: root
    password: 123456
    druid:
     max-active: 5
  data:
    redis:
      host: 192.168.28.132
      port: 6379
      password: 01star
    mongodb:
      uri: mongodb://用户名:密码@主机:端口/数据库名
```

---

## 代码提交前自检清单

- [ ] 所有注释使用英文
- [ ] 源文件中无中文字符（zh-dict.yaml的值除外）
- [ ] 类名大驼峰，方法名小驼峰
- [ ] 已检查"已有能力清单"——没有重复造轮子
- [ ] DO继承BaseDO，构造函数传入表名，注册了所有字段
- [ ] DTO/VO/Query包裹在`OATPP_CODEGEN_BEGIN(DTO)`/`OATPP_CODEGEN_END(DTO)`中
- [ ] DAO继承BaseDAO，先用已有的`insert`/`update`/`deleteById`，不够用再写自定义SQL
- [ ] Controller端点同时有`API_DEF_*`（文档）和`API_HANDLER_*`（路由）宏
- [ ] Redis/Mongo客户端用`ZO_CREATE_*`宏创建，不要手动拼连接参数
- [ ] 事务使用`TransactionManager`模式
- [ ] ID使用`SnowFlake`或`UuidFacade`生成，不要自己手动构造