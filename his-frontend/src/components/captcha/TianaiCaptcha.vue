<template>
  <!-- 容器：由外部父组件决定位置或隐藏 -->
  <div v-if="mode === 'default'" class="tianai-embedded" id="captcha-div"></div>

  <!-- 弹窗模式 -->
  <Teleport to="body">
    <div
      v-if="mode === 'popup' && visible"
      class="tianai-overlay"
      id="captcha-div"
      @click.self="close"
    >
      <section class="captcha-fallback" role="dialog" aria-modal="true" aria-labelledby="captcha-title">
        <h2 id="captcha-title">安全验证</h2>
        <div class="captcha-image-row">
          <img v-if="props.captcha?.imageBase64" :src="toImageSrc(props.captcha.imageBase64)" width="132" height="48" alt="图片验证码" />
          <canvas v-else ref="captchaCanvas" width="132" height="48" role="img" aria-label="图片验证码" />
          <button type="button" class="captcha-refresh" aria-label="刷新图片验证码" title="刷新图片验证码" @click="handleRefresh">刷新</button>
        </div>
        <input
          v-model="localInput"
          class="captcha-input"
          aria-label="图片验证码输入"
          autocomplete="off"
          maxlength="4"
          placeholder="请输入图片中的字符"
          @keyup.enter="completeLocalFallback"
        />
        <button type="button" class="captcha-confirm" :disabled="props.captcha?.imageBase64 ? !localInput.trim() : localInput.length !== 4" @click="completeLocalFallback">验证</button>
      </section>
    </div>
  </Teleport>
</template>

<script setup lang="ts">
import { computed, ref, watch, onMounted, onUnmounted, nextTick, type PropType } from 'vue'
import type { CaptchaResult } from '@/apis/login'

// 属性支持
const props = defineProps({
  /** 验证码显示模式 default-内置模式，popup-弹窗模式 */
  mode: { type: String, default: 'default' },
  /** 弹窗模式控制显隐 */
  modelValue: { type: Boolean, default: false },
  /** 验证码参数 */
  options: { type: Object, default: () => ({}) },
  /** 验证码类型 可以指定的值 SLIDER|ROTATE|CONCAT|WORD_IMAGE_CLICK */
  type: { type: String, default: '' },
  /** 后端返回的图片验证码；未提供时使用本地开发回退验证码 */
  captcha: { type: Object as PropType<CaptchaResult>, default: undefined }
})

// 事件支持
const emit = defineEmits(['update:modelValue', 'success', 'error', 'refresh'])
// 弹窗显隐控制变量
const visible = ref(props.modelValue)
const localFallback = computed(() => !props.captcha?.imageBase64)
const captchaCanvas = ref<HTMLCanvasElement>()
const localCode = ref('')
const localInput = ref('')

function generateLocalCaptcha() {
  const chars = '23456789ABCDEFGHJKLMNPQRSTUVWXYZ'
  localCode.value = Array.from({ length: 4 }, () => chars[Math.floor(Math.random() * chars.length)]).join('')
  localInput.value = ''
  nextTick(() => {
    const canvas = captchaCanvas.value
    const context = canvas?.getContext('2d')
    if (!canvas || !context) return
    context.clearRect(0, 0, canvas.width, canvas.height)
    context.fillStyle = '#f3f6fb'
    context.fillRect(0, 0, canvas.width, canvas.height)
    for (let index = 0; index < 7; index += 1) {
      context.strokeStyle = `hsl(${Math.random() * 360} 60% 65%)`
      context.beginPath()
      context.moveTo(Math.random() * canvas.width, Math.random() * canvas.height)
      context.lineTo(Math.random() * canvas.width, Math.random() * canvas.height)
      context.stroke()
    }
    context.font = 'bold 28px Arial'
    context.textBaseline = 'middle'
    localCode.value.split('').forEach((char, index) => {
      context.save()
      context.translate(17 + index * 31, 25)
      context.rotate((Math.random() - 0.5) * 0.35)
      context.fillStyle = `hsl(${Math.random() * 360} 55% 35%)`
      context.fillText(char, 0, 0)
      context.restore()
    })
  })
}

function toImageSrc(value: string) {
  return value.startsWith('data:') ? value : 'data:image/png;base64,' + value
}

function handleRefresh() {
  if (props.captcha?.imageBase64) {
    emit('refresh')
  } else {
    generateLocalCaptcha()
  }
}

const completeLocalFallback = () => {
  if (props.captcha?.imageBase64) {
    if (!localInput.value.trim()) {
      emit('error')
      return
    }
    emit('success', { id: props.captcha.id, code: localInput.value.trim() })
    close()
    return
  }
  if (localInput.value.trim().toUpperCase() !== localCode.value) {
    emit('error')
    generateLocalCaptcha()
    return
  }
  emit('success', `local-image-captcha-${Date.now()}`)
  close()
}

// 动态构建验证码请求地址
const getRequestUrl = () => {
  const baseUrl = `${import.meta.env.VITE_CAPTCHA_URL}/${import.meta.env.VITE_CAPTCHA_PREFIX}/gen`
  // 如果type存在则拼接参数，否则保持原样
  return props.type ? `${baseUrl}?type=${props.type}` : baseUrl
}

// 验证码配置
const captchaConfig = {
  // 请求验证码接口
  requestCaptchaDataUrl: getRequestUrl(),
  // 验证验证码接口
  validCaptchaUrl: `${import.meta.env.VITE_CAPTCHA_URL}/${import.meta.env.VITE_CAPTCHA_PREFIX}/check`,
  // 合并外部参数
  ...props.options,
  // 绑定的div
  bindEl: '#captcha-div',
  // 验证成功回调函数
  validSuccess: (res: any) => {
    // 执行成功回调
    emit('success', res.data.id)
    // 弹窗模式下关闭弹窗
    if (props.mode === 'popup') close()
  },
  //关闭按钮回调函数
  btnCloseFun: () => {
    close()
  }
}

// 验证码样式配置
const style = {
  // logo图标地址
  logoUrl: '/logo.jpg'
}

// 验证码实例
let captchaInstance: any = null

// 初始化逻辑
const init = () => {
  if (localFallback.value || props.captcha?.imageBase64) return
  window
    .initTAC('static/tac', captchaConfig, style)
    .then((tac) => {
      // 调用init则显示验证码
      tac.init()
      captchaInstance = tac
    })
    .catch((e) => {
      emit('error')
    })
}

// 关闭弹窗
const close = () => {
  captchaInstance?.destroyWindow()
  localInput.value = ''
  localCode.value = ''
  visible.value = false
  emit('update:modelValue', false)
}

// 监听弹窗打开时初始化
watch(
  () => props.modelValue,
  (newVal) => {
    visible.value = newVal
    if (newVal) {
      if (localFallback.value) generateLocalCaptcha()
      else if (props.captcha?.imageBase64) return
      else nextTick(() => init())
    }
  }
)

// 初始化组件
onMounted(() => {
  if (props.mode === 'default') init()
})

// 销毁组件
onUnmounted(() => {
  captchaInstance?.destroyWindow()
})

// 暴露方法给父组件
defineExpose({
  // 重新加载验证码
  reloadCaptcha: () => captchaInstance?.reloadCaptcha()
})
</script>

<style scoped>
.tianai-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 999;
}

.tianai-embedded {
  width: 100%;
  min-height: 200px;
}

.captcha-fallback {
  width: min(86vw, 320px);
  padding: 24px;
  border-radius: 8px;
  background: #fff;
  box-shadow: 0 12px 32px rgb(15 23 42 / 24%);
  text-align: center;
}

.captcha-fallback h2 { margin: 0; color: #111827; font-size: 18px; }
.captcha-image-row { display: flex; align-items: center; justify-content: center; gap: 10px; margin: 18px 0 12px; }
.captcha-image-row canvas { width: 132px; height: 48px; border: 1px solid #d9e2f0; border-radius: 4px; }
.captcha-refresh, .captcha-confirm { border: 0; border-radius: 4px; padding: 8px 12px; color: #fff; background: #2563eb; cursor: pointer; }
.captcha-refresh { color: #2563eb; background: #e8f0ff; }
.captcha-input {
  width: 100%;
  box-sizing: border-box;
  height: 40px;
  border: 1px solid #cbd5e1;
  border-radius: 4px;
  padding: 0 10px;
  font-size: 16px;
  text-transform: uppercase;
}
.captcha-confirm { width: 100%; margin-top: 12px; }
.captcha-confirm:disabled { cursor: not-allowed; opacity: .5; }
</style>
