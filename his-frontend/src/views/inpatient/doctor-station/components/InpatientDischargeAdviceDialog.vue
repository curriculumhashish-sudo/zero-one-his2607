<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { WarningFilled } from '@element-plus/icons-vue'
import { ElMessage, type FormInstance, type FormRules } from 'element-plus'

import {
  createInpatientDischargeAdvice,
  getInpatientDischargeAdvicePreparation,
  type InpatientDischargeAdvicePreparationVO,
  type InpatientDischargeAdviceVO,
  type InpatientDoctorPatientQueueItemVO
} from '@/apis/inpatient/doctor-station'
import type { InpatientDoctorStationTab } from '../type'

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
  review: [tab: InpatientDoctorStationTab]
}>()

const visible = defineModel<boolean>({ default: false })
const formRef = ref<FormInstance>()
const preparation = ref<InpatientDischargeAdvicePreparationVO | null>(null)
const loading = ref(false)
const submitting = ref(false)
const error = ref('')
const remindersAcknowledged = ref(false)
let latestRequestId = 0

const form = reactive({
  plannedDischargeTime: '',
  outWayCode: '',
  reasonText: ''
})

const rules: FormRules = {
  plannedDischargeTime: [{ required: true, message: '请选择计划出院时间', trigger: 'change' }],
  outWayCode: [{ required: true, message: '请选择出院方式', trigger: 'change' }]
}

const blockers = computed(
  () => preparation.value?.checks.filter((check) => check.level === 'BLOCKER') ?? []
)
const reminders = computed(
  () => preparation.value?.checks.filter((check) => check.level === 'REMINDER') ?? []
)
const sourceLocationDisplay = computed(() => {
  const patient = props.patient
  if (!patient) return '—'
  return [patient.wardName, patient.bedName].filter(Boolean).join(' · ') || '—'
})
const canSubmit = computed(
  () =>
    Boolean(preparation.value?.canCreate) &&
    (reminders.value.length === 0 || remindersAcknowledged.value) &&
    !loading.value &&
    !submitting.value
)

function resetForm() {
  form.plannedDischargeTime = dayjs().format('YYYY-MM-DD HH:mm:ss')
  form.outWayCode = ''
  form.reasonText = ''
  remindersAcknowledged.value = false
  preparation.value = null
  error.value = ''
  formRef.value?.clearValidate()
}

async function loadPreparation() {
  const patient = props.patient
  if (!patient) {
    preparation.value = null
    error.value = '请先选择患者'
    return
  }

  const requestId = ++latestRequestId
  loading.value = true
  error.value = ''
  try {
    const result = await getInpatientDischargeAdvicePreparation(
      patient.encounterId,
      patient.patientId
    )
    if (requestId !== latestRequestId) return
    preparation.value = result
  } catch (caught) {
    if (requestId !== latestRequestId) return
    preparation.value = null
    error.value = caught instanceof Error ? caught.message : '出院医嘱前置校验失败'
  } finally {
    if (requestId === latestRequestId) loading.value = false
  }
}

function handleReview(tab?: InpatientDoctorStationTab) {
  if (!tab) return
  visible.value = false
  emit('review', tab)
}

async function handleSubmit() {
  const patient = props.patient
  const currentPreparation = preparation.value
  if (!patient || !currentPreparation || !canSubmit.value) return

  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid) return

  submitting.value = true
  try {
    const advice = await createInpatientDischargeAdvice({
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      encounterVersion: currentPreparation.encounterVersion,
      plannedDischargeTime: form.plannedDischargeTime,
      outWayCode: form.outWayCode,
      reasonText: form.reasonText.trim() || undefined,
      sourceLocationId: patient.bedId || patient.wardId
    })
    ElMessage.success('出院医嘱已开立，患者进入待出院状态')
    visible.value = false
    emit('success', advice)
  } catch (caught) {
    ElMessage.error(caught instanceof Error ? caught.message : '开立出院医嘱失败')
    await loadPreparation()
  } finally {
    submitting.value = false
  }
}

watch(visible, (open) => {
  if (!open) {
    latestRequestId += 1
    return
  }
  resetForm()
  void loadPreparation()
})

watch(
  () => props.patient?.encounterId,
  () => {
    if (!visible.value) return
    resetForm()
    void loadPreparation()
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
    <el-skeleton v-if="loading" :rows="8" animated />

    <el-result v-else-if="error" icon="error" title="前置校验加载失败" :sub-title="error">
      <template #extra>
        <el-button type="primary" @click="loadPreparation">重新加载</el-button>
      </template>
    </el-result>

    <template v-else-if="preparation && patient">
      <el-descriptions :column="2" border class="discharge-advice-dialog__summary">
        <el-descriptions-item label="患者">{{ patient.patientName }}</el-descriptions-item>
        <el-descriptions-item label="住院号">{{ patient.inpatientNo }}</el-descriptions-item>
        <el-descriptions-item label="原科室">{{ patient.departmentName }}</el-descriptions-item>
        <el-descriptions-item label="原病区/床位">
          {{ sourceLocationDisplay }}
        </el-descriptions-item>
      </el-descriptions>

      <el-alert
        v-if="blockers.length > 0"
        title="当前不能重复开立出院医嘱"
        type="error"
        :closable="false"
        show-icon
        class="discharge-advice-dialog__alert"
      >
        <template #default>
          <div v-for="check in blockers" :key="check.code" class="discharge-advice-dialog__check">
            <span>{{ check.message }}</span>
            <el-button
              v-if="check.targetTab"
              type="primary"
              link
              @click="handleReview(check.targetTab)"
            >
              前往处理
            </el-button>
          </div>
        </template>
      </el-alert>

      <section v-if="reminders.length > 0" class="discharge-advice-dialog__reminders">
        <header>
          <el-icon color="var(--el-color-warning)"><WarningFilled /></el-icon>
          <strong>开立前请确认</strong>
        </header>
        <div v-for="check in reminders" :key="check.code" class="discharge-advice-dialog__check">
          <span>{{ check.message }}</span>
          <el-button
            v-if="check.targetTab"
            type="primary"
            link
            @click="handleReview(check.targetTab)"
          >
            前往查看
          </el-button>
        </div>
        <el-checkbox v-model="remindersAcknowledged">
          我已核对以上事项，确认继续开立出院医嘱
        </el-checkbox>
      </section>

      <el-form
        ref="formRef"
        :model="form"
        :rules="rules"
        label-width="110px"
        class="discharge-advice-dialog__form"
      >
        <el-form-item label="计划出院时间" prop="plannedDischargeTime">
          <el-date-picker
            v-model="form.plannedDischargeTime"
            type="datetime"
            value-format="YYYY-MM-DD HH:mm:ss"
            format="YYYY-MM-DD HH:mm"
            placeholder="请选择计划出院时间"
            :disabled="!preparation.canCreate || submitting"
          />
        </el-form-item>
        <el-form-item label="出院方式" prop="outWayCode">
          <el-select
            v-model="form.outWayCode"
            placeholder="请选择出院方式"
            filterable
            :disabled="!preparation.canCreate || submitting"
          >
            <el-option
              v-for="item in preparation.outWayOptions"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="出院原因">
          <el-input
            v-model="form.reasonText"
            type="textarea"
            :rows="3"
            maxlength="200"
            show-word-limit
            placeholder="选填"
            :disabled="!preparation.canCreate || submitting"
          />
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
.discharge-advice-dialog__summary,
.discharge-advice-dialog__alert,
.discharge-advice-dialog__reminders {
  margin-bottom: 16px;
}

.discharge-advice-dialog__reminders {
  padding: 12px 14px;
  border: 1px solid var(--el-color-warning-light-5);
  border-radius: var(--el-border-radius-base);
  background: var(--el-color-warning-light-9);
}

.discharge-advice-dialog__reminders header {
  display: flex;
  align-items: center;
  gap: 7px;
  margin-bottom: 8px;
}

.discharge-advice-dialog__check {
  display: flex;
  min-height: 28px;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.discharge-advice-dialog__reminders :deep(.el-checkbox) {
  height: auto;
  margin-top: 8px;
  white-space: normal;
}

.discharge-advice-dialog__form :deep(.el-date-editor),
.discharge-advice-dialog__form :deep(.el-select) {
  width: 100%;
}
</style>
