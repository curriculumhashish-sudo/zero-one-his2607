# PrintActionPanel

## 组件定位

`PrintActionPanel` 是打印操作控制面板，负责模板选择、打印机选择、预览/打印状态以及失败重试。组件不创建业务模板、不读取单据接口，也不绑定 hiprint、浏览器打印或本地打印客户端；实际预览和打印函数由页面注入。

## 基本用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import PrintActionPanel from '@/components/common/PrintActionPanel.vue'
import type {
  PrintActionContext,
  PrinterOption,
  PrintTemplateOption
} from '@/components/common/PrintActionPanel.vue'

const templateId = ref<string | number | null>('outpatient-record')
const printerName = ref<string | null>('office-a4')

const templates: PrintTemplateOption[] = [
  { value: 'outpatient-record', label: '门诊病历模板' },
  { value: 'prescription', label: '处方笺模板' }
]

const printers: PrinterOption[] = [
  { value: 'office-a4', label: '诊室 A4 打印机', status: 'online' },
  { value: 'pharmacy', label: '药房打印机', status: 'offline' }
]

async function preview(context: PrintActionContext) {
  // 调用页面已有的预览实现
  return createPreview(context.templateValue)
}

async function print(context: PrintActionContext) {
  // 调用页面已有的 hiprint、浏览器打印或客户端打印实现
  return printDocument(context.templateValue, context.printerValue)
}
</script>

<template>
  <PrintActionPanel
    v-model:template-value="templateId"
    v-model:printer-value="printerName"
    :templates="templates"
    :printers="printers"
    :preview="preview"
    :print="print"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `templateValue` | `string \| number \| null` | `null` | 当前模板，支持 `v-model:template-value` |
| `printerValue` | `string \| null` | `null` | 当前打印机，支持 `v-model:printer-value` |
| `templates` | `PrintTemplateOption[]` | `[]` | 模板列表 |
| `printers` | `PrinterOption[]` | `[]` | 打印机列表 |
| `preview` | `PrintActionHandler` | - | 页面注入的预览函数 |
| `print` | `PrintActionHandler` | - | 页面注入的打印函数 |
| `loadPrinters` | `PrinterLoader` | - | 可选的打印机加载函数 |
| `loading` | `boolean` | `false` | 页面外部加载状态 |
| `disabled` | `boolean` | `false` | 整体禁用 |
| `requirePrinter` | `boolean` | `true` | 打印前是否必须选择打印机 |
| `autoLoadPrinters` | `boolean` | `true` | 挂载后是否调用 `loadPrinters` |
| `title` | `string` | `打印设置` | 面板标题 |
| `templateLabel` | `string` | `打印模板` | 模板字段标签 |
| `printerLabel` | `string` | `打印机` | 打印机字段标签 |
| `previewText` | `string` | `打印预览` | 预览按钮文字 |
| `printText` | `string` | `开始打印` | 打印按钮文字 |

`PrintActionContext` 包含 `templateValue`、`printerValue` 和 `signal`。页面的异步实现应监听 `signal`，组件卸载或任务取消后停止后续工作。

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:templateValue` | `value` | 模板变化 |
| `update:printerValue` | `value` | 打印机变化 |
| `preview` | `context` | 开始执行预览 |
| `print` | `context` | 开始执行打印 |
| `success` | `type, result` | 预览或打印成功 |
| `error` | `error, type` | 预览、打印或打印机加载失败 |
| `retry` | `type` | 用户重试失败操作 |
| `printersLoaded` | `printers` | 打印机加载完成 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `header` | 自定义标题区域，参数 `{ busy }` |
| `fields` | 完全替换模板和打印机字段 |
| `actions` | 自定义操作按钮，提供 `preview`、`print`、`canPreview`、`canPrint`、`busy` |

## 暴露方法

- `preview()`：执行预览。
- `print()`：执行打印。
- `reloadPrinters()`：重新加载打印机。
- `retry()`：重试最近一次失败操作。
- `abort()`：取消当前异步操作。

## 职责边界

- 模板、打印机、单据数据和权限由页面负责。
- 组件不写死打印地址、模板 JSON 或打印客户端。
- 组件只串联选择、调用、加载、成功、失败和重试状态。
