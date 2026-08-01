<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  clearDischargedBed,
  executeInpatientDischarge,
  queryDischargePendingExecutions,
  queryDischargePendingMedications,
  type NurseStationBedOccupantVO,
  type NurseStationDischargePendingExecutionVO,
  type NurseStationDischargePendingMedicationVO
} from '@/apis/inpatient/nurse-station'

defineOptions({ name: 'NurseStationDischargePanel' })

const props = defineProps<{
  patient: NurseStationBedOccupantVO | null
}>()

const emit = defineEmits<{
  changed: []
}>()

const loading = ref(false)
const actionLoading = ref(false)
const therapyFilter = ref<'all' | 'long' | 'temporary'>('all')
const executionRows = ref<NurseStationDischargePendingExecutionVO[]>([])
const medicationRows = ref<NurseStationDischargePendingMedicationVO[]>([])

const filteredExecutionRows = computed(() => {
  if (therapyFilter.value === 'all') return executionRows.value
  const target = therapyFilter.value === 'long' ? 'LONG_TERM' : 'TEMPORARY'
  return executionRows.value.filter((row) => row.therapyEnum === target)
})
const hasBlockingItems = computed(
  () => executionRows.value.length > 0 || medicationRows.value.length > 0
)
const canDischarge = computed(
  () => props.patient?.statusEnum === 'ON_HOLD' && !hasBlockingItems.value
)
const canClearBed = computed(() =>
  ['PENDING', 'COMPLETED'].includes(props.patient?.statusEnum ?? '')
)

async function loadPendingItems() {
  if (!props.patient) {
    executionRows.value = []
    medicationRows.value = []
    return
  }
  loading.value = true
  try {
    const [executions, medications] = await Promise.all([
      queryDischargePendingExecutions(props.patient.encounterId),
      queryDischargePendingMedications(props.patient.encounterId)
    ])
    executionRows.value = executions
    medicationRows.value = medications
  } finally {
    loading.value = false
  }
}

watch(
  () => props.patient?.encounterId,
  () => void loadPendingItems(),
  { immediate: true }
)

async function handleDischarge() {
  if (!props.patient) return
  await ElMessageBox.confirm(`确认办理患者“${props.patient.patientName}”出院？`, '办理出院', {
    type: 'warning'
  })
  actionLoading.value = true
  try {
    await executeInpatientDischarge({ encounterId: props.patient.encounterId })
    ElMessage.success('办理出院成功，患者进入待结算状态')
    emit('changed')
  } finally {
    actionLoading.value = false
  }
}

async function handleClearBed() {
  if (!props.patient) return
  await ElMessageBox.confirm(`确认释放患者“${props.patient.patientName}”当前床位？`, '清床离院', {
    type: 'warning'
  })
  actionLoading.value = true
  try {
    await clearDischargedBed({ encounterId: props.patient.encounterId })
    ElMessage.success('清床离院成功')
    emit('changed')
  } finally {
    actionLoading.value = false
  }
}
</script>

<template>
  <section v-loading="loading || actionLoading" class="discharge-panel">
    <el-alert
      v-if="!patient"
      title="请从上方床位图选择待出院或待结算患者"
      type="info"
      show-icon
      :closable="false"
    />
    <template v-else>
      <div class="discharge-panel__toolbar">
        <div class="discharge-panel__patient">
          <strong>{{ patient.patientName }}</strong>
          <el-tag size="small" effect="plain">{{ patient.statusEnumText || '-' }}</el-tag>
          <el-radio-group v-model="therapyFilter" size="small">
            <el-radio-button value="all">全部</el-radio-button>
            <el-radio-button value="long">长期</el-radio-button>
            <el-radio-button value="temporary">临时</el-radio-button>
          </el-radio-group>
        </div>
        <div>
          <el-button plain @click="loadPendingItems">刷新待办</el-button>
          <el-button type="success" :disabled="!canDischarge" @click="handleDischarge"
            >办理出院</el-button
          >
          <el-button type="primary" :disabled="!canClearBed" @click="handleClearBed"
            >清床离院</el-button
          >
        </div>
      </div>

      <el-alert
        v-if="hasBlockingItems"
        title="仍有未完成医嘱或药品待办，暂不能办理出院"
        type="warning"
        show-icon
        :closable="false"
      />

      <div class="discharge-panel__tables">
        <section>
          <h3>待执行医嘱（{{ filteredExecutionRows.length }}）</h3>
          <el-table :data="filteredExecutionRows" border height="160" empty-text="暂无阻塞医嘱">
            <el-table-column prop="therapyEnum" label="长临" width="90" />
            <el-table-column
              prop="displayName"
              label="医嘱内容"
              min-width="180"
              show-overflow-tooltip
            />
            <el-table-column prop="doseText" label="剂量" width="80" />
            <el-table-column prop="rateCode" label="频次" width="80" />
            <el-table-column prop="blockingReason" label="阻塞原因" min-width="160" />
          </el-table>
        </section>
        <section>
          <h3>待发/待退药（{{ medicationRows.length }}）</h3>
          <el-table :data="medicationRows" border height="160" empty-text="暂无药品待办">
            <el-table-column prop="medicationName" label="药品" min-width="160" />
            <el-table-column prop="statusEnumText" label="状态" width="90" />
            <el-table-column prop="quantity" label="数量" width="70" />
            <el-table-column prop="unitCode" label="单位" width="70" />
            <el-table-column prop="performLocationName" label="库房" min-width="120" />
          </el-table>
        </section>
      </div>
    </template>
  </section>
</template>

<style scoped>
.discharge-panel {
  display: flex;
  min-height: 260px;
  flex-direction: column;
  gap: 10px;
  padding: 12px;
  border-top: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
  color: var(--nurse-text-regular, #606266);
}

.discharge-panel__toolbar,
.discharge-panel__patient {
  display: flex;
  align-items: center;
  gap: 10px;
}

.discharge-panel__toolbar {
  justify-content: space-between;
}

.discharge-panel__patient strong {
  color: var(--nurse-text-primary, #303133);
  font-weight: 600;
}

.discharge-panel__tables {
  display: grid;
  grid-template-columns: minmax(0, 1fr) minmax(0, 1fr);
  gap: 12px;
}

.discharge-panel__tables h3 {
  margin: 0 0 6px;
  color: var(--nurse-text-primary, #303133);
  font-size: 14px;
  font-weight: 500;
}
</style>
