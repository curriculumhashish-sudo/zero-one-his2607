# PatientQueuePanel

## 组件定位

`PatientQueuePanel` 是工作站左侧患者队列面板，统一组合查询、日期、空状态和患者卡片。

内部复用：

| 组件 | 用途 |
| --- | --- |
| `SearchPanel`（C01） | 关键词查询、查询按钮和重置按钮 |
| `DateRangeField`（C02） | 日期范围输入 |
| `EmptyState`（C05） | 无患者时的明确空状态 |
| `PatientCard` | 患者队列卡片 |

组件只接收页面提供的状态并发出用户意图，不请求接口、不筛选患者，也不维护当前选中患者。当前患者摘要应放在中间业务区，由页面单独组合 `PatientInfoBar`。

组件路径：

```text
src/components/patient/PatientQueuePanel.vue
```

## 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { DateRangeValue } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'

const loading = ref(false)
const selectedId = ref<string | number | null>(null)
const patients = ref<PatientSummary[]>([
  {
    id: 'visit-1001',
    name: '张三',
    gender: '男',
    age: 38,
    serviceType: '普通门诊',
    patientNo: 'MR20260001',
    visitNo: 'OP20260001',
    status: '候诊中',
    statusTone: 'warning'
  }
])

function handleSearch(keyword: string) {
  console.log('查询关键词', keyword)
}

function handleDateChange(value: DateRangeValue) {
  console.log('日期范围', value)
}

function handleRefresh() {
  console.log('刷新患者队列')
}

function handleSelect(patient: PatientSummary) {
  selectedId.value = patient.id
}
</script>

<template>
  <div class="queue-container">
    <PatientQueuePanel
      :patients="patients"
      :loading="loading"
      :selected-id="selectedId"
      @search="handleSearch"
      @date-change="handleDateChange"
      @refresh="handleRefresh"
      @select="handleSelect"
    />
  </div>
</template>

<style scoped>
.queue-container {
  width: 360px;
  height: calc(100vh - 60px);
  min-height: 0;
}
</style>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `patients` | `PatientSummary[]` | `[]` | 页面已经加载并转换好的患者摘要列表 |
| `loading` | `boolean` | `false` | 查询按钮、刷新按钮和列表加载状态 |
| `selectedId` | `string \| number \| null` | `null` | 当前选中患者 ID |

`selectedId` 只负责展示选中状态。组件不会自行修改它，页面收到 `select` 后决定是否更新。

## 事件

| 事件 | 参数 | 触发方式 |
| --- | --- | --- |
| `search` | `keyword: string` | 点击查询或在关键词输入框按 Enter |
| `dateChange` | `[start: string, end: string] \| null` | 选择或清空日期范围 |
| `refresh` | 无 | 点击刷新按钮或空状态刷新 |
| `select` | `patient: PatientSummary` | 点击患者卡片或卡片“查看” |

模板监听事件时使用 kebab-case：

```vue
<PatientQueuePanel
  @search="handleSearch"
  @date-change="handleDateChange"
  @refresh="handleRefresh"
  @select="handleSelect"
/>
```

## 页面请求示例

接口请求必须写在页面或 Store 中。

```vue
<script setup lang="ts">
import { onMounted, ref } from 'vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { DateRangeValue } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'
import { getPatientQueue } from '@/apis/station'

const patients = ref<PatientSummary[]>([])
const loading = ref(false)
const selectedId = ref<string | number | null>(null)

let keyword = ''
let dateRange: DateRangeValue = null

async function loadPatients() {
  loading.value = true
  try {
    const result = await getPatientQueue({
      keyword,
      startDate: dateRange?.[0],
      endDate: dateRange?.[1]
    })

    patients.value = result.map((item) => ({
      id: item.visitId,
      name: item.patientName,
      gender: item.genderName,
      age: item.ageText,
      patientNo: item.medicalRecordNo,
      visitNo: item.visitNo,
      status: item.statusName
    }))
  } finally {
    loading.value = false
  }
}

function searchPatients(value: string) {
  keyword = value
  void loadPatients()
}

function changeDate(value: DateRangeValue) {
  dateRange = value
  void loadPatients()
}

function selectPatient(patient: PatientSummary) {
  selectedId.value = patient.id
}

onMounted(loadPatients)
</script>

<template>
  <PatientQueuePanel
    :patients="patients"
    :loading="loading"
    :selected-id="selectedId"
    @search="searchPatients"
    @date-change="changeDate"
    @refresh="loadPatients"
    @select="selectPatient"
  />
</template>
```

示例中的 API 名称仅表示页面调用方式，不是组件依赖。

## 与 StationTriPaneLayout 组合

```vue
<script setup lang="ts">
import { computed } from 'vue'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'

const selectedPatient = computed(
  () => patients.value.find((patient) => patient.id === selectedId.value) ?? null
)
</script>

<template>
  <div class="station-page">
    <StationTriPaneLayout left-width="360px" right-width="320px">
      <template #left>
        <PatientQueuePanel
          :patients="patients"
          :loading="loading"
          :selected-id="selectedId"
          @search="searchPatients"
          @date-change="changeDate"
          @refresh="loadPatients"
          @select="selectPatient"
        />
      </template>

      <template #main>
        <PatientInfoBar :patient="selectedPatient" />
        <MainWorkspace :patient-id="selectedId" />
      </template>

      <template #right>
        <AssistantPanel :patient-id="selectedId" />
      </template>
    </StationTriPaneLayout>
  </div>
</template>

<style scoped>
.station-page {
  height: calc(100vh - 60px);
  min-width: 0;
  min-height: 0;
}
</style>
```

## 查询和重置规则

- 关键词输入框只维护输入值，不自行筛选 `patients`
- 点击查询或按 Enter 时发出 `search`
- 选择日期后立即发出 `dateChange`
- 点击重置会清空关键词和日期
- 重置后依次发出 `search('')` 和 `dateChange(null)`
- 页面收到事件后决定是否重新请求

## 选中规则

- `PatientCard` 的选中样式由 `patient.id === selectedId` 决定
- 点击卡片只发出 `select`，不会在组件内部修改 `selectedId`
- 面板不展示 `PatientInfoBar`；页面应根据 `selectedId` 计算当前患者，并在中间业务区展示摘要

## 加载和空状态

```vue
<PatientQueuePanel
  :patients="patients"
  :loading="loading"
  :selected-id="selectedId"
  @refresh="loadPatients"
/>
```

- `loading=true`：查询和刷新按钮进入加载状态，列表显示加载遮罩
- `loading=false` 且 `patients=[]`：使用 `EmptyState` 显示“暂无患者”
- 空状态中的刷新按钮同样发出 `refresh`
- `patients` 有值时：使用 `PatientCard` 渲染列表

## 日期范围

日期事件类型定义在：

```ts
import type { DateRangeValue } from '@/components/common/type'
```

类型为：

```ts
type DateRangeValue = [start: string, end: string] | null
```

默认日期格式是 `YYYY-MM-DD`。队列面板隐藏 C02 的时间和未来日期选项，只使用日期范围输入。

## 高度和滚动

组件使用 `height: 100%`，患者卡片列表独立滚动。父容器必须有明确高度和 `min-height: 0`：

```css
.queue-container {
  display: flex;
  height: 100%;
  min-height: 0;
  overflow: hidden;
}
```

不要在每个 `PatientCard` 外层添加滚动容器，否则会产生多层滚动条。

## 职责边界

组件内部负责：

- 组合 C01、C02、C05
- 渲染患者卡片
- 展示加载和空状态
- 转发搜索、日期、刷新和选择意图

页面或 Store 负责：

- 请求患者队列
- 接口 DTO 转换
- 搜索和日期参数保存
- 更新 `patients`、`loading` 和 `selectedId`
- 权限、分页和业务筛选
- 选中患者后的业务区加载

不要在 `PatientQueuePanel` 中加入：

- Axios 或其他接口请求
- 患者接诊、召回和分诊逻辑
- 医嘱、处方和收费处理
- 固定后端状态码判断
