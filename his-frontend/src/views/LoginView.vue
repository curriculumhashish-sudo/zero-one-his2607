<template>
  <div class="login-container">
    <PolygonBackground />
    <div class="glass-card">
      <div class="login-header">
        <h2 class="title">欢迎登录</h2>
        <div class="mode-tabs" v-if="isDev">
          <div
            class="tab-item"
            :class="{ active: loginMode === 'account' }"
            @click="loginMode = 'account'"
          >
            账号登录
          </div>
          <div
            class="tab-item"
            :class="{ active: loginMode === 'oauth2' }"
            @click="loginMode = 'oauth2'"
          >
            授权登录
          </div>
        </div>
        <div class="mode-tabs single" v-else>
          <div class="tab-item active">授权登录</div>
        </div>
      </div>

      <div v-if="loginMode === 'account'" class="form-wrapper">
        <el-form :model="formData" status-icon label-width="0">
          <el-form-item prop="username">
            <el-input
              v-model="formData.username"
              placeholder="请输入账号"
              clearable
              @keyup.enter="submitForm()"
            >
              <template #prefix>
                <el-icon><User /></el-icon>
              </template>
            </el-input>
          </el-form-item>
          <el-form-item prop="password">
            <el-input
              v-model="formData.password"
              type="password"
              placeholder="请输入密码"
              show-password
              clearable
              @keyup.enter="submitForm()"
            >
              <template #prefix>
                <el-icon><Lock /></el-icon>
              </template>
            </el-input>
          </el-form-item>
          <el-form-item>
            <el-button type="primary" class="login-btn" @click="submitForm()">立即登录</el-button>
          </el-form-item>
        </el-form>
      </div>

      <div v-else class="oauth-wrapper">
        <div class="oauth-content">
          <p class="oauth-tip">选择授权平台</p>
          <div class="oauth-icons">
            <div class="oauth-icon-item" @click="handleOAuth2Login">
              <div class="icon-circle planet-bg">
                <div class="planet-surface"></div>
                <div class="orbit-dot"></div>
              </div>
              <span class="icon-label">01星球</span>
            </div>
            <!-- 可以预留其他平台 -->
            <div class="oauth-icon-item disabled">
              <div class="icon-circle gray-bg">
                <el-icon :size="32"><ChatDotRound /></el-icon>
              </div>
              <span class="icon-label">微信</span>
            </div>
            <div class="oauth-icon-item disabled">
              <div class="icon-circle gray-bg">
                <el-icon :size="32"><Position /></el-icon>
              </div>
              <span class="icon-label">钉钉</span>
            </div>
          </div>
        </div>
      </div>

      <!-- 测试示例访问入口 -->
      <div class="footer-links" v-if="showTestLink">
        <router-link to="/sample" class="sample-link">进入示例演示页面</router-link>
      </div>
    </div>
  </div>
  <!-- 验证码组件 -->
  <TianaiCaptcha mode="popup" v-model="showCaptcha" @success="doLogin" />
</template>

<script setup lang="ts">
import PolygonBackground from '@/components/background/PolygonBackground.vue'
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'
import { ref, reactive } from 'vue'
import { login } from '@/apis/login/index'
import { ElMessage } from 'element-plus'
import { useRouter } from 'vue-router'
import { useTabStore } from '@/stores/tab'
import { useUserStore } from '@/stores/user'
import { User, Lock, ChatDotRound, Position } from '@element-plus/icons-vue'
import { useAuthManager } from '@/plugins/auth-manager'
import { closeLoading, showLoading } from '@/apis/http'

// 是否为开发环境
const isDev = import.meta.env.DEV

// 登录模式：account 或 oauth2，仅开发环境支持 account 模式
const loginMode = ref<'account' | 'oauth2'>(isDev ? 'account' : 'oauth2')

// 是否显示示例演示界面连接
const showTestLink = ref(isDev)

// 是否启用验证码
const enableVerify = ref(!isDev)

// 获取router对象
const $router = useRouter()

// 定义登录数据对象
const formData = reactive({
  username: '',
  password: ''
})

/**
 * 执行登录
 * @param code 验证码字符串
 */
function doLogin(code: string) {
  // 发送登录请求
  login(
    {
      ...formData,
      code: code
    },
    () => {
      // 重置标签页数据
      useTabStore().reset()
      // 跳转到首页
      $router.push('/home')
      // 登录成功提示
      ElMessage.success('登录成功，前往首页')
    },
    () => {
      ElMessage.error('账号或密码错误')
    }
  )
}

// 定义登录提交函数
function submitForm() {
  if (!formData.username || !formData.password) {
    ElMessage.warning('请填写账号和密码')
    return
  }
  // 是否启用验证码
  if (!enableVerify.value) {
    doLogin('')
    return
  }
  // 弹出验证码框
  useVerify()
}

/**
 * 处理 OAuth2 登录
 */
function handleOAuth2Login() {
  // 显示加载中
  showLoading()
  // 重置保存数据
  useUserStore().resetSaveData()
  // 重置标签页数据
  useTabStore().reset()
  // 登录请求
  useAuthManager()
    .login()
    .catch((err) => {
      // 登录失败提示
      ElMessage.error('无法启动OAuth2登录流程')
      console.error(err)
    })
    .finally(() => {
      // 关闭加载中
      closeLoading()
    })
}

// 验证码显示状态
const showCaptcha = ref(false)

/**
 * 弹出验证码框
 */
function useVerify() {
  showCaptcha.value = true
}
</script>

<style scoped>
.login-container {
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
  overflow: hidden;
}

.glass-card {
  width: 500px;
  padding: 50px;
  background: var(--glass-bg);
  backdrop-filter: var(--glass-blur);
  -webkit-backdrop-filter: var(--glass-blur);
  border: 1px solid var(--glass-border);
  border-radius: 24px;
  box-shadow: var(--glass-shadow);
  transition: all 0.3s ease;
}

.login-header {
  margin-bottom: 40px;
  text-align: center;
}

.title {
  color: var(--title-color);
  font-size: 36px;
  margin-bottom: 25px;
  font-weight: 700;
  letter-spacing: 2px;
}

.mode-tabs {
  display: flex;
  justify-content: center;
  gap: 30px;
  margin-bottom: 15px;
}

.tab-item {
  padding: 10px 20px;
  cursor: pointer;
  color: #606266;
  font-size: 20px;
  position: relative;
  transition: all 0.3s ease;
}

.tab-item:after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 50%;
  width: 0;
  height: 3px;
  background: #409eff;
  transition: all 0.3s ease;
  transform: translateX(-50%);
}

.tab-item.active {
  color: #409eff;
  font-weight: bold;
}

.tab-item.active:after {
  width: 100%;
}

.form-wrapper {
  margin-top: 20px;
}

.login-btn {
  width: 100%;
  height: 54px;
  font-size: 20px;
  border-radius: 12px;
  background: var(--primary-gradient);
  border: none;
  box-shadow: 0 4px 15px rgba(25, 113, 194, 0.2);
  margin-top: 10px;
}

.login-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(25, 113, 194, 0.3);
}

.oauth-wrapper {
  padding: 30px 0 10px;
  text-align: center;
}

.oauth-tip {
  color: #909399;
  font-size: 18px;
  margin-bottom: 30px;
}

.oauth-icons {
  display: flex;
  justify-content: center;
  gap: 40px;
}

.oauth-icon-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
  cursor: pointer;
  transition: all 0.3s ease;
}

.oauth-icon-item:hover {
  transform: translateY(-5px);
}

.oauth-icon-item.disabled {
  cursor: not-allowed;
  opacity: 0.6;
}

.oauth-icon-item.disabled:hover {
  transform: none;
}

.icon-circle {
  width: 64px;
  height: 64px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  color: white;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.planet-bg {
  background: radial-gradient(circle at 30% 30%, #4facfe 0%, #00f2fe 40%, #0072ff 100%);
  position: relative;
  /* 移除 overflow: hidden 以免切割轨道上的光点 */
  box-shadow:
    inset -10px -10px 20px rgba(0, 0, 0, 0.4),
    0 0 20px rgba(79, 172, 254, 0.4);
}

.planet-surface {
  position: absolute;
  width: 100%;
  height: 100%;
  border-radius: 50%; /* 显式设置圆角以保持球体形状 */
  background-image:
    radial-gradient(circle at 70% 80%, rgba(255, 255, 255, 0.1) 0%, transparent 20%),
    radial-gradient(circle at 20% 60%, rgba(255, 255, 255, 0.1) 0%, transparent 15%),
    radial-gradient(circle at 50% 20%, rgba(255, 255, 255, 0.05) 0%, transparent 25%);
  opacity: 0.8;
}

.planet-bg::after {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  border-radius: 50%; /* 保持高光层为圆形 */
  background: linear-gradient(135deg, rgba(255, 255, 255, 0.2) 0%, transparent 50%);
  pointer-events: none;
}

.orbit-dot {
  position: absolute;
  top: 50%;
  left: 50%;
  width: 6px;
  height: 6px;
  margin: -3px 0 0 -3px;
  background-color: #fff;
  border-radius: 50%;
  box-shadow: 0 0 15px 2px #fff;
  animation: orbit 2.5s linear infinite;
  z-index: 10;
}

/* 增强的拖尾效果 */
.orbit-dot::before {
  content: '';
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  border-radius: 50%;
  box-shadow:
    -2px 0 10px 2px rgba(255, 255, 255, 1),
    -6px 0 15px 4px rgba(255, 255, 255, 0.8),
    -12px 0 20px 6px rgba(255, 255, 255, 0.5),
    -20px 0 30px 8px rgba(255, 255, 255, 0.3),
    -35px 0 45px 12px rgba(255, 255, 255, 0.1);
  filter: blur(1px);
}

@keyframes orbit {
  from {
    transform: rotate(0deg) translateY(-32px);
  }
  to {
    transform: rotate(360deg) translateY(-32px);
  }
}

.gray-bg {
  background: #e4e7ed;
  color: #909399;
}

.icon-label {
  font-size: 16px;
  color: #606266;
}

.footer-links {
  margin-top: 35px;
  text-align: center;
  border-top: 1px solid rgba(255, 255, 255, 0.2);
  padding-top: 20px;
}

.sample-link {
  color: #409eff;
  text-decoration: none;
  font-size: 16px;
}

.sample-link:hover {
  text-decoration: underline;
}

:deep(.el-input__wrapper) {
  background-color: rgba(255, 255, 255, 0.5) !important;
  box-shadow: none !important;
  border: 1px solid rgba(255, 255, 255, 0.5);
  border-radius: 10px;
  height: 48px;
}

:deep(.el-input__inner) {
  font-size: 18px;
}

:deep(.el-input__wrapper.is-focus) {
  border-color: #409eff;
}
</style>
