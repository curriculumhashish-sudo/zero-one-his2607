# RemoteSelectField

远程搜索选择字段。适用于科室、医生、患者、药品、诊疗项目等数据量较大、选项需要由接口查询的表单字段。

组件只负责输入、加载状态和选项选择；接口地址、权限和业务查询条件由页面通过 `loadOptions` 提供。

## 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'

interface DepartmentOption {
  deptId: string
  deptName: string
  disabled?: boolean
}

const departmentId = ref<string | null>(null)
const initialDepartments = ref<DepartmentOption[]>([
  { deptId: 'D01', deptName: '心内科' }
])

async function loadDepartments(keyword: string, context: { signal: AbortSignal }) {
  const response = await fetch(`/api/departments?keyword=${encodeURIComponent(keyword)}`, {
    signal: context.signal
  })
  return (await response.json()) as DepartmentOption[]
}
</script>

<template>
  <RemoteSelectField
    v-model="departmentId"
    :load-options="loadDepartments"
    :initial-options="initialDepartments"
    value-key="deptId"
    label-key="deptName"
    placeholder="请选择科室"
  />
</template>
```

## 多选和自定义选项

```vue
<RemoteSelectField
  v-model="doctorIds"
  multiple
  :load-options="loadDoctors"
  value-key="userId"
  label-key="realName"
  :option-disabled="(doctor) => doctor.disabled"
>
  <template #option="{ option }">
    <span>{{ option.realName }} · {{ option.departmentName }}</span>
  </template>
</RemoteSelectField>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
|---|---|---|---|
| `modelValue` | `string \| number \| Array<string \| number> \| null` | `null` | 受控选择值 |
| `loadOptions` | `(keyword, { signal }) => Promise<T[]>` | 必填 | 外部选项加载函数 |
| `initialOptions` | `T[]` | `[]` | 编辑回显或默认选项 |
| `valueKey` | `string` | `value` | 选项值字段 |
| `labelKey` | `string` | `label` | 选项显示字段 |
| `multiple` | `boolean` | `false` | 是否多选 |
| `disabled` | `boolean` | `false` | 是否禁用 |
| `clearable` | `boolean` | `true` | 是否允许清空 |
| `filterable` | `boolean` | `true` | 是否允许输入查询 |
| `debounce` | `number` | `300` | 查询防抖毫秒数 |
| `minKeywordLength` | `number` | `0` | 发起查询的最小关键字长度 |
| `placeholder` | `string` | `请选择` | 占位文字 |
| `collapseTags` | `boolean` | `true` | 多选时是否折叠标签 |
| `maxCollapseTags` | `number` | `2` | 折叠前展示的标签数量 |
| `optionDisabled` | `(option: T) => boolean` | - | 判断选项是否禁用 |

其余 Element Plus `el-select` 属性会通过 `$attrs` 继续传递。

## Events

| 事件 | 参数 | 说明 |
|---|---|---|
| `update:modelValue` | `value` | 选择值变化 |
| `change` | `value, selected` | 返回值和当前匹配的选项对象 |
| `search` | `keyword` | 开始加载选项 |
| `loadError` | `error, keyword` | 加载失败 |
| `retry` | `keyword` | 用户点击重试 |

## Slots

| 插槽 | 参数 | 说明 |
|---|---|---|
| `option` | `{ option }` | 自定义选项内容 |
| `prefix` | - | 输入框前缀 |
| `empty` | `{ loading, error, retry }` | 自定义空态或失败状态 |

## 暴露方法

- `search(keyword)`：立即查询，不经过防抖。
- `reload()`：使用最后一次关键字重新加载。

## 边界说明

- 组件会取消上一次请求，并通过请求序号阻止旧结果覆盖新结果。
- `initialOptions` 用于编辑回显。只有 ID 没有显示对象时，页面应先提供相应初始选项。
- 组件不得直接导入科室、医生、患者或药品 API；业务组件可在外层封装查询参数。

