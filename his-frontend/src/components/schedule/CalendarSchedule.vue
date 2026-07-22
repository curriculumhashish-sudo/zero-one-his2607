<template>
  <section class="calendar-schedule" :aria-busy="loading">
    <header class="calendar-schedule__toolbar">
      <slot name="toolbar" :range="range" :view="view" :title="title">
        <div class="calendar-schedule__nav">
          <el-button-group>
            <el-button :disabled="readonly" @click="move(-1)">上一{{ view === 'month' ? '月' : '周' }}</el-button>
            <el-button :disabled="readonly" @click="goToday">今天</el-button>
            <el-button :disabled="readonly" @click="move(1)">下一{{ view === 'month' ? '月' : '周' }}</el-button>
          </el-button-group>
          <span class="calendar-schedule__title">{{ title }}</span>
        </div>
        <el-radio-group
          :model-value="view"
          :disabled="readonly"
          size="small"
          @update:model-value="emit('update:view', $event as CalendarView)"
        >
          <el-radio-button value="week">周</el-radio-button>
          <el-radio-button value="month">月</el-radio-button>
        </el-radio-group>
      </slot>
    </header>

    <div v-if="loading" class="calendar-schedule__state"><el-skeleton :rows="5" animated /></div>
    <template v-else>
      <div v-if="events.length === 0" class="calendar-schedule__state calendar-schedule__state--empty">
        <slot name="empty">{{ emptyText }}</slot>
      </div>
      <div class="calendar-schedule__grid" :class="`calendar-schedule__grid--${view}`">
        <div v-for="label in weekLabels" :key="label" class="calendar-schedule__week-label">{{ label }}</div>
        <article
          v-for="day in days"
          :key="day.date"
          class="calendar-schedule__day"
          :class="{ 'is-other': !day.isCurrentMonth, 'is-today': day.isToday, 'is-weekend': day.isWeekend, 'is-anchor': day.date === modelValue }"
          @click="onDateClick(day)"
        >
          <slot name="dayHeader" :cell="day">
            <div class="calendar-schedule__day-head">
              <button
                type="button"
                class="calendar-schedule__date"
                :disabled="readonly"
                @click.stop="onSlotClick(day)"
              >
                {{ day.day }}
              </button>
              <span v-if="day.isToday" class="calendar-schedule__today-tag">今天</span>
            </div>
          </slot>
          <div class="calendar-schedule__events">
            <button
              v-for="event in visibleEvents(day)"
              :key="event.key"
              type="button"
              class="calendar-schedule__event"
              :style="eventStyle(event)"
              @click.stop="emit('eventClick', event)"
            >
              <slot name="event" :event="event" :status-text="statusText(event)">
                <span v-if="event.start" class="calendar-schedule__event-time">{{ event.start }}</span>
                <span class="calendar-schedule__event-title">{{ event.title }}</span>
                <span v-if="statusText(event)" class="calendar-schedule__event-status">{{ statusText(event) }}</span>
              </slot>
            </button>
            <button
              v-if="overflow(day) > 0"
              type="button"
              class="calendar-schedule__more"
              @click.stop="onMoreClick(day)"
            >
              <slot name="more" :cell="day" :count="overflow(day)">+{{ overflow(day) }} 更多</slot>
            </button>
          </div>
        </article>
      </div>
    </template>
  </section>
</template>

<script setup lang="ts">
import { computed, watch } from 'vue'
import type {
  CalendarDateRange,
  CalendarDayCell,
  CalendarView,
  ScheduleEvent,
  ScheduleResource,
  ScheduleStatusMap
} from './type'

/**
 * C28 CalendarSchedule —— 排班/预约日历（月视图 / 周视图）。
 *
 * 纯展示与交互组件：events / resources 由页面注入；切换范围仅发出 rangeChange，
 * 由页面决定何时加载数据。不请求任何排班接口，不写死状态码。
 */
defineOptions({ name: 'CalendarSchedule' })

const props = withDefaults(
  defineProps<{
    /** 当前锚点日期（受控），格式 `YYYY-MM-DD` */
    modelValue: string
    /** 视图模式（受控） */
    view?: CalendarView
    /** 事件列表 */
    events?: ScheduleEvent[]
    /** 资源列表，用于事件着色 */
    resources?: ScheduleResource[]
    /** 加载中 */
    loading?: boolean
    /** 每周第一天：0=周日，1=周一 */
    firstDayOfWeek?: 0 | 1
    /** 状态映射（文案 + 语义色），组件不写死状态码 */
    statusMap?: ScheduleStatusMap
    /** 只读 */
    readonly?: boolean
    /** 单元格最多展示的事件数，超出折叠为「更多」 */
    maxEventsPerDay?: number
    /** 空数据文案 */
    emptyText?: string
  }>(),
  {
    view: 'week',
    events: () => [],
    resources: () => [],
    loading: false,
    firstDayOfWeek: 1,
    statusMap: () => ({}),
    readonly: false,
    maxEventsPerDay: 3,
    emptyText: '当前范围暂无排班'
  }
)

const emit = defineEmits<{
  'update:modelValue': [value: string]
  'update:view': [value: CalendarView]
  rangeChange: [range: CalendarDateRange]
  dateClick: [date: string, cell: CalendarDayCell]
  slotClick: [date: string, cell: CalendarDayCell]
  eventClick: [event: ScheduleEvent]
  eventAction: [action: string, event: ScheduleEvent]
}>()

const weekLabels = computed(() =>
  props.firstDayOfWeek === 1
    ? ['周一', '周二', '周三', '周四', '周五', '周六', '周日']
    : ['周日', '周一', '周二', '周三', '周四', '周五', '周六']
)

const today = formatDate(new Date())

const range = computed<CalendarDateRange>(() =>
  getRange(props.modelValue, props.view, props.firstDayOfWeek)
)

const title = computed(() => {
  const date = parseDate(props.modelValue)
  if (props.view === 'week') return `${range.value.start} ~ ${range.value.end}`
  return `${date.getFullYear()}年${date.getMonth() + 1}月`
})

const eventsByDate = computed(() => {
  const map = new Map<string, ScheduleEvent[]>()
  for (const event of props.events) {
    const list = map.get(event.date)
    if (list) list.push(event)
    else map.set(event.date, [event])
  }
  return map
})

const days = computed<CalendarDayCell[]>(() => {
  const start = parseDate(range.value.start)
  const end = parseDate(range.value.end)
  const currentMonth = parseDate(props.modelValue).getMonth()
  const result: CalendarDayCell[] = []
  for (let date = new Date(start); date <= end; date.setDate(date.getDate() + 1)) {
    const key = formatDate(date)
    const weekday = date.getDay()
    result.push({
      date: key,
      day: date.getDate(),
      isCurrentMonth: props.view === 'week' || date.getMonth() === currentMonth,
      isToday: key === today,
      isWeekend: weekday === 0 || weekday === 6,
      events: eventsByDate.value.get(key) ?? []
    })
  }
  return result
})

watch(range, (value) => emit('rangeChange', value), { immediate: true, deep: true })

function visibleEvents(day: CalendarDayCell): ScheduleEvent[] {
  return day.events.slice(0, props.maxEventsPerDay)
}

function overflow(day: CalendarDayCell): number {
  return Math.max(0, day.events.length - props.maxEventsPerDay)
}

function statusText(event: ScheduleEvent): string {
  if (!event.status) return ''
  return props.statusMap[event.status]?.text ?? event.status
}

function eventStyle(event: ScheduleEvent) {
  const resource = props.resources.find((item) => item.key === event.resourceKey)
  const type = event.status ? props.statusMap[event.status]?.type : undefined
  const color = resource?.color ?? (type ? `var(--el-color-${type})` : 'var(--el-color-primary)')
  return { borderLeftColor: color }
}

function onDateClick(day: CalendarDayCell) {
  if (props.readonly) return
  emit('dateClick', day.date, day)
}

function onSlotClick(day: CalendarDayCell) {
  if (props.readonly) return
  emit('slotClick', day.date, day)
}

function onMoreClick(day: CalendarDayCell) {
  if (props.readonly) return
  emit('dateClick', day.date, day)
}

function move(offset: number) {
  const date = parseDate(props.modelValue)
  if (props.view === 'month') date.setMonth(date.getMonth() + offset)
  else date.setDate(date.getDate() + offset * 7)
  emit('update:modelValue', formatDate(date))
}

function goToday() {
  emit('update:modelValue', today)
}

function parseDate(value: string) {
  const [y, m, d] = value.slice(0, 10).split('-').map((n) => Number.parseInt(n, 10))
  return new Date(y ?? 1970, (m ?? 1) - 1, d ?? 1)
}

function formatDate(value: Date) {
  return `${value.getFullYear()}-${String(value.getMonth() + 1).padStart(2, '0')}-${String(
    value.getDate()
  ).padStart(2, '0')}`
}

function getRange(value: string, view: CalendarView, firstDay: 0 | 1): CalendarDateRange {
  const date = parseDate(value)
  if (view === 'week') {
    const delta = (date.getDay() - firstDay + 7) % 7
    date.setDate(date.getDate() - delta)
    const end = new Date(date)
    end.setDate(end.getDate() + 6)
    return { start: formatDate(date), end: formatDate(end) }
  }
  const start = new Date(date.getFullYear(), date.getMonth(), 1)
  const leading = (start.getDay() - firstDay + 7) % 7
  start.setDate(start.getDate() - leading)
  const end = new Date(start)
  end.setDate(end.getDate() + 41)
  return { start: formatDate(start), end: formatDate(end) }
}

defineExpose({ range, days })
</script>

<style scoped>
.calendar-schedule {
  min-width: 0;
  overflow: hidden;
  background: var(--el-bg-color);
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
}

.calendar-schedule__toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.calendar-schedule__nav {
  display: inline-flex;
  gap: 12px;
  align-items: center;
}

.calendar-schedule__title {
  font-size: 14px;
  font-weight: 700;
  color: var(--el-text-color-primary);
}

.calendar-schedule__grid {
  display: grid;
  grid-template-columns: repeat(7, minmax(0, 1fr));
}

.calendar-schedule__week-label {
  padding: 8px;
  font-size: 12px;
  color: var(--el-text-color-secondary);
  text-align: center;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.calendar-schedule__day {
  min-height: 118px;
  padding: 6px;
  overflow: hidden;
  cursor: pointer;
  border-right: 1px solid var(--el-border-color-lighter);
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.calendar-schedule__grid--week .calendar-schedule__day {
  min-height: 230px;
}

.calendar-schedule__day.is-other {
  color: var(--el-text-color-placeholder);
  background: var(--el-fill-color-lighter);
}

.calendar-schedule__day.is-weekend {
  background: var(--el-fill-color-blank);
}

.calendar-schedule__day.is-today {
  box-shadow: inset 0 2px var(--el-color-primary);
}

.calendar-schedule__day.is-anchor {
  background: var(--el-color-primary-light-9);
  box-shadow: inset 0 0 0 1px var(--el-color-primary-light-5);
}

.calendar-schedule__day-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.calendar-schedule__date {
  font: inherit;
  color: inherit;
  cursor: pointer;
  background: transparent;
  border: 0;
}

.calendar-schedule__today-tag {
  padding: 0 6px;
  font-size: 11px;
  line-height: 18px;
  color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
  border-radius: 999px;
}

.calendar-schedule__events {
  display: grid;
  gap: 4px;
  margin-top: 5px;
}

.calendar-schedule__event,
.calendar-schedule__more {
  display: flex;
  flex-wrap: wrap;
  gap: 4px;
  align-items: center;
  min-width: 0;
  padding: 4px 6px;
  overflow: hidden;
  font-size: 12px;
  color: var(--el-text-color-regular);
  text-align: left;
  cursor: pointer;
  background: var(--el-fill-color-light);
  border: 1px solid transparent;
  border-left: 3px solid var(--el-color-primary);
  border-radius: 4px;
  transition:
    box-shadow 0.15s ease,
    background-color 0.15s ease;
}

.calendar-schedule__event:hover {
  background: var(--el-fill-color);
  box-shadow: 0 1px 6px rgb(0 0 0 / 8%);
}

.calendar-schedule__more {
  border-left-color: var(--el-color-info);
}

.calendar-schedule__event-time {
  font-size: 11px;
  color: var(--el-text-color-secondary);
}

.calendar-schedule__event-title {
  font-weight: 600;
}

.calendar-schedule__event-status {
  font-size: 11px;
  color: var(--el-text-color-secondary);
}

.calendar-schedule__state {
  padding: 24px;
  color: var(--el-text-color-secondary);
  text-align: center;
}

.calendar-schedule__state--empty {
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-lighter);
}
</style>
