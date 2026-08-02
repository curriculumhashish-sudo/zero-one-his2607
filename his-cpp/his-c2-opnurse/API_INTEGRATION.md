# 门诊护士站 - 接口对接修改建议

> 基于后端 `his-c2-opnurse` 当前实现 vs 前端 OpenAPI 契约（`1.json`），给前端开发人员的适配建议。
> 范围：患者队列、执行项目、耗材项目（三个查询接口）。

---

## 1. 接口路径对照

| # | 前端期望路径 | 后端实际路径 | 方法 |
|---|------------|------------|------|
| 1 | `/ons/patient-queue` | `/patient/query` | GET |
| 2 | `/ons/encounters/{encounterId}/execution-items` | `/patient/query-act/{encounterId}` | GET |
| 3 | `/ons/encounters/{encounterId}/consumable-items` | `/patient/query-consum/{encounterId}` | GET |

**建议：前端改为请求后端实际路径。**

---

## 2. 患者队列 `/patient/query`

### 2.1 查询条件

| 前端参数 | 后端参数 | 适配 |
|---------|---------|------|
| `patient_name` | `name` | 字段名映射 |
| `visit_date` | `date` | 字段名映射 |
| `status` | ❌ | 暂不支持，前端可不传 |
| `department_id` | ❌ | 暂不支持 |
| `doctor_id` | ❌ | 暂不支持 |
| `pageIndex` | `pageIndex` | ✅ |
| `pageSize` | `pageSize` | ✅ |

**额外能力：** `name` 参数同时支持姓名模糊匹配和排队号精确匹配（输入"002"可直接搜排队号）。

### 2.2 响应字段

| 前端字段 | 后端字段 | 适配 |
|------|------|------|
| `encounter_id` | `encounterId` | 字段名映射 |
| `patient_name` | `name` | 字段名映射 |
| `patient_gender` | `gender` | 字段名映射 |
| `patient_age` | `age` | 字段名映射 |
| `visit_order` | `queueNumber` | 字段名映射 |
| `register_time` | `visitTime` | 字段名映射 |
| `status_name` | `paymentType` | 字段名映射（语义略有不同） |
| `id` | ❌ | 前端兜底 `"-"` |
| `patient_id` | ❌ | 前端兜底 `"-"` |
| `patient_card_no` | ❌ | 前端兜底 `"-"` |
| `department_name` | ❌ | 前端兜底 `"-"` |
| `doctor_name` | ❌ | 前端兜底 `"-"` |
| `visit_no` | ❌ | 前端兜底 `"-"` |
| `status` (int) | ❌ | 前端写死为 1（就诊中） |
| `department_id` | ❌ | 前端兜底 `""` |
| `doctor_id` | ❌ | 前端兜底 `""` |

### 2.3 后端实际返回示例

```json
{
  "code": 0,
  "message": "success",
  "data": {
    "pageIndex": 1,
    "pageSize": 10,
    "total": 1,
    "pages": 1,
    "rows": [
      {
        "encounterId": "EN2026-08-290001",
        "name": "周女士",
        "gender": "女性",
        "age": 26,
        "visitTime": "2026-08-29 15:01:28",
        "queueNumber": "002",
        "paymentType": "自费"
      }
    ]
  }
}
```

### 2.4 前端适配清单

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query` |
| 2 | 参数名：`patient_name` → `name`，`visit_date` → `date` |
| 3 | 删除 `status`/`department_id`/`doctor_id` 参数 |
| 4 | adapter 层做字段名映射（见上表） |
| 5 | 缺失字段给默认值（`"-"` 或 `""`） |
| 6 | 成功判断用 `code === 0`（不是 10000） |

---

## 3. 执行项目 `/patient/query-act/{encounterId}`

### 3.1 查询条件

| 前端参数 | 后端参数 | 适配 |
|---------|---------|------|
| `encounterId` (path) | `encounterId` (path) | ✅ |
| 其他 query 参数 | **无** | ✅ |

**重大变更：** 后端不再接受 `act`/`status`/`orderContent` 作为查询参数，改为全量返回。前端按 `isAct` 自行分组。

### 3.2 响应结构

**后端使用 Wrapper 结构（与前端纯数组不同）：**

```json
{
  "code": 0,
  "message": "success",
  "data": {
    "name": "周女士",
    "gender": "女性",
    "age": 26,
    "items": [
      {
        "status": "待执行",
        "orderContent": "静脉注射",
        "amount": "已结算；6.00元",
        "isAct": 0
      }
    ]
  }
}
```

### 3.3 响应字段

| 前端字段 | 后端字段 | 适配 |
|------|------|------|
| `orderContent` | `items[i].orderContent` | ✅ |
| `statusName` | `items[i].status` | ✅（已是中文） |
| `status` (enum) | `items[i].isAct` | 映射：`isAct==1 ? "COMPLETED" : "PENDING"` |
| 勾选框 ☑ | `items[i].isAct` | `isAct==1` 打钩 |
| `itemId` | ❌ | 前端兜底 `items[i].orderContent` |
| `requestId` | ❌ | 前端兜底 `""` |
| `requestType` | ❌ | 前端兜底 `"SERVICE"` |
| `requestTypeName` | ❌ | 前端兜底 `"处置"` |
| `specification` | ❌ | 前端兜底 `"-"` |
| `plannedCount` | ❌ | 前端兜底 `1` |
| `executedCount` | ❌ | 前端用 `isAct` |
| `version` | ❌ | 前端兜底 `0` |
| `printTypes` | ❌ | 前端兜底 `[]` |

### 3.4 前端适配清单

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query-act/{encounterId}` |
| 2 | **删除所有 query 参数**（act/status/orderContent） |
| 3 | 患者信息从 `data.name`/`data.gender`/`data.age` 取（顶层） |
| 4 | 列表数据从 `data.items[]` 取 |
| 5 | ☑ 勾选框用 `items[i].isAct` 控制 |
| 6 | 前端按 `isAct` 分组显示（已执行/未执行） |
| 7 | adapter 层补齐缺失字段（见上表） |
| 8 | 成功判断用 `code === 0` |

---

## 4. 耗材项目 `/patient/query-consum/{encounterId}`

### 4.1 查询条件

| 前端参数 | 后端参数 | 适配 |
|---------|---------|------|
| `encounterId` (path) | `encounterId` (path) | ✅ |
| — | `consumableRoom` (query, 可选) | 前端新增：耗材房下拉值传入 |

### 4.2 响应结构

**后端返回纯数组（与前端一致 ✅）：**

```json
{
  "code": 0,
  "message": "success",
  "data": [
    {
      "consumableName": "一次性使用精密输液器（带针）",
      "modelSpec": "ZJ-B-BV23(400支/件)",
      "quantity": "11 ×1",
      "status": "待发放",
      "consumableRoom": "门诊西药房",
      "isEmpty": 0,
      "isAct": 0
    }
  ]
}
```

### 4.3 响应字段

| 前端字段 | 后端字段 | 适配 |
|------|------|------|
| `materialName` | `consumableName` | 字段名映射 |
| `specification` | `modelSpec` | 字段名映射 |
| `quantity` | `quantity` | ⚠️ 后端是字符串 "11 ×1"，前端如需数字需解析 |
| `unitName` | 含在 `quantity` 中 | 从 `quantity` 解析或前端写死 |
| `roomName` | `consumableRoom` | 字段名映射 |
| `statusName` | `status` | ✅（已是中文） |
| `status` (enum) | `status` | 映射：`"待发放" ? "PENDING" : "DISPENSED"` |
| 勾选框 ☑ | `isAct` | `isAct==1` 打钩（前端新增） |
| `dispenseItemId` | ❌ | 前端兜底 `consumableName + index` |
| `materialId` | ❌ | 前端兜底 `""` |

### 4.4 前端适配清单

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query-consum/{encounterId}` |
| 2 | 新增 query 参数：`consumableRoom`（耗材房下拉选中值，空选省略） |
| 3 | adapter 层做字段名映射（见上表） |
| 4 | ☑ 勾选框用 `isAct` 控制（前端新增此字段） |
| 5 | `quantity` 字符串解析或直接展示 |
| 6 | 缺失字段给默认值 |
| 7 | 成功判断用 `code === 0` |

---

## 5. 数据联动

```
步骤1：GET /patient/query?name=&date=
       → 取 rows[i].encounterId

步骤2：用户点击卡片
       → 并行调用：
         GET /patient/query-act/{encounterId}
         GET /patient/query-consum/{encounterId}?consumableRoom=...
```

---

## 6. 通用差异

| 项目 | 前端期望 | 后端实际 | 适配 |
|------|---------|---------|------|
| 成功码 | `10000` | `0` | 判断 `code === 0` |
| 错误码 | 自定义 | `9995`/`9994`/`9999` | 判断 `code !== 0` |
| 字段命名 | snake_case | camelCase | adapter 层转换 |
| 鉴权 | Bearer JWT | Bearer JWT | ✅ |

---

## 7. 待办

| # | 项目 | 负责方 | 优先级 |
|---|------|--------|--------|
| 1 | 患者队列补齐 `patient_id`/`department_name` 等字段 | 后端 | 高 |
| 2 | 执行项目补齐 `itemId`/`requestId` 等字段 | 后端 | 中 |
| 3 | 耗材 `quantity` 改为数字 + `unitName` 独立字段 | 后端 | 低 |
| 4 | 前端 adapter 层做字段映射 | 前端 | 高 |
| 5 | 前端路径改为后端实际路径 | 前端 | 高 |
| 6 | 前端去掉执行项目 query 参数 | 前端 | 高 |
