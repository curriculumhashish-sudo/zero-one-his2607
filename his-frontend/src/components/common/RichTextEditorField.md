# RichTextEditorField

## 组件定位

`RichTextEditorField` 是基于 wangEditor 的富文本编辑器封装组件，用于表单中的富文本内容编辑和展示。

它支持受控模式、只读切换、自定义图片上传、工具栏配置等功能，自动处理编辑器生命周期和样式适配。

## 适用场景

- 表单中的富文本内容输入
- 只读模式下的富文本内容展示
- 需要自定义图片上传逻辑的富文本编辑
- 需要精简工具栏按钮的场景

## 基本用法

```vue
<script setup lang="ts">
import RichTextEditorField from '@/components/common/RichTextEditorField.vue'

const content = ref('')

async function handleUploadImage(file: File) {
  const url = await uploadFileToServer(file)
  return url
}
</script>

<template>
  <RichTextEditorField
    v-model="content"
    placeholder="请输入内容..."
    :upload-image="handleUploadImage"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `modelValue` | `string` | `''` | 受控 HTML 内容 |
| `readonly` | `boolean` | `false` | 只读模式 |
| `placeholder` | `string` | `'请输入内容...'` | 占位文字 |
| `minHeight` | `string` | `'300px'` | 编辑器最小高度（CSS 长度） |
| `toolbarKeys` | `string[]` | `[]` | 保留的工具栏按钮 |
| `excludeKeys` | `string[]` | `['uploadImage', 'uploadVideo']` | 排除的工具栏按钮 |
| `mode` | `'default' \| 'simple'` | `'default'` | 编辑器模式 |
| `uploadImage` | `(file: File) => Promise<string>` | `undefined` | 图片上传函数：返回图片 URL |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:modelValue` | `value` | 内容变更时触发（v-model） |
| `change` | `value` | 内容变更时触发 |
| `focus` | `editor` | 编辑器获得焦点时触发 |
| `blur` | `editor` | 编辑器失去焦点时触发 |
| `ready` | `editor` | 编辑器创建完成时触发 |
| `error` | `err` | 图片上传失败等错误时触发 |
| `retry` | - | 点击重试按钮时触发 |

## 插槽

当前组件没有插槽。

## 职责边界

不要在组件内写死图片上传 URL 或后端接口。页面负责提供 `uploadImage` 函数，组件只负责调用和插入图片。

编辑器样式已适配项目主题，如需深度定制，使用 `:deep()` 覆盖 wangEditor 内置样式。