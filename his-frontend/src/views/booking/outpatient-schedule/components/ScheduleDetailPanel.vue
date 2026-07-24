<script setup lang="ts">
import type { ScheduleRecord } from '@/apis/booking/outpatient-schedule'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'

import { computed } from 'vue'

import DescriptionList from '@/components/common/DescriptionList.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import { scheduleStatusOptions } from '../constants'

const props = defineProps<{
  modelValue: boolean
  data: ScheduleRecord | null
  loading?: boolean
  actionLoading?: boolean
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  edit: [record: ScheduleRecord]
  publish: [record: ScheduleRecord]
  stop: [record: ScheduleRecord]
}>()

const visible = computed({
  get: () => props.modelValue,
  set: (value) => emit('update:modelValue', value)
})

const items: DescriptionItem[] = [
  { prop: 'departmentName', label: '科室' },
  { prop: 'doctorName', label: '医生' },
  { prop: 'roomName', label: '诊室' },
  { prop: 'date', label: '日期' },
  { prop: 'timeRange', label: '时间' },
  { prop: 'capacity', label: '总号数' },
  { prop: 'feeText', label: '挂号费' },
  { prop: 'stopReason', label: '停诊原因', span: 2, hidden: (data) => !data.stopReason }
]

const displayData = computed(() =>
  props.data
    ? {
        ...props.data,
        timeRange: `${props.data.startTime} - ${props.data.endTime}`,
        feeText: `¥${props.data.fee.toFixed(2)}`
      }
    : null
)
</script>

<template>
  <el-drawer v-model="visible" title="排班详情" size="520px" destroy-on-close>
    <div v-loading="loading" class="schedule-detail">
      <div v-if="data" class="schedule-detail__status">
        <span>当前状态</span>
        <StatusDisplay :value="data.status" :options="scheduleStatusOptions" />
      </div>
      <DescriptionList v-if="data" :data="displayData" :items="items" :column="2" dense />
      <el-empty v-else description="未找到排班详情" />
    </div>
    <template #footer>
      <div v-if="data" class="schedule-detail__actions">
        <el-button
          :disabled="actionLoading || data.status === 'stopped'"
          @click="emit('edit', data)"
          >编辑</el-button
        >
        <el-button
          v-if="data.status === 'draft'"
          type="success"
          :loading="actionLoading"
          @click="emit('publish', data)"
          >发布</el-button
        >
        <el-button
          v-if="data.status !== 'stopped'"
          type="danger"
          :loading="actionLoading"
          @click="emit('stop', data)"
          >停诊</el-button
        >
      </div>
    </template>
  </el-drawer>
</template>

<style scoped>
.schedule-detail {
  --el-bg-color: #ffffff;
  --el-fill-color-lighter: #f5f7fa;
  --el-text-color-primary: #303133;
  --el-text-color-regular: #606266;
  --el-text-color-secondary: #909399;

  min-height: 220px;
  color: #303133;
}
.schedule-detail__status {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 16px;
  padding: 12px;
  color: #303133;
  background: #f5f7fa;
  border-radius: 6px;
}
.schedule-detail :deep(.el-descriptions__label) {
  color: #606266 !important;
  background: #f5f7fa !important;
}
.schedule-detail :deep(.el-descriptions__content) {
  color: #303133 !important;
  background: #ffffff !important;
}
.schedule-detail__actions {
  display: flex;
  justify-content: flex-end;
  gap: 8px;
}
</style>
