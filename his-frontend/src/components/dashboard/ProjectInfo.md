# ProjectInfo 项目信息组件

展示当前项目的技术栈及版本信息。

## 特性
- **描述列表**：使用 `el-descriptions` 结构化展示数据。
- **行级动画**：表格内容按行交错淡入，提升视觉体验。
- **自适应布局**：在 Dashboard 中自动填充空间以实现底部对齐。

## Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| delay | 卡片整体动画延迟 (ms) | number | 250 |

## 使用示例
```vue
<template>
  <ProjectInfo :delay="250" />
</template>

<script setup lang="ts">
import ProjectInfo from '@/components/dashboard/ProjectInfo.vue'
</script>
```
