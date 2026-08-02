# 门诊护士站 - 前端接口对接修改建议

> 基于后端 `his-c2-opnurse` 当前实现 vs 前端 OpenAPI 契约（`1.json`），给前端开发人员的适配建议。
> 范围：患者队列、执行项目、耗材项目（三个查询接口）。

---

## 1. 接口路径对照

| # | 前端期望路径 | 后端实际路径 | 方法 |
|---|------------|------------|------|
| 1 | `/ons/patient-queue` | `/patient/query` | GET |
| 2 | `/ons/encounters/{encounterId}/execution-items` | `/patient/query-act/{encounterId}` | GET |
| 3 | `/ons/encounters/{encounterId}/consumable-items` | `/patient/query-consum/{encounterId}` | GET |

**前端需要改为请求后端实际路径。**

---

## 2. 患者队列 `/patient/query`

### 2.1 请求

```
GET /patient/query?keyword=&startDate=&endDate=&pageIndex=1&pageSize=10
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `keyword` | string | 否 | 姓名模糊匹配 或 排队号精确匹配 |
| `startDate` | string | 否 | 就诊开始日期（yyyy-MM-dd） |
| `endDate` | string | 否 | 就诊结束日期（yyyy-MM-dd） |
| `pageIndex` | int | 是 | 页码（从 1 开始） |
| `pageSize` | int | 是 | 每页条数（1-100） |

### 2.2 响应

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

| 后端字段 | 类型 | 说明 |
|---------|------|------|
| `encounterId` | string | 就诊 ID（联动执行/耗材接口） |
| `name` | string | 患者姓名 |
| `gender` | string | 性别（已翻译为"男"/"女"） |
| `age` | int | 年龄 |
| `visitTime` | string | 就诊时间 |
| `queueNumber` | string | 排队号 |
| `paymentType` | string | 付费类型 |

### 2.3 前端适配

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query` |
| 2 | 参数名映射：前端 `patient_name` → 后端 `keyword` |
| 3 | 日期参数：前端 `visit_date` → 后端 `startDate`，前端 `visit_date` → 后端 `endDate` |
| 4 | 删除 `status`/`department_id`/`doctor_id` 参数（后端暂不支持） |
| 5 | 字段名映射：`encounterId`→`encounter_id`、`name`→`patient_name`、`gender`→`patient_gender`、`age`→`patient_age`、`visitTime`→`register_time`、`queueNumber`→`visit_order`、`paymentType`→`status_name` |
| 6 | 缺失字段兜底：`id:"-"`、`patient_id:"-"`、`patient_card_no:"-"`、`department_name:"-"`、`doctor_name:"-"`、`visit_no:"-"`、`status:1`、`department_id:""`、`doctor_id:""` |
| 7 | 成功码判断：`code === 0`（不是 10000） |

---

## 3. 执行项目 `/patient/query-act/{encounterId}`

### 3.1 请求

```
GET /patient/query-act/{encounterId}
```

**无 query 参数。** 后端返回全量医嘱，前端按 `isAct` 自行分组。

### 3.2 响应

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

| 层级 | 字段 | 类型 | 说明 |
|------|------|------|------|
| 顶层 | `name` | string | 患者姓名 |
| 顶层 | `gender` | string | 患者性别 |
| 顶层 | `age` | int | 患者年龄 |
| items[] | `status` | string | 状态文案（"待执行"/"已执行"） |
| items[] | `orderContent` | string | 医嘱内容 |
| items[] | `amount` | string | 金额（"已结算；6.00元" 或 "未结算"） |
| items[] | `isAct` | int | 0=未执行（☐），1=已执行（☑） |

### 3.3 前端适配

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query-act/{encounterId}` |
| 2 | **删除所有 query 参数**（不要传 act/status/orderContent） |
| 3 | 患者信息从 `data.name`/`data.gender`/`data.age` 取（顶层，不在 items 内） |
| 4 | 列表从 `data.items[]` 取 |
| 5 | ☑ 勾选框：`items[i].isAct === 1` 打钩 |
| 6 | 前端按 `isAct` 分组（已执行/未执行） |
| 7 | `status` (enum)：`isAct===1 ? "COMPLETED" : "PENDING"` |
| 8 | 缺失字段兜底：`itemId: items[i].orderContent`、`requestId:""`、`requestType:"SERVICE"`、`requestTypeName:"处置"`、`specification:"-"`、`plannedCount:1`、`executedCount: items[i].isAct`、`version:0`、`printTypes:[]` |
| 9 | 成功码判断：`code === 0` |

---

## 4. 耗材项目 `/patient/query-consum/{encounterId}`

### 4.1 请求

```
GET /patient/query-consum/{encounterId}?consumableRoom=门诊西药房
```

| 参数 | 类型 | 必填 | 说明 |
|------|------|------|------|
| `encounterId` | path | 是 | 就诊 ID |
| `consumableRoom` | query | 否 | 耗材房筛选，空选省略 |

### 4.2 响应

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

| 字段 | 类型 | 说明 |
|------|------|------|
| `consumableName` | string | 耗材名称 |
| `modelSpec` | string | 型号规格（含总容量） |
| `quantity` | string | 数量 + 单位（"11 ×1"） |
| `status` | string | 状态（"待发放"/"已发放"） |
| `consumableRoom` | string | 耗材房 |
| `isEmpty` | int | 是否空（0=非空，1=空） |
| `isAct` | int | ☑ 勾选框状态（0=未勾选，1=已勾选） |

**返回纯数组（无顶层患者信息），与前端契约一致。**

### 4.3 前端适配

| # | 适配项 |
|---|--------|
| 1 | 路径改为 `/patient/query-consum/{encounterId}` |
| 2 | 新增 query 参数：耗材房下拉选中值 → `?consumableRoom=门诊西药房`，空选省略 |
| 3 | 字段名映射：`consumableName`→`materialName`、`modelSpec`→`specification`、`consumableRoom`→`roomName` |
| 4 | ☑ 勾选框：用 `isAct`（前端 adapter 新增此字段） |
| 5 | `status` (enum)：`status==="待发放" ? "PENDING" : "DISPENSED"` |
| 6 | `unitName`：从 `quantity` 字符串解析，或前端写死默认单位 |
| 7 | 缺失字段兜底：`dispenseItemId: consumableName + index`、`materialId:""` |
| 8 | 成功码判断：`code === 0` |

---

## 5. 数据联动

```
步骤1：GET /patient/query?keyword=&startDate=&endDate=&pageIndex=1&pageSize=10
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
| 成功码 | `10000` | `0` | `code === 0` |
| 错误码 | 自定义 | `9995`/`9994`/`9999` | `code !== 0` 即失败 |
| 字段命名 | snake_case | camelCase | adapter 层转换 |
| 鉴权 | Bearer JWT | Bearer JWT | ✅ 一致 |

---

## 7. 前端适配清单汇总

| # | 接口 | 适配项 | 优先级 |
|---|------|--------|--------|
| 1 | 患者队列 | 路径改为 `/patient/query` | 高 |
| 2 | 患者队列 | 参数名映射（keyword/startDate/endDate） | 高 |
| 3 | 患者队列 | 字段名 adapter 转换 + 缺失字段兜底 | 高 |
| 4 | 执行项目 | 路径改为 `/patient/query-act/{encounterId}` | 高 |
| 5 | 执行项目 | **删除所有 query 参数** | 高 |
| 6 | 执行项目 | Wrapper 结构取 data.items[] | 高 |
| 7 | 耗材项目 | 路径改为 `/patient/query-consum/{encounterId}` | 高 |
| 8 | 耗材项目 | 新增 `consumableRoom` 查询参数 | 中 |
| 9 | 耗材项目 | 字段名 adapter 转换 | 中 |
| 10 | 全部 | 成功码判断 `code === 0` | 高 |
