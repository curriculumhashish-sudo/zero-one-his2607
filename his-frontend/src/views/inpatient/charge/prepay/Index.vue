<script setup lang="ts">
import type {
  InpatientPatient,
  PrepayChannel,
  PrepayRecord,
  PrepaySummary
} from '@/apis/inpatient/charge/prepay'
import type { PatientSummary } from '@/components/patient/type'
import type { RowActionItem } from '@/components/common/type'
import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Plus, Refresh } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import {
  cancelPrepayPayment,
  collectInpatientPrepay,
  getInpatientPrepaySummary,
  pageInpatientQueue,
  pagePrepayRecords,
  queryPrepayPaymentResult,
  recoverPrepaySettlement,
  refundInpatientPrepay
} from '@/apis/inpatient/charge/prepay'
import { toPrepayPatientSummary } from './adapter'
import CollectPrepayDialog from './components/CollectPrepayDialog.vue'
import RefundPrepayDialog from './components/RefundPrepayDialog.vue'
import type { PrepayPageQuery } from './type'

defineOptions({ name: 'InpatientPrepay' })
const query = reactive<PrepayPageQuery>({ keyword: '', page: 1, pageSize: 10 })
const patients = ref<InpatientPatient[]>([])
const queueTotal = ref(0)
const queueLoading = ref(false)
const detailLoading = ref(false)
const submitting = ref(false)
const polling = ref(false)
const errorMessage = ref('')
const selectedEncounterId = ref<string | null>(null)
const summary = ref<PrepaySummary | null>(null)
const records = ref<PrepayRecord[]>([])
const recordPage = ref(1)
const recordPageSize = ref(10)
const recordTotal = ref(0)
const pendingPaymentId = ref('')
const refundRecord = ref<PrepayRecord | null>(null)
const collectDialog = ref<InstanceType<typeof CollectPrepayDialog>>()
const refundDialog = ref<InstanceType<typeof RefundPrepayDialog>>()
let queueSequence = 0
let detailSequence = 0
let paymentPollTimer: ReturnType<typeof setInterval> | undefined
const queueRows = computed(() => patients.value.map(toPrepayPatientSummary))
const currentPatient = computed(() =>
  summary.value ? toPrepayPatientSummary(summary.value.patient) : null
)
const summaryItems = computed(() =>
  summary.value
    ? [
        { key: 'balance', label: '当前余额', value: `¥${summary.value.balance.toFixed(2)}` },
        {
          key: 'collected',
          label: '累计收款',
          value: `¥${summary.value.totalCollected.toFixed(2)}`
        },
        { key: 'refunded', label: '累计退款', value: `¥${summary.value.totalRefunded.toFixed(2)}` },
        {
          key: 'refundable',
          label: '可退余额',
          value: `¥${summary.value.refundableBalance.toFixed(2)}`
        },
        {
          key: 'ward',
          label: '病区床位',
          value: `${summary.value.patient.wardName} · ${summary.value.patient.bedNo}`
        }
      ]
    : []
)
const statusOptions = [
  { value: 'processing', label: '处理中', type: 'warning' as const },
  { value: 'settled', label: '已结算', type: 'success' as const },
  { value: 'refunded', label: '已退款', type: 'info' as const },
  { value: 'cancelled', label: '已取消', type: 'info' as const }
]
const actions = computed(() => [
  {
    key: 'collect',
    label: '收取预交金',
    type: 'primary' as const,
    icon: Plus,
    disabled: !summary.value || submitting.value || Boolean(pendingPaymentId.value)
  },
  { key: 'refresh', label: '刷新', icon: Refresh }
])
async function loadQueue() {
  const seq = ++queueSequence
  queueLoading.value = true
  errorMessage.value = ''
  try {
    const result = await pageInpatientQueue(query)
    if (seq !== queueSequence) return
    patients.value = result.rows
    queueTotal.value = result.total
    if (
      selectedEncounterId.value &&
      !result.rows.some((item) => item.encounterId === selectedEncounterId.value)
    )
      clearSelection()
  } catch (error) {
    if (seq === queueSequence) {
      patients.value = []
      errorMessage.value = error instanceof Error ? error.message : '住院患者加载失败'
    }
  } finally {
    if (seq === queueSequence) queueLoading.value = false
  }
}
function clearSelection() {
  stopPaymentPolling()
  detailSequence += 1
  selectedEncounterId.value = null
  summary.value = null
  records.value = []
  pendingPaymentId.value = ''
  refundRecord.value = null
  collectDialog.value?.close()
  refundDialog.value?.close()
}
async function selectPatient(patient: PatientSummary) {
  clearSelection()
  const id = String(patient.id)
  selectedEncounterId.value = id
  const seq = ++detailSequence
  detailLoading.value = true
  try {
    const [summaryResult, recordResult] = await Promise.all([
      getInpatientPrepaySummary(id),
      pagePrepayRecords(id, { page: recordPage.value, pageSize: recordPageSize.value })
    ])
    if (seq !== detailSequence) return
    summary.value = summaryResult
    records.value = recordResult.rows
    recordTotal.value = recordResult.total
    pendingPaymentId.value =
      recordResult.rows.find((item) => item.status === 'processing')?.id ?? ''
    if (pendingPaymentId.value) startPaymentPolling(pendingPaymentId.value)
  } catch (error) {
    if (seq === detailSequence) {
      clearSelection()
      ElMessage.error(error instanceof Error ? error.message : '预交金数据加载失败')
    }
  } finally {
    if (seq === detailSequence) detailLoading.value = false
  }
}
async function refreshCurrent() {
  const id = selectedEncounterId.value
  await loadQueue()
  if (id) await selectPatient({ id } as PatientSummary)
}
function handleSearch(keyword: string) {
  query.keyword = keyword
  query.page = 1
  clearSelection()
  void loadQueue()
}
function handleQueuePageChange(page: number) {
  query.page = page
  clearSelection()
  void loadQueue()
}
async function collect(data: { amount: number; channel: PrepayChannel }) {
  if (!selectedEncounterId.value || submitting.value) return
  try {
    await ElMessageBox.confirm(`确认收取预交金 ¥${data.amount.toFixed(2)}？`, '收款确认', {
      type: 'warning'
    })
  } catch {
    return
  }
  submitting.value = true
  try {
    const record = await collectInpatientPrepay({ encounterId: selectedEncounterId.value, ...data })
    collectDialog.value?.close()
    ElMessage.success(
      record.status === 'processing' ? '扫码收款已发起，请确认支付结果' : '预交金收取成功'
    )
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '收款失败')
  } finally {
    submitting.value = false
  }
}
function openRefund(record: PrepayRecord) {
  refundRecord.value = record
  refundDialog.value?.open(record)
}
async function refund(data: { amount: number; channel: PrepayChannel }) {
  if (!selectedEncounterId.value || !refundRecord.value || submitting.value) return
  try {
    await ElMessageBox.confirm(`确认按原渠道退款 ¥${data.amount.toFixed(2)}？`, '退款确认', {
      type: 'warning'
    })
  } catch {
    return
  }
  submitting.value = true
  try {
    const record = await refundInpatientPrepay({
      encounterId: selectedEncounterId.value,
      recordId: refundRecord.value.id,
      ...data
    })
    refundDialog.value?.close()
    ElMessage.success(
      record.status === 'processing' ? '扫码退款已发起，系统将自动查询结果' : '预交金退款成功'
    )
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退款失败')
  } finally {
    submitting.value = false
  }
}
async function confirmPending(recover = false) {
  if (!pendingPaymentId.value || submitting.value) return
  submitting.value = true
  stopPaymentPolling()
  try {
    await (recover
      ? recoverPrepaySettlement(pendingPaymentId.value)
      : queryPrepayPaymentResult(pendingPaymentId.value))
    ElMessage.success('支付结果已确认')
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '支付结果确认失败')
  } finally {
    submitting.value = false
  }
}
function stopPaymentPolling() {
  if (paymentPollTimer) clearInterval(paymentPollTimer)
  paymentPollTimer = undefined
  polling.value = false
}
async function pollPending(paymentId: string) {
  if (pendingPaymentId.value !== paymentId || submitting.value || polling.value) return
  polling.value = true
  try {
    const record = await queryPrepayPaymentResult(paymentId)
    if (record.status !== 'processing') {
      stopPaymentPolling()
      ElMessage.success('扫码交易结果已自动确认')
      await refreshCurrent()
    }
  } catch (error) {
    stopPaymentPolling()
    ElMessage.error(error instanceof Error ? error.message : '自动查询支付结果失败')
  } finally {
    polling.value = false
  }
}
function startPaymentPolling(paymentId: string) {
  stopPaymentPolling()
  paymentPollTimer = setInterval(() => void pollPending(paymentId), 1500)
}
async function cancelPending() {
  if (!pendingPaymentId.value || submitting.value) return
  try {
    await ElMessageBox.confirm('确认取消当前未完成的扫码交易？', '取消交易', { type: 'warning' })
  } catch {
    return
  }
  submitting.value = true
  stopPaymentPolling()
  try {
    await cancelPrepayPayment(pendingPaymentId.value)
    ElMessage.success('扫码交易已取消，未发生余额变更')
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '取消交易失败')
  } finally {
    submitting.value = false
  }
}
function recordActions(row: PrepayRecord): RowActionItem[] {
  return [
    {
      key: 'refund',
      label: '退款',
      type: 'danger',
      disabled: row.type !== 'collect' || row.status !== 'settled' || row.refundableAmount <= 0
    }
  ]
}
function handleRecordAction(_: RowActionItem, source: unknown) {
  openRefund(source as unknown as PrepayRecord)
}
function handleToolbar(action: { key: string | number }) {
  if (action.key === 'collect') collectDialog.value?.open()
  else void refreshCurrent()
}
onMounted(() => void loadQueue())
onBeforeUnmount(() => {
  stopPaymentPolling()
  queueSequence += 1
  detailSequence += 1
})
</script>
<template>
  <main class="prepay-page">
    <aside class="queue-pane">
      <div class="queue-title">在院患者</div>
      <PatientQueuePanel
        :patients="queueRows"
        :selected-id="selectedEncounterId"
        :loading="queueLoading"
        @search="handleSearch"
        @refresh="loadQueue"
        @select="selectPatient"
      />
      <el-pagination
        v-if="queueTotal > query.pageSize"
        v-model:current-page="query.page"
        :page-size="query.pageSize"
        :total="queueTotal"
        layout="prev,pager,next"
        @change="handleQueuePageChange"
      />
    </aside>
    <section class="prepay-workspace">
      <el-alert v-if="errorMessage" type="error" :title="errorMessage" :closable="false"
        ><el-button type="primary" link @click="loadQueue">重试</el-button></el-alert
      ><EmptyState
        v-if="!selectedEncounterId"
        title="请选择住院患者"
        description="选择患者后查看余额并办理收退预交金"
      /><template v-else
        ><PatientInfoBar :patient="currentPatient" /><el-alert
          v-if="pendingPaymentId"
          type="warning"
          :closable="false"
          title="存在处理中的扫码收款或退款，系统正在自动查询结果。"
          ><el-button
            type="primary"
            link
            :loading="submitting || polling"
            @click="confirmPending(false)"
            >查询支付结果</el-button
          ><el-button link :disabled="submitting || polling" @click="confirmPending(true)"
            >恢复交易</el-button
          ><el-button link type="danger" :disabled="submitting || polling" @click="cancelPending"
            >取消交易</el-button
          ></el-alert
        ><DetailSummary
          title="预交金账户"
          :items="summaryItems"
          :loading="detailLoading" /><ActionToolbar
          title="预交金流水"
          :actions="actions"
          @action="handleToolbar" /><DataTablePanel
          :data="records"
          :loading="detailLoading"
          :total="recordTotal"
          :page="recordPage"
          :page-size="recordPageSize"
          min-height="330px"
          @update:page="
            (value: number) => {
              recordPage = value
              refreshCurrent()
            }
          "
          @update:page-size="
            (value: number) => {
              recordPageSize = value
              recordPage = 1
              refreshCurrent()
            }
          "
          ><el-table-column prop="transactionNo" label="交易号" min-width="180" /><el-table-column
            label="业务类型"
            width="100"
            ><template #default="{ row }"
              ><el-tag :type="row.type === 'collect' ? 'primary' : 'danger'">{{
                row.type === 'collect' ? '收款' : '退款'
              }}</el-tag></template
            ></el-table-column
          ><el-table-column label="金额" width="110"
            ><template #default="{ row }"
              ><strong :class="row.type"
                >{{ row.type === 'collect' ? '+' : '-' }}¥{{ row.amount.toFixed(2) }}</strong
              ></template
            ></el-table-column
          ><el-table-column prop="channel" label="渠道" width="100" /><el-table-column
            prop="createdAt"
            label="交易时间"
            width="170" /><el-table-column label="状态" width="100"
            ><template #default="{ row }"
              ><StatusDisplay
                :value="row.status"
                :options="statusOptions" /></template></el-table-column
          ><el-table-column label="可退金额" width="110"
            ><template #default="{ row }"
              >¥{{ row.refundableAmount.toFixed(2) }}</template
            ></el-table-column
          ><el-table-column label="操作" width="100"
            ><template #default="{ row }"
              ><RowActionGroup
                :row="row"
                :actions="recordActions(row as unknown as PrepayRecord)"
                @action="handleRecordAction" /></template></el-table-column></DataTablePanel
      ></template>
    </section>
    <CollectPrepayDialog
      ref="collectDialog"
      :submitting="submitting"
      @confirm="collect"
    /><RefundPrepayDialog
      ref="refundDialog"
      :record="refundRecord"
      :balance="summary?.refundableBalance ?? 0"
      :submitting="submitting"
      @confirm="refund"
    />
  </main>
</template>
<style scoped>
.prepay-page {
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
.queue-title {
  padding: 14px 16px 0;
  font-weight: 600;
}
.queue-pane :deep(.patient-queue-panel) {
  min-height: 0;
}
.prepay-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  gap: 12px;
  overflow: auto;
}
.collect {
  color: var(--el-color-success);
}
.refund {
  color: var(--el-color-danger);
}
@media (max-width: 900px) {
  .prepay-page {
    height: auto;
    grid-template-columns: 1fr;
  }
  .queue-pane {
    height: 520px;
  }
}
</style>
