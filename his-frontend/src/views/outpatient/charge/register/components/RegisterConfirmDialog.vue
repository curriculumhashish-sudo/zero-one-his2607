<script setup lang="ts">
import type {
  PreSettleOutpatientRegisterResult,
  RegisterInitResult
} from '@/apis/outpatient/charge/register'

import { computed, ref, watch } from 'vue'
import { ElMessage } from 'element-plus'

import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'

const props = defineProps<{
  init: RegisterInitResult | null
  modelValue: boolean
  preview: PreSettleOutpatientRegisterResult | null
  submitting: boolean
}>()
const emit = defineEmits<{
  confirm: [data: { channel: string; scanCode?: string }]
  'update:modelValue': [value: boolean]
}>()
const channel = ref('CASH')
const scanCode = ref('')
const payableAmount = computed(() => {
  const data = props.preview
  if (!data) return 0
  return Number(data.insurancePreSettled ? (data.ybPsnCashPay ?? 0) : data.totalAmount)
})
const showScanInput = computed(() => channel.value === 'SCAN_PAY' && payableAmount.value > 0)
const items = computed(() => {
  if (!props.preview) return []
  return [
    { key: 'total', label: '应收金额', value: `¥${Number(props.preview.totalAmount).toFixed(2)}` },
    {
      key: 'fund',
      label: '医保基金',
      value: `¥${Number(props.preview.ybFundPaySumamt ?? 0).toFixed(2)}`
    },
    {
      key: 'account',
      label: '个人账户',
      value: `¥${Number(props.preview.ybAcctPay ?? 0).toFixed(2)}`
    },
    {
      key: 'cash',
      label: '个人支付',
      value: `¥${Number(props.preview.ybPsnCashPay ?? props.preview.totalAmount).toFixed(2)}`
    }
  ]
})
watch(
  () => props.modelValue,
  (visible) => {
    if (visible) {
      channel.value = props.init?.paymentChannelOptions?.[0]?.value ?? 'CASH'
      scanCode.value = ''
    }
  }
)

function confirm() {
  if (showScanInput.value && !scanCode.value.trim()) {
    ElMessage.warning('请输入付款码')
    return
  }
  emit('confirm', {
    channel: channel.value,
    scanCode: showScanInput.value ? scanCode.value.trim() : undefined
  })
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="挂号与缴费确认"
    width="680px"
    :close-on-click-modal="false"
    @update:model-value="(value: boolean) => emit('update:modelValue', value)"
  >
    <DetailSummary title="费用试算" :items="items" :column="2" />
    <DataTablePanel
      class="charge-items"
      :data="preview?.chargeItems ?? []"
      :total="preview?.chargeItems.length ?? 0"
      min-height="180px"
    >
      <el-table-column prop="chargeItemName" label="收费项目" min-width="160" />
      <el-table-column prop="quantity" label="数量" width="80" />
      <el-table-column label="单价" width="100"
        ><template #default="{ row }"
          >¥{{ Number(row.unitPrice).toFixed(2) }}</template
        ></el-table-column
      >
      <el-table-column label="金额" width="100"
        ><template #default="{ row }"
          >¥{{ Number(row.amount).toFixed(2) }}</template
        ></el-table-column
      >
    </DataTablePanel>
    <el-form label-width="90px">
      <el-form-item label="支付渠道" required>
        <el-select v-model="channel" :disabled="submitting" style="width: 100%">
          <el-option
            v-for="item in init?.paymentChannelOptions ?? []"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
      </el-form-item>
      <el-form-item v-if="showScanInput" label="付款码" required>
        <el-input
          v-model="scanCode"
          :disabled="submitting"
          placeholder="请扫描或输入付款码"
          @keyup.enter="confirm"
        />
      </el-form-item>
    </el-form>
    <template #footer
      ><el-button :disabled="submitting" @click="emit('update:modelValue', false)"
        >返回修改</el-button
      ><el-button type="primary" :loading="submitting" @click="confirm"
        >确认缴费并挂号</el-button
      ></template
    >
  </el-dialog>
</template>

<style scoped>
.charge-items {
  margin: 12px 0;
}
</style>
