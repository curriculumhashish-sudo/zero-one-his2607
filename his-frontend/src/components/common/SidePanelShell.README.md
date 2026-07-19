# SidePanelShell（C09）

`SidePanelShell` 是左侧栏外壳组件，用来统一侧栏的标题、搜索区、滚动内容区、底部区、加载状态、空状态和折叠行为。

组件只处理布局和通用状态，不应在内部调用业务接口，也不应写入患者、单据、分类树等具体业务字段。

组件路径：

```text
src/components/common/SidePanelShell.vue
```

## 1. 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import SidePanelShell from '@/components/common/SidePanelShell.vue'

const collapsed = ref(false)
const loading = ref(false)
const records = ref([
  { id: 1, name: '门诊记录一' },
  { id: 2, name: '门诊记录二' }
])
</script>

<template>
  <SidePanelShell
    v-model:collapsed="collapsed"
    title="就诊记录"
    :width="320"
    :loading="loading"
    :empty="records.length === 0"
    empty-description="暂无就诊记录"
    collapsible
  >
    <template #search>
      <el-input placeholder="搜索记录" clearable />
    </template>

    <div v-for="record in records" :key="record.id">
      {{ record.name }}
    </div>

    <template #footer>
      共 {{ records.length }} 条记录
    </template>
  </SidePanelShell>
</template>
```

## 2. Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 默认标题；提供 `title` 插槽后可自定义 |
| `width` | `string \| number` | `320` | 展开宽度；数字会自动转换为 `px` |
| `loading` | `boolean` | `false` | 是否显示内容区加载状态 |
| `empty` | `boolean` | `false` | 是否显示空状态 |
| `emptyDescription` | `string` | `'暂无数据'` | 空状态说明 |
| `collapsible` | `boolean` | `false` | 是否显示折叠按钮 |
| `collapsed` | `boolean` | `false` | 当前是否折叠；折叠后宽度为 `48px` |

## 3. 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:collapsed` | `value: boolean` | 用户点击折叠/展开按钮时触发 |

建议使用：

```vue
<SidePanelShell v-model:collapsed="collapsed" collapsible />
```

等价于：

```vue
<SidePanelShell
  :collapsed="collapsed"
  collapsible
  @update:collapsed="collapsed = $event"
/>
```

## 4. 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题内容 |
| `search` | 搜索、筛选或工具区域 |
| `default` | 中间主体内容，内容过高时独立滚动 |
| `footer` | 固定在侧栏底部的操作或说明区域 |

标题插槽示例：

```vue
<SidePanelShell :width="280">
  <template #title>
    <span>待处理记录</span>
    <el-tag size="small">12</el-tag>
  </template>

  记录列表
</SidePanelShell>
```

## 5. 状态规则

- `loading=true` 时，主体内容暂时不渲染并显示加载遮罩。
- `loading=false` 且 `empty=true` 时，显示 `el-empty`。
- `loading=false` 且 `empty=false` 时，渲染默认插槽。
- 折叠时只保留折叠栏，不渲染搜索区、主体区和底部区。

## 6. 布局要求

组件使用 `height: 100%`。如果需要主体区域正确独立滚动，父容器必须具有明确高度：

```css
.page-layout {
  display: flex;
  height: calc(100vh - 60px);
  min-height: 0;
}
```

推荐父容器同时设置 `min-height: 0`，避免 Flex 子元素把页面撑高。

## 7. 职责边界

适合放入：

- 搜索输入框
- 分类树
- 记录列表
- 通用加载和空状态
- 底部统计或通用操作

不应放入组件内部：

- 历史记录查询接口
- 分类树接口
- 固定业务按钮
- 患者、单据等具体字段

