<script setup lang="ts">
import { computed, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox, type FormInstance, type FormRules } from 'element-plus'

import {
  checkInpatientAdmission,
  listInpatientAdmissionWards,
  loadInpatientAdmissionOptions,
  requestInpatientAdmission,
  type CheckInpatientAdmissionVO,
  type DoctorPatientContextVO,
  type InpatientAdmissionFormOptionsVO,
  type InpatientAdmissionWardVO,
  type RequestInpatientAdmissionVO
} from '@/apis/outpatient/doctor-station'
import type {
  ClinicalDiagnosisCandidate,
  ClinicalDiagnosisDictionaryLoader,
  ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import DetailSummary from '@/components/common/DetailSummary.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import type { RemoteSelectLoadOptions, SelectValue } from '@/components/common/select-type'
import type { DetailSummaryItem } from '@/components/common/type'
import type { PatientSummary } from '@/components/patient/type'

defineOptions({
  name: 'InpatientAdmissionDialog'
})

interface InpatientAdmissionDialogProps {
  modelValue: boolean
  patient?: PatientSummary | null
  context?: DoctorPatientContextVO | null
  diagnoses?: ClinicalDiagnosisItem[]
  loadDiagnosisDictionary: ClinicalDiagnosisDictionaryLoader
}

const props = withDefaults(defineProps<InpatientAdmissionDialogProps>(), {
  patient: null,
  context: null,
  diagnoses: () => []
})

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  submitted: [result: RequestInpatientAdmissionVO]
}>()

interface AdmissionFormModel {
  targetDepartmentId: string
  wardId: string
  diagnosisCatalogId: string
  diagnosisDescription: string
  admissionTypeCode: string
  admissionWayCode: string
  priorityCode: string
}

const createEmptyOptions = (): InpatientAdmissionFormOptionsVO => ({
  departmentOptions: [],
  admissionTypeOptions: [],
  admissionWayOptions: [],
  priorityOptions: []
})

const formRef = ref<FormInstance>()
const form = reactive<AdmissionFormModel>({
  targetDepartmentId: '',
  wardId: '',
  diagnosisCatalogId: '',
  diagnosisDescription: '',
  admissionTypeCode: '',
  admissionWayCode: 'OUTPATIENT_TRANSFER',
  priorityCode: ''
})
const formOptions = ref(createEmptyOptions())
const wards = ref<InpatientAdmissionWardVO[]>([])
const selectedDiagnosis = ref<ClinicalDiagnosisCandidate | null>(null)
const existingAdmission = ref<CheckInpatientAdmissionVO>({ exists: false })
const initialLoading = ref(false)
const wardsLoading = ref(false)
const submitting = ref(false)
const loadError = ref('')
const initialFormSnapshot = ref('')

const rules: FormRules<AdmissionFormModel> = {
  targetDepartmentId: [{ required: true, message: '请选择入院科室', trigger: 'change' }],
  diagnosisCatalogId: [{ required: true, message: '请选择入院主诊断', trigger: 'change' }]
}

function serializeForm() {
  return JSON.stringify({
    targetDepartmentId: form.targetDepartmentId,
    wardId: form.wardId,
    diagnosisCatalogId: form.diagnosisCatalogId,
    diagnosisDescription: form.diagnosisDescription.trim(),
    admissionTypeCode: form.admissionTypeCode,
    admissionWayCode: form.admissionWayCode,
    priorityCode: form.priorityCode
  })
}

const formDirty = computed(
  () => Boolean(initialFormSnapshot.value) && serializeForm() !== initialFormSnapshot.value
)

const submittingBlocked = computed(
  () =>
    initialLoading.value ||
    submitting.value ||
    Boolean(loadError.value) ||
    existingAdmission.value.exists ||
    !props.context
)

const existingAdmissionHint = computed(() => {
  if (!existingAdmission.value.exists) return ''
  const statusName =
    existingAdmission.value.statusName || existingAdmission.value.status || '未知状态'
  const inpatientNo = existingAdmission.value.inpatientEncounterNo
  if (existingAdmission.value.status === 'PLANNED') {
    return inpatientNo
      ? `该患者已提交住院申请（住院号 ${inpatientNo}），当前为${statusName}。`
      : `该患者已提交住院申请，当前为${statusName}。`
  }
  return `该门诊就诊已有住院记录，当前为${statusName}，不可重复申请。`
})

function display(value: string | number | null | undefined) {
  const text = value == null ? '' : String(value).trim()
  return text || '—'
}

const patientSummaryItems = computed<DetailSummaryItem[]>(() => [
  { key: 'name', label: '姓名', value: display(props.patient?.name) },
  {
    key: 'genderAge',
    label: '性别/年龄',
    value: [display(props.patient?.gender), display(props.patient?.age)]
      .filter((item) => item !== '—')
      .join(' / ')
  },
  { key: 'phone', label: '联系电话', value: display(props.context?.phone) },
  { key: 'patientNo', label: '患者号', value: display(props.patient?.patientNo) },
  { key: 'visitNo', label: '门诊号', value: display(props.patient?.visitNo) },
  { key: 'department', label: '就诊科室', value: display(props.patient?.departmentName) },
  { key: 'serviceType', label: '号别', value: display(props.patient?.serviceType) },
  { key: 'status', label: '就诊状态', value: display(props.context?.visitStatusName) },
  { key: 'acceptedAt', label: '接诊时间', value: display(props.context?.acceptedAt) },
  {
    key: 'doctor',
    label: '接诊医师',
    value: display(props.context?.attendingDoctorName)
  }
])

const initialDiagnosisOptions = computed<ClinicalDiagnosisCandidate[]>(() =>
  props.diagnoses.map((item) => ({
    id: item.catalogId,
    code: item.code,
    name: item.name,
    typeName: item.typeName
  }))
)

/** 将诊断编辑器的分页加载能力适配成 RemoteSelectField 使用的数组加载能力。 */
const loadDiagnosisOptions: RemoteSelectLoadOptions<ClinicalDiagnosisCandidate> = async (
  keyword,
  context
) => {
  const result = await props.loadDiagnosisDictionary(
    {
      keyword,
      pageIndex: 1,
      pageSize: 30
    },
    context
  )
  return result.rows
}

function resetForm() {
  const primaryDiagnosis =
    props.diagnoses.find((diagnosis) => diagnosis.primary) ?? props.diagnoses[0]

  form.targetDepartmentId = ''
  form.wardId = ''
  form.diagnosisCatalogId = primaryDiagnosis?.catalogId ?? ''
  form.diagnosisDescription = primaryDiagnosis?.name ?? ''
  form.admissionTypeCode = ''
  form.admissionWayCode = 'OUTPATIENT_TRANSFER'
  form.priorityCode = ''
  selectedDiagnosis.value = primaryDiagnosis
    ? {
        id: primaryDiagnosis.catalogId,
        code: primaryDiagnosis.code,
        name: primaryDiagnosis.name,
        typeName: primaryDiagnosis.typeName
      }
    : null
  formOptions.value = createEmptyOptions()
  wards.value = []
  existingAdmission.value = { exists: false }
  loadError.value = ''
  initialFormSnapshot.value = serializeForm()
  formRef.value?.clearValidate()
}

/** 弹窗打开时同时加载固定选项和防重复状态。 */
async function handleOpen() {
  resetForm()
  const encounterId = props.context?.encounterId
  if (!encounterId) {
    loadError.value = '缺少当前门诊就诊信息'
    return
  }

  initialLoading.value = true
  try {
    const [options, existing] = await Promise.all([
      loadInpatientAdmissionOptions(),
      checkInpatientAdmission(encounterId)
    ])
    formOptions.value = options
    existingAdmission.value = existing
  } catch (error) {
    loadError.value = error instanceof Error ? error.message : '办理住院信息加载失败'
  } finally {
    initialLoading.value = false
  }
}

/** 科室改变后原病区不再有效，必须清空并按新科室重新加载。 */
async function handleDepartmentChange(departmentId: SelectValue | null) {
  form.targetDepartmentId = departmentId == null ? '' : String(departmentId)
  form.wardId = ''
  wards.value = []
  if (!form.targetDepartmentId) return

  wardsLoading.value = true
  try {
    wards.value = await listInpatientAdmissionWards(form.targetDepartmentId)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '入院病区加载失败')
  } finally {
    wardsLoading.value = false
  }
}

function handleDiagnosisChange(
  value: SelectValue | SelectValue[] | null,
  option: ClinicalDiagnosisCandidate | ClinicalDiagnosisCandidate[] | undefined
) {
  form.diagnosisCatalogId = value == null || Array.isArray(value) ? '' : String(value)
  selectedDiagnosis.value = !option || Array.isArray(option) ? null : option
  if (selectedDiagnosis.value) form.diagnosisDescription = selectedDiagnosis.value.name
}

async function confirmClose(done?: () => void) {
  if (submitting.value) return

  if (formDirty.value) {
    try {
      await ElMessageBox.confirm('住院申请尚未提交，关闭后填写内容将丢失，是否继续？', '提示', {
        type: 'warning',
        confirmButtonText: '放弃并关闭',
        cancelButtonText: '继续填写'
      })
    } catch {
      return
    }
  }

  if (done) done()
  else emit('update:modelValue', false)
}

function handleDialogModelValue(value: boolean) {
  emit('update:modelValue', value)
}

async function handleSubmit() {
  if (submittingBlocked.value || submitting.value) {
    if (existingAdmission.value.exists) {
      ElMessage.warning(existingAdmissionHint.value || '当前门诊就诊不可重复办理住院')
    }
    return
  }

  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid) return

  const context = props.context
  const diagnosis = selectedDiagnosis.value
  if (!context) {
    ElMessage.warning('缺少当前门诊就诊信息')
    return
  }
  if (!diagnosis) {
    ElMessage.warning('请选择入院主诊断')
    return
  }

  const savedDiagnosis = props.diagnoses.find(
    (item) => item.catalogId === diagnosis.id && item.recordId
  )
  submitting.value = true
  try {
    const result = await requestInpatientAdmission({
      encounterId: context.encounterId,
      patientId: context.patientId,
      targetDepartmentId: form.targetDepartmentId,
      wardId: form.wardId || undefined,
      admissionDiagnoses: [
        {
          catalogId: diagnosis.id,
          diagnosisRecordId: savedDiagnosis?.recordId,
          code: diagnosis.code,
          name: diagnosis.name,
          description: form.diagnosisDescription.trim() || undefined,
          primary: true
        }
      ],
      admissionTypeCode: form.admissionTypeCode || undefined,
      admissionWayCode: form.admissionWayCode || undefined,
      priorityCode: form.priorityCode || undefined
    })

    initialFormSnapshot.value = serializeForm()
    ElMessage.success(`住院申请已提交，住院号 ${result.inpatientEncounterNo}`)
    emit('submitted', result)
    emit('update:modelValue', false)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '住院申请提交失败')
  } finally {
    submitting.value = false
  }
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="办理住院"
    width="720px"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
    :before-close="confirmClose"
    @open="handleOpen"
    @update:model-value="handleDialogModelValue"
  >
    <div v-loading="initialLoading" class="inpatient-admission-dialog">
      <DetailSummary title="患者信息" :items="patientSummaryItems" :column="2" label-width="92px" />

      <el-alert
        v-if="existingAdmission.exists"
        :title="existingAdmissionHint"
        type="warning"
        show-icon
        :closable="false"
      />
      <el-alert v-else-if="loadError" :title="loadError" type="error" show-icon :closable="false" />

      <el-form
        ref="formRef"
        :model="form"
        :rules="rules"
        label-width="112px"
        class="inpatient-admission-dialog__form"
      >
        <el-form-item label="入院科室" prop="targetDepartmentId">
          <el-select
            v-model="form.targetDepartmentId"
            filterable
            clearable
            placeholder="请选择入院科室"
            :disabled="submittingBlocked"
            @change="handleDepartmentChange"
          >
            <el-option
              v-for="option in formOptions.departmentOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>

        <el-form-item label="入院病区">
          <el-select
            v-model="form.wardId"
            filterable
            clearable
            placeholder="请先选择入院科室"
            :loading="wardsLoading"
            :disabled="submittingBlocked || !form.targetDepartmentId"
          >
            <el-option
              v-for="option in wards"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>

        <el-form-item label="入院主诊断" prop="diagnosisCatalogId">
          <RemoteSelectField
            v-model="form.diagnosisCatalogId"
            :load-options="loadDiagnosisOptions"
            :initial-options="initialDiagnosisOptions"
            value-key="id"
            label-key="name"
            placeholder="请输入编码或名称查询诊断"
            :disabled="submittingBlocked"
            @change="handleDiagnosisChange"
          >
            <template #option="{ option }">
              <span>{{ option.code }}　{{ option.name }}</span>
            </template>
          </RemoteSelectField>
        </el-form-item>

        <el-form-item label="诊断说明">
          <el-input
            v-model="form.diagnosisDescription"
            maxlength="200"
            show-word-limit
            placeholder="可补充入院诊断说明"
            :disabled="submittingBlocked"
          />
        </el-form-item>

        <el-form-item label="入院类型">
          <el-select
            v-model="form.admissionTypeCode"
            clearable
            placeholder="请选择"
            :disabled="submittingBlocked"
          >
            <el-option
              v-for="option in formOptions.admissionTypeOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>

        <el-form-item label="入院方式">
          <el-select
            v-model="form.admissionWayCode"
            clearable
            placeholder="请选择"
            :disabled="submittingBlocked"
          >
            <el-option
              v-for="option in formOptions.admissionWayOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>

        <el-form-item label="病情优先级">
          <el-select
            v-model="form.priorityCode"
            clearable
            placeholder="请选择"
            :disabled="submittingBlocked"
          >
            <el-option
              v-for="option in formOptions.priorityOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </el-form-item>
      </el-form>
    </div>

    <template #footer>
      <el-button :disabled="submitting" @click="confirmClose()">取消</el-button>
      <el-button
        type="primary"
        :loading="submitting"
        :disabled="submittingBlocked"
        @click="handleSubmit"
      >
        提交申请
      </el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.inpatient-admission-dialog {
  display: flex;
  min-height: 360px;
  flex-direction: column;
  gap: 16px;
}

.inpatient-admission-dialog :deep(.detail-summary) {
  background: var(--el-fill-color-light);
}

.inpatient-admission-dialog :deep(.detail-summary__body) {
  padding-bottom: 12px;
}

.inpatient-admission-dialog__form {
  padding-top: 4px;
}

.inpatient-admission-dialog__form :deep(.el-form-item) {
  margin-bottom: 16px;
}

.inpatient-admission-dialog__form :deep(.el-select),
.inpatient-admission-dialog__form :deep(.el-input) {
  width: 100%;
}

@media (max-width: 760px) {
  .inpatient-admission-dialog :deep(.el-descriptions__body .el-descriptions__table) {
    table-layout: fixed;
  }
}
</style>
