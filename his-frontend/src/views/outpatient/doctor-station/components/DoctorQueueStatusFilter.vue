<script setup lang="ts">
import { DOCTOR_QUEUE_STATUS_OPTIONS } from '../constants'
import type { DoctorQueueStatusFilter } from '../type'

defineOptions({
  name: 'DoctorQueueStatusFilter'
})

withDefaults(
  defineProps<{
    /** 加载队列时暂时禁止切换，避免重复发起相同请求 */
    loading?: boolean
  }>(),
  {
    loading: false
  }
)

// defineModel 让父组件可以使用 v-model 读写当前状态。
const currentStatus = defineModel<DoctorQueueStatusFilter>({ default: 'ALL' })

const emit = defineEmits<{
  change: [status: DoctorQueueStatusFilter]
}>()

/**
 * Element Plus 的值类型比业务类型更宽，因此在组件边界统一收窄。
 */
function handleStatusChange(value: string | number | boolean) {
  const status = value as DoctorQueueStatusFilter
  currentStatus.value = status
  emit('change', status)
}
</script>

<template>
  <section class="doctor-queue-status-filter" aria-label="患者队列状态筛选">
    <el-segmented
      :model-value="currentStatus"
      :options="DOCTOR_QUEUE_STATUS_OPTIONS"
      :disabled="loading"
      block
      @update:model-value="handleStatusChange"
    />
  </section>
</template>

<style scoped>
.doctor-queue-status-filter {
  flex-shrink: 0;
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.doctor-queue-status-filter :deep(.el-segmented) {
  width: 100%;
}

.doctor-queue-status-filter :deep(.el-segmented__item) {
  min-width: 0;
  flex: 1;
}
</style>
