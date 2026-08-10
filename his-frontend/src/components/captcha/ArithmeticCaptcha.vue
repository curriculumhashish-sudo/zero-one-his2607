<template>
  <el-dialog
    :model-value="modelValue"
    title="安全验证"
    width="360px"
    :close-on-click-modal="false"
    :show-close="true"
    append-to-body
    @update:model-value="handleVisibleChange"
  >
    <div class="arithmetic-captcha">
      <p class="captcha-tip">请输入下方图片中的计算结果</p>
      <div class="captcha-image-wrapper">
        <img v-if="imageSrc" :src="imageSrc" alt="验证码" class="captcha-image" />
        <div v-else class="captcha-placeholder">验证码加载中...</div>
      </div>
      <el-input
        v-model="answer"
        placeholder="请输入计算结果"
        autocomplete="off"
        class="captcha-input"
        clearable
        @keyup.enter="handleConfirm"
      />
    </div>
    <template #footer>
      <el-button @click="handleCancel">取消</el-button>
      <el-button type="primary" :disabled="!answer.trim()" @click="handleConfirm">确认</el-button>
    </template>
  </el-dialog>
</template>

<script setup lang="ts">
import { computed, ref, watch } from 'vue'

const props = defineProps<{
  /** v-model 控制显示/隐藏 */
  modelValue: boolean
  /** base64 图片，可以带 data: 前缀 */
  image: string
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  success: [code: string]
  error: []
}>()

const answer = ref('')

/** 计算图片 src，自动补全 data: 前缀 */
const imageSrc = computed(() => {
  if (!props.image) return ''
  if (props.image.startsWith('data:')) return props.image
  return `data:image/png;base64,${props.image}`
})

/** 弹窗打开时清空输入 */
watch(
  () => props.modelValue,
  (val) => {
    if (val) answer.value = ''
  }
)

function handleConfirm() {
  const code = answer.value.trim()
  if (!code) {
    emit('error')
    return
  }
  emit('success', code)
  emit('update:modelValue', false)
}

function handleCancel() {
  emit('update:modelValue', false)
}

function handleVisibleChange(val: boolean) {
  emit('update:modelValue', val)
}
</script>

<style scoped>
.arithmetic-captcha {
  display: flex;
  flex-direction: column;
  gap: 16px;
  align-items: center;
}
.captcha-tip {
  margin: 0;
  color: #6b7280;
  font-size: 14px;
  text-align: center;
}
.captcha-image-wrapper {
  display: flex;
  justify-content: center;
  align-items: center;
  width: 100%;
}
.captcha-image {
  max-width: 100%;
  height: 60px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
}
.captcha-placeholder {
  width: 200px;
  height: 60px;
  display: flex;
  align-items: center;
  justify-content: center;
  color: #909399;
  font-size: 14px;
  border: 1px dashed #dcdfe6;
  border-radius: 4px;
}
.captcha-input {
  width: 100%;
}
</style>
