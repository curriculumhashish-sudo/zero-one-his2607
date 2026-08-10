<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, ref } from 'vue'

import ActionToolbar from '@/components/common/ActionToolbar.vue'
import type { ActionToolbarItem, CommonComponentKey } from '@/components/common/type'
import type { DoctorStationTabItem } from './type'

defineOptions({
  name: 'DoctorStationHeader'
})

/**
 * 公共 Header 只接收已经计算好的页签和按钮。
 *
 * 例如“何时允许接诊”属于门诊业务，“何时允许停嘱”属于住院业务，
 * 这些规则应该由各自页面计算后，通过 actions 传进来。
 */
const props = withDefaults(
  defineProps<{
    activeTab: CommonComponentKey
    tabs?: DoctorStationTabItem[]
    actions?: ActionToolbarItem[]
    maxVisible?: number
    compact?: boolean
    loading?: boolean
    disabled?: boolean
  }>(),
  {
    tabs: () => [],
    actions: () => [],
    maxVisible: 5,
    compact: true,
    loading: false,
    disabled: false
  }
)

const headerRef = ref<HTMLElement | null>(null)
const containerWidth = ref(Number.POSITIVE_INFINITY)
let resizeObserver: ResizeObserver | undefined

/**
 * 空间不足时不再强制把业务按钮收入“更多”，而是让页签和操作区分成上下两行。
 * maxVisible 仍由使用方控制：门诊传入足够大的值即可完整展示全部操作，
 * 住院等其他场景仍可主动保留“更多”菜单。
 */
const responsiveMaxVisible = computed(() => props.maxVisible)
const stacked = computed(() => containerWidth.value < 980)

function updateContainerWidth(width?: number) {
  const nextWidth = width ?? headerRef.value?.getBoundingClientRect().width ?? 0
  if (nextWidth > 0) containerWidth.value = nextWidth
}

onMounted(() => {
  updateContainerWidth()
  if (typeof ResizeObserver === 'undefined' || !headerRef.value) return

  resizeObserver = new ResizeObserver((entries) => {
    updateContainerWidth(entries[0]?.contentRect.width)
  })
  resizeObserver.observe(headerRef.value)
})

onBeforeUnmount(() => {
  resizeObserver?.disconnect()
})

/**
 * 公共组件把完整按钮对象和页签 key 原样交还给场景页面。
 * 门诊或住院页面再根据自己的类型处理具体业务。
 */
const emit = defineEmits<{
  action: [action: ActionToolbarItem]
  tabChange: [tab: CommonComponentKey]
}>()

function handleAction(action: ActionToolbarItem) {
  emit('action', action)
}

function handleTabChange(tab: CommonComponentKey) {
  emit('tabChange', tab)
}
</script>

<template>
  <section ref="headerRef" class="doctor-station-header" :class="{ 'is-stacked': stacked }">
    <!-- 页签完全由使用方配置，因此门诊和住院可以展示不同的工作区。 -->
    <el-tabs
      :model-value="activeTab"
      class="doctor-station-header__tabs"
      @update:model-value="handleTabChange"
    >
      <el-tab-pane
        v-for="tab in tabs"
        :key="tab.key"
        :name="tab.key"
        :label="tab.label"
        :disabled="tab.disabled"
      />
    </el-tabs>

    <!--
      按钮沿用公共 ActionToolbar。
      确认提示、加载状态、禁用状态都由 actions 中的配置控制。
    -->
    <ActionToolbar
      class="doctor-station-header__toolbar"
      :compact="compact"
      :actions="actions"
      :max-visible="responsiveMaxVisible"
      :loading="loading"
      :disabled="disabled"
      @action="handleAction"
    />
  </section>
</template>

<style scoped>
.doctor-station-header {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  align-items: stretch;
  overflow: hidden;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.doctor-station-header__tabs {
  box-sizing: border-box;
  width: auto;
  min-width: 0;
  max-width: 52%;
  flex: 0 1 auto;
  padding: 0 12px;
}

.doctor-station-header__tabs :deep(.el-tabs__header) {
  margin: 0;
}

/* 页面正文仍由父页面渲染，这里只使用 Element Plus 的页签头。 */
.doctor-station-header__tabs :deep(.el-tabs__content) {
  display: none;
}

.doctor-station-header__toolbar {
  box-sizing: border-box;
  width: auto;
  min-width: 0;
  flex: 1 1 auto;
  justify-content: flex-end;
  border: 0;
  border-left: 1px solid var(--el-border-color-lighter);
}

/* 宽度充足时保持单行。 */
.doctor-station-header__toolbar :deep(.action-toolbar__actions) {
  min-width: 0;
  max-width: 100%;
  flex: 1 1 auto;
  flex-wrap: nowrap;
  justify-content: flex-end;
}

/*
 * 三栏医生站的中间栏通常比浏览器窗口窄。
 * 当中栏不足 980px 时让操作区换到第二行，避免按钮被“更多”隐藏或互相重叠。
 */
.doctor-station-header.is-stacked {
  flex-wrap: wrap;
}

.doctor-station-header.is-stacked .doctor-station-header__tabs {
  width: 100%;
  max-width: none;
  flex: 1 0 100%;
}

.doctor-station-header.is-stacked .doctor-station-header__toolbar {
  width: 100%;
  flex: 1 0 100%;
  border-top: 1px solid var(--el-border-color-lighter);
  border-left: 0;
}

.doctor-station-header.is-stacked .doctor-station-header__toolbar :deep(.action-toolbar__actions) {
  flex-wrap: wrap;
}

.doctor-station-header__toolbar :deep(.action-toolbar__main:empty) {
  display: none;
}

.doctor-station-header__toolbar :deep(.el-button) {
  min-width: 0;
  padding-right: 10px;
  padding-left: 10px;
}
</style>
