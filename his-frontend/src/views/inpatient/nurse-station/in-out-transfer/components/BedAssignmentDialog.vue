<script setup lang="ts">
import { reactive, watch } from 'vue'

import type {
  NurseStationAdmissionParticipantDTO,
  NurseStationAdmissionVitalSignsDTO,
  NurseStationBedLocationVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../../type'

defineOptions({ name: 'NurseStationBedAssignmentDialog' })

const props = defineProps<{
  modelValue: boolean
  patient: NurseStationPatientSummary | null
  bed: NurseStationBedLocationVO | null
  submitting?: boolean
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  confirm: [
    payload: {
      priorityEnum: string
      participants: NurseStationAdmissionParticipantDTO[]
      admissionVitalSigns: NurseStationAdmissionVitalSignsDTO
    }
  ]
}>()

const form = reactive({
  priorityEnum: 'NORMAL',
  doctorId: 'doctor-001',
  nurseId: 'nurse-001',
  temperature: '36.5',
  pulse: '80',
  heartRate: '80',
  bloodPressureHigh: '120',
  bloodPressureLow: '80',
  heightCm: '168',
  weightKg: '60'
})

// 当前阶段使用稳定 Mock 选项，真实接口完成后改由页面初始化接口提供。
const doctorOptions = [
  { label: '李医生', value: 'doctor-001' },
  { label: '王医生', value: 'doctor-002' }
]
const nurseOptions = [
  { label: '周护士', value: 'nurse-001' },
  { label: '陈护士', value: 'nurse-002' }
]

watch(
  () => props.modelValue,
  (open) => {
    if (!open) return
    Object.assign(form, {
      priorityEnum: 'NORMAL',
      doctorId: 'doctor-001',
      nurseId: 'nurse-001',
      temperature: '36.5',
      pulse: '80',
      heartRate: '80',
      bloodPressureHigh: '120',
      bloodPressureLow: '80',
      heightCm: '168',
      weightKg: '60'
    })
  }
)

function handleConfirm() {
  emit('confirm', {
    priorityEnum: form.priorityEnum,
    participants: [
      { practitionerId: form.doctorId, roleCode: 'ATTENDING_DOCTOR' },
      { practitionerId: form.nurseId, roleCode: 'PRIMARY_NURSE' }
    ],
    admissionVitalSigns: {
      temperature: form.temperature,
      pulse: form.pulse,
      heartRate: form.heartRate,
      bloodPressureHigh: form.bloodPressureHigh,
      bloodPressureLow: form.bloodPressureLow,
      heightCm: form.heightCm,
      weightKg: form.weightKg
    }
  })
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="入科选床"
    width="680px"
    destroy-on-close
    @update:model-value="emit('update:modelValue', $event)"
  >
    <el-descriptions :column="3" border class="assignment-summary">
      <el-descriptions-item label="患者">{{ patient?.patientName || '-' }}</el-descriptions-item>
      <el-descriptions-item label="病区">{{ bed?.wardName || '-' }}</el-descriptions-item>
      <el-descriptions-item label="床位"
        >{{ bed?.houseName }} {{ bed?.bedName }}</el-descriptions-item
      >
    </el-descriptions>

    <el-form :model="form" label-width="92px" class="assignment-form">
      <el-row :gutter="16">
        <el-col :span="8">
          <el-form-item label="护理级别">
            <el-select v-model="form.priorityEnum">
              <el-option label="普通" value="NORMAL" />
              <el-option label="一级护理" value="LEVEL_ONE" />
              <el-option label="特级护理" value="SPECIAL" />
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="8">
          <el-form-item label="主治医生">
            <el-select v-model="form.doctorId">
              <el-option v-for="item in doctorOptions" :key="item.value" v-bind="item" />
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="8">
          <el-form-item label="责任护士">
            <el-select v-model="form.nurseId">
              <el-option v-for="item in nurseOptions" :key="item.value" v-bind="item" />
            </el-select>
          </el-form-item>
        </el-col>
      </el-row>
      <el-divider content-position="left">入院体征</el-divider>
      <el-row :gutter="16">
        <el-col :span="8"
          ><el-form-item label="体温"><el-input v-model="form.temperature" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="脉搏"><el-input v-model="form.pulse" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="心率"><el-input v-model="form.heartRate" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="收缩压"><el-input v-model="form.bloodPressureHigh" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="舒张压"><el-input v-model="form.bloodPressureLow" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="身高(cm)"><el-input v-model="form.heightCm" /></el-form-item
        ></el-col>
        <el-col :span="8"
          ><el-form-item label="体重(kg)"><el-input v-model="form.weightKg" /></el-form-item
        ></el-col>
      </el-row>
    </el-form>

    <template #footer>
      <el-button @click="emit('update:modelValue', false)">取消</el-button>
      <el-button type="primary" :loading="submitting" @click="handleConfirm">确认分床</el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.assignment-summary {
  margin-bottom: 18px;
}

.assignment-form :deep(.el-select) {
  width: 100%;
}
</style>
