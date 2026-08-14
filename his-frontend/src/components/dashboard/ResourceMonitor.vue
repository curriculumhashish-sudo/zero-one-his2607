<template>
  <div class="system-status-bar">
    <div class="status-item" v-for="res in items" :key="res.name">
      <div class="status-icon" :style="{ color: res.color }">
        <el-icon :size="20"><component :is="res.icon" /></el-icon>
      </div>
      <div class="status-info">
        <div class="status-label">{{ res.name }}</div>
        <div class="status-progress-wrapper">
          <el-progress
            :percentage="res.displayValue"
            :color="res.color"
            :show-text="false"
            :stroke-width="6"
          />
          <span class="status-value" :style="{ color: res.color }">
            {{ Math.floor(res.displayValue) }}%
          </span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import type { ResourceItem } from '@/components/dashboard/type'

/**
 * 资源监控条组件
 */
interface Props {
  /** 资源项列表 */
  items: ResourceItem[]
}

defineProps<Props>()
</script>

<style scoped>
.system-status-bar {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 24px;
  background: white;
  padding: 16px 24px;
  border-radius: 12px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.05);
}

.status-item {
  display: flex;
  align-items: center;
  gap: 12px;
}

.status-icon {
  width: 36px;
  height: 36px;
  background: #f8f9fa;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.status-info {
  flex: 1;
}

.status-label {
  font-size: 12px;
  color: var(--desc-color, #606266);
  margin-bottom: 4px;
  font-weight: 600;
}

.status-progress-wrapper {
  display: flex;
  align-items: center;
  gap: 10px;
}

.status-progress-wrapper :deep(.el-progress) {
  flex: 1;
}

.status-value {
  font-size: 12px;
  font-weight: 700;
  min-width: 35px;
  text-align: right;
}
</style>
