# ResourceMonitor 资源监控组件

实时展示系统资源（CPU、内存、磁盘、网络）的占用情况。

## 特性
- **动态增长**：进度条和百分比数字支持从 0 开始的快速增长动画。
- **状态可视化**：使用不同颜色区分资源类型。
- **精简设计**：横向排布，节省空间。

## 类型
- `items` 使用的模型为 [ResourceItem](./type.ts)

## Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| items | 资源项列表 | ResourceItem[] | - |

## 使用示例
```vue
<template>
  <ResourceMonitor :items="resources" />
</template>

<script setup lang="ts">
import ResourceMonitor from '@/components/dashboard/ResourceMonitor.vue'
import type { ResourceItem } from '@/components/dashboard/type'
import { Cpu } from '@element-plus/icons-vue'

const resources: ResourceItem[] = [
  { name: 'CPU', percentage: 28, displayValue: 0, color: '#409eff', icon: Cpu }
]
</script>
```
