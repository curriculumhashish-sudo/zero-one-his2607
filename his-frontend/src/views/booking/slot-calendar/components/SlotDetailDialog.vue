<script setup lang="ts">
import type { SlotRecord } from '@/apis/booking/slot-calendar'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'
import type { StatusDisplayOption } from '@/components/common/type'

import { computed, ref } from 'vue'

import DescriptionList from '@/components/common/DescriptionList.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'

const props = defineProps<{ data: SlotRecord | null; loading?: boolean }>()
const visible = ref(false)

const statusOptions: StatusDisplayOption[] = [
  { value: 'available', label: '可约', type: 'success' },
  { value: 'full', label: '已满', type: 'danger' },
  { value: 'stopped', label: '停诊', type: 'info' },
  { value: 'blocked', label: '已封锁', type: 'warning' },
  { value: 'expired', label: '过期', type: 'info' }
]

const items = computed<DescriptionItem[]>(() => [
  { prop: 'organizationName', label: '科室' },
  { prop: 'practitionerName', label: '医师' },
  { prop: 'healthcareServiceName', label: '号源类型' },
  { prop: 'status', label: '状态' },
  { prop: 'date', label: '日期' },
  { prop: 'timeRange', label: '时段' },
  { prop: 'capacity', label: '容量' },
  { prop: 'bookedCount', label: '已约' },
  { prop: 'remainingCount', label: '剩余' },
  { prop: 'overbookedText', label: '超售' },
  { prop: 'unavailableReason', label: '停诊/不可用原因' },
  { prop: 'commentText', label: '备注' },
  { prop: 'slotId', label: '号源 ID' },
  { prop: 'scheduleId', label: '日程 ID' }
])

function formatTime(value: string) {
  const matched = value.match(/(?:T|\s)(\d{2}:\d{2})/)
  return matched?.[1] ?? value.slice(0, 5)
}

const displayData = computed(() =>
  props.data
    ? {
        ...props.data,
        organizationName: props.data.organizationName || props.data.departmentName || '—',
        practitionerName: props.data.practitionerName || props.data.doctorName || '医师待定',
        healthcareServiceName: props.data.healthcareServiceName || '—',
        timeRange: `${formatTime(props.data.startTime)} — ${formatTime(props.data.endTime)}`,
        overbookedText: props.data.overbookedFlag ? '是' : '否',
        unavailableReason: props.data.unavailableReason || '—',
        commentText: props.data.commentText || '—'
      }
    : null
)
function open() {
  visible.value = true
}
function close() {
  visible.value = false
}
defineExpose({ open, close })
</script>

<template>
  <el-drawer v-model="visible" title="号源详情" size="min(440px, 100%)" destroy-on-close>
    <div v-loading="loading" class="slot-detail">
      <DescriptionList
        v-if="data"
        :data="displayData"
        :items="items"
        :column="1"
        label-width="130px"
        empty-text="—"
      >
        <template #value-status>
          <StatusDisplay
            :value="data.status"
            :options="statusOptions"
            :fallback-label="data.displayStatus || data.statusCode"
          />
        </template>
      </DescriptionList>
      <el-empty v-else description="未找到号源详情" />
    </div>
    <template #footer>
      <div class="slot-detail__actions">
        <el-button @click="close">关闭</el-button>
      </div>
    </template>
  </el-drawer>
</template>

<style scoped>
.slot-detail {
  --el-bg-color: #ffffff;
  --el-fill-color-lighter: #f5f7fa;
  --el-text-color-primary: #303133;
  --el-text-color-regular: #606266;
  --el-text-color-secondary: #909399;

  min-height: 220px;
  color: #303133;
}
.slot-detail :deep(.el-descriptions__label) {
  color: #1f2937 !important;
  background: #f3f6fa !important;
  font-weight: 600 !important;
}
.slot-detail :deep(.el-descriptions__content) {
  color: #303133 !important;
  background: #ffffff !important;
}
.slot-detail :deep(.el-descriptions__table) {
  width: 100%;
  table-layout: fixed;
}
.slot-detail :deep(.el-descriptions__content) {
  overflow-wrap: anywhere;
}
.slot-detail__actions {
  display: flex;
  align-items: center;
  justify-content: flex-end;
  gap: 12px;
}
</style>
