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
    ></div>
  </Teleport>
</template>

<script setup lang="ts">
import { ref, watch, onMounted, onUnmounted, nextTick } from 'vue'

// 属性支持
const props = defineProps({
  /** 验证码显示模式 default-内置模式，popup-弹窗模式 */
  mode: { type: String, default: 'default' },
  /** 弹窗模式控制显隐 */
  modelValue: { type: Boolean, default: false },
  /** 验证码参数 */
  options: { type: Object, default: () => ({}) },
  /** 验证码类型 可以指定的值 SLIDER|ROTATE|CONCAT|WORD_IMAGE_CLICK */
  type: { type: String, default: '' }
})

// 事件支持
const emit = defineEmits(['update:modelValue', 'success', 'error'])
// 弹窗显隐控制变量
const visible = ref(props.modelValue)

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
  window
    .initTAC('static/tac', captchaConfig, style)
    .then((tac) => {
      // 调用init则显示验证码
      tac.init()
      captchaInstance = tac
    })
    .catch((e) => {
      emit('error')
      console.log('初始化tac失败', e)
    })
}

// 关闭弹窗
const close = () => {
  captchaInstance?.destroyWindow()
  visible.value = false
  emit('update:modelValue', false)
}

// 监听弹窗打开时初始化
watch(
  () => props.modelValue,
  (newVal) => {
    visible.value = newVal
    if (newVal) {
      nextTick(() => init())
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
</style>
