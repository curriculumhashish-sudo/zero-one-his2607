<script setup lang="ts">
import { computed, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  clearDischargedBed,
  executeInpatientDischarge,
  type NurseStationBedOccupantVO
} from '@/apis/inpatient/nurse-station'

defineOptions({ name: 'NurseStationDischargePanel' })

const props = defineProps<{
  patient: NurseStationBedOccupantVO | null
}>()

const emit = defineEmits<{
  changed: []
}>()

const actionLoading = ref(false)
const canDischarge = computed(() => props.patient?.statusEnum === 'ON_HOLD')
const canClearBed = computed(() => props.patient?.statusEnum === 'COMPLETED')

async function handleDischarge() {
  if (!props.patient) return
  try {
    await ElMessageBox.confirm(`确认办理患者“${props.patient.patientName}”出院？`, '办理出院', {
      type: 'warning'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    await executeInpatientDischarge({ encounterId: props.patient.encounterId })
    ElMessage.success('办理出院成功，患者进入待结算状态')
    emit('changed')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '办理出院失败')
  } finally {
    actionLoading.value = false
  }
}

async function handleClearBed() {
  if (!props.patient) return
  try {
    await ElMessageBox.confirm(`确认释放患者“${props.patient.patientName}”当前床位？`, '清床离院', {
      type: 'warning'
    })
  } catch {
    return
  }

  actionLoading.value = true
  try {
    await clearDischargedBed({ encounterId: props.patient.encounterId })
    ElMessage.success('清床离院成功')
    emit('changed')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '清床离院失败')
  } finally {
    actionLoading.value = false
  }
}
</script>

<template>
  <section v-loading="actionLoading" class="discharge-panel">
    <el-alert
      v-if="!patient"
      title="请从上方床位图选择待出院或已完成结算的患者"
      type="info"
      show-icon
      :closable="false"
    />
    <template v-else>
      <div class="discharge-panel__toolbar">
        <div class="discharge-panel__patient">
          <strong>{{ patient.patientName }}</strong>
          <el-tag size="small" effect="plain">{{ patient.statusEnumText || '-' }}</el-tag>
        </div>
        <div>
          <el-button type="success" :disabled="!canDischarge" @click="handleDischarge">
            办理出院
          </el-button>
          <el-button type="primary" :disabled="!canClearBed" @click="handleClearBed">
            清床离院
          </el-button>
        </div>
      </div>

      <el-alert
        title="出院前医嘱、药品和结算条件由后端在提交时校验。"
        type="info"
        show-icon
        :closable="false"
      />
    </template>
  </section>
</template>

<style scoped>
.discharge-panel {
  display: flex;
  min-height: 120px;
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
</style>
