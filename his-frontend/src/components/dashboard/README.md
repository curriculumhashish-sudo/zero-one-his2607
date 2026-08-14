# Dashboard 组件集

本目录用于承载 Dashboard 首页的可复用子组件与共享类型定义。

## 目录结构
- [StatCard](./StatCard.md)：顶部核心指标卡片
- [FeatureGrid](./FeatureGrid.md)：常用功能入口网格
- [ProjectInfo](./ProjectInfo.md)：项目信息展示
- [SystemTimeline](./SystemTimeline.md)：系统动态时间轴
- [ResourceMonitor](./ResourceMonitor.md)：资源监控条
- [type.ts](./type.ts)：共享数据模型（StatCardItem/ResourceItem/Activity/FeatureItem 等）

## 引用方式
```ts
import StatCard from '@/components/dashboard/StatCard.vue'
import FeatureGrid from '@/components/dashboard/FeatureGrid.vue'
import ProjectInfo from '@/components/dashboard/ProjectInfo.vue'
import SystemTimeline from '@/components/dashboard/SystemTimeline.vue'
import ResourceMonitor from '@/components/dashboard/ResourceMonitor.vue'
```
