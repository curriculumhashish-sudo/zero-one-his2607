<script setup lang="ts">
import { computed, nextTick, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  applyDoctorRefund,
  queryDoctorRefundItems,
  type DoctorRefundItemVO
} from '@/apis/outpatient/doctor-station'
import DetailSummary from '@/components/common/DetailSummary.vue'
import type { DetailSummaryItem } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'

defineOptions({
  name: 'RefundApplicationDialog'
})

interface RefundApplicationDialogProps {
  modelValue: boolean
  patient?: PatientSummary | null
}

const props = withDefaults(defineProps<RefundApplicationDialogProps>(), {
  patient: null
})

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  submitted: [applicationNo: string]
}>()

interface RefundTableRow extends DoctorRefundItemVO {
  /** 后端没有提供行 ID，页面使用收费信息组合生成稳定表格键。 */
  _rowKey: string
}

interface RefundTableInstance {
  clearSelection: () => void
  toggleRowSelection: (row: RefundTableRow, selected?: boolean) => void
}

const tableRef = ref<RefundTableInstance>()
const tableRows = ref<RefundTableRow[]>([])
const selectedRows = ref<RefundTableRow[]>([])
const loading = ref(false)
const submitting = ref(false)
const loadError = ref('')
let requestSequence = 0

function display(value: string | number | null | undefined) {
  const text = value == null ? '' : String(value).trim()
  return text || '—'
}

const patientSummaryItems = computed<DetailSummaryItem[]>(() => [
  { key: 'name', label: '患者姓名', value: display(props.patient?.name) },
  { key: 'patientNo', label: '病历号', value: display(props.patient?.patientNo) },
  { key: 'visitNo', label: '就诊号', value: display(props.patient?.visitNo) },
  { key: 'department', label: '就诊科室', value: display(props.patient?.departmentName) }
])

function amountToCents(value: string | number | undefined) {
  const amount = Number(value ?? 0)
  return Number.isFinite(amount) ? Math.round(amount * 100) : 0
}

function formatMoneyFromCents(cents: number) {
  return (cents / 100).toFixed(2)
}

function formatMoney(value: string | number | undefined) {
  return formatMoneyFromCents(amountToCents(value))
}

function isRefundable(row: DoctorRefundItemVO) {
  const chargeStatus = row.chargeStatus?.trim() ?? ''
  const refundStatus = row.refundStatus?.trim() ?? ''
  return (
    ['已收费', '已支付'].includes(chargeStatus) &&
    ['', '未申请', '未退费', '可申请'].includes(refundStatus)
  )
}

const refundableRows = computed(() => tableRows.value.filter(isRefundable))
const selectedAmountCents = computed(() =>
  selectedRows.value.reduce((sum, row) => sum + amountToCents(row.amount), 0)
)

function createTableRows(items: DoctorRefundItemVO[]): RefundTableRow[] {
  return items.map((item, index) => ({
    ...item,
    _rowKey: [item.paymentNo, item.itemName, item.quantity, item.amount, index].join('|')
  }))
}

function toApiItem(row: RefundTableRow): DoctorRefundItemVO {
  const item: DoctorRefundItemVO & { _rowKey?: string } = { ...row }
  delete item._rowKey
  return item
}

async function syncSelection(selectAll: boolean) {
  const rowsToSelect = selectAll ? [...refundableRows.value] : []
  selectedRows.value = rowsToSelect
  await nextTick()
  const table = tableRef.value
  if (!table) return
  table.clearSelection()
  rowsToSelect.forEach((row) => table.toggleRowSelection(row, true))
  // 表格事件可能异步触发，业务状态仍以本次明确的全选/清空意图为准。
  selectedRows.value = rowsToSelect
}

async function loadRefundItems(selectAll = true) {
  const patientNo = String(props.patient?.patientNo ?? '').trim()
  const currentRequest = ++requestSequence
  tableRows.value = []
  selectedRows.value = []
  loadError.value = ''

  if (!patientNo) {
    loadError.value = '当前患者缺少病历号，无法查询退费清单'
    return
  }

  loading.value = true
  try {
    const items = await queryDoctorRefundItems({ num: patientNo })
    if (currentRequest !== requestSequence) return
    tableRows.value = createTableRows(items)
    await syncSelection(selectAll)
  } catch (error) {
    if (currentRequest !== requestSequence) return
    loadError.value = error instanceof Error ? error.message : '退费清单加载失败'
  } finally {
    if (currentRequest === requestSequence) loading.value = false
  }
}

function handleOpen() {
  void loadRefundItems(true)
}

function handleClosed() {
  requestSequence += 1
  tableRows.value = []
  selectedRows.value = []
  loadError.value = ''
  loading.value = false
}

function handleSelectionChange(rows: RefundTableRow[]) {
  // 即使未来表格配置被误改，提交前也不会接受不可退项目。
  selectedRows.value = rows.filter(isRefundable)
}

function setAllSelected(selected: boolean) {
  void syncSelection(selected)
}

function chargeStatusType(status?: string) {
  return ['已收费', '已支付'].includes(status?.trim() ?? '') ? 'success' : 'info'
}

function refundStatusType(status?: string) {
  const value = status?.trim() ?? ''
  if (value === '退费中') return 'warning'
  if (value === '已退费') return 'success'
  return 'info'
}

function closeDialog() {
  if (submitting.value) return
  emit('update:modelValue', false)
}

function handleDialogModelValue(value: boolean) {
  emit('update:modelValue', value)
}

function handleBeforeClose(done: () => void) {
  if (!submitting.value) done()
}

async function handleSubmit() {
  if (submitting.value) return
  const patientNo = String(props.patient?.patientNo ?? '').trim()
  if (!patientNo) {
    ElMessage.warning('当前患者缺少病历号')
    return
  }
  if (selectedRows.value.length === 0) {
    ElMessage.warning('请至少勾选一项可退费用')
    return
  }

  try {
    await ElMessageBox.confirm(
      `确认提交退费申请？共 ${selectedRows.value.length} 项，合计 ¥${formatMoneyFromCents(
        selectedAmountCents.value
      )}。`,
      '提交退费申请',
      {
        type: 'warning',
        confirmButtonText: '确认提交',
        cancelButtonText: '取消'
      }
    )
  } catch {
    return
  }

  submitting.value = true
  try {
    const applicationNo = await applyDoctorRefund({
      patientNo,
      apply: selectedRows.value.map(toApiItem),
      applyNo: selectedRows.value.length,
      applySum: selectedAmountCents.value / 100
    })
    ElMessage.success(`退费申请已提交，申请号 ${applicationNo}`)
    emit('submitted', applicationNo)
    emit('update:modelValue', false)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退费申请提交失败')
  } finally {
    submitting.value = false
  }
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="退费申请"
    width="min(920px, calc(100vw - 32px))"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
    :before-close="handleBeforeClose"
    @open="handleOpen"
    @closed="handleClosed"
    @update:model-value="handleDialogModelValue"
  >
    <div class="refund-application-dialog">
      <DetailSummary title="患者信息" :items="patientSummaryItems" :column="2" label-width="88px" />

      <section class="refund-application-dialog__list">
        <header class="refund-application-dialog__toolbar">
          <div>
            <strong>收费明细</strong>
            <span>仅“已收费且未申请”的项目可以勾选</span>
          </div>
          <div class="refund-application-dialog__toolbar-actions">
            <el-button
              link
              type="primary"
              :disabled="loading || submitting"
              @click="loadRefundItems(false)"
            >
              刷新
            </el-button>
            <el-button
              link
              type="primary"
              :disabled="loading || submitting"
              @click="setAllSelected(true)"
            >
              全选可退项
            </el-button>
            <el-button link :disabled="loading || submitting" @click="setAllSelected(false)">
              清空
            </el-button>
          </div>
        </header>

        <div v-if="loading" class="refund-application-dialog__loading" aria-busy="true">
          <el-skeleton :rows="6" animated />
        </div>

        <div v-else-if="loadError" class="refund-application-dialog__error">
          <el-alert :title="loadError" type="error" show-icon :closable="false" />
          <el-button type="primary" plain @click="loadRefundItems(true)">重新加载</el-button>
        </div>

        <el-table
          v-else
          ref="tableRef"
          :data="tableRows"
          :row-key="(row: RefundTableRow) => row._rowKey"
          border
          stripe
          height="360"
          empty-text="当前患者暂无收费明细"
          @selection-change="handleSelectionChange"
        >
          <el-table-column type="selection" width="48" align="center" :selectable="isRefundable" />
          <el-table-column
            prop="paymentNo"
            label="支付单号"
            min-width="155"
            show-overflow-tooltip
          />
          <el-table-column prop="itemName" label="收费项目" min-width="190" show-overflow-tooltip />
          <el-table-column prop="quantity" label="数量" width="96" align="right">
            <template #default="{ row }">{{ display(row.quantity) }}</template>
          </el-table-column>
          <el-table-column label="金额" width="108" align="right">
            <template #default="{ row }">¥{{ formatMoney(row.amount) }}</template>
          </el-table-column>
          <el-table-column label="收费状态" width="104" align="center">
            <template #default="{ row }">
              <el-tag size="small" effect="plain" :type="chargeStatusType(row.chargeStatus)">
                {{ display(row.chargeStatus) }}
              </el-tag>
            </template>
          </el-table-column>
          <el-table-column label="退费状态" width="104" align="center">
            <template #default="{ row }">
              <el-tag size="small" effect="plain" :type="refundStatusType(row.refundStatus)">
                {{ display(row.refundStatus) }}
              </el-tag>
            </template>
          </el-table-column>
        </el-table>

        <footer v-if="!loading && !loadError" class="refund-application-dialog__summary">
          <span>可退 {{ refundableRows.length }} 项</span>
          <span>
            已选 <strong>{{ selectedRows.length }}</strong> 项，合计
            <strong>¥{{ formatMoneyFromCents(selectedAmountCents) }}</strong>
          </span>
        </footer>
      </section>
    </div>

    <template #footer>
      <el-button :disabled="submitting" @click="closeDialog">取消</el-button>
      <el-button
        type="primary"
        :loading="submitting"
        :disabled="loading || Boolean(loadError) || selectedRows.length === 0"
        @click="handleSubmit"
      >
        提交申请
      </el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.refund-application-dialog {
  display: flex;
  min-height: 500px;
  flex-direction: column;
  gap: 16px;
  color: var(--el-text-color-primary);
}

.refund-application-dialog :deep(.detail-summary) {
  background: var(--el-fill-color-light);
}

.refund-application-dialog__list {
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.refund-application-dialog__toolbar,
.refund-application-dialog__summary {
  display: flex;
  min-height: 48px;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 0 12px;
}

.refund-application-dialog__toolbar {
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.refund-application-dialog__toolbar > div:first-child {
  display: flex;
  min-width: 0;
  align-items: baseline;
  gap: 12px;
}

.refund-application-dialog__toolbar span {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.refund-application-dialog__toolbar-actions {
  display: flex;
  flex-shrink: 0;
}

.refund-application-dialog__loading,
.refund-application-dialog__error {
  min-height: 360px;
  padding: 20px;
}

.refund-application-dialog__error {
  display: flex;
  align-items: center;
  flex-direction: column;
  justify-content: center;
  gap: 16px;
}

.refund-application-dialog__summary {
  border-top: 1px solid var(--el-border-color-lighter);
  color: var(--el-text-color-regular);
  font-size: 13px;
}

.refund-application-dialog__summary strong {
  color: var(--el-color-primary);
}

.refund-application-dialog :deep(.el-table) {
  color: var(--el-text-color-primary);
}

@media (max-width: 720px) {
  .refund-application-dialog__toolbar {
    align-items: flex-start;
    flex-direction: column;
    padding: 10px 12px;
  }

  .refund-application-dialog__toolbar > div:first-child {
    align-items: flex-start;
    flex-direction: column;
    gap: 4px;
  }
}
</style>
