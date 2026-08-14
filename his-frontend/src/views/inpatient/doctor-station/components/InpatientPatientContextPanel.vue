<script setup lang="ts">
import type { InpatientDoctorPatientQueueItemVO } from '@/apis/inpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'

defineOptions({
  name: 'InpatientPatientContextPanel'
})

withDefaults(
  defineProps<{
    patient?: InpatientDoctorPatientQueueItemVO | null
  }>(),
  {
    patient: null
  }
)
</script>

<template>
  <section class="inpatient-context-panel" aria-label="住院信息">
    <header class="inpatient-context-panel__header">住院信息</header>

    <EmptyState
      v-if="!patient"
      compact
      title="未选择患者"
      description="选择在院患者后查看住院信息"
    />

    <el-descriptions v-else :column="1" border size="small">
      <el-descriptions-item label="住院号">{{ patient.inpatientNo }}</el-descriptions-item>
      <el-descriptions-item label="科室">{{ patient.departmentName }}</el-descriptions-item>
      <el-descriptions-item label="病区">{{ patient.wardName }}</el-descriptions-item>
      <el-descriptions-item label="床位">{{ patient.bedName }}</el-descriptions-item>
      <el-descriptions-item label="入院时间">{{ patient.admissionAt }}</el-descriptions-item>
      <el-descriptions-item label="当前诊断">
        {{ patient.diagnosisSummary || '—' }}
      </el-descriptions-item>
    </el-descriptions>
  </section>
</template>

<style scoped>
.inpatient-context-panel {
  box-sizing: border-box;
  height: 100%;
  padding: 12px;
  overflow: auto;
}

.inpatient-context-panel__header {
  margin-bottom: 12px;
  color: var(--el-text-color-primary);
  font-size: 15px;
  font-weight: 600;
}

.inpatient-context-panel :deep(.el-descriptions__label) {
  width: 84px;
  white-space: nowrap;
}
</style>
