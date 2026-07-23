# 患者管理业务接口测试文档

## 1. 测试对象

| 项目 | 内容 |
|---|---|
| 模块 | `his-j3-patient` |
| 分支 | `j3-patient-zhaoyeqing` |
| 负责人 | 照夜清 |
| 本次接口 | 患者管理列表、新增患者、修改患者 |
| 服务名 | `his-j3-patient` |
| 默认端口 | `10202` |
| 直接访问根路径 | `http://localhost:10202/j3-basedata` |
| Java | JDK 17 |

本文件同时给出自动化测试、Nacos 环境启动方法和可逐条执行的
PowerShell 手工验收步骤。所有新增、并发和修改测试只能在开发或测试数据库执行。

## 2. 实现前后验收结论

### 2.1 实现前基线

在相同 HTTP 路由下，业务实现前的桩接口表现为：

1. 患者管理列表返回成功码，但 `data` 为 `null`。
2. 姓名、性别、机构均为空的新增请求仍返回 `code=10000`。
3. 修改一个不存在的患者仍返回 `code=10000`。

因此实现前不满足原型中的真实查询、必填校验、持久化和修改限制。

### 2.2 实现后验收

2026-07-23 使用项目自带 `zo_his.sql` 和 `zo_his_test_data.sql` 创建隔离
MySQL 8.0 测试库，执行了 32 项真实 HTTP + 数据库断言：

- 32 项通过，0 项失败；
- Maven 自动化测试 17 项通过，0 项失败；
- Reactor 打包结果为 `BUILD SUCCESS`。

真实接口验收覆盖分页、排序、筛选、软删除、脱敏、校验、正式建档默认值、
患者编号、身份证重复、同证件并发建档、修改、乐观锁和终态限制。

## 3. 本次业务口径

### 3.1 患者管理列表

1. 页码从 1 开始。
2. 默认按 `created_at DESC, id DESC` 排序。
3. 不返回软删除数据。
4. 姓名、患者编号、身份证号、手机号支持各自条件查询。
5. 综合关键字匹配姓名、患者编号、身份证号、拼音码和已有五笔码。
6. 性别、患者状态、启用标记、临时建档标记和机构为精确匹配。
7. 返回列表时身份证号和手机号必须脱敏。

### 3.2 新增患者

1. 姓名、性别和建档机构必填。
2. 性别仅支持 `MALE`、`FEMALE`、`UNKNOWN`。
3. 出生日期不能晚于当天。
4. 身份证号为 15 位数字或 18 位（最后一位允许 `X`）。
5. 身份证号会去除首尾空格并转换为大写。
6. 身份证号与现存或软删除患者重复时禁止新增。
7. 正式建档默认：
   `statusEnum=ACTIVE`、`activeFlag=true`、`tempFlag=false`。
8. 患者编号按租户序列生成，格式为 `PA` 加 8 位数字。
9. 自动生成姓名拼音首字母码。

### 3.3 修改患者

1. 患者必须存在且未软删除。
2. 只有 `DRAFT`、`ACTIVE` 状态允许修改基本资料。
3. 必须携带当前 `version`，版本不一致时拒绝修改。
4. 身份证重复检查排除患者自己。
5. 普通资料修改不允许改变机构、状态、启用标记或临时建档标记。
6. 修改姓名后重新计算拼音码。

## 4. 先运行自动化测试

打开 PowerShell：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java

mvn -s .\settings.xml `
  -pl his-j3-patient `
  -am `
  package `
  "-DskipTests=false"
```

注意：PowerShell 中 `-DskipTests=false` 建议加双引号。你之前遇到的
`Unknown lifecycle phase ".run.profiles=test"` 也是 PowerShell 拆分 `-D` 参数造成的。

预期结果：

```text
Tests run: 17, Failures: 0, Errors: 0, Skipped: 0
BUILD SUCCESS
```

测试类及覆盖范围：

| 测试类 | 数量 | 覆盖范围 |
|---|---:|---|
| `PatientControllerTest` | 2 | 列表、新增、修改正确委托 Service |
| `PatientDtoValidationTest` | 4 | DTO 必填、日期和版本校验 |
| `PatientMapperContractTest` | 1 | SQL 软删除、筛选、排序和字段契约 |
| `PatientServiceImplTest` | 8 | 新增、列表、重复、默认值、版本和状态限制 |
| `PatientNameCodeUtilsTest` | 2 | 中文与混合姓名拼音首字母 |

## 5. 准备 Nacos、MySQL 环境

### 5.1 启动虚拟机

当前开发虚拟机文件为：

```text
D:\VMs\AnolisOS-23.3-his-dev\AnolisOS-23.3-his-dev.vmx
```

在 VMware 中启动该虚拟机。进入虚拟机后执行：

```bash
ip -4 addr
sudo docker ps -a
sudo ss -lntp | grep -E ':8080|:8848|:9848|:3306'
```

记录虚拟机实际 IP。以下示例使用 `192.168.28.132`；如果 IP 已变化，后续命令
全部替换为新 IP。

在 Windows PowerShell 检查连通性：

```powershell
$vmIp = "192.168.28.132"

Test-NetConnection $vmIp -Port 8080
Test-NetConnection $vmIp -Port 8848
Test-NetConnection $vmIp -Port 9848
Test-NetConnection $vmIp -Port 3306
```

四个端口的 `TcpTestSucceeded` 都应为 `True`。

### 5.2 检查 Nacos

浏览器打开：

```text
http://192.168.28.132:8080
```

登录后完成以下检查：

1. 进入命名空间 `his-dev`。
2. 配置列表应包含：
   `system.yaml`、`data-source.yaml`、`third-services.yaml`、
   `gateway.yaml`、`key-config.yaml`。
3. `data-source.yaml` 的 JDBC 地址必须指向当前能访问的测试 MySQL。
4. 不要把 Nacos 或数据库密码复制到代码、测试文档或 Git 提交中。
5. `system.yaml` 如果配置患者服务变量，应使用：

```yaml
sp:
  j3-patient: 10202
sn:
  j3-patient: his-j3-patient
```

即使 `system.yaml` 未配置这两个键，本模块也会使用上面的默认值。

## 6. 通过 Nacos 启动患者服务

先完成第 4 节打包，然后执行：

```powershell
cd D:\AIWorkspace\projects\zero-one-his2607\his-java\his-j3-patient

$env:NACOS_USERNAME = "<你的 Nacos 用户名>"
$env:NACOS_PASSWORD = "<你的 Nacos 密码>"

java -jar .\target\his-j3-patient-1.0.0-SNAPSHOT.jar `
  "--spring.profiles.active=dev" `
  "--spring.cloud.nacos.server-addr=192.168.28.132:8848"
```

不要将账号密码写入仓库。保持该窗口运行。

启动成功的判断标准：

1. 日志出现 `Started PatientApplication`。
2. 未出现 Nacos 配置导入失败或 MySQL 连接失败。
3. `http://localhost:10202/j3-basedata/actuator/health` 可访问。
4. Nacos 的“服务管理 → 服务列表”中能查询到 `his-j3-patient`，实例健康。

接口文档：

- Swagger UI：
  `http://localhost:10202/j3-basedata/swagger-ui.html`
- OpenAPI JSON：
  `http://localhost:10202/j3-basedata/v3/api-docs`

若通过网关访问，路径前还需要增加网关路由前缀 `/j3-patient`。本次手工验收建议
先使用上述直接地址，避免把网关鉴权问题混入患者业务测试。

## 7. PowerShell 手工验收

另开一个 PowerShell 窗口，先定义公共变量：

```powershell
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()

$baseUrl = "http://localhost:10202/j3-basedata"
$orgId = "322663105141149696"
$testIdCard = "11010119900101123X"
$testPhone = "13800001234"
```

`$orgId` 必须替换为测试数据库中真实存在、状态为 `ACTIVE` 且启用的机构 ID。
`$testIdCard` 如果已经存在，也要更换为一个未使用、格式合规的测试值，并在下列
所有步骤保持一致。

### 7.1 健康检查

```powershell
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/actuator/health"
```

预期：`status` 为 `UP`。

### 7.2 初始列表、分页、排序和脱敏

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

逐项检查：

1. 两个响应的 `code` 都是 `10000`。
2. `pageIndex`、`pageSize`、`total`、`pages` 和 `rows` 均有真实值。
3. 第 1 页与第 2 页患者 ID 不重复。
4. 记录按创建时间从新到旧排列。
5. 18 位身份证显示为“前 6 位 + 星号 + 后 4 位”。
6. 11 位手机号显示为“前 3 位 + 星号 + 后 4 位”。

非法分页：

```powershell
$badPage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=0&pageSize=10"

$badPage
```

预期：`code` 不等于 `10000`。

### 7.3 条件查询

```powershell
# 姓名模糊查询
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&name=小"

# 身份证号模糊查询
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=220322"

# 手机号模糊查询
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&phone=139448"

# 精确组合筛选
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&genderEnum=MALE&statusEnum=ACTIVE&activeFlag=true&tempFlag=false&organizationId=$orgId"
```

预期：均返回 `code=10000`，`rows` 中的数据满足所传条件。

### 7.4 必填项和格式校验

```powershell
$invalidBody = @{
  name = ""
  genderEnum = ""
  organizationId = ""
} | ConvertTo-Json

$invalidResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $invalidBody

$invalidResult
```

预期：`code` 不等于 `10000`。

再测未来出生日期：

```powershell
$futureBody = @{
  name = "未来日期"
  genderEnum = "UNKNOWN"
  birthDate = "2999-01-01"
  organizationId = $orgId
} | ConvertTo-Json

Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $futureBody
```

预期：`code` 不等于 `10000`。

### 7.5 正式新增患者“照夜清”

```powershell
$createBodyObject = @{
  name = "  照夜清  "
  genderEnum = "MALE"
  birthDate = "1990-01-01"
  idCard = " $testIdCard "
  phone = " $testPhone "
  address = "北京市测试路1号"
  organizationId = $orgId
}

$createResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body ($createBodyObject | ConvertTo-Json)

$createResult
```

预期：`code=10000`、`data=新增成功`。

用未脱敏身份证作为查询条件取回新患者：

```powershell
$createdPage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=$testIdCard"

$createdPatient = $createdPage.data.rows | Select-Object -First 1
$createdPatient
$patientId = [string]$createdPatient.id
```

逐项检查：

1. `$createdPage.data.total` 为 `1`。
2. 姓名已经去除空格，等于 `照夜清`。
3. `busNo` 符合 `^PA\d{8}$`。
4. `statusEnum=ACTIVE`。
5. `activeFlag=True`。
6. `tempFlag=False`。
7. 身份证和手机号已脱敏。

拼音码查询：

```powershell
Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&keyword=ZYQ"
```

预期：能查到刚新增的“照夜清”。

### 7.6 精确重复建档

复用相同身份证再次新增：

```powershell
$duplicateBody = @{
  name = "重复患者"
  genderEnum = "MALE"
  idCard = $testIdCard
  organizationId = $orgId
} | ConvertTo-Json

$duplicateResult = Invoke-RestMethod `
  -Method Post `
  -Uri "$baseUrl/patients" `
  -ContentType "application/json; charset=utf-8" `
  -Body $duplicateBody

$duplicateResult
```

预期：`code` 不等于 `10000`，数据库中仍只有一个该身份证患者。

### 7.7 修改患者

新患者初始版本为 `0`，执行第一次修改：

```powershell
$updateBodyObject = @{
  name = "照夜清改"
  genderEnum = "MALE"
  birthDate = "1990-01-01"
  idCard = $testIdCard
  phone = "13900005678"
  address = "北京市测试路2号"
  organizationId = $orgId
  statusEnum = "ACTIVE"
  activeFlag = $true
  tempFlag = $false
  version = 0
}

$updateResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body ($updateBodyObject | ConvertTo-Json)

$updateResult
```

预期：`code=10000`、`data=修改成功`。

再次查询：

```powershell
$updatedPage = Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&idCard=$testIdCard"

$updatedPage.data.rows | Select-Object -First 1

Invoke-RestMethod `
  -Method Get `
  -Uri "$baseUrl/patients/page?pageIndex=1&pageSize=10&keyword=ZYQG"
```

预期：

1. 姓名为 `照夜清改`。
2. 手机号脱敏结果对应 `13900005678`。
3. 使用新拼音码 `ZYQG` 能查到患者。

### 7.8 乐观锁冲突

不要修改 `$updateBodyObject.version`，仍用旧版本 `0` 重复提交：

```powershell
$staleResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body ($updateBodyObject | ConvertTo-Json)

$staleResult
```

预期：`code` 不等于 `10000`，提示刷新后重试。

### 7.9 普通修改接口不能改变状态字段

第一次成功修改后数据库版本为 `1`：

```powershell
$stateBody = $updateBodyObject.Clone()
$stateBody.version = 1
$stateBody.activeFlag = $false

$stateResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/$patientId" `
  -ContentType "application/json; charset=utf-8" `
  -Body ($stateBody | ConvertTo-Json)

$stateResult
```

预期：`code` 不等于 `10000`，启用标记没有改变。

### 7.10 修改不存在患者

```powershell
$missingBody = $updateBodyObject.Clone()
$missingBody.version = 0

$missingResult = Invoke-RestMethod `
  -Method Put `
  -Uri "$baseUrl/patients/999999999999999999" `
  -ContentType "application/json; charset=utf-8" `
  -Body ($missingBody | ConvertTo-Json)

$missingResult
```

预期：`code` 不等于 `10000`。

### 7.11 可选：同身份证并发建档

此步骤会再产生一位测试患者，只能在可清理的独立测试库执行。

```powershell
$raceIdCard = "11010119900202234X"
$raceBody = @{
  name = "并发患者"
  genderEnum = "FEMALE"
  idCard = $raceIdCard
  phone = "13700009999"
  organizationId = $orgId
} | ConvertTo-Json

$jobs = 1..2 | ForEach-Object {
  Start-Job -ScriptBlock {
    param($uri, $json)
    Invoke-RestMethod `
      -Method Post `
      -Uri $uri `
      -ContentType "application/json; charset=utf-8" `
      -Body $json
  } -ArgumentList "$baseUrl/patients", $raceBody
}

$results = $jobs | Wait-Job | Receive-Job
$jobs | Remove-Job
$results
```

预期：两个响应中恰好一个 `code=10000`，另一个为失败码；随后按
`$raceIdCard` 查询只能得到一条患者。

## 8. Nacos 注册检查

完成接口测试时返回 Nacos：

1. 打开“服务管理 → 服务列表”。
2. 搜索 `his-j3-patient`。
3. 实例数应至少为 1，健康实例数应至少为 1。
4. 打开实例详情，IP 和端口应对应当前启动服务的主机与 `10202`。
5. 停止患者服务后，等待心跳超时，实例应变为不健康或被移除。

这一步验证的是服务发现；患者列表、新增和修改的业务正确性仍以第 7 节的 HTTP
结果和数据库结果为准。

## 9. 原型验收对照表

| 原型/业务点 | 手工步骤 | 预期 |
|---|---|---|
| 列表分页 | 7.2 | 真实分页数据 |
| 条件查询 | 7.3 | 返回数据全部满足条件 |
| 必填及格式校验 | 7.4 | 非法请求失败 |
| 正式建档 | 7.5 | 默认 ACTIVE、启用、非临时 |
| 患者编号 | 7.5 | `PA` + 8 位数字 |
| 身份证标准化 | 7.5 | 去空格、大写、查询可命中 |
| 精确重复患者 | 7.6 | 阻止重复新增 |
| 修改资料 | 7.7 | 数据和拼音码更新 |
| 版本冲突 | 7.8 | 旧版本请求失败 |
| 状态字段保护 | 7.9 | 普通资料接口不能更改 |
| 不存在患者 | 7.10 | 修改失败 |
| 并发重复 | 7.11 | 仅一笔成功 |
| Nacos 服务发现 | 第 8 节 | 健康实例可见 |

## 10. 当前边界和剩余风险

1. 本次只实现分工表中的“患者管理列表、增加患者、修改患者”。患者选择列表、
   详情和删除仍属于其他同学负责的接口。
2. 原型中的“新增权限”“建档来源后续业务”“疑似患者确认”“专用审计日志”
   涉及鉴权、挂号/住院或新增契约，不在这三个接口的现有输入输出范围内。
3. 列表可以查询数据库已有五笔码；新增和修改会生成拼音码，但当前项目未提供
   可合法复用的五笔编码组件，因此新患者的五笔码不会自动生成。
4. 同一服务实例和同一数据库上的并发重复建档已通过；数据库表当前没有身份证
   唯一索引，多实例部署下仍建议由数据库唯一约束提供最后一道保证。
5. 不要用当前尚未实现业务的 DELETE 接口清理测试数据。测试结束后应恢复测试库
   快照，或由数据库负责人按准确患者 ID 清理；不要在生产库执行本文件的写操作。

## 11. 测试记录模板

| 用例 | 执行时间 | 实际结果 | 通过/失败 | 备注 |
|---|---|---|---|---|
| 自动化测试 |  |  |  |  |
| Nacos/MySQL 连通 |  |  |  |  |
| 列表分页/排序/脱敏 |  |  |  |  |
| 条件查询 |  |  |  |  |
| 非法新增 |  |  |  |  |
| 正式新增 |  |  |  |  |
| 重复新增 |  |  |  |  |
| 修改资料 |  |  |  |  |
| 乐观锁 |  |  |  |  |
| 状态字段保护 |  |  |  |  |
| 不存在患者 |  |  |  |  |
| 并发重复（可选） |  |  |  |  |
| Nacos 服务注册 |  |  |  |  |
