<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'

import {
  createInpatientDischargeAdvice,
  type InpatientDischargeAdviceVO,
  type InpatientDoctorPatientQueueItemVO
} from '@/apis/inpatient/doctor-station'
import { INPATIENT_DISCHARGE_WAY_OPTIONS } from '@/apis/inpatient/doctor-station/adapter'

defineOptions({
  name: 'InpatientDischargeAdviceDialog'
})

const props = withDefaults(
  defineProps<{
    patient?: InpatientDoctorPatientQueueItemVO | null
  }>(),
  {
    patient: null
  }
)

const emit = defineEmits<{
  success: [advice: InpatientDischargeAdviceVO]
}>()

const visible = defineModel<boolean>({ default: false })
const formRef = ref<FormInstance>()
const submitting = ref(false)

const form = reactive({
  outWayCode: ''
})

const rules: FormRules = {
  outWayCode: [{ required: true, message: '请选择出院方式', trigger: 'change' }]
}

const sourceLocationDisplay = computed(() => {
  const patient = props.patient
  if (!patient) return '—'
  return [patient.wardName, patient.bedName].filter(Boolean).join(' · ') || '—'
})
const canSubmit = computed(() => Boolean(props.patient) && !submitting.value)

function resetForm() {
  form.outWayCode = ''
  formRef.value?.clearValidate()
}

async function handleSubmit() {
  const patient = props.patient
  if (!patient || !canSubmit.value) return

  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid) return

  submitting.value = true
  try {
    const advice = await createInpatientDischargeAdvice({
      encounterId: patient.encounterId,
      plannedDischargeTime: dayjs().format('YYYY-MM-DD HH:mm:ss'),
      outWayCode: form.outWayCode
    })
    visible.value = false
    emit('success', advice)
  } catch (caught) {
    ElMessage.error(caught instanceof Error ? caught.message : '开立出院医嘱失败')
  } finally {
    submitting.value = false
  }
}

watch(visible, (open) => {
  if (open) resetForm()
})

watch(
  () => props.patient?.encounterId,
  () => {
    if (visible.value) resetForm()
  }
)
</script>

<template>
  <el-dialog
    v-model="visible"
    title="开立出院医嘱"
    width="620px"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
  >
    <template v-if="patient">
      <el-descriptions :column="2" border class="discharge-advice-dialog__summary">
        <el-descriptions-item label="患者">{{ patient.patientName }}</el-descriptions-item>
        <el-descriptions-item label="住院号">{{ patient.inpatientNo }}</el-descriptions-item>
        <el-descriptions-item label="原科室">{{ patient.departmentName }}</el-descriptions-item>
        <el-descriptions-item label="原病区/床位">
          {{ sourceLocationDisplay }}
        </el-descriptions-item>
      </el-descriptions>

      <el-form
        ref="formRef"
        :model="form"
        :rules="rules"
        label-width="110px"
        class="discharge-advice-dialog__form"
      >
        <el-form-item label="出院方式" prop="outWayCode">
          <el-select
            v-model="form.outWayCode"
            placeholder="请选择出院方式"
            filterable
            :disabled="submitting"
          >
            <el-option
              v-for="item in INPATIENT_DISCHARGE_WAY_OPTIONS"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
      </el-form>
    </template>

    <template #footer>
      <el-button :disabled="submitting" @click="visible = false">取消</el-button>
      <el-button type="primary" :loading="submitting" :disabled="!canSubmit" @click="handleSubmit">
        提交出院医嘱
      </el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.discharge-advice-dialog__summary {
  margin-bottom: 16px;
}

.discharge-advice-dialog__form :deep(.el-select) {
  width: 100%;
}
</style>
