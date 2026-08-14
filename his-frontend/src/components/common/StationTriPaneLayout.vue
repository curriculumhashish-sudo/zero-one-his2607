<script setup lang="ts">
import { computed } from 'vue'
import { ArrowLeftBold, ArrowRightBold } from '@element-plus/icons-vue'

defineOptions({
  name: 'StationTriPaneLayout'
})

interface StationTriPaneLayoutProps {
  /** 左栏展开宽度。数字按 px 处理，也可以传入合法 CSS 宽度。 */
  leftWidth?: number | string
  /** 右栏展开宽度。数字按 px 处理，也可以传入合法 CSS 宽度。 */
  rightWidth?: number | string
  /** 左右栏折叠后的宽度。 */
  collapsedWidth?: number | string
  /** 三栏之间的间距。 */
  gap?: number | string
  /** 左栏是否折叠。 */
  leftCollapsed?: boolean
  /** 右栏是否折叠。 */
  rightCollapsed?: boolean
  /** 是否显示左栏折叠控制。 */
  leftCollapsible?: boolean
  /** 是否显示右栏折叠控制。 */
  rightCollapsible?: boolean
}

const props = withDefaults(defineProps<StationTriPaneLayoutProps>(), {
  leftWidth: 280,
  rightWidth: 320,
  collapsedWidth: 40,
  gap: 8,
  leftCollapsed: false,
  rightCollapsed: false,
  leftCollapsible: true,
  rightCollapsible: true
})

const emit = defineEmits<{
  'update:leftCollapsed': [value: boolean]
  'update:rightCollapsed': [value: boolean]
}>()

defineSlots<{
  left?(): any
  main?(): any
  right?(): any
}>()

function toCssSize(value: number | string) {
  return typeof value === 'number' ? `${Math.max(0, value)}px` : value
}

const layoutStyle = computed(() => ({
  '--station-left-width': props.leftCollapsed
    ? toCssSize(props.collapsedWidth)
    : toCssSize(props.leftWidth),
  '--station-right-width': props.rightCollapsed
    ? toCssSize(props.collapsedWidth)
    : toCssSize(props.rightWidth),
  '--station-pane-gap': toCssSize(props.gap)
}))

function toggleLeft() {
  emit('update:leftCollapsed', !props.leftCollapsed)
}

function toggleRight() {
  emit('update:rightCollapsed', !props.rightCollapsed)
}
</script>

<template>
  <section class="station-tri-pane-layout" :style="layoutStyle">
    <aside
      class="station-tri-pane-layout__pane station-tri-pane-layout__pane--left"
      :class="{ 'is-collapsed': leftCollapsed }"
      aria-label="左侧工作区"
    >
      <div v-show="!leftCollapsed" class="station-tri-pane-layout__scroll">
        <slot name="left" />
      </div>

      <el-button
        v-if="leftCollapsible"
        class="station-tri-pane-layout__toggle station-tri-pane-layout__toggle--left"
        circle
        size="small"
        :aria-label="leftCollapsed ? '展开左栏' : '折叠左栏'"
        :title="leftCollapsed ? '展开左栏' : '折叠左栏'"
        @click="toggleLeft"
      >
        <el-icon>
          <ArrowRightBold v-if="leftCollapsed" />
          <ArrowLeftBold v-else />
        </el-icon>
      </el-button>
    </aside>

    <main
      class="station-tri-pane-layout__pane station-tri-pane-layout__pane--main"
      aria-label="中间工作区"
    >
      <div class="station-tri-pane-layout__scroll">
        <slot name="main" />
      </div>
    </main>

    <aside
      class="station-tri-pane-layout__pane station-tri-pane-layout__pane--right"
      :class="{ 'is-collapsed': rightCollapsed }"
      aria-label="右侧工作区"
    >
      <div v-show="!rightCollapsed" class="station-tri-pane-layout__scroll">
        <slot name="right" />
      </div>

      <el-button
        v-if="rightCollapsible"
        class="station-tri-pane-layout__toggle station-tri-pane-layout__toggle--right"
        circle
        size="small"
        :aria-label="rightCollapsed ? '展开右栏' : '折叠右栏'"
        :title="rightCollapsed ? '展开右栏' : '折叠右栏'"
        @click="toggleRight"
      >
        <el-icon>
          <ArrowLeftBold v-if="rightCollapsed" />
          <ArrowRightBold v-else />
        </el-icon>
      </el-button>
    </aside>
  </section>
</template>

<style scoped>
.station-tri-pane-layout {
  display: grid;
  grid-template-columns:
    var(--station-left-width)
    minmax(0, 1fr)
    var(--station-right-width);
  box-sizing: border-box;
  width: 100%;
  height: 100%;
  min-width: 0;
  min-height: 0;
  gap: var(--station-pane-gap);
  overflow: hidden;
  transition: grid-template-columns 0.2s ease;
}

.station-tri-pane-layout__pane {
  position: relative;
  min-width: 0;
  min-height: 0;
  overflow: visible;
  border: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.station-tri-pane-layout__pane.is-collapsed {
  overflow: hidden;
  background: var(--el-fill-color-extra-light);
}

.station-tri-pane-layout__scroll {
  width: 100%;
  height: 100%;
  min-width: 0;
  min-height: 0;
  overflow: auto;
  overscroll-behavior: contain;
  scrollbar-gutter: stable;
}

.station-tri-pane-layout__toggle {
  position: absolute;
  top: 50%;
  z-index: 2;
  transform: translateY(-50%);
  box-shadow: var(--el-box-shadow-light);
}

.station-tri-pane-layout__toggle--left {
  right: -13px;
}

.station-tri-pane-layout__toggle--right {
  left: -13px;
}

.station-tri-pane-layout__pane--left.is-collapsed .station-tri-pane-layout__toggle--left,
.station-tri-pane-layout__pane--right.is-collapsed .station-tri-pane-layout__toggle--right {
  top: 16px;
  right: auto;
  left: 50%;
  transform: translateX(-50%);
}
</style>
