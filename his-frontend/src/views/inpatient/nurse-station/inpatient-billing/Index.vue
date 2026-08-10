<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  deleteNurseStationAutoBillingRule,
  queryNurseStationAutoBillingRules,
  saveNurseStationAutoBillingRule,
  type NurseStationAutoBillingDataVO,
  type NurseStationAutoBillingRuleVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationInpatientBilling' })

const props = defineProps<{
  patients: NurseStationPatientSummary[]
}>()

interface AutoRuleForm {
  ruleId?: string
  ruleName: string
  activityId: string
  chargeDefinitionId: string
  quantity: number
}

const selectedPatient = computed(() => (props.patients.length === 1 ? props.patients[0] : null))
const loading = ref(false)
const saving = ref(false)
const activeRequestCount = ref(0)
const autoBillingData = ref<NurseStationAutoBillingDataVO>({ rules: [] })
const autoRuleDialogVisible = ref(false)
const autoRuleForm = reactive<AutoRuleForm>(createEmptyAutoRuleForm())
let requestSerial = 0

function createEmptyAutoRuleForm(): AutoRuleForm {
  return {
    ruleName: '',
    activityId: '',
    chargeDefinitionId: '',
    quantity: 1
  }
}

function formatCurrency(value: number | undefined) {
  return `￥${Number(value ?? 0).toFixed(2)}`
}

async function withLoading<T>(task: () => Promise<T>) {
  activeRequestCount.value += 1
  loading.value = true
  try {
    return await task()
  } finally {
    activeRequestCount.value -= 1
    loading.value = activeRequestCount.value > 0
  }
}

async function loadAutoRules() {
  const patient = selectedPatient.value
  if (!patient) {
    autoBillingData.value = { rules: [] }
    return
  }
  const result = await queryNurseStationAutoBillingRules({
    encounterId: patient.encounterId,
    patientId: patient.patientId
  })
  if (selectedPatient.value?.encounterId !== patient.encounterId) return
  autoBillingData.value = result
}

watch(
  selectedPatient,
  async () => {
    const serial = ++requestSerial
    autoBillingData.value = { rules: [] }
    if (!selectedPatient.value) return
    try {
      await withLoading(loadAutoRules)
      if (serial !== requestSerial) return
    } catch (error) {
      ElMessage.error(error instanceof Error ? error.message : '加载自动计费规则失败')
    }
  },
  { immediate: true }
)

function openAutoRuleDialog(rule?: NurseStationAutoBillingRuleVO) {
  Object.assign(autoRuleForm, createEmptyAutoRuleForm())
  if (rule) {
    Object.assign(autoRuleForm, {
      ruleId: rule.ruleId,
      ruleName: rule.ruleName,
      activityId: rule.activityId,
      chargeDefinitionId: rule.chargeDefinitionId,
      quantity: rule.quantity
    })
  }
  autoRuleDialogVisible.value = true
}

function openAutoRuleDialogFromTable(row: unknown) {
  openAutoRuleDialog(row as NurseStationAutoBillingRuleVO)
}

async function submitAutoRule() {
  const patient = selectedPatient.value
  if (!patient) return
  if (!autoRuleForm.ruleName.trim()) {
    ElMessage.warning('请填写规则名称')
    return
  }
  if (!autoRuleForm.activityId.trim()) {
    ElMessage.warning('请填写触发活动 ID')
    return
  }
  if (!autoRuleForm.chargeDefinitionId.trim()) {
    ElMessage.warning('请填写收费定义 ID')
    return
  }

  saving.value = true
  try {
    const result = await saveNurseStationAutoBillingRule({
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      ruleId: autoRuleForm.ruleId,
      ruleName: autoRuleForm.ruleName.trim(),
      activityId: autoRuleForm.activityId.trim(),
      chargeDefinitionId: autoRuleForm.chargeDefinitionId.trim(),
      quantity: autoRuleForm.quantity
    })
    if (!result.success) {
      ElMessage.warning(result.message)
      return
    }
    ElMessage.success(result.message)
    autoRuleDialogVisible.value = false
    await loadAutoRules()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '保存自动计费规则失败')
  } finally {
    saving.value = false
  }
}

async function deleteAutoRule(rule: NurseStationAutoBillingRuleVO) {
  const patient = selectedPatient.value
  if (!patient) return
  try {
    await ElMessageBox.confirm(`确认删除自动计费规则“${rule.ruleName}”？`, '删除规则', {
      type: 'warning',
      confirmButtonText: '确定删除',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  saving.value = true
  try {
    const result = await deleteNurseStationAutoBillingRule({
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      ruleId: rule.ruleId
    })
    if (!result.success) {
      ElMessage.warning(result.message)
      return
    }
    ElMessage.success(result.message)
    await loadAutoRules()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '删除自动计费规则失败')
  } finally {
    saving.value = false
  }
}

function deleteAutoRuleFromTable(row: unknown) {
  return deleteAutoRule(row as NurseStationAutoBillingRuleVO)
}
</script>

<template>
  <section class="inpatient-billing-module">
    <el-empty v-if="!selectedPatient" description="请先从左侧选择一位患者" />

    <div v-else v-loading="loading" class="billing-workspace">
      <header class="billing-patient-header">
        <div class="patient-identity">
          <strong>{{ selectedPatient.patientName }}</strong>
          <span
            >{{ selectedPatient.genderText || '—' }} / {{ selectedPatient.ageText || '—' }}</span
          >
          <el-tag size="small" effect="plain">{{ selectedPatient.statusText || '在院' }}</el-tag>
        </div>
        <dl class="patient-summary">
          <div>
            <dt>床位</dt>
            <dd>{{ selectedPatient.wardName }} {{ selectedPatient.bedName }}</dd>
          </div>
          <div>
            <dt>住院号</dt>
            <dd>{{ selectedPatient.encounterBusNo || '—' }}</dd>
          </div>
          <div>
            <dt>账户</dt>
            <dd>{{ selectedPatient.accountId || '缺失' }}</dd>
          </div>
          <div class="balance-summary" :class="{ 'is-warning': selectedPatient.lowBalanceWarning }">
            <dt>可用余额</dt>
            <dd>{{ formatCurrency(selectedPatient.availableBalance) }}</dd>
          </div>
        </dl>
        <p class="diagnosis-text">诊断：{{ selectedPatient.diagnosisSummary || '—' }}</p>
      </header>

      <section class="billing-panel auto-billing-panel">
        <div class="panel-toolbar">
          <div>
            <strong>患者自动计费规则</strong>
            <span class="scope-tip">仅展示后端当前已支持的规则字段</span>
          </div>
          <div>
            <el-button @click="loadAutoRules">刷新</el-button>
            <el-button type="primary" @click="openAutoRuleDialog()">新增规则</el-button>
          </div>
        </div>

        <el-table
          :data="autoBillingData.rules"
          border
          stripe
          height="100%"
          row-key="ruleId"
          data-testid="auto-billing-table"
        >
          <el-table-column prop="ruleName" label="规则名称" min-width="180" />
          <el-table-column label="触发活动" min-width="180">
            <template #default="{ row }">{{ row.activityName || row.activityId || '—' }}</template>
          </el-table-column>
          <el-table-column prop="activityId" label="触发活动 ID" min-width="170" />
          <el-table-column prop="chargeDefinitionId" label="收费定义 ID" min-width="170" />
          <el-table-column prop="quantity" label="数量" width="90" align="right" />
          <el-table-column prop="statusText" label="状态" width="90">
            <template #default="{ row }">
              <el-tag :type="row.status === 'ACTIVE' ? 'success' : 'info'">
                {{ row.statusText }}
              </el-tag>
            </template>
          </el-table-column>
          <el-table-column label="操作" width="130" fixed="right">
            <template #default="{ row }">
              <el-button link type="primary" @click="openAutoRuleDialogFromTable(row)"
                >编辑</el-button
              >
              <el-button link type="danger" @click="deleteAutoRuleFromTable(row)">删除</el-button>
            </template>
          </el-table-column>
        </el-table>
      </section>
    </div>

    <el-dialog
      v-model="autoRuleDialogVisible"
      :title="autoRuleForm.ruleId ? '编辑自动计费规则' : '新增自动计费规则'"
      width="600px"
    >
      <el-alert
        title="触发活动和收费项目暂未提供选择接口，请填写后端真实 ID。"
        type="info"
        :closable="false"
      />
      <el-form label-width="112px" class="auto-rule-form">
        <el-form-item label="规则名称" required>
          <el-input v-model="autoRuleForm.ruleName" />
        </el-form-item>
        <el-form-item label="触发活动 ID" required>
          <el-input v-model="autoRuleForm.activityId" placeholder="例如：ACT_MORNING_ROUTINE" />
        </el-form-item>
        <el-form-item label="收费定义 ID" required>
          <el-input
            v-model="autoRuleForm.chargeDefinitionId"
            placeholder="例如：CDEF_NURSING_CARE"
          />
        </el-form-item>
        <el-form-item label="每次数量">
          <el-input-number v-model="autoRuleForm.quantity" :min="0.01" :precision="2" />
        </el-form-item>
      </el-form>
      <template #footer>
        <el-button @click="autoRuleDialogVisible = false">取消</el-button>
        <el-button type="primary" :loading="saving" @click="submitAutoRule">保存</el-button>
      </template>
    </el-dialog>
  </section>
</template>

<style scoped>
.inpatient-billing-module {
  height: 100%;
  min-height: 0;
}

.billing-workspace {
  display: grid;
  grid-template-rows: auto minmax(0, 1fr);
  gap: 12px;
  height: 100%;
  min-height: 0;
}

.billing-patient-header {
  padding: 14px 18px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}

.patient-identity {
  display: flex;
  align-items: center;
  gap: 12px;
}

.patient-identity strong {
  font-size: 18px;
}

.patient-identity span {
  color: var(--el-text-color-secondary);
}

.patient-summary {
  display: grid;
  grid-template-columns: repeat(4, minmax(120px, 1fr));
  gap: 12px;
  margin: 14px 0 0;
}

.patient-summary div {
  display: flex;
  align-items: baseline;
  gap: 8px;
}

.patient-summary dt {
  color: var(--el-text-color-secondary);
}

.patient-summary dd {
  margin: 0;
  font-weight: 600;
}

.balance-summary.is-warning dd {
  color: var(--el-color-danger);
}

.diagnosis-text {
  margin: 12px 0 0;
  color: var(--el-text-color-regular);
}

.billing-panel {
  display: grid;
  grid-template-rows: auto minmax(0, 1fr);
  min-height: 0;
  padding: 14px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}

.panel-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding-bottom: 12px;
}

.scope-tip {
  margin-left: 12px;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.auto-rule-form {
  margin-top: 18px;
}

@media (max-width: 900px) {
  .patient-summary {
    grid-template-columns: repeat(2, minmax(120px, 1fr));
  }

  .panel-toolbar {
    align-items: flex-start;
  }

  .scope-tip {
    display: block;
    margin: 4px 0 0;
  }
}
</style>
