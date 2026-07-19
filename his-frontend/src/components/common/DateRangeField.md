# DateRangeField

## 组件定位

`DateRangeField` 是日期范围输入示例组件，用于展示日期范围、时间开关、未来日期限制和最大跨度控制。

当前实现内部持有日期值，适合示例页展示；如果要在业务页面复用，建议后续补 `v-model`、`disabledDate`、`maxRangeDays` 等受控 props。

## 适用场景

- 查询区里的开始日期到结束日期
- 需要限制最大跨度的日期范围选择
- 需要在日期和日期时间之间切换的场景

## 基本用法

```vue
<template>
  <el-form-item label="就诊日期">
    <DateRangeField />
  </el-form-item>
</template>
```

## 当前能力

- 支持 `daterange` 和 `datetimerange` 切换
- 支持是否限制未来日期
- 支持最大跨度校验，默认 30 天
- 超过最大跨度时自动修正结束日期并提示

## Props

当前组件没有对外 props。

## 事件

当前组件没有对外事件。

## 插槽

当前组件没有插槽。

## 职责边界

不要在组件内绑定具体业务字段。业务页面复用前，建议先改造成受控组件，由页面传入日期值并接收变更事件。
