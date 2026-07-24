<script setup lang="ts">
import type { BookingOption, SlotRecord } from '@/apis/booking/slot-calendar'
import type { TimeSlot, TimeSlotStatusMap } from '@/components/schedule/type'
import type { SlotQueryModel } from './type'

import { computed, nextTick, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'

import {
  getSlotDetail,
  loadDepartmentOptions,
  loadDoctorOptions,
  loadSlotInit,
  pageSlotsByDate
} from '@/apis/booking/slot-calendar'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
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
  doctorId: null
})
const departments = ref<BookingOption[]>([])
const doctors = ref<BookingOption[]>([])
const rows = ref<SlotRecord[]>([])
const loading = ref(false)
const initLoading = ref(false)
const detailLoading = ref(false)
const initErrorMessage = ref('')
const listErrorMessage = ref('')
const selectedSlotKey = ref<string | number | null>(null)
const selectedSlot = ref<SlotRecord | null>(null)
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

const slotStatusMap: TimeSlotStatusMap = {
  available: { text: '可约', type: 'success', selectable: true },
  full: { text: '已满', type: 'danger', selectable: false },
  stopped: { text: '停诊', type: 'warning', selectable: false },
  expired: { text: '过期', type: 'info', selectable: false }
}

async function initialize() {
  initLoading.value = true
  initErrorMessage.value = ''
  try {
    const result = await loadSlotInit()
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
  if (!query.date) {
    rows.value = []
    listErrorMessage.value = '请选择查询日期'
    return
  }
  const currentController = new AbortController()
  controller = currentController
  const sequence = ++requestSequence
  loading.value = true
  listErrorMessage.value = ''
  try {
    const result = await pageSlotsByDate({ ...query }, { signal: currentController.signal })
    if (sequence !== requestSequence || currentController.signal.aborted) return
    rows.value = result
  } catch (error) {
    if (currentController.signal.aborted) return
    listErrorMessage.value = error instanceof Error ? error.message : '号源加载失败，请重试'
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}

function resetQuery() {
  Object.assign(query, { date: todayText(), keyword: '', departmentId: null, doctorId: null })
  void loadSlots()
}

function handleDepartmentChange() {
  const doctor = doctors.value.find((item) => item.value === query.doctorId)
  if (doctor && doctor.departmentId !== query.departmentId) query.doctorId = null
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
    const result = await getSlotDetail(String(slot.key), { signal: currentController.signal })
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
      @search="loadSlots"
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
          <el-tag type="info" effect="plain">过期</el-tag>
        </div>
      </header>

      <div class="slot-page__summary">
        <div>
          <span>总号数</span><strong>{{ summary.capacity }}</strong>
        </div>
        <div>
          <span>已用号数</span><strong>{{ summary.used }}</strong>
        </div>
        <div>
          <span>剩余号数</span><strong>{{ summary.remaining }}</strong>
        </div>
        <div>
          <span>停诊时段</span><strong>{{ summary.stopped }}</strong>
        </div>
      </div>

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
    </section>

    <SlotDetailDialog ref="detailDialog" :data="selectedSlot" :loading="detailLoading" />
  </main>
</template>

<style scoped>
.slot-page {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 12px;
  padding: 16px;
}
.slot-page__workspace {
  display: flex;
  min-height: 360px;
  flex-direction: column;
  gap: 18px;
  padding: 18px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}
.slot-page__header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 16px;
}
.slot-page__header h2 {
  margin: 0;
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
  font-size: 22px;
  letter-spacing: 0;
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
}
</style>
