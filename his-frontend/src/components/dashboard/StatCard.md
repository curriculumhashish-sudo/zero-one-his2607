# StatCard 统计卡片组件

用于展示 Dashboard 顶部的核心指标数据，支持数字滚动动画和趋势显示。

## 特性
- **数字动画**：支持从 0 开始的增长动画。
- **趋势显示**：支持上升/下降趋势标签。
- **自定义图标**：支持 Element Plus 图标及其背景色自定义。
- **入场动画**：支持交错淡入效果。

## 类型
- `trendType` 使用的模型为 [TrendType](./type.ts)

## Props
| 参数 | 说明 | 类型 | 默认值 |
| --- | --- | --- | --- |
| label | 指标名称 | string | - |
| value | 目标数值 | number | - |
| displayValue | 当前显示数值（用于动画） | number | - |
| unit | 数值单位 | string | '' |
| trendValue | 趋势百分比 | string | - |
| trendType | 趋势类型 | TrendType | - |
| icon | 图标组件 | any | - |
| color | 图标颜色 | string | - |
| bgColor | 图标背景色 | string | - |
| delay | 入场动画延迟 (ms) | number | 0 |

## 使用示例
```vue
<StatCard
  label="在线用户"
  :value="1284"
  :display-value="displayValue"
  trend-value="12%"
  trend-type="up"
  :icon="User"
  color="#409eff"
  bg-color="#ecf5ff"
  :delay="100"
/>
```
