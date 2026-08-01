<script setup lang="ts">
import { computed, reactive, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  cancelNurseStationBilling,
  confirmNurseStationBilling,
  deleteNurseStationAutoBillingRule,
  queryNurseStationAutoBillingRules,
  queryNurseStationBillingCatalog,
  queryNurseStationPricedOrders,
  queryNurseStationSupplementOrders,
  saveNurseStationAutoBillingRule,
  setNurseStationAutoBillingRuleStatus,
  type NurseStationAutoBillingDataVO,
  type NurseStationAutoBillingRuleVO,
  type NurseStationBillingCatalogCategory,
  type NurseStationBillingCatalogItemVO,
  type NurseStationBillingPricingMode,
  type NurseStationPricedOrderVO,
  type NurseStationSupplementOrderVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationInpatientBilling' })

const props = defineProps<{
  /** 住院记账只能处理一个住院账户，因此该数组正常情况下只有一位患者。 */
  patients: NurseStationPatientSummary[]
}>()

type BillingTab = 'auto' | 'direct' | 'order'

interface SelectedCatalogItem {
  clientItemId: string
  catalog: NurseStationBillingCatalogItemVO
  quantity: number
  unitCode: string
}

interface AutoRuleForm {
  ruleId?: string
  version?: number
  ruleName: string
  activityId: string
  activityName: string
  chargeDefinitionId: string
  chargeDefinitionName: string
  quantity: number
  priority: number
  remark: string
}

const selectedPatient = computed(() => (props.patients.length === 1 ? props.patients[0] : null))
const activeTab = ref<BillingTab>('direct')
const loading = ref(false)
const activeRequestCount = ref(0)
const saving = ref(false)
const pricedOrders = ref<NurseStationPricedOrderVO[]>([])
const selectedPricedOrders = ref<NurseStationPricedOrderVO[]>([])
const pricingTimeRange = ref<string[]>([])
const supplementOrders = ref<NurseStationSupplementOrderVO[]>([])
const orderTherapyType = ref<'ALL' | 'LONG' | 'TEMP'>('ALL')
const orderKeyword = ref('')
const pricingDialogVisible = ref(false)
const pricingMode = ref<NurseStationBillingPricingMode>('DIRECT')
const supplementParent = ref<NurseStationSupplementOrderVO | null>(null)
const catalogCategory = ref<NurseStationBillingCatalogCategory>('DEVICE')
const catalogKeyword = ref('')
const catalogRows = ref<NurseStationBillingCatalogItemVO[]>([])
const catalogPageIndex = ref(1)
const catalogTotal = ref(0)
const selectedCatalogItems = ref<SelectedCatalogItem[]>([])
const cancelDialogVisible = ref(false)
const cancelReason = ref('护士站误操作')
const autoBillingData = ref<NurseStationAutoBillingDataVO>({
  nursingLevelReadonly: null,
  rules: []
})
const autoRuleDialogVisible = ref(false)
const autoRuleForm = reactive<AutoRuleForm>(createEmptyAutoRuleForm())
let requestSerial = 0

const selectedAmount = computed(() =>
  selectedCatalogItems.value.reduce((sum, item) => sum + item.catalog.unitPrice * item.quantity, 0)
)
const pricedAmount = computed(() =>
  pricedOrders.value
    .filter((row) => row.chargeStatus !== 'CANCELLED')
    .reduce((sum, row) => sum + row.totalPrice, 0)
)
const cancellableSelection = computed(() =>
  selectedPricedOrders.value.filter((row) => row.cancellable)
)

function createEmptyAutoRuleForm(): AutoRuleForm {
  return {
    ruleName: '',
    activityId: '',
    activityName: '',
    chargeDefinitionId: '',
    chargeDefinitionName: '',
    quantity: 1,
    priority: 10,
    remark: ''
  }
}

function createIdempotencyKey(action: string) {
  return `${action}-${Date.now()}-${Math.random().toString(36).slice(2, 10)}`
}

function formatCurrency(value: number | undefined) {
  return `￥${Number(value ?? 0).toFixed(2)}`
}

/**
 * 多个面板可能同时请求数据。计数器只有在全部请求结束后才关闭遮罩，避免一个较快请求
 * 提前把另一个仍在进行的请求遮罩移除。
 */
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

async function loadPricedOrders() {
  const patient = selectedPatient.value
  if (!patient) {
    pricedOrders.value = []
    return
  }
  const [pricedTimeFrom, pricedTimeTo] = pricingTimeRange.value
  const rows = await queryNurseStationPricedOrders({
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    pricedTimeFrom: pricedTimeFrom ? `${pricedTimeFrom} 00:00:00` : undefined,
    pricedTimeTo: pricedTimeTo ? `${pricedTimeTo} 23:59:59` : undefined
  })
  // 请求返回前患者可能已切换；此时丢弃旧患者结果，避免费用串到新患者页面。
  if (selectedPatient.value?.encounterId !== patient.encounterId) return
  pricedOrders.value = rows
  selectedPricedOrders.value = []
}

async function loadSupplementOrders() {
  const patient = selectedPatient.value
  if (!patient) {
    supplementOrders.value = []
    return
  }
  const rows = await queryNurseStationSupplementOrders({
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    therapyType: orderTherapyType.value,
    keyword: orderKeyword.value.trim() || undefined
  })
  if (selectedPatient.value?.encounterId !== patient.encounterId) return
  supplementOrders.value = rows
}

async function loadAutoRules() {
  const patient = selectedPatient.value
  if (!patient) {
    autoBillingData.value = { nursingLevelReadonly: null, rules: [] }
    return
  }
  const result = await queryNurseStationAutoBillingRules({
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    billDate: dayjs().format('YYYY-MM-DD')
  })
  if (selectedPatient.value?.encounterId !== patient.encounterId) return
  autoBillingData.value = result
}

/** 患者改变时清空旧患者数据，再并行载入三个页签所需信息。 */
watch(
  selectedPatient,
  async () => {
    const serial = ++requestSerial
    pricedOrders.value = []
    supplementOrders.value = []
    autoBillingData.value = { nursingLevelReadonly: null, rules: [] }
    if (!selectedPatient.value) return
    try {
      await withLoading(() =>
        Promise.all([loadPricedOrders(), loadSupplementOrders(), loadAutoRules()])
      )
      if (serial !== requestSerial) return
    } catch (error) {
      ElMessage.error(error instanceof Error ? error.message : '加载住院记账数据失败')
    }
  },
  { immediate: true }
)

async function loadCatalog() {
  const patient = selectedPatient.value
  if (!patient) return
  const result = await queryNurseStationBillingCatalog({
    pageIndex: catalogPageIndex.value,
    pageSize: 10,
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    category: catalogCategory.value,
    keyword: catalogKeyword.value.trim() || undefined
  })
  if (selectedPatient.value?.encounterId !== patient.encounterId) return
  catalogRows.value = result.rows ?? []
  catalogTotal.value = result.total
}

function openPricingDialog(parent?: NurseStationSupplementOrderVO) {
  const patient = selectedPatient.value
  if (!patient?.accountId) {
    ElMessage.warning('当前患者缺少有效住院账户，暂时不能划价')
    return
  }
  pricingMode.value = parent ? 'SUPPLEMENT' : 'DIRECT'
  supplementParent.value = parent ?? null
  catalogCategory.value = 'DEVICE'
  catalogKeyword.value = ''
  catalogPageIndex.value = 1
  selectedCatalogItems.value = []
  pricingDialogVisible.value = true
  void withLoading(loadCatalog)
}

/** Element Plus 表格插槽把 row 声明为通用对象，这个适配函数只负责恢复业务行类型。 */
function openSupplementDialogFromTable(row: unknown) {
  openPricingDialog(row as NurseStationSupplementOrderVO)
}

function handleCatalogCategoryChange() {
  catalogPageIndex.value = 1
  void withLoading(loadCatalog)
}

/** 点击目录行加入右侧清单；重复点击同一项目时只累加数量。 */
function addCatalogItem(row: NurseStationBillingCatalogItemVO) {
  const existing = selectedCatalogItems.value.find(
    (item) => item.catalog.productDefinitionId === row.productDefinitionId
  )
  if (existing) {
    existing.quantity += 1
    return
  }
  selectedCatalogItems.value.push({
    clientItemId: `billing-row-${Date.now()}-${Math.random().toString(36).slice(2, 6)}`,
    catalog: row,
    quantity: 1,
    unitCode: row.unitCode
  })
}

function removeCatalogItem(clientItemId: string) {
  selectedCatalogItems.value = selectedCatalogItems.value.filter(
    (item) => item.clientItemId !== clientItemId
  )
}

async function submitPricing() {
  const patient = selectedPatient.value
  if (!patient?.accountId) return
  if (!selectedCatalogItems.value.length) {
    ElMessage.warning('请先选择收费项目')
    return
  }
  if (selectedCatalogItems.value.some((item) => item.quantity <= 0)) {
    ElMessage.warning('项目数量必须大于 0')
    return
  }

  saving.value = true
  try {
    const result = await confirmNurseStationBilling({
      idempotencyKey: createIdempotencyKey('confirm-billing'),
      mode: pricingMode.value,
      items: selectedCatalogItems.value.map((item) => ({
        clientItemId: item.clientItemId,
        encounterId: patient.encounterId,
        patientId: patient.patientId,
        accountId: patient.accountId!,
        productTableName: item.catalog.productTableName,
        productDefinitionId: item.catalog.productDefinitionId,
        chargeItemDefinitionId: item.catalog.chargeItemDefinitionId,
        quantity: item.quantity,
        unitCode: item.unitCode,
        previewUnitPrice: item.catalog.unitPrice,
        performLocationId: item.catalog.defaultDispenseLocationId,
        parentOrderAnchor: supplementParent.value
          ? {
              orderType: supplementParent.value.orderType,
              requestId: supplementParent.value.requestId
            }
          : undefined
      }))
    })
    if (result.processedCount) ElMessage.success(`成功划价 ${result.processedCount} 个项目`)
    if (!result.success) {
      ElMessage.warning(result.results.find((item) => !item.success)?.message ?? '部分项目划价失败')
    }
    pricingDialogVisible.value = false
    await Promise.all([loadPricedOrders(), loadSupplementOrders()])
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '划价失败')
  } finally {
    saving.value = false
  }
}

function handlePricedSelection(rows: NurseStationPricedOrderVO[]) {
  selectedPricedOrders.value = rows
}

function openCancelDialog() {
  if (!cancellableSelection.value.length) {
    ElMessage.warning('请勾选至少一条可以取消的划价记录')
    return
  }
  cancelReason.value = '护士站误操作'
  cancelDialogVisible.value = true
}

async function submitCancel() {
  if (!cancelReason.value.trim()) {
    ElMessage.warning('请填写取消原因')
    return
  }
  saving.value = true
  try {
    const result = await cancelNurseStationBilling({
      idempotencyKey: createIdempotencyKey('cancel-billing'),
      reason: cancelReason.value.trim(),
      items: cancellableSelection.value.map((row) => ({
        requestId: row.requestId,
        orderType: row.orderType,
        encounterId: row.encounterId
      }))
    })
    if (result.processedCount) ElMessage.success(`已取消 ${result.processedCount} 条划价记录`)
    if (!result.success) {
      ElMessage.warning(result.results.find((item) => !item.success)?.message ?? '部分记录取消失败')
    }
    cancelDialogVisible.value = false
    await Promise.all([loadPricedOrders(), loadSupplementOrders()])
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '取消划价失败')
  } finally {
    saving.value = false
  }
}

function openAutoRuleDialog(rule?: NurseStationAutoBillingRuleVO) {
  Object.assign(autoRuleForm, createEmptyAutoRuleForm())
  if (rule) {
    Object.assign(autoRuleForm, {
      ruleId: rule.ruleId,
      version: rule.version,
      ruleName: rule.ruleName,
      activityId: rule.activityId,
      activityName: rule.activityName,
      chargeDefinitionId: rule.chargeDefinitionId,
      chargeDefinitionName: rule.chargeDefinitionName,
      quantity: rule.quantity,
      priority: rule.priority,
      remark: rule.remark ?? ''
    })
  }
  autoRuleDialogVisible.value = true
}

/** 表格插槽行类型适配：打开自动计费规则编辑框。 */
function openAutoRuleDialogFromTable(row: unknown) {
  openAutoRuleDialog(row as NurseStationAutoBillingRuleVO)
}

async function submitAutoRule() {
  const patient = selectedPatient.value
  if (!patient) return
  if (!autoRuleForm.ruleName.trim() || !autoRuleForm.activityName.trim()) {
    ElMessage.warning('请填写规则名称和触发项目')
    return
  }
  if (!autoRuleForm.chargeDefinitionName.trim()) {
    ElMessage.warning('请填写收费项目')
    return
  }
  saving.value = true
  try {
    const result = await saveNurseStationAutoBillingRule({
      idempotencyKey: createIdempotencyKey('save-auto-billing'),
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      ruleId: autoRuleForm.ruleId,
      expectedVersion: autoRuleForm.version,
      ruleName: autoRuleForm.ruleName,
      activityId: autoRuleForm.activityId || `activity-${Date.now()}`,
      activityName: autoRuleForm.activityName,
      chargeDefinitionId: autoRuleForm.chargeDefinitionId || `charge-${Date.now()}`,
      chargeDefinitionName: autoRuleForm.chargeDefinitionName,
      quantity: autoRuleForm.quantity,
      priority: autoRuleForm.priority,
      remark: autoRuleForm.remark
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

async function changeAutoRuleStatus(rule: NurseStationAutoBillingRuleVO) {
  const patient = selectedPatient.value
  if (!patient) return
  const targetStatus = rule.status === 'ACTIVE' ? 'INACTIVE' : 'ACTIVE'
  const result = await setNurseStationAutoBillingRuleStatus({
    idempotencyKey: createIdempotencyKey('status-auto-billing'),
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    ruleId: rule.ruleId,
    expectedVersion: rule.version,
    targetStatus
  })
  if (!result.success) {
    ElMessage.warning(result.message)
    return
  }
  ElMessage.success(result.message)
  await loadAutoRules()
}

/** 表格插槽行类型适配：修改规则状态。 */
function changeAutoRuleStatusFromTable(row: unknown) {
  return changeAutoRuleStatus(row as NurseStationAutoBillingRuleVO)
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
  const result = await deleteNurseStationAutoBillingRule({
    idempotencyKey: createIdempotencyKey('delete-auto-billing'),
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    ruleId: rule.ruleId,
    expectedVersion: rule.version
  })
  if (!result.success) {
    ElMessage.warning(result.message)
    return
  }
  ElMessage.success(result.message)
  await loadAutoRules()
}

/** 表格插槽行类型适配：删除规则。 */
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
            <dt>费别</dt>
            <dd>{{ selectedPatient.paymentTypeText || '自费' }}</dd>
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

      <nav class="billing-tabs" aria-label="住院记账业务分类">
        <button :class="{ 'is-active': activeTab === 'order' }" @click="activeTab = 'order'">
          医嘱计费
        </button>
        <button :class="{ 'is-active': activeTab === 'direct' }" @click="activeTab = 'direct'">
          非医嘱计费
        </button>
        <button :class="{ 'is-active': activeTab === 'auto' }" @click="activeTab = 'auto'">
          自动计费
        </button>
      </nav>

      <section v-if="activeTab === 'direct'" class="billing-panel direct-billing-panel">
        <div class="panel-toolbar">
          <div class="toolbar-left">
            <el-date-picker
              v-model="pricingTimeRange"
              type="daterange"
              value-format="YYYY-MM-DD"
              range-separator="至"
              start-placeholder="划价开始日期"
              end-placeholder="划价结束日期"
              @change="loadPricedOrders"
            />
            <el-button @click="loadPricedOrders">查询</el-button>
          </div>
          <div>
            <el-button type="danger" plain @click="openCancelDialog">取消划价</el-button>
            <el-button type="primary" @click="openPricingDialog()">批量划价</el-button>
          </div>
        </div>

        <el-table
          :data="pricedOrders"
          border
          stripe
          height="100%"
          row-key="requestId"
          data-testid="priced-order-table"
          @selection-change="handlePricedSelection"
        >
          <el-table-column
            type="selection"
            width="46"
            :selectable="(row: NurseStationPricedOrderVO) => row.cancellable"
          />
          <el-table-column
            prop="productName"
            label="收费项目"
            min-width="180"
            show-overflow-tooltip
          />
          <el-table-column prop="pricingModeText" label="划价方式" width="92" />
          <el-table-column label="数量" width="82" align="right">
            <template #default="{ row }">{{ row.quantity }} {{ row.unitName }}</template>
          </el-table-column>
          <el-table-column label="单价" width="96" align="right">
            <template #default="{ row }">{{ formatCurrency(row.unitPrice) }}</template>
          </el-table-column>
          <el-table-column label="金额" width="100" align="right">
            <template #default="{ row }">{{ formatCurrency(row.totalPrice) }}</template>
          </el-table-column>
          <el-table-column prop="pricedTime" label="划价时间" min-width="152" />
          <el-table-column prop="chargeStatusText" label="收费状态" width="88" />
          <el-table-column prop="dispenseStatusText" label="发放状态" width="88" />
          <el-table-column label="取消说明" min-width="150" show-overflow-tooltip>
            <template #default="{ row }">{{ row.cancelReasonDisabled || '可取消' }}</template>
          </el-table-column>
        </el-table>
        <footer class="table-summary">
          <span>共 {{ pricedOrders.length }} 条</span>
          <span
            >有效金额：<strong>{{ formatCurrency(pricedAmount) }}</strong></span
          >
        </footer>
      </section>

      <section v-else-if="activeTab === 'order'" class="billing-panel order-billing-panel">
        <div class="panel-toolbar">
          <div class="toolbar-left">
            <el-radio-group v-model="orderTherapyType">
              <el-radio-button value="ALL">全部</el-radio-button>
              <el-radio-button value="LONG">长期</el-radio-button>
              <el-radio-button value="TEMP">临时</el-radio-button>
            </el-radio-group>
            <el-input
              v-model="orderKeyword"
              clearable
              placeholder="医嘱内容"
              @keyup.enter="loadSupplementOrders"
            />
            <el-button type="primary" @click="loadSupplementOrders">查询</el-button>
          </div>
        </div>

        <el-table
          :data="supplementOrders"
          border
          stripe
          height="100%"
          row-key="requestId"
          data-testid="supplement-order-table"
        >
          <el-table-column type="expand">
            <template #default="{ row }">
              <div class="charge-detail-box">
                <h4>已补费用明细</h4>
                <el-empty
                  v-if="!row.chargeItems.length"
                  description="暂无补费项目"
                  :image-size="52"
                />
                <el-table v-else :data="row.chargeItems" border size="small">
                  <el-table-column prop="productName" label="项目名称" min-width="160" />
                  <el-table-column label="数量" width="90">
                    <template #default="scope"
                      >{{ scope.row.quantity }} {{ scope.row.unitName }}</template
                    >
                  </el-table-column>
                  <el-table-column label="金额" width="100">
                    <template #default="scope">{{ formatCurrency(scope.row.totalPrice) }}</template>
                  </el-table-column>
                  <el-table-column prop="pricedTime" label="划价时间" min-width="150" />
                  <el-table-column prop="chargeStatusText" label="状态" width="90" />
                </el-table>
              </div>
            </template>
          </el-table-column>
          <el-table-column prop="therapyTypeText" label="长临" width="66" />
          <el-table-column
            prop="adviceDisplayName"
            label="医嘱内容"
            min-width="220"
            show-overflow-tooltip
          />
          <el-table-column prop="specificationText" label="规格" width="100" />
          <el-table-column prop="dosageDisplay" label="剂量" width="80" />
          <el-table-column prop="usageDisplay" label="用法" width="100" />
          <el-table-column prop="frequencyDisplay" label="频次" width="100" />
          <el-table-column prop="requesterName" label="医生" width="88" />
          <el-table-column prop="statusText" label="状态" width="82" />
          <el-table-column label="已有费用" width="88" align="center">
            <template #default="{ row }">{{ row.chargeItems.length }} 项</template>
          </el-table-column>
          <el-table-column label="操作" width="82" fixed="right">
            <template #default="{ row }">
              <el-button link type="primary" @click="openSupplementDialogFromTable(row)">
                补费
              </el-button>
            </template>
          </el-table-column>
        </el-table>
      </section>

      <section v-else class="billing-panel auto-billing-panel">
        <article class="nursing-readonly-card">
          <div>
            <h3>护理级别自动计费</h3>
            <p>该项目由护理级别医嘱自动生成，护士站仅可查看，不能在此修改。</p>
          </div>
          <dl v-if="autoBillingData.nursingLevelReadonly">
            <div>
              <dt>护理项目</dt>
              <dd>{{ autoBillingData.nursingLevelReadonly.activityName }}</dd>
            </div>
            <div>
              <dt>收费项目</dt>
              <dd>{{ autoBillingData.nursingLevelReadonly.chargeDefinitionName }}</dd>
            </div>
            <div>
              <dt>数量</dt>
              <dd>
                {{ autoBillingData.nursingLevelReadonly.quantity }}
                {{ autoBillingData.nursingLevelReadonly.unitName }}
              </dd>
            </div>
            <div>
              <dt>计费日期</dt>
              <dd>{{ autoBillingData.nursingLevelReadonly.billDate }}</dd>
            </div>
          </dl>
        </article>

        <div class="panel-toolbar auto-toolbar">
          <div><strong>患者自定义自动计费规则</strong></div>
          <el-button type="primary" @click="openAutoRuleDialog()">新增规则</el-button>
        </div>
        <el-table
          :data="autoBillingData.rules"
          border
          stripe
          height="100%"
          row-key="ruleId"
          data-testid="auto-billing-table"
        >
          <el-table-column prop="ruleName" label="规则名称" min-width="150" />
          <el-table-column prop="activityName" label="触发项目" min-width="140" />
          <el-table-column prop="chargeDefinitionName" label="收费项目" min-width="140" />
          <el-table-column prop="quantity" label="数量" width="72" align="right" />
          <el-table-column prop="performingOrgName" label="执行科室" width="110" />
          <el-table-column prop="priority" label="优先级" width="72" />
          <el-table-column prop="statusText" label="状态" width="72">
            <template #default="{ row }">
              <el-tag :type="row.status === 'ACTIVE' ? 'success' : 'info'">{{
                row.statusText
              }}</el-tag>
            </template>
          </el-table-column>
          <el-table-column prop="updatedAt" label="更新时间" min-width="150" />
          <el-table-column label="操作" width="180" fixed="right">
            <template #default="{ row }">
              <el-button link type="primary" @click="openAutoRuleDialogFromTable(row)">
                编辑
              </el-button>
              <el-button link type="warning" @click="changeAutoRuleStatusFromTable(row)">
                {{ row.status === 'ACTIVE' ? '停用' : '启用' }}
              </el-button>
              <el-button link type="danger" @click="deleteAutoRuleFromTable(row)">删除</el-button>
            </template>
          </el-table-column>
        </el-table>
      </section>
    </div>

    <el-dialog
      v-model="pricingDialogVisible"
      :title="pricingMode === 'DIRECT' ? '批量划价' : '医嘱补费'"
      width="min(1380px, 94vw)"
      destroy-on-close
      class="billing-dialog"
    >
      <div class="pricing-context">
        <span>患者：{{ selectedPatient?.bedName }} {{ selectedPatient?.patientName }}</span>
        <span>模式：{{ pricingMode === 'DIRECT' ? '直接划价' : '医嘱补费' }}</span>
        <span v-if="supplementParent">关联医嘱：{{ supplementParent.adviceDisplayName }}</span>
      </div>
      <div class="pricing-dialog-body">
        <section class="catalog-panel">
          <header>
            <el-radio-group v-model="catalogCategory" @change="handleCatalogCategoryChange">
              <el-radio-button value="DEVICE">耗材</el-radio-button>
              <el-radio-button value="SERVICE">诊疗项目</el-radio-button>
            </el-radio-group>
            <el-input
              v-model="catalogKeyword"
              clearable
              placeholder="编码/名称"
              @keyup.enter="loadCatalog"
            />
            <el-button type="primary" @click="loadCatalog">查询</el-button>
          </header>
          <el-table
            :data="catalogRows"
            border
            height="390"
            row-key="productDefinitionId"
            @row-click="addCatalogItem"
          >
            <el-table-column prop="busNo" label="编码" width="100" />
            <el-table-column prop="name" label="项目名称" min-width="170" show-overflow-tooltip />
            <el-table-column prop="specificationText" label="规格" width="110" />
            <el-table-column prop="unitName" label="单位" width="64" />
            <el-table-column label="单价" width="90" align="right">
              <template #default="{ row }">{{ formatCurrency(row.unitPrice) }}</template>
            </el-table-column>
          </el-table>
          <el-pagination
            v-model:current-page="catalogPageIndex"
            :page-size="10"
            :total="catalogTotal"
            layout="total, prev, pager, next"
            @current-change="loadCatalog"
          />
        </section>

        <section class="selected-panel">
          <header>
            <strong>已选项目</strong>
            <el-button link type="danger" @click="selectedCatalogItems = []">清空</el-button>
          </header>
          <el-table :data="selectedCatalogItems" border height="430" row-key="clientItemId">
            <el-table-column label="项目名称" min-width="160" show-overflow-tooltip>
              <template #default="{ row }">{{ row.catalog.name }}</template>
            </el-table-column>
            <el-table-column label="数量" width="112">
              <template #default="{ row }">
                <el-input-number
                  v-model="row.quantity"
                  :min="1"
                  :precision="0"
                  controls-position="right"
                />
              </template>
            </el-table-column>
            <el-table-column label="单位" width="78">
              <template #default="{ row }">{{ row.catalog.unitName }}</template>
            </el-table-column>
            <el-table-column label="金额" width="94" align="right">
              <template #default="{ row }">{{
                formatCurrency(row.catalog.unitPrice * row.quantity)
              }}</template>
            </el-table-column>
            <el-table-column label="操作" width="66">
              <template #default="{ row }">
                <el-button link type="danger" @click="removeCatalogItem(row.clientItemId)"
                  >删除</el-button
                >
              </template>
            </el-table-column>
          </el-table>
        </section>
      </div>
      <template #footer>
        <div class="dialog-summary">
          <span>已选 {{ selectedCatalogItems.length }} 项</span>
          <strong>合计：{{ formatCurrency(selectedAmount) }}</strong>
          <el-button @click="pricingDialogVisible = false">取消</el-button>
          <el-button type="primary" :loading="saving" @click="submitPricing">
            {{ pricingMode === 'DIRECT' ? '确认划价' : '确认补费' }}
          </el-button>
        </div>
      </template>
    </el-dialog>

    <el-dialog v-model="cancelDialogVisible" title="取消划价" width="480px">
      <el-alert title="仅能取消尚未收费、发放或结算的项目" type="warning" :closable="false" />
      <el-form label-width="84px" class="cancel-form">
        <el-form-item label="取消数量">{{ cancellableSelection.length }} 条</el-form-item>
        <el-form-item label="取消原因" required>
          <el-input
            v-model="cancelReason"
            type="textarea"
            :rows="3"
            maxlength="100"
            show-word-limit
          />
        </el-form-item>
      </el-form>
      <template #footer>
        <el-button @click="cancelDialogVisible = false">取消</el-button>
        <el-button type="danger" :loading="saving" @click="submitCancel">确认取消划价</el-button>
      </template>
    </el-dialog>

    <el-dialog
      v-model="autoRuleDialogVisible"
      :title="autoRuleForm.ruleId ? '编辑自动计费规则' : '新增自动计费规则'"
      width="640px"
    >
      <el-form label-width="100px" class="auto-rule-form">
        <el-form-item label="规则名称" required
          ><el-input v-model="autoRuleForm.ruleName"
        /></el-form-item>
        <el-form-item label="触发项目" required>
          <el-input v-model="autoRuleForm.activityName" placeholder="例如：氧气吸入" />
        </el-form-item>
        <el-form-item label="收费项目" required>
          <el-input v-model="autoRuleForm.chargeDefinitionName" placeholder="例如：氧气吸入费" />
        </el-form-item>
        <el-form-item label="每次数量">
          <el-input-number v-model="autoRuleForm.quantity" :min="0.01" :precision="2" />
        </el-form-item>
        <el-form-item label="优先级">
          <el-input-number v-model="autoRuleForm.priority" :min="1" :max="99" :precision="0" />
        </el-form-item>
        <el-form-item label="备注">
          <el-input v-model="autoRuleForm.remark" type="textarea" :rows="3" />
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
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #f2f4f7;
  color: #303133;
  -webkit-text-fill-color: currentColor;
}

.billing-workspace {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
}

.billing-patient-header {
  position: relative;
  padding: 12px 16px 10px;
  border-bottom: 1px solid #dcdfe6;
  background: #fff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.patient-identity {
  display: flex;
  align-items: center;
  gap: 10px;
}

.patient-identity strong {
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 18px;
}
.patient-identity span {
  color: #606266;
  -webkit-text-fill-color: #606266;
}

.patient-summary {
  display: grid;
  grid-template-columns: repeat(5, minmax(120px, 1fr));
  gap: 14px;
  margin: 10px 0 0;
}

.patient-summary > div {
  display: flex;
  min-width: 0;
  gap: 6px;
}
.patient-summary dt {
  flex: 0 0 auto;
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
}
.patient-summary dd {
  margin: 0;
  overflow: hidden;
  color: #303133;
  -webkit-text-fill-color: #303133;
  text-overflow: ellipsis;
  white-space: nowrap;
}
.balance-summary dd {
  color: #16a34a;
  -webkit-text-fill-color: #16a34a;
  font-weight: 700;
}
.balance-summary.is-warning dd {
  color: #d97706;
  -webkit-text-fill-color: #d97706;
}
.diagnosis-text {
  margin: 8px 0 0;
  color: #606266;
  -webkit-text-fill-color: #606266;
  font-size: 13px;
}

.billing-tabs {
  display: flex;
  min-height: 44px;
  padding: 0 14px;
  border-bottom: 1px solid #dcdfe6;
  background: #fff;
}

.billing-tabs button {
  position: relative;
  min-width: 112px;
  padding: 0 18px;
  cursor: pointer;
  border: 0;
  background: transparent;
  color: #606266;
  -webkit-text-fill-color: #606266;
  font: inherit;
}

.billing-tabs button:hover,
.billing-tabs button.is-active {
  color: #409eff;
  -webkit-text-fill-color: #409eff;
}
.billing-tabs button.is-active {
  font-weight: 600;
}
.billing-tabs button.is-active::after {
  position: absolute;
  right: 16px;
  bottom: 0;
  left: 16px;
  height: 2px;
  background: #409eff;
  content: '';
}

.billing-panel {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
  padding: 12px;
  overflow: hidden;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.panel-toolbar {
  display: flex;
  min-height: 44px;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  margin-bottom: 10px;
  padding: 8px 10px;
  border: 1px solid #e4e7ed;
  border-radius: 4px;
  background: #fff;
}

.toolbar-left {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 8px;
}
.toolbar-left :deep(.el-input) {
  width: 220px;
}
.table-summary {
  display: flex;
  justify-content: flex-end;
  gap: 24px;
  padding: 10px 6px 0;
  color: #606266;
  -webkit-text-fill-color: #606266;
}
.table-summary strong {
  color: #f56c6c;
  -webkit-text-fill-color: #f56c6c;
  font-size: 16px;
}

.charge-detail-box {
  padding: 10px 50px;
  background: #f8fafc;
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.charge-detail-box h4 {
  margin: 0 0 10px;
}

.nursing-readonly-card {
  display: grid;
  grid-template-columns: minmax(260px, 0.8fr) 2fr;
  gap: 20px;
  margin-bottom: 10px;
  padding: 14px 16px;
  border: 1px solid #b3d8ff;
  border-radius: 4px;
  background: #ecf5ff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.nursing-readonly-card h3 {
  margin: 0 0 6px;
  font-size: 16px;
}
.nursing-readonly-card p {
  margin: 0;
  color: #606266;
  -webkit-text-fill-color: #606266;
  font-size: 13px;
}
.nursing-readonly-card dl {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 10px;
  margin: 0;
}
.nursing-readonly-card dt {
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
  font-size: 12px;
}
.nursing-readonly-card dd {
  margin: 4px 0 0;
  color: #111827;
  -webkit-text-fill-color: #111827;
}
.auto-toolbar {
  margin-bottom: 10px;
}

.pricing-context {
  display: flex;
  flex-wrap: wrap;
  gap: 20px;
  margin-bottom: 12px;
  padding: 9px 12px;
  border-radius: 4px;
  background: #ecf5ff;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.pricing-dialog-body {
  display: grid;
  grid-template-columns: minmax(0, 1.08fr) minmax(0, 0.92fr);
  gap: 12px;
}
.catalog-panel,
.selected-panel {
  min-width: 0;
  padding: 10px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
}
.catalog-panel > header,
.selected-panel > header {
  display: flex;
  min-height: 36px;
  align-items: center;
  gap: 8px;
  margin-bottom: 8px;
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.catalog-panel > header :deep(.el-input) {
  flex: 1;
}
.selected-panel > header {
  justify-content: space-between;
}
.catalog-panel :deep(.el-pagination) {
  justify-content: flex-end;
  margin-top: 8px;
}
.selected-panel :deep(.el-input-number) {
  width: 92px;
}
.dialog-summary {
  display: flex;
  align-items: center;
  justify-content: flex-end;
  gap: 16px;
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.dialog-summary strong {
  color: #f56c6c;
  -webkit-text-fill-color: #f56c6c;
  font-size: 17px;
}
.cancel-form {
  margin-top: 18px;
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.auto-rule-form {
  padding-right: 18px;
  color: #303133;
  -webkit-text-fill-color: #303133;
}
.inpatient-billing-module > :deep(.el-empty) {
  min-height: 0;
  flex: 1;
}

.billing-panel :deep(.el-table),
.billing-panel :deep(.el-table th.el-table__cell),
.billing-panel :deep(.el-table td.el-table__cell) {
  color: #303133;
  -webkit-text-fill-color: #303133;
}

@media (max-width: 1366px) {
  .billing-patient-header {
    padding: 9px 12px 8px;
  }
  .patient-summary {
    grid-template-columns: repeat(3, minmax(120px, 1fr));
    gap: 6px 12px;
  }
  .billing-panel {
    padding: 9px;
  }
  .nursing-readonly-card {
    grid-template-columns: 1fr;
    gap: 10px;
  }
}
</style>
