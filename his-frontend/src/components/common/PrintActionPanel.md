# PrintActionPanel

## 组件定位

`PrintActionPanel` 是 PDF 文件预览组件，基于 iframe 实现浏览器原生 PDF 渲染。

它支持 URL 和 Blob 两种数据源，自动处理加载状态、错误状态和空状态，提供下载、新窗口打开等操作按钮，并在组件卸载时清理资源。

> **注意**：当前组件代码与 `PdfPreviewPanel` 完全相同，疑似代码复制未进行功能扩展。建议根据实际打印需求进行差异化开发，如添加打印按钮、页面设置等功能。

## 适用场景

- 文档详情页的 PDF 在线预览
- 表单中的 PDF 文件预览
- 需要支持 Blob 对象预览的场景
- 需要自定义加载、错误、空状态展示的场景

## 基本用法

```vue
<script setup lang="ts">
import PrintActionPanel from '@/components/common/PrintActionPanel.vue'

const pdfUrl = ref('https://example.com/document.pdf')
const pdfBlob = ref<Blob | null>(null)

function handleLoad(event: { url: string; fileName: string }) {
  console.log('PDF 加载完成:', event)
}

function handleError(error: Error) {
  console.error('PDF 加载失败:', error)
}
</script>

<template>
  <!-- 方式一：使用 URL -->
  <PrintActionPanel
    :source="pdfUrl"
    file-name="病历文档.pdf"
    height="600px"
    @load="handleLoad"
    @error="handleError"
  />

  <!-- 方式二：使用 Blob -->
  <PrintActionPanel
    :source="pdfBlob"
    file-name="报告.pdf"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `source` | `string \| Blob \| null` | `null` | PDF 源：URL 字符串或 Blob 对象 |
| `fileName` | `string` | `'document.pdf'` | 文件名（用于下载和显示） |
| `height` | `string \| number` | `'600px'` | 预览高度（CSS 长度） |
| `showToolbar` | `boolean` | `true` | 是否显示工具栏 |
| `downloadable` | `boolean` | `true` | 是否允许下载 |
| `openable` | `boolean` | `true` | 是否允许在新窗口打开 |
| `timeout` | `number` | `30000` | 加载超时时间（毫秒） |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `load` | `{ url, fileName }` | PDF 加载完成时触发 |
| `error` | `error` | 加载失败或超时时触发 |
| `retry` | - | 点击重试按钮时触发 |
| `download` | `{ url, fileName }` | 点击下载按钮时触发 |
| `open` | `{ url, fileName }` | 点击新窗口按钮时触发 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `loading` | 自定义加载状态内容 |
| `error` | 自定义错误状态内容 |
| `empty` | 自定义空状态内容 |
| `toolbar` | 自定义工具栏内容 |

## 暴露方法

| 方法 | 返回值 | 说明 |
| --- | --- | --- |
| `reload` | `void` | 重新加载当前 PDF |
| `getPdfSrc` | `string \| null` | 获取当前 PDF 源 URL |
| `getLoading` | `boolean` | 获取加载状态 |
| `retry` | `void` | 清除错误并重试 |

## 职责边界

不要在组件内写死 PDF 文件路径或后端接口。页面负责提供 `source`（URL 或 Blob），组件只负责预览和基础操作。

当使用 Blob 作为源时，组件会自动创建 Object URL 并在卸载时释放，无需手动清理。

**代码质量提示**：当前组件代码与 `PdfPreviewPanel` 完全重复，建议考虑以下优化方向：
- 如果两者功能一致，可考虑删除其中一个，统一使用 `PdfPreviewPanel`
- 如果需要打印专属功能，建议扩展当前组件，添加打印按钮、页面设置、打印预览等功能