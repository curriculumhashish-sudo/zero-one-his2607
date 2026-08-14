# SidePanelShell

## 组件定位

`SidePanelShell` 是 C09 侧栏基础外壳，用于统一侧栏的标题、搜索区、滚动主体、固定底部、加载、空状态和折叠行为。

它是 `RecordSidebar` 和 `SideNavigator` 的基础容器。

## 适用场景

- 左侧记录列表外壳
- 左侧分类树外壳
- 任意“标题 / 搜索 / 内容 / 底部”结构的侧栏

## 基本用法

```vue
<template>
  <SidePanelShell
    v-model:collapsed="collapsed"
    title="就诊记录"
    :width="300"
    :empty="records.length === 0"
    empty-description="暂无记录"
    collapsible
  >
    <template #search>
      <el-input placeholder="搜索记录" clearable />
    </template>

    <div v-for="record in records" :key="record.id">
      {{ record.name }}
    </div>

    <template #footer>
      共 {{ records.length }} 条
    </template>
  </SidePanelShell>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 标题 |
| `width` | `string \| number` | `320` | 展开宽度，数字自动转 px |
| `loading` | `boolean` | `false` | 主体加载状态 |
| `empty` | `boolean` | `false` | 是否显示空状态 |
| `emptyDescription` | `string` | `'暂无数据'` | 空状态说明 |
| `collapsible` | `boolean` | `false` | 是否显示折叠按钮 |
| `collapsed` | `boolean` | `false` | 是否折叠，折叠宽度为 `48px` |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:collapsed` | `value` | 点击展开/收起时触发 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题 |
| `search` | 搜索或筛选区 |
| `default` | 主体内容，超高时独立滚动 |
| `footer` | 固定底部区域 |

## 空状态

`empty=true` 且 `loading=false` 时，组件使用项目统一的 `EmptyState` 展示 `emptyDescription`。

## 布局要求

父容器需要有明确高度，并建议设置 `min-height: 0`，否则主体滚动区可能无法按预期工作。

## 职责边界

不要在组件内写历史查询接口、分类树接口、固定业务按钮或具体字段。
