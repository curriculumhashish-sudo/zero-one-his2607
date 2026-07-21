# DateRangeField

## 组件定位

`DateRangeField` 是受控日期范围输入组件，支持普通日期、日期时间、未来日期限制和最大跨度校验。

组件不绑定具体业务字段，页面通过 `v-model` 传入日期范围，并通过 `change` 接收最终有效值。

## 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import type { DateRangeValue } from '@/components/common/type'

const dateRange = ref<DateRangeValue>(null)
</script>

<template>
  <DateRangeField v-model="dateRange" @change="console.log($event)" />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `modelValue` | `DateRangeValue` | `null` | 当前日期范围，建议使用 `v-model` |
| `enableTime` | `boolean` | `false` | 是否启用时分秒，支持 `v-model:enable-time` |
| `limitFuture` | `boolean` | `false` | 是否禁止选择未来日期，支持 `v-model:limit-future` |
| `maxRangeDays` | `number` | `30` | 最大日期跨度 |
| `showOptions` | `boolean` | `true` | 是否显示时间、未来日期和跨度选项 |

`DateRangeValue` 定义：

```ts
type DateRangeValue = [start: string, end: string] | null
```

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:modelValue` | `DateRangeValue` | 日期范围变化 |
| `update:enableTime` | `boolean` | 时间模式变化 |
| `update:limitFuture` | `boolean` | 未来日期限制变化 |
| `change` | `DateRangeValue` | 选择、清空或跨度修正完成后触发 |

## 紧凑用法

队列侧栏等空间较小的区域可以隐藏附加选项：

```vue
<DateRangeField
  v-model="dateRange"
  :show-options="false"
  :max-range-days="7"
  @change="handleDateChange"
/>
```

## 日期时间

```vue
<script setup lang="ts">
import { ref } from 'vue'
import type { DateRangeValue } from '@/components/common/type'

const dateRange = ref<DateRangeValue>(null)
const enableTime = ref(true)
const limitFuture = ref(true)
</script>

<template>
  <DateRangeField
    v-model="dateRange"
    v-model:enable-time="enableTime"
    v-model:limit-future="limitFuture"
  />
</template>
```

- 普通日期格式：`YYYY-MM-DD`
- 日期时间格式：`YYYY-MM-DD HH:mm:ss`
- 超过 `maxRangeDays` 时，组件自动调整结束日期并发出修正后的值

## 职责边界

组件负责日期输入和通用跨度校验，不负责：

- 查询接口调用
- 业务默认日期
- 将日期转换为具体接口参数
- 不同工作站的日期范围策略
