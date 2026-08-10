<script setup lang="ts">
import type {
  BookableSlotHint,
  ClientScanResultDto,
  ConfirmOutpatientRegisterPayload,
  LoadRegisterBenefitPreviewResult,
  PreSettleOutpatientRegisterResult,
  RegisterHistoryRecord,
  RegisterInitResult,
  RegisterOption,
  RegisterPatient,
  RegisterTicketResult,
  TodayRegisterSummary
} from '@/apis/outpatient/charge/register'
import type { SelectTableLoadPage } from '@/components/common/select-type'

import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  cancelOutpatientRegister,
  cancelRegisterWithRefund,
  confirmOutpatientRegister,
  getRegisterAppointment,
  getRegisterTicket,
  getTodayRegisterSummary,
  listBookableSlotHints,
  listRegisterPractitioners,
  loadRegisterBenefitPreview,
  loadOutpatientRegisterInit,
  pageOutpatientRegisterHistory,
  pageRegisterPatientOptions,
  preSettleOutpatientRegister,
  validateRegisterCancellation
} from '@/apis/outpatient/charge/register'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DescriptionList from '@/components/common/DescriptionList.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import PrintActionPanel from '@/components/common/PrintActionPanel.vue'
import { formatLocalDate, toTicketDetail } from './adapter'
import DailySlotPanel from './components/DailySlotPanel.vue'
import RegisterConfirmDialog from './components/RegisterConfirmDialog.vue'
import RegisterFormPanel from './components/RegisterFormPanel.vue'
import RegisterHistoryPanel from './components/RegisterHistoryPanel.vue'
import type { OutpatientRegisterFormModel } from './type'

defineOptions({ name: 'OutpatientChargeRegister' })

const today = formatLocalDate()
type UiRegisterOption = RegisterOption & Record<string, unknown>
const init = ref<RegisterInitResult | null>(null)
const form = reactive<OutpatientRegisterFormModel>({
  appointmentId: '',
  appointmentMode: 'WALK_IN',
  age: null,
  birthDate: '',
  contractId: 'CONTRACT-SELF',
  gender: 'MALE',
  idCardNo: '',
  name: '',
  organizationId: '',
  patientId: '',
  patientBusNo: '',
  phone: '',
  practitionerId: '',
  registrationDate: today,
  serviceTypeId: 'SERVICE-NORMAL',
  slotId: '',
  ybRegisterFee: false,
  ybUseAcctPay: false,
  psnNo: '',
  insuplcAdmdvs: '',
  insutype: '',
  ybMediumType: '02',
  ybCardInfo: '',
  medType: '12'
})
const doctorOptions = ref<UiRegisterOption[]>([])
const slots = ref<BookableSlotHint[]>([])
const history = ref<RegisterHistoryRecord[]>([])
const historyTotal = ref(0)
const summary = ref<TodayRegisterSummary | null>(null)
const preview = ref<PreSettleOutpatientRegisterResult | null>(null)
const benefitPreview = ref<LoadRegisterBenefitPreviewResult | null>(null)
const ticket = ref<RegisterTicketResult | null>(null)
const confirmVisible = ref(false)
const ticketVisible = ref(false)
const rightTab = ref('slots')
const loading = ref(false)
const slotLoading = ref(false)
const submitting = ref(false)
const benefitLoading = ref(false)
const historyQuery = reactive({ pageIndex: 1, pageSize: 20 })
const pendingRegisterPayload = ref<ConfirmOutpatientRegisterPayload | null>(null)
const pendingRegisterMessage = ref('')
const errorMessage = ref('')
let dashboardSequence = 0
let slotSequence = 0
let dashboardController: AbortController | undefined
let slotController: AbortController | undefined

const summaryItems = computed(() =>
  summary.value
    ? [
        { key: 'total', label: '今日挂号', value: summary.value.totalCount },
        { key: 'active', label: '有效挂号', value: summary.value.activeCount },
        { key: 'cancelled', label: '已退号', value: summary.value.cancelledCount }
      ]
    : []
)
const ticketDetail = computed(() => toTicketDetail(ticket.value))
const ticketItems = [
  { prop: 'slipNo', label: '小票号' },
  { prop: 'patientName', label: '患者姓名' },
  { prop: 'gender', label: '性别' },
  { prop: 'department', label: '挂号科室' },
  { prop: 'doctor', label: '医生' },
  { prop: 'serviceType', label: '服务类型' },
  { prop: 'feeNature', label: '费用性质' },
  { prop: 'total', label: '合计金额' },
  { prop: 'chargeTime', label: '收费时间' },
  { prop: 'cashier', label: '收费员' },
  { prop: 'printCount', label: '打印次数' }
]
const insuranceSelected = computed(() =>
  ['3', '4'].includes(init.value?.contractCategoryById?.[form.contractId] ?? '')
)

function resetForm() {
  Object.assign(form, {
    appointmentId: '',
    appointmentMode: 'WALK_IN',
    age: null,
    birthDate: '',
    contractId: 'CONTRACT-SELF',
    gender: 'MALE',
    idCardNo: '',
    name: '',
    organizationId: '',
    patientId: '',
    patientBusNo: '',
    phone: '',
    practitionerId: '',
    registrationDate: today,
    serviceTypeId: 'SERVICE-NORMAL',
    slotId: '',
    ybRegisterFee: false,
    ybUseAcctPay: false,
    psnNo: '',
    insuplcAdmdvs: '',
    insutype: '',
    ybMediumType: '02',
    ybCardInfo: '',
    medType: '12'
  })
  doctorOptions.value = []
  preview.value = null
  benefitPreview.value = null
}

function requestReset() {
  if (pendingRegisterPayload.value) {
    ElMessage.warning('请先完成待补结算交易')
    return
  }
  resetForm()
}

function changeHistoryPage(value: number) {
  historyQuery.pageIndex = value
  void loadDashboard()
}

function changeHistoryPageSize(value: number) {
  historyQuery.pageSize = value
  historyQuery.pageIndex = 1
  void loadDashboard()
}

function updateForm(value: OutpatientRegisterFormModel) {
  Object.assign(form, value)
}

async function initialize() {
  loading.value = true
  try {
    init.value = await loadOutpatientRegisterInit()
    if (!init.value.serviceTypeOptions.some((item) => item.value === form.serviceTypeId)) {
      form.serviceTypeId = init.value.serviceTypeOptions[0]?.value ?? ''
    }
    if (!init.value.contractOptions.some((item) => item.value === form.contractId)) {
      form.contractId = init.value.contractOptions[0]?.value ?? ''
    }
    await loadDashboard()
  } catch (error) {
    errorMessage.value = error instanceof Error ? error.message : '门诊挂号初始化失败'
  } finally {
    loading.value = false
  }
}

async function loadDashboard() {
  dashboardController?.abort()
  const controller = new AbortController()
  dashboardController = controller
  const sequence = ++dashboardSequence
  loading.value = true
  errorMessage.value = ''
  try {
    const [historyResult, summaryResult] = await Promise.all([
      pageOutpatientRegisterHistory(
        {
          dateFrom: today,
          dateTo: today,
          pageIndex: historyQuery.pageIndex,
          pageSize: historyQuery.pageSize
        },
        { signal: controller.signal }
      ),
      getTodayRegisterSummary({ date: today }, { signal: controller.signal })
    ])
    if (sequence !== dashboardSequence) return
    history.value = historyResult.records
    historyTotal.value = Number(historyResult.total)
    summary.value = summaryResult
  } catch (error) {
    if (controller.signal.aborted || sequence !== dashboardSequence) return
    history.value = []
    historyTotal.value = 0
    errorMessage.value = error instanceof Error ? error.message : '挂号记录加载失败'
  } finally {
    if (sequence === dashboardSequence) loading.value = false
  }
}

async function loadSlots() {
  slotController?.abort()
  const controller = new AbortController()
  slotController = controller
  const sequence = ++slotSequence
  form.slotId = ''
  preview.value = null
  if (!form.organizationId) {
    slots.value = []
    return
  }
  slotLoading.value = true
  try {
    const result = await listBookableSlotHints(
      {
        organizationId: form.organizationId,
        practitionerId: form.practitionerId || undefined,
        healthcareServiceId: form.serviceTypeId || undefined,
        registrationDate: form.registrationDate
      },
      { signal: controller.signal }
    )
    if (sequence === slotSequence) slots.value = result.hints
  } catch (error) {
    if (!controller.signal.aborted && sequence === slotSequence) {
      slots.value = []
      ElMessage.error(error instanceof Error ? error.message : '号源加载失败')
    }
  } finally {
    if (sequence === slotSequence) slotLoading.value = false
  }
}

async function departmentChanged() {
  form.practitionerId = ''
  form.slotId = ''
  doctorOptions.value = form.organizationId
    ? (
        await listRegisterPractitioners({ organizationId: form.organizationId })
      ).practitionerOptions.map((item) => ({ ...item }))
    : []
  await loadSlots()
}

const loadDoctors = async (keyword: string, context: { signal: AbortSignal }) =>
  form.organizationId
    ? listRegisterPractitioners(
        { organizationId: form.organizationId, keyword },
        { signal: context.signal }
      ).then((result) => result.practitionerOptions.map((item) => ({ ...item })))
    : []
const loadPatients: SelectTableLoadPage<RegisterPatient & Record<string, unknown>> = async (
  request,
  context
) => {
  const result = await pageRegisterPatientOptions(
    { keyword: request.keyword, pageIndex: request.pageIndex, pageSize: request.pageSize },
    { signal: context.signal }
  )
  return {
    rows: result.records as Array<RegisterPatient & Record<string, unknown>>,
    total: Number(result.total)
  }
}

function patientChanged(patient: RegisterPatient) {
  if (pendingRegisterPayload.value) {
    ElMessage.warning('请先完成待补结算交易')
    return
  }
  Object.assign(form, {
    patientId: patient.patientId,
    patientBusNo: patient.patientBusNo,
    name: patient.name,
    age: patient.age ?? null,
    gender: patient.genderCode ?? 'UNKNOWN',
    birthDate: patient.birthDate ?? '',
    idCardNo: patient.idCardNoMasked ?? '',
    phone: patient.phoneMasked ?? ''
  })
  preview.value = null
  benefitPreview.value = null
  if (insuranceSelected.value) void readInsuranceBenefit()
}

function selectSlot(slot: BookableSlotHint) {
  if ((slot.remainingCount ?? 0) <= 0 || submitting.value || slot.slotId == null) return
  Object.assign(form, {
    slotId: String(slot.slotId)
  })
  preview.value = null
}

async function loadAppointment() {
  if (!form.appointmentId) return
  try {
    const data = await getRegisterAppointment(form.appointmentId)
    Object.assign(form, {
      appointmentId: String(data.appointmentId),
      organizationId: String(data.organizationId),
      practitionerId: data.practitionerId == null ? '' : String(data.practitionerId),
      registrationDate: data.slotStart?.slice(0, 10) ?? form.registrationDate,
      serviceTypeId: String(data.healthcareServiceId),
      slotId: data.slotId == null ? '' : String(data.slotId)
    })
    doctorOptions.value = (
      await listRegisterPractitioners({ organizationId: form.organizationId })
    ).practitionerOptions.map((item) => ({ ...item }))
    await loadSlots()
    form.slotId = data.slotId == null ? '' : String(data.slotId)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '预约信息加载失败')
  }
}

async function readInsuranceBenefit() {
  if (!insuranceSelected.value) {
    benefitPreview.value = null
    return
  }
  benefitLoading.value = true
  try {
    const result = await loadRegisterBenefitPreview({
      contractId: form.contractId,
      insuplcAdmdvs: form.insuplcAdmdvs || undefined,
      insutype: form.insutype || undefined,
      medType: form.medType || undefined,
      patientId: form.patientId || undefined,
      psnNo: form.psnNo || undefined,
      runCumulativeQuery: true,
      runOpspFilingQuery: true,
      runTreatmentCheck: true,
      ybCardInfo: form.ybCardInfo || undefined,
      ybMediumType: form.ybMediumType || undefined,
      ybPsnName: form.name || undefined
    })
    benefitPreview.value = result
    form.psnNo = result.ybPreview?.psnNo ?? form.psnNo
    form.insuplcAdmdvs = result.ybPreview?.insuplcAdmdvs ?? form.insuplcAdmdvs
    form.insutype = result.ybPreview?.insutype ?? form.insutype
    if (result.contractMatch?.blockRegister) {
      ElMessage.warning(result.contractMatch.hintMessage || '当前医保待遇不允许挂号')
    }
  } catch (error) {
    benefitPreview.value = null
    ElMessage.error(error instanceof Error ? error.message : '医保待遇读取失败')
  } finally {
    benefitLoading.value = false
  }
}

function contractChanged() {
  preview.value = null
  benefitPreview.value = null
  form.ybRegisterFee = insuranceSelected.value
  if (insuranceSelected.value) void readInsuranceBenefit()
}

async function preSettle() {
  if (submitting.value) return
  if (
    !form.name ||
    !form.birthDate ||
    !form.idCardNo ||
    !form.phone ||
    !form.organizationId ||
    !form.practitionerId ||
    !form.serviceTypeId ||
    !form.contractId ||
    !form.slotId
  ) {
    ElMessage.warning('请完整填写患者、科室、医生、服务类型、费用性质并选择号源')
    return
  }
  if (insuranceSelected.value) {
    if (!benefitPreview.value) {
      ElMessage.warning('请先读取医保待遇信息')
      return
    }
    if (benefitPreview.value.contractMatch?.blockRegister) {
      ElMessage.warning(benefitPreview.value.contractMatch.hintMessage || '当前医保待遇不允许挂号')
      return
    }
  }
  submitting.value = true
  try {
    preview.value = await preSettleOutpatientRegister({
      appointmentId: form.appointmentId || undefined,
      birthDate: form.birthDate,
      contractId: form.contractId,
      gender: form.gender,
      idCardNo: form.idCardNo,
      name: form.name,
      organizationId: form.organizationId,
      patientId: form.patientId || undefined,
      phone: form.phone,
      practitionerId: form.practitionerId,
      serviceTypeId: form.serviceTypeId,
      slotId: form.slotId,
      ybRegisterFee: form.ybRegisterFee,
      ybUseAcctPay: form.ybUseAcctPay,
      psnNo: form.psnNo || undefined,
      insuplcAdmdvs: form.insuplcAdmdvs || undefined,
      insutype: form.insutype || undefined,
      ybMediumType: form.ybMediumType || undefined,
      ybCardInfo: form.ybCardInfo || undefined,
      medType: form.medType || undefined
    })
    confirmVisible.value = true
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '挂号预结算失败')
  } finally {
    submitting.value = false
  }
}

function createClientScanResult(
  scanCode: string,
  paymentDraftId: number | string,
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

async function completeRegister(payload: ConfirmOutpatientRegisterPayload) {
  await confirmOutpatientRegister(payload)
  pendingRegisterPayload.value = null
  pendingRegisterMessage.value = ''
  confirmVisible.value = false
  ElMessage.success('缴费成功，挂号已完成')
  resetForm()
  slots.value = []
  historyQuery.pageIndex = 1
  await loadDashboard()
}

async function confirmRegister(data: { channel: string; scanCode?: string }) {
  const currentPreview = preview.value
  if (!currentPreview || submitting.value) return
  const payableAmount = Number(
    currentPreview.insurancePreSettled
      ? (currentPreview.ybPsnCashPay ?? 0)
      : currentPreview.totalAmount
  )
  if (data.channel === 'SCAN_PAY' && !data.scanCode?.trim() && payableAmount > 0) {
    ElMessage.warning('请输入付款码')
    return
  }
  const payload: ConfirmOutpatientRegisterPayload = {
    appointmentId: currentPreview.appointmentId,
    paymentDetails:
      payableAmount > 0 ? [{ payAmount: payableAmount, payChannelCode: data.channel }] : [],
    paymentDraftId: currentPreview.paymentDraftId
  }
  if (data.channel === 'SCAN_PAY' && data.scanCode) {
    payload.clientScanResult = createClientScanResult(
      data.scanCode.trim(),
      currentPreview.paymentDraftId,
      payableAmount
    )
  }
  submitting.value = true
  try {
    await completeRegister(payload)
  } catch (error) {
    const message = error instanceof Error ? error.message : '挂号确认失败'
    if (payload.clientScanResult) {
      pendingRegisterPayload.value = payload
      pendingRegisterMessage.value = message
      confirmVisible.value = false
      ElMessage.warning('扫码结果已保留，请执行补结算')
    } else {
      ElMessage.error(message)
    }
  } finally {
    submitting.value = false
  }
}

async function recoverRegister() {
  const payload = pendingRegisterPayload.value
  if (!payload || submitting.value) return
  submitting.value = true
  try {
    await completeRegister(payload)
  } catch (error) {
    pendingRegisterMessage.value = error instanceof Error ? error.message : '补结算失败'
    ElMessage.error(pendingRegisterMessage.value)
  } finally {
    submitting.value = false
  }
}

async function cancelRegister(source: unknown) {
  const record = source as RegisterHistoryRecord
  if (submitting.value) return
  try {
    const validation = await validateRegisterCancellation(record.encounterId)
    if (!validation.cancelable) {
      ElMessage.warning(validation.reasonText || '当前挂号记录不允许退号')
      return
    }
    const refundRequired = Boolean(record.paymentId)
    const { value } = await ElMessageBox.prompt(
      refundRequired ? '该记录已缴费，将按原渠道退款。请输入退号原因' : '请输入退号原因',
      '退号确认',
      { type: 'warning', inputValidator: (reason) => Boolean(reason.trim()) || '请输入退号原因' }
    )
    submitting.value = true
    const result = refundRequired
      ? await cancelRegisterWithRefund({
          encounterId: record.encounterId,
          cancelReason: value,
          refundRequestId: crypto.randomUUID()
        })
      : await cancelOutpatientRegister({ encounterId: record.encounterId, cancelReason: value })
    ElMessage.success(
      'appointmentReleaseMessage' in result
        ? result.appointmentReleaseMessage || '退号成功'
        : '退号成功'
    )
    await loadDashboard()
    if (form.organizationId) await loadSlots()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '退号失败')
  } finally {
    submitting.value = false
  }
}

async function openTicket(source: unknown) {
  const record = source as RegisterHistoryRecord
  try {
    ticket.value = await getRegisterTicket(record.encounterId)
    ticketVisible.value = true
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '挂号小票加载失败')
  }
}

function printTicket() {
  window.print()
  return Promise.resolve(ticket.value)
}

onMounted(() => void initialize())
onBeforeUnmount(() => {
  dashboardController?.abort()
  slotController?.abort()
  dashboardSequence += 1
  slotSequence += 1
})
</script>

<template>
  <main class="register-page">
    <el-alert
      v-if="pendingRegisterPayload"
      class="page-error"
      type="warning"
      :closable="false"
      show-icon
      title="扫码扣款结果已保留，挂号结算尚未完成"
      :description="pendingRegisterMessage"
    >
      <el-button type="warning" link :loading="submitting" @click="recoverRegister">
        补结算
      </el-button>
    </el-alert>
    <el-alert
      v-if="errorMessage"
      class="page-error"
      :title="errorMessage"
      type="error"
      :closable="false"
      ><el-button type="primary" link @click="loadDashboard">重试</el-button></el-alert
    >
    <section class="register-column">
      <ActionToolbar
        title="门诊挂号"
        description="患者与号源确认后先进行费用试算"
        :actions="[
          { key: 'reset', label: '重置' },
          { key: 'submit', label: '费用试算', type: 'primary', loading: submitting }
        ]"
        :disabled="submitting || Boolean(pendingRegisterPayload)"
        @action="(action) => (action.key === 'reset' ? requestReset() : preSettle())"
      />
      <el-form label-width="86px"
        ><RegisterFormPanel
          :model-value="form"
          :init="init"
          :disabled="submitting || Boolean(pendingRegisterPayload)"
          :benefit-preview="benefitPreview"
          :benefit-loading="benefitLoading"
          :doctor-options="doctorOptions"
          :load-doctors="loadDoctors"
          :load-patients="loadPatients"
          @update:model-value="updateForm"
          @patient-change="patientChanged"
          @department-change="departmentChanged"
          @doctor-change="loadSlots"
          @refresh-slots="loadSlots"
          @appointment-load="loadAppointment"
          @benefit-load="readInsuranceBenefit"
          @contract-change="contractChanged"
      /></el-form>
    </section>
    <section class="dashboard-column">
      <DetailSummary title="今日挂号统计" :items="summaryItems" :loading="loading" />
      <el-tabs v-model="rightTab" class="right-tabs">
        <el-tab-pane label="当日号源" name="slots"
          ><DailySlotPanel
            :slots="slots"
            :selected-id="form.slotId"
            :loading="slotLoading"
            @select="selectSlot"
            @refresh="loadSlots"
        /></el-tab-pane>
        <el-tab-pane label="就诊记录" name="history"
          ><RegisterHistoryPanel
            :records="history"
            :total="historyTotal"
            :page="historyQuery.pageIndex"
            :page-size="historyQuery.pageSize"
            :loading="loading"
            @cancel="cancelRegister"
            @print="openTicket"
            @refresh="loadDashboard"
            @update:page="changeHistoryPage"
            @update:page-size="changeHistoryPageSize"
        /></el-tab-pane>
      </el-tabs>
    </section>
    <RegisterConfirmDialog
      v-model="confirmVisible"
      :init="init"
      :preview="preview"
      :submitting="submitting"
      @confirm="confirmRegister"
    />
    <el-dialog v-model="ticketVisible" title="挂号小票" width="720px">
      <DescriptionList :data="ticketDetail" :items="ticketItems" :column="2" title="票据信息" />
      <PrintActionPanel
        :template-value="'register-ticket'"
        :printer-value="'local-printer'"
        :templates="[{ value: 'register-ticket', label: '门诊挂号小票' }]"
        :printers="[{ value: 'local-printer', label: '本机默认打印机', status: 'online' }]"
        :preview="async () => ticket"
        :print="printTicket"
      >
        <template #header><strong>打印挂号小票</strong></template>
      </PrintActionPanel>
    </el-dialog>
  </main>
</template>

<style scoped>
.register-page {
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
  min-height: calc(100vh - 96px);
  grid-template-columns: minmax(420px, 0.9fr) minmax(520px, 1.1fr);
  gap: 14px;
  padding: 16px;
  background: var(--el-fill-color-lighter);
}
.page-error {
  grid-column: 1 / -1;
}
.register-column,
.dashboard-column {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 12px;
}
.register-column {
  padding: 14px;
  background: var(--el-bg-color);
}
.right-tabs {
  padding: 0 14px 14px;
  background: var(--el-bg-color);
}
:deep(.print-action-panel) {
  margin-top: 14px;
}
:deep(.el-select__wrapper.is-disabled .el-select__placeholder) {
  color: #6b7280;
}
@media (max-width: 1050px) {
  .register-page {
    grid-template-columns: 1fr;
  }
}
</style>
