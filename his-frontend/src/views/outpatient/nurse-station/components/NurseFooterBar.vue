<script setup lang="ts">
defineOptions({
  name: 'NurseFooterBar'
})

interface Props {
  executing?: boolean
  selectedCount?: number
  pendingCount?: number
}

withDefaults(defineProps<Props>(), {
  executing: false,
  selectedCount: 0,
  pendingCount: 0
})

const emit = defineEmits<{
  clear: []
  execute: []
}>()
</script>

<template>
  <footer class="nurse-footer-bar">
    <div class="nurse-footer-bar__summary">
      <span
        >待执行 <strong>{{ pendingCount }}</strong> 项</span
      >
      <span
        >已选择 <strong>{{ selectedCount }}</strong> 项</span
      >
    </div>
    <div class="nurse-footer-bar__actions">
      <el-button :disabled="selectedCount === 0 || executing" @click="emit('clear')">
        清空选择
      </el-button>
      <el-button
        type="primary"
        :loading="executing"
        :disabled="selectedCount === 0"
        @click="emit('execute')"
      >
        批量执行
      </el-button>
    </div>
  </footer>
</template>

<style scoped>
.nurse-footer-bar {
  display: flex;
  min-height: 58px;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  padding: 10px 16px;
  border-top: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.nurse-footer-bar__summary,
.nurse-footer-bar__actions {
  display: flex;
  align-items: center;
  gap: 16px;
}

.nurse-footer-bar__summary {
  color: var(--el-text-color-regular);
  font-size: 14px;
}

.nurse-footer-bar__summary strong {
  color: var(--el-color-primary);
}
</style>
