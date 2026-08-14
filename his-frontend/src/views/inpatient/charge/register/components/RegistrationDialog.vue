<script setup lang="ts">
import type {
  RegistrationContext,
  LoadRegisterBenefitPreviewParams,
  LoadRegisterBenefitPreviewResult,
  RegistrationOption,
  RegistrationPatientOption,
  SubmitRegistrationPayload,
  WardCapacity,
  WardOption
} from '@/apis/inpatient/charge/register'
import type { SelectTableLoadPage } from '@/components/common/select-type'

import { computed, onBeforeUnmount, reactive, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import DescriptionList from '@/components/common/DescriptionList.vue'
import FormSection from '@/components/common/FormSection.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import SectionTitle from '@/components/common/SectionTitle.vue'
import SelectTable from '@/components/common/SelectTable.vue'
import {
  calculateRegistrationAge,
  parseRegistrationIdCard,
  toRegistrationDetail
} from '../adapter'
import { PATIENT_CONDITION_OPTIONS } from '../constants'
import type { RegistrationInitModel } from '../type'

type UiRegistrationOption = RegistrationOption & Record<string, unknown>

const props = defineProps<{
  modelValue: boolean
  context: RegistrationContext | null
  init: RegistrationInitModel | null
  submitting: boolean
  loadPatients: SelectTableLoadPage<RegistrationPatientOption & Record<string, unknown>>
  loadWards: (organizationId: string, signal: AbortSignal) => Promise<WardOption[]>
  loadDoctors: (
    keyword: string,
    organizationId: string,
    wardId: string,
    signal: AbortSignal
  ) => Promise<UiRegistrationOption[]>
  loadCapacity: (wardId: string) => Promise<WardCapacity>
  loadBenefitPreview: (
    payload: LoadRegisterBenefitPreviewParams,
    context?: { signal?: AbortSignal }
  ) => Promise<LoadRegisterBenefitPreviewResult>
}>()

const emit = defineEmits<{
  'update:modelValue': [value: boolean]
  submit: [payload: Omit<SubmitRegistrationPayload, 'operatorId'>]
}>()

const formRef = ref()
const patientVisible = ref(false)
const selectedPatient = ref<RegistrationPatientOption | null>(null)
const wardOptions = ref<UiRegistrationOption[]>([])
const capacity = ref<WardCapacity | null>(null)
const benefitPreview = ref<LoadRegisterBenefitPreviewResult | null>(null)
const benefitLoading = ref(false)
const dirty = ref(false)
const identityDerivedFromIdCard = ref(false)
let wardLoadController: AbortController | undefined

const form = reactive({
  patientId: '',
  patientName: '',
  genderEnum: 'MALE',
  age: null as number | null,
  birthDate: '',
  idCard: '',
  phone: '',
  organizationId: '',
  wardId: '',
  attendingDoctorId: '',
  startTime: '',
  inWayCode: '1',
  admitSourceCode: '6',
  diagnosisDescription: '',
  contractId: 'CONTRACT-SELF',
  priorityEnum: 'GENERAL',
  advanceDepositAmount: 0,
  payChannelCode: 'CASH',
  psnNo: '',
  insuplcAdmdvs: '',
  insutype: '',
  medType: '21',
  ybCardInfo: '',
  ybMediumType: '02'
})

const readOnly = computed(() => props.context?.readOnly ?? false)
const detail = computed(() => (props.context ? toRegistrationDetail(props.context) : null))
const benefitDetails = computed(() => {
  const preview = benefitPreview.value?.ybPreview
  return {
    psnNo: preview?.psnNo ?? '—',
    insuplcAdmdvs: preview?.insuplcAdmdvs ?? '—',
    insutype: preview?.insutype ?? '—',
    balc: preview?.balc == null ? '—' : `¥${Number(preview.balc).toFixed(2)}`,
    treatmentStatus: preview?.treatmentBlocked ? '待遇检查未通过' : '待遇检查通过'
  }
})
const benefitDetailItems = [
  { prop: 'psnNo', label: '人员编号' },
  { prop: 'insuplcAdmdvs', label: '参保地' },
  { prop: 'insutype', label: '险种' },
  { prop: 'balc', label: '个人账户余额' },
  { prop: 'treatmentStatus', label: '待遇检查', span: 2 }
]
const patientColumns = [
  { prop: 'patientBusNo', label: '病历号', width: 130 },
  { prop: 'name', label: '姓名', width: 100 },
  { prop: 'genderText', label: '性别', width: 70 },
  { prop: 'birthDate', label: '出生日期', width: 110 },
  { prop: 'age', label: '年龄', width: 80 },
  { prop: 'phoneMasked', label: '联系电话', width: 130 },
  { prop: 'idCardNoMasked', label: '身份证号', minWidth: 180 }
]

const detailItems = [
  { prop: 'patientName', label: '患者姓名' },
  { prop: 'patientNo', label: '患者编号' },
  { prop: 'gender', label: '性别' },
  { prop: 'birthDate', label: '出生日期' },
  { prop: 'age', label: '年龄' },
  { prop: 'phone', label: '联系电话' },
  { prop: 'idCard', label: '身份证号', span: 2 },
  { prop: 'inpatientNo', label: '住院号' },
  { prop: 'department', label: '入院科室' },
  { prop: 'ward', label: '病区' },
  { prop: 'bed', label: '床位' },
  { prop: 'outpatientDoctor', label: '门诊医生' },
  { prop: 'attendingDoctor', label: '主治医生' },
  { prop: 'diagnosis', label: '入院诊断', span: 2 },
  { prop: 'patientCondition', label: '患者病情' },
  { prop: 'admissionSource', label: '入院类型' },
  { prop: 'admissionWay', label: '入院方式' },
  { prop: 'admissionTime', label: '入院时间' },
  { prop: 'status', label: '状态' }
]

function resetForm() {
  const context = props.context
  Object.assign(form, {
    patientId: context?.patient?.patientId ?? '',
    patientName: context?.patient?.name ?? '',
    genderEnum: context?.patient?.genderEnum ?? 'MALE',
    age: calculateRegistrationAge(context?.patient?.birthDate),
    birthDate: context?.patient?.birthDate ?? '',
    idCard: context?.patient?.idCard ?? '',
    phone: context?.patient?.phone ?? '',
    organizationId: context?.encounter?.organizationId ?? '',
    wardId: context?.encounter?.wardId ?? '',
    attendingDoctorId: context?.encounter?.attendingDoctorId ?? '',
    startTime:
      context?.encounter?.startTime ?? new Date().toISOString().slice(0, 16).replace('T', ' '),
    inWayCode: context?.encounter?.inWayCode ?? '1',
    admitSourceCode: context?.encounter?.admitSourceCode ?? '6',
    diagnosisDescription: context?.encounter?.primaryConditionDefinitionName ?? '',
    contractId: context?.encounter?.statusEnum === 'ADMITTED' ? 'CONTRACT-MI' : 'CONTRACT-SELF',
    priorityEnum: context?.encounter?.priorityEnum ?? 'GENERAL',
    advanceDepositAmount: 0,
    payChannelCode: 'CASH',
    psnNo: '',
    insuplcAdmdvs: '',
    insutype: '',
    medType: '21',
    ybCardInfo: '',
    ybMediumType: '02'
  })
  selectedPatient.value = context?.patient?.patientId
    ? {
        birthDate: context.patient.birthDate,
        genderCode: context.patient.genderEnum,
        genderText: context.patient.genderEnum,
        idCardNoMasked: context.patient.idCard,
        name: context.patient.name ?? '',
        patientBusNo: context.patient.patientId,
        patientId: context.patient.patientId,
        phoneMasked: context.patient.phone
      }
    : null
  wardOptions.value = context?.encounter?.wardId
    ? [
        {
          value: context.encounter.wardId,
          label: context.encounter.wardName ?? context.encounter.wardId
        }
      ]
    : []
  capacity.value = null
  benefitPreview.value = null
  identityDerivedFromIdCard.value = false
  dirty.value = false
}

watch(
  () => [props.modelValue, props.context] as const,
  ([visible]) => {
    if (visible) resetForm()
  },
  { immediate: true }
)

function markDirty() {
  dirty.value = true
}

async function choosePatient(
  _value: string | number | (string | number)[] | null,
  rows: Array<RegistrationPatientOption & Record<string, unknown>>
) {
  const patient = rows[0]
  if (!patient) return
  if (dirty.value && patient.patientId !== form.patientId) {
    try {
      await ElMessageBox.confirm('切换患者将放弃当前未保存内容，是否继续？', '切换患者确认', {
        type: 'warning'
      })
    } catch {
      return
    }
  }
  selectedPatient.value = patient
  Object.assign(form, {
    patientId: patient.patientId ?? '',
    patientName: patient.name ?? '',
    genderEnum: patient.genderCode ?? 'MALE',
    age: patient.age ?? calculateRegistrationAge(patient.birthDate),
    birthDate: patient.birthDate ?? '',
    idCard: patient.idCardNoMasked ?? '',
    phone: patient.phoneMasked ?? ''
  })
  benefitPreview.value = null
  identityDerivedFromIdCard.value = false
  dirty.value = true
}

function idCardChanged(value: string) {
  const parsed = parseRegistrationIdCard(value)
  if (!parsed) return
  form.birthDate = parsed.birthDate
  form.age = parsed.age
  form.genderEnum = parsed.genderEnum
  identityDerivedFromIdCard.value = true
  markDirty()
}

function birthDateChanged(value: string) {
  form.age = calculateRegistrationAge(value)
  markDirty()
}

function normalizeBirthDate(value: string) {
  return /^\d{4}-\d{2}-\d{2}$/.test(value) ? `${value} 00:00:00` : value
}

async function refreshBenefitPreview() {
  if (!form.contractId) return
  benefitLoading.value = true
  try {
    const result = await props.loadBenefitPreview({
      contractId: form.contractId,
      insuplcAdmdvs: form.insuplcAdmdvs || undefined,
      insutype: form.insutype || undefined,
      medType: form.medType || undefined,
      patientId: form.patientId || undefined,
      psnNo: form.psnNo || undefined,
      runTreatmentCheck: true,
      ybCardInfo: form.ybCardInfo || undefined,
      ybMediumType: form.ybMediumType || undefined,
      ybPsnName: form.patientName || undefined
    })
    benefitPreview.value = result
    form.psnNo = result.ybPreview?.psnNo ?? form.psnNo
    form.insuplcAdmdvs = result.ybPreview?.insuplcAdmdvs ?? form.insuplcAdmdvs
    form.insutype = result.ybPreview?.insutype ?? form.insutype
    if (result.contractMatch?.blockRegister) {
      ElMessage.warning(result.contractMatch.hintMessage || '当前医保待遇不允许办理入院')
    }
  } catch (error) {
    benefitPreview.value = null
    ElMessage.error(error instanceof Error ? error.message : '医保待遇读取失败')
  } finally {
    benefitLoading.value = false
  }
}

function contractChanged() {
  benefitPreview.value = null
  markDirty()
  void refreshBenefitPreview()
}

function applyRecommendedContract() {
  const recommended = benefitPreview.value?.contractMatch?.recommendedContractId
  if (!recommended) return
  form.contractId = recommended
  markDirty()
  void refreshBenefitPreview()
}

async function loadWardOptions(keyword: string, context: { signal: AbortSignal }) {
  if (!form.organizationId) return []
  const rows = await props.loadWards(form.organizationId, context.signal)
  wardOptions.value = rows
    .filter((item) => !keyword || item.wardName.includes(keyword))
    .map((item) => ({ value: item.wardId, label: item.wardName }))
  return wardOptions.value
}

async function loadDoctorOptions(keyword: string, context: { signal: AbortSignal }) {
  if (!form.organizationId || !form.wardId) return []
  return props.loadDoctors(keyword, form.organizationId, form.wardId, context.signal)
}

async function departmentChanged() {
  wardLoadController?.abort()
  form.wardId = ''
  form.attendingDoctorId = ''
  wardOptions.value = []
  capacity.value = null
  markDirty()
  if (!form.organizationId) return
  const controller = new AbortController()
  wardLoadController = controller
  try {
    await loadWardOptions('', { signal: controller.signal })
  } catch (error) {
    if (!controller.signal.aborted) {
      const reason = error instanceof Error ? error.message : '未知错误'
      ElMessage.error(`病区选项加载失败：${reason}`)
    }
  }
}

async function wardChanged() {
  form.attendingDoctorId = ''
  capacity.value = form.wardId ? await props.loadCapacity(form.wardId) : null
  markDirty()
}

async function requestClose() {
  if (!readOnly.value && dirty.value) {
    try {
      await ElMessageBox.confirm('表单存在未保存修改，确认放弃？', '关闭确认', { type: 'warning' })
    } catch {
      return
    }
  }
  emit('update:modelValue', false)
}

function submit() {
  if (readOnly.value || props.submitting) return
  const requiredFields = [
    [form.patientName, '患者'],
    [form.idCard, '证件号码'],
    [form.birthDate, '出生日期'],
    [form.genderEnum, '性别'],
    [form.phone, '联系方式'],
    [form.organizationId, '入院科室'],
    [form.wardId, '入院病区'],
    [form.diagnosisDescription, '入院诊断'],
    [form.contractId, '费用性质'],
    [form.startTime, '入院日期']
  ].filter(([value]) => !value)
  if (requiredFields.length) {
    ElMessage.warning(`请填写：${requiredFields.map(([, label]) => label).join('、')}`)
    return
  }
  if (form.advanceDepositAmount < 0) {
    ElMessage.warning('预交金金额不能小于0')
    return
  }
  emit('submit', {
    admissionDiagnoses: form.diagnosisDescription
      ? [{ description: form.diagnosisDescription, primaryFlag: true }]
      : [],
    admitSourceCode: form.admitSourceCode,
    advanceDepositAmount: form.advanceDepositAmount,
    attendingDoctorId: form.attendingDoctorId || undefined,
    birthDate: normalizeBirthDate(form.birthDate),
    contractId: form.contractId,
    encounterId: props.context?.encounter?.encounterId,
    genderEnum: form.genderEnum,
    idCard: form.idCard,
    inWayCode: form.inWayCode,
    organizationId: form.organizationId,
    patientId: form.patientId || undefined,
    patientName: form.patientName,
    paymentDetails:
      form.advanceDepositAmount > 0
        ? [{ amount: form.advanceDepositAmount, payChannelCode: form.payChannelCode }]
        : [],
    phone: form.phone,
    priorityEnum: form.priorityEnum,
    psnNo: form.psnNo || undefined,
    insuplcAdmdvs: form.insuplcAdmdvs || undefined,
    insutype: form.insutype || undefined,
    medType: form.medType || undefined,
    startTime: form.startTime,
    updatePatientFromIdCard: identityDerivedFromIdCard.value || undefined,
    wardId: form.wardId,
    ybCardInfo: form.ybCardInfo || undefined,
    ybMediumType: form.ybMediumType || undefined
  })
}

onBeforeUnmount(() => wardLoadController?.abort())

defineExpose({ resetDirty: () => (dirty.value = false) })
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    :title="readOnly ? '住院登记详情' : '办理住院登记'"
    width="1120px"
    :close-on-click-modal="false"
    :before-close="requestClose"
    @update:model-value="(value: boolean) => !value && requestClose()"
  >
    <DescriptionList
      v-if="readOnly"
      :data="detail"
      :items="detailItems"
      :column="2"
      title="登记信息"
    />
    <el-form v-else ref="formRef" :model="form" label-width="100px" @change="markDirty">
      <FormSection class="registration-form-section" min-table-height="auto">
        <SectionTitle title="患者信息" />
        <div class="patient-line">
          <el-button type="primary" plain @click="patientVisible = true">选择患者</el-button>
          <span>{{ form.patientName || '尚未选择患者' }}</span>
          <span v-if="form.idCard">{{ form.idCard }}</span>
        </div>
        <el-row :gutter="16">
          <el-col :span="8"
            ><el-form-item label="患者姓名" required
              ><el-input v-model="form.patientName" @input="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="性别" required
              ><el-select v-model="form.genderEnum" @change="markDirty"
                ><el-option label="男" value="MALE" /><el-option
                  label="女"
                  value="FEMALE" /><el-option label="未知" value="UNKNOWN" /></el-select></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="年龄" required
              ><el-input-number
                v-model="form.age"
                :min="0"
                :max="150"
                :controls="false"
                style="width: 100%"
                @change="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="出生日期" required
              ><el-date-picker
                v-model="form.birthDate"
                value-format="YYYY-MM-DD"
                @change="birthDateChanged" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="联系电话" required
              ><el-input v-model="form.phone" @input="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="证件号码" required
              ><el-input
                v-model="form.idCard"
                maxlength="18"
                placeholder="输入18位身份证后自动识别出生日期、年龄和性别"
                @input="idCardChanged" /></el-form-item
          ></el-col>
        </el-row>
      </FormSection>
      <FormSection class="registration-form-section" min-table-height="auto">
        <SectionTitle title="入院信息" />
        <el-row :gutter="16">
          <el-col :span="8">
            <el-form-item label="住院号">
              <el-input :model-value="context?.encounter?.encounterBusNo ?? ''" disabled placeholder="登记后生成" />
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item label="住院次数">
              <el-input model-value="—" disabled />
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item label="入科床位">
              <el-input
                :model-value="[context?.encounter?.wardName, context?.encounter?.bedName].filter(Boolean).join(' / ')"
                disabled
                placeholder="由护士分床后回填"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12"
            ><el-form-item label="入院科室" required
              ><el-select
                v-model="form.organizationId"
                style="width: 100%"
                @change="departmentChanged"
                ><el-option
                  v-for="item in init?.organizationOptions ?? []"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value" /></el-select></el-form-item
          ></el-col>
          <el-col :span="12">
            <el-form-item label="门诊医生">
              <el-input
                :model-value="context?.encounter?.outpatientDoctorName ?? ''"
                disabled
                placeholder="门诊转住院时回填"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12"
            ><el-form-item label="病区" required
              ><RemoteSelectField
                v-model="form.wardId"
                :load-options="loadWardOptions"
                :initial-options="wardOptions"
                :disabled="!form.organizationId"
                @change="wardChanged" /></el-form-item
          ></el-col>
          <el-col :span="12"
            ><el-form-item label="主治医生"
              ><RemoteSelectField
                v-model="form.attendingDoctorId"
                :load-options="loadDoctorOptions"
                :disabled="!form.wardId"
                @change="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="12"
            ><el-form-item label="入院时间" required
              ><el-date-picker
                v-model="form.startTime"
                type="datetime"
                value-format="YYYY-MM-DD HH:mm:ss"
                style="width: 100%"
                @change="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="入院方式"
              ><el-select v-model="form.inWayCode" style="width: 100%" @change="markDirty"
                ><el-option
                  v-for="item in init?.inWayCodeOptions ?? []"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value" /></el-select></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="入院类型"
              ><el-select v-model="form.admitSourceCode" style="width: 100%" @change="markDirty"
                ><el-option
                  v-for="item in init?.admitSourceCodeOptions ?? []"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value" /></el-select></el-form-item
          ></el-col>
          <el-col :span="8">
            <el-form-item label="患者病情">
              <el-select v-model="form.priorityEnum" style="width: 100%" @change="markDirty">
                <el-option
                  v-for="item in PATIENT_CONDITION_OPTIONS"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="24"
            ><el-form-item label="入院诊断" required
              ><el-input
                v-model="form.diagnosisDescription"
                type="textarea"
                :rows="2"
                @input="markDirty" /></el-form-item
          ></el-col>
        </el-row>
        <el-alert
          v-if="capacity"
          type="info"
          :closable="false"
          :title="`病区床位：共 ${capacity.totalBedCount} 张，空余 ${capacity.vacantBedCount} 张`"
        />
      </FormSection>
      <FormSection class="registration-form-section" min-table-height="auto">
        <SectionTitle title="费用与预交金" />
        <el-row :gutter="16">
          <el-col :span="8"
            ><el-form-item label="费用性质" required
              ><el-select v-model="form.contractId" style="width: 100%" @change="contractChanged"
                ><el-option
                  v-for="item in init?.contractOptions ?? []"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value" /></el-select></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="预交金"
              ><el-input-number
                v-model="form.advanceDepositAmount"
                :min="0"
                :precision="2"
                style="width: 100%"
                @change="markDirty" /></el-form-item
          ></el-col>
          <el-col :span="8"
            ><el-form-item label="支付渠道"
              ><el-select
                v-model="form.payChannelCode"
                :disabled="form.advanceDepositAmount <= 0"
                style="width: 100%"
                @change="markDirty"
                ><el-option
                  v-for="item in init?.paymentChannelOptions ?? []"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value" /></el-select></el-form-item
          ></el-col>
        </el-row>
        <el-row :gutter="16">
          <el-col :span="8">
            <el-form-item label="医保介质">
              <el-select v-model="form.ybMediumType" style="width: 100%" @change="markDirty">
                <el-option label="电子凭证" value="01" />
                <el-option label="社会保障卡" value="02" />
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="10">
            <el-form-item label="医保凭证">
              <el-input
                v-model="form.ybCardInfo"
                placeholder="读取医保凭证后回填"
                @input="markDirty"
              />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-button :loading="benefitLoading" @click="refreshBenefitPreview">
              读取医保待遇
            </el-button>
          </el-col>
        </el-row>
        <el-alert
          v-if="benefitPreview?.contractMatch?.hintMessage"
          :title="benefitPreview.contractMatch.hintMessage"
          :type="benefitPreview.contractMatch.blockRegister ? 'error' : 'success'"
          :closable="false"
          show-icon
        >
          <el-button
            v-if="
              benefitPreview.contractMatch.recommendedContractId &&
              benefitPreview.contractMatch.recommendedContractId !== form.contractId
            "
            type="primary"
            link
            @click="applyRecommendedContract"
          >
            采用推荐费用性质：{{ benefitPreview.contractMatch.recommendedContractName }}
          </el-button>
        </el-alert>
        <DescriptionList
          v-if="benefitPreview?.benefitKind === 'INSURANCE'"
          :data="benefitDetails"
          :items="benefitDetailItems"
          :column="2"
          title="医保待遇信息"
        />
      </FormSection>
    </el-form>
    <template #footer>
      <el-button @click="requestClose">关闭</el-button>
      <el-button
        v-if="!readOnly"
        type="primary"
        :loading="submitting"
        :disabled="submitting"
        @click="submit"
        >确认登记</el-button
      >
    </template>
    <SelectTable
      v-model:visible="patientVisible"
      row-key="patientId"
      title="选择患者"
      :columns="patientColumns"
      :load-page="loadPatients"
      keyword-placeholder="请输入姓名、病历号或证件号"
      search-label="患者信息"
      @confirm="choosePatient"
    />
  </el-dialog>
</template>

<style scoped>
.patient-line {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-bottom: 14px;
  color: var(--el-text-color-regular);
}
:deep(.registration-form-section .form-section__table) {
  overflow: visible;
}
:deep(.el-dialog__body) {
  max-height: 72vh;
  overflow-y: auto;
  overflow-x: hidden;
}
:deep(.remote-select-field),
:deep(.el-select),
:deep(.el-date-editor) {
  width: 100%;
}
</style>
