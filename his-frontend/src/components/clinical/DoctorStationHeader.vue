<script setup lang="ts">
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
withDefaults(
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
  <section class="doctor-station-header">
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
      :max-visible="maxVisible"
      :loading="loading"
      :disabled="disabled"
      @action="handleAction"
    />
  </section>
</template>

<style scoped>
.doctor-station-header {
  display: flex;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.doctor-station-header__tabs {
  min-width: 260px;
  padding-left: 16px;
}

.doctor-station-header__tabs :deep(.el-tabs__header) {
  margin: 0;
}

/* 页面正文仍由父页面渲染，这里只使用 Element Plus 的页签头。 */
.doctor-station-header__tabs :deep(.el-tabs__content) {
  display: none;
}

.doctor-station-header__toolbar {
  flex: 1;
  justify-content: flex-end;
  border: 0;
}

/* 窄屏时改为上下两行，避免操作按钮挤压页签。 */
@media (max-width: 1100px) {
  .doctor-station-header {
    align-items: stretch;
    flex-direction: column;
    gap: 0;
  }

  .doctor-station-header__toolbar {
    justify-content: flex-start;
    border-top: 1px solid var(--el-border-color-lighter);
  }
}
</style>
