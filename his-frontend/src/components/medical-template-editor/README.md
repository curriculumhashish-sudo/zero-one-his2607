# 📝 病历模板编辑器组件

基于 **tiptap** 构建的病历模板编辑器，支持模板信息配置、元数据变量插入、富文本编辑等功能。

---

## ✨ 组件特性

- **🎨 三栏布局**：左侧模板信息面板、中间元数据变量面板、右侧富文本编辑器
- **📊 元数据变量**：支持拖拽插入模板字段，字段分为只读和可改两种状态
- **✏️ 富文本编辑**：支持标题、粗体、斜体、下划线、列表、对齐等常用格式
- **📦 数据驱动**：支持外部传入模板数据，通过事件回调输出编辑结果
- **🔧 扩展工具**：字体、字号、行间距、段间距、缩进等样式工具
- **➕ 插入功能**：支持插入横线、链接、图片、表格等内容
- **↩️ 撤销重做**：完善的撤销/重做功能，支持多次操作回退
- **📱 响应式设计**：工具栏支持横向滚动，适应窄屏显示

---

## 📋 类型定义

### 2.1 元数据字段模型

```typescript
/**
 * 病历模板元数据字段模型
 */
export interface MedicalMetadataItem {
  /** 字段唯一标识，用于模板中变量替换 */
  key: string
  /** 字段显示名称 */
  label: string
  /** 是否只读字段 */
  readOnly: boolean
}
```

### 2.2 模板信息模型

```typescript
/**
 * 病历模板信息模型
 */
export interface MedicalTemplateInfo {
  /** 模板名称 */
  name: string
  /** 模板编码 */
  code: string
  /** 文书类型 */
  documentType: string
  /** 适用就诊类型 */
  visitType: string
  /** 使用范围 */
  scope: string
  /** 是否启用 */
  enabled: boolean
}
```

### 2.3 模板数据模型

```typescript
/**
 * 病历模板数据模型
 */
export interface MedicalTemplate {
  /** 模板基本信息 */
  info: MedicalTemplateInfo
  /** 模板HTML内容 */
  content: string
}
```

### 2.4 枚举值

```typescript
/** 文书类型选项 */
export const DOCUMENT_TYPE_OPTIONS = [
  { label: '病程记录', value: '病程记录' },
  { label: '入院记录', value: '入院记录' },
  { label: '出院记录', value: '出院记录' },
  { label: '医嘱', value: '医嘱' }
]

/** 就诊类型选项 */
export const VISIT_TYPE_OPTIONS = [
  { label: '住院', value: '住院' },
  { label: '门诊', value: '门诊' },
  { label: '急诊', value: '急诊' }
]

/** 使用范围选项 */
export const SCOPE_OPTIONS = [
  { label: '全院', value: '全院' },
  { label: '科室', value: '科室' },
  { label: '个人', value: '个人' }
]
```

---

## ⚙️ 组件属性

| 属性名 | 类型 | 必填 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| `templateInfo` | `MedicalTemplateInfo` | 否 | 默认模板信息 | 模板基本信息，支持外部传入 |
| `content` | `string` | 否 | 默认模板内容 | 模板HTML内容，支持外部传入 |
| `metadata` | `MedicalMetadataItem[]` | 否 | 默认元数据列表 | 元数据变量列表，支持外部传入 |
| `showMetadata` | `boolean` | 否 | `true` | 是否显示元数据面板 |
| `showTemplateInfo` | `boolean` | 否 | `true` | 是否显示模板信息面板 |

---

## 🎯 组件事件

| 事件名 | 参数 | 说明 |
| :--- | :--- | :--- |
| `save` | `template: MedicalTemplate` | 保存模板时触发，返回完整模板数据 |
| `cancel` | 无 | 取消编辑时触发 |
| `copy` | `template: MedicalTemplate` | 复制为新模板时触发，返回复制后的模板数据 |

---

## 🚀 使用示例

### 5.1 基本使用

```html
<template>
  <MedicalTemplateEditor
    @save="handleSave"
    @cancel="handleCancel"
    @copy="handleCopy"
  />
</template>

<script setup lang="ts">
import MedicalTemplateEditor from '@/components/medical-template-editor/MedicalTemplateEditor.vue'
import type { MedicalTemplate } from '@/components/medical-template-editor/type'

function handleSave(template: MedicalTemplate) {
  console.log('保存模板:', template)
}

function handleCancel() {
  console.log('取消编辑')
}

function handleCopy(template: MedicalTemplate) {
  console.log('复制模板:', template)
}
</script>
```

### 5.2 传入自定义数据

```html
<template>
  <MedicalTemplateEditor
    :template-info="customTemplateInfo"
    :content="customContent"
    :metadata="customMetadata"
    @save="handleSave"
  />
</template>

<script setup lang="ts">
import { ref } from 'vue'
import MedicalTemplateEditor from '@/components/medical-template-editor/MedicalTemplateEditor.vue'
import type { MedicalTemplateInfo, MedicalMetadataItem, MedicalTemplate } from '@/components/medical-template-editor/type'

const customTemplateInfo = ref<MedicalTemplateInfo>({
  name: '入院记录',
  code: 'ADMISSION_RECORD',
  documentType: '入院记录',
  visitType: '住院',
  scope: '科室',
  enabled: true
})

const customContent = ref('<h1>入院记录</h1><p>患者基本信息：</p>')

const customMetadata = ref<MedicalMetadataItem[]>([
  { key: 'patientName', label: '患者姓名', readOnly: false },
  { key: 'gender', label: '性别', readOnly: true },
  { key: 'age', label: '年龄', readOnly: true }
])

function handleSave(template: MedicalTemplate) {
  console.log('保存模板:', template)
}
</script>
```

### 5.3 使用工厂函数创建数据

```html
<template>
  <MedicalTemplateEditor
    :template-info="templateInfo"
    @save="handleSave"
  />
</template>

<script setup lang="ts">
import { ref } from 'vue'
import MedicalTemplateEditor from '@/components/medical-template-editor/MedicalTemplateEditor.vue'
import { createMedicalTemplateInfo, type MedicalTemplate } from '@/components/medical-template-editor/type'

const templateInfo = ref(createMedicalTemplateInfo({
  name: '出院记录',
  code: 'DISCHARGE_RECORD'
}))

function handleSave(template: MedicalTemplate) {
  console.log('保存模板:', template)
}
</script>
```

### 5.4 渲染模式（隐藏侧边栏）

```html
<template>
  <MedicalTemplateEditor
    :template-info="templateInfo"
    :content="content"
    :metadata="metadata"
    :show-metadata="false"
    :show-template-info="false"
  />
</template>
```

---

## 🛠️ 编辑器工具栏功能

### 工具栏顺序（从左到右）

| 分组 | 功能 | 说明 |
| :--- | :--- | :--- |
| **↩️ 撤销** | `↩` | 撤销上一步操作 |
| **↪️ 重做** | `↪` | 重做已撤销的操作 |
| **📝 基础格式** | **B** | 粗体 |
| | *I* | 斜体 |
| | <u>U</u> | 下划线 |
| **📌 标题** | H1/H2/H3 | 一级/二级/三级标题 |
| **📋 列表** | • | 无序列表 |
| | 1. | 有序列表 |
| **➕ 插入** | ➕ | 下拉菜单（横线、链接、图片、表格） |
| **🎨 对齐** | ←/—/→/⇄ | 左对齐/居中/右对齐/两端对齐 |
| **🔤 字体** | 默认字体 | 字体选择器 |
| | 12px | 字号选择器 |
| | 1.0 | 行间距选择器 |
| | ↑↓ | 段间距选择器 |
| | ↔ | 缩进选择器 |

### 插入菜单功能

| 功能 | 说明 |
| :--- | :--- |
| **横线** | 插入水平分隔线 |
| **链接** | 插入超链接（需输入URL） |
| **图片** | 插入图片（需输入图片URL） |
| **表格** | 插入表格（支持2x2、3x3等规格） |

---

## 📊 元数据使用

元数据变量可以通过拖拽方式插入到编辑器中，插入后显示为 `[患者姓名]` 格式。

- **🔵 可改字段**：使用时可以修改其值，显示为蓝色标签
- **🔘 只读字段**：使用时不可修改，通常为系统自动填充的信息，显示为灰色标签

---

## 🖼️ 模板渲染

模板保存后，使用 `renderTemplate` 函数将元数据变量替换为实际数据。

### 8.1 渲染函数

```typescript
/**
 * 渲染模板内容，将元数据变量替换为实际数据
 * @param content 模板HTML内容
 * @param data 实际数据，键为元数据key，值为要替换的内容
 * @param options 渲染选项
 * @returns 渲染后的HTML内容
 */
export function renderTemplate(
  content: string,
  data: MedicalTemplateData = {},
  options?: {
    placeholder?: string
    preserveReadOnlyStyle?: boolean
  }
): string
```

### 8.2 使用示例

```html
<template>
  <div v-html="renderedContent"></div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { renderTemplate } from '@/components/medical-template-editor/type'

const templateContent = ref('<p>患者姓名：<span data-metadata-variable data-key="patientName" data-label="患者姓名">[患者姓名]</span></p><p>性别：<span data-metadata-variable data-key="gender" data-label="性别">[性别]</span></p>')

const patientData = {
  patientName: '张三',
  gender: '男',
  age: 35,
  department: '内科'
}

const renderedContent = computed(() => {
  return renderTemplate(templateContent.value, patientData)
})
</script>
```

### 8.3 渲染结果

输入模板：
```html
<p>患者姓名：<span data-metadata-variable data-key="patientName">[患者姓名]</span></p>
<p>性别：<span data-metadata-variable data-key="gender">[性别]</span></p>
```

传入数据：
```javascript
{ patientName: '张三', gender: '男' }
```

渲染输出：
```html
<p>患者姓名：张三</p>
<p>性别：男</p>
```

### 8.4 提取模板中的变量

使用 `extractMetadataKeys` 函数提取模板中使用的所有元数据变量：

```typescript
import { extractMetadataKeys } from '@/components/medical-template-editor/type'

const content = '<span data-metadata-variable data-key="patientName">[患者姓名]</span>'
const keys = extractMetadataKeys(content)
// keys: ['patientName']
```

---

## 📁 目录结构

```
medical-template-editor/
├── MedicalTemplateEditor.vue  # 🎯 主编辑器组件
├── type.ts                    # 📋 类型定义与工具函数
├── README.md                  # 📖 组件文档
└── extensions/                # 🔧 自定义扩展
    ├── MetadataVariable.ts    # 元数据变量扩展
    └── ParagraphStyle.ts      # 段落样式扩展
```

---

## 🎮 快捷键

| 快捷键 | 功能 |
| :--- | :--- |
| `Ctrl + Z` | 撤销 |
| `Ctrl + Y` | 重做 |
| `Ctrl + B` | 粗体 |
| `Ctrl + I` | 斜体 |
| `Ctrl + U` | 下划线 |

---

## 📝 注意事项

1. **🌐 图片链接**：插入图片时请使用有效的URL地址，支持在线图片和base64编码图片
2. **🔗 链接文本**：插入链接时如果没有选中文本，会自动插入"链接文本"作为默认内容
3. **📱 窄屏适配**：工具栏支持横向滚动，当屏幕宽度不足以显示所有工具时，可左右滑动查看
4. **💾 数据安全**：建议在保存前验证模板内容的合法性，防止注入攻击
