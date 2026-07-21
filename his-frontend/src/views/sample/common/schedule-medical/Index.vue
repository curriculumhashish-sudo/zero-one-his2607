<template>
  <div class="e-demo-page">
    <header class="demo-header">
      <div>
        <h2>E组组件交互示例（排班预约 · 医嘱展示）</h2>
        <p>展示 C28 CalendarSchedule、C29 TimeSlotGrid、C54 AdviceOrderTable、C55 AdviceOrderDisplayCell。示例数据仅用于验证组件展示与事件联动。</p>
      </div>
    </header>

    <div class="component-overview">
      <div v-for="item in overview" :key="item.code" class="overview-item">
        <span class="overview-code">{{ item.code }}</span>
        <strong>{{ item.name }}</strong>
        <span>{{ item.description }}</span>
      </div>
    </div>

    <el-tabs v-model="activeTab" class="demo-tabs">
      <!-- C28 -->
      <el-tab-pane label="C28 排班日历" name="calendar">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>月/周排班日历</h3>
              <p>受控 modelValue + view；切换范围仅发出 rangeChange，事件按状态着色，支持折叠与只读。</p>
            </div>
            <el-tag effect="plain">CalendarSchedule</el-tag>
          </div>

          <div class="demo-controls">
            <el-radio-group v-model="calendarDataset" size="small">
              <el-radio-button value="clinic">门诊排班</el-radio-button>
              <el-radio-button value="exam">体检排班</el-radio-button>
            </el-radio-group>
            <el-switch v-model="calendarLoading" active-text="加载态" inline-prompt />
            <el-switch v-model="calendarReadonly" active-text="只读" inline-prompt />
            <el-switch v-model="calendarEmpty" active-text="空数据" inline-prompt />
            <span class="demo-hint">当前范围：{{ calendarRange.start }} ~ {{ calendarRange.end }}</span>
          </div>

          <CalendarSchedule
            v-model="calendarDate"
            v-model:view="calendarView"
            :events="calendarEmpty ? [] : currentCalendarEvents"
            :resources="calendarResources"
            :status-map="calendarStatusMap"
            :loading="calendarLoading"
            :readonly="calendarReadonly"
            @range-change="calendarRange = $event"
            @event-click="onEventClick"
            @date-click="onDateClick"
          />
          <p v-if="calendarLog" class="demo-log">{{ calendarLog }}</p>
        </section>
      </el-tab-pane>

      <!-- C29 -->
      <el-tab-pane label="C29 号源网格" name="slot">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>号源/时段选择</h3>
              <p>受控单选；可选性由剩余量与 statusMap.selectable 决定，不可选项仍发 disabledClick。</p>
            </div>
            <el-tag effect="plain">TimeSlotGrid</el-tag>
          </div>

          <div class="demo-controls">
            <el-radio-group v-model="slotDataset" size="small">
              <el-radio-button value="morning">上午/下午分组</el-radio-button>
              <el-radio-button value="expert">专家号（混合状态）</el-radio-button>
            </el-radio-group>
            <el-switch v-model="slotLoading" active-text="加载态" inline-prompt />
            <el-switch v-model="slotReadonly" active-text="只读" inline-prompt />
            <span class="demo-hint">已选：{{ selectedSlotLabel }}</span>
          </div>

          <TimeSlotGrid
            v-model="selectedSlot"
            :slots="slotLoading ? [] : currentSlots"
            group-by="group"
            :loading="slotLoading"
            :readonly="slotReadonly"
            @select="onSlotSelect"
            @disabled-click="onSlotDisabled"
          />
          <p v-if="slotLog" class="demo-log">{{ slotLog }}</p>
        </section>
      </el-tab-pane>

      <!-- C54 + C55 -->
      <el-tab-pane label="C54/C55 医嘱表格" name="advice">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>医嘱表格与展示单元</h3>
              <p>门诊/住院通用；order 列内嵌 AdviceOrderDisplayCell，受控当前行高亮，行操作透传。</p>
            </div>
            <el-tag effect="plain">AdviceOrderTable + AdviceOrderDisplayCell</el-tag>
          </div>

          <div class="demo-controls">
            <el-radio-group v-model="adviceDataset" size="small">
              <el-radio-button value="outpatient">门诊处方</el-radio-button>
              <el-radio-button value="inpatient">住院长期医嘱</el-radio-button>
            </el-radio-group>
            <el-switch v-model="adviceLoading" active-text="加载态" inline-prompt />
            <el-switch v-model="adviceEmpty" active-text="空数据" inline-prompt />
            <span class="demo-hint">当前行：{{ currentAdviceLabel }}</span>
          </div>

          <AdviceOrderTable
            :rows="adviceEmpty ? [] : currentAdviceRows"
            :columns="adviceColumns"
            :current-row-key="currentAdviceKey"
            :loading="adviceLoading"
            @current-change="onAdviceCurrent"
            @row-action="onAdviceAction"
          >
            <template #cell-status="{ row }">
              <el-tag size="small" :type="adviceTagType(row.status)" effect="light">
                {{ adviceStatusMap[row.status ?? ''] ?? row.status ?? '—' }}
              </el-tag>
            </template>
            <template #actions="{ row, action }">
              <el-button link type="primary" @click="action('detail')">详情</el-button>
              <el-button link type="danger" :disabled="row.status === 'stopped'" @click="action('stop')">停止</el-button>
            </template>
          </AdviceOrderTable>
          <p v-if="adviceLog" class="demo-log">{{ adviceLog }}</p>

          <div class="cell-preview">
            <h4>AdviceOrderDisplayCell 独立预览</h4>
            <div class="cell-preview__grid">
              <div v-for="order in currentAdviceRows" :key="order.key" class="cell-preview__item">
                <AdviceOrderDisplayCell :order="order" show-status :status-map="adviceStatusMap" />
              </div>
            </div>
          </div>
        </section>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup lang="ts">
import { computed, ref } from 'vue'
import CalendarSchedule from '@/components/schedule/CalendarSchedule.vue'
import TimeSlotGrid from '@/components/schedule/TimeSlotGrid.vue'
import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import AdviceOrderDisplayCell from '@/components/medical/AdviceOrderDisplayCell.vue'
import type {
  CalendarDateRange,
  CalendarView,
  ScheduleEvent,
  ScheduleResource,
  ScheduleStatusMap,
  TimeSlot
} from '@/components/schedule/type'
import type { AdviceColumn, AdviceOrder, MedicalKey } from '@/components/medical/type'

const activeTab = ref('calendar')

const overview = [
  { code: 'C28', name: 'CalendarSchedule', description: '月/周排班日历' },
  { code: 'C29', name: 'TimeSlotGrid', description: '号源/时段选择网格' },
  { code: 'C54', name: 'AdviceOrderTable', description: '门诊/住院医嘱表格' },
  { code: 'C55', name: 'AdviceOrderDisplayCell', description: '医嘱内容展示单元' }
]

// ---------------- C28 ----------------
const calendarDate = ref('2026-07-21')
const calendarView = ref<CalendarView>('week')
const calendarDataset = ref<'clinic' | 'exam'>('clinic')
const calendarLoading = ref(false)
const calendarReadonly = ref(false)
const calendarEmpty = ref(false)
const calendarRange = ref<CalendarDateRange>({ start: '', end: '' })
const calendarLog = ref('')

const calendarResources: ScheduleResource[] = [
  { key: 'r1', name: '张医生', color: 'var(--el-color-primary)' },
  { key: 'r2', name: '李医生', color: 'var(--el-color-success)' }
]

const calendarStatusMap: ScheduleStatusMap = {
  open: { text: '可约', type: 'success' },
  full: { text: '约满', type: 'warning' },
  stopped: { text: '停诊', type: 'danger' }
}

const clinicEvents: ScheduleEvent[] = [
  { key: 'c1', date: '2026-07-20', start: '08:00', end: '12:00', title: '张医生门诊', status: 'open', resourceKey: 'r1' },
  { key: 'c2', date: '2026-07-21', start: '08:00', end: '12:00', title: '李医生门诊', status: 'full', resourceKey: 'r2' },
  { key: 'c3', date: '2026-07-21', start: '14:00', end: '17:00', title: '张医生专家号', status: 'open', resourceKey: 'r1' },
  { key: 'c4', date: '2026-07-22', start: '08:00', end: '12:00', title: '内科联合门诊', status: 'stopped' },
  { key: 'c5', date: '2026-07-23', start: '09:00', end: '11:00', title: '李医生门诊', status: 'open', resourceKey: 'r2' },
  { key: 'c6', date: '2026-07-24', start: '08:00', end: '10:00', title: '早间加号', status: 'open', resourceKey: 'r1' }
]

const examEvents: ScheduleEvent[] = [
  { key: 'e1', date: '2026-07-20', start: '07:30', end: '09:00', title: '入职体检', status: 'open' },
  { key: 'e2', date: '2026-07-22', start: '07:30', end: '11:00', title: '健康套餐A', status: 'full' },
  { key: 'e3', date: '2026-07-25', start: '07:30', end: '09:30', title: 'VIP体检', status: 'open' }
]

const currentCalendarEvents = computed(() =>
  calendarDataset.value === 'clinic' ? clinicEvents : examEvents
)

function onEventClick(event: ScheduleEvent) {
  calendarLog.value = `点击事件：${event.title}（${event.date} ${event.start ?? ''}）`
}

function onDateClick(date: string) {
  calendarLog.value = `点击日期：${date}`
}

// ---------------- C29 ----------------
const selectedSlot = ref<MedicalKey | null>(null)
const slotDataset = ref<'morning' | 'expert'>('morning')
const slotLoading = ref(false)
const slotReadonly = ref(false)
const slotLog = ref('')

const morningSlots: TimeSlot[] = [
  { key: 'm1', start: '08:00', end: '08:30', remaining: 3, capacity: 5, status: 'available', group: '上午' },
  { key: 'm2', start: '08:30', end: '09:00', remaining: 0, capacity: 5, status: 'full', group: '上午' },
  { key: 'm3', start: '09:00', end: '09:30', remaining: 1, capacity: 5, status: 'available', group: '上午' },
  { key: 'a1', start: '14:00', end: '14:30', remaining: 4, capacity: 5, status: 'available', group: '下午' },
  { key: 'a2', start: '14:30', end: '15:00', remaining: 2, capacity: 5, status: 'available', group: '下午' }
]

const expertSlots: TimeSlot[] = [
  { key: 'x1', label: '特需专家号', start: '10:00', end: '10:30', remaining: 1, status: 'available', group: '特需' },
  { key: 'x2', label: '停诊时段', start: '10:30', end: '11:00', status: 'stopped', group: '特需' },
  { key: 'x3', label: '已过号', start: '08:00', end: '08:30', status: 'expired', group: '普通' },
  { key: 'x4', label: '普通号', start: '11:00', end: '11:30', remaining: 6, status: 'available', group: '普通' }
]

const currentSlots = computed(() => (slotDataset.value === 'morning' ? morningSlots : expertSlots))

const selectedSlotLabel = computed(() => {
  if (selectedSlot.value === null) return '未选择'
  const found = currentSlots.value.find((s) => s.key === selectedSlot.value)
  return found ? (found.label ?? `${found.start} - ${found.end}`) : '未选择'
})

function onSlotSelect(slot: TimeSlot) {
  slotLog.value = `选中号源：${slot.label ?? `${slot.start} - ${slot.end}`}`
}

function onSlotDisabled(slot: TimeSlot) {
  slotLog.value = `不可选：${slot.label ?? `${slot.start} - ${slot.end}`}`
}

// ---------------- C54 / C55 ----------------
const adviceDataset = ref<'outpatient' | 'inpatient'>('outpatient')
const adviceLoading = ref(false)
const adviceEmpty = ref(false)
const currentAdviceKey = ref<MedicalKey | null>(null)
const adviceLog = ref('')

const adviceStatusMap: Record<string, string> = {
  pending: '待执行',
  active: '执行中',
  done: '已完成',
  stopped: '已停止'
}

const adviceColumns: AdviceColumn[] = [
  { prop: 'order', label: '医嘱内容', minWidth: 240 },
  { prop: 'frequency', label: '频次', width: 90 },
  { prop: 'status', label: '状态', width: 100 }
]

const outpatientRows: AdviceOrder[] = [
  { key: 'o1', name: '阿莫西林胶囊', specification: '0.25g', dose: 2, unit: '粒', frequency: 'tid', route: '口服', days: 3, status: 'pending', group: '处方A' },
  { key: 'o2', name: '布洛芬缓释胶囊', specification: '0.3g', dose: 1, unit: '粒', frequency: 'bid', route: '口服', days: 5, status: 'active', group: '处方A' },
  { key: 'o3', name: '血常规', frequency: '一次', status: 'done', remark: '空腹', group: '检验' }
]

const inpatientRows: AdviceOrder[] = [
  { key: 'i1', name: '0.9%氯化钠注射液', specification: '250ml', dose: 250, unit: 'ml', frequency: 'qd', route: '静脉滴注', status: 'active', group: '长期' },
  { key: 'i2', name: '注射用头孢曲松钠', specification: '2g', dose: 2, unit: 'g', frequency: 'qd', route: '静脉滴注', status: 'active', remark: '皮试阴性', group: '长期' },
  { key: 'i3', name: '心电监护', frequency: '持续', status: 'stopped', remark: '病情稳定已停', group: '护理' }
]

const currentAdviceRows = computed(() =>
  adviceDataset.value === 'outpatient' ? outpatientRows : inpatientRows
)

const currentAdviceLabel = computed(() => {
  if (currentAdviceKey.value === null) return '未选择'
  const found = currentAdviceRows.value.find((r) => r.key === currentAdviceKey.value)
  return found ? found.name : '未选择'
})

function adviceTagType(status?: string): 'success' | 'warning' | 'danger' | 'info' {
  if (status === 'done') return 'success'
  if (status === 'active') return 'warning'
  if (status === 'stopped') return 'danger'
  return 'info'
}

function onAdviceCurrent(row?: AdviceOrder) {
  currentAdviceKey.value = row ? row.key : null
}

function onAdviceAction(payload: { row: AdviceOrder; action: string }) {
  adviceLog.value = `${payload.action === 'stop' ? '停止' : '查看详情'}：${payload.row.name}`
}
</script>

<style scoped>
.e-demo-page {
  display: flex;
  flex-direction: column;
  gap: 16px;
  padding: 16px;
}

.demo-header h2 {
  margin: 0 0 4px;
  font-size: 20px;
}

.demo-header p {
  margin: 0;
  color: var(--el-text-color-secondary);
}

.component-overview {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
  gap: 10px;
}

.overview-item {
  display: flex;
  flex-direction: column;
  gap: 2px;
  padding: 10px 12px;
  background: var(--el-fill-color-light);
  border-radius: 6px;
}

.overview-code {
  font-size: 12px;
  font-weight: 700;
  color: var(--el-color-primary);
}

.demo-section {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.section-heading {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
}

.section-heading h3 {
  margin: 0 0 4px;
}

.section-heading p {
  margin: 0;
  font-size: 13px;
  color: var(--el-text-color-secondary);
}

.demo-controls {
  display: flex;
  flex-wrap: wrap;
  gap: 16px;
  align-items: center;
  padding: 10px 12px;
  background: var(--el-fill-color-lighter);
  border-radius: 6px;
}

.demo-hint {
  font-size: 13px;
  color: var(--el-text-color-regular);
}

.demo-log {
  padding: 8px 12px;
  margin: 0;
  font-size: 13px;
  color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
  border-radius: 6px;
}

.cell-preview {
  margin-top: 8px;
}

.cell-preview h4 {
  margin: 0 0 8px;
  font-size: 14px;
}

.cell-preview__grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(240px, 1fr));
  gap: 10px;
}

.cell-preview__item {
  padding: 10px 12px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 6px;
}
</style>
