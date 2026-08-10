<script setup lang="ts">
import { computed } from 'vue'
import { InfoFilled, UserFilled } from '@element-plus/icons-vue'

import type { NurseStationOperationConfig } from '../type'

defineOptions({
  name: 'NurseStationModulePlaceholder'
})

const props = defineProps<{
  config: NurseStationOperationConfig
  selectedPatientCount: number
}>()

const selectionHint = computed(() => {
  if (props.config.selectionPolicy === 'multiple') return '请从左侧选择一名或多名患者'
  return '请从左侧选择一名患者'
})
</script>

<template>
  <section class="module-placeholder" :aria-label="config.id">
    <header class="module-placeholder__header">
      <div>
        <h2>{{ config.id }}</h2>
        <p>{{ config.description }}</p>
      </div>
      <el-tag effect="plain" :type="config.selectionPolicy === 'multiple' ? 'warning' : 'info'">
        {{ config.selectionPolicy === 'multiple' ? '支持患者多选' : '患者单选' }}
      </el-tag>
    </header>

    <div class="module-placeholder__toolbar">
      <span class="module-placeholder__toolbar-title">业务操作区</span>
      <span class="module-placeholder__selection">
        <el-icon><UserFilled /></el-icon>
        已选 {{ selectedPatientCount }} 人
      </span>
    </div>

    <div class="module-placeholder__content">
      <el-empty :description="selectionHint">
        <template #image>
          <el-icon class="module-placeholder__empty-icon"><InfoFilled /></el-icon>
        </template>
        <p class="module-placeholder__note">当前阶段已完成页面骨架，业务接口将在契约确认后接入。</p>
      </el-empty>
    </div>
  </section>
</template>

<style scoped>
.module-placeholder {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  background: var(--el-bg-color);
}

.module-placeholder__header {
  display: flex;
  min-height: 74px;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  padding: 12px 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.module-placeholder__header h2 {
  margin: 0;
  color: var(--el-text-color-primary);
  font-size: 18px;
  line-height: 1.5;
}

.module-placeholder__header p {
  margin: 3px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.module-placeholder__toolbar {
  display: flex;
  min-height: 44px;
  align-items: center;
  justify-content: space-between;
  padding: 0 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-extra-light);
}

.module-placeholder__toolbar-title {
  color: var(--el-text-color-primary);
  font-weight: 600;
}

.module-placeholder__selection {
  display: inline-flex;
  align-items: center;
  gap: 5px;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.module-placeholder__content {
  display: grid;
  min-height: 0;
  flex: 1;
  place-items: center;
  overflow: auto;
  background: var(--el-fill-color-lighter);
}

.module-placeholder__content :deep(.el-empty) {
  min-width: min(460px, 90%);
  padding: 48px 32px;
  border: 1px dashed var(--el-border-color);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.module-placeholder__empty-icon {
  color: var(--el-color-primary-light-3);
  font-size: 68px;
}

.module-placeholder__note {
  margin: 8px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}
</style>
