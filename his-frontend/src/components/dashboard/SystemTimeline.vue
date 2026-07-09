<template>
  <el-card shadow="never" class="notice-card">
    <template #header>
      <div class="card-header">
        <span class="header-title">系统动态</span>
        <el-button link type="primary">刷新</el-button>
      </div>
    </template>
    <el-timeline class="dashboard-timeline">
      <el-timeline-item
        v-for="(activity, index) in items"
        :key="index"
        :type="activity.type"
        :color="activity.color"
        :timestamp="activity.time"
        size="normal"
        class="activity-item animate-on-load"
        :style="{ animationDelay: `${delayOffset + index * delayStep}ms` }"
      >
        <h4 class="activity-title">{{ activity.title }}</h4>
        <p class="activity-content">{{ activity.content }}</p>
      </el-timeline-item>
    </el-timeline>
  </el-card>
</template>

<script setup lang="ts">
import type { Activity } from '@/components/dashboard/type'

/**
 * 系统动态时间轴组件
 */
interface Props {
  /** 动态条目列表 */
  items: Activity[]
  /** 初始动画延迟(ms) */
  delayOffset?: number
  /** 每一项的延迟步长(ms) */
  delayStep?: number
}

withDefaults(defineProps<Props>(), {
  delayOffset: 150,
  delayStep: 80
})
</script>

<style scoped>
.notice-card {
  height: 100%;
  border: none;
  border-radius: 12px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.header-title {
  font-size: 16px;
  font-weight: 700;
  color: var(--title-color, #2c3e50);
}

.dashboard-timeline {
  padding: 10px 5px;
}

.animate-on-load {
  opacity: 0;
  animation: fadeInRight 0.4s ease-out forwards;
}

@keyframes fadeInRight {
  from {
    opacity: 0;
    transform: translateX(15px);
  }
  to {
    opacity: 1;
    transform: translateX(0);
  }
}

.activity-title {
  margin: 0 0 8px 0;
  font-size: 14px;
  color: var(--title-color, #2c3e50);
  transition: color 0.3s;
}

.activity-item:hover .activity-title {
  color: var(--el-color-primary, #409eff);
}

.activity-content {
  margin: 0;
  font-size: 13px;
  color: var(--desc-color, #606266);
}
</style>
