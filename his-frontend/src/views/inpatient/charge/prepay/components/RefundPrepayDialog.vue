<script setup lang="ts">
import type { PrepayChannel, PrepayRecord } from '@/apis/inpatient/charge/prepay'
import { computed, reactive, ref } from 'vue'
const props = defineProps<{ record: PrepayRecord | null; balance: number; submitting: boolean }>()
const emit = defineEmits<{ confirm: [data: { amount: number; channel: PrepayChannel }] }>()
const visible = ref(false)
const form = reactive<{ amount: number; channel: PrepayChannel }>({ amount: 0, channel: 'card' })
const maxRefund = computed(() => Math.min(props.balance, props.record?.refundableAmount ?? 0))
function open(record: PrepayRecord) {
  form.amount = Math.min(props.balance, record.refundableAmount)
  form.channel = record.channel
  visible.value = true
}
function close() {
  visible.value = false
}
function submit() {
  if (form.amount > 0 && form.amount <= maxRefund.value) emit('confirm', { ...form })
}
defineExpose({ open, close })
</script>
<template>
  <el-dialog v-model="visible" title="退还住院预交金" width="520px" :close-on-click-modal="false"
    ><el-alert
      type="warning"
      :closable="false"
      :title="`当前记录最多可退 ¥${maxRefund.toFixed(2)}`"
    /><el-form label-width="90px" class="refund-form"
      ><el-form-item label="原交易号"
        ><el-input :model-value="record?.transactionNo" disabled /></el-form-item
      ><el-form-item label="退款金额" required
        ><el-input-number
          v-model="form.amount"
          :min="0.01"
          :max="maxRefund"
          :precision="2"
          style="width: 100%" /></el-form-item
      ><el-form-item label="退款渠道"
        ><el-select v-model="form.channel" disabled
          ><el-option label="现金" value="cash" /><el-option
            label="银行卡"
            value="card" /><el-option label="微信扫码" value="wechat" /><el-option
            label="支付宝扫码"
            value="alipay" /></el-select></el-form-item></el-form
    ><template #footer
      ><el-button :disabled="submitting" @click="close">取消</el-button
      ><el-button type="danger" :loading="submitting" @click="submit">确认退款</el-button></template
    ></el-dialog
  >
</template>
<style scoped>
.refund-form {
  margin-top: 16px;
}
</style>
