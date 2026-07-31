<script setup lang="ts">
import type {
  BookingOption,
  GenerateFormalSchedulesPayload,
  ScheduleImpactPreview,
  ScheduleRecord
} from '@/apis/booking/outpatient-schedule'
import type { ActionToolbarItem } from '@/components/common/type'
import type {
  CalendarDateRange,
  CalendarDayCell,
  CalendarView,
  ScheduleEvent
} from '@/components/schedule/type'
import type { ScheduleQueryModel, ScheduleSavePayload } from './type'

import { computed, nextTick, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Plus, Refresh } from '@element-plus/icons-vue'

import {
  clearScheduleSubstitution,
  createSchedule,
  deleteReleasedSlots,
  generateFormalSchedules,
  getScheduleDetail,
  listReleasedSlots,
  loadDepartmentOptions,
  loadDoctorOptions,
  loadScheduleGenerationOptions,
  loadScheduleInit,
  pageSchedulesByRange,
  partialStopSchedule,
  previewScheduleImpact,
  provisionScheduleSlots,
  publishSchedule,
  resumeSchedule,
  setScheduleSubstitution,
  stopSchedule,
  unlockPartialStop,
  updateSchedule,
  voidSchedule
} from '@/apis/booking/outpatient-schedule'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import CalendarSchedule from '@/components/schedule/CalendarSchedule.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ScheduleDetailPanel from './components/ScheduleDetailPanel.vue'
import ScheduleEditDialog from './components/ScheduleEditDialog.vue'
import ScheduleGenerateDialog from './components/ScheduleGenerateDialog.vue'
import { toDoctorResources, toScheduleEvents } from './adapter'
import { scheduleStatusMap } from './constants'

defineOptions({ name: 'BookingOutpatientSchedule' })

function todayText() {
  const date = new Date()
  return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

const query = reactive<ScheduleQueryModel>({
  keyword: '',
  departmentId: null,
  doctorId: null,
  roomId: null
})
const departments = ref<BookingOption[]>([])
const doctors = ref<BookingOption[]>([])
const rooms = ref<BookingOption[]>([])
const organizationId = ref('')
const rows = ref<ScheduleRecord[]>([])
const anchorDate = ref(todayText())
const viewMode = ref<CalendarView>('week')
const currentRange = ref<CalendarDateRange>()
const loading = ref(false)
const initLoading = ref(false)
const initErrorMessage = ref('')
const listErrorMessage = ref('')
const saving = ref(false)
const actionLoading = ref(false)
const generationLoading = ref(false)
const detailLoading = ref(false)
const detailVisible = ref(false)
const selectedSchedule = ref<ScheduleRecord | null>(null)
const editDialog = ref<InstanceType<typeof ScheduleEditDialog>>()
const generationDialog = ref<InstanceType<typeof ScheduleGenerateDialog>>()
const dailyTemplateOptions = ref<BookingOption[]>([])
const weeklyTemplateOptions = ref<BookingOption[]>([])

let listController: AbortController | undefined
let listSequence = 0

const scheduleEvents = computed(() => toScheduleEvents(rows.value))
const doctorResources = computed(() => toDoctorResources(doctors.value))
const filteredDoctors = computed(() =>
  doctors.value.filter((item) => !query.departmentId || item.departmentId === query.departmentId)
)

const toolbarActions = computed<ActionToolbarItem[]>(() => [
  { key: 'create', label: '新增排班', type: 'primary', icon: Plus },
  { key: 'generate', label: '生成正式排班', type: 'success' },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])

async function initialize() {
  initLoading.value = true
  initErrorMessage.value = ''
  try {
    const result = await loadScheduleInit()
    organizationId.value = result.organizationId
    departments.value = result.departments
    doctors.value = result.doctors
    rooms.value = result.rooms
    return true
  } catch (error) {
    initErrorMessage.value = error instanceof Error ? error.message : '初始化排班页面失败'
    return false
  } finally {
    initLoading.value = false
  }
}

async function loadSchedules(range = currentRange.value) {
  if (!range) return
  listController?.abort()
  const controller = new AbortController()
  listController = controller
  const sequence = ++listSequence
  loading.value = true
  listErrorMessage.value = ''
  try {
    const result = await pageSchedulesByRange(
      {
        organizationId: organizationId.value,
        rangeStart: range.start,
        rangeEnd: range.end,
        practitionerId: query.doctorId,
        locationId: query.roomId,
        healthcareServiceId: query.departmentId
      },
      { signal: controller.signal }
    )
    if (sequence !== listSequence || controller.signal.aborted) return
    const keyword = query.keyword.trim().toLocaleLowerCase()
    const records = keyword
      ? result.records.filter((item) =>
          `${item.doctorName}${item.departmentName}${item.roomName}`
            .toLocaleLowerCase()
            .includes(keyword)
        )
      : result.records
    rows.value = records
    if (
      selectedSchedule.value &&
      !records.some((item) => item.id === selectedSchedule.value?.id)
    ) {
      selectedSchedule.value = null
      detailVisible.value = false
    }
  } catch (error) {
    if (controller.signal.aborted) return
    listErrorMessage.value = error instanceof Error ? error.message : '排班加载失败，请重试'
  } finally {
    if (sequence === listSequence) loading.value = false
  }
}

function handleRangeChange(range: CalendarDateRange) {
  currentRange.value = range
  void loadSchedules(range)
}

function resetQuery() {
  Object.assign(query, { keyword: '', departmentId: null, doctorId: null, roomId: null })
  void loadSchedules()
}

function handleDepartmentChange() {
  const doctor = doctors.value.find((item) => item.value === query.doctorId)
  if (doctor && doctor.departmentId !== query.departmentId) query.doctorId = null
}

function openCreateDialog(date = anchorDate.value) {
  editDialog.value?.open(undefined, date)
}

function handleDateClick(date: string, cell: CalendarDayCell) {
  if (cell.events.length === 0) openCreateDialog(date)
}

async function retryInitialize() {
  if (await initialize()) await loadSchedules()
}

async function openScheduleDetail(event: ScheduleEvent) {
  detailVisible.value = true
  detailLoading.value = true
  selectedSchedule.value = event.raw as ScheduleRecord
  try {
    selectedSchedule.value = await getScheduleDetail(String(event.key))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '排班详情加载失败')
    detailVisible.value = false
  } finally {
    detailLoading.value = false
  }
}

function editSchedule(record: ScheduleRecord) {
  detailVisible.value = false
  editDialog.value?.open(record)
}

async function handleSave(id: string | undefined, payload: ScheduleSavePayload) {
  if (saving.value) return
  saving.value = true
  try {
    const planningHorizonStart = `${payload.date}T${payload.startTime}:00`
    const planningHorizonEnd = `${payload.date}T${payload.endTime}:00`
    const actors = [
      { actorType: 10, actorId: payload.doctorId, sortNo: 1 },
      { actorType: 20, actorId: payload.roomId, sortNo: 2 },
      { actorType: 30, actorId: payload.departmentId, sortNo: 3 }
    ]
    let scheduleId = id
    if (id) {
      await updateSchedule({
        scheduleId: id,
        name: `${doctors.value.find((item) => item.value === payload.doctorId)?.label ?? '门诊'} ${payload.date}`,
        actors,
        planningHorizonStart,
        planningHorizonEnd
      })
    } else {
      const created = await createSchedule({
        organizationId: organizationId.value,
        name: `${doctors.value.find((item) => item.value === payload.doctorId)?.label ?? '门诊'} ${payload.date}`,
        actors,
        planningHorizonStart,
        planningHorizonEnd
      })
      scheduleId = created.scheduleId
    }
    if (!scheduleId) throw new Error('排班保存成功但未返回 scheduleId')
    await provisionScheduleSlots({
      scheduleId,
      organizationId: organizationId.value,
      slotTemplates: [
        { startTime: payload.startTime, endTime: payload.endTime, capacity: payload.capacity }
      ]
    })
    if (payload.publish)
      await publishSchedule({ scheduleId, planningHorizonStart, planningHorizonEnd })
    editDialog.value?.close()
    ElMessage.success(id ? '排班已更新' : '排班已新增')
    await loadSchedules()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '排班保存失败')
  } finally {
    saving.value = false
  }
}

async function handlePublish(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    await ElMessageBox.confirm(
      `确认发布 ${record.date} ${record.doctorName} 的排班？`,
      '发布排班',
      { type: 'warning' }
    )
  } catch {
    return
  }
  actionLoading.value = true
  try {
    await publishSchedule({
      scheduleId: record.id,
      planningHorizonStart: record.planningHorizonStart ?? `${record.date}T${record.startTime}:00`,
      planningHorizonEnd: record.planningHorizonEnd ?? `${record.date}T${record.endTime}:00`
    })
    selectedSchedule.value = await getScheduleDetail(record.id)
    ElMessage.success('排班已发布')
    await loadSchedules()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '发布失败')
  } finally {
    actionLoading.value = false
  }
}

async function handleStop(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    const impact = await previewScheduleImpact(record.id)
    const needsDisposition = impact.appointmentCount > 0 && impact.blockDeactivateIfUnhandled
    const disposition = needsDisposition ? await promptStopDisposition(impact) : null
    const { value: reason } = await ElMessageBox.prompt(
      `将停诊整个日程，影响 ${impact.slotCount} 个号源、${impact.appointmentCount} 个预约。请填写停诊原因。`,
      '整日停诊确认',
      {
      type: 'warning',
      inputPlaceholder: '请输入停诊原因',
      inputValidator: (value) => (value.trim() ? true : '停诊原因不能为空')
      }
    )
    actionLoading.value = true
    if (disposition) {
      await partialStopSchedule({
        organizationId: record.organizationId ?? organizationId.value,
        scheduleId: record.id,
        slotIds: [],
        reason: reason.trim(),
        ...disposition
      })
    } else {
      await stopSchedule({ scheduleId: record.id, reason: reason.trim() })
    }
    ElMessage.success('排班已停诊')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '停诊失败')
  } finally {
    actionLoading.value = false
  }
}

async function refreshSelected(scheduleId: string) {
  selectedSchedule.value = await getScheduleDetail(scheduleId)
  await loadSchedules()
}

async function openGenerateDialog() {
  const range = currentRange.value
  if (!range || generationLoading.value) return
  if (!organizationId.value) {
    ElMessage.warning('未取得当前机构，请刷新页面后重试')
    return
  }
  generationLoading.value = true
  try {
    const options = await loadScheduleGenerationOptions(organizationId.value)
    dailyTemplateOptions.value = options.dailyTemplates
    weeklyTemplateOptions.value = options.weeklyTemplates
    if (!options.dailyTemplates.length && !options.weeklyTemplates.length) {
      ElMessage.warning('没有可用的日排班或周排班，请先到“号源设置”中维护模板')
      return
    }
    await nextTick()
    generationDialog.value?.open(range)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '排班模板加载失败')
  } finally {
    generationLoading.value = false
  }
}

async function generateSchedules(
  payload: Omit<GenerateFormalSchedulesPayload, 'organizationId'>
) {
  if (generationLoading.value || !organizationId.value) return
  generationLoading.value = true
  try {
    const result = await generateFormalSchedules({
      organizationId: organizationId.value,
      ...payload
    })
    generationDialog.value?.close()
    Object.assign(query, { keyword: '', departmentId: null, doctorId: null, roomId: null })
    selectedSchedule.value = null
    detailVisible.value = false
    anchorDate.value = payload.rangeStart
    viewMode.value = 'week'
    await loadSchedules({ start: payload.rangeStart, end: payload.rangeEnd })
    let message = `已生成 ${result.generatedCount} 条日程`
    if (result.skippedCount) message += `，跳过 ${result.skippedCount} 天`
    if (result.provisionedSlotCount) message += `，放号 ${result.provisionedSlotCount} 个`
    if (result.generatedCount > 0 && !payload.autoPublish) message += '；请在日历中发布草稿'
    else if (result.generatedCount > 0 && !payload.autoProvision) message += '；请对已发布日程放号'
    ElMessage.success(message)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '生成正式排班失败')
  } finally {
    generationLoading.value = false
  }
}

async function setSubstitute(record: ScheduleRecord, doctorId: string) {
  if (actionLoading.value) return
  try {
    const { value } = await ElMessageBox.prompt('请输入替诊原因（可选）。', '设置替诊', {
      type: 'warning'
    })
    actionLoading.value = true
    await setScheduleSubstitution(record.id, doctorId, value)
    ElMessage.success('替诊设置成功')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '替诊设置失败')
  } finally {
    actionLoading.value = false
  }
}

async function clearSubstitute(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    await ElMessageBox.confirm('确认撤销当前替诊设置？', '撤销替诊', { type: 'warning' })
    actionLoading.value = true
    await clearScheduleSubstitution(record.id)
    ElMessage.success('替诊已撤销')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '撤销替诊失败')
  } finally {
    actionLoading.value = false
  }
}

async function promptStopDisposition(impact: ScheduleImpactPreview) {
  const { value } = await ElMessageBox.prompt(
    '请输入预约处置方式：MANUAL（人工处理）、CANCEL（取消预约）或 TRANSFER（逐条转移）。',
    '预约处置方式',
    {
      inputValue: impact.appointmentCount ? 'MANUAL' : 'CANCEL',
      inputValidator: (mode) =>
        ['MANUAL', 'CANCEL', 'TRANSFER'].includes(mode.trim().toUpperCase()) ||
        '请输入 MANUAL、CANCEL 或 TRANSFER'
    }
  )
  const dispositionMode = value.trim().toUpperCase() as 'TRANSFER' | 'CANCEL' | 'MANUAL'
  let transferItems: Array<{ appointmentId: string; targetSlotId: string }> | undefined
  if (dispositionMode === 'TRANSFER') {
    const { value: transferText } = await ElMessageBox.prompt(
      '逐行输入 appointmentId:targetSlotId，多个用逗号分隔。',
      '预约转移目标',
      {
        inputValue: impact.items
          .filter((item) => item.targetSlotId)
          .map((item) => `${item.appointmentId}:${item.targetSlotId}`)
          .join(','),
        inputValidator: (text) => Boolean(text.trim()) || '请填写逐条预约转移目标'
      }
    )
    transferItems = transferText.split(',').map((item) => {
      const [appointmentId, targetSlotId] = item.split(':').map((part) => part.trim())
      if (!appointmentId || !targetSlotId)
        throw new Error('预约转移格式应为 appointmentId:targetSlotId')
      return { appointmentId, targetSlotId }
    })
  }
  return {
    dispositionMode,
    transferItems,
    deferManualHandling: dispositionMode === 'MANUAL'
  }
}

async function partialStop(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    const [slots, impact] = await Promise.all([
      listReleasedSlots(record.id),
      previewScheduleImpact(record.id)
    ])
    const defaultIds = slots.content
      .filter((slot) => slot.statusCode === 'FREE')
      .map((slot) => slot.slotId)
    if (!defaultIds.length) throw new Error('当前排班没有可停诊号源')
    const { value } = await ElMessageBox.prompt(
      `影响预览：${impact.slotCount} 个号源、${impact.appointmentCount} 个预约。请输入需要停诊的号源 ID，多个用逗号分隔。`,
      '部分停诊',
      { type: 'warning', inputValue: defaultIds.join(',') }
    )
    const slotIds = value
      .split(',')
      .map((item) => item.trim())
      .filter(Boolean)
    const disposition = await promptStopDisposition(impact)
    actionLoading.value = true
    const result = await partialStopSchedule({
      organizationId: record.organizationId ?? organizationId.value,
      scheduleId: record.id,
      slotIds,
      ...disposition,
      reason: '部分停诊'
    })
    ElMessage.success(`已封锁 ${result.blockedSlotCount} 个号源`)
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '部分停诊失败')
  } finally {
    actionLoading.value = false
  }
}

async function unlockStop(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    await ElMessageBox.confirm('确认解锁该日程的部分停诊号源？', '解锁停诊', { type: 'warning' })
    actionLoading.value = true
    await unlockPartialStop(
      record.organizationId ?? organizationId.value,
      record.id,
      record.partialStoppedSlotIds
    )
    ElMessage.success('停诊号源已解锁')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '解锁停诊失败')
  } finally {
    actionLoading.value = false
  }
}

async function resume(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    await ElMessageBox.confirm('确认恢复该日程开诊？', '恢复开诊', { type: 'warning' })
    actionLoading.value = true
    const result = await resumeSchedule(record.id)
    ElMessage.success(result.needReprovision ? '已恢复开诊，请核对并重新放号' : '已恢复开诊')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '恢复开诊失败')
  } finally {
    actionLoading.value = false
  }
}

async function provision(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    await ElMessageBox.confirm(`按 ${record.startTime}-${record.endTime} 生成号源？`, '批量放号', {
      type: 'warning'
    })
    actionLoading.value = true
    const result = await provisionScheduleSlots({
      scheduleId: record.id,
      organizationId: record.organizationId ?? organizationId.value,
      slotTemplates: [
        { startTime: record.startTime, endTime: record.endTime, capacity: record.capacity }
      ]
    })
    ElMessage.success(`已生成 ${result.generatedCount} 个号源`)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '批量放号失败')
  } finally {
    actionLoading.value = false
  }
}

async function deleteSlots(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    const result = await listReleasedSlots(record.id)
    if (!result.content.length) throw new Error('当前日程没有已放号源')
    await ElMessageBox.confirm(`确认删除 ${result.content.length} 个未使用号源？`, '删除号源', {
      type: 'warning'
    })
    actionLoading.value = true
    await deleteReleasedSlots(result.content.map((slot) => slot.slotId))
    ElMessage.success('号源已删除')
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '删除号源失败')
  } finally {
    actionLoading.value = false
  }
}

async function voidRecord(record: ScheduleRecord) {
  if (actionLoading.value) return
  try {
    const { value } = await ElMessageBox.prompt('请输入作废原因。', '作废日程', {
      type: 'warning',
      inputValidator: (reason) => Boolean(reason.trim()) || '请输入作废原因'
    })
    actionLoading.value = true
    await voidSchedule(record.id, value)
    ElMessage.success('日程已作废')
    await refreshSelected(record.id)
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '作废日程失败')
  } finally {
    actionLoading.value = false
  }
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') openCreateDialog()
  if (action.key === 'generate') void openGenerateDialog()
  if (action.key === 'refresh') void loadSchedules()
}

onMounted(() => void initialize())
onBeforeUnmount(() => listController?.abort())
</script>

<template>
  <main class="schedule-page" v-loading="initLoading">
    <SearchPanel
      v-model:keyword="query.keyword"
      keyword-label="快速检索"
      keyword-placeholder="医生、科室或诊室"
      :field-count="3"
      :loading="loading"
      @search="loadSchedules()"
      @reset="resetQuery"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">科室</label>
          <RemoteSelectField
            v-model="query.departmentId"
            :load-options="loadDepartmentOptions"
            :initial-options="departments"
            value-key="value"
            label-key="label"
            placeholder="选择科室"
            @change="handleDepartmentChange"
          />
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">医生</label>
          <RemoteSelectField
            v-model="query.doctorId"
            :load-options="
              (keyword, context) => loadDoctorOptions(keyword, query.departmentId, context)
            "
            :initial-options="filteredDoctors"
            value-key="value"
            label-key="label"
            placeholder="选择医生"
          />
        </div>
        <div class="query-shell__field">
          <label class="query-shell__label">诊室</label>
          <el-select v-model="query.roomId" clearable placeholder="选择诊室" style="width: 100%">
            <el-option
              v-for="room in rooms"
              :key="room.value"
              :label="room.label"
              :value="room.value"
            />
          </el-select>
        </div>
      </template>
    </SearchPanel>

    <ActionToolbar
      title="门诊排班"
      description="按当前日历范围加载排班，点击日期可新增"
      :actions="toolbarActions"
      @action="handleToolbarAction"
    />

    <el-alert
      v-if="initErrorMessage"
      :title="initErrorMessage"
      type="error"
      show-icon
      :closable="false"
    >
      <template #default
        ><el-button type="primary" link @click="retryInitialize">重新初始化</el-button></template
      >
    </el-alert>
    <el-alert
      v-if="listErrorMessage"
      :title="listErrorMessage"
      type="error"
      show-icon
      :closable="false"
    >
      <template #default
        ><el-button type="primary" link @click="loadSchedules()">重新加载排班</el-button></template
      >
    </el-alert>

    <section class="schedule-page__calendar">
      <CalendarSchedule
        v-model="anchorDate"
        v-model:view="viewMode"
        :events="scheduleEvents"
        :resources="doctorResources"
        :status-map="scheduleStatusMap"
        :loading="loading"
        :first-day-of-week="1"
        empty-text="当前范围暂无排班，可点击日期新建"
        @range-change="handleRangeChange"
        @date-click="handleDateClick"
        @slot-click="openCreateDialog"
        @event-click="openScheduleDetail"
      />
    </section>

    <ScheduleEditDialog
      ref="editDialog"
      :departments="departments"
      :doctors="doctors"
      :rooms="rooms"
      :saving="saving"
      :load-departments="loadDepartmentOptions"
      :load-doctors="
        (keyword, context, departmentId) => loadDoctorOptions(keyword, departmentId, context)
      "
      @submit="handleSave"
    />
    <ScheduleGenerateDialog
      ref="generationDialog"
      :daily-templates="dailyTemplateOptions"
      :weekly-templates="weeklyTemplateOptions"
      :submitting="generationLoading"
      @submit="generateSchedules"
    />
    <ScheduleDetailPanel
      v-model="detailVisible"
      :data="selectedSchedule"
      :loading="detailLoading"
      :action-loading="actionLoading"
      :doctors="doctors"
      @edit="editSchedule"
      @publish="handlePublish"
      @stop="handleStop"
      @substitute="setSubstitute"
      @clear-substitute="clearSubstitute"
      @partial-stop="partialStop"
      @unlock="unlockStop"
      @resume="resume"
      @provision="provision"
      @delete-slots="deleteSlots"
      @void="voidRecord"
    />
  </main>
</template>

<style scoped>
.schedule-page {
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
.schedule-page__calendar {
  min-width: 0;
  padding: 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}
.schedule-page__calendar :deep(.calendar-schedule__event-time),
.schedule-page__calendar :deep(.calendar-schedule__event-status),
.schedule-page__calendar :deep(.calendar-schedule__today-tag) {
  font-size: 12px;
}
@media (max-width: 768px) {
  .schedule-page {
    padding: 10px;
  }
  .schedule-page__calendar {
    padding: 10px;
    overflow-x: auto;
  }
  .schedule-page__calendar :deep(.calendar-schedule),
  .schedule-page__calendar :deep(.calendar-schedule__grid) {
    min-width: 350px;
  }
  .schedule-page__calendar :deep(.calendar-schedule__week-label) {
    padding-inline: 4px;
    white-space: nowrap;
  }
}
</style>
