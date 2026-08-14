<template>
  <div v-if="error" class="error-boundary">
    <div class="error-content">
      <h2>页面出错了</h2>
      <p>{{ error }}</p>
      <el-button type="primary" @click="retry">尝试重试</el-button>
    </div>
  </div>
  <slot v-else></slot>
</template>

<script setup lang="ts">
import { ref, onErrorCaptured } from 'vue'

const error = ref<string | null>(null)

// 捕获子组件抛出的错误
onErrorCaptured((err: any) => {
  console.error('Captured Error:', err)
  error.value = err.message || '未知组件错误'
  // 返回 false 阻止错误继续向上传播
  return false
})

const retry = () => {
  error.value = null
  window.location.reload()
}
</script>

<style scoped>
.error-boundary {
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background: #f5f7fa;
  text-align: center;
}

.error-content {
  padding: 40px;
  background: white;
  border-radius: 12px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

h2 {
  color: #f56c6c;
  margin-bottom: 15px;
}

p {
  color: #606266;
  margin-bottom: 30px;
}
</style>
