<script setup lang="ts">
import type {
  ClientScanResultDto,
  ConfirmInpatientSettlePayload,
  InpatientSettleChargeCategory,
  InpatientSettleChargeLine,
  InpatientSettleInitResult,
  InpatientSettlePatient,
  InpatientSettleSummary,
  InpatientSettlementHistory,
  InpatientSettlementPayment,
  PreSettleInpatientResult
} from '@/apis/inpatient/charge/settle'
import type { ActionToolbarItem, DateRangeValue } from '@/components/common/type'
import type { SelectTableLoadPage } from '@/components/common/select-type'
import type { PatientSummary } from '@/components/patient/type'

import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Refresh, Search, Wallet } from '@element-plus/icons-vue'

import {
  confirmInpatientSettle,
  getInpatientSettleSummary,
  getSettledInpatientPayment,
  listInpatientSettleCharges,
  listInpatientSettlementHistory,
  loadInpatientSettleInit,
  pageInpatientSettleQueue,
  preSettleInpatientCharges,
  reverseInpatientSettle
} from '@/apis/inpatient/charge/settle'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import SelectTable from '@/components/common/SelectTable.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import { toSettlePatientSummary, toSettleSummaryItems } from './adapter'
import InpatientSettleDialog from './components/InpatientSettleDialog.vue'
import { SETTLEMENT_RECORD_STATUS_OPTIONS, SETTLE_LINE_STATUS_OPTIONS } from './constants'
import type { InpatientSettlePageQuery } from './type'

defineOptions({ name: 'InpatientChargeSettle' })

const query = reactive<InpatientSettlePageQuery>({
  admissionDate: null,
  keyword: '',
  pageNo: 1,
  pageSize: 20,
  presentationMode: 'DETAIL',
  settleQueueTab: 'PENDING_SETTLE'
})
const init = ref<InpatientSettleInitResult | null>(null)
const queueRows = ref<InpatientSettlePatient[]>([])
const total = ref(0)
const selectedPatient = ref<InpatientSettlePatient | null>(null)
const selectedPatientId = ref<string | null>(null)
const patientSelectorVisible = ref(false)
const summary = ref<InpatientSettleSummary | null>(null)
const categories = ref<InpatientSettleChargeCategory[]>([])
const lines = ref<InpatientSettleChargeLine[]>([])
const selectedLines = ref<InpatientSettleChargeLine[]>([])
const history = ref<InpatientSettlementHistory | null>(null)
const preview = ref<PreSettleInpatientResult | null>(null)
const activeTab = ref('categories')
const settleMode = ref('INTERIM')
const dialogVisible = ref(false)
const queueLoading = ref(false)
const detailLoading = ref(false)
const submitting = ref(false)
const pendingSettlePayload = ref<ConfirmInpatientSettlePayload | null>(null)
const pendingSettleMessage = ref('')
const errorMessage = ref('')
let queueSequence = 0
let detailSequence = 0
let summarySequence = 0
let queueController: AbortController | undefined
let detailController: AbortController | undefined
let summaryController: AbortController | undefined

const patientColumns = [
  { prop: 'encounterBusNo', label: '住院号', width: 170 },
  { prop: 'patientName', label: '姓名', width: 100 },
  { prop: 'genderEnumText', label: '性别', width: 70 },
  { prop: 'ageText', label: '年龄', width: 70 },
  { prop: 'organizationName', label: '科室', minWidth: 130 },
  { prop: 'wardName', label: '病区', minWidth: 120 },
  { prop: 'bedName', label: '床号', width: 80 },
  { prop: 'statusEnumText', label: '就诊状态', width: 100 },
  { prop: 'accountBalanceSummaryText', label: '账户状态', minWidth: 120 }
]

const patientCards = computed(() => queueRows.value.map(toSettlePatientSummary))
const currentPatient = computed<PatientSummary | null>(() =>
  selectedPatient.value ? toSettlePatientSummary(selectedPatient.value) : null
)
const selectedPatientRows = computed(() =>
  selectedPatient.value
    ? [selectedPatient.value as InpatientSettlePatient & Record<string, unknown>]
    : []
)
const summaryItems = computed(() => toSettleSummaryItems(summary.value))
const actions = computed<ActionToolbarItem[]>(() => [
  {
    key: 'settle',
    label: '预结算',
    type: 'primary',
    icon: Wallet,
    disabled:
      !selectedPatient.value ||
      selectedLines.value.length === 0 ||
      selectedPatient.value.statusEnum !== 'PENDING_SETTLE',
    loading: submitting.value
  },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: detailLoading.value }
])

function clearDetail() {
  detailController?.abort()
  summaryController?.abort()
  detailSequence += 1
  summarySequence += 1
  summary.value = null
  categories.value = []
  lines.value = []
  selectedLines.value = []
  history.value = null
  preview.value = null
  dialogVisible.value = false
}

async function initialize() {
  try {
    init.value = await loadInpatientSettleInit()
    query.presentationMode = init.value.defaultPresentationMode
    query.settleQueueTab = init.value.defaultSettleQueueTab
    await loadQueue()
  } catch (error) {
    errorMessage.value = error instanceof Error ? error.message : '住院结算初始化失败'
  }
}

async function loadQueue() {
  queueController?.abort()
  const controller = new AbortController()
  queueController = controller
  const sequence = ++queueSequence
  queueLoading.value = true
  errorMessage.value = ''
  try {
    const result = await pageInpatientSettleQueue(
      {
        admissionDate: query.admissionDate ?? undefined,
        keyword: query.keyword,
        PageIndex: query.pageNo,
        PageSize: query.pageSize,
        settleQueueTab: query.settleQueueTab
      },
      { signal: controller.signal }
    )
    if (sequence !== queueSequence) return
    queueRows.value = result.content
    total.value = result.totalCount
    if (
      selectedPatient.value &&
      !result.content.some((item) => item.encounterId === selectedPatient.value?.encounterId)
    ) {
      selectedPatient.value = null
      selectedPatientId.value = null
      clearDetail()
    }
  } catch (error) {
    if (controller.signal.aborted || sequence !== queueSequence) return
    queueRows.value = []
    total.value = 0
    errorMessage.value = error instanceof Error ? error.message : '患者列表加载失败'
  } finally {
    if (sequence === queueSequence) queueLoading.value = false
  }
}

async function loadPatientData(patient: InpatientSettlePatient) {
  detailController?.abort()
  const controller = new AbortController()
  detailController = controller
  const sequence = ++detailSequence
  detailLoading.value = true
  errorMessage.value = ''
  try {
    const [summaryResult, chargeResult, historyResult] = await Promise.all([
      getInpatientSettleSummary(patient.encounterId, patient.patientId, [], {
        signal: controller.signal
      }),
      listInpatientSettleCharges(patient.encounterId, patient.patientId, query.presentationMode, {
        signal: controller.signal
      }),
      listInpatientSettlementHistory(patient.encounterId, patient.patientId, {
        signal: controller.signal
      })
    ])
    if (sequence !== detailSequence) return
    summary.value = summaryResult
    categories.value = chargeResult.categories ?? []
    lines.value = chargeResult.lines ?? []
    history.value = historyResult
    selectedLines.value = (chargeResult.lines ?? []).filter(
      (line) => line.statusEnum === 'PENDING' && line.selected
    )
  } catch (error) {
    if (controller.signal.aborted || sequence !== detailSequence) return
    clearDetail()
    errorMessage.value = error instanceof Error ? error.message : '患者结算数据加载失败'
  } finally {
    if (sequence === detailSequence) detailLoading.value = false
  }
}

function activatePatient(patient: InpatientSettlePatient) {
  if (submitting.value || pendingSettlePayload.value) {
    if (pendingSettlePayload.value) ElMessage.warning('请先完成待补结算交易')
    return
  }
  clearDetail()
  selectedPatient.value = patient
  selectedPatientId.value = patient.encounterId
  void loadPatientData(patient)
}

function selectPatient(source: PatientSummary) {
  const patient = queueRows.value.find((item) => item.encounterId === String(source.id))
  if (patient) activatePatient(patient)
}

const loadSelectablePatients: SelectTableLoadPage<
  InpatientSettlePatient & Record<string, unknown>
> = async (request, context) => {
  const result = await pageInpatientSettleQueue(
    {
      admissionDate: query.admissionDate ?? undefined,
      keyword: request.keyword,
      PageIndex: request.pageIndex,
      PageSize: request.pageSize,
      settleQueueTab: query.settleQueueTab
    },
    { signal: context.signal }
  )
  return {
    rows: result.content as Array<InpatientSettlePatient & Record<string, unknown>>,
    total: result.totalCount
  }
}

function choosePatient(
  _value: unknown,
  rows: Array<InpatientSettlePatient & Record<string, unknown>>
) {
  const patient = rows[0]
  if (patient) activatePatient(patient)
}

function search(keyword: string) {
  if (pendingSettlePayload.value) {
    ElMessage.warning('请先完成待补结算交易')
    return
  }
  query.keyword = keyword
  query.pageNo = 1
  selectedPatient.value = null
  selectedPatientId.value = null
  clearDetail()
  void loadQueue()
}

function changeDate(value: DateRangeValue) {
  if (pendingSettlePayload.value) {
    ElMessage.warning('请先完成待补结算交易')
    return
  }
  query.admissionDate = value?.[0] ?? null
  query.pageNo = 1
  selectedPatient.value = null
  selectedPatientId.value = null
  clearDetail()
  void loadQueue()
}

function changeQueueTab(value: string | number | boolean | undefined) {
  if (value !== 'PENDING_SETTLE' && value !== 'SETTLED') return
  if (pendingSettlePayload.value) {
    ElMessage.warning('请先完成待补结算交易')
    return
  }
  query.settleQueueTab = value
  query.pageNo = 1
  selectedPatient.value = null
  selectedPatientId.value = null
  patientSelectorVisible.value = false
  clearDetail()
  void loadQueue()
}

function handleQueuePaginationChange(payload: { page: number; pageSize: number }) {
  const pageSizeChanged = payload.pageSize !== query.pageSize
  query.pageNo = pageSizeChanged ? 1 : payload.page
  query.pageSize = payload.pageSize
  selectedPatient.value = null
  selectedPatientId.value = null
  clearDetail()
  void loadQueue()
}

async function changeSelection(source: unknown[]) {
  selectedLines.value = (source as InpatientSettleChargeLine[]).filter(
    (line) => line.statusEnum === 'PENDING'
  )
  preview.value = null
  const patient = selectedPatient.value
  if (!patient) return
  summaryController?.abort()
  const controller = new AbortController()
  summaryController = controller
  const sequence = ++summarySequence
  const selectedChargeItemIds = selectedLines.value.map((line) => line.chargeItemId)
  try {
    const value = await getInpatientSettleSummary(
      patient.encounterId,
      patient.patientId,
      selectedChargeItemIds,
      { signal: controller.signal }
    )
    if (sequence !== summarySequence || controller.signal.aborted) return
    const currentIds = selectedLines.value.map((line) => line.chargeItemId)
    if (
      selectedPatient.value?.encounterId === patient.encounterId &&
      currentIds.length === selectedChargeItemIds.length &&
      currentIds.every((id, index) => id === selectedChargeItemIds[index])
    ) {
      summary.value = value
    }
  } catch (error) {
    if (controller.signal.aborted || sequence !== summarySequence) return
    ElMessage.error(error instanceof Error ? error.message : '费用摘要刷新失败')
  }
}

async function preSettle() {
  const patient = selectedPatient.value
  if (!patient || submitting.value || selectedLines.value.length === 0) return
  submitting.value = true
  try {
    preview.value = await preSettleInpatientCharges({
      chargeItemIds: selectedLines.value.map((line) => line.chargeItemId),
      contractId: patient.contractId,
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      settleMode: settleMode.value
    })
    dialogVisible.value = true
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '预结算失败')
  } finally {
    submitting.value = false
  }
}

function createClientScanResult(
  scanCode: string,
  paymentDraftId: string,
  amount: number
): ClientScanResultDto {
  const recoveryTrade = scanCode.toUpperCase().includes('RECOVER')
  return {
    merTradeNo: `MER-${paymentDraftId}`,
    tradeNo: recoveryTrade ? `RECOVER-${paymentDraftId}` : `TRADE-${paymentDraftId}`,
    payType: 'SCAN_PAY',
    respCode: '00',
    respMsg: '支付成功',
    txnAmtFen: Math.round(amount * 100)
  }
}

async function completeSettlement(payload: ConfirmInpatientSettlePayload) {
  await confirmInpatientSettle(payload)
  pendingSettlePayload.value = null
  pendingSettleMessage.value = ''
  dialogVisible.value = false
  ElMessage.success('住院结算成功')
  selectedPatient.value = null
  selectedPatientId.value = null
  clearDetail()
  await loadQueue()
}

async function confirmSettle(data: { payChannelCode: string; scanCode?: string }) {
  const patient = selectedPatient.value
  const draft = preview.value
  if (!patient || !draft || submitting.value) return
  const payableAmount = Number(
    draft.insurancePreSettled ? (draft.displayAmount ?? 0) : draft.receivableTotal
  )
  if (data.payChannelCode === 'SCAN_PAY' && !data.scanCode?.trim() && payableAmount > 0) {
    ElMessage.warning('请输入付款码')
    return
  }
  const payload: ConfirmInpatientSettlePayload = {
    chargeItemIds: selectedLines.value.map((line) => line.chargeItemId),
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    paymentDetails:
      payableAmount > 0 ? [{ amount: payableAmount, payChannelCode: data.payChannelCode }] : [],
    paymentDraftId: draft.paymentDraftId,
    ybPreSettleId: draft.ybPreSettleId
  }
  if (data.payChannelCode === 'SCAN_PAY' && data.scanCode) {
    payload.clientScanResult = createClientScanResult(
      data.scanCode.trim(),
      draft.paymentDraftId,
      payableAmount
    )
  }
  submitting.value = true
  try {
    await completeSettlement(payload)
  } catch (error) {
    const message = error instanceof Error ? error.message : '确认结算失败'
    if (payload.clientScanResult) {
      pendingSettlePayload.value = payload
      pendingSettleMessage.value = message
      dialogVisible.value = false
      ElMessage.warning('扫码结果已保留，请执行补结算')
    } else {
      ElMessage.error(message)
    }
  } finally {
    submitting.value = false
  }
}

async function recoverSettlement() {
  const payload = pendingSettlePayload.value
  if (!payload || submitting.value) return
  submitting.value = true
  try {
    await completeSettlement(payload)
  } catch (error) {
    pendingSettleMessage.value = error instanceof Error ? error.message : '补结算失败'
    ElMessage.error(pendingSettleMessage.value)
  } finally {
    submitting.value = false
  }
}

async function viewSettlement(source: unknown) {
  const payment = source as InpatientSettlementPayment
  try {
    const patient = selectedPatient.value
    if (!patient) return
    history.value = await getSettledInpatientPayment(
      patient.encounterId,
      patient.patientId,
      payment.paymentId
    )
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '结算单加载失败')
  }
}

async function reverseSettlement(source: unknown) {
  const payment = source as InpatientSettlementPayment
  if (!payment.reverseEnabled || submitting.value) return
  try {
    const { value } = await ElMessageBox.prompt('请输入反结原因', '反结确认', {
      type: 'warning',
      inputValidator: (reason) => Boolean(reason.trim()) || '请输入反结原因'
    })
    submitting.value = true
    const patient = selectedPatient.value
    if (!patient) return
    await reverseInpatientSettle(patient.encounterId, patient.patientId, payment.paymentId, value)
    ElMessage.success('反结成功')
    selectedPatient.value = null
    selectedPatientId.value = null
    clearDetail()
    await loadQueue()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '反结失败')
  } finally {
    submitting.value = false
  }
}

function handleToolbar(action: ActionToolbarItem) {
  if (action.key === 'settle') void preSettle()
  else if (selectedPatient.value) void loadPatientData(selectedPatient.value)
}

onMounted(() => void initialize())
onBeforeUnmount(() => {
  queueController?.abort()
  detailController?.abort()
  summaryController?.abort()
  queueSequence += 1
  detailSequence += 1
  summarySequence += 1
})
</script>

<template>
  <main class="settle-page">
    <aside class="queue-pane">
      <div class="queue-tabs">
        <el-segmented
          :model-value="query.settleQueueTab"
          :options="init?.settleQueueTabOptions ?? []"
          :disabled="submitting"
          @change="changeQueueTab"
        />
      </div>
      <div class="patient-selector-trigger">
        <el-button
          type="primary"
          plain
          :icon="Search"
          :disabled="submitting || Boolean(pendingSettlePayload)"
          @click="patientSelectorVisible = true"
        >
          选择住院患者
        </el-button>
        <span v-if="selectedPatient">当前：{{ selectedPatient.patientName }}</span>
        <span v-else>按姓名或住院号分页查询</span>
      </div>
      <PatientQueuePanel
        :patients="patientCards"
        :selected-id="selectedPatient?.encounterId"
        :loading="queueLoading"
        @search="search"
        @date-change="changeDate"
        @refresh="loadQueue"
        @select="selectPatient"
      />
      <PaginationBar
        v-if="total > 0"
        class="queue-pagination"
        :page="query.pageNo"
        :page-size="query.pageSize"
        :total="total"
        :disabled="queueLoading || submitting"
        small
        @change="handleQueuePaginationChange"
      />
    </aside>

    <section class="settle-workspace">
      <el-alert
        v-if="pendingSettlePayload"
        type="warning"
        :closable="false"
        show-icon
        title="扫码扣款结果已保留，住院结算尚未完成"
        :description="pendingSettleMessage"
      >
        <el-button type="warning" link :loading="submitting" @click="recoverSettlement">
          补结算
        </el-button>
      </el-alert>
      <el-alert v-if="errorMessage" :title="errorMessage" type="error" :closable="false" show-icon>
        <el-button
          type="primary"
          link
          @click="selectedPatient ? loadPatientData(selectedPatient) : loadQueue()"
          >重试</el-button
        >
      </el-alert>
      <EmptyState
        v-if="!selectedPatient"
        title="请选择住院患者"
        description="选择患者后查看费用汇总、费用大类、费用明细和历史结算"
      />
      <template v-else>
        <PatientInfoBar :patient="currentPatient" />
        <DetailSummary title="费用合计" :items="summaryItems" :loading="detailLoading">
          <template #extra>
            <el-tag v-if="summary?.lowBalanceWarning" type="danger">余额不足</el-tag>
          </template>
        </DetailSummary>
        <ActionToolbar
          title="住院费用结算"
          :description="`已选择 ${selectedLines.length} 项费用`"
          :actions="actions"
          :disabled="submitting"
          @action="handleToolbar"
        >
          <template #left>
            <div class="settle-options">
              <strong>住院费用结算</strong>
              <el-radio-group v-model="settleMode" :disabled="submitting" size="small">
                <el-radio-button value="INTERIM">中途结算</el-radio-button>
                <el-radio-button value="DISCHARGE">出院结算</el-radio-button>
              </el-radio-group>
            </div>
          </template>
        </ActionToolbar>
        <el-tabs v-model="activeTab" class="data-tabs">
          <el-tab-pane label="费用大类" name="categories">
            <DataTablePanel
              :data="categories"
              :loading="detailLoading"
              :total="categories.length"
              min-height="280px"
            >
              <el-table-column prop="categoryDisplay" label="费用类别" min-width="160" />
              <el-table-column prop="lineCount" label="项目数" width="100" />
              <el-table-column prop="amount" label="金额" width="130">
                <template #default="{ row }">¥{{ Number(row.amount).toFixed(2) }}</template>
              </el-table-column>
            </DataTablePanel>
          </el-tab-pane>
          <el-tab-pane label="费用明细" name="details">
            <DataTablePanel
              :data="lines"
              :loading="detailLoading"
              :total="lines.length"
              selectable
              min-height="320px"
              @selectable="changeSelection"
            >
              <el-table-column prop="occurrenceTime" label="发生时间" width="170" />
              <el-table-column prop="categoryDisplay" label="费用类别" width="110" />
              <el-table-column prop="itemName" label="项目名称" min-width="180" />
              <el-table-column prop="specText" label="规格" min-width="120" />
              <el-table-column label="数量" width="100">
                <template #default="{ row }"
                  >{{ row.quantityValue }} {{ row.quantityUnit }}</template
                >
              </el-table-column>
              <el-table-column prop="lineAmount" label="金额" width="110">
                <template #default="{ row }">¥{{ Number(row.lineAmount).toFixed(2) }}</template>
              </el-table-column>
              <el-table-column label="状态" width="100">
                <template #default="{ row }"
                  ><StatusDisplay :value="row.statusEnum" :options="SETTLE_LINE_STATUS_OPTIONS"
                /></template>
              </el-table-column>
            </DataTablePanel>
          </el-tab-pane>
          <el-tab-pane label="历史结算" name="history">
            <DataTablePanel
              :data="history?.payments ?? []"
              :loading="detailLoading"
              :total="history?.payments.length ?? 0"
              min-height="300px"
            >
              <el-table-column prop="paymentId" label="结算单号" min-width="180" />
              <el-table-column prop="settleTime" label="结算时间" width="170" />
              <el-table-column prop="payMethodText" label="支付方式" width="110" />
              <el-table-column label="金额" width="120"
                ><template #default="{ row }"
                  >¥{{ Number(row.signedAmount ?? row.receivableTotal).toFixed(2) }}</template
                ></el-table-column
              >
              <el-table-column label="类型" width="90"
                ><template #default="{ row }">{{
                  row.recordType === 'REVERSE' ? '反结' : '结算'
                }}</template></el-table-column
              >
              <el-table-column label="状态" width="90"
                ><template #default="{ row }"
                  ><StatusDisplay
                    :value="row.statusEnum"
                    :options="SETTLEMENT_RECORD_STATUS_OPTIONS" /></template
              ></el-table-column>
              <el-table-column label="操作" width="150" fixed="right">
                <template #default="{ row }">
                  <el-button type="primary" link @click="viewSettlement(row)">查看</el-button>
                  <el-button
                    type="danger"
                    link
                    :disabled="!row.reverseEnabled || submitting"
                    @click="reverseSettlement(row)"
                    >反结</el-button
                  >
                </template>
              </el-table-column>
            </DataTablePanel>
          </el-tab-pane>
        </el-tabs>
      </template>
    </section>
    <InpatientSettleDialog
      v-model="dialogVisible"
      :init="init"
      :preview="preview"
      :submitting="submitting"
      @confirm="confirmSettle"
    />
    <SelectTable
      v-model="selectedPatientId"
      v-model:visible="patientSelectorVisible"
      row-key="encounterId"
      title="选择住院患者"
      width="1050px"
      :columns="patientColumns"
      :load-page="loadSelectablePatients"
      :selected-rows="selectedPatientRows"
      search-label="患者信息"
      keyword-placeholder="请输入姓名或住院号"
      @confirm="choosePatient"
    />
  </main>
</template>

<style scoped>
.settle-page {
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
  min-height: 650px;
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
.queue-tabs {
  padding: 12px 12px 0;
}
.queue-pagination {
  justify-content: center;
  padding: 8px;
}
.queue-tabs :deep(.el-segmented) {
  width: 100%;
}
.patient-selector-trigger {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 10px;
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}
.patient-selector-trigger span {
  min-width: 0;
  overflow: hidden;
  color: var(--el-text-color-regular);
  font-size: 13px;
  text-overflow: ellipsis;
  white-space: nowrap;
}
.queue-pane :deep(.patient-queue-panel) {
  min-height: 0;
  border: 0;
}
.settle-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  gap: 12px;
  overflow: auto;
}
.settle-options {
  display: flex;
  align-items: center;
  gap: 14px;
}
.settle-options strong {
  color: #1f2937;
  font-size: 15px;
  font-weight: 700;
}
.data-tabs {
  min-height: 360px;
}
@media (max-width: 960px) {
  .settle-page {
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
