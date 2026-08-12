<script setup lang="ts">
import type {
  RegistrationContext,
  RegistrationPatientOption,
  RegistrationQueueRecord,
  SubmitRegistrationPayload
} from '@/apis/inpatient/charge/register'
import type { ActionToolbarItem, DateRangeValue, RowActionItem } from '@/components/common/type'

import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Plus, Refresh } from '@element-plus/icons-vue'

import {
  cancelInpatientRegistration,
  getInpatientRegistrationContext,
  getWardCapacity,
  listAttendingDoctors,
  listRegistrationWards,
  loadInpatientRegistrationBenefitPreview,
  loadInpatientRegistrationInit,
  pageInpatientRegistrationQueue,
  pageRegistrationPatientOptions,
  revokeInsuranceAdmission,
  submitInpatientRegistration
} from '@/apis/inpatient/charge/register'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import RegistrationDialog from './components/RegistrationDialog.vue'
import { REGISTRATION_STATUS_OPTIONS } from './constants'
import { useUserStore } from '@/stores/user'
import type { RegistrationInitModel, RegistrationPageQuery } from './type'

defineOptions({ name: 'InpatientChargeRegister' })

type RegistrationSubmitData = Omit<SubmitRegistrationPayload, 'operatorId'>

const query = reactive<RegistrationPageQuery>({
  keyword: '',
  idCard: '',
  status: '',
  dateRange: null,
  pageIndex: 1,
  pageSize: 20
})
const rows = ref<RegistrationQueueRecord[]>([])
const total = ref(0)
const loading = ref(false)
const initializing = ref(false)
const submitting = ref(false)
const errorMessage = ref('')
const init = ref<RegistrationInitModel | null>(null)
const dialogVisible = ref(false)
const dialogContext = ref<RegistrationContext | null>(null)
const dialogRef = ref<InstanceType<typeof RegistrationDialog>>()
const userStore = useUserStore()
let requestSequence = 0
let requestController: AbortController | undefined

const toolbarActions = computed<ActionToolbarItem[]>(() => [
  { key: 'create', label: '住院登记', type: 'primary', icon: Plus, disabled: submitting.value },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])

async function initialize() {
  initializing.value = true
  try {
    const [pageInit, context] = await Promise.all([
      loadInpatientRegistrationInit(),
      getInpatientRegistrationContext()
    ])
    init.value = {
      statusOptions: [
        { value: '', label: '全部状态' },
        { value: 'WAIT_ADMISSION', label: '待登记' },
        { value: 'ADMITTED', label: '已登记' },
        { value: 'CANCELLED', label: '已取消' }
      ],
      paymentChannelOptions: pageInit.paymentChannelOptions ?? [],
      contractOptions: context.contractOptions,
      organizationOptions: context.organizationOptions ?? [],
      inWayCodeOptions: context.inWayCodeOptions ?? [],
      admitSourceCodeOptions: context.admitSourceCodeOptions ?? []
    }
    await loadRows()
  } catch (error) {
    errorMessage.value = error instanceof Error ? error.message : '住院登记初始化失败'
  } finally {
    initializing.value = false
  }
}

async function loadRows() {
  requestController?.abort()
  const controller = new AbortController()
  requestController = controller
  const sequence = ++requestSequence
  loading.value = true
  errorMessage.value = ''
  try {
    const registrationListStatuses = query.status
      ? query.status === 'ADMITTED'
        ? ['REGISTERED', 'IN_PROGRESS', 'INHOSPITAL']
        : query.status === 'WAIT_ADMISSION'
          ? ['PLANNED', 'WAITING', 'PENDING']
          : ['CANCELLED', 'CANCELED']
      : ['PLANNED', 'REGISTERED', 'IN_PROGRESS', 'INHOSPITAL']
    const result = await pageInpatientRegistrationQueue(
      {
        pageIndex: query.pageIndex,
        pageSize: query.pageSize,
        keyword: query.keyword,
        idCard: query.idCard,
        // C1 后端 DTO 要求逗号分隔的字符串，不能传数组；同时显式包含 INHOSPITAL，
        // 否则新增住院登记成功后会被后端默认状态过滤掉。
        registrationListStatuses: registrationListStatuses.join(',')
      },
      { signal: controller.signal },
      { dateFrom: query.dateRange?.[0], dateTo: query.dateRange?.[1] }
    )
    if (sequence !== requestSequence) return
    rows.value = result.content
    total.value = result.totalCount
  } catch (error) {
    if (controller.signal.aborted || sequence !== requestSequence) return
    rows.value = []
    total.value = 0
    errorMessage.value = error instanceof Error ? error.message : '住院登记列表加载失败'
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}

function search() {
  query.pageIndex = 1
  void loadRows()
}

function reset() {
  Object.assign(query, { keyword: '', idCard: '', status: '', dateRange: null, pageIndex: 1 })
  void loadRows()
}

function updateDateRange(value: DateRangeValue) {
  query.dateRange = value
}

async function openDialog(record?: RegistrationQueueRecord) {
  try {
    dialogContext.value = await getInpatientRegistrationContext(record?.encounterId)
    dialogVisible.value = true
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '登记上下文加载失败')
  }
}

async function save(payload: RegistrationSubmitData) {
  if (submitting.value) return
  const operatorId = userStore.getUser?.id
  const previousTotal = total.value
  const previousEncounterIds = new Set(rows.value.map((item) => item.encounterId))
  submitting.value = true
  try {
    const result = await submitInpatientRegistration({
      ...payload,
      ...(operatorId ? { operatorId } : {})
    })
    dialogRef.value?.resetDirty()
    dialogVisible.value = false
    await loadRows()
    const listContainsNewRecord =
      total.value > previousTotal ||
      rows.value.some((item) => !previousEncounterIds.has(item.encounterId))
    if (!listContainsNewRecord) {
      ElMessage.warning('后端已返回登记成功，但登记列表未返回新记录，请后端修正新增与列表的数据范围')
      return
    }
    ElMessage.success(result.paymentId ? '住院登记成功，预交金支付已完成' : '住院登记成功')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '住院登记失败')
  } finally {
    submitting.value = false
  }
}

async function cancelRecord(record: RegistrationQueueRecord) {
  if (submitting.value) return
  try {
    const { value } = await ElMessageBox.prompt(
      '请输入取消原因',
      record.insurancePlatformRegistered ? '医保退入院确认' : '取消登记确认',
      {
        inputValidator: (value) => Boolean(value.trim()) || '请输入取消原因',
        type: 'warning'
      }
    )
    submitting.value = true
    if (record.insurancePlatformRegistered) {
      await revokeInsuranceAdmission(record.encounterId, value)
      ElMessage.success('医保平台入院已撤销，登记已取消')
    } else {
      await cancelInpatientRegistration(record.encounterId, value)
      ElMessage.success('登记已取消')
    }
    await loadRows()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '取消登记失败')
  } finally {
    submitting.value = false
  }
}

function rowActions(source: unknown): RowActionItem[] {
  const record = source as RegistrationQueueRecord
  return [
    { key: 'view', label: '查看', type: 'primary', disabled: submitting.value },
    {
      key: 'register',
      label: '登记',
      type: 'success',
      disabled: submitting.value || record.statusEnum !== 'WAIT_ADMISSION'
    },
    {
      key: 'cancel',
      label: record.insurancePlatformRegistered ? '医保退入院' : '取消登记',
      type: 'danger',
      disabled: submitting.value || !record.cancelable || record.statusEnum === 'CANCELLED'
    }
  ]
}

function handleRowAction(action: RowActionItem, source: unknown) {
  const record = source as RegistrationQueueRecord
  if (action.key === 'view' || action.key === 'register') void openDialog(record)
  else if (action.key === 'cancel') void cancelRecord(record)
}

function handleToolbar(action: ActionToolbarItem) {
  if (action.key === 'create') void openDialog()
  else void loadRows()
}

const loadPatients = async (
  request: { keyword: string; pageIndex: number; pageSize: number },
  context: { signal: AbortSignal }
) => {
  const result = await pageRegistrationPatientOptions(
    { keyword: request.keyword, pageIndex: request.pageIndex, pageSize: request.pageSize },
    { signal: context.signal }
  )
  return {
    rows: result.records as Array<RegistrationPatientOption & Record<string, unknown>>,
    total: Number(result.total)
  }
}

onMounted(() => void initialize())
onBeforeUnmount(() => {
  requestController?.abort()
  requestSequence += 1
})
</script>

<template>
  <main class="registration-page">
    <SearchPanel
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.status"
      keyword-label="患者信息"
      keyword-placeholder="姓名、住院号或身份证号"
      dropdown-label="登记状态"
      :dropdown-options="init?.statusOptions ?? []"
      :loading="loading || initializing"
      :field-count="2"
      @search="search"
      @reset="reset"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">身份证号</label>
          <el-input v-model="query.idCard" clearable />
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">申请日期</label>
          <DateRangeField :model-value="query.dateRange" @update:model-value="updateDateRange" />
        </div>
      </template>
    </SearchPanel>
    <ActionToolbar title="住院登记记录" :actions="toolbarActions" @action="handleToolbar" />
    <el-alert v-if="errorMessage" type="error" :closable="false" :title="errorMessage">
      <el-button type="primary" link @click="loadRows">重试</el-button>
    </el-alert>
    <DataTablePanel
      :data="rows"
      :loading="loading"
      :total="total"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      show-index
      min-height="430px"
      @update:page="
        (value: number) => {
          query.pageIndex = value
          loadRows()
        }
      "
      @update:page-size="
        (value: number) => {
          query.pageSize = value
          query.pageIndex = 1
          loadRows()
        }
      "
    >
      <el-table-column prop="encounterBusNo" label="住院号/申请号" min-width="170" />
      <el-table-column prop="patientName" label="患者姓名" width="110" />
      <el-table-column prop="organizationName" label="入院科室" min-width="130" />
      <el-table-column prop="wardName" label="病区" min-width="120" />
      <el-table-column prop="bedName" label="床位" width="80" />
      <el-table-column prop="startTime" label="申请/入院时间" width="170" />
      <el-table-column label="医保入院" width="100">
        <template #default="{ row }"
          ><el-tag :type="row.insurancePlatformRegistered ? 'success' : 'info'">{{
            row.insurancePlatformRegistered ? '已办理' : '未办理'
          }}</el-tag></template
        >
      </el-table-column>
      <el-table-column label="状态" width="100">
        <template #default="{ row }"
          ><StatusDisplay :value="row.statusEnum" :options="REGISTRATION_STATUS_OPTIONS"
        /></template>
      </el-table-column>
      <el-table-column label="操作" width="250" fixed="right">
        <template #default="{ row }"
          ><RowActionGroup :row="row" :actions="rowActions(row)" @action="handleRowAction"
        /></template>
      </el-table-column>
    </DataTablePanel>
    <RegistrationDialog
      ref="dialogRef"
      v-model="dialogVisible"
      :context="dialogContext"
      :init="init"
      :submitting="submitting"
      :load-patients="loadPatients"
      :load-wards="
        async (organizationId, signal) =>
          (await listRegistrationWards(organizationId, { signal })).wards
      "
      :load-doctors="
        async (keyword, organizationId, wardId, signal) =>
          (
            (await listAttendingDoctors({ organizationId, wardLocationId: wardId }, { signal }))
              .doctorOptions ?? []
          )
            .filter((item) => !keyword || item.label.includes(keyword))
            .map((item) => ({ ...item }))
      "
      :load-capacity="(wardId) => getWardCapacity(wardId)"
      :load-benefit-preview="loadInpatientRegistrationBenefitPreview"
      @submit="save"
    />
  </main>
</template>

<style scoped>
.registration-page {
  --el-color-primary: #1d4ed8;
  --el-color-success: #15803d;
  --el-color-warning: #a16207;
  --el-color-danger: #b91c1c;
  --el-color-info: #4b5563;
  --el-text-color-primary: #1f2937;
  --el-text-color-regular: #374151;
  --el-text-color-secondary: #4b5563;
  --el-text-color-placeholder: #6b7280;
  display: flex;
  flex-direction: column;
  gap: 12px;
  min-height: calc(100vh - 96px);
  padding: 16px;
}
:deep(.query-shell__field > .el-input),
:deep(.query-shell__field > .date-range-field) {
  min-width: 0;
  flex: 1;
}
:deep(.el-button.is-link.is-disabled) {
  color: #6b7280;
}
</style>
