<template>
  <el-card shadow="never" class="feature-card">
    <template #header>
      <div class="card-header">
        <span class="header-title">常用功能</span>
        <el-button type="primary" link :icon="Setting">管理快捷入口</el-button>
      </div>
    </template>
    <div class="feature-grid">
      <div
        class="feature-item animate-on-load"
        v-for="(link, index) in items"
        :key="link.name"
        :style="{ animationDelay: `${delayOffset + index * delayStep}ms` }"
      >
        <div class="feature-icon" :style="{ backgroundColor: link.bgColor }">
          <el-icon :size="22" color="#fff"><component :is="link.icon" /></el-icon>
        </div>
        <span class="feature-name">{{ link.name }}</span>
        <span class="feature-desc">{{ link.desc }}</span>
      </div>
    </div>
  </el-card>
</template>

<script setup lang="ts">
import { Setting } from '@element-plus/icons-vue'
import type { FeatureItem } from '@/components/dashboard/type'

/**
 * 常用功能入口网格组件
 */
interface Props {
  /** 功能项列表 */
  items: FeatureItem[]
  /** 初始动画延迟(ms) */
  delayOffset?: number
  /** 每一项的延迟步长(ms) */
  delayStep?: number
}

withDefaults(defineProps<Props>(), {
  delayOffset: 100,
  delayStep: 30
})
</script>

<style scoped>
.feature-card {
  border: none;
  border-radius: 12px;
  margin-bottom: 12px;
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

.feature-grid {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 16px;
}

.feature-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center;
  padding: 16px;
  border-radius: 10px;
  transition: all 0.3s;
  border: 1px solid transparent;
}

.feature-item:hover {
  background-color: #fff;
  box-shadow: 0 8px 20px rgba(0, 0, 0, 0.08);
  border-color: #e4e7ed;
  transform: translateY(-5px);
}

.animate-on-load {
  opacity: 0;
  animation: fadeInUp 0.4s ease-out forwards;
}

@keyframes fadeInUp {
  from {
    opacity: 0;
    transform: translateY(10px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.feature-icon {
  width: 44px;
  height: 44px;
  border-radius: 10px;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-bottom: 12px;
}

.feature-name {
  font-size: 14px;
  font-weight: 600;
  color: var(--title-color, #2c3e50);
  margin-bottom: 4px;
}

.feature-desc {
  font-size: 12px;
  color: #909399;
}
</style>
