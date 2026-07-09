<template>
  <div class="status-container">
    <PolygonBackground />
    <div class="glass-card">
      <div class="error-code">{{ code }}</div>
      <div class="error-info">
        <h2 class="title">{{ title }}</h2>
        <p class="description">{{ description }}</p>
      </div>
      <div class="action-wrapper">
        <el-button type="primary" class="back-btn" @click="goHome">返回首页</el-button>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
/**
 * ErrorPage 基础组件
 * 提供统一的错误页面布局，包含动态多边形背景和毛玻璃效果内容卡片。
 */
import PolygonBackground from '@/components/background/PolygonBackground.vue'
import { useRouter } from 'vue-router'

/**
 * 组件属性定义
 */
interface Props {
  /** 错误代码 (如: 404, 403, 500) */
  code: string | number
  /** 错误标题 (如: 页面未找到) */
  title: string
  /** 详细错误描述信息 */
  description: string
}

defineProps<Props>()

// 获取路由实例
const router = useRouter()

/**
 * 跳转回系统首页
 */
const goHome = () => {
  router.push('/home')
}
</script>

<style scoped>
/* 全屏容器，禁止滚动手势以优化响应性能 */
.status-container {
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  overflow: hidden;
  touch-action: none;
}

/* 毛玻璃质感内容卡片 */
.glass-card {
  width: 500px;
  padding: 60px 50px;
  background: var(--glass-bg);
  backdrop-filter: var(--glass-blur);
  -webkit-backdrop-filter: var(--glass-blur);
  border: 1px solid var(--glass-border);
  border-radius: 24px;
  box-shadow: var(--glass-shadow);
  text-align: center;
  transition: all 0.3s ease;
}

/* 渐变色状态码文字 */
.error-code {
  font-size: 120px;
  font-weight: 900;
  background: var(--primary-gradient);
  -webkit-background-clip: text;
  background-clip: text;
  -webkit-text-fill-color: transparent;
  margin-bottom: 20px;
  line-height: 1;
  letter-spacing: -5px;
}

/* 错误标题样式 */
.title {
  color: var(--title-color);
  font-size: 32px;
  margin-bottom: 15px;
  font-weight: 700;
}

/* 描述文案样式 */
.description {
  color: var(--desc-color);
  font-size: 18px;
  margin-bottom: 40px;
}

/* 响应式渐变色按钮 */
.back-btn {
  width: 200px;
  height: 50px;
  font-size: 18px;
  border-radius: 12px;
  background: var(--primary-gradient);
  border: none;
  box-shadow: 0 4px 15px rgba(25, 113, 194, 0.2);
  transition: all 0.3s ease;
}

.back-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(25, 113, 194, 0.3);
}
</style>
