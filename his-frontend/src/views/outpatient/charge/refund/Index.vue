<script setup lang="ts">
import type { RefundDetail, RefundItem } from '@/apis/outpatient/charge/refund'
import type { PatientSummary } from '@/components/patient/type'
import type { DateRangeValue } from '@/components/common/type'
import { computed, nextTick, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Printer, Refresh, RefreshLeft } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import {
  getRefundableExecutionDetail,
  pageRefundPatientQueue,
  queryRefundResult,
  recoverRefund,
  refundOutpatientCharge,
  switchRetainedItemsInsuranceMode,
  verifyOutpatientRefund
} from '@/apis/outpatient/charge/refund'
import { toRefundPatientSummary } from './adapter'
import RefundSettlementDialog from './components/RefundSettlementDialog.vue'
import type { RefundPageQuery, RefundVerificationView } from './type'

defineOptions({ name: 'OutpatientChargeRefund' })
interface RefundReceipt {
  patientName: string
  encounterBusNo: string
  originalPaymentNo: string
  reversePaymentId: string
  refundTime: string
  totalAmount: number
  items: Array<{ id: string; name: string; quantity: number; amount: number }>
}

const query = reactive<RefundPageQuery>({
  keyword: '',
  startDate: null,
  endDate: null,
  status: 'pending',
  page: 1,
  pageSize: 10
})
const patients = ref<any[]>([])
const total = ref(0)
const queueLoading = ref(false)
const detailLoading = ref(false)
const submitting = ref(false)
const errorMessage = ref('')
const selectedEncounterId = ref<string | null>(null)
const detail = ref<RefundDetail | null>(null)
const selectedItems = ref<RefundItem[]>([])
const verification = ref<RefundVerificationView | null>(null)
const pendingRefundId = ref('')
const lastReceipt = ref<RefundReceipt | null>(null)
const pendingReceipt = ref<RefundReceipt | null>(null)
const dialog = ref<InstanceType<typeof RefundSettlementDialog>>()
let queueSequence = 0
let detailSequence = 0
const queueRows = computed(() => patients.value.map(toRefundPatientSummary))
const currentPatient = computed(() =>
  detail.value ? toRefundPatientSummary(detail.value.patient) : null
)
const selectedAmount = computed(() =>
  selectedItems.value.reduce((sum, item) => sum + item.refundableAmount, 0)
)
const summaryItems = computed(() =>
  detail.value
    ? [
        {
          key: 'groups',
          label: '原支付记录',
          value: `${new Set(detail.value.items.map((item) => item.paymentRecordId)).size} 笔`
        },
        {
          key: 'refundable',
          label: '可退项目',
          value: `${detail.value.items.filter((item) => item.status === 'refundable').length} 项`
        },
        { key: 'selected', label: '已选金额', value: `¥${selectedAmount.value.toFixed(2)}` }
      ]
    : []
)
const statusOptions = [
  { value: 'refundable', label: '可退', type: 'success' as const },
  { value: 'partial', label: '部分可退', type: 'warning' as const },
  { value: 'refunded', label: '已退', type: 'info' as const },
  { value: 'unavailable', label: '不可退', type: 'danger' as const }
]
const actions = computed(() => [
  {
    key: 'refund',
    label: '校验并退费',
    type: 'danger' as const,
    icon: RefreshLeft,
    disabled: !selectedItems.value.length || submitting.value || Boolean(pendingRefundId.value)
  },
  {
    key: 'print',
    label: '打印退费小票',
    icon: Printer,
    disabled: !lastReceipt.value || submitting.value
  },
  { key: 'refresh', label: '刷新', icon: Refresh }
])
async function loadQueue() {
  const seq = ++queueSequence
  queueLoading.value = true
  errorMessage.value = ''
  try {
    const result = await pageRefundPatientQueue({
      keyword: query.keyword,
      startDate: query.startDate,
      endDate: query.endDate,
      status: query.status,
      PageIndex: query.page,
      PageSize: query.pageSize
    })
    if (seq !== queueSequence) return
    patients.value = result.rows
    total.value = result.total
    if (
      selectedEncounterId.value &&
      !result.rows.some((item) => item.encounterId === selectedEncounterId.value)
    )
      clearSelection()
  } catch (error) {
    if (seq === queueSequence) {
      patients.value = []
      total.value = 0
      errorMessage.value = error instanceof Error ? error.message : '退费患者队列加载失败'
    }
  } finally {
    if (seq === queueSequence) queueLoading.value = false
  }
}
function clearSelection() {
  detailSequence += 1
  selectedEncounterId.value = null
  detail.value = null
  selectedItems.value = []
  verification.value = null
  pendingRefundId.value = ''
  dialog.value?.close()
}
async function selectPatient(patient: PatientSummary) {
  const id = String(patient.id)
  if (selectedEncounterId.value && selectedEncounterId.value !== id) {
    lastReceipt.value = null
    pendingReceipt.value = null
  }
  clearSelection()
  selectedEncounterId.value = id
  const seq = ++detailSequence
  detailLoading.value = true
  try {
    const result = await getRefundableExecutionDetail(id)
    if (seq === detailSequence) {
      detail.value = result
      pendingRefundId.value = result.interruptedRefundId ?? ''
    }
  } catch (error) {
    if (seq === detailSequence) {
      clearSelection()
      ElMessage.error(error instanceof Error ? error.message : '可退明细加载失败')
    }
  } finally {
    if (seq === detailSequence) detailLoading.value = false
  }
}
function handleSelection(rows: RefundItem[]) {
  const valid = rows.filter((item) => item.status === 'refundable')
  if (new Set(valid.map((item) => item.paymentRecordId)).size > 1) {
    selectedItems.value = []
    ElMessage.warning('一次退费只能选择同一笔原支付记录，请重新勾选')
    return
  }
  selectedItems.value = valid
  if (valid.length !== rows.length) ElMessage.warning('已自动忽略不可退项目')
}
async function verifyRefund() {
  if (!detail.value || !selectedItems.value.length || submitting.value) return
  if (pendingRefundId.value) {
    ElMessage.warning('请先确认处理中的退款结果，避免重复退款')
    return
  }
  submitting.value = true
  try {
    const check = await verifyOutpatientRefund({
      encounterId: detail.value.patient.encounterId,
      refundingChargeItemIds: selectedItems.value.map((item) => item.chargeItemId ?? item.id)
    })
    if (!check.allowed) throw new Error(check.rejectReasonCode ?? '当前项目不可退')
    const paymentRecordId = selectedItems.value[0]!.paymentRecordId
    const groupItems = detail.value.items.filter((item) => item.paymentRecordId === paymentRecordId)
    const selectedIds = selectedItems.value.map((item) => item.chargeItemId ?? item.id)
    const retainedItems = groupItems.filter(
      (item) => !selectedIds.includes(item.chargeItemId ?? item.id)
    )
    const refundAmount = selectedItems.value.reduce((sum, item) => sum + item.refundableAmount, 0)
    verification.value = {
      allowed: true,
      originalPaymentId: paymentRecordId,
      paymentRecordId,
      refundingChargeItemIds: selectedIds,
      preservedNewChargeItemIds: retainedItems.map((item) => item.chargeItemId ?? item.id),
      halfRefundOnPayment: retainedItems.length > 0,
      originalPayChannelCodes: [...new Set(selectedItems.value.map((item) => item.channel))],
      refundAmount,
      retainedAmount: retainedItems.reduce((sum, item) => sum + item.refundableAmount, 0),
      channel: selectedItems.value[0]!.channel,
      message: `原支付单 ${selectedItems.value[0]!.paymentNo}，本次可退 ¥${refundAmount.toFixed(2)}`
    }
    dialog.value?.open()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退费校验失败')
  } finally {
    submitting.value = false
  }
}
async function confirmRefund(data: { retainedMode: 'insurance' | 'self-pay'; scanCode?: string }) {
  if (!detail.value || !verification.value || submitting.value) return
  try {
    await ElMessageBox.confirm(
      `确认按原支付渠道退回 ¥${verification.value.refundAmount.toFixed(2)}？`,
      '退费二次确认',
      { type: 'warning' }
    )
  } catch {
    return
  }
  submitting.value = true
  try {
    const receipt: RefundReceipt = {
      patientName: detail.value.patient.patientName ?? detail.value.patient.name,
      encounterBusNo: detail.value.patient.encounterBusNo ?? detail.value.patient.visitNo,
      originalPaymentNo: selectedItems.value[0]?.paymentNo ?? verification.value.originalPaymentId,
      reversePaymentId: '',
      refundTime: new Date().toLocaleString('zh-CN', { hour12: false }),
      totalAmount: verification.value.refundAmount,
      items: selectedItems.value.map((item) => ({
        id: item.chargeItemId ?? item.id,
        name: item.itemDisplayName ?? item.itemName,
        quantity: item.quantity,
        amount: item.refundableAmount
      }))
    }
    const retainedIds = verification.value.preservedNewChargeItemIds
    if (retainedIds.length && data.retainedMode === 'self-pay')
      await switchRetainedItemsInsuranceMode({ encounterId: detail.value.patient.encounterId })
    const record = await refundOutpatientCharge({
      encounterId: detail.value.patient.encounterId,
      originalPaymentId: verification.value.originalPaymentId,
      refundingChargeItemIds: verification.value.refundingChargeItemIds,
      preservedNewChargeItemIds: verification.value.preservedNewChargeItemIds,
      refundPaymentDetails: [
        {
          amount: verification.value.refundAmount,
          payChannelCode: verification.value.channel
        }
      ],
      clientRefundResult: data.scanCode
        ? {
            merTradeNo: verification.value.originalPaymentId,
            tradeNo: data.scanCode,
            respCode: 'PROCESSING',
            respMsg: '等待渠道确认',
            refundAmountYuan: verification.value.refundAmount
          }
        : undefined
    })
    dialog.value?.close()
    const pendingId = data.scanCode ? (record.reversePaymentId ?? '') : ''
    receipt.reversePaymentId = record.reversePaymentId
    if (data.scanCode) pendingReceipt.value = receipt
    else lastReceipt.value = receipt
    ElMessage.success(data.scanCode ? '扫码退款已发起，请确认退款结果' : '退费成功')
    await refreshCurrent()
    pendingRefundId.value = pendingId
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退费失败')
  } finally {
    submitting.value = false
  }
}
async function refreshCurrent() {
  const id = selectedEncounterId.value
  await loadQueue()
  if (id) await selectPatient({ id } as PatientSummary)
}
async function confirmPending(recover = false) {
  if (!pendingRefundId.value) return
  submitting.value = true
  try {
    await (recover
      ? recoverRefund(pendingRefundId.value)
      : queryRefundResult(pendingRefundId.value))
    if (pendingReceipt.value) lastReceipt.value = pendingReceipt.value
    pendingReceipt.value = null
    pendingRefundId.value = ''
    ElMessage.success('退款结果已确认')
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退款结果确认失败')
  } finally {
    submitting.value = false
  }
}
function changeStatus(value: any) {
  query.status = value
  query.page = 1
  lastReceipt.value = null
  pendingReceipt.value = null
  clearSelection()
  void loadQueue()
}
function handleSearch(keyword: string) {
  query.keyword = keyword
  query.page = 1
  lastReceipt.value = null
  pendingReceipt.value = null
  clearSelection()
  void loadQueue()
}
function handleDateFilterChange(value: DateRangeValue) {
  query.startDate = value?.[0] ?? null
  query.endDate = value?.[1] ?? null
  query.page = 1
  lastReceipt.value = null
  pendingReceipt.value = null
  clearSelection()
  void loadQueue()
}
function handleQueuePaginationChange(payload: { page: number; pageSize: number }) {
  const pageSizeChanged = payload.pageSize !== query.pageSize
  query.page = pageSizeChanged ? 1 : payload.page
  query.pageSize = payload.pageSize
  lastReceipt.value = null
  pendingReceipt.value = null
  clearSelection()
  void loadQueue()
}
function handleToolbar(action: { key: string | number | boolean }) {
  if (action.key === 'refund') void verifyRefund()
  else if (action.key === 'print') void handlePrint()
  else void refreshCurrent()
}
async function handlePrint() {
  if (!lastReceipt.value) {
    ElMessage.warning('暂无可打印的退费记录')
    return
  }
  await nextTick()
  window.print()
}
function spanMethod({ row, columnIndex }: { row: RefundItem; columnIndex: number }) {
  if (columnIndex !== 1 || !detail.value) return
  const group = detail.value.items.filter((item) => item.paymentRecordId === row.paymentRecordId)
  const index = group.findIndex((item) => item.id === row.id)
  return index === 0 ? { rowspan: group.length, colspan: 1 } : { rowspan: 0, colspan: 0 }
}
onMounted(() => void loadQueue())
onBeforeUnmount(() => {
  queueSequence += 1
  detailSequence += 1
})
</script>
<template>
  <main class="refund-page">
    <aside class="queue-pane">
      <div class="queue-status">
        <el-segmented
          :model-value="query.status"
          :options="[
            { label: '待退费', value: 'pending' },
            { label: '已退费', value: 'refunded' },
            { label: '全部', value: 'all' }
          ]"
          @change="changeStatus"
        />
      </div>
      <PatientQueuePanel
        :patients="queueRows"
        :selected-id="selectedEncounterId"
        :loading="queueLoading"
        @search="handleSearch"
        @date-change="handleDateFilterChange"
        @refresh="loadQueue"
        @select="selectPatient"
      />
      <PaginationBar
        v-if="total > 0"
        class="queue-pagination"
        :page="query.page"
        :page-size="query.pageSize"
        :total="total"
        :disabled="queueLoading"
        small
        @change="handleQueuePaginationChange"
      />
    </aside>
    <section class="refund-workspace">
      <el-alert v-if="errorMessage" type="error" :title="errorMessage" :closable="false"
        ><el-button type="primary" link @click="loadQueue">重试</el-button></el-alert
      ><EmptyState
        v-if="!selectedEncounterId"
        title="请选择患者"
        description="选择患者后按原支付记录办理退费"
      /><template v-else
        ><PatientInfoBar :patient="currentPatient" /><el-alert
          v-if="pendingRefundId"
          type="warning"
          :closable="false"
          title="存在未完成扫码退款，请先确认结果。"
          ><el-button type="primary" link :loading="submitting" @click="confirmPending(false)"
            >查询退款结果</el-button
          ><el-button link @click="confirmPending(true)">恢复退款</el-button></el-alert
        ><el-alert
          v-if="detail?.refundFailureReason"
          type="error"
          :closable="false"
          show-icon
          :title="detail.refundFailureReason"
          description="退款失败原因已保留，请核对原支付渠道后重新发起或恢复退款。" />
        <DetailSummary
          title="退费摘要"
          :items="summaryItems"
          :loading="detailLoading" /><ActionToolbar
          title="可退执行明细"
          description="一次只能选择同一笔原支付记录"
          :actions="actions"
          :selection-count="selectedItems.length"
          @action="handleToolbar" /><DataTablePanel
          :data="detail?.items ?? []"
          :loading="detailLoading"
          :total="detail?.items.length ?? 0"
          selectable
          min-height="330px"
          :span-method="spanMethod"
          @selectable="handleSelection"
          ><el-table-column prop="paymentNo" label="原支付单号" min-width="170" /><el-table-column
            prop="paidAt"
            label="支付时间"
            width="160" /><el-table-column
            prop="itemName"
            label="项目名称"
            min-width="170" /><el-table-column
            prop="category"
            label="类别"
            width="90" /><el-table-column prop="amount" label="原金额" width="100"
            ><template #default="{ row }">¥{{ row.amount.toFixed(2) }}</template></el-table-column
          ><el-table-column prop="refundableAmount" label="可退金额" width="110"
            ><template #default="{ row }"
              >¥{{ row.refundableAmount.toFixed(2) }}</template
            ></el-table-column
          ><el-table-column label="费用属性" width="100"
            ><template #default="{ row }">{{
              row.insuranceMode === 'insurance' ? '医保' : '自费'
            }}</template></el-table-column
          ><el-table-column label="状态" width="100"
            ><template #default="{ row }"
              ><StatusDisplay
                :value="row.status"
                :options="statusOptions" /></template></el-table-column></DataTablePanel
      ></template>
    </section>
    <RefundSettlementDialog
      ref="dialog"
      :data="verification"
      :submitting="submitting"
      @confirm="confirmRefund"
    />
    <section v-if="lastReceipt" class="refund-print-sheet" aria-hidden="true">
      <h1>门诊退费小票</h1>
      <dl>
        <dt>患者</dt>
        <dd>{{ lastReceipt.patientName }}</dd>
        <dt>门诊号</dt>
        <dd>{{ lastReceipt.encounterBusNo }}</dd>
        <dt>原支付单</dt>
        <dd>{{ lastReceipt.originalPaymentNo }}</dd>
        <dt>退费流水</dt>
        <dd>{{ lastReceipt.reversePaymentId }}</dd>
        <dt>退费时间</dt>
        <dd>{{ lastReceipt.refundTime }}</dd>
      </dl>
      <table>
        <thead>
          <tr>
            <th>项目</th>
            <th>数量</th>
            <th>退费金额</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="item in lastReceipt.items" :key="item.id">
            <td>{{ item.name }}</td>
            <td>{{ item.quantity }}</td>
            <td>¥{{ item.amount.toFixed(2) }}</td>
          </tr>
        </tbody>
        <tfoot>
          <tr>
            <th colspan="2">退费合计</th>
            <th>¥{{ lastReceipt.totalAmount.toFixed(2) }}</th>
          </tr>
        </tfoot>
      </table>
    </section>
  </main>
</template>
<style scoped>
.refund-page {
  --el-color-primary: #1d4ed8;
  --el-color-success: #15803d;
  --el-color-warning: #a16207;
  --el-color-danger: #b91c1c;
  --el-color-info: #4b5563;
  --el-text-color-primary: #1f2937;
  --el-text-color-regular: #374151;
  --el-text-color-secondary: #4b5563;
  --el-text-color-placeholder: #6b7280;
  display: grid;
  height: calc(100vh - 96px);
  min-height: 620px;
  grid-template-columns: 320px minmax(0, 1fr);
  gap: 14px;
  padding: 16px;
}
.queue-pane {
  display: flex;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}
.queue-pagination {
  justify-content: center;
  padding: 8px;
}
.queue-status {
  padding: 12px 12px 0;
}
.queue-status :deep(.el-segmented) {
  width: 100%;
}
.queue-pane :deep(.patient-queue-panel) {
  min-height: 0;
}
.refund-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  gap: 12px;
  overflow: auto;
}
.refund-print-sheet {
  display: none;
}
@media print {
  @page {
    size: 80mm auto;
    margin: 5mm;
  }
  .refund-page {
    display: block;
    height: auto;
  }
  .refund-page > :not(.refund-print-sheet) {
    display: none !important;
  }
  .refund-print-sheet {
    display: block;
    color: #000;
    font-size: 11px;
  }
  .refund-print-sheet h1 {
    margin: 0 0 10px;
    text-align: center;
    font-size: 18px;
  }
  .refund-print-sheet dl {
    display: grid;
    grid-template-columns: 64px 1fr;
    margin: 0 0 10px;
  }
  .refund-print-sheet dt,
  .refund-print-sheet dd {
    margin: 0;
    padding: 2px 0;
    overflow-wrap: anywhere;
  }
  .refund-print-sheet table {
    width: 100%;
    border-collapse: collapse;
  }
  .refund-print-sheet th,
  .refund-print-sheet td {
    padding: 4px 2px;
    border-bottom: 1px solid #777;
    text-align: left;
  }
  .refund-print-sheet th:last-child,
  .refund-print-sheet td:last-child {
    text-align: right;
  }
}
@media (max-width: 900px) {
  .refund-page {
    height: auto;
    grid-template-columns: 1fr;
  }
  .queue-pane {
    height: 520px;
    overflow-x: auto;
  }
  .queue-pane :deep(.patient-queue-panel) {
    min-width: 360px;
  }
}
</style>
