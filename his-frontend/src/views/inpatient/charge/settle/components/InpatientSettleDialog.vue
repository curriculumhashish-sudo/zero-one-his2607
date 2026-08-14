<script setup lang="ts">
import type {
  InpatientSettleInitResult,
  PreSettleInpatientResult
} from '@/apis/inpatient/charge/settle'

import { computed, reactive, watch } from 'vue'
import { ElMessage } from 'element-plus'

import DetailSummary from '@/components/common/DetailSummary.vue'

const props = defineProps<{
  init: InpatientSettleInitResult | null
  modelValue: boolean
  preview: PreSettleInpatientResult | null
  submitting: boolean
}>()

const emit = defineEmits<{
  confirm: [data: { payChannelCode: string; scanCode?: string }]
  'update:modelValue': [value: boolean]
}>()

const form = reactive({ payChannelCode: 'CASH', scanCode: '' })
const payableAmount = computed(() => {
  const data = props.preview
  if (!data) return 0
  return Number(data.insurancePreSettled ? (data.displayAmount ?? 0) : data.receivableTotal)
})
const showScanInput = computed(() => form.payChannelCode === 'SCAN_PAY' && payableAmount.value > 0)
const items = computed(() => {
  const data = props.preview
  if (!data) return []
  return [
    { key: 'total', label: '应收金额', value: `¥${Number(data.receivableTotal).toFixed(2)}` },
    { key: 'fund', label: '医保基金', value: `¥${Number(data.ybFundPaySumamt ?? 0).toFixed(2)}` },
    { key: 'account', label: '个人账户', value: `¥${Number(data.ybAcctPay ?? 0).toFixed(2)}` },
    {
      key: 'cash',
      label: '个人现金',
      value: `¥${Number(data.ybPsnCashPay ?? data.receivableTotal).toFixed(2)}`
    }
  ]
})

watch(
  () => props.modelValue,
  (visible) => {
    if (visible) {
      form.payChannelCode = props.init?.paymentChannelOptions[0]?.value ?? 'CASH'
      form.scanCode = ''
    }
  }
)

function confirm() {
  if (!form.payChannelCode) {
    ElMessage.warning('请选择支付渠道')
    return
  }
  if (showScanInput.value && !form.scanCode.trim()) {
    ElMessage.warning('请输入付款码')
    return
  }
  emit('confirm', {
    payChannelCode: form.payChannelCode,
    scanCode: showScanInput.value ? form.scanCode.trim() : undefined
  })
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="住院结算确认"
    width="620px"
    :close-on-click-modal="false"
    :close-on-press-escape="!submitting"
    @update:model-value="(value: boolean) => emit('update:modelValue', value)"
  >
    <el-alert
      v-if="preview?.previewText"
      :title="preview.previewText"
      type="info"
      :closable="false"
      show-icon
    />
    <DetailSummary class="settle-preview" title="预结算结果" :items="items" :column="2" />
    <el-form label-width="90px">
      <el-form-item label="支付渠道" required>
        <el-select v-model="form.payChannelCode" :disabled="submitting" style="width: 100%">
          <el-option
            v-for="option in init?.paymentChannelOptions ?? []"
            :key="option.value"
            :label="option.label"
            :value="option.value"
            :disabled="option.disabled"
          />
        </el-select>
      </el-form-item>
      <el-form-item v-if="showScanInput" label="付款码" required>
        <el-input
          v-model="form.scanCode"
          :disabled="submitting"
          placeholder="请扫描或输入付款码"
          @keyup.enter="confirm"
        />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button :disabled="submitting" @click="emit('update:modelValue', false)">取消</el-button>
      <el-button type="primary" :loading="submitting" @click="confirm">确认结算</el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.settle-preview {
  margin: 12px 0 18px;
}
</style>
