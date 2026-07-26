# 患者管理模块接口测试文档

## 1. 文档信息

| 项目 | 内容 |
|---|---|
| 模块 | `his-j3-patient` |
| 分支 | `j3-patient-zhaoyeqing` |
| 负责人 | 照夜清 |
| 文档修订日期 | 2026-07-26 |
| 测试状态 | 原型契约改造已完成自动化验证；真实环境需按第 9 节重新手工验收 |
| 服务名 | `his-j3-patient` |
| 服务端口 | `10202` |
| 上下文路径 | `/j3-basedata` |
| 直接访问地址 | `http://localhost:10202/j3-basedata` |
| Nacos 命名空间 | `his-dev` |
| Nacos 分组 | `DEFAULT_GROUP` |

本文档用于说明患者管理列表、新增患者和修改患者三个接口的请求契约、JSON
传输格式、自动化测试、真实环境手工测试步骤、实际结果及当前边界。

所有会写入患者数据的用例只能在开发库或测试库执行，禁止在生产环境执行。

## 2. 测试范围

### 2.1 本次验收接口

| 功能 | 方法 | 服务内路径 | 请求数据位置 |
|---|---|---|---|
| 获取患者管理列表 | `GET` | `/patients/page` | URL Query 参数 |
| 新增患者 | `POST` | `/patients` | JSON Request Body |
| 修改患者 | `PUT` | `/patients/{id}` | Path 参数 + JSON Request Body |

完整直接访问路径为：

```text
http://localhost:10202/j3-basedata/patients/page
http://localhost:10202/j3-basedata/patients
http://localhost:10202/j3-basedata/patients/{id}
```

### 2.2 不在本次分工内的接口

以下接口由其他负责人实现，不作为本文档的通过条件：

- 获取患者选择列表；
- 获取患者详情；
- 删除患者。

这些接口的实现和验收状态由对应负责人确认，不能用本文档的测试结果代替。

## 3. HTTP 与 JSON 传输规范

### 3.1 通用约定

1. JSON 编码统一使用 UTF-8。
2. `POST`、`PUT` 请求必须携带：

   ```http
   Content-Type: application/json; charset=utf-8
   ```

3. `GET` 请求使用 URL Query 参数，不发送 JSON Body。
4. 属性名使用 `lowerCamelCase`，例如 `organizationId`。
5. 患者 ID、机构 ID、患者编号和身份证号按字符串传输，不能转为浮点数。
6. `birthDate` 请求同时接受 `yyyy-MM-dd` 和原型使用的
   `yyyy-MM-dd HH:mm:ss`。
7. `deceasedDate` 请求使用 `yyyy-MM-dd HH:mm:ss`；日期时间响应采用
   ISO-8601 本地日期时间，例如
   `2026-07-24T14:00:24.961003`。
8. 布尔值使用 JSON 原生值 `true`、`false`，不能使用字符串
   `"true"`、`"false"`。
9. 枚举值使用大写英文编码，例如 `MALE`、`ACTIVE`。
10. JSON 不允许注释、尾随逗号或未加双引号的属性名。

### 3.2 统一响应结构

所有患者接口使用以下响应外壳：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": {}
}
```

本次实测出现的响应码如下：

| `code` | 含义 | 示例 |
|---:|---|---|
| `10000` | 操作成功 | 查询、新增或修改成功 |
| `9995` | 参数校验失败 | 必填项为空、页码小于 1 |
| `9999` | 患者业务校验失败 | 身份证重复、版本冲突 |

判断业务是否成功应检查 `code`，不能只根据 HTTP 请求是否返回判断。

### 3.3 PowerShell 序列化要求

PowerShell 中应先创建对象，再使用 `ConvertTo-Json` 序列化：

```powershell
$bodyObject = @{
  name = "照夜清"
  genderEnum = "MALE"
}

$bodyJson = $bodyObject | ConvertTo-Json -Depth 5
$bodyJson
```

发送请求时，将序列化后的 JSON 字符串传给 `-Body`：

```powershell
Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $bodyJson
```

## 4. 接口数据契约

### 4.1 患者管理列表查询参数

请求示例：

```http
GET /patients/page?pageIndex=1&pageSize=10&keyword=ZYQ
```

| 参数 | 类型 | 必填 | 说明 |
|---|---|---|---|
| `pageIndex` | integer | 是 | 页码，从 1 开始 |
| `pageSize` | integer | 是 | 每页数量，当前实现只限制最小值为 1 |
| `keyword` | string | 否 | 匹配姓名、患者编号、身份证、拼音码、已有五笔码 |
| `name` | string | 否 | 姓名模糊查询 |
| `busNo` | string | 否 | 患者编号查询 |
| `idCard` | string | 否 | 身份证号查询，传未脱敏原值 |
| `phone` | string | 否 | 手机号查询，传未脱敏原值 |
| `genderEnum` | string | 否 | 性别编码 |
| `statusEnum` | string | 否 | 患者状态编码 |
| `activeFlag` | boolean | 否 | 启用标记 |
| `tempFlag` | boolean | 否 | 临时建档标记 |
| `organizationId` | string | 否 | 建档机构 ID |

分页成功响应示例：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": {
    "pageIndex": 1,
    "pageSize": 10,
    "total": 1,
    "pages": 1,
    "rows": [
      {
        "id": "2080508921788395521",
        "busNo": "PA00000006",
        "name": "照夜清修改",
        "age": 36,
        "genderEnum": "MALE",
        "genderEnumText": "男性",
        "birthDate": "1990-01-01",
        "idCard": "11010119900101957X",
        "phone": "13900005678",
        "statusEnum": "ACTIVE",
        "activeFlag": true,
        "tempFlag": false,
        "organizationId": "322663308812357632",
        "createdAt": "2026-07-24T12:22:35.484487",
        "updatedAt": "2026-07-24T14:00:24.961003"
      }
    ]
  }
}
```

原型表格直接绑定 `age`、`genderEnumText`、`phone` 和 `idCard`，
没有前端字段转换或解密逻辑，因此管理列表接口直接返回这些字段的原值。

### 4.2 新增患者 JSON

最小请求 JSON：

```json
{
  "name": "照夜清",
  "genderEnum": "MALE",
  "age": 36,
  "birthDate": "1990-01-01",
  "idCard": "11010119900101957X",
  "organizationId": "322663308812357632"
}
```

本次手工测试使用的请求 JSON：

```json
{
  "name": "照夜清",
  "genderEnum": "MALE",
  "age": 36,
  "birthDate": "1990-01-01",
  "idCard": "11010119900101957X",
  "phone": "13800004553",
  "organizationId": "322663308812357632"
}
```

新增字段定义：

| 字段 | 类型 | 必填 | 约束或说明 |
|---|---|---|---|
| `name` | string | 是 | 患者姓名，最大 255 个字符 |
| `genderEnum` | string | 是 | `MALE`、`FEMALE`、`UNKNOWN` |
| `age` | integer | 是 | 0 至 150，必须与出生日期计算结果一致 |
| `birthDate` | string | 是 | `yyyy-MM-dd` 或 `yyyy-MM-dd HH:mm:ss`，不能晚于当天 |
| `idCard` | string | 是 | 15 位数字或 18 位，末位允许 `X`/`x` |
| `maritalStatusEnum` | string | 否 | 婚姻状态编码 |
| `prfsEnum` | string | 否 | 职业编码 |
| `phone` | string | 否 | 联系电话，当前业务校验长度为 6 至 32 |
| `address` | string | 否 | 详细地址，最大 255 个字符 |
| `addressProvince` | string | 否 | 省级行政区编码 |
| `addressCity` | string | 否 | 市级行政区编码 |
| `addressDistrict` | string | 否 | 区县行政区编码 |
| `addressStreet` | string | 否 | 街道编码 |
| `nationalityCode` | string | 否 | 国籍编码，如 `CHN` |
| `bloodAbo` | integer | 否 | ABO 血型编码，由业务字典约束 |
| `bloodRh` | integer | 否 | RH 血型编码，由业务字典约束 |
| `workCompany` | string | 否 | 工作单位 |
| `nativePlace` | string | 否 | 籍贯 |
| `countryCode` | string | 否 | 国家或地区编码 |
| `linkName` | string | 否 | 联系人姓名 |
| `linkRelationCode` | integer | 否 | 联系人关系编码 |
| `linkTelcom` | string | 否 | 联系人电话 |
| `deceasedDate` | string | 否 | `yyyy-MM-dd HH:mm:ss`，不能晚于当前时间或早于出生日期 |
| `organizationId` | string | 是 | 有效且启用的建档机构 ID |

新增成功响应：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "新增成功"
}
```

新增成功后，服务端自动设置：

- `statusEnum=ACTIVE`；
- `activeFlag=true`；
- `tempFlag=false`；
- `version=0`；
- `busNo=PA` 加 8 位数字；
- 根据姓名生成拼音首字母码。

### 4.3 修改患者 JSON

修改接口使用：

```http
PUT /patients/{id}
```

请求体继承新增患者字段，证件号同样必填，不能省略、置空或删除。
`version` 与参考前端兼容，允许不传；传入时仍执行客户端版本检查，
数据库更新始终使用 MyBatis-Plus 乐观锁。

本次手工测试使用的请求 JSON：

```json
{
  "name": "照夜清修改",
  "genderEnum": "MALE",
  "age": 36,
  "birthDate": "1990-01-01",
  "idCard": "11010119900101957X",
  "phone": "13900005678",
  "organizationId": "322663308812357632",
  "version": 0
}
```

修改专用字段：

| 字段 | 类型 | 必填 | 说明 |
|---|---|---|---|
| `version` | integer | 否 | 当前乐观锁版本，最小值为 0；传入旧版本时拒绝修改 |
| `statusEnum` | string | 否 | DTO 支持传入，但普通资料修改不允许改变现有状态 |
| `activeFlag` | boolean | 否 | DTO 支持传入，但普通资料修改不允许改变现有值 |
| `tempFlag` | boolean | 否 | DTO 支持传入，但普通资料修改不允许改变现有值 |

修改成功响应：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "修改成功"
}
```

使用旧版本号再次提交时，响应示例：

```json
{
  "code": 9999,
  "message": "患者资料已被其他人修改，请刷新后重试"
}
```

### 4.4 错误响应 JSON

必填字段校验失败：

```json
{
  "code": 9995,
  "message": "上传参数异常",
  "data": "[organizationId]建档机构ID不能为空"
}
```

证件号格式错误：

```json
{
  "code": 9999,
  "message": "证件号格式不正确，应为15位或18位身份证号"
}
```

重复身份证：

```json
{
  "code": 9999,
  "message": "该身份证号已存在患者档案"
}
```

禁止修改状态字段：

```json
{
  "code": 9999,
  "message": "普通资料修改不允许变更启用标记"
}
```

患者不存在：

```json
{
  "code": 9999,
  "message": "患者不存在或已删除"
}
```

## 5. 测试环境

| 组件 | 本次环境 |
|---|---|
| 操作系统 | Windows + Anolis OS 虚拟机 |
| Java | JDK 17 |
| Maven | 项目 Maven 配置 |
| 数据库 | MySQL 8 |
| Nacos | 3.1.1，standalone |
| Nacos 控制台 | `http://192.168.28.132:8080` |
| Nacos 服务端口 | `192.168.28.132:8848` |
| 数据库 | `zo_his` |
| 测试机构 ID | `322663308812357632` |

测试数据使用虚构姓名、电话和身份证格式数据，不使用真实患者信息。

## 6. 自动化测试

### 6.1 标准执行命令

必须从 Maven 聚合工程执行，不能只在 `his-j3-patient` 子目录执行：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java

mvn -s .\settings.xml `
  -pl his-j3-patient `
  -am `
  "-Dmaven.test.skip=false" `
  "-DskipTests=false" `
  test
```

参数含义：

- `-pl his-j3-patient`：选择患者模块；
- `-am`：同时构建患者模块依赖的仓库内模块；
- 两个跳过测试参数都显式设为 `false`：覆盖父工程中的跳过测试配置；
- `test`：编译并执行测试。

如果在子模块直接执行 `mvn test`，Maven 可能尝试从远程仓库下载本项目的
`his-apis` 和 `his-common` 快照并报依赖解析失败。

### 6.2 2026-07-26 原型契约改造后的实际结果

```text
[INFO] his-java ........................................... SUCCESS
[INFO] his-common ......................................... SUCCESS
[INFO] his-domain ......................................... SUCCESS
[INFO] his-apis ........................................... SUCCESS
[INFO] his-j3-patient ..................................... SUCCESS
[INFO] BUILD SUCCESS
```

测试统计：

| 测试类 | 数量 | 失败 | 错误 | 跳过 |
|---|---:|---:|---:|---:|
| `PatientControllerTest` | 2 | 0 | 0 | 0 |
| `PatientDtoValidationTest` | 5 | 0 | 0 | 0 |
| `PatientJsonContractTest` | 2 | 0 | 0 | 0 |
| `PatientMapperContractTest` | 1 | 0 | 0 | 0 |
| `PatientCreateUpdateServiceTest` | 5 | 0 | 0 | 0 |
| `PatientServiceImplTest`（含嵌套测试） | 11 | 0 | 0 | 0 |
| `PatientNameCodeUtilsTest` | 2 | 0 | 0 | 0 |
| **合计** | **28** | **0** | **0** | **0** |

## 7. Nacos 与服务启动

### 7.1 Nacos 配置检查

在 `his-dev` 命名空间、`DEFAULT_GROUP` 中确认以下 Data ID 存在：

- `system.yaml`；
- `data-source.yaml`；
- `third-services.yaml`；
- `gateway.yaml`；
- `key-config.yaml`。

`data-source.yaml` 必须连接开发或测试 MySQL。配置文件、测试文档和 Git 提交中
不得出现真实密码、Token 或密钥。

### 7.2 安全设置 Nacos 凭据

不要把密码直接写入命令历史或本文档。使用安全输入：

```powershell
$env:NACOS_USERNAME = "<Nacos 用户名>"

$secureNacosPassword = Read-Host `
  "请输入 Nacos 密码" `
  -AsSecureString

$env:NACOS_PASSWORD = [System.Net.NetworkCredential]::new(
  "",
  $secureNacosPassword
).Password
```

### 7.3 启动患者服务

如果当前尚未生成可执行 JAR，先从聚合工程打包。`package` 会再次执行测试：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java

mvn -s .\settings.xml `
  -pl his-j3-patient `
  -am `
  "-Dmaven.test.skip=false" `
  "-DskipTests=false" `
  package
```

然后启动患者服务：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java\his-j3-patient

java "-Dfile.encoding=UTF-8" `
  -jar .\target\his-j3-patient-1.0.0-SNAPSHOT.jar `
  "--spring.profiles.active=dev" `
  "--spring.cloud.nacos.server-addr=192.168.28.132:8848"
```

如果 Nacos 自动选择了虚拟机无法访问的 Windows 网卡，可增加：

```powershell
"--spring.cloud.nacos.discovery.ip=192.168.28.1"
```

启动成功标准：

1. 日志出现 `Started PatientApplication`；
2. 未出现 Nacos 配置导入失败；
3. 未出现数据库连接失败；
4. 健康检查返回 `UP`；
5. Nacos 服务列表存在 `his-j3-patient` 健康实例。

测试结束后可清除当前 PowerShell 会话中的密码变量：

```powershell
Remove-Item Env:NACOS_PASSWORD -ErrorAction SilentlyContinue
```

## 8. 手工验收准备

另开一个 PowerShell 窗口：

```powershell
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()

$baseUrl = "http://localhost:10202/j3-basedata"
$orgId = "322663308812357632"
$testBirthDate = "1990-01-01"
$testAge = [int]((Get-Date).Date.Year - 1990)

$idCardSuffix = Get-Random -Minimum 1000 -Maximum 10000
$testIdCard = "11010119900101$idCardSuffix"

$phoneSuffix = Get-Random -Minimum 10000000 -Maximum 100000000
$testPhone = "138$phoneSuffix"
```

注意：

- `$orgId` 必须是当前测试库中有效、启用的机构；
- 随机身份证仅用于当前版本的结构格式测试，不代表真实身份信息；
- 如果后续增加身份证校验位验证，应改用经过校验的专用测试夹具；
- 如果执行日期早于当年 1 月 1 日以外的测试生日，应按实际生日重新计算
  `$testAge`，确保年龄与出生日期一致；
- 当前窗口中的 `$testIdCard`、`$testPhone` 在整个测试流程中保持不变。

## 9. 手工测试用例

### 9.1 健康检查

```powershell
$healthResult = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/actuator/health"

$healthResult | ConvertTo-Json -Depth 8
```

预期：`status=UP`，数据库和服务发现组件健康。

### 9.2 列表、分页、排序和原型展示字段

```powershell
$page1 = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=2"

$page2 = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=2&pageSize=2"

$page1 | ConvertTo-Json -Depth 8
$page2 | ConvertTo-Json -Depth 8
```

检查：

1. 两个响应的 `code` 都为 `10000`；
2. `pageIndex`、`pageSize`、`total`、`pages` 和 `rows` 正确；
3. 两页患者 ID 不重复；
4. 数据按 `createdAt`、`id` 从新到旧排序；
5. 每行包含原型直接绑定的 `age` 和 `genderEnumText`；
6. `idCard`、`phone` 返回数据库原值，由前端表格宽度负责省略展示，不由后端脱敏。

超大页码应返回空数组：

```powershell
$emptyPageResult = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=9999&pageSize=10"

$emptyPageResult | ConvertTo-Json -Depth 8
```

非法页码应被拒绝：

```powershell
$invalidPageResult = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=0&pageSize=10"

$invalidPageResult | ConvertTo-Json -Depth 8
```

预期：

```json
{
  "code": 9995,
  "message": "上传参数异常",
  "data": "[pageIndex]页码最小值为1"
}
```

当前 `PageQuery` 没有定义 `pageSize` 上限，因此 `pageSize=101` 会被接受。该行为
记录为后续性能优化项，不作为当前接口失败：

```powershell
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=101"
```

### 9.3 必填项和身份证格式

缺少必填项：

```powershell
$invalidRequiredBody = @{
  name = ""
  genderEnum = ""
  organizationId = ""
} | ConvertTo-Json

$invalidRequiredResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $invalidRequiredBody

$invalidRequiredResult | ConvertTo-Json -Depth 8
```

非法身份证：

```powershell
$invalidIdCardBody = @{
  name = "非法证件患者"
  genderEnum = "MALE"
  age = $testAge
  birthDate = $testBirthDate
  idCard = "123456"
  organizationId = $orgId
} | ConvertTo-Json

$invalidIdCardResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $invalidIdCardBody

$invalidIdCardResult | ConvertTo-Json -Depth 8
```

预期两个请求均失败，不能写入患者数据。

### 9.4 正式新增患者

```powershell
$createBodyObject = @{
  name = "  照夜清  "
  genderEnum = "MALE"
  age = $testAge
  birthDate = "$testBirthDate 00:00:00"
  idCard = $testIdCard
  phone = " $testPhone "
  organizationId = $orgId
}

$createBodyJson = $createBodyObject |
  ConvertTo-Json -Depth 5

$createBodyJson

$createResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $createBodyJson

$createResult | ConvertTo-Json -Depth 8
```

预期：

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": "新增成功"
}
```

查询新增患者：

```powershell
$createdPage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=$testIdCard"

$createdPatient = $createdPage.data.rows |
  Select-Object -First 1

$patientId = [string]$createdPatient.id

$createdPage | ConvertTo-Json -Depth 8
```

检查：

- 姓名去除首尾空格后为 `照夜清`；
- `busNo` 符合 `^PA\d{8}$`；
- `statusEnum=ACTIVE`；
- `activeFlag=true`；
- `tempFlag=false`；
- `age=$testAge`、`genderEnumText=男性`；
- 身份证和手机号为请求中的原值；
- 患者 ID 按字符串保存。

### 9.5 拼音与组合条件查询

拼音查询：

```powershell
$pinyinResult = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&keyword=ZYQ"

$pinyinResult | ConvertTo-Json -Depth 8
```

组合查询：

```powershell
$encodedName = [Uri]::EscapeDataString("照夜清")

$filterUri = "$baseUrl/patients/page" +
  "?pageIndex=1&pageSize=10" +
  "&name=$encodedName" +
  "&busNo=$($createdPatient.busNo)" +
  "&idCard=$testIdCard" +
  "&phone=$testPhone" +
  "&genderEnum=MALE" +
  "&statusEnum=ACTIVE" +
  "&activeFlag=true" +
  "&tempFlag=false" +
  "&organizationId=$orgId"

$filterResult = Invoke-RestMethod `
  -Method Get `
  -Uri $filterUri

$filterResult | ConvertTo-Json -Depth 8
```

预期只返回刚创建的患者。

### 9.6 重复身份证

```powershell
$duplicateBody = @{
  name = "重复患者"
  genderEnum = "MALE"
  age = $testAge
  birthDate = $testBirthDate
  idCard = $testIdCard
  phone = "13900001111"
  organizationId = $orgId
} | ConvertTo-Json

$duplicateResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $duplicateBody

$duplicateResult | ConvertTo-Json -Depth 8
```

预期：

```json
{
  "code": 9999,
  "message": "该身份证号已存在患者档案"
}
```

### 9.7 修改患者

新创建患者初始版本为 `0`：

```powershell
$updateBodyObject = @{
  name = "照夜清修改"
  genderEnum = "MALE"
  age = $testAge
  birthDate = $testBirthDate
  idCard = $testIdCard
  phone = "13900005678"
  organizationId = $orgId
  version = 0
}

$updateBodyJson = $updateBodyObject |
  ConvertTo-Json -Depth 5

$updateResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body $updateBodyJson

$updateResult | ConvertTo-Json -Depth 8
```

修改后查询：

```powershell
$updatedPage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=$testIdCard"

$updatedPinyin = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&keyword=ZYQXG"

$updatedPage | ConvertTo-Json -Depth 8
$updatedPinyin | ConvertTo-Json -Depth 8
```

检查：

- 姓名为 `照夜清修改`；
- 手机号为 `13900005678`，证件号保持为 `$testIdCard`；
- 患者 ID、患者编号和创建时间不变；
- 更新时间发生变化；
- 新拼音码 `ZYQXG` 能查询到该患者。

### 9.8 乐观锁冲突

再次提交仍携带 `version=0` 的 `$updateBodyJson`：

```powershell
$staleResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body $updateBodyJson

$staleResult | ConvertTo-Json -Depth 8
```

预期返回版本冲突，不能覆盖第一次修改。

### 9.9 普通修改禁止改变状态字段

第一次成功修改后数据库版本为 `1`：

```powershell
$stateChangeBody = @{
  name = "照夜清修改"
  genderEnum = "MALE"
  age = $testAge
  birthDate = $testBirthDate
  idCard = $testIdCard
  phone = "13900005678"
  organizationId = $orgId
  activeFlag = $false
  version = 1
} | ConvertTo-Json

$stateChangeResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body $stateChangeBody

$stateChangeResult | ConvertTo-Json -Depth 8
```

预期返回失败，`activeFlag` 保持 `true`。

### 9.10 修改不存在患者

```powershell
$missingBody = @{
  name = "不存在患者"
  genderEnum = "MALE"
  age = $testAge
  birthDate = $testBirthDate
  idCard = $testIdCard
  organizationId = $orgId
  version = 0
} | ConvertTo-Json

$missingResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/999999999999999999" `
  -ContentType "application/json; charset=utf-8" `
  -Body $missingBody

$missingResult | ConvertTo-Json -Depth 8
```

预期：

```json
{
  "code": 9999,
  "message": "患者不存在或已删除"
}
```

### 9.11 同身份证并发建档

此用例会产生一条额外测试数据，只能在可恢复的测试库执行。

```powershell
$raceSuffix = Get-Random -Minimum 1000 -Maximum 10000
$raceIdCard = "11010119900202$raceSuffix"

$raceBody = @{
  name = "并发测试患者"
  genderEnum = "MALE"
  age = $testAge
  birthDate = "1990-02-02"
  idCard = $raceIdCard
  phone = "13700001234"
  organizationId = $orgId
} | ConvertTo-Json -Compress

$jobs = 1..2 | ForEach-Object {
  Start-Job -ScriptBlock {
    param($url, $body)

    try {
      Invoke-RestMethod `
        -Method Post `
        -Uri $url `
        -ContentType "application/json; charset=utf-8" `
        -Body $body
    }
    catch {
      [pscustomobject]@{
        code = -1
        message = $_.Exception.Message
      }
    }
  } -ArgumentList "$baseUrl/patients", $raceBody
}

$raceResults = @($jobs | Receive-Job -Wait)
$jobs | Remove-Job

$raceResults | ConvertTo-Json -Depth 8

$successCount = @(
  $raceResults | Where-Object code -eq 10000
).Count

$failureCount = @(
  $raceResults | Where-Object code -ne 10000
).Count

if ($successCount -ne 1 -or $failureCount -ne 1) {
  throw "并发重复建档结果异常"
}
```

最终按身份证查询：

```powershell
$racePage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=$raceIdCard"

$racePage | ConvertTo-Json -Depth 8

if ($racePage.data.total -ne 1) {
  throw "并发建档产生重复患者"
}
```

预期一笔成功、一笔失败，数据库中最终只有一条记录。

## 10. Nacos 注册验收

进入 Nacos 控制台的 `his-dev` 命名空间，打开“服务管理 → 服务列表”。

验收项：

| 项目 | 预期 |
|---|---|
| 服务名 | `his-j3-patient` |
| 分组 | `DEFAULT_GROUP` |
| 实例数 | 至少 1 |
| 健康实例数 | 至少 1 |
| 端口 | `10202` |
| 实例状态 | 健康 |

2026-07-24 实测服务列表显示：

- 集群数目：1；
- 实例数：1；
- 健康实例数：1；
- 触发保护阈值：`false`。

## 11. 2026-07-24 变更前实际验收记录

本节保留用户已经完成的真实环境测试记录，仅用于追溯。其请求体和“后端脱敏”
结论属于本次原型契约改造前的旧版本，不能作为 2026-07-26 代码的最终验收结果；
新版本必须按第 9 节重新执行手工验收。

| 编号 | 测试项 | 实际结果 | 结论 |
|---:|---|---|---|
| 1 | Maven 聚合测试 | 5 个 Reactor 模块成功，17 个测试全部通过 | 通过 |
| 2 | 健康检查 | `status=UP` | 通过 |
| 3 | 初始列表 | 查询到 5 条初始化患者数据 | 通过 |
| 4 | 第二页分页 | `pageIndex=2`，返回 2 条，顺序正确 | 通过 |
| 5 | 超大页码 | `pageIndex=9999` 返回空 `rows` | 通过 |
| 6 | 非法页码 | `pageIndex=0` 返回 `code=9995` | 通过 |
| 7 | 必填字段校验 | 返回 `code=9995` | 通过 |
| 8 | 非法身份证 | 返回 `code=9999` | 通过 |
| 9 | 正式新增 | 返回 `code=10000` | 通过 |
| 10 | 新患者持久化 | ID 为字符串，编号为 `PA00000006` | 通过 |
| 11 | 默认状态 | `ACTIVE`、启用、非临时 | 通过 |
| 12 | 旧版敏感字段脱敏 | 身份证、手机号均按旧规则脱敏 | 历史结果，已被新契约替代 |
| 13 | 拼音码查询 | `ZYQ` 查询到“照夜清” | 通过 |
| 14 | 多条件组合查询 | 所有条件组合后只返回目标患者 | 通过 |
| 15 | 重复身份证 | 返回 `code=9999`，未重复写入 | 通过 |
| 16 | 修改患者 | 姓名和手机号更新成功 | 通过 |
| 17 | 修改后拼音 | `ZYQXG` 查询到“照夜清修改” | 通过 |
| 18 | 乐观锁 | 旧版本返回 `code=9999` | 通过 |
| 19 | 状态字段保护 | 修改 `activeFlag` 被拒绝 | 通过 |
| 20 | 不存在患者 | 返回 `code=9999` | 通过 |
| 21 | 同身份证并发 | 一笔成功、一笔失败 | 通过 |
| 22 | 并发最终数据 | 仅保留一条，编号为 `PA00000007` | 通过 |
| 23 | Nacos 注册 | 1 个实例，1 个健康实例 | 通过 |
| 24 | `pageSize=101` | 当前接口接受该值 | 已记录优化项 |

## 12. 当前验收结论

截至 2026-07-26：

- Maven 聚合构建成功；
- 28 个自动化测试全部通过；
- 自动化测试已覆盖列表原型字段、原始证件号/手机号、新增和修改必填证件号、
  原型 JSON 字段、两种出生日期格式、字段映射及乐观锁；
- 2026-07-24 的健康检查、数据库流程、并发建档和 Nacos 注册结果仅是变更前
  环境记录；
- 原型契约改造后的真实环境列表、新增、修改仍需按第 9 节重新执行。

因此，当前代码已达到自动化验收标准，但在完成第 9 节真实环境重测前，不声明
端到端手工验收完成。

## 13. 已知边界和后续优化

1. 公共 `PageQuery` 目前只限制 `pageSize>=1`，没有最大值；如需限制为 100，
   应由项目组统一修改公共分页契约。
2. 新增接口当前只返回“新增成功”，没有直接返回患者 ID 和患者编号。
3. 修改接口允许不传 `version`；传入时会校验旧版本。完整编辑回显仍依赖其他
   负责人维护的患者详情接口补齐原型所需字段。
4. 身份证重复目前有应用层校验；多实例生产环境仍建议增加数据库唯一约束。
5. 本次直接访问患者服务，未覆盖网关鉴权、角色权限和租户数据隔离。
6. 新增和修改会生成拼音码；当前未生成新的五笔码。
7. 患者选择列表、详情和删除不属于本次分工，其契约和集成验收由对应负责人确认。
8. 原型的“其他证件信息”包含 `useEnum`，而当前
   `adm_patient_identifier` 表没有对应字段；未获得数据库结构变更授权前，本次
   没有新增 `patientIdInfoList` 持久化。
9. 当前登录上下文没有提供 `tenantId` 和当前机构 ID，因此新增、修改仍要求请求
   显式传入 `organizationId`；改为从登录上下文取值需要统一认证模块支持。
10. 删除可能关联其他业务表且需要管理员权限，不能将删除接口当作常规测试清理
    手段；应恢复测试库快照，或由数据库负责人按准确患者 ID 清理。
11. 测试截图、终端输出和提交内容不得包含真实患者信息或任何凭据。
