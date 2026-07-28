<script setup lang="ts">
import type { PrepayChannel } from '@/apis/inpatient/charge/prepay'
import { reactive, ref } from 'vue'
defineProps<{ submitting: boolean }>()
const emit = defineEmits<{ confirm: [data: { amount: number; channel: PrepayChannel }] }>()
const visible = ref(false)
const form = reactive<{ amount: number; channel: PrepayChannel }>({ amount: 1000, channel: 'card' })
const quickAmounts = [500, 1000, 2000, 5000]
const channels: Array<{ value: PrepayChannel; label: string }> = [
  { value: 'cash', label: '现金' },
  { value: 'card', label: '银行卡' },
  { value: 'wechat', label: '微信扫码' },
  { value: 'alipay', label: '支付宝扫码' }
]
function open() {
  Object.assign(form, { amount: 1000, channel: 'card' })
  visible.value = true
}
function close() {
  visible.value = false
}
function submit() {
  if (form.amount > 0) emit('confirm', { ...form })
}
defineExpose({ open, close })
</script>
<template>
  <el-dialog v-model="visible" title="收取住院预交金" width="520px" :close-on-click-modal="false"
    ><el-form label-width="90px"
      ><el-form-item label="快捷金额"
        ><el-button
          v-for="amount in quickAmounts"
          :key="amount"
          :type="form.amount === amount ? 'primary' : 'default'"
          @click="form.amount = amount"
          >{{ amount }}元</el-button
        ></el-form-item
      ><el-form-item label="收款金额" required
        ><el-input-number
          v-model="form.amount"
          :min="1"
          :max="1000000"
          :precision="2"
          style="width: 100%" /></el-form-item
      ><el-form-item label="支付渠道"
        ><el-select v-model="form.channel"
          ><el-option
            v-for="item in channels"
            :key="item.value"
            :label="item.label"
            :value="item.value" /></el-select></el-form-item
      ><el-alert
        v-if="['wechat', 'alipay'].includes(form.channel)"
        type="info"
        :closable="false"
        title="扫码收款提交后需确认支付结果。" /></el-form
    ><template #footer
      ><el-button :disabled="submitting" @click="close">取消</el-button
      ><el-button type="primary" :loading="submitting" @click="submit"
        >确认收取</el-button
      ></template
    ></el-dialog
  >
</template>
