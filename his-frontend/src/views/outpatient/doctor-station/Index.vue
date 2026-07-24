<script setup lang="ts">
// computed 创建派生状态，watch 负责在患者 ID 变化时加载上下文。
import { computed, onBeforeUnmount, onMounted, reactive, ref, watch } from 'vue'
import { Check, CircleCheck, Clock, Refresh, Select } from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'

// 复用项目公共组件，页面本身只负责组织数据和处理业务事件。
import {
  acceptPatient,
  completePatient,
  getAdviceOrders,
  getDiagnoses,
  getMedicalRecord,
  getMedicalRecordTemplate,
  getPatientContext,
  listMedicalRecordTemplates,
  leavePatient,
  pageAdviceOrderCatalog,
  pageDiagnosisDictionary,
  pageDoctorPatientQueue,
  saveDiagnoses,
  saveAdviceOrders,
  saveMedicalRecord,
  type DoctorAdviceOrderListVO,
  type DoctorDiagnosisListVO,
  type DoctorMedicalRecordTemplateNodeVO,
  type DoctorMedicalRecordVO,
  type DoctorPatientContextVO,
  type DoctorPatientQueueQuery
} from '@/apis/outpatient/doctor-station'

import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import type {
  ActionToolbarItem,
  CommonComponentKey,
  DateRangeValue
} from '@/components/common/type'
import { DoctorStationHeader } from '@/components/clinical'
import {
  ClinicalDiagnosisEditor,
  hasValidPrimaryDiagnosis,
  type ClinicalDiagnosisDictionaryLoader,
  type ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import {
  ClinicalOrderEditor,
  type ClinicalOrderCatalogLoader,
  type ClinicalOrderContext,
  type ClinicalOrderDraft
} from '@/components/clinical/order'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { PatientSummary } from '@/components/patient/type'

import {
  toClinicalDiagnosis,
  toClinicalDiagnosisCandidate,
  toPatientSummary,
  toSaveDoctorDiagnosisItem
} from './adapter'
import {
  outpatientAdviceAdapter,
  toClinicalOrder,
  toClinicalOrderCandidate
} from './advice/outpatientAdviceAdapter'
import DoctorStationAssistantPanel from './components/DoctorStationAssistantPanel.vue'
import DoctorQueueStatusFilter from './components/DoctorQueueStatusFilter.vue'
import MedicalRecordPanel from './components/MedicalRecordPanel.vue'
import { DOCTOR_STATION_TABS } from './constants'
import { saveDirtyStationSections } from './save-workflow'
import type {
  DoctorQueueStatusFilter as DoctorQueueStatusFilterValue,
  DoctorStationAction,
  DoctorStationTab
} from './type'

// 设置页面组件名，便于 Vue DevTools 中识别。
defineOptions({
  name: 'OutpatientDoctorStation'
})

// 当前打开的诊疗页签；默认进入门诊病历。
const activeTab = ref<DoctorStationTab>('record')

// 左侧患者队列由 API 数据经过 adapter.ts 转换后得到。
const patients = ref<PatientSummary[]>([])

// 只保存当前就诊 ID，避免同时维护“ID”和“患者对象”两份可能不一致的数据。
const selectedPatientId = ref<string | number | null>(null)

// 根据队列和选中 ID 自动计算当前患者；队列刷新后也会自动更新。
const selectedPatient = computed(
  () => patients.value.find((patient) => patient.id === selectedPatientId.value) ?? null
)

// 左右栏折叠状态由页面统一管理，再通过 v-model 传给布局组件。
const leftCollapsed = ref(false)
const rightCollapsed = ref(false)

// 患者队列的查询状态。公共组件只发出查询条件，不在组件内部过滤数据。
const queueQuery = reactive<DoctorPatientQueueQuery>({
  pageIndex: 1,
  pageSize: 50,
  keyword: undefined,
  startDate: undefined,
  endDate: undefined
})
const queueLoading = ref(false)
const selectedQueueStatus = ref<DoctorQueueStatusFilterValue>('ALL')

// 当前患者的详细就诊上下文与独立加载状态。
const patientContext = ref<DoctorPatientContextVO | null>(null)
const contextLoading = ref(false)
const contextError = ref('')
const accepting = ref(false)
const leaving = ref(false)
const completing = ref(false)

// 顶部按钮状态只依赖已加载的上下文，不根据中文显示文字判断。
const patientAccepted = computed(() => patientContext.value?.visitStatus === 'IN_PROGRESS')
const patientReadonly = computed(() => patientContext.value?.visitStatus === 'COMPLETED')

// 病历正文、最近一次服务端快照和保存状态由页面统一管理。
const medicalRecord = ref<DoctorMedicalRecordVO | null>(null)
const medicalRecordContent = ref('')
const savedMedicalRecordContent = ref('')
const medicalRecordLoading = ref(false)
const medicalRecordSaving = ref(false)
const medicalRecordError = ref('')
const medicalRecordDirty = computed(
  () => medicalRecordContent.value !== savedMedicalRecordContent.value
)

// 右侧模板树全页面只加载一次；点击模板时再按需加载正文。
const medicalRecordTemplates = ref<DoctorMedicalRecordTemplateNodeVO[]>([])
const templatesLoading = ref(false)
const templatesError = ref('')
const applyingTemplate = ref(false)

// 公共诊断模型与门诊接口版本分开保存，便于未来住院医生站复用同一个编辑器。
const diagnosisList = ref<DoctorDiagnosisListVO | null>(null)
const diagnoses = ref<ClinicalDiagnosisItem[]>([])
// 空诊断列表的快照必须是 "[]"，不能使用空字符串，否则首次进入页面会被误判为已修改。
const savedDiagnosisSnapshot = ref('[]')
const diagnosesLoading = ref(false)
const diagnosesSaving = ref(false)
const diagnosesError = ref('')

/**
 * 生成只包含业务字段的稳定快照。
 * key 只是前端行标识，不参与“是否修改”的判断。
 */
function serializeDiagnoses(items: ClinicalDiagnosisItem[]) {
  return JSON.stringify(
    items.map((item) => ({
      recordId: item.recordId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      typeName: item.typeName,
      primary: item.primary
    }))
  )
}

const diagnosesDirty = computed(
  () =>
    diagnosisList.value !== null &&
    !diagnosesLoading.value &&
    serializeDiagnoses(diagnoses.value) !== savedDiagnosisSnapshot.value
)

// 公共医嘱草稿与门诊接口版本分离，保存时由 outpatientAdviceAdapter 转换 DTO。
const adviceOrderList = ref<DoctorAdviceOrderListVO | null>(null)
const adviceOrders = ref<ClinicalOrderDraft[]>([])
const savedAdviceOrderSnapshot = ref('[]')
const adviceOrdersLoading = ref(false)
const adviceOrdersSaving = ref(false)
const adviceOrdersError = ref('')

/** 医嘱适配器使用的通用诊疗上下文。 */
const adviceOrderContext = computed<ClinicalOrderContext | null>(() => {
  if (!patientContext.value || selectedPatientId.value === null) return null
  return {
    scope: 'outpatient',
    patientId: patientContext.value.patientId,
    encounterKey: String(selectedPatientId.value),
    departmentName: selectedPatient.value?.departmentName ?? undefined
  }
})

/** 生成不包含前端 key 的医嘱业务快照。 */
function serializeAdviceOrders(items: ClinicalOrderDraft[]) {
  return JSON.stringify(
    items.map((item) => ({
      recordId: item.recordId,
      catalogId: item.catalogId,
      code: item.code,
      name: item.name,
      category: item.category,
      specification: item.specification,
      dose: item.dose,
      unit: item.unit,
      frequency: item.frequency,
      route: item.route,
      days: item.days,
      quantity: item.quantity,
      quantityUnit: item.quantityUnit,
      remark: item.remark
    }))
  )
}

const ordersDirty = computed(
  () =>
    adviceOrderList.value !== null &&
    !adviceOrdersLoading.value &&
    serializeAdviceOrders(adviceOrders.value) !== savedAdviceOrderSnapshot.value
)

/** 汇总当前患者所有未保存的诊疗模块名称。 */
const unsavedSectionNames = computed(() =>
  [
    medicalRecordDirty.value ? '病历' : '',
    diagnosesDirty.value ? '诊断' : '',
    ordersDirty.value ? '医嘱' : ''
  ].filter(Boolean)
)

// 任意模块正在保存时，都不允许同时暂离、完诊或刷新。
const stationSaving = computed(
  () => medicalRecordSaving.value || diagnosesSaving.value || adviceOrdersSaving.value
)
const stationDataLoading = computed(
  () =>
    contextLoading.value ||
    medicalRecordLoading.value ||
    diagnosesLoading.value ||
    adviceOrdersLoading.value
)
const patientTransitioning = computed(() => accepting.value || leaving.value || completing.value)

/**
 * 门诊医生站自己的顶部按钮配置。
 *
 * 公共 Header 不知道“接诊”和“完诊”的业务规则；页面先根据患者状态
 * 算出 disabled、loading 等属性，再把结果交给公共组件展示。
 */
const stationHeaderActions = computed<ActionToolbarItem[]>(() => {
  const patientSelected = Boolean(selectedPatient.value && patientContext.value)

  return [
    {
      key: 'accept',
      label: '接诊',
      type: 'primary',
      icon: Select,
      disabled:
        !patientSelected ||
        patientAccepted.value ||
        patientReadonly.value ||
        leaving.value ||
        completing.value,
      loading: accepting.value
    },
    {
      key: 'save',
      label: '保存',
      type: 'success',
      icon: Check,
      disabled:
        !patientSelected ||
        !patientAccepted.value ||
        patientReadonly.value ||
        patientTransitioning.value ||
        stationDataLoading.value,
      loading: stationSaving.value
    },
    {
      key: 'refresh',
      label: '刷新',
      icon: Refresh,
      disabled:
        !patientSelected ||
        patientTransitioning.value ||
        stationSaving.value ||
        stationDataLoading.value
    },
    {
      key: 'leave',
      label: '暂离',
      icon: Clock,
      disabled:
        !patientSelected ||
        !patientAccepted.value ||
        patientReadonly.value ||
        patientTransitioning.value ||
        stationSaving.value ||
        stationDataLoading.value,
      loading: leaving.value
    },
    {
      key: 'complete',
      label: '完诊',
      type: 'warning',
      icon: CircleCheck,
      disabled:
        !patientSelected ||
        !patientAccepted.value ||
        patientReadonly.value ||
        patientTransitioning.value ||
        stationSaving.value ||
        stationDataLoading.value,
      loading: completing.value
    }
  ]
})

// 每次请求递增序号，只允许最后一次请求更新页面，避免旧响应覆盖新结果。
let latestQueueRequestId = 0
let latestContextRequestId = 0
let latestMedicalRecordRequestId = 0
let latestTemplateRequestId = 0
let latestDiagnosesRequestId = 0
let latestAdviceOrdersRequestId = 0

/**
 * 从 API 加载患者队列。
 *
 * 请求成功后做 DTO 转换；如果刷新后当前患者已不在列表中，
 * 同时清除旧选择，防止页面继续显示过期患者。
 */
async function loadPatients() {
  const requestId = ++latestQueueRequestId
  queueLoading.value = true

  try {
    const result = await pageDoctorPatientQueue({ ...queueQuery })

    // 用户可能已经发起了更新的查询，旧请求返回时直接丢弃。
    if (requestId !== latestQueueRequestId) return

    patients.value = (result.rows ?? []).map(toPatientSummary)

    if (!patients.value.some((patient) => patient.id === selectedPatientId.value)) {
      selectedPatientId.value = null
    }
  } catch (error) {
    if (requestId !== latestQueueRequestId) return

    patients.value = []
    selectedPatientId.value = null
    const message = error instanceof Error ? error.message : '患者队列加载失败'
    ElMessage.error(`${message}，请点击刷新重试`)
  } finally {
    // 旧请求不能把新请求仍在使用的 loading 提前关闭。
    if (requestId === latestQueueRequestId) {
      queueLoading.value = false
    }
  }
}

/**
 * 加载当前患者的详细上下文。
 *
 * 和队列查询一样使用请求序号：快速点击多个患者时，
 * 较早患者的慢响应不会覆盖最后选择的患者。
 */
async function loadPatientContext(encounterId: string | number) {
  const requestId = ++latestContextRequestId
  contextLoading.value = true
  contextError.value = ''
  patientContext.value = null

  try {
    const result = await getPatientContext(String(encounterId))
    if (requestId !== latestContextRequestId) return
    patientContext.value = result
  } catch (error) {
    if (requestId !== latestContextRequestId) return
    contextError.value = error instanceof Error ? error.message : '患者就诊信息加载失败'
  } finally {
    if (requestId === latestContextRequestId) {
      contextLoading.value = false
    }
  }
}

/**
 * 加载选中患者的门诊病历。
 *
 * 正文和“最近保存快照”同时赋值，因此刚加载完成时 dirty 为 false；
 * 医生编辑后只有正文变化，dirty 才会自动变为 true。
 */
async function loadCurrentMedicalRecord(encounterId: string | number) {
  const requestId = ++latestMedicalRecordRequestId
  medicalRecordLoading.value = true
  medicalRecordError.value = ''
  medicalRecord.value = null
  medicalRecordContent.value = ''
  savedMedicalRecordContent.value = ''

  try {
    const result = await getMedicalRecord(String(encounterId))
    if (requestId !== latestMedicalRecordRequestId) return

    medicalRecord.value = result
    medicalRecordContent.value = result.content
    savedMedicalRecordContent.value = result.content
  } catch (error) {
    if (requestId !== latestMedicalRecordRequestId) return
    medicalRecordError.value = error instanceof Error ? error.message : '门诊病历加载失败'
  } finally {
    if (requestId === latestMedicalRecordRequestId) {
      medicalRecordLoading.value = false
    }
  }
}

/**
 * 加载当前患者诊断，并通过 adapter 转换为公共组件模型。
 */
async function loadCurrentDiagnoses(encounterId: string | number) {
  const requestId = ++latestDiagnosesRequestId
  diagnosesLoading.value = true
  diagnosesError.value = ''
  diagnosisList.value = null
  diagnoses.value = []
  savedDiagnosisSnapshot.value = serializeDiagnoses([])

  try {
    const result = await getDiagnoses(String(encounterId))
    if (requestId !== latestDiagnosesRequestId) return

    const clinicalRows = result.rows
      .slice()
      .sort((left, right) => left.orderNo - right.orderNo)
      .map(toClinicalDiagnosis)
    diagnosisList.value = result
    diagnoses.value = clinicalRows
    savedDiagnosisSnapshot.value = serializeDiagnoses(clinicalRows)
  } catch (error) {
    if (requestId !== latestDiagnosesRequestId) return
    diagnosesError.value = error instanceof Error ? error.message : '门诊诊断加载失败'
  } finally {
    if (requestId === latestDiagnosesRequestId) {
      diagnosesLoading.value = false
    }
  }
}

/**
 * 公共选择弹窗注入的诊断字典加载器。
 *
 * 这里把 SelectTable 的查询参数翻译成门诊 API 查询参数，再将返回 VO 转成公共候选项。
 * 住院医生站以后可以注入自己的同类型函数。
 */
const loadDiagnosisDictionary: ClinicalDiagnosisDictionaryLoader = async (query, context) => {
  if (context.signal.aborted) return { rows: [], total: 0 }

  const result = await pageDiagnosisDictionary({
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    keyword: query.keyword || undefined
  })

  if (context.signal.aborted) return { rows: [], total: 0 }
  return {
    rows: (result.rows ?? []).map(toClinicalDiagnosisCandidate),
    total: result.total
  }
}

/** 加载当前患者门诊医嘱，并转换为公共草稿模型。 */
async function loadCurrentAdviceOrders(encounterId: string | number) {
  const requestId = ++latestAdviceOrdersRequestId
  adviceOrdersLoading.value = true
  adviceOrdersError.value = ''
  adviceOrderList.value = null
  adviceOrders.value = []
  savedAdviceOrderSnapshot.value = serializeAdviceOrders([])

  try {
    const result = await getAdviceOrders(String(encounterId))
    if (requestId !== latestAdviceOrdersRequestId) return

    const clinicalRows = result.rows
      .slice()
      .sort((left, right) => left.orderNo - right.orderNo)
      .map(toClinicalOrder)
    adviceOrderList.value = result
    adviceOrders.value = clinicalRows
    savedAdviceOrderSnapshot.value = serializeAdviceOrders(clinicalRows)
  } catch (error) {
    if (requestId !== latestAdviceOrdersRequestId) return
    adviceOrdersError.value = error instanceof Error ? error.message : '门诊医嘱加载失败'
  } finally {
    if (requestId === latestAdviceOrdersRequestId) {
      adviceOrdersLoading.value = false
    }
  }
}

/** 公共医嘱项目选择弹窗使用的门诊目录加载器。 */
const loadAdviceOrderCatalog: ClinicalOrderCatalogLoader = async (query, context) => {
  if (context.signal.aborted) return { rows: [], total: 0 }

  const result = await pageAdviceOrderCatalog({
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    keyword: query.keyword || undefined
  })

  if (context.signal.aborted) return { rows: [], total: 0 }
  return {
    rows: (result.rows ?? []).map(toClinicalOrderCandidate),
    total: result.total
  }
}

/** 加载右侧病历模板树。 */
async function loadMedicalRecordTemplates() {
  templatesLoading.value = true
  templatesError.value = ''

  try {
    medicalRecordTemplates.value = await listMedicalRecordTemplates()
  } catch (error) {
    medicalRecordTemplates.value = []
    templatesError.value = error instanceof Error ? error.message : '病历模板加载失败'
  } finally {
    templatesLoading.value = false
  }
}

/**
 * PatientQueuePanel 重置时会连续发出 search 和 date-change。
 * 使用微任务把同一轮中的多次修改合并成一次请求。
 */
let queueReloadScheduled = false
function scheduleLoadPatients() {
  if (queueReloadScheduled) return

  queueReloadScheduled = true
  queueMicrotask(() => {
    queueReloadScheduled = false
    void loadPatients()
  })
}

function searchPatients(keyword: string) {
  queueQuery.keyword = keyword || undefined
  queueQuery.pageIndex = 1
  scheduleLoadPatients()
}

function changeQueueDate(dateRange: DateRangeValue) {
  queueQuery.startDate = dateRange?.[0]
  queueQuery.endDate = dateRange?.[1]
  queueQuery.pageIndex = 1
  scheduleLoadPatients()
}

function changeQueueStatus(status: DoctorQueueStatusFilterValue) {
  queueQuery.visitStatus = status === 'ALL' ? undefined : status
  queueQuery.pageIndex = 1
  scheduleLoadPatients()
}

let patientSwitchPrompting = false
let pendingPatientToSelect: PatientSummary | null = null

/**
 * 选择其他患者前检查未保存数据。
 *
 * PatientQueuePanel 的“点击卡片”和“查看”都会进入这个统一入口，
 * 因此不会出现某一种切换方式绕过提醒的情况。
 */
async function selectPatient(patient: PatientSummary) {
  if (patient.id === selectedPatientId.value) return

  pendingPatientToSelect = patient
  if (patientSwitchPrompting) return

  // 刷新后尚未选择患者时不存在“当前患者”，必须直接允许首次选择。
  if (selectedPatientId.value === null || unsavedSectionNames.value.length === 0) {
    selectedPatientId.value = patient.id
    pendingPatientToSelect = null
    return
  }

  patientSwitchPrompting = true
  try {
    await ElMessageBox.confirm(
      '当前患者的' +
        unsavedSectionNames.value.join('、') +
        '尚未保存。建议先返回保存；如果继续切换，这些修改将会丢失。',
      '存在未保存的诊疗数据',
      {
        type: 'warning',
        confirmButtonText: '放弃修改并切换',
        cancelButtonText: '返回保存',
        distinguishCancelAndClose: true
      }
    )

    // 弹窗打开期间如果又点击了其他患者，以最后一次点击的患者为准。
    if (pendingPatientToSelect) {
      selectedPatientId.value = pendingPatientToSelect.id
    }
  } catch {
    // 点击“返回保存”、关闭弹窗或按 Esc 时都保留当前患者和编辑内容。
  } finally {
    patientSwitchPrompting = false
    pendingPatientToSelect = null
  }
}

/**
 * 执行接诊并刷新患者队列。
 *
 * 如果医生正在查看“候诊”，接诊成功后自动切换到“在诊”，
 * 这样患者不会因为离开候诊列表而从当前工作区消失。
 */
async function handleAcceptPatient() {
  const encounterId = selectedPatientId.value
  if (!encounterId || !patientContext.value || accepting.value) return

  accepting.value = true
  try {
    patientContext.value = await acceptPatient(String(encounterId))
    ElMessage.success('接诊成功')

    if (selectedQueueStatus.value === 'WAITING') {
      selectedQueueStatus.value = 'IN_PROGRESS'
      queueQuery.visitStatus = 'IN_PROGRESS'
    }

    await loadPatients()
  } catch (error) {
    const message = error instanceof Error ? error.message : '接诊失败'
    ElMessage.error(message)
  } finally {
    accepting.value = false
  }
}

/** 把富文本转换为可判断的纯文字，避免把只有空标签的病历误认为有效内容。 */
function hasMeaningfulMedicalRecordContent(content: string) {
  return Boolean(
    content
      .replace(/<[^>]*>/g, '')
      .replace(/&nbsp;|&#160;/gi, '')
      .trim()
  )
}

/** 根据第一个未保存模块跳转页签，让医生可以立即找到需要处理的内容。 */
function focusFirstUnsavedSection() {
  if (medicalRecordDirty.value) activeTab.value = 'record'
  else if (diagnosesDirty.value) activeTab.value = 'diagnosis'
  else if (ordersDirty.value) activeTab.value = 'orders'
}

/**
 * 暂离当前患者。
 *
 * 暂离会把患者退回候诊队列。如果存在草稿，医生必须明确选择是否放弃；
 * 暂离成功后重新加载三个诊疗模块，确保页面不会保留已放弃的内容。
 */
async function handleLeavePatient() {
  const encounterId = selectedPatientId.value
  if (!encounterId || !patientAccepted.value || patientTransitioning.value || stationSaving.value) {
    return
  }

  const encounterIdText = String(encounterId)
  leaving.value = true

  try {
    if (unsavedSectionNames.value.length > 0) {
      try {
        await ElMessageBox.confirm(
          '当前患者的' +
            unsavedSectionNames.value.join('、') +
            '尚未保存。暂离会放弃这些修改，是否继续？',
          '存在未保存的诊疗数据',
          {
            type: 'warning',
            confirmButtonText: '放弃修改并暂离',
            cancelButtonText: '返回保存'
          }
        )
      } catch {
        return
      }
    }

    const result = await leavePatient(encounterIdText)
    if (String(selectedPatientId.value) !== encounterIdText) return

    patientContext.value = result
    if (selectedQueueStatus.value === 'IN_PROGRESS') {
      selectedQueueStatus.value = 'WAITING'
      queueQuery.visitStatus = 'WAITING'
    }

    await loadPatients()
    if (String(selectedPatientId.value) === encounterIdText) {
      await Promise.all([
        loadCurrentMedicalRecord(encounterIdText),
        loadCurrentDiagnoses(encounterIdText),
        loadCurrentAdviceOrders(encounterIdText)
      ])
    }
    ElMessage.success('患者已暂离并返回候诊队列')
  } catch (error) {
    const message = error instanceof Error ? error.message : '暂离失败'
    ElMessage.error(message)
  } finally {
    leaving.value = false
  }
}

/**
 * 完成本次门诊就诊。
 *
 * 完诊是不可继续编辑的状态变更，因此不允许像切换患者那样直接丢弃草稿；
 * 医生必须先保存病历、诊断和医嘱，再执行最终确认。
 */
async function handleCompletePatient() {
  const encounterId = selectedPatientId.value
  if (!encounterId || !patientAccepted.value || patientTransitioning.value || stationSaving.value) {
    return
  }

  completing.value = true
  try {
    if (unsavedSectionNames.value.length > 0) {
      focusFirstUnsavedSection()
      ElMessage.warning(
        '请先保存当前患者的' + unsavedSectionNames.value.join('、') + '，再执行完诊'
      )
      return
    }

    if (
      !medicalRecord.value ||
      medicalRecord.value.version <= 0 ||
      !hasMeaningfulMedicalRecordContent(savedMedicalRecordContent.value)
    ) {
      activeTab.value = 'record'
      ElMessage.warning('请先填写并保存门诊病历')
      return
    }

    if (!diagnosisList.value || !hasValidPrimaryDiagnosis(diagnoses.value)) {
      activeTab.value = 'diagnosis'
      ElMessage.warning('请先保存诊断，并且只能设置一个主诊断')
      return
    }

    try {
      await ElMessageBox.confirm(
        '完诊后病历、诊断和医嘱将转为只读，确认完成当前患者的本次就诊吗？',
        '确认完诊',
        {
          type: 'warning',
          confirmButtonText: '确认完诊',
          cancelButtonText: '取消'
        }
      )
    } catch {
      return
    }

    const encounterIdText = String(encounterId)
    const result = await completePatient(encounterIdText)
    if (String(selectedPatientId.value) !== encounterIdText) return

    patientContext.value = result
    if (selectedQueueStatus.value === 'IN_PROGRESS') {
      selectedQueueStatus.value = 'COMPLETED'
      queueQuery.visitStatus = 'COMPLETED'
    }

    await loadPatients()
    ElMessage.success('患者已完诊，诊疗内容已转为只读')
  } catch (error) {
    const message = error instanceof Error ? error.message : '完诊失败'
    ElMessage.error(message)
  } finally {
    completing.value = false
  }
}

/**
 * 保存当前病历。
 *
 * version 来自最后一次读取或保存结果，Mock 会像真实后端一样校验版本，
 * 防止当前页面覆盖其他窗口已经更新的病历。
 */
async function handleSaveMedicalRecord(notifySuccess = true): Promise<boolean> {
  const encounterId = selectedPatientId.value
  if (!encounterId || !patientAccepted.value || !medicalRecord.value || medicalRecordSaving.value) {
    return false
  }

  const encounterIdText = String(encounterId)
  const contentToSave = medicalRecordContent.value
  medicalRecordSaving.value = true

  try {
    const result = await saveMedicalRecord(encounterIdText, {
      content: contentToSave,
      version: medicalRecord.value.version
    })

    // 保存期间如果用户切换了患者，不把旧患者的响应写入新患者页面。
    if (String(selectedPatientId.value) !== encounterIdText) return false

    medicalRecord.value = result
    savedMedicalRecordContent.value = result.content
    if (notifySuccess) ElMessage.success('门诊病历保存成功')
    return true
  } catch (error) {
    const message = error instanceof Error ? error.message : '门诊病历保存失败'
    ElMessage.error(message)
    return false
  } finally {
    medicalRecordSaving.value = false
  }
}

/**
 * 保存当前诊断列表。
 *
 * 公共组件负责编辑交互，页面负责门诊规则校验、DTO 转换和接口调用。
 */
async function handleSaveDiagnoses(notifySuccess = true): Promise<boolean> {
  const encounterId = selectedPatientId.value
  if (!encounterId || !patientAccepted.value || !diagnosisList.value || diagnosesSaving.value) {
    return false
  }

  if (!hasValidPrimaryDiagnosis(diagnoses.value)) {
    ElMessage.warning('请至少添加一条诊断，并且只能设置一个主诊断')
    return false
  }

  const encounterIdText = String(encounterId)
  diagnosesSaving.value = true

  try {
    const result = await saveDiagnoses(encounterIdText, {
      version: diagnosisList.value.version,
      items: diagnoses.value.map(toSaveDoctorDiagnosisItem)
    })

    // 保存期间切换了患者时，丢弃旧患者的响应。
    if (String(selectedPatientId.value) !== encounterIdText) return false

    const clinicalRows = result.rows
      .slice()
      .sort((left, right) => left.orderNo - right.orderNo)
      .map(toClinicalDiagnosis)
    diagnosisList.value = result
    diagnoses.value = clinicalRows
    savedDiagnosisSnapshot.value = serializeDiagnoses(clinicalRows)
    if (notifySuccess) ElMessage.success('门诊诊断保存成功')
    return true
  } catch (error) {
    const message = error instanceof Error ? error.message : '门诊诊断保存失败'
    ElMessage.error(message)
    return false
  } finally {
    diagnosesSaving.value = false
  }
}

/** 保存当前门诊医嘱，校验和 DTO 映射均委托给门诊场景适配器。 */
async function handleSaveAdviceOrders(notifySuccess = true): Promise<boolean> {
  const encounterId = selectedPatientId.value
  const context = adviceOrderContext.value
  if (
    !encounterId ||
    !context ||
    !patientAccepted.value ||
    !adviceOrderList.value ||
    adviceOrdersSaving.value
  ) {
    return false
  }

  const validation = outpatientAdviceAdapter.validate(adviceOrders.value, context)
  if (!validation.valid) {
    ElMessage.warning(validation.errors[0]?.message ?? '请完善医嘱内容')
    return false
  }

  const encounterIdText = String(encounterId)
  const request = outpatientAdviceAdapter.mapSaveRequest(
    adviceOrders.value,
    context,
    adviceOrderList.value.version
  )
  adviceOrdersSaving.value = true

  try {
    const result = await saveAdviceOrders(encounterIdText, request)
    if (String(selectedPatientId.value) !== encounterIdText) return false

    const clinicalRows = result.rows
      .slice()
      .sort((left, right) => left.orderNo - right.orderNo)
      .map(toClinicalOrder)
    adviceOrderList.value = result
    adviceOrders.value = clinicalRows
    savedAdviceOrderSnapshot.value = serializeAdviceOrders(clinicalRows)
    if (notifySuccess) ElMessage.success('门诊医嘱保存成功')
    return true
  } catch (error) {
    const message = error instanceof Error ? error.message : '门诊医嘱保存失败'
    ElMessage.error(message)
    return false
  } finally {
    adviceOrdersSaving.value = false
  }
}

/**
 * 一键保存当前患者的全部诊疗修改。
 *
 * 先完成所有前端校验，再并行调用病历、诊断和医嘱保存函数。这样可以避免
 * 病历已经保存后，才发现诊断或医嘱内容不完整的常见操作困扰。
 */
async function handleSaveStation() {
  if (
    !selectedPatientId.value ||
    !patientAccepted.value ||
    patientTransitioning.value ||
    stationSaving.value ||
    stationDataLoading.value
  ) {
    return
  }

  if (unsavedSectionNames.value.length === 0) {
    ElMessage.info('当前病历、诊断和医嘱均已保存')
    return
  }

  // 统一校验必须发生在任何保存请求之前，尽量避免出现部分模块已保存的情况。
  if (diagnosesDirty.value && !hasValidPrimaryDiagnosis(diagnoses.value)) {
    activeTab.value = 'diagnosis'
    ElMessage.warning('请至少添加一条诊断，并且只能设置一个主诊断')
    return
  }

  if (ordersDirty.value) {
    const context = adviceOrderContext.value
    if (!context) {
      activeTab.value = 'orders'
      ElMessage.warning('患者医嘱上下文尚未加载完成，请稍后重试')
      return
    }

    const validation = outpatientAdviceAdapter.validate(adviceOrders.value, context)
    if (!validation.valid) {
      activeTab.value = 'orders'
      ElMessage.warning(validation.errors[0]?.message ?? '请完善医嘱内容')
      return
    }
  }

  const result = await saveDirtyStationSections([
    {
      name: '病历',
      dirty: medicalRecordDirty.value,
      save: () => handleSaveMedicalRecord(false)
    },
    {
      name: '诊断',
      dirty: diagnosesDirty.value,
      save: () => handleSaveDiagnoses(false)
    },
    {
      name: '医嘱',
      dirty: ordersDirty.value,
      save: () => handleSaveAdviceOrders(false)
    }
  ])

  if (result.failed.length === 0) {
    ElMessage.success(result.succeeded.join('、') + '保存成功')
    return
  }

  if (result.succeeded.length > 0) {
    ElMessage.warning(
      result.succeeded.join('、') +
        '已保存；' +
        result.failed.join('、') +
        '保存失败，请检查提示后重试'
    )
  }
}

/**
 * 有未保存内容时询问用户是否继续覆盖。
 *
 * 第四阶段只保护“刷新”和“套用模板”；切换患者、关闭页面等完整保护会在第七阶段统一实现。
 */
async function confirmDiscardMedicalRecord(message: string) {
  if (!medicalRecordDirty.value) return true

  try {
    await ElMessageBox.confirm(message, '存在未保存修改', {
      type: 'warning',
      confirmButtonText: '继续',
      cancelButtonText: '取消'
    })
    return true
  } catch {
    return false
  }
}

/** 刷新整个患者工作区前，同时检查病历、诊断和医嘱未保存内容。 */
async function confirmDiscardStationChanges() {
  const dirtySections = [
    medicalRecordDirty.value ? '病历' : '',
    diagnosesDirty.value ? '诊断' : '',
    ordersDirty.value ? '医嘱' : ''
  ].filter(Boolean)
  if (dirtySections.length === 0) return true

  try {
    await ElMessageBox.confirm(
      '刷新会丢弃当前未保存的' + dirtySections.join('、') + '修改，是否继续？',
      '存在未保存修改',
      {
        type: 'warning',
        confirmButtonText: '继续',
        cancelButtonText: '取消'
      }
    )
    return true
  } catch {
    return false
  }
}

/** 将右侧选中的模板正文放入编辑器，但不自动保存。 */
async function applyMedicalRecordTemplate(template: DoctorMedicalRecordTemplateNodeVO) {
  if (!patientAccepted.value || applyingTemplate.value) return

  const canContinue = await confirmDiscardMedicalRecord(
    `套用“${template.name}”会覆盖当前未保存的病历内容，是否继续？`
  )
  if (!canContinue) return

  const requestId = ++latestTemplateRequestId
  applyingTemplate.value = true

  try {
    const result = await getMedicalRecordTemplate(template.id)
    if (requestId !== latestTemplateRequestId) return

    medicalRecordContent.value = result.content
    activeTab.value = 'record'
    ElMessage.success(`已套用“${result.name}”，请检查后保存`)
  } catch (error) {
    if (requestId !== latestTemplateRequestId) return
    const message = error instanceof Error ? error.message : '病历模板加载失败'
    ElMessage.error(message)
  } finally {
    if (requestId === latestTemplateRequestId) {
      applyingTemplate.value = false
    }
  }
}

async function refreshCurrentPatient() {
  const canContinue = await confirmDiscardStationChanges()
  if (!canContinue) return

  const encounterId = selectedPatientId.value
  await loadPatients()
  if (encounterId && selectedPatientId.value === encounterId) {
    await Promise.all([
      loadPatientContext(encounterId),
      loadCurrentMedicalRecord(encounterId),
      loadCurrentDiagnoses(encounterId),
      loadCurrentAdviceOrders(encounterId)
    ])
  }
}

// 子组件切换页签后，父页面更新唯一的数据源 activeTab。
function handleTabChange(tab: DoctorStationTab) {
  activeTab.value = tab
}

/**
 * 公共 Header 使用 string | number 作为通用 key。
 * 门诊页面在边界处把它收窄成自己的 DoctorStationTab 类型。
 */
function handleHeaderTabChange(tab: CommonComponentKey) {
  handleTabChange(tab as DoctorStationTab)
}

/** 从公共按钮对象中取出门诊动作 key，再进入原有业务处理函数。 */
function handleHeaderAction(action: ActionToolbarItem) {
  handleStationAction(action.key as DoctorStationAction)
}

/**
 * 医生站所有顶部动作的统一入口。
 * 公共 Header 只发送动作 key，具体门诊流程全部在当前页面处理。
 */
function handleStationAction(action: DoctorStationAction) {
  if (action === 'accept') {
    void handleAcceptPatient()
    return
  }

  if (action === 'save') {
    void handleSaveStation()
    return
  }

  if (action === 'refresh') {
    void refreshCurrentPatient()
    return
  }

  if (action === 'leave') {
    void handleLeavePatient()
    return
  }

  if (action === 'complete') {
    void handleCompletePatient()
  }
}

/**
 * 浏览器刷新或关闭页面时保护未保存内容。
 *
 * 浏览器出于安全原因只显示系统统一提示，不能自定义这段提示的文字；
 * 没有实际修改时不会触发，因此正常刷新页面不会出现误报。
 */
function handleBeforeUnload(event: BeforeUnloadEvent) {
  if (unsavedSectionNames.value.length === 0) return
  event.preventDefault()
  event.returnValue = ''
}

// 选中患者时并行加载上下文、病历、诊断和医嘱；清空选择时同步清除旧数据。
watch(selectedPatientId, (encounterId) => {
  if (encounterId === null) {
    latestContextRequestId++
    latestMedicalRecordRequestId++
    latestDiagnosesRequestId++
    latestAdviceOrdersRequestId++
    patientContext.value = null
    contextError.value = ''
    contextLoading.value = false
    medicalRecord.value = null
    medicalRecordContent.value = ''
    savedMedicalRecordContent.value = ''
    medicalRecordError.value = ''
    medicalRecordLoading.value = false
    diagnosisList.value = null
    diagnoses.value = []
    savedDiagnosisSnapshot.value = serializeDiagnoses([])
    diagnosesError.value = ''
    diagnosesLoading.value = false
    adviceOrderList.value = null
    adviceOrders.value = []
    savedAdviceOrderSnapshot.value = serializeAdviceOrders([])
    adviceOrdersError.value = ''
    adviceOrdersLoading.value = false
    return
  }

  void loadPatientContext(encounterId)
  void loadCurrentMedicalRecord(encounterId)
  void loadCurrentDiagnoses(encounterId)
  void loadCurrentAdviceOrders(encounterId)
})

// 页面第一次进入时自动加载患者和模板，不需要用户先点击“查询”。
onMounted(() => {
  window.addEventListener('beforeunload', handleBeforeUnload)
  void loadPatients()
  void loadMedicalRecordTemplates()
})

// 页面组件销毁时移除全局监听，避免重新进入医生站后重复绑定。
onBeforeUnmount(() => {
  window.removeEventListener('beforeunload', handleBeforeUnload)
})
</script>

<template>
  <!-- 页面外壳必须有明确高度，三栏组件才能正确计算独立滚动区域。 -->
  <div class="doctor-station-page">
    <!--
      v-model:left-collapsed 是双向绑定的简写：
      布局组件发出折叠事件时，leftCollapsed 会自动更新。
    -->
    <StationTriPaneLayout
      v-model:left-collapsed="leftCollapsed"
      v-model:right-collapsed="rightCollapsed"
      left-width="320px"
      right-width="300px"
      :gap="8"
    >
      <!-- 左侧插槽：患者查询和患者队列。 -->
      <template #left>
        <section class="doctor-station-page__queue">
          <DoctorQueueStatusFilter
            v-model="selectedQueueStatus"
            :loading="queueLoading"
            @change="changeQueueStatus"
          />
          <PatientQueuePanel
            :patients="patients"
            :loading="queueLoading"
            :selected-id="selectedPatientId"
            @search="searchPatients"
            @date-change="changeQueueDate"
            @refresh="loadPatients"
            @select="selectPatient"
            @view="selectPatient"
          />
        </section>
      </template>

      <!-- 中间插槽：医生最主要的诊疗工作区。 -->
      <template #main>
        <section class="doctor-station-page__main">
          <!-- patient 为 null 时，公共组件会自动显示“未选择患者”。 -->
          <PatientInfoBar :patient="selectedPatient" />

          <!--
            公共 Header 只渲染配置；门诊页签和按钮规则都由当前页面提供。
            将来住院医生站可以复用相同外壳，并传入自己的配置。
          -->
          <DoctorStationHeader
            :active-tab="activeTab"
            :tabs="DOCTOR_STATION_TABS"
            :actions="stationHeaderActions"
            @action="handleHeaderAction"
            @tab-change="handleHeaderTabChange"
          />

          <!--
            使用 v-show 保留病历编辑器实例：
            医生切到诊断或医嘱再返回时，尚未保存的病历内容不会因为组件销毁而丢失。
          -->
          <div class="doctor-station-page__workspace">
            <MedicalRecordPanel
              v-show="activeTab === 'record'"
              v-model="medicalRecordContent"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :loading="medicalRecordLoading"
              :error="medicalRecordError"
              :dirty="medicalRecordDirty"
              :updated-at="medicalRecord?.updatedAt"
              @retry="selectedPatientId !== null && loadCurrentMedicalRecord(selectedPatientId)"
            />

            <ClinicalDiagnosisEditor
              v-show="activeTab === 'diagnosis'"
              v-model="diagnoses"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :loading="diagnosesLoading"
              :error="diagnosesError"
              :dirty="diagnosesDirty"
              :updated-at="diagnosisList?.updatedAt"
              :load-dictionary="loadDiagnosisDictionary"
              @retry="selectedPatientId !== null && loadCurrentDiagnoses(selectedPatientId)"
              @dictionary-error="ElMessage.error('诊断字典加载失败，请重试')"
            />
            <ClinicalOrderEditor
              v-show="activeTab === 'orders'"
              v-model="adviceOrders"
              :context="adviceOrderContext"
              :adapter="outpatientAdviceAdapter"
              :load-catalog="loadAdviceOrderCatalog"
              :patient-selected="Boolean(selectedPatient)"
              :accepted="patientAccepted"
              :readonly="patientReadonly"
              :loading="adviceOrdersLoading"
              :error="adviceOrdersError"
              :dirty="ordersDirty"
              :updated-at="adviceOrderList?.updatedAt"
              @retry="selectedPatientId !== null && loadCurrentAdviceOrders(selectedPatientId)"
              @catalog-error="ElMessage.error('医嘱项目加载失败，请重试')"
            />
          </div>
        </section>
      </template>

      <!-- 右侧插槽：病历模板和就诊信息通过页签共享右栏。 -->
      <template #right>
        <DoctorStationAssistantPanel
          :patient-selected="Boolean(selectedPatient)"
          :context="patientContext"
          :context-loading="contextLoading"
          :context-error="contextError"
          :templates="medicalRecordTemplates"
          :templates-loading="templatesLoading"
          :templates-error="templatesError"
          :applying-template="applyingTemplate"
          :can-apply-template="patientAccepted && !medicalRecordLoading"
          @retry-context="selectedPatientId !== null && loadPatientContext(selectedPatientId)"
          @retry-templates="loadMedicalRecordTemplates"
          @apply-template="applyMedicalRecordTemplate"
        />
      </template>
    </StationTriPaneLayout>
  </div>
</template>

<style scoped>
/*
 * 医生站位于 HomeView 的内容区域内。
 * 减去顶部标签栏高度后，剩余空间全部交给三栏工作站。
 */
.doctor-station-page {
  background: var(--el-fill-color-lighter);
  box-sizing: border-box;
  width: 100%;
  height: calc(100% - var(--el-tabs-header-height, 40px));
  min-width: 0;
  min-height: 0;
  padding: 8px;
  overflow: hidden;
}

/* 中间区域纵向排列：患者信息栏、操作区、具体工作区。 */
.doctor-station-page__main {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  gap: 8px;
  padding: 8px;
  overflow: hidden;
}

/* 状态筛选固定在顶部，患者队列占满左栏剩余空间并独立滚动。 */
.doctor-station-page__queue {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
}

.doctor-station-page__queue :deep(.patient-queue-panel) {
  min-height: 0;
  flex: 1;
  border: 0;
}

/* flex: 1 让具体业务区占满中间栏剩余高度。 */
.doctor-station-page__workspace {
  display: flex;
  min-height: 0;
  flex: 1;
  overflow: auto;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}
</style>
