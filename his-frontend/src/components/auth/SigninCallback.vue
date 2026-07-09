<script setup lang="ts">
import { onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthManager } from '@/plugins/auth-manager'
const router = useRouter()
onMounted(async () => {
  try {
    await useAuthManager().loginCallback()
    // 登录成功跳转首页
    router.push('/home')
  } catch (err) {
    console.error('登录回调失败', err)
    await useAuthManager().logout()
    // 登录失败跳转登录页
    router.push('/')
  }
})
</script>

<template>
  <div class="callback-loading">
    <div class="loader-container">
      <div class="planet-loader">
        <div class="planet-core"></div>
        <div class="orbit-path">
          <div class="orbit-satellite"></div>
        </div>
      </div>
      <div class="loading-text">
        <span class="text">正在安全登录</span>
        <span class="dots">...</span>
      </div>
    </div>
  </div>
</template>

<style scoped>
.callback-loading {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
  z-index: 9999;
}

.loader-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 40px;
}

.planet-loader {
  position: relative;
  width: 100px;
  height: 100px;
}

.planet-core {
  position: absolute;
  top: 50%;
  left: 50%;
  width: 40px;
  height: 40px;
  margin: -20px 0 0 -20px;
  background: radial-gradient(circle at 30% 30%, #4facfe 0%, #0072ff 100%);
  border-radius: 50%;
  box-shadow:
    inset -4px -4px 8px rgba(0, 0, 0, 0.3),
    0 0 20px rgba(79, 172, 254, 0.4);
}

.orbit-path {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  border: 1px dashed rgba(0, 114, 255, 0.2);
  border-radius: 50%;
  animation: rotate 4s linear infinite;
}

.orbit-satellite {
  position: absolute;
  top: 50%;
  left: -5px;
  width: 10px;
  height: 10px;
  background: #4facfe;
  border-radius: 50%;
  box-shadow: 0 0 15px #4facfe;
}

.loading-text {
  font-size: 20px;
  color: #2c3e50;
  font-weight: 500;
  letter-spacing: 2px;
}

.dots {
  display: inline-block;
  width: 24px;
  text-align: left;
  animation: blink 1.5s infinite;
}

@keyframes rotate {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}

@keyframes blink {
  0%,
  100% {
    opacity: 0;
  }
  50% {
    opacity: 1;
  }
}
</style>
