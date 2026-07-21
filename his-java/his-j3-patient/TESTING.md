# 患者管理接口测试文档

## 1. 文档信息

| 项目 | 内容 |
|---|---|
| 模块 | `his-j3-patient` |
| 测试对象 | 患者管理接口与领域模型定义 |
| 分支 | `j3-patient-zhaoyeqing` |
| 基准提交 | `9e4a63c` |
| Java 版本 | JDK 17 |
| 接口根路径 | `/basedata/patients` |

## 2. 测试范围和当前限制

本轮任务属于“接口与领域模型定义”阶段，测试目标如下：

1. 项目能够通过 Maven 编译。
2. Knife4j/Swagger 能正确展示 6 个患者接口。
3. 请求方式、路径、查询参数、路径参数和 JSON 请求体可以正确绑定。
4. Controller 的临时响应符合当前桩实现。

当前 Controller 尚未调用 Service、Mapper 和数据库，因此本轮**不验证**以下业务：

- 患者数据是否真正新增、修改或删除；
- 列表分页和条件查询是否返回真实数据；
- 身份证号、手机号的重复校验和格式校验；
- 患者编号生成、敏感信息脱敏和乐观锁；
- 患者被挂号或住院业务引用时的删除限制。

以上内容应在后续“业务接口实现”阶段补充集成测试。

## 3. 接口清单

| 编号 | 功能 | 方法 | 路径 | 当前预期数据 |
|---|---|---|---|---|
| PAT-01 | 获取选择患者列表 | GET | `/basedata/patients/options` | `data` 为 `null` 或不返回 |
| PAT-02 | 获取患者管理列表 | GET | `/basedata/patients/page` | `data` 为 `null` 或不返回 |
| PAT-03 | 获取患者详情 | GET | `/basedata/patients/{id}` | `data` 为 `null` 或不返回 |
| PAT-04 | 新增患者 | POST | `/basedata/patients` | `data` 为 `新增成功` |
| PAT-05 | 修改患者 | PUT | `/basedata/patients/{id}` | `data` 为 `修改成功` |
| PAT-06 | 删除患者 | DELETE | `/basedata/patients/{id}` | `data` 为 `删除成功` |

其中 PAT-02、PAT-04、PAT-05 为照夜清负责的接口。

## 4. 测试环境准备

### 4.1 Maven 编译

在 PowerShell 中执行：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java
mvn -s .\settings.xml -pl his-j3-patient -am clean compile -DskipTests
```

验收结果：各模块均显示 `SUCCESS`，最后显示 `BUILD SUCCESS`。

必须使用仓库内的 `settings.xml`，因为项目父 POM 位于课程的阿里云 Maven 私服，公共 Maven 仓库无法解析该父 POM。

### 4.2 启动患者服务

推荐在 IDEA 中启动：

1. 打开 `PatientApplication.java`。
2. 创建 Spring Boot 运行配置。
3. Active profiles 设置为 `test`。
4. 使用 JDK 17 运行。
5. 在控制台确认服务启动成功，并记录 `${sp.patient}` 对应的实际端口。

项目通过 Nacos 加载 `system.yaml`、`data-source.yaml` 等配置。启动前应确保能访问项目测试环境的 Nacos，并且 `system.yaml` 中已经配置 `sp.patient` 和 `sn.patient`。

如果测试环境的 Nacos 配置完整，可以直接按照上面的方式启动。当前测试环境的 `system.yaml`、`data-source.yaml` 等配置为空时，可使用下面的“本地接口契约测试模式”。该模式显式提供服务名和端口，并关闭当前桩接口不需要的数据源自动配置：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java
mvn -s .\settings.xml -pl his-j3-patient -am package -DskipTests

cd .\his-j3-patient
java -jar .\target\his-j3-patient-1.0.0-SNAPSHOT.jar `
  --spring.profiles.active=test `
  --sp.patient=10202 `
  --sn.patient=his-j3-patient `
  --spring.cloud.nacos.discovery.enabled=false `
  "--spring.autoconfigure.exclude=org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration,com.alibaba.druid.spring.boot3.autoconfigure.DruidDataSourceAutoConfigure,com.baomidou.mybatisplus.autoconfigure.MybatisPlusAutoConfiguration"
```

保持这个 PowerShell 窗口运行，再打开另一个 PowerShell 窗口执行第 6 节的接口请求。测试完成后，在服务窗口按 `Ctrl+C` 停止服务。

不建议从 `his-j3-patient` 目录直接执行 `mvn spring-boot:run`：PowerShell 需要特殊处理 `-D` 参数，而且该项目安装后的内部模块 POM 仍保留 `${revision}` 占位符，脱离根 reactor 后可能无法解析 `his-apis` 和 `his-common`。运行根目录构建生成的可执行 JAR 更稳定。

本地接口契约测试模式只用于验证路由、参数绑定、响应和 OpenAPI 文档，不能替代连接 Nacos、MySQL 后的真实业务集成测试。

### 4.3 设置测试地址

将下面的 `10202` 替换成启动日志中的实际端口：

```powershell
$baseUrl = "http://localhost:10202/basedata"
```

接口文档地址：

- Knife4j：`http://localhost:10202/basedata/doc.html`
- Swagger UI：`http://localhost:10202/basedata/swagger-ui.html`
- OpenAPI JSON：`http://localhost:10202/basedata/v3/api-docs`

### 4.4 Windows PowerShell 5.1 中文乱码

Windows PowerShell 5.1 的 `Invoke-RestMethod` 可能把 UTF-8 JSON 按其他字符集解码，出现 `æ...` 形式的中文乱码。只要响应中的 `code` 为 `10000`，接口仍然是成功的，乱码不属于后端接口错误。

推荐直接在 Knife4j/Swagger 中查看响应，或者使用下面的 UTF-8 原始响应读取方式：

```powershell
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()

$response = Invoke-WebRequest `
  -UseBasicParsing `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10"

$response.RawContentStream.Position = 0
$reader = [System.IO.StreamReader]::new(
  $response.RawContentStream,
  [System.Text.UTF8Encoding]::new($false)
)
$json = $reader.ReadToEnd()
$reader.Dispose()

$json
$json | ConvertFrom-Json
```

也可以安装并使用 PowerShell 7（命令为 `pwsh`），其 Web 请求对 UTF-8 响应的处理更可靠。

## 5. Knife4j 图形化测试方法

1. 打开 Knife4j 地址。
2. 找到 `patient / 患者管理` 分组。
3. 确认分组中展示 6 个接口。
4. 打开一个接口，点击“调试”或“Try it out”。
5. 填入本文件给出的参数或 JSON。
6. 点击“发送”，检查 HTTP 状态码及响应体。

当前桩接口的成功响应基本格式：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "具体返回内容"
}
```

当 `data` 为 `null` 时，由于项目启用了非空字段序列化，响应中可能不显示 `data` 字段，这两种结果均可接受。

## 6. PowerShell 接口测试

### 6.1 PAT-01 获取选择患者列表

```powershell
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/options?pageIndex=1&pageSize=10&name=张三"
```

预期：HTTP 200，`code` 为 `10000`，`message` 为 `操作成功`。

### 6.2 PAT-02 获取患者管理列表

基础分页测试：

```powershell
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10"
```

组合条件测试：

```powershell
$query = [System.Web.HttpUtility]::ParseQueryString('')
$query['pageIndex'] = '1'
$query['pageSize'] = '10'
$query['name'] = '张三'
$query['idCard'] = '110101199001011234'
$query['phone'] = '13800000000'
$query['genderEnum'] = 'MALE'
$query['statusEnum'] = 'ACTIVE'

Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?$($query.ToString())"
```

预期：HTTP 200，`code` 为 `10000`。当前阶段 `data` 为 `null` 或不显示。

### 6.3 PAT-03 获取患者详情

```powershell
$patientId = '322663105141149696'

Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/$patientId"
```

预期：HTTP 200，`code` 为 `10000`。当前阶段不会查询真实患者。

### 6.4 PAT-04 新增患者

```powershell
$createBody = @{
  name                 = '张三'
  genderEnum           = 'MALE'
  birthDate            = '1990-01-01'
  idCard               = '110101199001011234'
  maritalStatusEnum    = 'MARRIED'
  prfsEnum             = 'EMPLOYEE'
  phone                = '13800000000'
  address              = '北京市朝阳区示例路1号'
  addressProvince      = '北京市'
  addressCity          = '北京市'
  addressDistrict      = '朝阳区'
  addressStreet        = '示例街道'
  nationalityCode      = 'HAN'
  bloodTypeAbo         = 1
  bloodTypeRh          = 1
  workCompany          = '示例科技有限公司'
  nativePlace          = '北京市'
  countryCode          = 'CN'
  linkName             = '李四'
  linkRelationCode     = 1
  linkTelcom           = '13900000000'
  organizationId       = '322663105141149696'
} | ConvertTo-Json

Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType 'application/json; charset=utf-8' `
  -Body $createBody
```

当前预期响应：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "新增成功"
}
```

注意：当前只是桩响应，不代表数据库中已经产生患者记录。

### 6.5 PAT-05 修改患者

```powershell
$patientId = '322663105141149696'

$updateBody = @{
  name              = '张三（修改）'
  genderEnum        = 'MALE'
  phone             = '13800000001'
  address           = '北京市海淀区示例路2号'
  organizationId    = '322663105141149696'
  statusEnum        = 'ACTIVE'
  activeFlag        = $true
  tempFlag          = $false
  version           = 0
} | ConvertTo-Json

Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType 'application/json; charset=utf-8' `
  -Body $updateBody
```

当前预期响应：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "修改成功"
}
```

注意：当前不会检查患者是否存在，也不会真正执行乐观锁更新。

### 6.6 PAT-06 删除患者

```powershell
$patientId = '322663105141149696'

Invoke-RestMethod `
  -Method Delete `
  -Uri "$baseUrl/patients/$patientId"
```

当前预期响应：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "删除成功"
}
```

注意：当前不会真正软删除数据库记录。

## 7. 核心验收用例

| 用例编号 | 测试点 | 操作 | 预期结果 |
|---|---|---|---|
| TC-PAT-001 | 项目编译 | 执行第 4.1 节 Maven 命令 | `BUILD SUCCESS` |
| TC-PAT-002 | OpenAPI 暴露 | 打开 Knife4j 患者管理分组 | 展示 6 个接口，方法和路径正确 |
| TC-PAT-003 | 列表参数绑定 | 请求 PAT-02，传分页和组合条件 | HTTP 200，`code=10000` |
| TC-PAT-004 | 新增请求体绑定 | 使用完整 JSON 请求 PAT-04 | HTTP 200，`data=新增成功` |
| TC-PAT-005 | 修改路径及请求体绑定 | 使用患者 ID 和 JSON 请求 PAT-05 | HTTP 200，`data=修改成功` |
| TC-PAT-006 | 详情路径绑定 | 使用患者 ID 请求 PAT-03 | HTTP 200，`code=10000` |
| TC-PAT-007 | 删除路径绑定 | 使用患者 ID 请求 PAT-06 | HTTP 200，`data=删除成功` |

## 8. 测试记录模板

| 用例编号 | 执行时间 | 测试环境 | 实际结果 | 是否通过 | 问题说明 |
|---|---|---|---|---|---|
| TC-PAT-001 |  |  |  |  |  |
| TC-PAT-002 |  |  |  |  |  |
| TC-PAT-003 |  |  |  |  |  |
| TC-PAT-004 |  |  |  |  |  |
| TC-PAT-005 |  |  |  |  |  |
| TC-PAT-006 |  |  |  |  |  |
| TC-PAT-007 |  |  |  |  |  |

## 9. 后续业务实现阶段应补充的测试

1. 姓名、性别、建档机构等必填字段校验。
2. 身份证号、手机号和出生日期格式校验。
3. 身份证号重复、手机号重复及疑似患者匹配规则。
4. 患者编号生成及数据库唯一约束冲突处理。
5. 列表分页、筛选、排序和敏感信息脱敏。
6. 修改不存在患者、版本号冲突和越权修改。
7. 删除不存在患者、重复删除及业务引用限制。
8. 新增、修改和删除操作的审计日志。
