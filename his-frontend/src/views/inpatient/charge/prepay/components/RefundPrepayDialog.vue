<script setup lang="ts">
import type { PrepayChannel, PrepayRecord } from '@/apis/inpatient/charge/prepay'
import { computed, reactive, ref } from 'vue'
const props = defineProps<{ record: PrepayRecord | null; balance: number; submitting: boolean }>()
interface RefundForm {
  amount: number
  channel: PrepayChannel
  scanCode: string
  remark: string
}
const emit = defineEmits<{ confirm: [data: RefundForm] }>()
const visible = ref(false)
const form = reactive<RefundForm>({ amount: 0, channel: 'card', scanCode: '', remark: '' })
const maxRefund = computed(() => Math.min(props.balance, props.record?.refundableAmount ?? 0))
const submitDisabled = computed(
  () =>
    form.amount <= 0 ||
    form.amount > maxRefund.value ||
    (['wechat', 'alipay'].includes(form.channel) && !form.scanCode.trim())
)
function open(record: PrepayRecord) {
  form.amount = Math.min(props.balance, record.refundableAmount)
  form.channel = record.channel
  form.scanCode = ''
  form.remark = ''
  visible.value = true
}
function close() {
  visible.value = false
}
function submit() {
  if (form.amount <= 0 || form.amount > maxRefund.value) return
  if (['wechat', 'alipay'].includes(form.channel) && !form.scanCode.trim()) return
  emit('confirm', { ...form, scanCode: form.scanCode.trim(), remark: form.remark.trim() })
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
            value="alipay" /></el-select></el-form-item
      ><el-form-item v-if="['wechat', 'alipay'].includes(form.channel)" label="退款授权码" required
        ><el-input
          v-model="form.scanCode"
          maxlength="64"
          placeholder="请扫描或输入退款授权码" /></el-form-item
      ><el-form-item label="备注"
        ><el-input v-model="form.remark" maxlength="100" show-word-limit /></el-form-item></el-form
    ><template #footer
      ><el-button :disabled="submitting" @click="close">取消</el-button
      ><el-button type="danger" :loading="submitting" :disabled="submitDisabled" @click="submit"
        >确认退款</el-button
      ></template
    ></el-dialog
  >
</template>
<style scoped>
.refund-form {
  margin-top: 16px;
}
</style>
