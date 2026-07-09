# PolygonBackground 组件使用说明

`PolygonBackground.vue` 是一个基于 Vue 3 和 HTML5 Canvas 开发的动态多边形背景组件。它能够生成一组随机移动并具有碰撞反弹效果的多边形，为应用提供现代感和动态视觉体验。

## 功能特性

- **动态生成**：组件加载时会自动生成随机形状、颜色和透明度的多边形。
- **平滑动画**：使用 `requestAnimationFrame` 实现流畅的 60FPS 动画效果。
- **响应式设计**：自动监听窗口大小变化（`resize` 事件），并实时调整画布尺寸及多边形布局。
- **性能优化**：组件卸载时会自动清除动画帧和事件监听器，防止内存泄漏。
- **固定布局**：默认采用 `position: fixed`，覆盖全屏并作为底层背景（`z-index: -1`）。

## 使用方法

### 1. 基础用法

在你的 Vue 组件（如 `App.vue` 或页面组件）中直接引入并使用：

```vue
<template>
  <div class="app-container">
    <!-- 放置在最外层即可 -->
    <PolygonBackground />
    
    <main>
      <h1>欢迎使用应用</h1>
      <!-- 其他内容 -->
    </main>
  </div>
</template>

<script setup lang="ts">
import PolygonBackground from './components/background/PolygonBackground.vue'
</script>
```

### 2. 注意事项

- **层级关系**：该组件默认的 `z-index` 为 `-1`。如果你的内容背景色不透明，可能会遮挡该背景。请确保上层内容的背景设置为透明或半透明。
- **无 Props 接口**：目前该组件是一个自给自足的视觉组件，不接受外部参数。若需修改多边形数量、颜色范围等，可直接修改组件内部的 `polygonCount` 和 `colors` 常量。

## 技术细节

- **核心技术**：Vue 3 (Script Setup), TypeScript, HTML5 Canvas API。
- **关键逻辑**：
  - `initPolygons`: 初始化多边形的位置、顶点、速度和颜色。
  - `draw`: 负责每一帧的清屏、路径绘制、填充以及碰撞边界检测。
  - `handleResize`: 确保在屏幕尺寸变化时，背景能够无缝适配。

## 样式自定义

如果需要调整背景的渐变底层，可以修改组件底部的 CSS：

```css
.polygon-background {
  /* ... */
  background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
}
```
