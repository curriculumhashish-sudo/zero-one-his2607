<script setup lang="ts">
import { ref, watch } from 'vue'

import type {
  DoctorMedicalRecordHistoryItemVO,
  DoctorPatientContextVO
} from '@/apis/outpatient/doctor-station'
import type { PatientSummary } from '@/components/patient/type'

import MedicalRecordHistoryPanel from './MedicalRecordHistoryPanel.vue'
import PatientContextPanel from './PatientContextPanel.vue'

defineOptions({
  name: 'DoctorStationAssistantPanel'
})

withDefaults(
  defineProps<{
    patientSelected?: boolean
    patient?: PatientSummary | null
    context?: DoctorPatientContextVO | null
    contextLoading?: boolean
    contextError?: string
    historyRows?: DoctorMedicalRecordHistoryItemVO[]
    historyKeyword?: string
    historyPage?: number
    historyPageSize?: number
    historyTotal?: number
    historyLoading?: boolean
    historyError?: string
  }>(),
  {
    patientSelected: false,
    patient: null,
    context: null,
    contextLoading: false,
    contextError: '',
    historyRows: () => [],
    historyKeyword: '',
    historyPage: 1,
    historyPageSize: 10,
    historyTotal: 0,
    historyLoading: false,
    historyError: ''
  }
)

const emit = defineEmits<{
  retryContext: []
  historyActiveChange: [active: boolean]
  updateHistoryKeyword: [keyword: string]
  searchHistory: [keyword: string]
  changeHistoryPage: [payload: { page: number; pageSize: number }]
  selectHistory: [item: DoctorMedicalRecordHistoryItemVO]
  retryHistory: []
}>()

// 默认展示当前就诊信息；切换到历史病历时再通知父页面按需加载。
const activePane = ref<'history' | 'context'>('context')

watch(activePane, (pane) => {
  emit('historyActiveChange', pane === 'history')
})
</script>

<template>
  <section class="doctor-station-assistant">
    <el-tabs v-model="activePane" class="doctor-station-assistant__tabs" stretch>
      <el-tab-pane label="历史病历" name="history">
        <MedicalRecordHistoryPanel
          :patient-selected="patientSelected"
          :rows="historyRows"
          :keyword="historyKeyword"
          :page="historyPage"
          :page-size="historyPageSize"
          :total="historyTotal"
          :loading="historyLoading"
          :error="historyError"
          @update:keyword="emit('updateHistoryKeyword', $event)"
          @search="emit('searchHistory', $event)"
          @page-change="emit('changeHistoryPage', $event)"
          @select="emit('selectHistory', $event)"
          @retry="emit('retryHistory')"
        />
      </el-tab-pane>

      <el-tab-pane label="就诊信息" name="context">
        <PatientContextPanel
          :patient-selected="patientSelected"
          :patient="patient"
          :context="context"
          :loading="contextLoading"
          :error="contextError"
          @retry="emit('retryContext')"
        />
      </el-tab-pane>
    </el-tabs>
  </section>
</template>

<style scoped>
.doctor-station-assistant {
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  padding: 0 8px 8px;
  color: #303133;
  background: var(--el-bg-color);
}

.doctor-station-assistant__tabs {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
}

.doctor-station-assistant__tabs :deep(.el-tabs__header) {
  flex-shrink: 0;
  margin: 0 0 8px;
}

.doctor-station-assistant__tabs :deep(.el-tabs__item) {
  color: #606266;
}

.doctor-station-assistant__tabs :deep(.el-tabs__item.is-active) {
  color: var(--el-color-primary, #409eff);
  font-weight: 600;
}

.doctor-station-assistant__tabs :deep(.el-tabs__content),
.doctor-station-assistant__tabs :deep(.el-tab-pane) {
  min-height: 0;
  flex: 1;
  overflow: hidden;
}

.doctor-station-assistant__tabs :deep(.el-tabs__content) {
  display: flex;
  flex-direction: column;
}
</style>
