# StationTriPaneLayout

## 组件定位

`StationTriPaneLayout` 是工作站页面的三栏布局外壳，用于组织：

- 左侧队列区
- 中间主要业务区
- 右侧辅助区

组件只负责三栏结构、栏宽、左右栏折叠和三栏独立滚动，不包含患者、医嘱、处方、收费等业务逻辑。

组件路径：

```text
src/components/common/StationTriPaneLayout.vue
```

## 基础用法

```vue
<script setup lang="ts">
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
</script>

<template>
  <div class="station-page">
    <StationTriPaneLayout>
      <template #left>
        <div class="pane-content">左侧队列内容</div>
      </template>

      <template #main>
        <div class="pane-content">中间业务内容</div>
      </template>

      <template #right>
        <div class="pane-content">右侧辅助内容</div>
      </template>
    </StationTriPaneLayout>
  </div>
</template>

<style scoped>
.station-page {
  height: calc(100vh - 60px);
  min-width: 0;
  min-height: 0;
}

.pane-content {
  padding: 16px;
}
</style>
```

父容器必须有明确高度，否则组件无法计算独立滚动区域。

## Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `leftWidth` | `number \| string` | `280` | 左栏展开宽度或 CSS Grid track，数字按 `px` 处理 |
| `rightWidth` | `number \| string` | `320` | 右栏展开宽度或 CSS Grid track，数字按 `px` 处理 |
| `collapsedWidth` | `number \| string` | `40` | 左右栏折叠后保留的宽度 |
| `gap` | `number \| string` | `8` | 三栏间距，数字按 `px` 处理 |
| `leftCollapsed` | `boolean` | `false` | 左栏是否折叠 |
| `rightCollapsed` | `boolean` | `false` | 右栏是否折叠 |
| `leftCollapsible` | `boolean` | `true` | 是否显示左栏折叠按钮 |
| `rightCollapsible` | `boolean` | `true` | 是否显示右栏折叠按钮 |

宽度属性可以传数字：

```vue
<StationTriPaneLayout :left-width="260" :right-width="360" />
```

也可以传合法的 CSS 宽度或 Grid track：

```vue
<StationTriPaneLayout
  left-width="clamp(240px, 20vw, 320px)"
  right-width="minmax(280px, 24vw)"
/>
```

中间栏始终使用 `minmax(0, 1fr)`，自动占用剩余空间。

## 插槽

| 插槽 | 说明 |
| --- | --- |
| `left` | 左侧队列、列表、分类等内容 |
| `main` | 页面主要业务内容 |
| `right` | 辅助信息、上下文工具等内容 |

三个插槽都不提供业务参数。数据和事件由使用页面自行维护。

## 折叠

左右栏分别使用受控状态：

```vue
<script setup lang="ts">
import { ref } from 'vue'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'

const leftCollapsed = ref(false)
const rightCollapsed = ref(false)
</script>

<template>
  <StationTriPaneLayout
    v-model:left-collapsed="leftCollapsed"
    v-model:right-collapsed="rightCollapsed"
  >
    <template #left>左栏</template>
    <template #main>主栏</template>
    <template #right>右栏</template>
  </StationTriPaneLayout>
</template>
```

折叠按钮只发出状态更新，页面仍然是状态的唯一来源。

等价写法：

```vue
<StationTriPaneLayout
  :left-collapsed="leftCollapsed"
  :right-collapsed="rightCollapsed"
  @update:left-collapsed="leftCollapsed = $event"
  @update:right-collapsed="rightCollapsed = $event"
/>
```

## 事件

| 事件 | 参数 | 说明 |
| --- | --- | --- |
| `update:leftCollapsed` | `boolean` | 点击左栏折叠按钮时触发 |
| `update:rightCollapsed` | `boolean` | 点击右栏折叠按钮时触发 |

如果页面不需要内置折叠按钮，可以隐藏按钮并从外部控制：

```vue
<script setup lang="ts">
import { ref } from 'vue'

const leftCollapsed = ref(false)
</script>

<template>
  <el-button @click="leftCollapsed = !leftCollapsed">切换左栏</el-button>

  <StationTriPaneLayout
    v-model:left-collapsed="leftCollapsed"
    :left-collapsible="false"
  >
    <template #left>左栏</template>
    <template #main>主栏</template>
    <template #right>右栏</template>
  </StationTriPaneLayout>
</template>
```

折叠使用 `v-show` 隐藏插槽内容，不会销毁插槽中的表单、列表选中状态或组件实例。

## 三栏独立滚动

每个插槽外层都有独立的 `overflow: auto` 容器。一栏内容滚动时，不会带动另外两栏。

```vue
<StationTriPaneLayout>
  <template #left>
    <div v-for="index in 50" :key="index">队列记录 {{ index }}</div>
  </template>

  <template #main>
    <div v-for="index in 100" :key="index">业务记录 {{ index }}</div>
  </template>

  <template #right>
    <div v-for="index in 30" :key="index">辅助记录 {{ index }}</div>
  </template>
</StationTriPaneLayout>
```

不要给整个工作站页面设置 `overflow: auto`，否则容易出现页面和栏位两层滚动条。

推荐页面外壳：

```css
.station-page {
  display: flex;
  height: calc(100vh - var(--app-header-height));
  min-width: 0;
  min-height: 0;
  overflow: hidden;
}
```

如果组件位于多层 Flex 或 Grid 中，每一层都应设置 `min-height: 0`，避免内容把父容器撑高。

## 固定栏内头部

布局组件默认让整个插槽内容滚动。如果某一栏需要固定头部，应由插槽内容自行建立内部 Flex 布局：

```vue
<template #left>
  <section class="queue-pane">
    <header class="queue-pane__header">队列标题</header>
    <div class="queue-pane__body">
      <div v-for="index in 50" :key="index">记录 {{ index }}</div>
    </div>
  </section>
</template>

<style scoped>
.queue-pane {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
}

.queue-pane__header {
  flex-shrink: 0;
  padding: 12px;
}

.queue-pane__body {
  flex: 1;
  min-height: 0;
  overflow: auto;
  padding: 12px;
}
</style>
```

## 两栏工作站

组件始终保留三栏结构。暂时不需要某一侧内容时，可以将该栏折叠，并按需隐藏内置按钮：

```vue
<StationTriPaneLayout
  :right-collapsed="true"
  :right-collapsible="false"
  :collapsed-width="0"
>
  <template #left>查询列表</template>
  <template #main>详情内容</template>
</StationTriPaneLayout>
```

`collapsedWidth="0"` 会完全释放折叠栏宽度；默认值 `40` 会保留展开控制栏。

## 常见工作站组合

```vue
<script setup lang="ts">
import { ref } from 'vue'
import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'

const leftCollapsed = ref(false)
const rightCollapsed = ref(false)
</script>

<template>
  <div class="station-page">
    <StationTriPaneLayout
      v-model:left-collapsed="leftCollapsed"
      v-model:right-collapsed="rightCollapsed"
      left-width="clamp(260px, 22vw, 340px)"
      right-width="clamp(280px, 24vw, 380px)"
      :gap="10"
    >
      <template #left>
        <QueuePanel />
      </template>

      <template #main>
        <BusinessWorkspace />
      </template>

      <template #right>
        <AssistantPanel />
      </template>
    </StationTriPaneLayout>
  </div>
</template>
```

`QueuePanel`、`BusinessWorkspace` 和 `AssistantPanel` 只是页面自己的组件，布局组件不依赖这些名称或实现。

## 职责边界

适合放在组件中：

- 三栏 Grid 结构
- 左右栏宽度
- 左右栏折叠按钮
- 三栏独立滚动容器
- 基础边框、背景和间距

应由页面或插槽内容处理：

- 患者、队列和床位数据
- 医嘱、处方、检查、检验和收费逻辑
- 查询、分页、保存和提交接口
- 加载、空状态和权限判断
- 栏内标题、工具栏、表格和表单
- 小屏幕下自动折叠哪一栏的业务策略
