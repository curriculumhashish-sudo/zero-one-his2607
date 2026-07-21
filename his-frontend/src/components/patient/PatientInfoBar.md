# PatientInfoBar

## 组件定位

`PatientInfoBar` 用于工作站、医嘱页、病历页等业务页面顶部，持续展示当前患者摘要。

组件负责：

- 展示患者姓名、性别和年龄
- 展示患者号、就诊号、科室和就诊时间
- 展示就诊类别、状态、支付类型、优先级和风险提示
- 提供右侧操作插槽
- 未选择患者时展示明确占位

组件不请求接口、不切换患者，也不执行右侧操作。

组件路径：

```text
src/components/patient/PatientInfoBar.vue
```

患者模型路径：

```text
src/components/patient/type.ts
```

## 基础用法

```vue
<script setup lang="ts">
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
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
  visitTime: '2026-07-20 09:30',
  departmentName: '心内科',
  status: '已接诊',
  statusTone: 'success',
  paymentType: '医保'
}
</script>

<template>
  <PatientInfoBar :patient="patient" />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `patient` | `PatientSummary \| null` | `null` | 当前患者摘要 |

`patient` 为 `null` 或未传时，组件显示：

```text
当前未选择患者  请先从患者列表中选择一位患者
```

不需要在页面中重复编写患者空状态。

```vue
<PatientInfoBar :patient="selectedPatient" />
```

## 插槽

| 插槽 | 参数 | 说明 |
| --- | --- | --- |
| `actions` | `{ patient: PatientSummary }` | 右侧操作区，仅有患者且提供插槽时显示 |

插槽是可选的。没有业务操作时无需提供。

## 右侧操作示例

```vue
<script setup lang="ts">
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import type { PatientSummary } from '@/components/patient/type'

const selectedPatient: PatientSummary = {
  id: 'visit-1001',
  name: '张三',
  gender: '男',
  age: 38
}

function openHistory(patient: PatientSummary) {
  console.log('查看历史记录', patient.id)
}

function createOrder(patient: PatientSummary) {
  console.log('为患者开立医嘱', patient.id)
}
</script>

<template>
  <PatientInfoBar :patient="selectedPatient">
    <template #actions="{ patient }">
      <el-button @click="openHistory(patient)">历史记录</el-button>
      <el-button type="primary" @click="createOrder(patient)">开立医嘱</el-button>
    </template>
  </PatientInfoBar>
</template>
```

操作处理函数应使用插槽返回的 `patient`，避免闭包仍然引用上一次选中的患者。

## 与 PatientCard 联动

常见页面结构是左侧患者列表、右侧工作区顶部患者信息条。

```vue
<script setup lang="ts">
import { ref } from 'vue'
import PatientCard from '@/components/patient/PatientCard.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import type { PatientSummary } from '@/components/patient/type'

const patients = ref<PatientSummary[]>([])
const selectedPatient = ref<PatientSummary | null>(null)

function selectPatient(patient: PatientSummary) {
  selectedPatient.value = patient
}

function clearPatient() {
  selectedPatient.value = null
}
</script>

<template>
  <div class="workstation">
    <aside class="workstation__patients">
      <PatientCard
        v-for="patient in patients"
        :key="patient.id"
        :patient="patient"
        :selected="patient.id === selectedPatient?.id"
        @select="selectPatient"
        @view="selectPatient"
      />
    </aside>

    <main class="workstation__content">
      <PatientInfoBar :patient="selectedPatient">
        <template #actions>
          <el-button @click="clearPatient">取消选择</el-button>
        </template>
      </PatientInfoBar>

      <section v-if="selectedPatient">患者业务内容</section>
    </main>
  </div>
</template>
```

## 风险标签示例

`priority` 使用警告色，`riskHint` 使用危险色。

```vue
<script setup lang="ts">
import type { PatientSummary } from '@/components/patient/type'

const highRiskPatient: PatientSummary = {
  id: 'ip-1001',
  name: '李四',
  gender: '女',
  age: 76,
  patientType: 'inpatient',
  serviceType: '12床',
  patientNo: 'ZY20260088',
  departmentName: '神经内科',
  priority: '重点关注',
  riskHint: '高跌倒风险，磺胺类药物过敏',
  status: '在院',
  statusTone: 'success'
}
</script>

<template>
  <PatientInfoBar :patient="highRiskPatient" />
</template>
```

风险提示过长时会在标签内省略，鼠标悬停可查看完整文本。

## 门诊场景示例

```ts
import type { PatientSummary } from '@/components/patient/type'

const outpatient: PatientSummary = {
  id: 'encounter-001',
  name: '王五',
  gender: '男',
  age: '5岁6月',
  patientType: 'outpatient',
  serviceType: '儿科门诊',
  patientNo: 'MR20260720',
  visitNo: 'OP000120',
  visitTime: '2026-07-20 10:15',
  departmentName: '儿科',
  status: '诊疗中',
  statusTone: 'primary',
  paymentType: '自费'
}
```

## 住院场景示例

`serviceType` 可以承载床位摘要，`patientNo` 可以承载住院号。

```ts
import type { PatientSummary } from '@/components/patient/type'

const inpatient: PatientSummary = {
  id: 'inpatient-001',
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

`PatientInfoBar` 和 `PatientCard` 共用 `PatientSummary`。建议在页面或单独的 mapper 文件中完成转换。

```ts
import type { PatientSummary } from '@/components/patient/type'
import type { InpatientDto } from '@/apis/inpatient/type'

export function toInpatientSummary(dto: InpatientDto): PatientSummary {
  return {
    id: dto.id,
    name: dto.patientName,
    gender: dto.genderName,
    age: dto.age,
    patientType: 'inpatient',
    serviceType: dto.bedNo ? `${dto.bedNo}床` : null,
    patientNo: dto.inpatientNo,
    visitNo: dto.visitNo,
    visitTime: dto.admitAt,
    departmentName: dto.wardName,
    status: dto.statusName,
    statusTone: dto.discharged ? 'info' : 'success',
    paymentType: dto.insuranceType,
    priority: dto.priorityName,
    riskHint: dto.riskDescription
  }
}
```

页面使用转换后的模型：

```vue
<script setup lang="ts">
import { computed } from 'vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import type { InpatientDto } from '@/apis/inpatient/type'
import { toInpatientSummary } from './patient-mapper'

const props = defineProps<{
  patientDto: InpatientDto | null
}>()

const patient = computed(() =>
  props.patientDto ? toInpatientSummary(props.patientDto) : null
)
</script>

<template>
  <PatientInfoBar :patient="patient" />
</template>
```

## 加载状态处理

组件只区分“有患者”和“无患者”，不内置接口加载状态。页面加载数据时可以在外层使用骨架屏。

```vue
<script setup lang="ts">
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import type { PatientSummary } from '@/components/patient/type'

defineProps<{
  loading: boolean
  selectedPatient: PatientSummary | null
}>()

function refreshPatient(patientId: string | number) {
  console.log('刷新患者', patientId)
}
</script>

<template>
  <el-skeleton v-if="loading" :rows="1" animated />

  <PatientInfoBar v-else :patient="selectedPatient">
    <template #actions="{ patient }">
      <el-button @click="refreshPatient(patient.id)">刷新</el-button>
    </template>
  </PatientInfoBar>
</template>
```

不要用一个伪造的空患者对象表示加载状态：

```ts
// 不推荐
const patient = { id: '', name: '' } as PatientSummary

// 推荐
const patient = ref<PatientSummary | null>(null)
const loading = ref(true)
```

## 展示规则

- 姓名为空时显示“无档案登记”
- 性别和年龄仅在有值时展示
- 数字年龄自动补“岁”；包含“岁”“月”“天”的文本原样显示
- `patientNo`、`visitNo`、`departmentName`、`visitTime` 只在有值时展示
- `serviceType`、`status`、`paymentType` 以标签展示
- `statusTone` 未传时使用 `info`
- `priority` 使用警告标签
- `riskHint` 使用危险标签
- 小屏幕下详细信息自动换行，右侧操作区保持靠右

## PatientSummary 常用字段

| 字段 | 类型 | 信息条用途 |
| --- | --- | --- |
| `id` | `string \| number` | 患者唯一标识、插槽操作参数 |
| `name` | `string \| null` | 患者姓名 |
| `gender` | `string \| null` | 性别文本 |
| `age` | `string \| number \| null` | 年龄文本或数字 |
| `patientType` | `PatientType \| null` | 页面区分门诊、住院、急诊 |
| `serviceType` | `string \| null` | 就诊类别或床位摘要 |
| `patientNo` | `string \| null` | 患者号、病历号或住院号 |
| `visitNo` | `string \| null` | 本次就诊号 |
| `visitTime` | `string \| null` | 就诊、挂号或入院时间 |
| `departmentName` | `string \| null` | 科室或病区 |
| `status` | `string \| null` | 状态标签文案 |
| `statusTone` | `PatientStatusTone` | 状态标签语义色 |
| `paymentType` | `string \| null` | 支付或医保类型 |
| `priority` | `string \| null` | 优先级或重点关注标识 |
| `riskHint` | `string \| null` | 风险提示 |

## 职责边界

页面或业务层负责：

- 当前患者选择状态
- 患者详情请求
- 接口 DTO 转换
- 右侧按钮权限、禁用和加载状态
- 按钮点击后的路由、弹窗和接口调用
- 状态码、支付类型等字典转换

不要放进 `PatientInfoBar`：

- Axios 或其他接口请求
- 患者切换逻辑
- 医嘱、处方、病历等业务操作
- 固定后端字段和枚举判断
- 页面级权限判断
