<script setup lang="ts">
import type {
  LoadRegisterBenefitPreviewResult,
  RegisterInitResult,
  RegisterOption,
  RegisterPatient
} from '@/apis/outpatient/charge/register'
import type { SelectTableLoadPage } from '@/components/common/select-type'

import { computed, ref } from 'vue'

import DescriptionList from '@/components/common/DescriptionList.vue'
import FormSection from '@/components/common/FormSection.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import SectionTitle from '@/components/common/SectionTitle.vue'
import SelectTable from '@/components/common/SelectTable.vue'
import { calculateAge, formatLocalDate, parseResidentIdCard } from '../adapter'
import type { OutpatientRegisterFormModel } from '../type'

type UiRegisterOption = RegisterOption & Record<string, unknown>

const props = defineProps<{
  disabled: boolean
  doctorRequired: boolean
  doctorOptions: UiRegisterOption[]
  init: RegisterInitResult | null
  benefitPreview: LoadRegisterBenefitPreviewResult | null
  benefitLoading: boolean
  loadDoctors: (keyword: string, context: { signal: AbortSignal }) => Promise<UiRegisterOption[]>
  loadPatients: SelectTableLoadPage<RegisterPatient & Record<string, unknown>>
}>()

const emit = defineEmits<{
  appointmentLoad: []
  benefitLoad: []
  contractChange: []
  departmentChange: []
  doctorChange: []
  patientChange: [patient: RegisterPatient]
  refreshSlots: []
}>()

const model = defineModel<OutpatientRegisterFormModel>({ required: true })
const patientVisible = ref(false)
const patientColumns = [
  { prop: 'patientBusNo', label: '病历号', width: 140 },
  { prop: 'name', label: '姓名', width: 100 },
  { prop: 'genderText', label: '性别', width: 70 },
  { prop: 'age', label: '年龄', width: 80 },
  { prop: 'phoneMasked', label: '联系电话', width: 130 },
  { prop: 'idCardNoMasked', label: '身份证号', minWidth: 180 }
]
const appointmentModeOptions = [
  { value: 'WALK_IN', label: '现场挂号' },
  { value: 'APPOINTMENT', label: '预约转挂号' }
]
const insuranceSelected = computed(() =>
  ['3', '4'].includes(props.init?.contractCategoryById?.[model.value.contractId] ?? '')
)
const benefitDetails = computed(() => {
  const preview = props.benefitPreview?.ybPreview
  return {
    psnNo: preview?.psnNo ?? '—',
    insuplcAdmdvs: preview?.insuplcAdmdvs ?? '—',
    insutype: preview?.insutype ?? '—',
    balc: preview?.balc == null ? '—' : `¥${Number(preview.balc).toFixed(2)}`,
    treatmentStatus: preview?.treatmentBlocked ? '待遇检查未通过' : '待遇检查通过'
  }
})
const benefitItems = [
  { prop: 'psnNo', label: '人员编号' },
  { prop: 'insuplcAdmdvs', label: '参保地' },
  { prop: 'insutype', label: '险种' },
  { prop: 'balc', label: '个人账户余额' },
  { prop: 'treatmentStatus', label: '待遇检查', span: 2 }
]

function choosePatient(_value: unknown, source: Array<RegisterPatient & Record<string, unknown>>) {
  const patient = source[0]
  if (patient) emit('patientChange', patient)
}

function handleIdCardInput(value: string) {
  const parsed = parseResidentIdCard(value)
  if (!parsed) return
  model.value.birthDate = parsed.birthDate
  model.value.gender = parsed.gender
  model.value.age = parsed.age
}

function handleBirthDateChange(value: string) {
  model.value.age = calculateAge(value)
}

function handleAgeChange(value: number | undefined) {
  model.value.age = value ?? null
  if (model.value.birthDate || value == null) return
  const date = new Date()
  date.setFullYear(date.getFullYear() - value)
  model.value.birthDate = formatLocalDate(date)
}
</script>

<template>
  <section class="register-form-panel">
    <FormSection>
      <SectionTitle title="患者信息" />
      <div class="patient-actions">
        <el-button type="primary" plain :disabled="disabled" @click="patientVisible = true">
          选择已有患者
        </el-button>
        <span v-if="model.patientId">
          已选：{{ model.name }}（{{ model.patientBusNo || model.patientId }}）
        </span>
        <span v-else>点击按钮打开患者分页选择表格，也可直接录入无档案患者</span>
      </div>
      <el-row :gutter="12">
        <el-col :span="12"
          ><el-form-item label="姓名" required
            ><el-input v-model="model.name" :disabled="disabled" /></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="性别" required
            ><el-select v-model="model.gender" :disabled="disabled"
              ><el-option
                v-for="item in init?.genderOptions ?? []"
                :key="item.value"
                :label="item.label"
                :value="item.value" /></el-select></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="出生日期" required
            ><el-date-picker
              v-model="model.birthDate"
              value-format="YYYY-MM-DD"
              :disabled="disabled"
              @change="handleBirthDateChange" /></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="年龄" required
            ><el-input-number
              v-model="model.age"
              :min="0"
              :max="150"
              :controls="false"
              :disabled="disabled"
              placeholder="由身份证或出生日期自动计算"
              @change="handleAgeChange" /></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="联系电话" required
            ><el-input v-model="model.phone" :disabled="disabled" /></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="身份证号" required
            ><el-input
              v-model="model.idCardNo"
              :disabled="disabled"
              maxlength="18"
              placeholder="输入18位身份证后自动识别出生日期、年龄和性别"
              @input="handleIdCardInput" /></el-form-item
        ></el-col>
      </el-row>
    </FormSection>
    <FormSection>
      <SectionTitle title="挂号信息" />
      <el-row :gutter="12">
        <el-col :span="12"
          ><el-form-item label="挂号方式"
            ><el-select v-model="model.appointmentMode" :disabled="disabled"
              ><el-option
                v-for="item in appointmentModeOptions"
                :key="item.value"
                :label="item.label"
                :value="item.value" /></el-select></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="挂号日期"
            ><el-date-picker
              v-model="model.registrationDate"
              value-format="YYYY-MM-DD"
              :disabled="disabled"
              @change="emit('refreshSlots')" /></el-form-item
        ></el-col>
        <el-col v-if="model.appointmentMode === 'APPOINTMENT'" :span="24">
          <el-form-item label="预约单号" required>
            <div class="inline-field">
              <el-input v-model="model.appointmentId" :disabled="disabled" /><el-button
                :disabled="disabled || !model.appointmentId"
                @click="emit('appointmentLoad')"
                >读取预约</el-button
              >
            </div>
          </el-form-item>
        </el-col>
        <el-col :span="12"
          ><el-form-item label="科室" required
            ><el-select
              v-model="model.organizationId"
              :disabled="disabled"
              @change="emit('departmentChange')"
              ><el-option
                v-for="item in init?.departmentOptions ?? []"
                :key="item.value"
                :label="item.label"
                :value="item.value" /></el-select></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="医生" :required="doctorRequired"
            ><RemoteSelectField
              v-model="model.practitionerId"
              :load-options="props.loadDoctors"
              :initial-options="doctorOptions"
              :disabled="disabled || !model.organizationId"
              @change="emit('doctorChange')" /></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="服务类型" required
            ><el-select
              v-model="model.serviceTypeId"
              :disabled="disabled"
              @change="emit('refreshSlots')"
              ><el-option
                v-for="item in init?.serviceTypeOptions ?? []"
                :key="item.value"
                :label="item.label"
                :value="item.value" /></el-select></el-form-item
        ></el-col>
        <el-col :span="12"
          ><el-form-item label="费用性质" required
            ><el-select
              v-model="model.contractId"
              :disabled="disabled"
              @change="emit('contractChange')"
              ><el-option
                v-for="item in init?.contractOptions ?? []"
                :key="item.value"
                :label="item.label"
                :value="item.value" /></el-select></el-form-item
        ></el-col>
        <el-col :span="24"
          ><el-form-item label="所选号源"
            ><el-input
              :model-value="model.slotId || '请从右侧当日号源中选择'"
              readonly /></el-form-item
        ></el-col>
      </el-row>
    </FormSection>
    <FormSection v-if="insuranceSelected">
      <SectionTitle title="医保待遇" />
      <el-row :gutter="12">
        <el-col :span="8">
          <el-form-item label="医保介质">
            <el-select v-model="model.ybMediumType" :disabled="disabled">
              <el-option label="电子凭证" value="01" />
              <el-option label="社会保障卡" value="02" />
            </el-select>
          </el-form-item>
        </el-col>
        <el-col :span="10">
          <el-form-item label="医保凭证">
            <el-input
              v-model="model.ybCardInfo"
              :disabled="disabled"
              placeholder="读取医保凭证后回填"
            />
          </el-form-item>
        </el-col>
        <el-col :span="6">
          <el-button :loading="benefitLoading" :disabled="disabled" @click="emit('benefitLoad')">
            读取医保待遇
          </el-button>
        </el-col>
        <el-col :span="12">
          <el-form-item label="医保挂号费">
            <el-switch v-model="model.ybRegisterFee" :disabled="disabled" />
          </el-form-item>
        </el-col>
        <el-col :span="12">
          <el-form-item label="使用个账">
            <el-switch v-model="model.ybUseAcctPay" :disabled="disabled" />
          </el-form-item>
        </el-col>
      </el-row>
      <el-alert
        v-if="benefitPreview?.contractMatch?.hintMessage"
        :title="benefitPreview.contractMatch.hintMessage"
        :type="benefitPreview.contractMatch.blockRegister ? 'error' : 'success'"
        :closable="false"
        show-icon
      />
      <DescriptionList
        v-if="benefitPreview?.benefitKind === 'INSURANCE'"
        :data="benefitDetails"
        :items="benefitItems"
        :column="2"
        title="医保待遇信息"
      />
    </FormSection>
    <SelectTable
      :model-value="model.patientId || null"
      v-model:visible="patientVisible"
      row-key="patientId"
      title="选择患者"
      :columns="patientColumns"
      :load-page="loadPatients"
      keyword-placeholder="请输入姓名、病历号或证件号"
      search-label="患者信息"
      @confirm="choosePatient"
    />
  </section>
</template>

<style scoped>
.patient-actions {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-bottom: 14px;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}
.patient-actions span {
  min-width: 0;
  color: var(--el-text-color-regular);
  overflow-wrap: anywhere;
}
.inline-field {
  display: flex;
  width: 100%;
  gap: 8px;
}
:deep(.el-select),
:deep(.el-date-editor),
:deep(.remote-select-field),
:deep(.el-input-number) {
  width: 100%;
}
</style>
