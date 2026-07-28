<script setup lang="ts">
import type { RefundDetail, RefundItem, RefundVerification } from '@/apis/outpatient/charge/refund'
import type { PatientSummary } from '@/components/patient/type'
import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Refresh, RefreshLeft } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
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
import type { RefundPageQuery } from './type'

defineOptions({ name: 'OutpatientChargeRefund' })
const query = reactive<RefundPageQuery>({
  keyword: '',
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
const verification = ref<RefundVerification | null>(null)
const pendingRefundId = ref('')
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
  { key: 'refresh', label: '刷新', icon: Refresh }
])
async function loadQueue() {
  const seq = ++queueSequence
  queueLoading.value = true
  errorMessage.value = ''
  try {
    const result = await pageRefundPatientQueue(query)
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
  clearSelection()
  const id = String(patient.id)
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
    verification.value = await verifyOutpatientRefund({
      encounterId: detail.value.patient.encounterId,
      itemIds: selectedItems.value.map((item) => item.id)
    })
    dialog.value?.open()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '退费校验失败')
  } finally {
    submitting.value = false
  }
}
async function confirmRefund(retainedMode: 'insurance' | 'self-pay') {
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
    const allGroupItems = detail.value.items.filter(
      (item) => item.paymentRecordId === verification.value!.paymentRecordId
    )
    const retainedIds = allGroupItems
      .filter((item) => !verification.value!.itemIds.includes(item.id))
      .map((item) => item.id)
    if (retainedIds.length)
      detail.value = await switchRetainedItemsInsuranceMode({
        encounterId: detail.value.patient.encounterId,
        itemIds: retainedIds,
        mode: retainedMode
      })
    const record = await refundOutpatientCharge({
      verificationId: verification.value.verificationId,
      encounterId: detail.value.patient.encounterId,
      paymentRecordId: verification.value.paymentRecordId,
      itemIds: verification.value.itemIds,
      channel: verification.value.channel,
      retainedMode
    })
    dialog.value?.close()
    const pendingId = record.status === 'pending' ? record.id : ''
    ElMessage.success(record.status === 'pending' ? '扫码退款已发起，请确认退款结果' : '退费成功')
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
  clearSelection()
  void loadQueue()
}
function handleSearch(keyword: string) {
  query.keyword = keyword
  query.page = 1
  clearSelection()
  void loadQueue()
}
function handlePageChange(page: number) {
  query.page = page
  clearSelection()
  void loadQueue()
}
function handleToolbar(action: { key: string | number }) {
  if (action.key === 'refund') void verifyRefund()
  else void refreshCurrent()
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
        @refresh="loadQueue"
        @select="selectPatient"
      />
      <el-pagination
        v-if="total > query.pageSize"
        v-model:current-page="query.page"
        :page-size="query.pageSize"
        :total="total"
        layout="prev,pager,next"
        @change="handlePageChange"
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
  </main>
</template>
<style scoped>
.refund-page {
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
@media (max-width: 900px) {
  .refund-page {
    height: auto;
    grid-template-columns: 1fr;
  }
  .queue-pane {
    height: 520px;
  }
}
</style>
