# ErrorPage 错误页面组件

`ErrorPage` 是一个通用的状态展示组件，主要用于展示 403、404、500 等错误页面。它集成了动态背景、毛玻璃卡片样式以及返回首页的导航功能，确保全站错误页面的视觉风格高度统一。

## 1 功能特性

- **视觉统一**: 采用与登录页一致的 [PolygonBackground](../../components/background/PolygonBackground.vue) 动态背景。
- **响应式设计**: 自动适应不同屏幕尺寸，保持卡片居中。
- **毛玻璃效果**: 核心内容展示在半透明的模糊卡片中，具有现代感。
- **内置导航**: 包含一个“返回首页”按钮，点击后自动跳转至 `/home`。
- **性能优化**: 针对滚动事件进行了优化（`touch-action: none`），减少浏览器警告。

## 2 使用步骤

### 2.1 导入组件

在 Vue 页面中导入并使用该组件：

```html
<script setup lang="ts">
import ErrorPage from '@/components/status/ErrorPage.vue'
</script>

<template>
  <ErrorPage 
    code="404" 
    title="页面未找到" 
    description="抱歉，你访问的页面不存在或已被移除" 
  />
</template>
```

## 3 属性说明 (Props)

| 属性名 | 类型 | 是否必填 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| `code` | `string | number` | 是 | - | 错误状态码，如 "404", "403" |
| `title` | `string` | 是 | - | 错误标题，如 "禁止访问" |
| `description` | `string` | 是 | - | 详细的错误描述文案 |

## 4 样式定制

该组件深度集成了项目的全局 CSS 变量体系（详见 [base.css](../../assets/base.css)）：
- **--primary-gradient**: 用于状态码文字和按钮的渐变色。
- **--glass-bg / --glass-border / --glass-blur**: 用于控制毛玻璃卡片的质感。
- **--title-color / --desc-color**: 控制文字颜色，确保全站一致。

## 5 注意事项

1. **类型安全**: Props 采用了 TypeScript Interface 定义，支持严格的类型检查。
2. **路由依赖**: 组件内部使用了 `vue-router` 的 `useRouter` 进行跳转，确保在路由环境下使用。
3. **背景层级**: 背景组件使用了 `z-index: -1`，确保内容卡片始终在最上层。
