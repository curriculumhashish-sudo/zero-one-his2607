<template>
  <div>
    <el-date-picker
      v-model="dateRange"
      :type="enableTime ? 'datetimerange' : 'daterange'"
      :value-format="enableTime ? 'YYYY-MM-DD HH:mm:ss' : 'YYYY-MM-DD'"
      range-separator="至"
      start-placeholder="开始时间"
      end-placeholder="结束时间"
      :shortcuts="shortcuts"
      :disabled-date="disabledDate"
      @change="handleRangeChange"
    />
    <el-checkbox v-model="enableTime">显示时分秒</el-checkbox>
    <el-checkbox v-model="allowFuture">限制未来日期</el-checkbox>
    <span style="margin-left: 12px; color: #909399; font-size: 13px"
      >最大跨度 {{ maxRangeDays }} 天</span
    >
  </div>
</template>

<script lang="ts" setup>
import { ref } from 'vue'
import { ElMessage } from 'element-plus'
import dayjs from 'dayjs'

const dateRange = ref<[string, string] | null>(null)
// 是否显示时分秒
const enableTime = ref(false)
// 是否限制未来日期
const allowFuture = ref(false)
// 最大跨度天数
const maxRangeDays = ref(30)

// 快捷日期（注意：这些快捷日期是“未来”的，与 disabledDate 冲突？请根据实际需要调整）
const shortcuts = [
  {
    text: '7天后',
    value: () => {
      const start = new Date()
      const end = new Date()
      end.setTime(start.getTime() + 3600 * 1000 * 24 * 7)
      return [start, end]
    }
  },
  {
    text: '30天后',
    value: () => {
      const start = new Date()
      const end = new Date()
      end.setTime(start.getTime() + 3600 * 1000 * 24 * 30)
      return [start, end]
    }
  }
]

// 未来日期限制（动态）
const disabledDate = (time: Date) => {
  if (!allowFuture.value) return false // 不限制
  const todayEnd = new Date()
  todayEnd.setHours(23, 59, 59, 999)
  return time.getTime() > todayEnd.getTime()
}

// ========== 最大跨度处理 ==========
function handleRangeChange(newVal: [string, string] | null): void {
  if (!newVal || !Array.isArray(newVal) || newVal.length !== 2) {
    // 清空时无需处理
    return
  }

  const [start, end] = newVal
  const maxDays = maxRangeDays.value

  // 计算实际天数差（忽略时间，仅按日期）
  const startDay = dayjs(start).startOf('day')
  const endDay = dayjs(end).startOf('day')
  const diffDays = Math.abs(endDay.diff(startDay, 'day'))

  if (diffDays > maxDays) {
    // 自动修正结束日期 = 开始日期 + maxDays
    const correctedEnd = startDay.add(maxDays, 'day')
    let finalEnd: string

    if (enableTime.value) {
      // 保留原结束时间的时分秒（如果有）
      const endTime = dayjs(end).format('HH:mm:ss')
      finalEnd = correctedEnd.format('YYYY-MM-DD') + ' ' + endTime
    } else {
      finalEnd = correctedEnd.format('YYYY-MM-DD')
    }

    // 更新 dateRange 并提示
    dateRange.value = [start, finalEnd]
    ElMessage.warning(`日期跨度不能超过 ${maxDays} 天，已自动调整结束日期`)
  }
  // 未超出范围则不做改动
}
</script>

<style scoped>
.demo-date-picker {
  display: flex;
  width: 100%;
  padding: 0;
  flex-wrap: wrap;
}

.demo-date-picker .block {
  padding: 1.5rem 0;
  text-align: center;
  border-right: solid 1px var(--el-border-color);
  flex: 1;
  min-width: 300px;
  display: flex;
  flex-direction: column;
  align-items: center;
}

.demo-date-picker .block:last-child {
  border-right: none;
}

.demo-date-picker .block :deep(.el-date-editor) {
  width: 300px;
}

.demo-date-picker .demonstration {
  display: block;
  color: var(--el-text-color-secondary);
  font-size: 14px;
  margin-bottom: 1rem;
  width: 100%;
}

@media screen and (max-width: 1200px) {
  .demo-date-picker .block {
    flex: 0 0 50%;
    border-bottom: solid 1px var(--el-border-color);
  }

  .demo-date-picker .block:nth-child(2n) {
    border-right: none;
  }

  .demo-date-picker .block:nth-last-child(-n + 2):nth-child(2n + 1),
  .demo-date-picker .block:last-child {
    border-bottom: none;
  }
}

@media screen and (max-width: 768px) {
  .demo-date-picker .block {
    flex: 0 0 100%;
    padding: 1rem 0;
    min-width: auto;
    border-right: none;
    border-bottom: solid 1px var(--el-border-color);
  }

  .demo-date-picker .block:last-child {
    border-bottom: none;
  }
}
</style>
