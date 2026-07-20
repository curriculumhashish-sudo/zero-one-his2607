# 供应商管理模块 — 前后端交互接口文档

> **模块名称：** 供应商管理（Supplier）  
> **接口版本：** v1.0.0  
> **Base URL：** `/api/j3/supplier`  
> **文档日期：** 2026-07-19  
> **接口数量：** 3个（仅查询类）  
> **统一返回体：** `JsonVO<T>`（code + message + data）  
> **分页返回体：** `JsonVO<PageDTO<T>>`（pageIndex / pageSize / total / pages / rows）

---

## 统一返回结构

### 成功返回

```json
{ "code": 10000, "message": "操作成功", "data": { ... } }
```

### 失败返回（业务异常）

```json
{ "code": 9999, "message": "供应商ID不能为空" }
```

### 分页返回（JsonVO<PageDTO>）

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": {
    "pageIndex": 1,
    "pageSize": 10,
    "total": 5,
    "pages": 1,
    "rows": [ ... ]
  }
}
```

| 字段 | 类型 | 说明 |
|------|------|------|
| code | Integer | 10000=成功，9999=失败 |
| message | String | 提示消息 |
| data | Object | 无分页时为业务对象/列表；分页时为 PageDTO |
| data.pageIndex | Long | 当前页码 |
| data.pageSize | Long | 每页条数 |
| data.total | Long | 总记录数 |
| data.pages | Long | 总页数 |
| data.rows | Array | 当前页数据列表 |

---

## 接口一：获取供应商名称

### 基本信息

| 项目 | 内容 |
|------|------|
| **接口名称** | 获取供应商名称列表 |
| **业务用途** | 下拉选择组件专用，仅返回供应商ID和名称 |
| **适用场景** | 采购单选择供应商、入库单选择供应商、筛选条件中的供应商下拉、搜索框自动补全 |
| **请求方式** | `GET` |
| **请求URL** | `/api/j3/supplier/names` |

### 请求参数

| 参数名 | 类型 | 必填 | 默认值 | 参数说明 | 示例值 |
|--------|------|------|--------|----------|--------|
| statusEnum | String | 否 | 无 | 状态过滤，可选值 `ENABLED` / `DISABLED`；不传获取全部 | ENABLED |

### 成功响应（启用供应商列表）

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": [
    { "id": "1001", "name": "华润医药集团有限公司" },
    { "id": "1002", "name": "国药控股股份有限公司" },
    { "id": "1003", "name": "九州通医药集团股份有限公司" },
    { "id": "1004", "name": "上海医药集团股份有限公司" },
    { "id": "1005", "name": "广州白云山医药集团股份有限公司" }
  ]
}
```

### 失败响应示例

```json
{ "code": 9999, "message": "statusEnum 取值非法，仅支持 ENABLED 或 DISABLED" }
```

### 响应字段说明

| 字段 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| data[].id | String | 供应商主键ID | 1001 |
| data[].name | String | 供应商名称 | 华润医药集团有限公司 |

### 业务说明

- 仅返回 `id` 和 `name` 两个字段，数据量极小，适合下拉/自动补全
- 传入 `statusEnum=ENABLED` 时仅返回启用状态供应商，避免选择已停用的供应商
- 传入 `statusEnum=DISABLED` 时返回已停用供应商（自测用）
- 不传入 `statusEnum` 时返回全部供应商（含已停用）
- 传非法枚举值直接返回 `code=9999` + 错误提示
- 无分页，一次性返回所有匹配结果
- 前端使用提示：下拉组件打开时调用，可加本地缓存减轻服务端压力

---

## 接口二：获取供应商列表

### 基本信息

| 项目 | 内容 |
|------|------|
| **接口名称** | 获取供应商列表（条件+分页） |
| **业务用途** | 供应商管理列表页面，条件搜索+分页展示 |
| **适用场景** | 供应商列表页、选择供应商弹窗中的表格 |
| **请求方式** | `GET` |
| **请求URL** | `/api/j3/supplier/list` |

### 请求参数

| 参数名 | 类型 | 必填 | 默认值 | 校验规则 | 参数说明 | 示例值 |
|--------|------|------|--------|----------|----------|--------|
| pageIndex | Long | 否 | 1 | @Min(1) | 查询页码 | 1 |
| pageSize | Long | 否 | 10 | @Min(1) | 每页条数 | 10 |
| name | String | 否 | 无 | — | 供应商名称（模糊匹配） | 华润 |
| creditCode | String | 否 | 无 | — | 统一社会信用代码（精确匹配） | 91110000100000XXXX |
| statusEnum | String | 否 | 无 | @Pattern：仅 ENABLED / DISABLED | 状态枚举值 | ENABLED |
| typeEnum | String | 否 | 无 | — | 类型枚举值 | ENTERPRISE |

> **注意：** `statusEnum` 校验失败（如传非法值）会触发全局异常，返回 code=9995 "上传参数异常"。

### 成功响应

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": {
    "pageIndex": 1,
    "pageSize": 10,
    "total": 5,
    "pages": 1,
    "rows": [
      {
        "id": "1001", "busNo": "SUP20250001", "name": "华润医药集团有限公司",
        "typeEnum": "ENTERPRISE", "creditCode": "91110000100000AAAA",
        "legalRepresentative": "张三", "phone": "010-88888888",
        "statusEnum": "ENABLED", "displayOrder": 1, "updatedAt": "2025-07-18 10:00:00"
      },
      {
        "id": "1002", "busNo": "SUP20250002", "name": "国药控股股份有限公司",
        "typeEnum": "ENTERPRISE", "creditCode": "91110000100000BBBB",
        "legalRepresentative": "李四", "phone": "021-66666666",
        "statusEnum": "ENABLED", "displayOrder": 2, "updatedAt": "2025-07-17 15:30:00"
      }
    ]
  }
}
```

### 空列表响应（无匹配数据）

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": { "pageIndex": 1, "pageSize": 10, "total": 0, "pages": 0, "rows": [] }
}
```

### 响应字段说明（rows 列表项）

| 字段 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| id | String | 主键ID | 1001 |
| busNo | String | 业务编号 | SUP20250001 |
| name | String | 供应商名称 | 华润医药集团有限公司 |
| typeEnum | String | 类型枚举值 | ENTERPRISE |
| creditCode | String | 统一社会信用代码 | 91110000100000AAAA |
| legalRepresentative | String | 法定代表人 | 张三 |
| phone | String | 联系电话 | 010-88888888 |
| statusEnum | String | 状态（ENABLED/DISABLED） | ENABLED |
| displayOrder | Integer | 显示顺序 | 1 |
| updatedAt | String | 最后更新时间 | 2025-07-18 10:00:00 |

### 业务说明

- 列表仅返回精简关键字段，不含地址、邮箱、银行账户等完整信息，完整数据请调详情接口
- `name` 支持模糊匹配（模拟 LIKE %keyword%）
- `creditCode` 为精确匹配
- `statusEnum` 有正则校验 `@Pattern`，非法值返回 9995
- 所有筛选条件均为可选，多条件组合为 AND 逻辑
- 前端使用提示：列表行点击可跳转详情或弹出详情弹窗；无匹配数据时 rows 为空数组，total=0

---

## 接口三：获取供应商详情

### 基本信息

| 项目 | 内容 |
|------|------|
| **接口名称** | 获取供应商详情 |
| **业务用途** | 查看供应商完整资质信息，编辑表单回填 |
| **适用场景** | 供应商详情弹窗、编辑供应商表单回填、审核页面查看 |
| **请求方式** | `GET` |
| **请求URL** | `/api/j3/supplier/detail` |

### 请求参数

| 参数名 | 类型 | 必填 | 默认值 | 参数说明 | 示例值 |
|--------|------|------|--------|----------|--------|
| id | String | 是 | 无 | 供应商主键ID（有效值：1001~1005） | 1001 |

### 成功响应（id=1001）

```json
{
  "code": 10000,
  "message": "操作成功",
  "data": {
    "id": "1001",
    "busNo": "SUP20250001",
    "name": "华润医药集团有限公司",
    "typeEnum": "ENTERPRISE",
    "creditCode": "91110000100000AAAA",
    "legalRepresentative": "张三",
    "address": "北京市朝阳区建国路88号",
    "pyStr": "HRYY",
    "wbStr": "WXDG",
    "phone": "010-88888888",
    "email": "contact@huarun.com",
    "bankName": "中国工商银行北京分行",
    "bankAccount": "6222000000000000001",
    "orgId": "ORG001",
    "displayOrder": 1,
    "statusEnum": "ENABLED",
    "note": "央企背景，年合作额超千万，优质核心供应商"
  }
}
```

### 失败响应示例

**id 为空：**
```json
{ "code": 9999, "message": "供应商ID不能为空" }
```

**id 不存在（如 id=9999）：**
```json
{ "code": 9999, "message": "供应商不存在，请检查ID是否正确" }
```

### 响应字段说明

| 字段 | 类型 | 说明 | 示例值 |
|------|------|------|--------|
| id | String | 主键ID | 1001 |
| busNo | String | 业务编号 | SUP20250001 |
| name | String | 供应商名称 | 华润医药集团有限公司 |
| typeEnum | String | 类型枚举值 | ENTERPRISE |
| creditCode | String | 统一社会信用代码 | 91110000100000AAAA |
| legalRepresentative | String | 法定代表人 | 张三 |
| address | String | 地址 | 北京市朝阳区建国路88号 |
| pyStr | String | 拼音码 | HRYY |
| wbStr | String | 五笔码 | WXDG |
| phone | String | 联系电话 | 010-88888888 |
| email | String | 邮箱 | contact@huarun.com |
| bankName | String | 开户行 | 中国工商银行北京分行 |
| bankAccount | String | 银行账户 | 6222000000000000001 |
| orgId | String | 机构标识 | ORG001 |
| displayOrder | Integer | 显示顺序 | 1 |
| statusEnum | String | 状态（ENABLED/DISABLED） | ENABLED |
| note | String | 备注说明 | 央企背景，年合作额超千万，优质核心供应商 |

### 业务说明

- 返回供应商完整业务字段（17个字段）
- id 为必填参数，`@RequestParam(required = true)` 标记必传；不传时 Spring 直接返回 400
- 传入空字符串返回 `code=9999`，message="供应商ID不能为空"
- 传入不存在的 ID（1001~1005 之外的任意值）返回 `code=9999`，message="供应商不存在，请检查ID是否正确"
- 前端使用提示：编辑表单直接用返回数据回填各字段，无需额外转换

---

## Knife4j 自测用例速查

| 接口 | 测试输入 | 预期 code |
|------|----------|-----------|
| `/names` | 无参 | 10000（返回全部5条） |
| `/names` | `statusEnum=ENABLED` | 10000（返回4条启用） |
| `/names` | `statusEnum=DISABLED` | 10000（返回2条停用） |
| `/names` | `statusEnum=INVALID` | 9999 |
| `/list` | 无参 | 10000（全部5条，pageIndex=1） |
| `/list` | `name=华润` | 10000（1条命中） |
| `/list` | `statusEnum=ENABLED` | 10000（4条启用） |
| `/list` | `statusEnum=INVALID` | 9995（@Pattern 校验） |
| `/list` | `name=不存在的公司` | 10000（total=0，rows=[]） |
| `/detail` | `id=1001` | 10000（完整详情） |
| `/detail` | `id=1005` | 10000（停用供应商） |
| `/detail` | `id=9999` | 9999 |
| `/detail` | 不传 id | HTTP 400 |

---

## 附录A：文件清单

### his-domain（领域模型）

| 文件路径 | 类名 | 说明 |
|----------|------|------|
| `query/j3/supplier/SupplierQuery.java` | SupplierQuery | 列表查询条件（继承 PageQuery，statusEnum 加 @Pattern 校验） |
| `dto/j3/supplier/SupplierDTO.java` | SupplierDTO | 数据库完整映射DTO |
| `vo/j3/supplier/SupplierNameVO.java` | SupplierNameVO | 下拉名称VO（id + name） |
| `vo/j3/supplier/SupplierListVO.java` | SupplierListVO | 列表VO（精简10字段） |
| `vo/j3/supplier/SupplierDetailVO.java` | SupplierDetailVO | 详情VO（全量17业务字段） |

### his-apis（API接口定义）

| 文件路径 | 类名 | 说明 |
|----------|------|------|
| `j3/supplier/SupplierApi.java` | SupplierApi | 接口声明（@Tag + @Operation + @Parameter 完整） |

### his-j3-supplier（Controller实现）

| 文件路径 | 类名 | 说明 |
|----------|------|------|
| `j3/supplier/controller/SupplierController.java` | SupplierController | Mock桩 Controller：完整 Knife4j 注解 + 5条模拟数据 + 异常Mock分支 |

---

## 附录B：代码提交规范

### 分支策略

1. 基于主分支创建任务分支：`feature/j3-supplier-query-mock`
2. 本地 Knife4j 自测通过后发起 Merge Request

### 自测要求

1. 启动 `his-j3-supplier` 模块
2. 访问 `http://localhost:10205/doc.html`
3. 找到 `供应商管理` 分组，按上方「自测用例速查」表格逐项测试
4. 验证正常/异常返回 JSON 结构与本文档一致

### Commit Message 规范

```
feat(j3-supplier): Controller补全Knife4j注解+Mock数据+异常分支

- 类级 @Tag / 方法级 @Operation 注解完整
- 全部接口去除 return null，改用 JsonVO.success/fail 包装
- 构造5条真实模拟数据覆盖下拉/列表/详情
- 模拟异常：id为空、id不存在、非法statusEnum
- SupplierQuery 增加 @Pattern 校验注解
```

### 后续开发注意事项

1. Controller 中 Mock 逻辑仅供联调/自测，接入 Service 后全部替换为真实业务调用
2. `SupplierDTO` 字段名与 `cus_supplier` 表完全对齐，可用 MyBatis-Plus BaseMapper 直接映射
3. 列表查询：DTO → BeanUtil.copyProperties → SupplierListVO
4. 详情查询：DTO → BeanUtil.copyProperties → SupplierDetailVO
5. 下拉查询：直接返回 SupplierNameVO（字段少，可用 selectMaps）
6. 所有真实查询需过滤 `is_deleted = 0`
