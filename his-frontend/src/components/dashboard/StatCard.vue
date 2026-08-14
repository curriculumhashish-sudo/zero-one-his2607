<template>
  <el-card
    shadow="hover"
    class="stat-card animate-on-load"
    :style="{ animationDelay: `${delay}ms` }"
    :body-style="{ padding: '20px' }"
  >
    <div class="stat-card-body">
      <div class="stat-icon-wrapper" :style="{ color: color, backgroundColor: bgColor }">
        <el-icon :size="24"><component :is="icon" /></el-icon>
      </div>
      <div class="stat-main">
        <div class="stat-label">{{ label }}</div>
        <div class="stat-count">{{ formattedValue }}{{ unit }}</div>
      </div>
    </div>
    <div class="stat-footer">
      <span :class="['trend-tag', trendType]">
        {{ trendValue }}
        <el-icon>
          <CaretTop v-if="trendType === 'up'" />
          <CaretBottom v-else />
        </el-icon>
      </span>
      <span class="footer-desc">较昨日同期</span>
    </div>
  </el-card>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { CaretTop, CaretBottom } from '@element-plus/icons-vue'
import type { TrendType } from '@/components/dashboard/type'

/**
 * 核心指标统计卡片组件
 */
interface Props {
  /** 指标名称 */
  label: string
  /** 目标值（保留用于上层数据语义/可能的后续计算） */
  value: number
  /** 动画展示值（从 0 增长到 value） */
  displayValue: number
  /** 单位（如 %） */
  unit?: string
  /** 趋势文案（如 12%） */
  trendValue: string
  /** 趋势类型 */
  trendType: TrendType
  /** Element Plus 图标组件 */
  icon: any
  /** 图标/主色 */
  color: string
  /** 图标背景色 */
  bgColor: string
  /** 入场动画延迟(ms) */
  delay?: number
}

const props = withDefaults(defineProps<Props>(), {
  unit: '',
  delay: 0
})

const formattedValue = computed(() => {
  if (props.label === '系统负载') {
    return props.displayValue.toFixed(1)
  }
  return Math.floor(props.displayValue).toLocaleString()
})
</script>

<style scoped>
.stat-card {
  border: none;
  border-radius: 12px;
  transition: transform 0.3s ease;
}

.stat-card:hover {
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.1);
}

.animate-on-load {
  opacity: 0;
  animation: fadeIn 0.6s ease forwards;
}

@keyframes fadeIn {
  from {
    opacity: 0;
  }
  to {
    opacity: 1;
  }
}

.stat-card-body {
  display: flex;
  align-items: center;
  gap: 16px;
  margin-bottom: 12px;
}

.stat-icon-wrapper {
  width: 48px;
  height: 48px;
  border-radius: 10px;
  display: flex;
  justify-content: center;
  align-items: center;
}

.stat-label {
  font-size: 14px;
  color: var(--desc-color, #606266);
  margin-bottom: 4px;
}

.stat-count {
  font-size: 20px;
  font-weight: 800;
  color: var(--title-color, #2c3e50);
}

.stat-footer {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 12px;
}

.trend-tag {
  display: flex;
  align-items: center;
  gap: 2px;
  font-weight: 600;
}

.trend-tag.up {
  color: #67c23a;
}
.trend-tag.down {
  color: #f56c6c;
}

.footer-desc {
  color: #909399;
}
</style>
