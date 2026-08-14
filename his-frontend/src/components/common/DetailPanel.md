# DetailPanel

## 组件定位

`DetailPanel` 是右侧详情面板外壳，用于统一详情区标题、顶部操作、内容滚动、空状态、加载状态和底部操作。

它不负责查询详情、不保存数据，也不转换保存参数。

## 适用场景

- 左侧列表、右侧详情的主从页面
- 单据详情页
- 编辑态和只读态切换的详情区域
- 未选中记录时显示空状态的右侧面板

## 基本用法

```vue
<template>
  <DetailPanel
    title="就诊详情"
    :readonly="readonly"
    :empty="!currentRecord"
    @edit="readonly = false"
    @save="handleSave"
    @cancel="readonly = true"
  >
    <DetailSummary :items="summaryItems" />
  </DetailPanel>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 标题 |
| `loading` | `boolean` | `false` | 加载状态 |
| `readonly` | `boolean` | `true` | 是否只读 |
| `empty` | `boolean` | `false` | 是否显示空状态 |
| `emptyDescription` | `string` | `'请选择一条记录'` | 空状态说明 |
| `stickyFooter` | `boolean` | `true` | 底部操作是否吸底 |

## 事件

| 事件 | 说明 |
| --- | --- |
| `edit` | 点击默认编辑按钮 |
| `save` | 点击默认保存按钮 |
| `cancel` | 点击默认取消按钮 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `title` | 自定义标题 |
| `actions` | 自定义顶部操作，作用域参数：`readonly` |
| `default` | 详情主体，作用域参数：`readonly` |
| `footer` | 自定义底部操作，作用域参数：`readonly` |

## 空状态

`empty=true` 且 `loading=false` 时，组件使用项目统一的 `EmptyState` 展示 `emptyDescription`。

## 职责边界

不要在组件内写详情接口、保存接口、路由跳转或业务字段。它只提供详情区域的结构。
