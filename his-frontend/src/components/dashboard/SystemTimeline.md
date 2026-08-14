# SystemTimeline 系统动态组件

展示系统最近的操作日志或动态。

## 特性
- **时间轴布局**：使用 `el-timeline` 清晰展示时间顺序。
- **滑入动画**：各条目从右侧交错滑入。
- **高亮反馈**：悬停时标题颜色切换。

## 类型
- `items` 使用的模型为 [Activity](./type.ts)

## Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| items | 动态条目列表 | Activity[] | - |
| delayOffset | 初始动画延迟 (ms) | number | 150 |
| delayStep | 各项之间的动画步长 (ms) | number | 80 |

## 使用示例
```vue
<template>
  <SystemTimeline :items="items" />
</template>

<script setup lang="ts">
import SystemTimeline from '@/components/dashboard/SystemTimeline.vue'
import type { Activity } from '@/components/dashboard/type'

const items: Activity[] = [
  {
    title: '部署提醒',
    content: '生产环境 v2.5.1 版本部署完成',
    time: '10:30',
    type: 'primary',
    color: '#409eff'
  }
]
</script>
```
