# 前后端API接口文档

**简介**：科室管理前后端 API 接口与 Controller 桩测试文档（很帅大王负责部分）

**HOST**：`http://127.0.0.1:10201/basedata`

**联系人**：零壹星球

**Version**：1.0.0

**接口路径**：`/basedata/v3/api-docs/default`

## 1. 测试范围

本文档只测试本人负责的 3 个接口：

1. 获取科室列表（条件+分页）。
2. 修改科室。
3. 删除科室。

当前只完成领域模型、API 契约和 Controller 层。Controller 方法暂时为 `return null`，本轮只验证 OpenAPI 展示和参数绑定，不验证 Service、Mapper、数据库及真实业务规则。

## 2. 接口清单

| 编号 | 功能 | 方法 | 路径 | 请求模型 | 响应模型 |
|---|---|---|---|---|---|
| DEP-02 | 获取科室列表（条件+分页） | GET | `/basedata/departments/page` | `DepartmentQuery` | `PageDTO<DepartmentVO>` |
| DEP-05 | 修改科室 | PUT | `/basedata/departments/{id}` | `DepartmentUpdateDTO` | `String` |
| DEP-06 | 删除科室 | DELETE | `/basedata/departments/{id}` | 路径参数 `id` | `String` |

## 3. 测试环境

在 `his-java` 目录编译：

```powershell
mvn -s .\settings.xml -pl his-j3-department -am compile -DskipTests
```

使用 JDK 17 启动：

```text
com.zeroone.star.j3.department.DepartmentApplication
```

接口文档地址：

- Knife4j：`http://127.0.0.1:10201/basedata/doc.html`
- Swagger UI：`http://127.0.0.1:10201/basedata/swagger-ui.html`
- OpenAPI JSON：`http://127.0.0.1:10201/basedata/v3/api-docs/default`

PowerShell 测试地址：

```powershell
$baseUrl = "http://127.0.0.1:10201/basedata"
```

## 4. 接口测试

### 4.1 DEP-02 获取科室列表（条件+分页）

```powershell
$pageQuery = [System.Web.HttpUtility]::ParseQueryString('')
$pageQuery['pageIndex'] = '1'
$pageQuery['pageSize'] = '10'
$pageQuery['keyword'] = '内科'
$pageQuery['statusEnum'] = 'ACTIVE'
$pageQuery['typeEnum'] = 'DEPARTMENT'
$pageQuery['classEnum'] = 'CLINIC'

Invoke-WebRequest `
  -UseBasicParsing `
  -Method Get `
  -Uri "$baseUrl/departments/page?$($pageQuery.ToString())"
```

预期：

- HTTP 200。
- OpenAPI 展示分页参数和四个筛选条件。
- 响应模型为 `PageDTO<DepartmentVO>`。
- 当前响应体为空，不代表已经执行真实查询。

非法分页参数：

```powershell
Invoke-WebRequest `
  -UseBasicParsing `
  -Method Get `
  -Uri "$baseUrl/departments/page?pageIndex=0&pageSize=0"
```

预期：触发 `PageQuery` 最小值校验，返回参数错误响应。

### 4.2 DEP-05 修改科室

```powershell
$departmentId = '322663218269917184'
$updateBody = @{
  name             = '门诊心血管内科'
  typeEnum         = 'DEPARTMENT'
  classEnum        = 'CLINIC'
  parentId         = '322663105141149696'
  parentIdChanged  = $false
  ybNo             = 'HIS001'
  ybName           = '门诊心血管内科'
  medinsId         = 'H12345678'
  medinsAdmdvs     = '440100'
  medinsType       = 'HOSPITAL'
  medinsLv         = 'LEVEL_3_A'
  pyStr            = 'MZXXGNK'
  wbStr            = 'UYMT'
  defDoctorId      = '322663105141149700'
  caty             = '01'
  displayOrder     = 2
} | ConvertTo-Json

Invoke-WebRequest `
  -UseBasicParsing `
  -Method Put `
  -Uri "$baseUrl/departments/$departmentId" `
  -ContentType 'application/json; charset=utf-8' `
  -Body $updateBody
```

预期：HTTP 200，`id` 为路径参数，请求体模型为 `DepartmentUpdateDTO`。当前不会真正修改数据库。

### 4.3 DEP-06 删除科室

```powershell
$departmentId = '322663218269917184'

Invoke-WebRequest `
  -UseBasicParsing `
  -Method Delete `
  -Uri "$baseUrl/departments/$departmentId"
```

预期：HTTP 200，`id` 为路径参数。当前不会真正删除数据，也不会检查子科室或挂号、住院等业务引用。

## 5. OpenAPI 验收

打开 `http://127.0.0.1:10201/basedata/v3/api-docs/default`，只验收：

1. “获取科室列表（条件+分页）”接口的方法、路径、查询参数及响应模型。
2. “修改科室”接口的路径参数和 `DepartmentUpdateDTO` 请求体。
3. “删除科室”接口的 DELETE 方法和路径参数。

## 6. 测试记录

| 用例编号 | 测试点 | 预期结果 |
|---|---|---|
| TC-DEP-001 | Maven 编译 | `BUILD SUCCESS` |
| TC-DEP-002 | 分页接口 | 方法、路径、查询参数和响应模型正确 |
| TC-DEP-003 | 修改接口 | `id` 和 `DepartmentUpdateDTO` 正确 |
| TC-DEP-004 | 删除接口 | DELETE 方法和 `id` 正确 |

## 7. 后续业务测试

1. 分页边界、组合条件、排序和租户隔离。
2. 修改不存在科室、上级变更和循环引用。
3. 删除不存在科室、存在子科室及业务引用限制。
4. 修改和删除的审计字段及事务回滚。
