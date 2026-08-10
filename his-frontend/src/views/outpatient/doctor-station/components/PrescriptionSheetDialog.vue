<script setup lang="ts">
import { ref } from 'vue'

import {
  queryDoctorPrescriptionSheets,
  type DoctorPrescriptionScope,
  type DoctorPrescriptionSheetVO
} from '@/apis/outpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'
import PrescriptionSheetView from '@/components/medical/PrescriptionSheetView.vue'
import type { PatientSummary } from '@/components/patient/type'

defineOptions({ name: 'PrescriptionSheetDialog' })

const props = withDefaults(
  defineProps<{
    modelValue?: boolean
    patient?: PatientSummary | null
  }>(),
  {
    modelValue: false,
    patient: null
  }
)

const emit = defineEmits<{
  'update:modelValue': [visible: boolean]
}>()

const scope = ref<DoctorPrescriptionScope>('CURRENT_ENCOUNTER')
const sheets = ref<DoctorPrescriptionSheetVO[]>([])
const loading = ref(false)
const error = ref('')
let requestSequence = 0

async function loadSheets() {
  const patientNo = String(props.patient?.patientNo ?? '').trim()
  const currentRequest = ++requestSequence
  sheets.value = []
  error.value = ''

  if (!patientNo) {
    error.value = '当前患者缺少病历号，无法查询处方单'
    return
  }

  loading.value = true
  try {
    const result = await queryDoctorPrescriptionSheets({
      num: patientNo,
      type: scope.value
    })
    if (currentRequest !== requestSequence) return
    sheets.value = result
  } catch (loadError) {
    if (currentRequest !== requestSequence) return
    error.value = loadError instanceof Error ? loadError.message : '处方单加载失败'
  } finally {
    if (currentRequest === requestSequence) loading.value = false
  }
}

function handleOpen() {
  scope.value = 'CURRENT_ENCOUNTER'
  void loadSheets()
}

function handleScopeChange(value: string | number | boolean | undefined) {
  scope.value = value === 'HISTORY' ? 'HISTORY' : 'CURRENT_ENCOUNTER'
  void loadSheets()
}

function handleClosed() {
  requestSequence += 1
  scope.value = 'CURRENT_ENCOUNTER'
  sheets.value = []
  loading.value = false
  error.value = ''
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="处方单查看"
    width="min(920px, calc(100vw - 32px))"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
    class="prescription-sheet-dialog"
    @open="handleOpen"
    @closed="handleClosed"
    @update:model-value="emit('update:modelValue', $event)"
  >
    <div class="prescription-sheet-dialog__scope">
      <el-radio-group :model-value="scope" size="small" @change="handleScopeChange">
        <el-radio-button value="CURRENT_ENCOUNTER">本次就诊</el-radio-button>
        <el-radio-button value="HISTORY">历史处方</el-radio-button>
      </el-radio-group>
    </div>

    <div class="prescription-sheet-dialog__content">
      <div v-if="loading" class="prescription-sheet-dialog__skeleton" aria-busy="true">
        <el-skeleton :rows="14" animated />
      </div>

      <EmptyState
        v-else-if="error"
        title="处方单加载失败"
        :description="error"
        action-text="重试"
        @action="loadSheets"
      />

      <template v-else-if="sheets.length > 0">
        <PrescriptionSheetView
          v-for="(sheet, index) in sheets"
          :key="sheet.prescriptionNo || index"
          :data="sheet"
        />
      </template>

      <EmptyState
        v-else
        title="暂无处方单"
        :description="
          scope === 'CURRENT_ENCOUNTER' ? '当前就诊尚无已签发的药品处方' : '该患者暂无历史处方记录'
        "
      />
    </div>

    <template #footer>
      <el-button @click="emit('update:modelValue', false)">关闭</el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.prescription-sheet-dialog__scope {
  display: flex;
  justify-content: center;
  padding-bottom: 12px;
}

.prescription-sheet-dialog__content {
  display: flex;
  max-height: min(720px, 76vh);
  min-height: 480px;
  overflow: auto;
  flex-direction: column;
  gap: 24px;
  padding: 12px;
  background: var(--el-fill-color-light);
  color: var(--el-text-color-primary);
}

.prescription-sheet-dialog__skeleton {
  width: min(100%, 760px);
  margin: 0 auto;
  padding: 24px;
  background: var(--el-bg-color);
}
</style>
