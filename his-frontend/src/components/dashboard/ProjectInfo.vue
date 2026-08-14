<template>
  <el-card
    shadow="never"
    class="project-info-card animate-on-load"
    :style="{ animationDelay: `${delay}ms` }"
  >
    <template #header>
      <div class="card-header">
        <span class="header-title">项目信息</span>
        <el-tag size="small">v1.0.0</el-tag>
      </div>
    </template>
    <el-descriptions :column="2" border>
      <el-descriptions-item
        label="前端框架"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >Vue 3.5</el-descriptions-item
      >
      <el-descriptions-item
        label="构建工具"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >Vite 8.0</el-descriptions-item
      >
      <el-descriptions-item
        label="状态管理"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >Pinia 3.0</el-descriptions-item
      >
      <el-descriptions-item
        label="UI 组件库"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >Element Plus</el-descriptions-item
      >
      <el-descriptions-item
        label="认证协议"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >OAuth2 / OIDC</el-descriptions-item
      >
      <el-descriptions-item
        label="开发语言"
        class-name="info-item-animate"
        label-class-name="info-item-animate"
        >TypeScript 6.0</el-descriptions-item
      >
    </el-descriptions>
  </el-card>
</template>

<script setup lang="ts">
/**
 * 项目信息展示组件
 */
interface Props {
  /** 卡片整体动画延迟(ms) */
  delay?: number
}

withDefaults(defineProps<Props>(), {
  delay: 250
})
</script>

<style scoped>
.project-info-card {
  border: none;
  border-radius: 12px;
  flex: 1;
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

:deep(.info-item-animate) {
  animation: fadeIn 0.5s ease-out both;
}

:deep(.el-descriptions__row:nth-child(1) .info-item-animate) {
  animation-delay: 350ms;
}
:deep(.el-descriptions__row:nth-child(2) .info-item-animate) {
  animation-delay: 450ms;
}
:deep(.el-descriptions__row:nth-child(3) .info-item-animate) {
  animation-delay: 550ms;
}

:deep(.el-descriptions__label) {
  width: 120px;
  background-color: #fafafa;
  font-weight: 600;
}
</style>
