<script setup lang="ts">
import type { RefundVerificationView } from '../type'
import { computed, ref } from 'vue'
const props = defineProps<{ data: RefundVerificationView | null; submitting: boolean }>()
const emit = defineEmits<{
  confirm: [data: { retainedMode: 'insurance' | 'self-pay'; scanCode?: string }]
}>()
const visible = ref(false)
const retainedMode = ref<'insurance' | 'self-pay'>('insurance')
const scanCode = ref('')
function open() {
  retainedMode.value = 'insurance'
  scanCode.value = ''
  visible.value = true
}
function close() {
  visible.value = false
}
function submit() {
  const requiresScan = ['wechat', 'alipay'].includes(String(dataRef.value?.channel ?? ''))
  if (requiresScan && !scanCode.value.trim()) return
  emit('confirm', {
    retainedMode: retainedMode.value,
    scanCode: scanCode.value.trim() || undefined
  })
}
const dataRef = computed(() => props.data)
const submitDisabled = computed(
  () =>
    props.submitting ||
    (['wechat', 'alipay'].includes(String(props.data?.channel ?? '')) && !scanCode.value.trim())
)
defineExpose({ open, close })
</script>
<template>
  <el-dialog v-model="visible" title="门诊退费确认" width="560px" :close-on-click-modal="false"
    ><el-alert
      v-if="data"
      :title="data.message"
      type="warning"
      :closable="false"
      show-icon
    /><el-descriptions v-if="data" :column="2" border class="refund-summary"
      ><el-descriptions-item label="原支付记录">{{ data.paymentRecordId }}</el-descriptions-item
      ><el-descriptions-item label="原支付渠道">{{ data.channel }}</el-descriptions-item
      ><el-descriptions-item label="应退金额"
        ><strong>¥{{ data.refundAmount.toFixed(2) }}</strong></el-descriptions-item
      ><el-descriptions-item label="保留项目金额"
        >¥{{ data.retainedAmount.toFixed(2) }}</el-descriptions-item
      ></el-descriptions
    ><el-form v-if="data?.retainedAmount" label-width="120px" class="retained-form"
      ><el-form-item label="保留项目费用属性"
        ><el-radio-group v-model="retainedMode"
          ><el-radio value="insurance">医保</el-radio
          ><el-radio value="self-pay">自费</el-radio></el-radio-group
        ></el-form-item
      ></el-form
    ><el-form v-if="['wechat', 'alipay'].includes(String(data?.channel ?? ''))" label-width="120px"
      ><el-form-item label="退款授权码" required
        ><el-input
          v-model="scanCode"
          maxlength="64"
          placeholder="请扫描或输入退款授权码" /></el-form-item></el-form
    ><template #footer
      ><el-button :disabled="submitting" @click="close">取消</el-button
      ><el-button type="danger" :loading="submitting" :disabled="submitDisabled" @click="submit"
        >确认退费</el-button
      ></template
    ></el-dialog
  >
</template>
<style scoped>
.refund-summary {
  margin-top: 16px;
}
.refund-summary strong {
  color: var(--el-color-danger);
  font-size: 19px;
}
.retained-form {
  margin-top: 16px;
}
</style>
