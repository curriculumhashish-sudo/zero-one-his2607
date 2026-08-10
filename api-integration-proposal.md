# 基础数据模块 API 对接方案

> **版本**: v3.0  
> **日期**: 2026-08-02  
> **范围**: 6 个基础数据模块（药品、频次、诊断、诊疗、器材、样本）  
> **核心策略**: RESTful + 字段映射层 + HTTP 优先 Mock 回退

---

## 一、背景与目标

### 1.1 现状问题

当前 6 个模块的 API 层存在**风格分裂**：

| 模块 | 当前风格 | 字段映射层 | 状态 |
|------|---------|-----------|------|
| frequency | 旧方案（全 POST） | 无 | 已提交 |
| medication | 旧方案（全 POST） | 无 | 已提交 |
| equipment | 旧方案（全 POST） | 无 | 已提交 |
| diagnosis | 新方案（RESTful） | 有 | 未提交 |
| treatment | 新方案（RESTful） | 有 | 未提交 |
| sample | 新方案（RESTful） | 有 | 未提交 |

**三大问题**：

1. **前后端耦合严重**：旧方案 3 个模块直接用前端字段名调后端，后端字段一变就全崩
2. **接口风格不统一**：旧方案全 POST 无语义，新方案 RESTful 但 3 个模块未提交
3. **公共工具缺失**：`shared.ts` 被 3 个模块 import 但文件不存在，编译会报错

### 1.2 方案目标

- **统一 6 个模块为 RESTful + 字段映射层架构**
- **创建 `shared.ts` 公共工具**，消除重复代码
- **补齐选择接口**，6 个模块全部对接后端 select 接口
- **保留 Mock 回退**，后端未就绪时不影响开发调试
- **页面层零改动**，所有变化封装在 `index.ts` 内部

---

## 二、后端实际接口总览

基于后端 Apifox 导出文档（41 个接口），后端使用 **GET/POST/PUT/DELETE** 四种方法，路径为 **kebab-case** 风格。

### 2.1 全量接口清单

| 模块 | 操作 | 方法 | 路径 | 请求参数 |
|------|------|------|------|---------|
| **药品** | 列表 | GET | `/Drug` | query: keywords, pageIndex, pageSize |
| | 选择 | GET | `/DrugSelect` | query: name, jiliang, yongfa, pageIndex, pageSize |
| | 详情 | GET | `/drug/query-one` | query: id |
| | 新增 | POST | `/Drug/add` | body: 药品DTO（拼音字段名） |
| | 修改 | PUT | `/drug/modify-drug` | body: 含 id + version |
| | 删除 | DELETE | `/drug/remove-drug` | body: `{ ids: [] }` |
| | 启停 | PUT | `/drug/enable-disable-drug` | body: `{ ids: [], activeFlag: bool }` |
| **频次** | 列表 | GET | `/frequency/query-all` | query: keywords, status, pageIndex, pageSize |
| | 选择 | GET | `/frequency/getname` | query: keywords, status, pageIndex, pageSize |
| | 详情 | GET | `/frequency/query-one` | query: id |
| | 新增 | POST | `/frequency/add` | body: 频次DTO |
| | 修改 | PUT | `/frequency/modify` | body: 含 id |
| | 删除 | DELETE | `/frequency/remove` | query: id |
| | 启停 | PUT | `/frequency/modify-status` | query: id, status(int) |
| **器材** | 列表 | GET | `/equipment/query` | query: keywords, classification, pageIndex, pageSize |
| | 选择 | GET | `/equipment/select` | query: keywords, classification, pageIndex, pageSize |
| | 详情 | GET | `/equipment/query-one` | query: id |
| | 新增 | POST | `/equipment/add` | body: 器材DTO |
| | 修改 | PUT | `/equipment/modify` | body: 含 id |
| | 删除 | DELETE | `/equipment/delete` | body: `{ ids: [] }` |
| | 启停 | PUT | `/equipment/status` | body: `{ ids: [], status: int }` |
| **诊断** | 列表 | GET | `/diagnosis/query-all` | query: keyword, sourceEnum, pageIndex, pageSize |
| | 选择 | GET | `/diagnosis/query-select` | query: keyword, diagnosisKindCode, pageIndex, pageSize |
| | 详情 | GET | `/diagnosis/query-one` | query: id (int64) |
| | 新增 | POST | `/diagnose` | body: 诊断DTO |
| | 修改 | PUT | `/diagnose` | body: 含 id |
| | 删除 | DELETE | `/diagnose` | body: `["id"]`（裸数组） |
| | 启停 | POST | `/diagnose/batch/status` | body: `{ ids: [], status: int }` |
| **诊疗** | 列表 | GET | `/treatment/query-all` | query: keywords, pageIndex, pageSize |
| | 选择 | GET | `/treatment/query-select` | query: keywords, status, pageIndex, pageSize |
| | 详情 | GET | `/treatment/query-one` | query: id |
| | 新增 | POST | `/treatment/add-treatment` | body: 诊疗DTO |
| | 修改 | POST | `/treatment/modify-treatment` | body: 含 id + editabilityCode |
| | 删除 | DELETE | `/treatment/remove-treatment` | body: `{ ids: [] }` |
| | 启停 | POST | `/treatment/modify-status` | body: `{ ids: [], status: int }` |
| **样本** | 列表 | GET | `/sample/list-page` | query: keyword, typeEnum, status, pageIndex, pageSize |
| | 选择 | GET | `/sample/select-name` | query: keyword, pageIndex, pageSize |
| | 详情 | GET | `/sample/get-sample-name` | query: id |
| | 新增 | POST | `/sample/add-sample` | body: 样本DTO |
| | 修改 | PUT | `/sample/edit-sample` | body: 含 id |
| | 删除 | DELETE | `/sample/remove-sample` | body: `{ ids: [] }` |
| | 启停 | PUT | `/sample/switch-status` | body: `{ ids: [], status: int }` |

### 2.2 后端关键差异速查

| 维度 | 药品 | 频次 | 器材 | 诊断 | 诊疗 | 样本 |
|------|------|------|------|------|------|------|
| 启停方法 | PUT | PUT | PUT | POST | POST | PUT |
| 启停状态字段 | `activeFlag`(bool) | `status`(int, query) | `status`(int) | `status`(int) | `status`(int) | `status`(int) |
| 启停批量 | 是 | **否（单 id）** | 是 | 是 | 是 | 是 |
| 删除方法 | DELETE | DELETE | DELETE | DELETE | DELETE | DELETE |
| 删除参数 | `{ ids: [] }` | query: id | `{ ids: [] }` | `["id"]` 裸数组 | `{ ids: [] }` | `{ ids: [] }` |
| 修改方法 | PUT | PUT | PUT | PUT | **POST** | PUT |
| 乐观锁 | version | 无 | 无 | 无 | 无 | 无 |
| 列表状态字段 | `statusEnum`+`activeFlag` | `status`(int) | `status`(int) | `statusEnum`(string) | `status`(int) | `status`(int) |
| 编码字段名 | `busNo` | `busNo` | `busNo` | `conditionCode` | `busNo` | `busNo` |
| 选择接口返回 | 分页 | ListJsonVO(数组) | 分页 | 分页 | 分页 | 非分页对象 |

---

## 三、架构设计

### 3.1 分层架构

```
┌─────────────────────────────────────────────────────┐
│  页面层（views/basicdata/*/Index.vue + EditDialog）  │  ← 零改动
├─────────────────────────────────────────────────────┤
│  API 层（apis/basicdata/*/index.ts）                 │  ← 本方案核心
│  ┌───────────────────────────────────────────────┐  │
│  │  1. 导出函数（listXxx, createXxx, ...）        │  │
│  │  2. 字段映射（mapListItem, mapDetail, toDTO）  │  │
│  │  3. HTTP 调用 + Mock 回退                      │  │
│  └───────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────┤
│  公共工具（apis/basicdata/shared.ts）                │  ← 新建
│  ┌───────────────────────────────────────────────┐  │
│  │  parseStatus / encodeStatus / buildBatchPayload│  │
│  └───────────────────────────────────────────────┘  │
├─────────────────────────────────────────────────────┤
│  类型定义（apis/basicdata/*/type.ts）                │  ← 前端独立
│  Mock 数据（apis/basicdata/*/mock.ts）               │  ← 回退用
├─────────────────────────────────────────────────────┤
│  HTTP 插件（plugins/http.ts）                        │  ← 已有
│  Vite 代理（/api → 后端网关）                        │  ← 已有
└─────────────────────────────────────────────────────┘
```

### 3.2 核心原则

1. **前端类型独立**：`type.ts` 中的字段名面向页面需求设计（`code`/`name`/`enabled`），不依赖后端字段名
2. **映射层隔离**：每个 `index.ts` 内部定义 `mapListItem`/`mapDetail`/`toCreateDTO`/`toUpdateDTO`，完成前后端字段转换
3. **HTTP 优先 Mock 回退**：try 块发 HTTP 请求，catch 块回退 Mock 数据，后端未就绪不影响前端开发
4. **页面层零改动**：所有 HTTP 方法、路径、字段映射的变化都封装在 `index.ts` 内部，页面调用的函数签名不变

### 3.3 公共工具 shared.ts

新建 `src/apis/basicdata/shared.ts`，统一处理各模块状态字段不统一的问题：

```typescript
/**
 * 状态解析：将后端各种格式的状态字段统一转为 boolean
 * 兼容：integer(0/1) | string("ACTIVE"/"INACTIVE") | boolean | string("0"/"1")
 */
export function parseStatus(value: unknown): boolean {
  if (typeof value === 'boolean') return value
  if (typeof value === 'number') return value === 1
  if (typeof value === 'string') {
    const s = value.toUpperCase()
    if (s === 'ACTIVE' || s === '1' || s === 'TRUE') return true
    if (s === 'INACTIVE' || s === '0' || s === 'FALSE') return false
  }
  return false
}

/**
 * 状态编码：将 boolean 转为后端期望的格式
 * - status: integer → 1/0
 * - statusEnum: string → 'ACTIVE'/'INACTIVE'
 * - activeFlag: boolean → true/false
 */
export function encodeStatusInt(enabled: boolean): number {
  return enabled ? 1 : 0
}

export function encodeStatusEnum(enabled: boolean): string {
  return enabled ? 'ACTIVE' : 'INACTIVE'
}

/**
 * 构建批量启停请求体
 * 大多数模块用 { ids: [], status: 1/0 }
 * 药品用 { ids: [], activeFlag: true/false }
 */
export function buildBatchStatusPayload(
  ids: string[],
  enabled: boolean,
  mode: 'int' | 'bool' = 'int'
): Record<string, unknown> {
  return {
    ids,
    ...(mode === 'int'
      ? { status: encodeStatusInt(enabled) }
      : { activeFlag: enabled })
  }
}
```

---

## 四、各模块改进详情

### 4.1 药品模块（medication）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 | 特殊说明 |
|------|------|------|---------|---------|
| 列表 | GET | `/Drug` | query: keywords, pageIndex, pageSize | 路径首字母大写 |
| 选择 | GET | `/DrugSelect` | query: name, jiliang, yongfa, pageIndex, pageSize | **新增**，参数用拼音名 |
| 详情 | GET | `/drug/query-one` | query: id | 返回含 version |
| 新增 | POST | `/Drug/add` | body: 药品DTO | **DTO 含拼音字段名** |
| 修改 | PUT | `/drug/modify-drug` | body: 含 id + version | 乐观锁 |
| 删除 | DELETE | `/drug/remove-drug` | body: `{ ids: [id] }` | 批量 |
| 启停 | PUT | `/drug/enable-disable-drug` | body: `{ ids: [id], activeFlag: bool }` | activeFlag 非 status |
| 表单选项 | POST | `/medication/getMedicationInit` | body: `{}` | 保留现有 |

#### 字段映射

| 前端字段 | 后端字段 | 说明 |
|---------|---------|------|
| id | id | — |
| code | busNo | 药品业务编码 |
| name | name | 通用名称 |
| genericName | merchandiseName | 商品名称 |
| category | categoryCode | 药品类别编码 |
| specification | totalVolume | 规格 |
| dosageForm | doseFormCode | 剂型编码 |
| unit | stockUnitCode | 库存单位编码 |
| manufacturer | manufacturerText | 生产厂家 |
| retailPrice | retailPrice | 零售价 |
| enabled | parseStatus(activeFlag ?? statusEnum) | 兼容双字段 |
| sort | sortNo | 排序号 |
| remark | descriptionText | 备注 |
| updatedAt | updatedAt | 更新时间 |

#### 启停接口特殊处理

```typescript
export const setMedicationEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.put('/drug/enable-disable-drug', {
      ids: [id],
      activeFlag: enabled  // 药品用 activeFlag 而非 status
    })
  } catch {
    await mockSetMedicationEnabled(id, enabled)
  }
}
```

#### 删除接口特殊处理

```typescript
export const removeMedication = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete('/drug/remove-drug', { data: { ids: [id] } })
  } catch {
    await mockRemoveMedication(id)
  }
}
```

#### 修改接口（乐观锁）

```typescript
export const updateMedication = async (
  id: string,
  data: MedicationSavePayload
): Promise<MedicationDetail> => {
  try {
    const http = useHttp()
    // 先获取 version
    const detailRes = await http.get<{ version?: string }>('/drug/query-one', { id })
    const version = detailRes.data?.version ?? 'V1.0'
    const res = await http.put('/drug/modify-drug', {
      ...toUpdateDTO(id, data),
      version
    })
    if (!res.data) throw new Error('修改药品失败')
    return mapDetail(res.data)
  } catch {
    return mockUpdateMedication(id, data)
  }
}
```

---

### 4.2 频次模块（frequency）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 | 特殊说明 |
|------|------|------|---------|---------|
| 列表 | GET | `/frequency/query-all` | query: keywords, status, pageIndex, pageSize | — |
| 选择 | GET | `/frequency/getname` | query: keywords, status, pageIndex, pageSize | **新增**，返回数组非分页 |
| 详情 | GET | `/frequency/query-one` | query: id | — |
| 新增 | POST | `/frequency/add` | body: 频次DTO | — |
| 修改 | PUT | `/frequency/modify` | body: 含 id | — |
| 删除 | DELETE | `/frequency/remove` | query: id | **唯一用 query 参数删除** |
| 启停 | PUT | `/frequency/modify-status` | query: id, status(int) | **唯一单 id 非批量** |
| 表单选项 | POST | `/frequency/getFrequencyInit` | body: `{}` | 保留现有 |

#### 字段映射

| 前端字段 | 后端字段 | 说明 |
|---------|---------|------|
| id | id | — |
| code | busNo | 频次编码 |
| name | name | 频次名称 |
| type | typeEnum | 频次类型 |
| typeName | typeCode_dictText | 类型名称（展示用） |
| timesPerDay | timesPerDay | 每日执行次数 |
| intervalHours | intervalHours | 间隔时间 |
| timePoints | timePoints | 执行时间点 |
| enabled | parseStatus(status) | 后端用 integer |
| sort | sortNo | 排序号 |
| remark | descriptionText | 备注 |
| updatedAt | updatedAt | 更新时间 |

#### 启停接口特殊处理

```typescript
export const setFrequencyEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    // 频次是唯一使用 query 参数传单个 id 的模块
    await http.put('/frequency/modify-status', undefined, {
      params: { id, status: encodeStatusInt(enabled) }
    })
  } catch {
    await mockSetFrequencyEnabled(id, enabled)
  }
}
```

#### 删除接口特殊处理

```typescript
export const removeFrequency = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    // 频次删除也是 query 参数
    await http.delete('/frequency/remove', undefined, { params: { id } })
  } catch {
    await mockRemoveFrequency(id)
  }
}
```

---

### 4.3 器材模块（equipment）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 |
|------|------|------|---------|
| 列表 | GET | `/equipment/query` | query: keywords, classification, pageIndex, pageSize |
| 选择 | GET | `/equipment/select` | query: keywords, classification, pageIndex, pageSize |
| 详情 | GET | `/equipment/query-one` | query: id |
| 新增 | POST | `/equipment/add` | body: 器材DTO |
| 修改 | PUT | `/equipment/modify` | body: 含 id |
| 删除 | DELETE | `/equipment/delete` | body: `{ ids: [id] }` |
| 启停 | PUT | `/equipment/status` | body: `{ ids: [id], status: int }` |
| 表单选项 | POST | `/equipment/getEquipmentInit` | body: `{}` |

#### 字段映射

| 前端字段 | 后端字段 | 说明 |
|---------|---------|------|
| id | id | — |
| code | busNo | 器材编码 |
| name | name | 器材名称 |
| pyCode | pyStr | 拼音码 |
| category | classificationCode | 器材分类编码 |
| categoryName | classificationCode_dictText | 分类名称 |
| specification | specification | 规格 |
| model | model | 型号 |
| unit | unitCode | 计量单位编码 |
| price | retailPrice | 单价 |
| enabled | parseStatus(status) | 后端用 integer |
| sort | sortNo | 排序号 |
| remark | descriptionText | 备注 |
| updatedAt | updatedAt | 更新时间 |

#### 启停接口（标准批量格式）

```typescript
export const setEquipmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.put('/equipment/status', buildBatchStatusPayload([id], enabled))
  } catch {
    await mockSetEquipmentEnabled(id, enabled)
  }
}
```

---

### 4.4 诊断模块（diagnosis）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 | 特殊说明 |
|------|------|------|---------|---------|
| 列表 | GET | `/diagnosis/query-all` | query: keyword, sourceEnum, pageIndex, pageSize | — |
| 选择 | GET | `/diagnosis/query-select` | query: keyword, diagnosisKindCode, pageIndex, pageSize | **新增** |
| 详情 | GET | `/diagnosis/query-one` | query: id | id 类型为 int64 |
| 新增 | POST | `/diagnose` | body: 诊断DTO | **路径与查询不同** |
| 修改 | PUT | `/diagnose` | body: 含 id | **路径与查询不同** |
| 删除 | DELETE | `/diagnose` | body: `["id"]` 裸数组 | **唯一裸数组格式** |
| 启停 | POST | `/diagnose/batch/status` | body: `{ ids: [id], status: int }` | **POST 非 PUT** |
| 表单选项 | — | 无后端接口 | Mock 回退 | — |

#### 字段映射

| 前端字段 | 后端列表字段 | 后端详情字段 | 新增DTO | 修改DTO |
|---------|-------------|-------------|---------|---------|
| id | id | id | — | id |
| code | conditionCode | conditionCode | diseaseCode | conditionCode |
| name | name | name | name | name |
| pyCode | — | pyStr | pyStr | pyStr |
| type | — | sourceEnum | sourceEnum | sourceEnum |
| typeName | — | sourceEnum_enumText | — | — |
| category | — | typeCode | typeCode | typeCode |
| categoryName | — | typeCode_dictText | — | — |
| enabled | parseStatus(statusEnum) | parseStatus(statusEnum) | encodeStatusEnum(enabled) | encodeStatusEnum(enabled) |
| sort | — | — | — | — |
| remark | description | description | description | description |
| updatedAt | — | — | — | — |

> **注意**：新增用 `diseaseCode`，修改用 `conditionCode`，这是后端接口的差异。

#### 删除接口特殊处理（裸数组）

```typescript
export const removeDiagnosis = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    // 诊断删除接口 body 为裸数组 ["id"]，非 { ids: [] }
    await http.delete('/diagnose', { data: [id] })
  } catch {
    await mockRemoveDiagnosis(id)
  }
}
```

#### 启停接口特殊处理（POST）

```typescript
export const setDiagnosisEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    // 诊断启停用 POST 而非 PUT
    await http.post('/diagnose/batch/status', buildBatchStatusPayload([id], enabled))
  } catch {
    await mockSetDiagnosisEnabled(id, enabled)
  }
}
```

---

### 4.5 诊疗模块（treatment）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 | 特殊说明 |
|------|------|------|---------|---------|
| 列表 | GET | `/treatment/query-all` | query: keywords, pageIndex, pageSize | — |
| 选择 | GET | `/treatment/query-select` | query: keywords, status, pageIndex, pageSize | 已有 |
| 详情 | GET | `/treatment/query-one` | query: id | 返回含 children 子项目 |
| 新增 | POST | `/treatment/add-treatment` | body: 诊疗DTO | — |
| 修改 | POST | `/treatment/modify-treatment` | body: 含 id + editabilityCode | **POST 非 PUT** |
| 删除 | DELETE | `/treatment/remove-treatment` | body: `{ ids: [id] }` | 批量 |
| 启停 | POST | `/treatment/modify-status` | body: `{ ids: [id], status: int }` | **POST 非 PUT** |
| 表单选项 | GET | `/treatment/get-form-options` | 无参数 | 已有 |

#### 字段映射

| 前端字段 | 后端字段 | 说明 |
|---------|---------|------|
| id | id | — |
| code | busNo | 诊疗项目编码 |
| name | name | 诊疗项目名称 |
| pyCode | pyStr | 拼音码 |
| category | categoryCode | 诊疗类别编码 |
| categoryName | categoryCode_dictText | 类别名称 |
| unit | packageUnitCode | 计价单位编码 |
| price | retailPrice | 价格 |
| enabled | parseStatus(status) | 后端用 integer |
| sort | sortNo | 排序号 |
| remark | descriptionText | 备注 |
| updatedAt | updatedAt | 更新时间 |
| — | editabilityCode | 修改时固定传 `'OK'` |

#### 修改接口特殊处理

```typescript
export const updateTreatment = async (
  id: string,
  data: TreatmentSavePayload
): Promise<TreatmentDetail> => {
  try {
    const http = useHttp()
    // 诊疗修改用 POST 而非 PUT
    const res = await http.post('/treatment/modify-treatment', {
      ...toUpdateDTO(id, data),
      editabilityCode: 'OK'
    })
    if (!res.data) throw new Error('修改诊疗项目失败')
    return mapDetail(res.data)
  } catch {
    return mockUpdateTreatment(id, data)
  }
}
```

---

### 4.6 样本模块（sample）

#### 后端接口对接

| 操作 | 方法 | 路径 | 请求参数 | 特殊说明 |
|------|------|------|---------|---------|
| 列表 | GET | `/sample/list-page` | query: keyword, typeEnum, status, pageIndex, pageSize | — |
| 选择 | GET | `/sample/select-name` | query: keyword, pageIndex, pageSize | **新增**，返回非分页 |
| 详情 | GET | `/sample/get-sample-name` | query: id | — |
| 新增 | POST | `/sample/add-sample` | body: 样本DTO | 返回 id 字符串 |
| 修改 | PUT | `/sample/edit-sample` | body: 含 id | — |
| 删除 | DELETE | `/sample/remove-sample` | body: `{ ids: [id] }` | 批量 |
| 启停 | PUT | `/sample/switch-status` | body: `{ ids: [id], status: int }` | 批量 |
| 表单选项 | — | 无后端接口 | Mock 回退 | — |

#### 字段映射

| 前端字段 | 后端字段 | 说明 |
|---------|---------|------|
| id | id | — |
| code | busNo | 样本编码 |
| name | name | 样本名称 |
| pyCode | pyStr | 拼音码 |
| type | typeEnum | 样本类型编码 |
| typeName | typeEnum_dictText | 类型名称 |
| collectionMethod | collectionMethod | 采集方法 |
| storageCondition | storageCondition | 保存条件 |
| validHours | validHours | 有效时长 |
| enabled | parseStatus(status) | 后端用 integer |
| sort | sortNo | 排序号 |
| remark | descriptionText | 备注 |
| updatedAt | updatedAt | 更新时间 |

> **注意**：后端实际 API 无 `version` 乐观锁，删除接口直接用 `{ ids: [id] }`。

#### 删除接口（修正：移除乐观锁逻辑）

```typescript
export const removeSample = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    // 后端实际 API 无 version 乐观锁，直接批量删除
    await http.delete('/sample/remove-sample', { data: { ids: [id] } })
  } catch {
    await mockRemoveSample(id)
  }
}
```

---

## 五、新增选择接口

当前前端完全没有对接后端的选择接口，全部依赖 Mock。本方案为 6 个模块新增 `selectXxx` 函数。

### 5.1 统一函数签名

```typescript
/** 选择接口统一签名 */
export interface SelectResult<T> {
  rows: T[]
  total: number
}

export const selectXxx = async (
  keyword: string,
  pageIndex: number = 1,
  pageSize: number = 20
): Promise<SelectResult<XxxRecord>> => { ... }
```

### 5.2 各模块选择接口

| 模块 | 函数名 | 方法 | 路径 | 返回处理 |
|------|--------|------|------|---------|
| 药品 | `selectMedication` | GET | `/DrugSelect` | 分页 → SelectResult |
| 频次 | `selectFrequency` | GET | `/frequency/getname` | ListJsonVO(数组) → SelectResult |
| 器材 | `selectEquipment` | GET | `/equipment/select` | 分页 → SelectResult |
| 诊断 | `selectDiagnosis` | GET | `/diagnosis/query-select` | 分页 → SelectResult |
| 诊疗 | `selectTreatment` | GET | `/treatment/query-select` | 分页 → SelectResult（已有） |
| 样本 | `selectSample` | GET | `/sample/select-name` | 非分页对象 → SelectResult |

### 5.3 特殊返回格式处理

**频次选择接口**返回的是 `ListJsonVO`（data 直接为数组），需要包装为分页格式：

```typescript
export const selectFrequency = async (
  keyword: string,
  pageIndex: number = 1,
  pageSize: number = 20
): Promise<SelectResult<FrequencyRecord>> => {
  try {
    const http = useHttp()
    const res = await http.get<FrequencyRecord[]>('/frequency/getname', {
      keywords: keyword,
      status: 1,
      pageIndex: String(pageIndex),
      pageSize: String(pageSize)
    })
    const all = res.data ?? []
    const start = (pageIndex - 1) * pageSize
    return {
      rows: all.slice(start, start + pageSize),
      total: all.length
    }
  } catch {
    return mockListFrequency({ keyword, pageIndex, pageSize })
  }
}
```

**样本选择接口**返回非分页对象，需要适配：

```typescript
export const selectSample = async (
  keyword: string,
  pageIndex: number = 1,
  pageSize: number = 20
): Promise<SelectResult<SampleRecord>> => {
  try {
    const http = useHttp()
    const res = await http.get<SampleRecord | SampleRecord[]>('/sample/select-name', {
      keyword,
      pageIndex: String(pageIndex),
      pageSize: String(pageSize)
    })
    const rows = Array.isArray(res.data) ? res.data : res.data ? [res.data] : []
    return { rows, total: rows.length }
  } catch {
    return mockListSample({ keyword, pageIndex, pageSize })
  }
}
```

---

## 六、改动文件清单

### 6.1 新建文件

| 文件 | 说明 |
|------|------|
| `src/apis/basicdata/shared.ts` | 公共工具（parseStatus / encodeStatus / buildBatchStatusPayload） |

### 6.2 修改文件（API 层）

| 文件 | 改动内容 |
|------|---------|
| `src/apis/basicdata/medication/index.ts` | 全量重写：加映射层、改 RESTful、加乐观锁、加 select |
| `src/apis/basicdata/frequency/index.ts` | 全量重写：加映射层、改 RESTful、处理单 id 启停/删除、加 select |
| `src/apis/basicdata/equipment/index.ts` | 全量重写：加映射层、改 RESTful、加 select |
| `src/apis/basicdata/diagnosis/index.ts` | 修正：路径改为 `/diagnose`、删除裸数组、启停改 POST、加 select |
| `src/apis/basicdata/treatment/index.ts` | 修正：确认 POST 修改、修正路径、已有 select 保持 |
| `src/apis/basicdata/sample/index.ts` | 修正：移除乐观锁、加 select、修正列表路径 |

### 6.3 不改动文件

| 文件 | 原因 |
|------|------|
| `src/apis/basicdata/*/type.ts` | 前端类型独立设计，不依赖后端字段 |
| `src/apis/basicdata/*/mock.ts` | Mock 数据保持前端字段名，回退时用 |
| `src/views/basicdata/**` | 页面层零改动，函数签名不变 |
| `src/config/frontend-menus.ts` | 菜单配置不变 |
| `src/router/main/basicdata/index.ts` | 路由配置不变 |

---

## 七、实施计划

### 7.1 分阶段实施

| 阶段 | 内容 | 预计工时 | 依赖 |
|------|------|---------|------|
| **P0** | 创建 `shared.ts` + 升级 frequency/equipment/medication | 2h | 无 |
| **P1** | 修正 diagnosis/treatment/sample 的路径和参数格式 | 1h | P0 |
| **P2** | 6 个模块新增 select 接口 | 1h | P1 |
| **P3** | 联调测试 + Vite 代理切换 | 2h | P2 |

### 7.2 实施顺序

```
P0: shared.ts
 ├→ frequency/index.ts（加映射层 + RESTful）
 ├→ equipment/index.ts（加映射层 + RESTful）
 └→ medication/index.ts（加映射层 + RESTful + 乐观锁）
      ↓
P1: 修正已升级模块
 ├→ diagnosis/index.ts（路径 /diagnose、裸数组删除、POST启停）
 ├→ treatment/index.ts（确认 POST 修改）
 └→ sample/index.ts（移除乐观锁、修正路径）
      ↓
P2: 新增选择接口
 ├→ selectMedication
 ├→ selectFrequency
 ├→ selectEquipment
 ├→ selectDiagnosis
 ├→ selectTreatment（已有，保持）
 └→ selectSample
      ↓
P3: 联调
 ├→ Vite 代理从 Apifox Mock 切换到后端网关
 ├→ 逐模块验证 CRUD + 启停 + 选择
 └→ 异常场景测试（网络断开回退 Mock）
```

### 7.3 验收标准

| 验收项 | 验证方法 |
|--------|---------|
| `shared.ts` 存在且导出 3 个函数 | `import { parseStatus, encodeStatusInt, buildBatchStatusPayload } from '../shared'` 不报错 |
| 6 个模块编译通过 | `npx vue-tsc --noEmit` 零错误 |
| 6 个模块有字段映射层 | 每个 `index.ts` 包含 `mapListItem` / `mapDetail` / `toCreateDTO` / `toUpdateDTO` |
| 6 个模块有 select 接口 | 每个 `index.ts` 导出 `selectXxx` 函数 |
| HTTP 方法符合后端 | 列表 GET、新增 POST、修改 PUT/POST、删除 DELETE、启停 PUT/POST |
| Mock 回退正常 | 后端未启动时，所有操作回退 Mock 不报错 |
| 页面功能正常 | 6 个模块的列表/新增/编辑/删除/启停全部可用 |

---

## 八、风险与应对

| 风险 | 影响 | 应对 |
|------|------|------|
| 后端药品新增 DTO 使用拼音字段名 | `toCreateDTO` 映射复杂 | 映射层隔离，只改一个函数 |
| 诊断模块路径分裂（`/diagnosis` 查询 vs `/diagnose` CRUD） | 容易混淆路径 | 代码注释标注，`currBaseUrl` 分查询和 CRUD 两个常量 |
| 诊断删除接口 body 为裸数组 | 不符合常规格式 | 单独处理，代码注释标注 |
| 频次启停/删除用 query 参数 | 与其他模块不一致 | 单独处理，代码注释标注 |
| 后端接口未全部就绪 | 部分接口 404 | HTTP 优先 + Mock 回退，逐个接口联调 |
| 药品修改需要乐观锁 version | 需先 GET 详情获取 version | 在 `updateMedication` 内部自动获取，页面无感 |

---

## 九、向后端反馈的设计问题

以下问题建议后端团队优化，但不阻塞前端开发（映射层已兼容）：

1. **药品新增 DTO 拼音字段名**：`changjia`(厂家)、`gongyingshang`(供应商)、`wenhao`(文号) 等拼音字段名与修改 DTO 的 camelCase 不一致，建议统一为 camelCase
2. **诊断路径分裂**：查询走 `/diagnosis/*`，CRUD 走 `/diagnose`，建议统一为一个前缀
3. **诊断删除裸数组**：body 为 `["id"]` 而非 `{ ids: [] }`，与其他模块不一致
4. **频次启停单 id**：唯一不支持批量启停的模块，建议改为 `{ ids: [], status }` 批量格式
5. **诊疗修改用 POST**：其他模块修改用 PUT，诊疗用 POST，建议统一
6. **频次选择返回非分页**：返回 `ListJsonVO`（数组），其他模块返回分页格式，建议统一

---

## 附录：各模块 index.ts 代码模板

### 标准模板（以 equipment 为例）

```typescript
import { useHttp } from '@/plugins/http'
import {
  mockCreateEquipment,
  mockGetEquipmentDetail,
  mockListEquipment,
  mockLoadEquipmentFormOptions,
  mockRemoveEquipment,
  mockSetEquipmentEnabled,
  mockUpdateEquipment
} from './mock'
import { parseStatus, buildBatchStatusPayload } from '../shared'
import type {
  EquipmentDetail,
  EquipmentFormOptions,
  EquipmentQuery,
  EquipmentRecord,
  EquipmentSavePayload
} from './type'

const currBaseUrl = '/equipment'

/* ── 后端响应类型（仅模块内部使用） ── */

interface BackendEquipmentItem {
  id: string
  busNo?: string
  name?: string
  pyStr?: string
  classificationCode?: string
  classificationCode_dictText?: string
  specification?: string
  model?: string
  unitCode?: string
  retailPrice?: number
  status?: number
  sortNo?: number
  descriptionText?: string
  updatedAt?: string
}

interface BackendPageDTO<T> {
  pageIndex?: number
  pageSize?: number
  total?: number
  pages?: number
  rows?: T[]
}

/* ── 字段映射 ── */

function mapListItem(item: BackendEquipmentItem): EquipmentRecord {
  return {
    id: item.id,
    code: item.busNo ?? '',
    name: item.name ?? '',
    pyCode: item.pyStr ?? '',
    category: item.classificationCode ?? '',
    categoryName: item.classificationCode_dictText ?? '',
    specification: item.specification ?? '',
    model: item.model ?? '',
    unit: item.unitCode ?? '',
    price: item.retailPrice ?? 0,
    enabled: parseStatus(item.status),
    sort: item.sortNo ?? 0,
    remark: item.descriptionText ?? '',
    updatedAt: item.updatedAt ?? ''
  }
}

function mapDetail(item: BackendEquipmentItem): EquipmentDetail {
  return {
    id: item.id,
    code: item.busNo ?? '',
    name: item.name ?? '',
    pyCode: item.pyStr ?? '',
    category: item.classificationCode ?? '',
    specification: item.specification ?? '',
    model: item.model ?? '',
    unit: item.unitCode ?? '',
    price: item.retailPrice ?? 0,
    enabled: parseStatus(item.status),
    sort: item.sortNo ?? 0,
    remark: item.descriptionText ?? ''
  }
}

function toCreateDTO(data: EquipmentSavePayload) {
  return {
    busNo: data.code,
    name: data.name,
    pyStr: data.pyCode,
    classificationCode: data.category,
    specification: data.specification,
    model: data.model,
    unitCode: data.unit,
    retailPrice: data.price,
    status: data.enabled ? 1 : 0,
    sortNo: data.sort,
    descriptionText: data.remark
  }
}

function toUpdateDTO(id: string, data: EquipmentSavePayload) {
  return { ...toCreateDTO(data), id }
}

/* ── API 函数 ── */

export const listEquipment = async (
  query: EquipmentQuery
): Promise<{ rows: EquipmentRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.get<BackendPageDTO<BackendEquipmentItem>>(
      currBaseUrl + '/query',
      {
        keywords: query.keyword ?? '',
        classification: query.category ?? '',
        pageIndex: String(query.pageIndex),
        pageSize: String(query.pageSize)
      }
    )
    const pageData = res.data
    return {
      rows: (pageData?.rows ?? []).map(mapListItem),
      total: pageData?.total ?? 0
    }
  } catch {
    return mockListEquipment(query)
  }
}

export const getEquipmentDetail = async (id: string): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.get<BackendEquipmentItem>(currBaseUrl + '/query-one', { id })
    if (!res.data) throw new Error('获取器材详情失败')
    return mapDetail(res.data)
  } catch {
    return mockGetEquipmentDetail(id)
  }
}

export const createEquipment = async (data: EquipmentSavePayload): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.post<BackendEquipmentItem>(currBaseUrl + '/add', toCreateDTO(data))
    if (!res.data) throw new Error('新增器材失败')
    return mapDetail(res.data)
  } catch {
    return mockCreateEquipment(data)
  }
}

export const updateEquipment = async (
  id: string,
  data: EquipmentSavePayload
): Promise<EquipmentDetail> => {
  try {
    const http = useHttp()
    const res = await http.put<BackendEquipmentItem>(currBaseUrl + '/modify', toUpdateDTO(id, data))
    if (!res.data) throw new Error('修改器材失败')
    return mapDetail(res.data)
  } catch {
    return mockUpdateEquipment(id, data)
  }
}

export const setEquipmentEnabled = async (id: string, enabled: boolean): Promise<void> => {
  try {
    const http = useHttp()
    await http.put(currBaseUrl + '/status', buildBatchStatusPayload([id], enabled))
  } catch {
    await mockSetEquipmentEnabled(id, enabled)
  }
}

export const removeEquipment = async (id: string): Promise<void> => {
  try {
    const http = useHttp()
    await http.delete(currBaseUrl + '/delete', { data: { ids: [id] } })
  } catch {
    await mockRemoveEquipment(id)
  }
}

export const loadEquipmentFormOptions = async (): Promise<EquipmentFormOptions> => {
  try {
    const http = useHttp()
    const res = await http.post<EquipmentFormOptions>(currBaseUrl + '/getEquipmentInit', {})
    if (!res.data) throw new Error('获取表单选项失败')
    return res.data
  } catch {
    return mockLoadEquipmentFormOptions()
  }
}

export const selectEquipment = async (
  keyword: string,
  pageIndex: number = 1,
  pageSize: number = 20
): Promise<{ rows: EquipmentRecord[]; total: number }> => {
  try {
    const http = useHttp()
    const res = await http.get<BackendPageDTO<BackendEquipmentItem>>(
      currBaseUrl + '/select',
      {
        keywords: keyword,
        pageIndex: String(pageIndex),
        pageSize: String(pageSize)
      }
    )
    const pageData = res.data
    return {
      rows: (pageData?.rows ?? []).map(mapListItem),
      total: pageData?.total ?? 0
    }
  } catch {
    return mockListEquipment({ keyword, pageIndex, pageSize })
  }
}

export type * from './type'
```

> 其他模块按此模板修改，注意替换路径、字段名和特殊处理逻辑。
