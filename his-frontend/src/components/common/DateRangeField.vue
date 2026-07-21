<script setup lang="ts">
import type { Dayjs } from 'dayjs'
import type { DateRangeValue } from './type'

import { computed } from 'vue'

import dayjs from 'dayjs'
import { ElMessage } from 'element-plus'

defineOptions({
  name: 'DateRangeField'
})

interface DateRangeFieldProps {
  maxRangeDays?: number
  showOptions?: boolean
}

const props = withDefaults(defineProps<DateRangeFieldProps>(), {
  maxRangeDays: 30,
  showOptions: true
})

const emit = defineEmits<{
  change: [value: DateRangeValue]
}>()

const dateRange = defineModel<DateRangeValue>({ default: null })
const enableTime = defineModel<boolean>('enableTime', { default: false })
const limitFuture = defineModel<boolean>('limitFuture', { default: false })

const pickerType = computed(() =>
  enableTime.value ? 'datetimerange' : 'daterange'
)

const valueFormat = computed(() =>
  enableTime.value ? 'YYYY-MM-DD HH:mm:ss' : 'YYYY-MM-DD'
)

function buildShortcutRange(start: Dayjs, end: Dayjs): [string, string] {
  return [
    start.format(valueFormat.value),
    end.format(valueFormat.value)
  ]
}

const shortcuts = [
  {
    text: '最近7天',
    value: () =>
      buildShortcutRange(
        dayjs().subtract(6, 'day').startOf('day'),
        dayjs().endOf('day')
      )
  },
  {
    text: '最近30天',
    value: () =>
      buildShortcutRange(
        dayjs().subtract(29, 'day').startOf('day'),
        dayjs().endOf('day')
      )
  },
  {
    text: '本月',
    value: () =>
      buildShortcutRange(
        dayjs().startOf('month'),
        dayjs().endOf('day')
      )
  }
]

function parseDate(value: string) {
  const matched = value.match(/^(\d{4})-(\d{2})-(\d{2})/)
  if (!matched) return null

  const year = Number(matched[1])
  const month = Number(matched[2])
  const day = Number(matched[3])
  const date = new Date(year, month - 1, day)

  return Number.isNaN(date.getTime()) ? null : date
}

function formatDate(date: Date) {
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')

  return `${year}-${month}-${day}`
}

function disabledDate(time: Date) {
  if (!limitFuture.value) return false

  const todayEnd = new Date()
  todayEnd.setHours(23, 59, 59, 999)

  return time.getTime() > todayEnd.getTime()
}

function handleRangeChange(value: DateRangeValue) {
  if (!value) {
    dateRange.value = null
    emit('change', null)
    return
  }

  const [start, end] = value
  const startDate = parseDate(start)
  const endDate = parseDate(end)
  const maxRangeDays = Math.max(0, props.maxRangeDays)

  if (!startDate || !endDate) {
    emit('change', value)
    return
  }

  const millisecondsPerDay = 24 * 60 * 60 * 1000
  const rangeDays =
    Math.abs(endDate.getTime() - startDate.getTime()) /
    millisecondsPerDay

  if (rangeDays <= maxRangeDays) {
    emit('change', value)
    return
  }

  const correctedEnd = new Date(startDate)
  correctedEnd.setDate(correctedEnd.getDate() + maxRangeDays)

  const endTime = enableTime.value
    ? end.split(' ')[1] || '23:59:59'
    : ''

  const correctedValue: DateRangeValue = [
    start,
    `${formatDate(correctedEnd)}${endTime ? ` ${endTime}` : ''}`
  ]

  dateRange.value = correctedValue

  ElMessage.warning(
    `日期跨度不能超过 ${maxRangeDays} 天，已自动调整结束日期`
  )

  emit('change', correctedValue)
}
</script>

<template>
  <div class="date-range-field">
    <el-date-picker
      v-model="dateRange"
      class="date-range-field__picker"
      :type="pickerType"
      :value-format="valueFormat"
      :shortcuts="shortcuts"
      range-separator="至"
      start-placeholder="开始日期"
      end-placeholder="结束日期"
      :disabled-date="disabledDate"
      @change="handleRangeChange"
    />

    <div
      v-if="showOptions"
      class="date-range-field__options"
    >
      <el-checkbox v-model="enableTime">
        显示时分秒
      </el-checkbox>

      <el-checkbox v-model="limitFuture">
        限制未来日期
      </el-checkbox>

      <span class="date-range-field__hint">
        最大跨度 {{ maxRangeDays }} 天
      </span>
    </div>
  </div>
</template>

<style scoped>
.date-range-field {
  display: flex;
  width: 100%;
  min-width: 0;
  flex-wrap: wrap;
  align-items: center;
  gap: 8px 12px;
}

.date-range-field__picker {
  width: min(100%, 360px);
}

.date-range-field__options {
  display: flex;
  min-width: 0;
  flex-wrap: wrap;
  align-items: center;
  gap: 4px 12px;
}

.date-range-field__options :deep(.el-checkbox) {
  margin-right: 0;
}

.date-range-field__hint {
  color: var(--el-text-color-secondary);
  font-size: 13px;
  white-space: nowrap;
}
</style>
