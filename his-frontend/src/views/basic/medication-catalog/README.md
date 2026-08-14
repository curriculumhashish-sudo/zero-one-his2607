# 药品目录管理模块 — 开发方案

## 一、需求概述

参照 OpenHis 系统的 `药品管理` 页面（`/basic/medicationcatalog`），在当前 `zero-one-his` 前端项目中实现**药品目录管理**模块。该模块属于"基础数据"业务域，提供药品的增删改查、批量启停用、分页查询、多条件搜索等功能。

---

## 二、目标页面功能分析

### 2.1 列表页功能

| 功能区域 | 具体内容 |
|---------|---------|
| 搜索栏 | 关键词（药品名称或编码）、药品名称、药品分类（下拉） |
| 工具栏 | 新增、批量启用、批量停用、删除（批量） |
| 数据表格 | 复选框、序号、编码、项目名（可点击查看详情）、规格、操作列（编辑/停用/删除） |
| 分页 | 10条/页，支持翻页、跳页 |
| 行操作 | 编辑、停用/启用（动态切换）、删除 |

### 2.2 新增/编辑表单字段

表单分为三大区域，共 **50+ 字段**：

**基础信息区**：

| 字段 | 类型 | 必填 | 说明 |
|------|------|------|------|
| 通用名称 | 文本 | 是 | 药品通用名 |
| 商品名称 | 文本 | 是 | 商品名 |
| 药品分类 | 下拉 | 是 | 药品分类编码 |
| 生产厂家 | 文本 | 是 | 生产厂家（纯文本，max 200） |
| 供应商 | 下拉 | 是 | 供应商标识 |
| 批准文号 | 文本 | 否 | NMPA 注册号 |
| 贯标国家编码 | 文本 | 否 | 国家药品贯标编码 |
| 医保编码 | 文本 | 否 | 医保编码 |
| 财务类型 | 下拉 | 是 | 财务分类 |
| 医保等级 | 下拉 | 否 | 甲/乙/丙类 |
| 病案结算类型 | 下拉 | 否 | 结算分类 |
| 医保类别 | 下拉 | 否 | 医保类别 |
| 医保对码 | 复选 | 否 | 是否已对码 |

**临床与规格信息区**：

| 字段 | 类型 | 必填 | 说明 |
|------|------|------|------|
| 药品性质 | 下拉 | 是 | 药品性质编码 |
| 规格 | 文本 | 是 | 如 2ml、0.25g |
| 剂量单位 | 下拉 | 是 | 5类单位之一 |
| 拆零/剂量倍数 | 数值 | 是 | 剂量→拆零换算 |
| 剂型 | 下拉 | 是 | 剂型编码 |
| 皮试判别 | 布尔 | 否 | 是否皮试药 |
| 注射药品 | 布尔 | 否 | 是否注射药 |
| 用量限定 | 文本 | 否 | 用量限制说明 |
| 默认用法 | 下拉 | 否 | 用法编码 |
| 默认用药频次 | 下拉 | 否 | 频次编码 |
| 单次剂量 | 数值 | 否 | 单次剂量 |
| 单次最大剂量 | 文本 | 否 | 单次最大剂量 |
| 用法说明 | 下拉 | 否 | 用法说明 |
| 限制使用范围 | 文本 | 否 | 限制范围 |
| 药品说明 | 文本 | 否 | 药品说明 |
| DDD值 | 文本 | 否 | 日限定剂量 |
| DDD单位 | 文本 | 否 | DDD 单位 |

**价格与单位信息区**：

| 字段 | 类型 | 必填 | 说明 |
|------|------|------|------|
| 采购单位 | 下拉 | 是 | 5类单位之一 |
| 采购/拆零倍数 | 数值 | 是 | 采购→拆零换算 |
| 购入价（包装单位） | 数值 | 是 | 采购价格 |
| 门诊拆分属性 | 下拉 | 是 | 门诊拆分属性 |
| 库存单位 | 下拉 | 是 | 5类单位之一 |
| 库存/拆零倍数 | 数值 | 是 | 库存→拆零换算 |
| 零售价（包装单位） | 数值 | 是 | 零售价格 |
| 住院临时医嘱拆分属性 | 下拉 | 是 | 住院拆分属性 |
| 包装单位 | 下拉 | 是 | 5类单位之一 |
| 包装/拆零倍数 | 数值 | 是 | 包装→拆零换算 |
| 最高零售价（包装单位） | 数值 | 否 | 最高限价 |
| 拆零单位 | 下拉 | 是 | 5类单位之一 |
| 警戒线最高值 | 数值 | 否 | 库存上限 |
| 警戒线最低值 | 数值 | 否 | 库存下限 |

**编码与标志区**：

| 字段 | 类型 | 必填 | 说明 |
|------|------|------|------|
| 药品拼音码 | 文本 | 否 | 自动生成/手输 |
| 药品五笔码 | 文本 | 否 | 自动生成/手输 |
| 商品拼音码 | 文本 | 否 | - |
| 商品五笔码 | 文本 | 否 | - |
| 69码 | 文本 | 否 | 药品 69 码 |
| 药品状态 | 下拉 | 否 | DRAFT/ACTIVE/INACTIVE/DISCARDED |
| 领域 | 下拉 | 否 | 适用范围 |
| 基药标识 | 布尔 | 否 | 是否基本药物 |
| 儿童用药标志 | 布尔 | 否 | - |
| 自制药标识 | 布尔 | 否 | - |
| 是否活性 | 布尔 | 否 | - |
| 限制使用 | 布尔 | 否 | - |
| 抗生素 | 布尔 | 否 | - |
| 处方标志 | 布尔 | 否 | 处方药标志 |

---

## 三、数据库表映射

对应数据库表：`med_medication_definition`（70+ 字段）

关键字段映射关系（前端字段 → 数据库字段）：

```
通用名称        → name
商品名称        → merchandise_name
药品分类        → category_code
生产厂家        → manufacturer_text / manufacturer_id
供应商          → supply_id
批准文号        → approval_number
贯标国家编码    → national_drug_code
医保编码        → yb_no
药品性质        → characteristic
规格            → total_volume
剂量单位        → dose_unit_code
拆零/剂量倍数   → dose_unit_ratio
剂型            → dose_form_code
皮试判别        → skin_test_flag
注射药品        → inject_flag
默认用法        → method_code
默认用药频次    → rate_code
单次剂量        → dose
单次最大剂量    → max_unit
用量限定        → usage_limit
采购单位        → purchase_unit_code
采购/拆零倍数   → purchase_unit_ratio
购入价          → (需关联价格表)
门诊拆分属性    → part_attribute_enum
库存单位        → stock_unit_code
库存/拆零倍数   → stock_unit_ratio
零售价          → (需关联价格表)
住院拆分属性    → tho_part_attribute_enum
包装单位        → package_unit_code
包装/拆零倍数   → package_unit_ratio
拆零单位        → split_unit_code
警戒线最高值    → item_max_quantity
警戒线最低值    → item_min_quantity
药品拼音码      → py_str
药品五笔码      → wb_str
商品拼音码      → merchandise_py_str
商品五笔码      → merchandise_wb_str
69码            → drug69_code
药品状态        → status_enum
启用标记        → active_flag
基药标识        → basic_flag
儿童用药标志    → children_flag
自制药标识      → self_flag
限制使用        → restricted_flag
抗生素          → antibiotic_flag
处方标志        → rx_flag
医保对码        → yb_match_flag
医保等级        → chrgitm_lv
药品说明        → medicationdefinition
DDD值           → ddd_code
DDD单位         → ddd_unit_code
限制使用范围    → restricted_scope
```

---

## 四、技术方案

### 4.1 文件结构

严格参照现有模块（`basic/department`）的目录组织，但与 `basic` 平级、独立放置：

```
src/apis/basicdata/
  type.ts              ← 类型定义（MedicationCatalogRecord / Query / SavePayload / FormOptions）
  index.ts             ← API 函数（list / get / create / update / delete / setEnabled / batchOperate）
  mock.ts              ← Mock 数据

src/views/basicdata/
  Index.vue            ← 主页面（SearchPanel + ActionToolbar + DataTablePanel）
  constants.ts         ← 下拉选项、状态映射、工具栏配置
  adapter.ts           ← 表单数据适配（Record ↔ FormModel 转换）
  type.ts              ← 页面局部类型（FormModel、Filter 类型）
  components/
    MedicationCatalogEditDialog.vue   ← 新增/编辑弹窗（分区表单）

src/router/main/basicdata/
  index.ts             ← 新建路由文件（自动被 glob 扫描加载）

src/config/
  frontend-menus.ts    ← 在"基础数据"分组下追加"药品目录"菜单项
```

### 4.2 API 层设计

#### type.ts

```typescript
/** 药品目录查询参数 */
export interface MedicationCatalogQuery {
  keyword?: string          // 关键词（名称或编码）
  name?: string             // 药品名称
  categoryCode?: string     // 药品分类
  enabled?: boolean | null  // 启用状态
  page: number
  pageSize: number
}

/** 药品目录记录（列表展示用） */
export interface MedicationCatalogRecord {
  id: string
  busNo: string             // 药品编码（如 MD0035）
  name: string              // 通用名称
  merchandiseName: string   // 商品名称
  categoryCode: string      // 药品分类编码
  categoryName: string      // 药品分类名称（展示用）
  totalVolume: string       // 规格
  activeFlag: boolean       // 启用状态
  manufacturerText: string  // 生产厂家
  updatedAt: string
}

/** 药品目录详情（编辑表单用，含全部字段） */
export interface MedicationCatalogDetail {
  id?: string
  busNo?: string
  name: string
  merchandiseName: string
  categoryCode: string
  manufacturerText: string
  manufacturerId?: string
  supplyId: string
  approvalNumber?: string
  nationalDrugCode?: string
  ybNo?: string
  // ... 完整字段参照数据库表
  activeFlag: boolean
}

/** 新增/保存参数 */
export interface MedicationCatalogSavePayload {
  id?: string
  name: string
  merchandiseName: string
  categoryCode: string
  // ... 与 Detail 对应
}

/** 分页结果 */
export interface MedicationCatalogPageResult {
  rows: MedicationCatalogRecord[]
  total: number
}

/** 表单选项数据 */
export interface MedicationCatalogFormOptions {
  categoryOptions: SelectOption[]
  supplierOptions: SelectOption[]
  financialTypeOptions: SelectOption[]
  propertyOptions: SelectOption[]
  doseUnitOptions: SelectOption[]
  doseFormOptions: SelectOption[]
  methodOptions: SelectOption[]
  rateOptions: SelectOption[]
  // ... 各下拉选项
}
```

#### index.ts

```typescript
import { mockListMedicationCatalog, mockGetMedicationCatalogDetail, ... } from './mock'

/** 分页查询药品目录 */
export const listMedicationCatalog = (query: MedicationCatalogQuery) =>
  mockListMedicationCatalog(query)

/** 获取药品详情 */
export const getMedicationCatalogDetail = (id: string) =>
  mockGetMedicationCatalogDetail(id)

/** 新增药品 */
export const createMedicationCatalog = (data: MedicationCatalogSavePayload) =>
  mockCreateMedicationCatalog(data)

/** 更新药品 */
export const updateMedicationCatalog = (id: string, data: MedicationCatalogSavePayload) =>
  mockUpdateMedicationCatalog(id, data)

/** 设置启用/停用 */
export const setMedicationCatalogEnabled = (id: string, enabled: boolean) =>
  mockSetMedicationCatalogEnabled(id, enabled)

/** 批量启用/停用 */
export const batchSetMedicationCatalogEnabled = (ids: string[], enabled: boolean) =>
  mockBatchSetMedicationCatalogEnabled(ids, enabled)

/** 批量删除 */
export const batchDeleteMedicationCatalog = (ids: string[]) =>
  mockBatchDeleteMedicationCatalog(ids)

/** 删除单个 */
export const removeMedicationCatalog = (id: string) =>
  mockRemoveMedicationCatalog(id)

/** 获取表单下拉选项 */
export const loadMedicationCatalogFormOptions = () =>
  mockLoadMedicationCatalogFormOptions()
```

### 4.3 页面层设计

#### Index.vue 布局

```
┌─────────────────────────────────────────────┐
│ SearchPanel                                  │
│  [关键词] [药品名称] [药品分类▼] [搜索][重置] │
├─────────────────────────────────────────────┤
│ ActionToolbar                                │
│  [新增] [批量启用] [批量停用] [删除]  [刷新]  │
├─────────────────────────────────────────────┤
│ DataTablePanel                               │
│  ☐ 序号  编码    项目名    规格   操作        │
│  ☐ 1    MD0035  党参      饮    编辑/停用/删除│
│  ☐ 2    MD0034  茯苓      饮    编辑/停用/删除│
│  ...                                         │
│  [上一页] [1] [2] [下一页]  10条/页  跳至_页  │
└─────────────────────────────────────────────┘
```

- 使用 `SearchPanel` 组件（与 department 模块一致）
- 使用 `ActionToolbar` 组件，增加批量操作按钮
- 使用 `DataTablePanel` 组件，增加复选框列支持批量选择
- 使用 `RowActionGroup` 组件处理行操作
- 使用 `StatusDisplay` 组件显示启用/停用状态

#### MedicationCatalogEditDialog.vue

弹窗使用 `MyDialog` 组件（与 department 一致），表单分 4 个区域用 `el-row`/`el-col` 布局：

```
┌─────────────── 新增药品 ────────────────────┐
│                                              │
│  ▎基础信息                                    │
│  通用名称*    [________]    商品名称*  [____]│
│  药品分类*    [下拉___▼]    生产厂家*  [____]│
│  供应商*      [下拉___▼]    批准文号    [____]│
│  贯标国家编码 [________]    医保编码    [____]│
│  财务类型*    [下拉___▼]    医保等级    [下拉]│
│  病案结算类型 [下拉___▼]    医保类别    [下拉]│
│  □ 医保对码                                  │
│                                              │
│  ▎临床与规格信息                              │
│  药品性质*    [下拉___▼]    规格*       [____]│
│  剂量单位*    [下拉___▼]    拆零倍数*   [数值]│
│  剂型*        [下拉___▼]    用量限定    [____]│
│  默认用法     [下拉___▼]    默认频次    [下拉]│
│  单次剂量     [数值___]    单次最大剂量 [____]│
│  □皮试  □注射  限制使用范围 [____]            │
│  DDD值        [________]    DDD单位     [____]│
│                                              │
│  ▎价格与单位信息                              │
│  采购单位*    [下拉___▼]    采购倍数*   [数值]│
│  购入价*      [数值___]    门诊拆分属性* [下拉]│
│  库存单位*    [下拉___▼]    库存倍数*   [数值]│
│  零售价*      [数值___]    住院拆分属性* [下拉]│
│  包装单位*    [下拉___▼]    包装倍数*   [数值]│
│  拆零单位*    [下拉___▼]    最高零售价   [数值]│
│  警戒线最高   [数值___]    警戒线最低   [数值]│
│                                              │
│  ▎编码与标志                                  │
│  药品拼音码   [________]    药品五笔码   [____]│
│  商品拼音码   [________]    商品五笔码   [____]│
│  69码         [________]    药品状态     [下拉]│
│  □基药  □儿童  □自制药  □活性  □限制  □抗生素 │
│  □处方药                                      │
│                                              │
│         [从国家目录选择]  [保存]  [取消]      │
└──────────────────────────────────────────────┘
```

### 4.4 路由注册

新建 `src/router/main/basicdata/index.ts`（会被 `import.meta.glob` 自动扫描加载）：

```typescript
{
  path: '/basicdata',
  name: 'BasicData',
  meta: { label: '药品目录' },
  component: () => import('@/views/basicdata/Index.vue')
}
```

### 4.5 菜单注册

在 `src/config/frontend-menus.ts` 中新增"基础数据"分组（`frontend-basicdata`），或追加到已有分组：

```typescript
{
  id: 'frontend-basicdata',
  text: '基础数据',
  icon: 'IconMenu',
  children: [
    {
      id: 'frontend-basicdata-medication-catalog',
      pid: 'frontend-basicdata',
      text: '药品目录',
      icon: 'IconTickets',
      href: '/basicdata'
    }
  ]
}
```

---

## 五、实施计划

### 阶段一：基础 CRUD（预计工作量：1-2天）

1. **API 层** — 创建 `type.ts`、`index.ts`、`mock.ts`
   - 定义 `MedicationCatalogRecord`、`MedicationCatalogQuery`、`MedicationCatalogSavePayload` 等类型
   - 实现分页查询、详情、新增、更新、删除、启停用、批量操作的 Mock
   - 准备 15-20 条测试数据（中西药混合）

2. **列表页** — 创建 `Index.vue`、`constants.ts`、`type.ts`
   - 搜索栏（关键词、药品名称、药品分类）
   - 工具栏（新增、批量启用、批量停用、删除、刷新）
   - 数据表格（复选框、序号、编码、名称、规格、状态、操作）
   - 分页

3. **路由 + 菜单** — 注册路由和菜单项

### 阶段二：编辑弹窗（预计工作量：2-3天）

4. **编辑弹窗** — 创建 `MedicationCatalogEditDialog.vue`、`adapter.ts`
   - 四区域分区表单（基础信息 / 临床规格 / 价格单位 / 编码标志）
   - 必填校验（20+ 必填字段）
   - 下拉选项加载
   - 新增/编辑模式切换
   - 拼音码/五笔码自动生成（可选用 `pinyin-pro` 库）

### 阶段三：增强功能（预计工作量：1天）

5. **批量操作** — 复选框选择 + 批量启停用/删除
6. **详情查看** — 点击药品名称链接查看详情（只读模式）
7. **"从国家目录选择"** — 预留入口，弹窗选择国家药品目录自动填充

### 阶段四：联调与优化（预计工作量：1天）

8. 后端接口对接（替换 Mock）
9. 表单校验完善
10. 性能优化（防抖搜索、分页大小）

---

## 六、技术要点

### 6.1 复用现有组件

| 组件 | 用途 | 来源 |
|------|------|------|
| `SearchPanel` | 搜索栏 | `@/components/common/SearchPanel.vue` |
| `ActionToolbar` | 工具栏 | `@/components/common/ActionToolbar.vue` |
| `DataTablePanel` | 数据表格 | `@/components/common/DataTablePanel.vue` |
| `RowActionGroup` | 行操作 | `@/components/common/RowActionGroup.vue` |
| `StatusDisplay` | 状态展示 | `@/components/common/StatusDisplay.vue` |
| `MyDialog` | 弹窗容器 | `@/components/mydialog/MyDialog.vue` |

### 6.2 拼音码自动生成

项目已集成 `pinyin-pro` 库，可在通用名称输入时自动生成拼音码和五笔码：

```typescript
import { pinyin } from 'pinyin-pro'
// 药品拼音码
const pyStr = pinyin(name, { toneType: 'none', type: 'array' }).join('')
```

### 6.3 竞态防护

参照 department 模块的 `requestSequence` 模式，防止快速切换查询条件时旧请求覆盖新结果。

### 6.4 单位换算联动

5 类单位（剂量/采购/库存/包装/拆零）之间存在换算倍数关系，编辑时需要实现联动校验：拆零单位为基准 1，其他单位的倍数必须 ≥ 1。

---

## 七、风险与注意事项

1. **字段数量多** — 表单 50+ 字段，需合理分区避免用户体验差，建议用折叠面板或分步表单
2. **下拉选项数据源** — 药品分类、剂型、用法、频次等下拉选项需后端提供字典接口，Mock 阶段先内置
3. **价格字段精度** — 价格使用 `DECIMAL(18,2)`，前端数值输入框需设置 `precision=2`、`min=0`
4. **单位换算一致性** — 5 类单位的换算倍数必须逻辑自洽，建议后端校验
5. **权限控制** — 批量删除、停用等操作需后端权限校验，前端仅控制按钮可见性
