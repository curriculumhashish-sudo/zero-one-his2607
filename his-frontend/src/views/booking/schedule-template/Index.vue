<script setup lang="ts">
import type { ActionToolbarItem, RowActionItem } from '@/components/common/type'
import type {
  DailyTemplate,
  HolidayException,
  ReleasePolicy,
  ReleasePolicySavePayload,
  ScheduleTemplateInit,
  TimePeriod,
  WeeklyTemplate,
  SaveCalendarExceptionRequest,
  SaveScheduleTemplateRequest,
  SaveSlotProvisionTemplateRequest,
  SaveWeekScheduleTemplateRequest
} from '@/apis/booking/schedule-template'
import { computed, onBeforeUnmount, onMounted, reactive, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Plus, Refresh, ArrowRight } from '@element-plus/icons-vue'
import { useRouter } from 'vue-router'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import {
  loadScheduleTemplateInit,
  pageDailyTemplates,
  pageHolidayExceptions,
  pageReleasePolicies,
  pageTimePeriods,
  pageWeeklyTemplates,
  removeDailyTemplate,
  removeHolidayException,
  removeReleasePolicy,
  removeTimePeriod,
  removeWeeklyTemplate,
  saveDailyTemplate,
  saveHolidayException,
  saveReleasePolicy,
  saveTimePeriod,
  saveWeeklyTemplate
} from '@/apis/booking/schedule-template'
import { TEMPLATE_STATUS_OPTIONS, WEEKDAYS } from './constants'
import type {
  DailyTemplateForm,
  HolidayExceptionForm,
  ScheduleTemplatePageQuery,
  TimePeriodForm,
  WeeklyTemplateForm
} from './type'

defineOptions({ name: 'BookingScheduleTemplate' })
const router = useRouter()
const activeTab = ref('period')
const init = ref<ScheduleTemplateInit | null>(null)
const initLoading = ref(false)
const initError = ref('')
const loading = ref(false)
const saving = ref(false)
const errorMessage = ref('')
const requestController = ref<AbortController>()
let requestSequence = 0
const query = reactive<ScheduleTemplatePageQuery>({
  keyword: '',
  status: '',
  pageIndex: 1,
  pageSize: 10
})
const periods = ref<TimePeriod[]>([])
const dailyRows = ref<DailyTemplate[]>([])
const weeklyRows = ref<WeeklyTemplate[]>([])
const periodTotal = ref(0)
const dailyTotal = ref(0)
const weeklyTotal = ref(0)
const holidays = ref<HolidayException[]>([])
const holidayTotal = ref(0)
const holidayType = ref<'' | 'CLOSED' | 'REDUCED' | 'OVERRIDE' | 'EXTRA'>('')
const policyRows = ref<ReleasePolicy[]>([])
const policyTotal = ref(0)
const policyDialog = ref(false)
const policyId = ref<string>()
const policyForm = reactive<ReleasePolicySavePayload>({
  policyCode: '',
  policyName: '',
  advanceDays: 14,
  slotMinutes: 30,
  capacityPerSlot: 10,
  channelQuotaJson: ''
})
const holidayCalendarDate = ref(new Date(2026, 9, 1))

const periodDialog = ref(false)
const periodId = ref<string>()
const periodForm = reactive<TimePeriodForm>({
  name: '',
  segments: [{ start: '08:00', end: '12:00', capacity: 40 }],
  status: 'enabled'
})
const dailyDialog = ref(false)
const dailyId = ref<string>()
const dailyForm = reactive<DailyTemplateForm>({
  name: '',
  scheduleMode: 'MODE-A',
  serviceType: '',
  periodIds: [],
  doctorId: '',
  roomId: '',
  releasePolicyId: 'policy-default',
  effectiveStart: '2026-07-01',
  effectiveEnd: '2026-12-31',
  priority: 0,
  defaultAppointment: true,
  status: 'enabled'
})
const weeklyDialog = ref(false)
const weeklyId = ref<string>()
const weeklyForm = reactive<WeeklyTemplateForm>({
  name: '',
  isDefault: true,
  appointmentOpen: true,
  status: 'enabled',
  items: [{ weekday: 1, dailyTemplateId: '', enabled: true }]
})
const holidayDialog = ref(false)
const holidayId = ref<string>()
const holidayForm = reactive<HolidayExceptionForm>({
  date: '2026-10-01',
  type: 'closed',
  reason: '',
  autoApplyOnGenerate: true
})

const toolbarActions = computed<ActionToolbarItem[]>(() =>
  activeTab.value === 'policy'
    ? [
        { key: 'create', label: '新增策略', type: 'primary', icon: Plus },
        { key: 'refresh', label: '刷新策略', icon: Refresh }
      ]
    : activeTab.value === 'holiday'
      ? [
          { key: 'create', label: '新增例外', type: 'primary', icon: Plus },
          { key: 'refresh', label: '刷新', icon: Refresh }
        ]
      : [
          { key: 'create', label: '新增', type: 'primary', icon: Plus },
          { key: 'refresh', label: '刷新', icon: Refresh }
        ]
)
const periodOptions = computed(() =>
  periods.value.map((item) => ({ value: item.id, label: item.name }))
)
const dailyOptions = computed(() =>
  dailyRows.value.map((item) => ({ value: item.id, label: item.name }))
)
const policyOptions = computed(() =>
  policyRows.value.map((item) => ({ value: item.id, label: item.policyName ?? item.name }))
)
const statusFilterOptions = TEMPLATE_STATUS_OPTIONS.map((item) => ({
  label: item.label,
  value: String(item.value)
}))

function monthRange(value: Date) {
  const start = new Date(value.getFullYear(), value.getMonth(), 1)
  const end = new Date(value.getFullYear(), value.getMonth() + 1, 0)
  const format = (date: Date) =>
    `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
  return { start: format(start), end: format(end) }
}
function dateText(value: Date) {
  return `${value.getFullYear()}-${String(value.getMonth() + 1).padStart(2, '0')}-${String(value.getDate()).padStart(2, '0')}`
}
function ensureCurrentDepartment() {
  if (init.value?.department?.value) return true
  ElMessage.warning('当前登录科室尚未加载，不能保存排班模板')
  return false
}

async function initialize() {
  initLoading.value = true
  initError.value = ''
  try {
    init.value = await loadScheduleTemplateInit()
    await loadCurrentTab()
  } catch (error) {
    initError.value = error instanceof Error ? error.message : '排班设置初始化失败'
  } finally {
    initLoading.value = false
  }
}
async function loadCurrentTab() {
  requestController.value?.abort()
  const controller = new AbortController()
  requestController.value = controller
  const sequence = ++requestSequence
  loading.value = true
  errorMessage.value = ''
  try {
    if (activeTab.value === 'period') {
      const result = await pageTimePeriods(
        {
          keyword: query.keyword,
          status: query.status || null,
          pageIndex: query.pageIndex,
          pageSize: query.pageSize
        },
        { signal: controller.signal }
      )
      periods.value = result.content
      periodTotal.value = result.totalCount
    } else if (activeTab.value === 'daily') {
      const result = await pageDailyTemplates(
        {
          keyword: query.keyword,
          status: query.status || null,
          pageIndex: query.pageIndex,
          pageSize: query.pageSize
        },
        { signal: controller.signal }
      )
      dailyRows.value = result.content
      dailyTotal.value = result.totalCount
      periods.value = (
        await pageTimePeriods({ pageIndex: 1, pageSize: 200 }, { signal: controller.signal })
      ).content
      policyRows.value = (
        await pageReleasePolicies({ pageIndex: 1, pageSize: 200 }, { signal: controller.signal })
      ).content
    } else if (activeTab.value === 'weekly') {
      const result = await pageWeeklyTemplates(
        {
          keyword: query.keyword,
          status: query.status || null,
          pageIndex: query.pageIndex,
          pageSize: query.pageSize
        },
        { signal: controller.signal }
      )
      weeklyRows.value = result.content
      weeklyTotal.value = result.totalCount
      dailyRows.value = (
        await pageDailyTemplates({ pageIndex: 1, pageSize: 200 }, { signal: controller.signal })
      ).content
    } else if (activeTab.value === 'policy') {
      const result = await pageReleasePolicies(
        {
          organizationId: init.value?.organizationId,
          keyword: query.keyword,
          pageIndex: query.pageIndex,
          pageSize: query.pageSize
        },
        { signal: controller.signal }
      )
      policyRows.value = result.content
      policyTotal.value = result.totalCount
    } else {
      const range = monthRange(holidayCalendarDate.value)
      const result = await pageHolidayExceptions(
        {
          organizationId: init.value?.organizationId,
          exceptionType: holidayType.value || undefined,
          rangeStart: range.start,
          rangeEnd: range.end,
          keyword: query.keyword,
          pageIndex: query.pageIndex,
          pageSize: query.pageSize
        },
        { signal: controller.signal }
      )
      holidays.value = result.content
      holidayTotal.value = result.totalCount
      dailyRows.value = (
        await pageDailyTemplates({ pageIndex: 1, pageSize: 200 }, { signal: controller.signal })
      ).content
    }
  } catch (error) {
    if (!controller.signal.aborted)
      errorMessage.value = error instanceof Error ? error.message : '当前页签加载失败'
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}
function resetQuery() {
  query.keyword = ''
  query.status = ''
  query.pageIndex = 1
  void loadCurrentTab()
}
function searchCurrentTab() {
  query.pageIndex = 1
  void loadCurrentTab()
}
function changePage(page: number) {
  query.pageIndex = page
  void loadCurrentTab()
}
function changePageSize(pageSize: number) {
  query.pageSize = pageSize
  query.pageIndex = 1
  void loadCurrentTab()
}
function rowActions(): RowActionItem[] {
  return [
    { key: 'edit', label: '编辑', type: 'primary' },
    {
      key: 'remove',
      label: '删除',
      type: 'danger',
      confirmText: '确认删除？已被其他模板引用的数据会被接口拒绝。'
    }
  ]
}
function openPeriod(row?: TimePeriod) {
  periodId.value = row?.id
  Object.assign(periodForm, {
    name: row?.name ?? '',
    segments: row?.segments.map((item) => ({
      start: item.start,
      end: item.end,
      capacity: item.capacity
    })) ?? [{ start: '08:00', end: '12:00', capacity: 40 }],
    status: row?.status ?? 'enabled'
  })
  periodDialog.value = true
}
function openDaily(row?: DailyTemplate) {
  dailyId.value = row?.id
  Object.assign(dailyForm, {
    name: row?.name ?? '',
    scheduleMode: row?.scheduleMode ?? 'MODE-A',
    serviceType: row?.serviceType ?? '',
    periodIds: row?.periodIds ?? [],
    doctorId: row?.doctorId ?? '',
    roomId: row?.roomId ?? '',
    releasePolicyId: row?.releasePolicyId ?? policyRows.value[0]?.id ?? '',
    effectiveStart: row?.effectiveStart ?? '2026-07-01',
    effectiveEnd: row?.effectiveEnd ?? '2026-12-31',
    priority: row?.priority ?? 0,
    defaultAppointment: row?.defaultAppointment ?? true,
    status: row?.status ?? 'enabled'
  })
  dailyDialog.value = true
}
function openWeekly(row?: WeeklyTemplate) {
  weeklyId.value = row?.id
  Object.assign(weeklyForm, {
    name: row?.name ?? '',
    isDefault: row?.isDefault ?? true,
    appointmentOpen: row?.appointmentOpen ?? true,
    status: row?.status ?? 'enabled',
    items: row?.items.map((item) => ({
      weekday: item.weekday,
      dailyTemplateId: item.dailyTemplateId,
      enabled: item.enabled
    })) ?? [{ weekday: 1, dailyTemplateId: '', enabled: true }]
  })
  weeklyDialog.value = true
}
function openHoliday(row?: HolidayException) {
  holidayId.value = row?.id
  Object.assign(holidayForm, {
    date: row?.date ?? dateText(holidayCalendarDate.value),
    rangeEnd: row?.rangeEnd,
    type: row?.type ?? 'closed',
    replacementDailyTemplateId: row?.replacementDailyTemplateId,
    reason: row?.reason ?? '',
    autoApplyOnGenerate: row?.autoApplyOnGenerate ?? true
  })
  holidayDialog.value = true
}
function openPolicy(row?: ReleasePolicy) {
  policyId.value = row?.id
  const channelQuotaJson =
    row?.channelQuotaJsonText ??
    (row?.channelQuotas ? JSON.stringify(row.channelQuotas, null, 2) : '')
  Object.assign(policyForm, {
    policyCode: row?.policyCode ?? '',
    policyName: row?.policyName ?? row?.name ?? '',
    advanceDays: row?.advanceDays ?? row?.openDays ?? 14,
    slotMinutes: row?.slotMinutes ?? 30,
    capacityPerSlot: row?.capacityPerSlot ?? 10,
    channelQuotaJson
  })
  policyDialog.value = true
}
function openHolidayForDate(date: string) {
  openHoliday()
  holidayForm.date = date
}
function handleHolidayStartChange(date?: string) {
  if (date && holidayForm.rangeEnd && holidayForm.rangeEnd < date) {
    holidayForm.rangeEnd = undefined
  }
}
function disableHolidayEndDate(date: Date) {
  return Boolean(holidayForm.date && dateText(date) < holidayForm.date)
}
async function persist(
  action: () => Promise<unknown>,
  close: () => void,
  message: string,
  afterSuccess?: () => Promise<void>
) {
  if (saving.value) return
  saving.value = true
  try {
    await action()
    close()
    ElMessage.success(message)
    await loadCurrentTab()
    await afterSuccess?.()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '保存失败')
  } finally {
    saving.value = false
  }
}
function savePeriodForm() {
  if (!ensureCurrentDepartment()) return
  if (
    !periodForm.name.trim() ||
    !periodForm.segments.length ||
    periodForm.segments.some(
      (item) => !item.start || !item.end || item.end <= item.start || item.capacity < 1
    )
  ) {
    ElMessage.warning('请填写名称，并确保每个结束时间晚于开始时间')
    return
  }
  void persist(
    () => {
      const payload: SaveSlotProvisionTemplateRequest = {
        id: periodId.value,
        organizationId: init.value?.organizationId,
        templateName: periodForm.name.trim(),
        slotTemplates: periodForm.segments.map((item) => ({
          startTime: item.start,
          endTime: item.end,
          capacity: item.capacity
        })),
        enabled: periodForm.status === 'enabled'
      }
      return saveTimePeriod(payload)
    },
    () => {
      periodDialog.value = false
    },
    '放号时段已保存'
  )
}
function saveDailyForm() {
  if (!ensureCurrentDepartment()) return
  if (
    !dailyForm.name ||
    !dailyForm.serviceType ||
    !dailyForm.periodIds.length ||
    !dailyForm.doctorId ||
    !dailyForm.roomId ||
    !dailyForm.releasePolicyId ||
    !dailyForm.effectiveStart ||
    !dailyForm.effectiveEnd ||
    dailyForm.effectiveEnd < dailyForm.effectiveStart
  ) {
    ElMessage.warning('请完整填写日排班配置')
    return
  }
  void persist(
    () => {
      const payload: SaveScheduleTemplateRequest = {
        id: dailyId.value,
        organizationId: init.value!.organizationId ?? init.value!.department.value,
        templateName: dailyForm.name.trim(),
        scheduleMode: dailyForm.scheduleMode,
        practitionerId: dailyForm.doctorId,
        locationId: dailyForm.roomId,
        healthcareServiceId: dailyForm.serviceType,
        slotProvisionTemplateId: dailyForm.periodIds[0],
        defaultReleasePolicyId: dailyForm.releasePolicyId,
        effectiveStart: dailyForm.effectiveStart,
        effectiveEnd: dailyForm.effectiveEnd,
        priority: dailyForm.priority,
        enabled: dailyForm.status === 'enabled'
      }
      return saveDailyTemplate(payload)
    },
    () => {
      dailyDialog.value = false
    },
    '日排班已保存'
  )
}
function saveWeeklyForm() {
  if (!ensureCurrentDepartment()) return
  if (
    !weeklyForm.name.trim() ||
    !weeklyForm.items.length ||
    weeklyForm.items.some((item) => !item.dailyTemplateId) ||
    new Set(weeklyForm.items.map((item) => item.weekday)).size !== weeklyForm.items.length
  ) {
    ElMessage.warning('请填写名称、周明细，并确保星期不重复')
    return
  }
  void persist(
    () => {
      const payload: SaveWeekScheduleTemplateRequest = {
        id: weeklyId.value,
        organizationId: init.value!.organizationId ?? init.value!.department.value,
        templateName: weeklyForm.name.trim(),
        fallbackBookable: weeklyForm.appointmentOpen,
        isDefault: weeklyForm.isDefault,
        enabled: weeklyForm.status === 'enabled',
        items: weeklyForm.items.map((item, index) => ({
          weekday: item.weekday,
          dayScheduleTemplateId: item.dailyTemplateId,
          sortNo: index + 1,
          enabled: item.enabled
        }))
      }
      return saveWeeklyTemplate(payload)
    },
    () => {
      weeklyDialog.value = false
    },
    '周排班已保存',
    async () => {
      try {
        await ElMessageBox.confirm(
          '模板保存后尚不能预约，请前往门诊排班生成正式排班并发布。',
          '下一步：门诊排班',
          { confirmButtonText: '前往门诊排班', cancelButtonText: '稍后', type: 'info' }
        )
        await router.push('/booking/outpatient-schedule')
      } catch {
        // 用户选择稍后。
      }
    }
  )
}
function saveHolidayForm() {
  if (!ensureCurrentDepartment()) return
  if (!holidayForm.date || !holidayForm.type) {
    ElMessage.warning('请填写起始日和例外类型')
    return
  }
  if (holidayForm.rangeEnd && holidayForm.rangeEnd < holidayForm.date) {
    ElMessage.warning('结束日期不能早于起始日')
    return
  }
  if (holidayForm.type !== 'closed' && !holidayForm.replacementDailyTemplateId) {
    ElMessage.warning('替换工作日、减量出诊或临时加诊必须选择替代日排班')
    return
  }
  void persist(
    () => {
      const typeMap = {
        closed: 'CLOSED',
        reduced: 'REDUCED',
        replace: 'OVERRIDE',
        extra: 'EXTRA'
      } as const
      const payload: SaveCalendarExceptionRequest = {
        id: holidayId.value,
        organizationId: init.value?.organizationId,
        exceptionDate: holidayForm.date,
        rangeEnd: holidayForm.rangeEnd,
        exceptionType: typeMap[holidayForm.type],
        overrideScheduleTemplateId: holidayForm.replacementDailyTemplateId,
        reason: holidayForm.reason.trim(),
        autoApplyOnGenerate: holidayForm.autoApplyOnGenerate
      }
      return saveHolidayException(payload)
    },
    () => {
      holidayDialog.value = false
    },
    '节假日例外已保存'
  )
}
async function remove(kind: string, id: string) {
  try {
    await ElMessageBox.confirm('删除后不可恢复，确认删除当前记录吗？', '删除确认', {
      confirmButtonText: '删除',
      cancelButtonText: '取消',
      type: 'warning'
    })
    if (kind === 'period') await removeTimePeriod(id)
    else if (kind === 'daily') await removeDailyTemplate(id)
    else if (kind === 'weekly') await removeWeeklyTemplate(id)
    else if (kind === 'policy') await removeReleasePolicy(id)
    else await removeHolidayException(id)
    ElMessage.success('删除成功')
    await loadCurrentTab()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '删除失败')
  }
}
function handleRowAction(action: RowActionItem, source: unknown) {
  const row = source as { id: string }
  if (action.key === 'remove') void remove(activeTab.value, row.id)
  else if (activeTab.value === 'period') openPeriod(source as TimePeriod)
  else if (activeTab.value === 'daily') openDaily(source as DailyTemplate)
  else if (activeTab.value === 'weekly') openWeekly(source as WeeklyTemplate)
  else if (activeTab.value === 'policy') openPolicy(source as ReleasePolicy)
  else openHoliday(source as HolidayException)
}
function handleToolbar(action: ActionToolbarItem) {
  if (action.key === 'refresh') void loadCurrentTab()
  else if (activeTab.value === 'period') openPeriod()
  else if (activeTab.value === 'daily') openDaily()
  else if (activeTab.value === 'weekly') openWeekly()
  else if (activeTab.value === 'policy') openPolicy()
  else openHoliday()
}
async function savePolicyForm() {
  if (!ensureCurrentDepartment()) return
  if (
    !policyForm.policyName.trim() ||
    policyForm.slotMinutes < 5 ||
    policyForm.capacityPerSlot < 1
  ) {
    ElMessage.warning('请填写策略名称、时段粒度和单段容量')
    return
  }
  try {
    if (policyForm.channelQuotaJson) JSON.parse(policyForm.channelQuotaJson)
  } catch {
    ElMessage.warning('渠道配额必须是有效 JSON')
    return
  }
  const payload: ReleasePolicySavePayload = {
    id: policyId.value,
    policyCode: policyForm.policyCode?.trim() || undefined,
    policyName: policyForm.policyName.trim(),
    advanceDays: Number(policyForm.advanceDays),
    slotMinutes: Number(policyForm.slotMinutes),
    capacityPerSlot: Number(policyForm.capacityPerSlot),
    channelQuotaJson: policyForm.channelQuotaJson?.trim() || undefined
  }
  await persist(
    () => saveReleasePolicy(payload),
    () => {
      policyDialog.value = false
    },
    '放号策略已保存'
  )
}
function addSegment() {
  periodForm.segments.push({ start: '14:00', end: '17:00', capacity: 30 })
}
function addWeeklyItem() {
  const used = new Set(weeklyForm.items.map((item) => item.weekday))
  const weekday = WEEKDAYS.find((item) => !used.has(item.value))?.value
  if (weekday === undefined) {
    ElMessage.warning('一周七天均已配置')
    return
  }
  weeklyForm.items.push({ weekday, dailyTemplateId: '', enabled: true })
}
function removeWeeklyItem(index: number) {
  if (weeklyForm.items.length <= 1) {
    ElMessage.warning('至少保留一条周明细')
    return
  }
  weeklyForm.items.splice(index, 1)
}
function removeSegment(index: number) {
  if (periodForm.segments.length <= 1) {
    ElMessage.warning('至少保留一个起止时间段')
    return
  }
  periodForm.segments.splice(index, 1)
}
watch(activeTab, () => {
  query.keyword = ''
  query.status = ''
  query.pageIndex = 1
  void loadCurrentTab()
})
watch(holidayCalendarDate, () => {
  if (activeTab.value === 'holiday') void loadCurrentTab()
})
onMounted(() => void initialize())
onBeforeUnmount(() => requestController.value?.abort())
</script>

<template>
  <main class="template-page" v-loading="initLoading">
    <header class="page-heading">
      <div>
        <h1>排班设置</h1>
        <p>{{ init?.department.label || '当前科室加载中' }} · 排班模板与放号规则</p>
      </div>
      <el-button :icon="ArrowRight" @click="router.push('/booking/outpatient-schedule')"
        >前往门诊排班</el-button
      >
    </header>
    <el-alert type="info" :closable="false" show-icon>
      <template #title>排班模板保存后尚不能预约</template>
      请依次维护放号时段、日排班和周排班，再前往门诊排班生成正式排班、发布并放号。
    </el-alert>
    <el-alert v-if="initError" type="error" :title="initError" show-icon :closable="false"
      ><el-button type="primary" link @click="initialize">重新初始化</el-button></el-alert
    >
    <el-tabs v-model="activeTab" class="template-tabs">
      <el-tab-pane label="① 放号时段" name="period" /><el-tab-pane
        label="② 日排班"
        name="daily"
      /><el-tab-pane label="③ 周排班" name="weekly" /><el-tab-pane
        label="放号策略（可选）"
        name="policy"
      /><el-tab-pane label="节假日例外（可选）" name="holiday" />
    </el-tabs>
    <SearchPanel
      v-if="activeTab !== 'holiday'"
      v-model:keyword="query.keyword"
      v-model:dropdown-value="query.status"
      keyword-placeholder="输入模板名称"
      dropdown-label="状态"
      :dropdown-options="statusFilterOptions"
      :loading="loading"
      @search="searchCurrentTab"
      @reset="resetQuery"
    />
    <SearchPanel
      v-if="activeTab === 'holiday'"
      v-model:keyword="query.keyword"
      v-model:dropdown-value="holidayType"
      keyword-placeholder="输入例外原因"
      dropdown-label="例外类型"
      :dropdown-options="[
        { value: 'CLOSED', label: '停诊' },
        { value: 'REDUCED', label: '减号' },
        { value: 'OVERRIDE', label: '替换模板' },
        { value: 'EXTRA', label: '加班' }
      ]"
      :loading="loading"
      @search="searchCurrentTab"
      @reset="
        () => {
          holidayType = ''
          resetQuery()
        }
      "
    />
    <ActionToolbar
      :title="
        activeTab === 'period'
          ? '放号时段'
          : activeTab === 'daily'
            ? '日排班模板'
            : activeTab === 'weekly'
              ? '周排班模板'
              : activeTab === 'policy'
                ? '放号策略'
                : '节假日例外'
      "
      :actions="toolbarActions"
      :loading="loading"
      @action="handleToolbar"
    />
    <el-alert v-if="errorMessage" type="error" :title="errorMessage" show-icon :closable="false"
      ><el-button type="primary" link @click="loadCurrentTab">重试</el-button></el-alert
    >

    <DataTablePanel
      v-if="activeTab === 'period'"
      :data="periods"
      :loading="loading"
      :total="periodTotal"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      min-height="300px"
      @update:page="changePage"
      @update:page-size="changePageSize"
    >
      <el-table-column prop="name" label="时段名称" min-width="150" /><el-table-column
        label="起止时间"
        min-width="240"
        ><template #default="{ row }">{{
          row.segments
            .map((item: any) => `${item.start}-${item.end}（${item.capacity}号）`)
            .join('、')
        }}</template></el-table-column
      ><el-table-column prop="usageCount" label="引用数" width="90" /><el-table-column
        label="状态"
        width="100"
        ><template #default="{ row }"
          ><StatusDisplay
            :value="row.status"
            :options="TEMPLATE_STATUS_OPTIONS" /></template></el-table-column
      ><el-table-column label="操作" width="160"
        ><template #default="{ row }"
          ><RowActionGroup :row="row" :actions="rowActions()" @action="handleRowAction" /></template
      ></el-table-column>
    </DataTablePanel>
    <DataTablePanel
      v-else-if="activeTab === 'daily'"
      :data="dailyRows"
      :loading="loading"
      :total="dailyTotal"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      min-height="300px"
      @update:page="changePage"
      @update:page-size="changePageSize"
    >
      <el-table-column prop="name" label="模板名称" min-width="150" /><el-table-column
        prop="scheduleMode"
        label="模式"
        width="95"
      /><el-table-column prop="serviceType" label="服务类型" width="110" /><el-table-column
        prop="doctorName"
        label="出诊人员"
        width="110"
      /><el-table-column prop="roomName" label="地点" min-width="150" /><el-table-column
        prop="priority"
        label="优先级"
        width="80"
      /><el-table-column prop="effectiveStart" label="生效开始" width="115" /><el-table-column
        prop="effectiveEnd"
        label="生效结束"
        width="115"
      /><el-table-column label="可预约" width="90"
        ><template #default="{ row }">{{
          row.defaultAppointment ? '是' : '否'
        }}</template></el-table-column
      ><el-table-column label="状态" width="90"
        ><template #default="{ row }"
          ><StatusDisplay
            :value="row.status"
            :options="TEMPLATE_STATUS_OPTIONS" /></template></el-table-column
      ><el-table-column label="操作" width="160"
        ><template #default="{ row }"
          ><RowActionGroup :row="row" :actions="rowActions()" @action="handleRowAction" /></template
      ></el-table-column>
    </DataTablePanel>
    <DataTablePanel
      v-else-if="activeTab === 'weekly'"
      :data="weeklyRows"
      :loading="loading"
      :total="weeklyTotal"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      min-height="300px"
      @update:page="changePage"
      @update:page-size="changePageSize"
    >
      <el-table-column prop="name" label="模板名称" min-width="160" /><el-table-column
        label="周明细"
        min-width="260"
        ><template #default="{ row }">{{
          row.items.map((item: any) => `${item.weekdayLabel}：${item.dailyTemplateName}`).join('；')
        }}</template></el-table-column
      ><el-table-column label="科室默认" width="100"
        ><template #default="{ row }">{{ row.isDefault ? '是' : '否' }}</template></el-table-column
      ><el-table-column label="开放预约" width="100"
        ><template #default="{ row }">{{
          row.appointmentOpen ? '开放' : '关闭'
        }}</template></el-table-column
      ><el-table-column label="状态" width="90"
        ><template #default="{ row }"
          ><StatusDisplay
            :value="row.status"
            :options="TEMPLATE_STATUS_OPTIONS" /></template></el-table-column
      ><el-table-column label="操作" width="160"
        ><template #default="{ row }"
          ><RowActionGroup :row="row" :actions="rowActions()" @action="handleRowAction" /></template
      ></el-table-column>
    </DataTablePanel>
    <DataTablePanel
      v-else-if="activeTab === 'policy'"
      :data="policyRows"
      :loading="loading"
      :total="policyTotal"
      :page="query.pageIndex"
      :page-size="query.pageSize"
      min-height="300px"
      @update:page="changePage"
      @update:page-size="changePageSize"
    >
      <el-table-column prop="policyCode" label="策略编码" min-width="140" />
      <el-table-column prop="policyName" label="策略名称" min-width="170" />
      <el-table-column prop="advanceDays" label="提前天数" width="100" />
      <el-table-column prop="slotMinutes" label="时段分钟" width="100" />
      <el-table-column prop="capacityPerSlot" label="单段容量" width="100" />
      <el-table-column label="操作" width="160">
        <template #default="{ row }">
          <RowActionGroup :row="row" :actions="rowActions()" @action="handleRowAction" />
        </template>
      </el-table-column>
    </DataTablePanel>
    <section v-else class="holiday-layout">
      <el-calendar v-model="holidayCalendarDate" class="holiday-calendar"
        ><template #date-cell="{ data }"
          ><div class="holiday-date-cell" @click="openHolidayForDate(data.day)">
            <span>{{ data.day.split('-').pop() }}</span
            ><el-badge
              v-if="holidays.some((item) => item.date === data.day)"
              is-dot
              type="warning"
            /></div></template
      ></el-calendar>
      <div class="holiday-list">
        <EmptyState
          v-if="!loading && !holidays.length"
          title="本月暂无节假日例外"
          action-text="新增例外"
          @action="openHoliday()"
        /><DataTablePanel
          v-else
          :data="holidays"
          :loading="loading"
          :total="holidayTotal"
          :page="query.pageIndex"
          :page-size="query.pageSize"
          min-height="280px"
          @update:page="changePage"
          @update:page-size="changePageSize"
          ><el-table-column prop="date" label="起始日" width="120" /><el-table-column
            prop="rangeEnd"
            label="结束日"
            width="120" /><el-table-column label="类型" width="110"
            ><template #default="{ row }">{{
              row.type === 'closed'
                ? '停诊'
                : row.type === 'replace'
                  ? '替换工作日'
                  : row.type === 'reduced'
                    ? '减量出诊'
                    : '临时加诊'
            }}</template></el-table-column
          ><el-table-column
            prop="replacementDailyTemplateName"
            label="替代日排班"
            min-width="150" /><el-table-column
            prop="reason"
            label="原因"
            min-width="180" /><el-table-column label="参与生成" width="90"
            ><template #default="{ row }">{{
              row.autoApplyOnGenerate ? '是' : '否'
            }}</template></el-table-column
          ><el-table-column label="操作" width="160"
            ><template #default="{ row }"
              ><RowActionGroup
                :row="row"
                :actions="rowActions()"
                @action="handleRowAction" /></template></el-table-column
        ></DataTablePanel>
      </div>
    </section>

    <el-dialog
      v-model="periodDialog"
      :title="periodId ? '编辑放号时段' : '新增放号时段'"
      width="620px"
      :close-on-click-modal="false"
      ><el-form label-width="90px"
        ><el-form-item label="名称" required><el-input v-model="periodForm.name" /></el-form-item
        ><el-form-item label="状态"
          ><el-radio-group v-model="periodForm.status"
            ><el-radio-button value="enabled">启用</el-radio-button
            ><el-radio-button value="disabled">停用</el-radio-button></el-radio-group
          ></el-form-item
        ><el-form-item label="时间段" required
          ><div class="segment-list">
            <div v-for="(segment, index) in periodForm.segments" :key="index" class="segment-row">
              <el-time-select v-model="segment.start" start="06:00" step="00:30" end="21:00" /><span
                >至</span
              ><el-time-select
                v-model="segment.end"
                start="06:30"
                step="00:30"
                end="22:00"
              /><el-input-number
                v-model="segment.capacity"
                :min="1"
                :max="999"
                controls-position="right"
              /><span class="field-unit">号</span
              ><el-button type="danger" link @click="removeSegment(index)">删除</el-button>
            </div>
            <el-button :icon="Plus" @click="addSegment">增加时间段</el-button>
          </div></el-form-item
        ></el-form
      ><template #footer
        ><el-button :disabled="saving" @click="periodDialog = false">取消</el-button
        ><el-button type="primary" :loading="saving" @click="savePeriodForm"
          >保存</el-button
        ></template
      ></el-dialog
    >
    <el-dialog
      v-model="dailyDialog"
      :title="dailyId ? '编辑日排班' : '新增日排班'"
      width="620px"
      :close-on-click-modal="false"
      ><el-form label-width="100px"
        ><el-form-item label="模板名称" required><el-input v-model="dailyForm.name" /></el-form-item
        ><el-form-item label="排班模式" required
          ><el-select v-model="dailyForm.scheduleMode"
            ><el-option label="医师门诊（MODE-A）" value="MODE-A" /><el-option
              label="科室号（MODE-B）"
              value="MODE-B" /><el-option
              label="诊区号（MODE-C）"
              value="MODE-C" /></el-select></el-form-item
        ><el-form-item label="服务类型" required
          ><el-select v-model="dailyForm.serviceType"
            ><el-option
              v-for="item in init?.serviceTypes"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="放号时段" required
          ><el-select v-model="dailyForm.periodIds" multiple
            ><el-option
              v-for="item in periodOptions"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="出诊人员" required
          ><el-select v-model="dailyForm.doctorId"
            ><el-option
              v-for="item in init?.doctors"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="地点" required
          ><el-select v-model="dailyForm.roomId"
            ><el-option
              v-for="item in init?.rooms"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="放号策略" required
          ><el-select v-model="dailyForm.releasePolicyId"
            ><el-option
              v-for="item in policyOptions"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="生效日期" required
          ><el-date-picker
            v-model="dailyForm.effectiveStart"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="开始日期" /><span class="date-separator">至</span
          ><el-date-picker
            v-model="dailyForm.effectiveEnd"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="结束日期" /></el-form-item
        ><el-form-item label="优先级"
          ><el-input-number v-model="dailyForm.priority" :min="0" :max="999" /></el-form-item
        ><el-form-item label="默认可预约"
          ><el-switch v-model="dailyForm.defaultAppointment" /></el-form-item
        ><el-form-item label="状态"
          ><el-radio-group v-model="dailyForm.status"
            ><el-radio-button value="enabled">启用</el-radio-button
            ><el-radio-button value="disabled">停用</el-radio-button></el-radio-group
          ></el-form-item
        ></el-form
      ><template #footer
        ><el-button @click="dailyDialog = false">取消</el-button
        ><el-button type="primary" :loading="saving" @click="saveDailyForm"
          >保存</el-button
        ></template
      ></el-dialog
    >
    <el-dialog
      v-model="weeklyDialog"
      :title="weeklyId ? '编辑周排班' : '新增周排班'"
      width="720px"
      :close-on-click-modal="false"
      ><el-form label-width="100px"
        ><el-form-item label="模板名称" required
          ><el-input v-model="weeklyForm.name" /></el-form-item
        ><el-form-item label="科室默认"><el-switch v-model="weeklyForm.isDefault" /></el-form-item
        ><el-form-item label="开放预约"
          ><el-switch v-model="weeklyForm.appointmentOpen" /></el-form-item
        ><el-form-item label="状态"
          ><el-radio-group v-model="weeklyForm.status"
            ><el-radio-button value="enabled">启用</el-radio-button
            ><el-radio-button value="disabled">停用</el-radio-button></el-radio-group
          ></el-form-item
        ><el-form-item label="周明细" required
          ><div class="weekly-list">
            <div v-for="(item, index) in weeklyForm.items" :key="index" class="weekly-row">
              <el-select v-model="item.weekday" style="width: 120px"
                ><el-option
                  v-for="weekday in WEEKDAYS"
                  :key="weekday.value"
                  :label="weekday.label"
                  :value="weekday.value" /></el-select
              ><el-select
                v-model="item.dailyTemplateId"
                placeholder="选择日排班"
                style="width: 280px"
                ><el-option
                  v-for="daily in dailyOptions"
                  :key="daily.value"
                  :label="daily.label"
                  :value="daily.value" /></el-select
              ><el-switch
                v-model="item.enabled"
                inline-prompt
                active-text="启"
                inactive-text="停"
              />
              <el-button type="danger" link @click="removeWeeklyItem(index)">删除</el-button>
            </div>
            <el-button :icon="Plus" @click="addWeeklyItem">增加周明细</el-button>
          </div></el-form-item
        ></el-form
      ><template #footer
        ><el-button @click="weeklyDialog = false">取消</el-button
        ><el-button type="primary" :loading="saving" @click="saveWeeklyForm"
          >保存</el-button
        ></template
      ></el-dialog
    >
    <el-dialog
      v-model="policyDialog"
      :title="policyId ? '编辑放号策略' : '新增放号策略'"
      width="560px"
      :close-on-click-modal="false"
    >
      <el-form label-width="110px">
        <el-form-item label="策略编码"><el-input v-model="policyForm.policyCode" /></el-form-item>
        <el-form-item label="策略名称" required
          ><el-input v-model="policyForm.policyName"
        /></el-form-item>
        <el-form-item label="提前天数" required
          ><el-input-number v-model="policyForm.advanceDays" :min="0" :max="365"
        /></el-form-item>
        <el-form-item label="时段分钟" required
          ><el-input-number v-model="policyForm.slotMinutes" :min="5" :max="240" :step="5"
        /></el-form-item>
        <el-form-item label="单段容量" required
          ><el-input-number v-model="policyForm.capacityPerSlot" :min="1" :max="999"
        /></el-form-item>
        <el-form-item label="渠道配额 JSON"
          ><el-input v-model="policyForm.channelQuotaJson" type="textarea" :rows="3"
        /></el-form-item>
      </el-form>
      <template #footer>
        <el-button :disabled="saving" @click="policyDialog = false">取消</el-button>
        <el-button type="primary" :loading="saving" @click="savePolicyForm">保存</el-button>
      </template>
    </el-dialog>
    <el-dialog
      v-model="holidayDialog"
      :title="holidayId ? '编辑节假日例外' : '新增节假日例外'"
      width="560px"
      :close-on-click-modal="false"
      ><el-form label-width="110px"
        ><el-form-item label="起始日" required
          ><el-date-picker
            v-model="holidayForm.date"
            type="date"
            value-format="YYYY-MM-DD"
            style="width: 100%"
            @change="handleHolidayStartChange" /></el-form-item
        ><el-form-item label="结束日"
          ><el-date-picker
            v-model="holidayForm.rangeEnd"
            type="date"
            value-format="YYYY-MM-DD"
            :disabled-date="disableHolidayEndDate"
            placeholder="空表示单日"
            style="width: 100%" /></el-form-item
        ><el-form-item label="例外类型" required
          ><el-select v-model="holidayForm.type"
            ><el-option label="停诊" value="closed" /><el-option
              label="替换工作日"
              value="replace" /><el-option label="减量出诊" value="reduced" /><el-option
              label="临时加诊"
              value="extra" /></el-select></el-form-item
        ><el-form-item v-if="holidayForm.type !== 'closed'" label="替代日排班" required
          ><el-select v-model="holidayForm.replacementDailyTemplateId"
            ><el-option
              v-for="item in dailyOptions"
              :key="item.value"
              :label="item.label"
              :value="item.value" /></el-select></el-form-item
        ><el-form-item label="说明"
          ><el-input
            v-model="holidayForm.reason"
            type="textarea"
            :rows="3"
            placeholder="国庆、调休等（可选）" /></el-form-item
        ><el-form-item label="参与生成"
          ><el-switch v-model="holidayForm.autoApplyOnGenerate" /></el-form-item></el-form
      ><template #footer
        ><el-button @click="holidayDialog = false">取消</el-button
        ><el-button type="primary" :loading="saving" @click="saveHolidayForm"
          >保存</el-button
        ></template
      ></el-dialog
    >
  </main>
</template>

<style scoped>
.template-page {
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
  min-width: 0;
  flex-direction: column;
  gap: 12px;
  padding: 16px;
}
.page-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
}
.page-heading h1 {
  margin: 0;
  font-size: 22px;
  letter-spacing: 0;
}
.page-heading p {
  margin: 5px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}
.template-tabs {
  padding: 0 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}
.policy-panel {
  padding: 20px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}
.policy-form {
  max-width: 560px;
}
.field-unit {
  margin-left: 8px;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}
.date-separator {
  margin: 0 8px;
  color: var(--el-text-color-secondary);
}
.holiday-layout {
  display: grid;
  grid-template-columns: minmax(320px, 420px) minmax(0, 1fr);
  gap: 14px;
  align-items: start;
}
.holiday-calendar {
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  color: #374151;
  background: #ffffff;
}
.holiday-calendar :deep(.el-calendar__title),
.holiday-calendar :deep(.el-calendar-table thead th) {
  color: #1f2937;
}
.holiday-calendar :deep(.el-calendar-table td),
.holiday-calendar :deep(.el-calendar-day) {
  color: #374151;
  background: #ffffff;
}
.holiday-calendar :deep(.el-calendar-table td.prev),
.holiday-calendar :deep(.el-calendar-table td.next) {
  color: #6b7280;
}
.holiday-calendar :deep(.el-calendar-table td.is-selected),
.holiday-calendar :deep(.el-calendar-table td.is-today) {
  color: #1d4ed8;
  background: #eff6ff;
}
.holiday-list {
  min-width: 0;
}
.segment-list {
  display: grid;
  width: 100%;
  gap: 10px;
}
.segment-row {
  display: flex;
  align-items: center;
  gap: 8px;
}
.segment-row :deep(.el-select) {
  flex: 1;
}
.weekly-list {
  display: grid;
  width: 100%;
  gap: 10px;
}
.weekly-row {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  align-items: center;
}
.holiday-date-cell {
  display: flex;
  align-items: center;
  justify-content: space-between;
  min-height: 42px;
  cursor: pointer;
  color: inherit;
}
.el-select {
  width: 100%;
}
@media (max-width: 900px) {
  .holiday-layout {
    grid-template-columns: 1fr;
  }
  .page-heading {
    align-items: flex-start;
    flex-direction: column;
  }
}
</style>
