# EmptyState

## 组件定位

`EmptyState` 是统一空状态展示组件，用于暂无数据、暂无单据、暂无模板等场景。

它只负责文案、插画和操作区展示，不负责新增、刷新等业务逻辑。

## 适用场景

- 表格无数据
- 侧栏无记录
- 详情未选择记录
- 搜索结果为空

## 基本用法

```vue
<template>
  <EmptyState
    title="暂无候诊患者"
    description="当前筛选条件下没有可展示记录"
    action-text="新增登记"
    show-refresh
    @action="handleCreate"
    @refresh="handleRefresh"
  />
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `'暂无数据'` | 主标题 |
| `description` | `string` | `''` | 补充说明 |
| `image` | `string` | `''` | 自定义图片 URL |
| `compact` | `boolean` | `false` | 紧凑模式 |
| `actionText` | `string` | `''` | 主操作按钮文案 |
| `showRefresh` | `boolean` | `false` | 是否显示刷新按钮 |

## 事件

| 事件 | 说明 |
| --- | --- |
| `action` | 点击主操作按钮 |
| `refresh` | 点击刷新按钮 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `image` | 替换默认插画 |
| `action` | 替换整个操作区 |
| `default` | 说明文字下方追加内容 |

## 职责边界

不要在组件内写新增、刷新、跳转逻辑。页面根据事件自行处理。
