<script setup lang="ts">
import type { NurseStationBedLocationVO } from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../../type'

defineOptions({ name: 'NurseStationBedAssignmentDialog' })

defineProps<{
  modelValue: boolean
  patient: NurseStationPatientSummary | null
  bed: NurseStationBedLocationVO | null
  submitting?: boolean
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  confirm: []
}>()
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="预约床位"
    width="520px"
    destroy-on-close
    @update:model-value="emit('update:modelValue', $event)"
  >
    <el-descriptions :column="1" border>
      <el-descriptions-item label="患者">{{ patient?.patientName || '-' }}</el-descriptions-item>
      <el-descriptions-item label="病区">{{ bed?.wardName || '-' }}</el-descriptions-item>
      <el-descriptions-item label="床位">
        {{ bed?.houseName || '-' }} {{ bed?.bedName || '-' }}
      </el-descriptions-item>
    </el-descriptions>

    <el-alert
      class="assignment-hint"
      title="当前操作调用后端床位预约接口，不包含责任医护、护理级别或入院体征登记。"
      type="info"
      show-icon
      :closable="false"
    />

    <template #footer>
      <el-button @click="emit('update:modelValue', false)">取消</el-button>
      <el-button type="primary" :loading="submitting" @click="emit('confirm')">
        确认预约
      </el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.assignment-hint {
  margin-top: 16px;
}
</style>
