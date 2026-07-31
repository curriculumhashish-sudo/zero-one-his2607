<script setup lang="ts">
import type { BookingOption, ScheduleRecord } from '@/apis/booking/outpatient-schedule'
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
  doctors?: BookingOption[]
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  edit: [record: ScheduleRecord]
  publish: [record: ScheduleRecord]
  stop: [record: ScheduleRecord]
  substitute: [record: ScheduleRecord, doctorId: string]
  clearSubstitute: [record: ScheduleRecord]
  resume: [record: ScheduleRecord]
  partialStop: [record: ScheduleRecord]
  unlock: [record: ScheduleRecord]
  provision: [record: ScheduleRecord]
  deleteSlots: [record: ScheduleRecord]
  void: [record: ScheduleRecord]
}>()

const visible = computed({
  get: () => props.modelValue,
  set: (value) => emit('update:modelValue', value)
})
const isVoided = computed(() => Boolean(props.data?.voidedFlag))
const isDraft = computed(() => props.data?.status === 'draft' && !isVoided.value)
const isPublished = computed(() => props.data?.status === 'published' && !isVoided.value)
const isStopped = computed(() => props.data?.status === 'stopped' && !isVoided.value)
const hasPartialStoppedSlots = computed(
  () => (props.data?.partialStoppedSlotIds?.length ?? 0) > 0
)

const items: DescriptionItem[] = [
  { prop: 'departmentName', label: '科室' },
  { prop: 'doctorName', label: '医生' },
  { prop: 'roomName', label: '诊室' },
  { prop: 'date', label: '日期' },
  { prop: 'timeRange', label: '时间' },
  { prop: 'capacity', label: '总号数' },
  {
    prop: 'substituteDisplay',
    label: '替诊信息',
    span: 2,
    hidden: (data) => !data.substituteDisplay
  },
  { prop: 'stopReason', label: '停诊原因', span: 2, hidden: (data) => !data.stopReason }
]

const displayData = computed(() =>
  props.data
    ? {
        ...props.data,
        timeRange: `${props.data.startTime} - ${props.data.endTime}`
      }
    : null
)

function handleMore(command: string) {
  if (!props.data) return
  if (command.startsWith('substitute:')) emit('substitute', props.data, command.slice(11))
  else if (command === 'clear-substitute') emit('clearSubstitute', props.data)
  else if (command === 'partial-stop') emit('partialStop', props.data)
  else if (command === 'unlock') emit('unlock', props.data)
  else if (command === 'provision') emit('provision', props.data)
  else if (command === 'delete-slots') emit('deleteSlots', props.data)
  else if (command === 'void') emit('void', props.data)
}
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
      <div v-if="data && !isVoided" class="schedule-detail__actions">
        <el-button
          v-if="isDraft"
          :disabled="actionLoading"
          @click="emit('edit', data)"
          >编辑</el-button
        >
        <el-button
          v-if="isDraft"
          type="success"
          :loading="actionLoading"
          @click="emit('publish', data)"
          >发布</el-button
        >
        <el-button
          v-if="isPublished"
          type="danger"
          :loading="actionLoading"
          @click="emit('stop', data)"
          >停诊</el-button
        >
        <el-button
          v-if="isStopped"
          type="success"
          :loading="actionLoading"
          @click="emit('resume', data)"
          >恢复开诊</el-button
        >
        <el-dropdown
          v-if="isPublished || isDraft || isStopped"
          :disabled="actionLoading"
          @command="handleMore"
        >
          <el-button>更多操作</el-button>
          <template #dropdown>
            <el-dropdown-menu>
              <template v-if="isPublished">
                <el-dropdown-item
                  v-for="doctor in doctors ?? []"
                  :key="doctor.value"
                  :command="`substitute:${doctor.value}`"
                  :disabled="doctor.value === data.doctorId"
                  >替诊：{{ doctor.label }}</el-dropdown-item
                >
              </template>
              <el-dropdown-item
                v-if="isPublished && data.substitutePractitionerId"
                command="clear-substitute"
                divided
                >撤销替诊</el-dropdown-item
              >
              <el-dropdown-item v-if="isPublished" command="partial-stop" divided
                >部分停诊</el-dropdown-item
              >
              <el-dropdown-item v-if="isPublished && hasPartialStoppedSlots" command="unlock"
                >解锁停诊</el-dropdown-item
              >
              <el-dropdown-item v-if="isPublished" command="provision">批量放号</el-dropdown-item>
              <el-dropdown-item v-if="isPublished" command="delete-slots"
                >删除号源</el-dropdown-item
              >
              <el-dropdown-item v-if="isDraft || isStopped" command="void" divided
                >作废日程</el-dropdown-item
              >
            </el-dropdown-menu>
          </template>
        </el-dropdown>
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
