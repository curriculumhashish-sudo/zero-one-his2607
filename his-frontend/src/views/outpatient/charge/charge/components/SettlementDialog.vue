<script setup lang="ts">
import type { PaymentChannel } from '@/apis/outpatient/charge/charge'
import type { ChargeSettlementView } from '../type'
import { computed, ref } from 'vue'
const props = defineProps<{ data: ChargeSettlementView | null; submitting: boolean }>()
const emit = defineEmits<{
  confirm: [data: { channel: PaymentChannel; scanCode?: string }]
}>()
const visible = ref(false)
const channel = ref<PaymentChannel>('card')
const scanCode = ref('')
const options: Array<{ value: PaymentChannel; label: string }> = [
  { value: 'cash', label: '现金' },
  { value: 'card', label: '银行卡' },
  { value: 'wechat', label: '微信扫码' },
  { value: 'alipay', label: '支付宝扫码' }
]
const amount = computed(() => props.data?.summary.totalAmount ?? 0)
const availableOptions = computed(() =>
  options.filter((item) => props.data?.channels.includes(item.value) ?? false)
)
const submitDisabled = computed(
  () => props.submitting || (['wechat', 'alipay'].includes(channel.value) && !scanCode.value.trim())
)
function open() {
  channel.value = 'card'
  scanCode.value = ''
  visible.value = true
}
function close() {
  visible.value = false
}
function submit() {
  if (['wechat', 'alipay'].includes(channel.value) && !scanCode.value.trim()) return
  emit('confirm', { channel: channel.value, scanCode: scanCode.value.trim() || undefined })
}
defineExpose({ open, close })
</script>
<template>
  <el-dialog v-model="visible" title="门诊收费结算" width="560px" :close-on-click-modal="false"
    ><el-descriptions v-if="data" :column="2" border
      ><el-descriptions-item label="应收金额"
        ><strong class="amount">¥{{ amount.toFixed(2) }}</strong></el-descriptions-item
      ><el-descriptions-item label="医保支付"
        >¥{{ data.summary.insuranceAmount.toFixed(2) }}</el-descriptions-item
      ><el-descriptions-item label="个人支付"
        >¥{{ data.summary.selfPayAmount.toFixed(2) }}</el-descriptions-item
      ><el-descriptions-item label="项目数">{{
        data.chargeItemIds.length
      }}</el-descriptions-item></el-descriptions
    ><el-form label-width="90px" class="pay-form"
      ><el-form-item label="支付渠道"
        ><el-radio-group v-model="channel"
          ><el-radio
            v-for="item in availableOptions"
            :key="item.value"
            :value="item.value"
            border
            >{{ item.label }}</el-radio
          ></el-radio-group
        ></el-form-item
      ><el-alert
        v-if="['wechat', 'alipay'].includes(channel)"
        type="info"
        :closable="false"
        title="提交后进入扫码支付确认，可查询或恢复未完成交易。" /><el-form-item
        v-if="['wechat', 'alipay'].includes(channel)"
        label="付款码"
        required
        ><el-input
          v-model="scanCode"
          maxlength="64"
          placeholder="请扫描或输入患者付款码" /></el-form-item></el-form
    ><template #footer
      ><el-button :disabled="submitting" @click="close">取消</el-button
      ><el-button type="primary" :loading="submitting" :disabled="submitDisabled" @click="submit"
        >确认结算</el-button
      ></template
    ></el-dialog
  >
</template>
<style scoped>
.amount {
  color: var(--el-color-danger);
  font-size: 20px;
}
.pay-form {
  margin-top: 18px;
}
.el-radio-group {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}
.el-radio {
  margin-right: 0;
}
</style>
