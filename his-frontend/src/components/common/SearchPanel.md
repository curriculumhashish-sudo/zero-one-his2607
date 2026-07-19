# SearchPanel

## 组件定位

`SearchPanel` 是查询区外壳组件，用于统一查询条件区域、查询按钮、重置按钮、回车查询和展开收起。

它不定义具体查询字段，也不负责清空表单。

## 适用场景

- 列表页顶部查询区
- 报表筛选区
- 配置页搜索条件区

## 基本用法

```vue
<template>
  <SearchPanel
    v-model:collapsed="collapsed"
    :model="query"
    collapsible
    label-width="90px"
    @search="loadList"
    @reset="resetQuery"
  >
    <el-row :gutter="16">
      <el-col :span="6">
        <el-form-item label="关键字">
          <el-input v-model="query.keyword" clearable />
        </el-form-item>
      </el-col>
    </el-row>
  </SearchPanel>
</template>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `loading` | `boolean` | `false` | 查询按钮加载状态 |
| `collapsible` | `boolean` | `false` | 是否支持展开收起 |
| `collapsed` | `boolean` | `false` | 当前折叠状态 |
| `labelWidth` | `string` | `'120px'` | 表单 label 宽度 |
| `submitOnEnter` | `boolean` | `true` | 是否回车查询 |
| `model` | `Record<string, any>` | `{}` | 查询表单数据 |

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `search` | `model` | 点击查询或回车时触发 |
| `reset` | 无 | 点击重置时触发 |
| `update:collapsed` | `value` | 折叠状态变化 |

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `default` | 查询字段，作用域参数：`collapsed`、`toggleCollapse` |

## 对外暴露

| 名称 | 说明 |
| --- | --- |
| `formRef` | 内部 `el-form` 引用 |
| `search` | 主动触发查询 |
| `reset` | 主动触发重置事件 |
| `toggleCollapse` | 主动切换折叠 |

## 职责边界

不要在组件内写具体查询字段、接口请求或清空逻辑。页面负责字段和重置行为。
