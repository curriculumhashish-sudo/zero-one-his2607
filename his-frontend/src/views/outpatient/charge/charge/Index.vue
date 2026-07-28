<script setup lang="ts">
import type {
  ChargeDetail,
  ChargeItem,
  ChargePatient,
  PaymentChannel,
  PreSettlement
} from '@/apis/outpatient/charge/charge'
import type { PatientSummary } from '@/components/patient/type'
import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { CreditCard, Printer, Refresh, Switch } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import {
  getChargeReceipt,
  getPendingChargeDetail,
  pageChargePatientQueue,
  preSettleOutpatientCharge,
  queryPaymentResult,
  recoverSettlement,
  settleOutpatientCharge,
  switchChargeInsuranceMode
} from '@/apis/outpatient/charge/charge'
import { toChargePatientSummary } from './adapter'
import SettlementDialog from './components/SettlementDialog.vue'
import type { ChargePageQuery } from './type'

defineOptions({ name: 'OutpatientCharge' })
const query = reactive<ChargePageQuery>({
  keyword: '',
  status: 'pending',
  page: 1,
  pageSize: 10
})
const patients = ref<ChargePatient[]>([])
const total = ref(0)
const queueLoading = ref(false)
const detailLoading = ref(false)
const errorMessage = ref('')
const selectedEncounterId = ref<string | null>(null)
const detail = ref<ChargeDetail | null>(null)
const selectedItems = ref<ChargeItem[]>([])
const settlement = ref<PreSettlement | null>(null)
const settling = ref(false)
const pendingPaymentId = ref('')
const lastRecordId = ref('')
const dialog = ref<InstanceType<typeof SettlementDialog>>()
let queueSequence = 0
let detailSequence = 0
const queueRows = computed(() => patients.value.map(toChargePatientSummary))
const currentPatient = computed<PatientSummary | null>(() =>
  detail.value ? toChargePatientSummary(detail.value.patient) : null
)
const summaryItems = computed(() =>
  detail.value
    ? [
        {
          key: 'total',
          label: '费用合计',
          value: `¥${detail.value.summary.totalAmount.toFixed(2)}`
        },
        {
          key: 'insurance',
          label: '医保支付',
          value: `¥${detail.value.summary.insuranceAmount.toFixed(2)}`
        },
        {
          key: 'self',
          label: '个人支付',
          value: `¥${detail.value.summary.selfPayAmount.toFixed(2)}`
        },
        { key: 'selected', label: '已选项目', value: `${selectedItems.value.length} 项` }
      ]
    : []
)
const toolbarActions = computed(() => [
  {
    key: 'settle',
    label: '预结算',
    type: 'primary' as const,
    icon: CreditCard,
    disabled: !selectedItems.value.length || settling.value || Boolean(pendingPaymentId.value)
  },
  {
    key: 'insurance',
    label: detail.value?.insuranceMode === 'insurance' ? '切换为自费' : '切换为医保',
    icon: Switch,
    disabled: !detail.value || settling.value || Boolean(pendingPaymentId.value)
  },
  { key: 'receipt', label: '补打票据', icon: Printer, disabled: !lastRecordId.value },
  { key: 'refresh', label: '刷新', icon: Refresh }
])

async function loadQueue() {
  const seq = ++queueSequence
  queueLoading.value = true
  errorMessage.value = ''
  try {
    const result = await pageChargePatientQueue(query)
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
      errorMessage.value = error instanceof Error ? error.message : '患者队列加载失败'
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
  settlement.value = null
  pendingPaymentId.value = ''
  lastRecordId.value = ''
  dialog.value?.close()
}
async function selectPatient(patient: PatientSummary) {
  clearSelection()
  const encounterId = String(patient.id)
  selectedEncounterId.value = encounterId
  const seq = ++detailSequence
  detailLoading.value = true
  try {
    const result = await getPendingChargeDetail(encounterId)
    if (seq !== detailSequence) return
    detail.value = result
    pendingPaymentId.value = result.interruptedPaymentId ?? ''
    lastRecordId.value = result.latestSettledRecordId ?? ''
  } catch (error) {
    if (seq === detailSequence) {
      clearSelection()
      ElMessage.error(error instanceof Error ? error.message : '收费明细加载失败')
    }
  } finally {
    if (seq === detailSequence) detailLoading.value = false
  }
}
function changeStatus(value: 'all' | 'pending' | 'settled') {
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
async function preSettle() {
  if (!detail.value || settling.value) return
  if (pendingPaymentId.value) {
    ElMessage.warning('请先确认未完成支付结果，避免重复扣款')
    return
  }
  settling.value = true
  try {
    settlement.value = await preSettleOutpatientCharge({
      encounterId: detail.value.patient.encounterId,
      itemIds: selectedItems.value.map((item) => item.id)
    })
    dialog.value?.open()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '预结算失败')
  } finally {
    settling.value = false
  }
}
async function confirmSettlement(channel: PaymentChannel) {
  if (!settlement.value || settling.value) return
  try {
    await ElMessageBox.confirm(
      `确认按所选渠道收取 ¥${settlement.value.summary.totalAmount.toFixed(2)}？`,
      '收费确认',
      { type: 'warning' }
    )
  } catch {
    return
  }
  settling.value = true
  try {
    const record = await settleOutpatientCharge({
      settlementId: settlement.value.settlementId,
      encounterId: settlement.value.encounterId,
      itemIds: settlement.value.itemIds,
      channel
    })
    dialog.value?.close()
    lastRecordId.value = record.status === 'success' ? record.id : ''
    pendingPaymentId.value = record.status === 'pending' ? record.id : ''
    ElMessage.success(
      record.status === 'success'
        ? record.printStatus === 'auto-queued'
          ? '收费结算成功，票据已加入自动打印队列'
          : '收费结算成功'
        : '扫码支付已发起，请确认支付结果'
    )
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '收费失败')
  } finally {
    settling.value = false
  }
}
async function refreshCurrent() {
  const id = selectedEncounterId.value
  await loadQueue()
  if (id) await selectPatient({ id } as PatientSummary)
}
async function confirmPending(recover = false) {
  if (!pendingPaymentId.value || settling.value) return
  settling.value = true
  try {
    const record = recover
      ? await recoverSettlement(pendingPaymentId.value)
      : await queryPaymentResult(pendingPaymentId.value)
    lastRecordId.value = record.id
    pendingPaymentId.value = ''
    ElMessage.success(
      record.printStatus === 'scheduled' ? '支付结果已确认，收费票据已安排打印' : '支付结果已确认'
    )
    await refreshCurrent()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '支付结果确认失败')
  } finally {
    settling.value = false
  }
}
async function switchInsurance() {
  if (!detail.value || settling.value) return
  const mode = detail.value.insuranceMode === 'insurance' ? 'self-pay' : 'insurance'
  try {
    await ElMessageBox.confirm(
      `确认将本次就诊切换为${mode === 'insurance' ? '医保' : '自费'}并重新计算？`,
      '费用属性切换',
      { type: 'warning' }
    )
    detail.value = await switchChargeInsuranceMode({
      encounterId: detail.value.patient.encounterId,
      mode
    })
    selectedItems.value = []
    ElMessage.success('费用属性及金额已重新计算')
  } catch (error) {
    if (error instanceof Error) ElMessage.error(error.message)
  }
}
async function printReceipt() {
  if (!lastRecordId.value) return
  try {
    const receipt = await getChargeReceipt(lastRecordId.value)
    await ElMessageBox.alert(receipt.previewText, '收费票据预览', { confirmButtonText: '关闭' })
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '票据加载失败')
  }
}
function handleToolbar(action: { key: string | number }) {
  if (action.key === 'settle') void preSettle()
  else if (action.key === 'insurance') void switchInsurance()
  else if (action.key === 'receipt') void printReceipt()
  else void refreshCurrent()
}
onMounted(() => void loadQueue())
onBeforeUnmount(() => {
  queueSequence += 1
  detailSequence += 1
})
</script>
<template>
  <main class="charge-page">
    <aside class="queue-pane">
      <div class="queue-status">
        <el-segmented
          :model-value="query.status"
          :options="[
            { label: '待收费', value: 'pending' },
            { label: '已收费', value: 'settled' },
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
      /><el-pagination
        v-if="total > query.pageSize"
        v-model:current-page="query.page"
        :page-size="query.pageSize"
        :total="total"
        layout="prev,pager,next"
        @change="loadQueue"
      />
    </aside>
    <section class="charge-workspace">
      <el-alert v-if="errorMessage" type="error" :title="errorMessage" :closable="false" show-icon
        ><el-button type="primary" link @click="loadQueue">重试</el-button></el-alert
      ><EmptyState
        v-if="!selectedEncounterId"
        title="请选择患者"
        description="从左侧队列选择患者后办理收费"
      /><template v-else
        ><PatientInfoBar :patient="currentPatient" /><el-alert
          v-if="pendingPaymentId"
          type="warning"
          :closable="false"
          title="发现未完成的扫码支付，请先确认结果，避免重复扣款。"
          ><el-button type="primary" link :loading="settling" @click="confirmPending(false)"
            >查询支付结果</el-button
          ><el-button link :disabled="settling" @click="confirmPending(true)"
            >恢复结算</el-button
          ></el-alert
        ><el-alert
          v-if="detail?.receiptWarning"
          type="error"
          :closable="false"
          show-icon
          :title="detail.receiptWarning"
          description="收费记录已成功，但电子票据状态异常；补打前必须先核对票据平台状态。"
        />
        <DetailSummary
          title="收费汇总"
          :items="summaryItems"
          :loading="detailLoading"
        /><ActionToolbar
          title="待收费项目"
          :actions="toolbarActions"
          :selection-count="selectedItems.length"
          @action="handleToolbar"
        /><DataTablePanel
          :data="detail?.items ?? []"
          :loading="detailLoading"
          :total="detail?.items.length ?? 0"
          selectable
          min-height="320px"
          @selectable="selectedItems = $event"
          ><el-table-column prop="category" label="类别" width="90" /><el-table-column
            prop="itemName"
            label="项目名称"
            min-width="180"
          /><el-table-column prop="specification" label="规格" width="130" /><el-table-column
            prop="quantity"
            label="数量"
            width="70"
          /><el-table-column prop="unitPrice" label="单价" width="90"
            ><template #default="{ row }"
              >¥{{ row.unitPrice.toFixed(2) }}</template
            ></el-table-column
          ><el-table-column prop="amount" label="金额" width="100"
            ><template #default="{ row }">¥{{ row.amount.toFixed(2) }}</template></el-table-column
          ><el-table-column prop="insuranceAmount" label="医保" width="100"
            ><template #default="{ row }"
              >¥{{ row.insuranceAmount.toFixed(2) }}</template
            ></el-table-column
          ><el-table-column prop="selfPayAmount" label="自费" width="100"
            ><template #default="{ row }"
              >¥{{ row.selfPayAmount.toFixed(2) }}</template
            ></el-table-column
          ><el-table-column label="状态" width="90"
            ><template #default="{ row }"
              ><el-tag :type="row.status === 'pending' ? 'warning' : 'success'">{{
                row.status === 'pending' ? '待收费' : '已收费'
              }}</el-tag></template
            ></el-table-column
          ></DataTablePanel
        ></template
      >
    </section>
    <SettlementDialog
      ref="dialog"
      :data="settlement"
      :submitting="settling"
      @confirm="confirmSettlement"
    />
  </main>
</template>
<style scoped>
.charge-page {
  display: grid;
  height: calc(100vh - 96px);
  min-height: 620px;
  grid-template-columns: 320px minmax(0, 1fr);
  gap: 14px;
  padding: 16px;
}
.queue-pane,
.charge-workspace {
  min-height: 0;
}
.queue-pane {
  display: flex;
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
.queue-pane :deep(.el-pagination) {
  justify-content: center;
  padding: 8px;
}
.charge-workspace {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 12px;
  overflow: auto;
}
@media (max-width: 900px) {
  .charge-page {
    height: auto;
    grid-template-columns: 1fr;
  }
  .queue-pane {
    height: 520px;
  }
}
</style>
