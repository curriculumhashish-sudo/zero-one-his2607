# PatientCard

## 组件定位

`PatientCard` 用于患者列表、候诊队列和床位列表，以卡片形式展示一条患者摘要。

组件只负责：

- 展示患者姓名、性别、年龄、就诊类别、就诊时间和科室
- 展示排队号、就诊号或患者号
- 展示支付类型、患者状态和风险提示
- 发出选中和查看事件

组件不请求接口、不维护患者列表，也不处理路由跳转和弹窗。

组件路径：

```text
src/components/patient/PatientCard.vue
```

患者模型路径：

```text
src/components/patient/type.ts
```

## 基础用法

```vue
<script setup lang="ts">
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

const patient: PatientSummary = {
  id: 'visit-1001',
  name: '张三',
  gender: '男',
  age: 38,
  patientType: 'outpatient',
  serviceType: '普通门诊',
  patientNo: 'MR20260001',
  visitNo: 'OP20260720001',
  queueNumber: 'A012',
  visitTime: '2026-07-20 09:30',
  departmentName: '心内科',
  status: '候诊中',
  statusTone: 'warning',
  paymentType: '医保'
}

function handleSelect(selectedPatient: PatientSummary) {
  console.log('选中患者', selectedPatient)
}

function handleView(selectedPatient: PatientSummary) {
  console.log('查看患者', selectedPatient)
}
</script>

<template>
  <PatientCard :patient="patient" @select="handleSelect" @view="handleView" />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 必填 | 说明 |
| --- | --- | --- | --- | --- |
| `patient` | `PatientSummary` | 无 | 是 | 当前卡片展示的患者摘要 |
| `selected` | `boolean` | `false` | 否 | 是否使用选中样式 |

`patient` 不接受 `null`。没有患者时，应由列表容器展示空状态，不要渲染空卡片。

```vue
<el-empty v-if="patients.length === 0" description="暂无患者" />
<PatientCard v-else v-for="patient in patients" :key="patient.id" :patient="patient" />
```

## 事件

| 事件 | 参数 | 触发方式 |
| --- | --- | --- |
| `select` | `PatientSummary` | 点击卡片，或聚焦卡片后按 Enter、Space |
| `view` | `PatientSummary` | 点击卡片右下角“查看”按钮 |

“查看”按钮会阻止卡片点击冒泡，因此不会同时触发 `select`。

## 患者列表选中示例

```vue
<script setup lang="ts">
import { computed, ref } from 'vue'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

const patients = ref<PatientSummary[]>([
  {
    id: '1',
    name: '张三',
    gender: '男',
    age: 38,
    queueNumber: 'A012',
    visitNo: 'OP001',
    visitTime: '2026-07-20 09:30',
    status: '候诊中',
    statusTone: 'warning'
  },
  {
    id: '2',
    name: '李四',
    gender: '女',
    age: '6岁3月',
    queueNumber: 'A013',
    visitNo: 'OP002',
    status: '已接诊',
    statusTone: 'success'
  }
])

const selectedId = ref<string | number>()
const selectedPatient = computed(
  () => patients.value.find((patient) => patient.id === selectedId.value) ?? null
)

function selectPatient(patient: PatientSummary) {
  selectedId.value = patient.id
}
</script>

<template>
  <div class="patient-list">
    <PatientCard
      v-for="patient in patients"
      :key="patient.id"
      :patient="patient"
      :selected="patient.id === selectedId"
      @select="selectPatient"
      @view="selectPatient"
    />
  </div>

  <div v-if="selectedPatient">当前患者：{{ selectedPatient.name }}</div>
</template>

<style scoped>
.patient-list {
  display: grid;
  gap: 12px;
}
</style>
```

## 查看详情示例

组件只发出 `view`，具体动作由页面决定。

### 打开详情面板

```vue
<script setup lang="ts">
import { ref } from 'vue'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

const patient: PatientSummary = {
  id: 'visit-1001',
  name: '张三',
  patientNo: 'MR20260001'
}

const detailVisible = ref(false)
const currentPatient = ref<PatientSummary | null>(null)

function openDetail(patient: PatientSummary) {
  currentPatient.value = patient
  detailVisible.value = true
}
</script>

<template>
  <PatientCard :patient="patient" @view="openDetail" />

  <el-drawer v-model="detailVisible" title="患者详情">
    <div>患者：{{ currentPatient?.name }}</div>
    <div>患者号：{{ currentPatient?.patientNo }}</div>
  </el-drawer>
</template>
```

### 跳转到详情页

```ts
import { useRouter } from 'vue-router'
import type { PatientSummary } from '@/components/patient/type'

const router = useRouter()

function viewPatient(patient: PatientSummary) {
  router.push({
    name: 'patient-detail',
    params: { id: patient.id }
  })
}
```

## 风险患者示例

```vue
<script setup lang="ts">
import type { PatientSummary } from '@/components/patient/type'

const patient: PatientSummary = {
  id: 'emergency-001',
  name: '王五',
  gender: '男',
  age: 72,
  patientType: 'emergency',
  serviceType: '急诊',
  priority: '优先',
  riskHint: '青霉素过敏，高跌倒风险',
  status: '待处置',
  statusTone: 'danger'
}
</script>

<template>
  <PatientCard :patient="patient" />
</template>
```

`riskHint` 会显示在卡片内容区。`priority` 在没有 `serviceType` 时显示在姓名右侧摘要中。

## 住院患者示例

统一模型不绑定门诊 DTO，住院数据也使用相同组件。

```ts
import type { PatientSummary } from '@/components/patient/type'

const inpatient: PatientSummary = {
  id: 'ip-1001',
  name: '赵六',
  gender: '女',
  age: 56,
  patientType: 'inpatient',
  serviceType: '08床',
  patientNo: 'ZY20260088',
  visitNo: 'IP20260088',
  visitTime: '2026-07-18 14:20',
  departmentName: '呼吸内科',
  status: '在院',
  statusTone: 'success',
  paymentType: '职工医保'
}
```

## 接口数据转换

页面层应先把接口 DTO 转为 `PatientSummary`，不要把接口字段判断写进组件。

```ts
import type { PatientSummary } from '@/components/patient/type'
import type { OutpatientDto } from '@/apis/outpatient/type'

function toPatientSummary(dto: OutpatientDto): PatientSummary {
  return {
    id: dto.encounterId,
    name: dto.patientName,
    gender: dto.genderName,
    age: dto.ageText,
    patientType: 'outpatient',
    serviceType: dto.serviceTypeName,
    patientNo: dto.medicalRecordNo,
    visitNo: dto.encounterNo,
    queueNumber: dto.queueNo,
    visitTime: dto.receptionTime,
    departmentName: dto.departmentName,
    status: dto.statusName,
    statusTone: dto.urgent ? 'danger' : 'info',
    paymentType: dto.paymentTypeName,
    riskHint: dto.riskDescription
  }
}
```

接口字段允许为空时，直接保留 `null` 或 `undefined`，不要使用无意义的字符串补位。

```ts
const patient: PatientSummary = {
  id: dto.id,
  name: dto.name || null,
  visitNo: dto.visitNo || null
}
```

## 展示规则

- 姓名为空时显示“无档案登记”
- 性别、年龄或就诊类别为空时显示 `-`
- 数字年龄自动补“岁”；包含“岁”“月”“天”的年龄文本原样显示
- 底部编号优先级为：`queueNumber`、`visitNo`、`patientNo`
- `visitTime`、`departmentName`、`riskHint` 为空时不占位
- `paymentType` 和 `status` 为空时不显示标签
- `statusTone` 未传时使用 `info`

## PatientSummary 常用字段

| 字段 | 类型 | 卡片用途 |
| --- | --- | --- |
| `id` | `string \| number` | 列表 key、事件返回值中的唯一标识 |
| `name` | `string \| null` | 患者姓名 |
| `gender` | `string \| null` | 性别文本 |
| `age` | `string \| number \| null` | 年龄文本或数字 |
| `patientType` | `PatientType \| null` | 页面区分门诊、住院、急诊；组件不据此请求数据 |
| `serviceType` | `string \| null` | 就诊类别、床位等摘要 |
| `patientNo` | `string \| null` | 患者号、病历号或住院号 |
| `visitNo` | `string \| null` | 本次就诊号 |
| `queueNumber` | `string \| number \| null` | 排队号 |
| `visitTime` | `string \| null` | 就诊或入院时间 |
| `departmentName` | `string \| null` | 科室 |
| `status` | `string \| null` | 状态标签文案 |
| `statusTone` | `PatientStatusTone` | 状态标签语义色 |
| `paymentType` | `string \| null` | 支付类型 |
| `priority` | `string \| null` | 优先级文本 |
| `riskHint` | `string \| null` | 风险提示 |

## 职责边界

应放在页面或业务层：

- 患者列表请求和分页
- 接口 DTO 到 `PatientSummary` 的转换
- 选中患者状态
- 查看详情、路由跳转和权限判断
- 状态码、支付类型等字典转换

不要放进 `PatientCard`：

- Axios 或其他接口请求
- Store 查询和修改
- 固定后端枚举值
- 弹窗、路由和医疗业务处理逻辑
