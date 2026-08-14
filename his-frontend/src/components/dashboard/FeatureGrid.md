# FeatureGrid 常用功能组件

展示 Dashboard 中的快捷功能入口网格。

## 特性
- **响应式网格**：自动对齐的功能图标展示。
- **浮入动画**：支持交错的向上浮入入场效果。
- **悬停反馈**：鼠标悬停时有向上浮动和投影加深效果。
- **纯展示模式**：目前根据需求去掉了点击跳转功能。

## 类型
- `items` 使用的模型为 [FeatureItem](./type.ts)

## Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| items | 功能项列表 | FeatureItem[] | - |
| delayOffset | 初始动画延迟 (ms) | number | 100 |
| delayStep | 各项之间的动画步长 (ms) | number | 30 |

## 使用示例
```vue
<template>
  <FeatureGrid :items="quickLinks" />
</template>

<script setup lang="ts">
import FeatureGrid from '@/components/dashboard/FeatureGrid.vue'
import type { FeatureItem } from '@/components/dashboard/type'
import { PieChart } from '@element-plus/icons-vue'

const quickLinks: FeatureItem[] = [
  { name: '报表统计', desc: '系统数据可视化分析', icon: PieChart, path: '/sample/echarts', bgColor: '#409eff' }
]
</script>
```
