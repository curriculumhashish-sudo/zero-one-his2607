<script setup lang="ts">
import type { SlotRecord } from '@/apis/booking/slot-calendar'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'
import type { StatusDisplayOption } from '@/components/common/type'

import { computed, ref } from 'vue'

import DescriptionList from '@/components/common/DescriptionList.vue'
import MyDialog from '@/components/mydialog/MyDialog.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'

const props = defineProps<{ data: SlotRecord | null; loading?: boolean }>()
const dialog = ref<{ openDialog: () => void; closeDialog: () => void }>()

const statusOptions: StatusDisplayOption[] = [
  { value: 'available', label: '可约', type: 'success' },
  { value: 'full', label: '已满', type: 'danger' },
  { value: 'stopped', label: '停诊', type: 'warning' },
  { value: 'expired', label: '过期', type: 'info' }
]

const items = computed<DescriptionItem[]>(() => [
  { prop: 'departmentName', label: '科室' },
  { prop: 'doctorName', label: '医生' },
  { prop: 'roomName', label: '诊室' },
  { prop: 'date', label: '日期' },
  { prop: 'timeRange', label: '时间' },
  { prop: 'group', label: '时段' },
  { prop: 'capacity', label: '总号数' },
  { prop: 'used', label: '已用号数' },
  { prop: 'remaining', label: '剩余号数' },
  {
    prop: 'unavailableReason',
    label: props.data?.status === 'stopped' ? '停诊原因' : '不可用原因',
    span: 2,
    hidden: (data) => !data.unavailableReason
  }
])

const displayData = computed(() =>
  props.data
    ? { ...props.data, timeRange: `${props.data.startTime} - ${props.data.endTime}` }
    : null
)

function open() {
  dialog.value?.openDialog()
}
function close() {
  dialog.value?.closeDialog()
}
defineExpose({ open, close })
</script>

<template>
  <MyDialog ref="dialog" :data="data ?? {}" title="号源详情" width="620px" align-center>
    <div v-loading="loading" class="slot-detail">
      <div v-if="data" class="slot-detail__status">
        <span>号源状态</span>
        <StatusDisplay :value="data.status" :options="statusOptions" />
      </div>
      <DescriptionList v-if="data" :data="displayData" :items="items" :column="2" dense />
      <el-empty v-else description="未找到号源详情" />
    </div>
    <template #footer>
      <div class="slot-detail__actions">
        <span>预约登记功能将在后续版本开放</span>
        <el-button type="primary" @click="close">关闭</el-button>
      </div>
    </template>
  </MyDialog>
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
.slot-detail__status {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 16px;
  padding: 12px;
  color: #303133;
  background: #f5f7fa;
  border-radius: 6px;
}
.slot-detail :deep(.el-descriptions__label) {
  color: #606266 !important;
  background: #f5f7fa !important;
}
.slot-detail :deep(.el-descriptions__content) {
  color: #303133 !important;
  background: #ffffff !important;
}
.slot-detail__actions {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  color: #606266;
  font-size: 13px;
}
</style>
