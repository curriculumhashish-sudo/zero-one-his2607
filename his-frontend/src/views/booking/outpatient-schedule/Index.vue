<script setup lang="ts">
import type {
  BookingOption,
  ScheduleRecord,
  ScheduleSavePayload
} from '@/apis/booking/outpatient-schedule'
import type { ActionToolbarItem } from '@/components/common/type'
import type {
  CalendarDateRange,
  CalendarDayCell,
  CalendarView,
  ScheduleEvent
} from '@/components/schedule/type'
import type { ScheduleQueryModel } from './type'

import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Plus, Refresh } from '@element-plus/icons-vue'

import {
  createSchedule,
  getScheduleDetail,
  loadDepartmentOptions,
  loadDoctorOptions,
  loadScheduleInit,
  pageSchedulesByRange,
  publishSchedule,
  stopSchedule,
  updateSchedule
} from '@/apis/booking/outpatient-schedule'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import CalendarSchedule from '@/components/schedule/CalendarSchedule.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import ScheduleDetailPanel from './components/ScheduleDetailPanel.vue'
import ScheduleEditDialog from './components/ScheduleEditDialog.vue'
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
const detailLoading = ref(false)
const detailVisible = ref(false)
const selectedSchedule = ref<ScheduleRecord | null>(null)
const editDialog = ref<InstanceType<typeof ScheduleEditDialog>>()

let listController: AbortController | undefined
let listSequence = 0

const scheduleEvents = computed(() => toScheduleEvents(rows.value))
const doctorResources = computed(() => toDoctorResources(doctors.value))
const filteredDoctors = computed(() =>
  doctors.value.filter((item) => !query.departmentId || item.departmentId === query.departmentId)
)

const toolbarActions = computed<ActionToolbarItem[]>(() => [
  { key: 'create', label: '新增排班', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])

async function initialize() {
  initLoading.value = true
  initErrorMessage.value = ''
  try {
    const result = await loadScheduleInit()
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
        start: range.start,
        end: range.end,
        keyword: query.keyword,
        departmentId: query.departmentId,
        doctorId: query.doctorId,
        roomId: query.roomId
      },
      { signal: controller.signal }
    )
    if (sequence !== listSequence || controller.signal.aborted) return
    rows.value = result
    if (selectedSchedule.value && !result.some((item) => item.id === selectedSchedule.value?.id)) {
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
    if (id) await updateSchedule(id, payload)
    else await createSchedule(payload)
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
    selectedSchedule.value = await publishSchedule(record.id)
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
  let reason = ''
  try {
    const result = await ElMessageBox.prompt('停诊后对应号源将不可预约，请填写原因。', '停诊确认', {
      type: 'warning',
      inputPlaceholder: '请输入停诊原因',
      inputValidator: (value) => (value.trim() ? true : '停诊原因不能为空')
    })
    reason = result.value
  } catch {
    return
  }
  actionLoading.value = true
  try {
    selectedSchedule.value = await stopSchedule(record.id, reason)
    ElMessage.success('排班已停诊')
    await loadSchedules()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '停诊失败')
  } finally {
    actionLoading.value = false
  }
}

function handleToolbarAction(action: ActionToolbarItem) {
  if (action.key === 'create') openCreateDialog()
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
    <ScheduleDetailPanel
      v-model="detailVisible"
      :data="selectedSchedule"
      :loading="detailLoading"
      :action-loading="actionLoading"
      @edit="editSchedule"
      @publish="handlePublish"
      @stop="handleStop"
    />
  </main>
</template>

<style scoped>
.schedule-page {
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
@media (max-width: 768px) {
  .schedule-page {
    padding: 10px;
  }
  .schedule-page__calendar {
    padding: 10px;
  }
}
</style>
