<script setup lang="ts">
import type { InpatientMedicalOrderAction } from '../composables/useInpatientMedicalOrder'

defineOptions({
  name: 'InpatientOrderActionBar'
})

withDefaults(
  defineProps<{
    selectedCount?: number
    loading?: boolean
    availability?: Record<InpatientMedicalOrderAction, boolean>
  }>(),
  {
    selectedCount: 0,
    loading: false,
    availability: () => ({
      delete: false,
      group: false,
      ungroup: false,
      sign: false,
      withdraw: false,
      stop: false,
      cancelStop: false
    })
  }
)

const emit = defineEmits<{
  action: [action: InpatientMedicalOrderAction]
}>()
</script>

<template>
  <div class="inpatient-order-action-bar">
    <div class="inpatient-order-action-bar__summary">
      <strong>医嘱操作</strong>
      <el-tag size="small" effect="plain">已选 {{ selectedCount }} 条</el-tag>
      <span>草稿可签发、删除和成组；停嘱仅适用于已签发长期医嘱</span>
    </div>

    <div class="inpatient-order-action-bar__actions">
      <el-button
        type="primary"
        :loading="loading"
        :disabled="!availability.sign"
        @click="emit('action', 'sign')"
      >
        签发
      </el-button>
      <el-button
        type="warning"
        plain
        :disabled="loading || !availability.withdraw"
        @click="emit('action', 'withdraw')"
      >
        撤回
      </el-button>
      <el-button
        type="warning"
        :disabled="loading || !availability.stop"
        @click="emit('action', 'stop')"
      >
        停嘱
      </el-button>
      <el-button
        type="warning"
        plain
        :disabled="loading || !availability.cancelStop"
        @click="emit('action', 'cancelStop')"
      >
        取消停嘱
      </el-button>
      <el-button :disabled="loading || !availability.group" @click="emit('action', 'group')">
        并组
      </el-button>
      <el-button :disabled="loading || !availability.ungroup" @click="emit('action', 'ungroup')">
        拆组
      </el-button>
      <el-button
        type="danger"
        plain
        :disabled="loading || !availability.delete"
        @click="emit('action', 'delete')"
      >
        删除
      </el-button>
    </div>
  </div>
</template>

<style scoped>
.inpatient-order-action-bar {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  justify-content: space-between;
  gap: 10px 16px;
  padding: 10px 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-extra-light);
}

.inpatient-order-action-bar__summary,
.inpatient-order-action-bar__actions {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 8px;
}

.inpatient-order-action-bar__summary {
  color: var(--el-text-color-secondary, #909399);
  font-size: 12px;
}

.inpatient-order-action-bar__summary strong {
  color: var(--el-text-color-primary, #303133);
  font-size: 14px;
}

.inpatient-order-action-bar__actions :deep(.el-button + .el-button) {
  margin-left: 0;
}
</style>
