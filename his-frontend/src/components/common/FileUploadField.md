# FileUploadField

受控文件上传字段。适用于病案附件、检查报告附件、系统配置附件和普通文件上传。

上传和删除函数由页面注入，组件不写死接口地址或服务端响应结构。

## 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import type { UploadRawFile } from 'element-plus'
import FileUploadField from '@/components/common/FileUploadField.vue'
import type { FileUploadValue } from '@/components/common/upload-type'

const files = ref<FileUploadValue[]>([])

async function uploadFile(file: UploadRawFile, context: { signal: AbortSignal; onProgress: (value: number) => void }) {
  context.onProgress(30)
  const formData = new FormData()
  formData.append('file', file)

  const response = await fetch('/api/files', {
    method: 'POST',
    body: formData,
    signal: context.signal
  })
  const result = await response.json()
  context.onProgress(100)

  return {
    uid: file.uid,
    name: file.name,
    size: file.size,
    url: result.url,
    response: result
  }
}
</script>

<template>
  <FileUploadField
    v-model="files"
    :upload="uploadFile"
    accept=".pdf,.doc,.docx"
    :max-size-m-b="10"
    :limit="3"
  />
</template>
```

说明：实际参数中的 `file` 类型为 Element Plus `UploadRawFile`，继承浏览器 `File` 并带有稳定 `uid`。

## 手动上传和已有文件

```vue
<FileUploadField
  v-model="files"
  :upload="uploadFile"
  :remove="removeFile"
  :auto-upload="false"
  tip="请上传本次就诊相关附件"
/>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
|---|---|---|---|
| `modelValue` | `FileUploadValue[]` | `[]` | 受控文件列表 |
| `upload` | `(file, context) => Promise<FileUploadValue>` | 必填 | 上传函数 |
| `remove` | `(file) => Promise<void> \| void` | - | 已上传文件删除函数 |
| `accept` | `string` | 空字符串 | 扩展名或 MIME 限制 |
| `maxSizeMB` | `number` | `20` | 单文件最大体积 |
| `limit` | `number` | `5` | 最大文件数量 |
| `multiple` | `boolean` | `true` | 是否允许多选 |
| `readonly` | `boolean` | `false` | 只能查看和下载 |
| `disabled` | `boolean` | `false` | 是否禁用 |
| `autoUpload` | `boolean` | `true` | 选择后是否立即上传 |
| `allowDuplicateName` | `boolean` | `false` | 是否允许同名文件 |
| `downloadable` | `boolean` | `true` | 是否显示下载操作 |
| `tip` | `string` | 空字符串 | 自定义提示文字 |

## FileUploadValue

```ts
interface FileUploadValue<TResponse = unknown> {
  uid: number
  name: string
  size?: number
  url?: string
  status?: 'ready' | 'uploading' | 'success' | 'fail'
  percentage?: number
  response?: TResponse
  errorMessage?: string
  raw?: UploadRawFile
}
```

## Events

| 事件 | 参数 | 说明 |
|---|---|---|
| `update:modelValue` | `files` | 文件列表变化 |
| `change` | `files` | 文件状态或列表变化 |
| `success` | `file` | 上传成功 |
| `error` | `error, file` | 上传或删除失败 |
| `remove` | `file` | 文件已移除 |
| `download` | `file` | 用户点击下载 |
| `exceed` | `files` | 选择数量超过限制 |

## Slots

| 插槽 | 参数 | 说明 |
|---|---|---|
| `trigger` | - | 自定义选择按钮 |
| `tip` | - | 自定义提示区域 |
| `file` | `{ file, retry, remove, download }` | 自定义文件行 |

## 暴露方法

- `submit()`：上传所有 ready/fail 文件。
- `retry(file)`：重试指定文件。
- `abort(uid?)`：取消指定或全部正在上传的文件。

## 边界说明

- 组件卸载时会取消仍在执行的上传请求。
- `readonly` 不允许新增和删除，但可以下载已有 URL。
- 图片裁剪、压缩、医学影像解析不属于本组件职责。
