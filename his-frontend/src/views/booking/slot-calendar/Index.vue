<script setup lang="ts">
import type { BookingOption, SlotRecord } from '@/apis/booking/slot-calendar'
import type { TimeSlot, TimeSlotStatusMap } from '@/components/schedule/type'
import type { ActionToolbarItem } from '@/components/common/type'
import type { SlotQueryModel } from './type'

import { computed, nextTick, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Lock, Refresh, Unlock, Warning } from '@element-plus/icons-vue'

import {
  batchBlockSlots,
  batchUnblockSlots,
  getSlotDetail,
  loadDepartmentOptions,
  loadDoctorOptions,
  loadSlotInit,
  markSlotOverbooked,
  pageSlotsByDate
} from '@/apis/booking/slot-calendar'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import TimeSlotGrid from '@/components/schedule/TimeSlotGrid.vue'
import SlotDetailDialog from './components/SlotDetailDialog.vue'
import { toTimeSlots } from './adapter'

defineOptions({ name: 'BookingSlotCalendar' })

function todayText() {
  const date = new Date()
  return `${date.getFullYear()}-${String(date.getMonth() + 1).padStart(2, '0')}-${String(date.getDate()).padStart(2, '0')}`
}

const query = reactive<SlotQueryModel>({
  date: todayText(),
  keyword: '',
  departmentId: null,
  doctorId: null,
  pageNo: 1,
  pageSize: 100
})
const departments = ref<BookingOption[]>([])
const doctors = ref<BookingOption[]>([])
const organizationId = ref('')
const rows = ref<SlotRecord[]>([])
const loading = ref(false)
const initLoading = ref(false)
const detailLoading = ref(false)
const initErrorMessage = ref('')
const listErrorMessage = ref('')
const selectedSlotKey = ref<string | number | null>(null)
const selectedSlot = ref<SlotRecord | null>(null)
const selectedSlotIds = ref<string[]>([])
const total = ref(0)
const actionLoading = ref(false)
const detailDialog = ref<InstanceType<typeof SlotDetailDialog>>()
let controller: AbortController | undefined
let requestSequence = 0
let detailController: AbortController | undefined
let detailSequence = 0

const timeSlots = computed(() => toTimeSlots(rows.value))
const filteredDoctors = computed(() =>
  doctors.value.filter((item) => !query.departmentId || item.departmentId === query.departmentId)
)
const summary = computed(() =>
  rows.value.reduce(
    (total, row) => ({
      capacity: total.capacity + row.capacity,
      used: total.used + row.used,
      remaining: total.remaining + row.remaining,
      stopped: total.stopped + (row.status === 'stopped' ? 1 : 0)
    }),
    { capacity: 0, used: 0, remaining: 0, stopped: 0 }
  )
)
const toolbarActions = computed<ActionToolbarItem[]>(() => [
  {
    key: 'block',
    label: '批量封锁',
    type: 'warning',
    icon: Lock,
    disabled: selectedSlotIds.value.length === 0
  },
  {
    key: 'unblock',
    label: '批量解封',
    type: 'success',
    icon: Unlock,
    disabled: selectedSlotIds.value.length === 0
  },
  {
    key: 'overbook',
    label: '标记超售',
    type: 'danger',
    icon: Warning,
    disabled: selectedSlotIds.value.length !== 1
  },
  { key: 'refresh', label: '刷新', icon: Refresh, loading: loading.value }
])

const slotStatusMap: TimeSlotStatusMap = {
  available: { text: '可约', type: 'success', selectable: true },
  full: { text: '已满', type: 'danger', selectable: false },
  stopped: { text: '停诊', type: 'warning', selectable: false },
  blocked: { text: '已封锁', type: 'warning', selectable: false },
  expired: { text: '过期', type: 'info', selectable: false }
}

async function initialize() {
  initLoading.value = true
  initErrorMessage.value = ''
  try {
    const result = await loadSlotInit()
    organizationId.value = result.organizationId
    query.departmentId = query.departmentId ?? result.departments[0]?.value ?? null
    departments.value = result.departments
    doctors.value = result.doctors
    await loadSlots()
  } catch (error) {
    initErrorMessage.value = error instanceof Error ? error.message : '号源页面初始化失败'
  } finally {
    initLoading.value = false
  }
}

async function loadSlots() {
  controller?.abort()
  detailController?.abort()
  detailSequence += 1
  detailDialog.value?.close()
  detailLoading.value = false
  selectedSlotKey.value = null
  selectedSlot.value = null
  selectedSlotIds.value = []
  if (!query.date) {
    rows.value = []
    total.value = 0
    listErrorMessage.value = '请选择查询日期'
    return
  }
  const currentController = new AbortController()
  controller = currentController
  const sequence = ++requestSequence
  loading.value = true
  listErrorMessage.value = ''
  try {
    const request = {
      organizationId: query.departmentId ?? organizationId.value,
      date: query.date,
      practitionerId: query.doctorId,
      PageIndex: query.keyword.trim() ? 1 : query.pageNo,
      PageSize: query.pageSize
    }
    const firstPage = await pageSlotsByDate(request, { signal: currentController.signal })
    if (sequence !== requestSequence || currentController.signal.aborted) return
    const keyword = query.keyword.trim().toLocaleLowerCase()
    if (!keyword) {
      rows.value = firstPage.content
      total.value = firstPage.totalCount
      return
    }
    const pageCount = Math.ceil(firstPage.totalCount / query.pageSize)
    const remainingPages = await Promise.all(
      Array.from({ length: Math.max(0, pageCount - 1) }, (_, index) =>
        pageSlotsByDate({ ...request, PageIndex: index + 2 }, { signal: currentController.signal })
      )
    )
    if (sequence !== requestSequence || currentController.signal.aborted) return
    const filtered = [firstPage, ...remainingPages]
      .flatMap((page) => page.content)
      .filter((item) =>
        `${item.doctorName}${item.departmentName}${item.roomName}${item.startTime}`
          .toLocaleLowerCase()
          .includes(keyword)
      )
    const start = (query.pageNo - 1) * query.pageSize
    rows.value = filtered.slice(start, start + query.pageSize)
    total.value = filtered.length
  } catch (error) {
    if (currentController.signal.aborted) return
    rows.value = []
    total.value = 0
    listErrorMessage.value = error instanceof Error ? error.message : '号源加载失败，请重试'
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}

function resetQuery() {
  Object.assign(query, {
    date: todayText(),
    keyword: '',
    departmentId: departments.value[0]?.value ?? null,
    doctorId: null,
    pageNo: 1
  })
  void loadSlots()
}

function submitQuery() {
  query.pageNo = 1
  void loadSlots()
}

function handleDepartmentChange() {
  const doctor = doctors.value.find((item) => item.value === query.doctorId)
  if (doctor && doctor.departmentId !== query.departmentId) query.doctorId = null
  query.pageNo = 1
}

function handlePaginationChange(payload: { page: number; pageSize: number }) {
  const pageSizeChanged = payload.pageSize !== query.pageSize
  query.pageNo = pageSizeChanged ? 1 : payload.page
  query.pageSize = payload.pageSize
  void loadSlots()
}

async function openSlotDetail(slot: TimeSlot) {
  detailController?.abort()
  const currentController = new AbortController()
  detailController = currentController
  const sequence = ++detailSequence
  detailLoading.value = true
  selectedSlot.value = slot.raw as SlotRecord
  await nextTick()
  detailDialog.value?.open()
  try {
    const result = await getSlotDetail(
      { slotId: String(slot.key) },
      { signal: currentController.signal }
    )
    if (sequence !== detailSequence || currentController.signal.aborted) return
    selectedSlot.value = result
  } catch (error) {
    if (currentController.signal.aborted) return
    ElMessage.error(error instanceof Error ? error.message : '号源详情加载失败')
    detailDialog.value?.close()
  } finally {
    if (sequence === detailSequence) detailLoading.value = false
  }
}

function showUnavailableReason(slot: TimeSlot) {
  const row = slot.raw as SlotRecord
  ElMessage.warning(row.unavailableReason || '该号源当前不可预约')
}

async function blockSelected() {
  if (!selectedSlotIds.value.length || actionLoading.value) return
  const slotIds = [...selectedSlotIds.value]
  actionLoading.value = true
  try {
    const { value } = await ElMessageBox.prompt(
      '封锁后号源不可预约，请填写原因。',
      '批量封锁确认',
      {
        type: 'warning',
        inputValidator: (reason) => Boolean(reason.trim()) || '请输入封锁原因'
      }
    )
    await batchBlockSlots({ slotIds, reason: value })
    ElMessage.success('号源已封锁')
    await loadSlots()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '批量封锁失败')
  } finally {
    actionLoading.value = false
  }
}

async function unblockSelected() {
  if (!selectedSlotIds.value.length || actionLoading.value) return
  const slotIds = [...selectedSlotIds.value]
  actionLoading.value = true
  try {
    await ElMessageBox.confirm(
      '确认解封所选号源？排班停诊产生的不可用号源不会恢复。',
      '批量解封确认',
      { type: 'warning' }
    )
    await batchUnblockSlots({ slotIds })
    ElMessage.success('号源已解封')
    await loadSlots()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '批量解封失败')
  } finally {
    actionLoading.value = false
  }
}

async function toggleOverbooked() {
  const id = selectedSlotIds.value[0]
  const row = rows.value.find((item) => item.slotId === id)
  if (!id || !row || actionLoading.value) return
  const next = !row.overbookedFlag
  actionLoading.value = true
  try {
    await ElMessageBox.confirm(`确认${next ? '标记' : '取消'}该号源的超售状态？`, '超售标记确认', {
      type: 'warning'
    })
    await markSlotOverbooked({ slotId: id, overbookedFlag: next })
    ElMessage.success(next ? '已标记超售' : '已取消超售标记')
    await loadSlots()
  } catch (error) {
    if (error === 'cancel' || error === 'close') return
    ElMessage.error(error instanceof Error ? error.message : '超售标记失败')
  } finally {
    actionLoading.value = false
  }
}

function handleToolbar(action: ActionToolbarItem) {
  if (action.key === 'block') void blockSelected()
  else if (action.key === 'unblock') void unblockSelected()
  else if (action.key === 'overbook') void toggleOverbooked()
  else void loadSlots()
}

onMounted(() => void initialize())
onBeforeUnmount(() => {
  controller?.abort()
  detailController?.abort()
})
</script>

<template>
  <main class="slot-page" v-loading="initLoading">
    <SearchPanel
      v-model:keyword="query.keyword"
      keyword-label="快速检索"
      keyword-placeholder="医生、科室、诊室或时间"
      :field-count="3"
      :loading="loading"
      @search="submitQuery"
      @reset="resetQuery"
    >
      <template #header>
        <div class="query-shell__field">
          <label class="query-shell__label">日期</label>
          <el-date-picker
            v-model="query.date"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="选择日期"
            style="width: 100%"
          />
        </div>
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
      </template>
    </SearchPanel>

    <el-alert
      v-if="initErrorMessage"
      :title="initErrorMessage"
      type="error"
      show-icon
      :closable="false"
    >
      <template #default
        ><el-button type="primary" link @click="initialize">重新初始化</el-button></template
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
        ><el-button type="primary" link @click="loadSlots">重新加载号源</el-button></template
      >
    </el-alert>

    <section class="slot-page__workspace">
      <header class="slot-page__header">
        <div>
          <h2>{{ query.date }} 号源</h2>
          <p>按上午、下午和晚间查看各时段余号</p>
        </div>
        <div class="slot-page__legend">
          <el-tag type="success" effect="plain">可约</el-tag>
          <el-tag type="danger" effect="plain">已满</el-tag>
          <el-tag type="warning" effect="plain">停诊</el-tag>
          <el-tag type="warning" effect="dark">已封锁</el-tag>
          <el-tag type="info" effect="plain">过期</el-tag>
        </div>
      </header>

      <div class="slot-page__summary">
        <div>
          <span>本页总号数</span><strong>{{ summary.capacity }}</strong>
        </div>
        <div>
          <span>本页已用号数</span><strong>{{ summary.used }}</strong>
        </div>
        <div>
          <span>本页剩余号数</span><strong>{{ summary.remaining }}</strong>
        </div>
        <div>
          <span>本页停诊时段</span><strong>{{ summary.stopped }}</strong>
        </div>
      </div>

      <ActionToolbar
        title="号源状态维护"
        description="选择一个或多个号源后执行封锁、解封；超售标记仅支持单个号源"
        :selection-count="selectedSlotIds.length"
        :actions="toolbarActions"
        :disabled="actionLoading"
        @action="handleToolbar"
      />
      <el-select
        v-model="selectedSlotIds"
        multiple
        collapse-tags
        clearable
        placeholder="选择需要维护状态的号源"
        :disabled="loading || actionLoading"
        style="width: 100%"
      >
        <el-option
          v-for="row in rows"
          :key="row.slotId"
          :label="`${row.startTime}-${row.endTime} ${row.doctorName}（${row.displayStatus}${row.overbookedFlag ? '，超售' : ''}）`"
          :value="row.slotId"
        />
      </el-select>

      <TimeSlotGrid
        v-model="selectedSlotKey"
        :slots="timeSlots"
        :status-map="slotStatusMap"
        :loading="loading"
        group-by="group"
        empty-text="当前条件下暂无号源"
        @select="openSlotDetail"
        @disabled-click="showUnavailableReason"
      />
      <PaginationBar
        v-if="total > 0"
        class="slot-pagination"
        :page="query.pageNo"
        :page-size="query.pageSize"
        :total="total"
        :disabled="loading"
        @change="handlePaginationChange"
      />
    </section>

    <SlotDetailDialog ref="detailDialog" :data="selectedSlot" :loading="detailLoading" />
  </main>
</template>

<style scoped>
.slot-page {
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
.slot-page__workspace {
  --el-bg-color: #ffffff;
  --el-fill-color-blank: #ffffff;
  --el-fill-color-light: #f5f7fa;
  --el-fill-color-lighter: #fafafa;
  --el-border-color: #dcdfe6;
  --el-border-color-light: #e4e7ed;
  --el-border-color-lighter: #ebeef5;
  --el-text-color-primary: #1f2937;
  --el-text-color-regular: #374151;
  --el-text-color-secondary: #4b5563;
  --el-text-color-placeholder: #6b7280;

  display: flex;
  min-height: 360px;
  flex-direction: column;
  gap: 18px;
  padding: 18px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  color: #303133;
  background: #ffffff;
}
.slot-pagination {
  justify-content: flex-end;
}
.slot-page__header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 16px;
}
.slot-page__header h2 {
  margin: 0;
  color: #303133;
  font-size: 18px;
  letter-spacing: 0;
}
.slot-page__header p {
  margin: 6px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}
.slot-page__legend {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}
.slot-page__summary {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 6px;
  background: var(--el-fill-color-blank);
}
.slot-page__summary > div {
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  gap: 8px;
  padding: 14px 16px;
  border-right: 1px solid var(--el-border-color-lighter);
}
.slot-page__summary > div:last-child {
  border-right: 0;
}
.slot-page__summary span {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}
.slot-page__summary strong {
  color: #303133;
  font-size: 22px;
  letter-spacing: 0;
}
.slot-page :deep(.time-slot-grid__status) {
  font-size: 13px;
}
@media (max-width: 768px) {
  .slot-page {
    padding: 10px;
  }
  .slot-page__header {
    flex-direction: column;
  }
  .slot-page__summary {
    grid-template-columns: repeat(2, 1fr);
  }
  .slot-page__summary > div:nth-child(2) {
    border-right: 0;
  }
  .slot-page :deep(.action-toolbar__title) {
    overflow: visible;
    white-space: normal;
  }
}
</style>
