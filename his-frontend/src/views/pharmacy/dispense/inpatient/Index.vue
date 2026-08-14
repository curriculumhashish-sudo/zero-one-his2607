<template>
  <div class="inpatient-dispense-page">
    <!-- 模式切换 -->
    <div class="mode-bar">
      <el-radio-group v-model="dispenseMode" size="default">
        <el-radio-button value="patient">按患者发药</el-radio-button>
        <el-radio-button value="summary">汇总发药</el-radio-button>
      </el-radio-group>
    </div>

    <!-- 按患者发药模式 -->
    <div v-if="dispenseMode === 'patient'" class="patient-mode">
      <div class="page__left">
        <div class="left-panel">
          <div class="left-panel__header">
            <el-icon class="left-panel__header-icon"><User /></el-icon>
            <span class="left-panel__header-title">发药患者</span>
          </div>

          <div class="left-panel__tabs">
            <el-button
              class="left-panel__tab-btn"
              :class="{ 'left-panel__tab-btn--active': patientScope === 'pending' }"
              @click="patientScope = 'pending'"
            >
              待发药
            </el-button>
            <el-button
              class="left-panel__tab-btn"
              :class="{ 'left-panel__tab-btn--active': patientScope === 'completed' }"
              @click="patientScope = 'completed'"
            >
              已发药
            </el-button>
          </div>

          <div class="left-panel__search">
            <el-input
              v-model="patientKeyword"
              placeholder="姓名/住院号/床号"
              clearable
              prefix-icon="Search"
              @keyup.enter="loadPatientQueue"
            />
          </div>

          <div class="left-panel__patient-list">
            <div v-if="patientSummaries.length === 0 && !patientQueueLoading" class="left-panel__empty">
              <span>暂无匹配患者</span>
            </div>
            <PatientCard
              v-for="patient in patientSummaries"
              :key="patient.id"
              :patient="patient"
              :selected="selectedPatientId === String(patient.id)"
              @click="handleSelectPatient(patient)"
            />
          </div>
        </div>
      </div>

      <div class="page__right">
        <div class="right-panel">
          <div class="right-panel__header">
            <el-icon class="right-panel__header-icon"><Document /></el-icon>
            <span class="right-panel__header-title">发药明细（药品）</span>
          </div>

          <div class="right-panel__toolbar">
            <el-select
              v-model="selectedDispenserId"
              placeholder="请选择发药人"
              size="default"
              style="width: 160px"
            >
              <el-option
                v-for="opt in dispenserOptions"
                :key="opt.value"
                :label="opt.label"
                :value="opt.value"
              />
            </el-select>

            <el-radio-group v-model="prescriptionType" size="default">
              <el-radio-button value="western">西药</el-radio-button>
              <el-radio-button value="herbal">中药</el-radio-button>
            </el-radio-group>

            <div class="right-panel__toolbar-spacer" />

            <el-button @click="handleRefreshPatient" :icon="Refresh">刷新</el-button>
            <el-button type="primary" :loading="dispensing" :disabled="!canPatientDispense" @click="handleConfirmPatientDispense">
              <el-icon><Check /></el-icon>
              确认发药
            </el-button>
          </div>

          <div class="right-panel__content">
            <div v-if="!selectedPatientId" class="right-panel__empty">
              <div class="right-panel__empty-icon">
                <el-icon :size="64"><Document /></el-icon>
              </div>
              <span class="right-panel__empty-text">请从左侧选择发药患者</span>
            </div>

            <DispenseCenterPanel
              v-else
              ref="patientCenterRef"
              :prescription-blocks="patientPrescriptionBlocks"
              :loading="patientDetailLoading"
              :queue-scope="patientScope"
              :prescription-type="prescriptionType"
              @selection-change="handlePatientSelectionChange"
            />
          </div>
        </div>
      </div>
    </div>

    <!-- 汇总发药模式 -->
    <div v-else class="summary-mode">
      <div class="page__left">
        <div class="left-panel">
          <div class="left-panel__header">
            <el-icon class="left-panel__header-icon"><List /></el-icon>
            <span class="left-panel__header-title">汇总领药单</span>
          </div>

          <div class="left-panel__tabs">
            <el-button
              class="left-panel__tab-btn"
              :class="{ 'left-panel__tab-btn--active': summaryScope === 'pending' }"
              @click="summaryScope = 'pending'"
            >
              待处理
            </el-button>
            <el-button
              class="left-panel__tab-btn"
              :class="{ 'left-panel__tab-btn--active': summaryScope === 'completed' }"
              @click="summaryScope = 'completed'"
            >
              已发药
            </el-button>
          </div>

          <div class="left-panel__search">
            <el-input
              v-model="summaryKeyword"
              placeholder="汇总单号/领药人"
              clearable
              prefix-icon="Search"
              @keyup.enter="loadSummaryQueue"
            />
          </div>

          <div class="left-panel__summary-list">
            <div v-if="summarySlips.length === 0 && !summaryQueueLoading" class="left-panel__empty">
              <span>暂无汇总单</span>
            </div>
            <div
              v-for="slip in summarySlips"
              :key="slip.summaryId"
              class="summary-card"
              :class="{ 'summary-card--active': selectedSummaryId === slip.summaryId }"
              @click="handleSelectSummary(slip)"
            >
              <div class="summary-card__header">
                <span class="summary-card__no">{{ slip.summaryNo }}</span>
                <el-tag :type="summaryStatusTagType(slip.status)" size="small">
                  {{ SUMMARY_STATUS_MAP[slip.status] }}
                </el-tag>
              </div>
              <div class="summary-card__info">
                <span class="summary-card__label">领药人：</span>
                <span class="summary-card__value">{{ slip.receiverName }}</span>
              </div>
              <div class="summary-card__info">
                <span class="summary-card__label">创建：</span>
                <span class="summary-card__value">{{ slip.createTime }}</span>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div class="page__right">
        <div class="right-panel">
          <div class="right-panel__header">
            <el-icon class="right-panel__header-icon"><Document /></el-icon>
            <span class="right-panel__header-title">汇总明细</span>
          </div>

          <div class="right-panel__toolbar">
            <el-select
              v-model="summaryDispenserId"
              placeholder="请选择发药人"
              size="default"
              style="width: 160px"
            >
              <el-option
                v-for="opt in dispenserOptions"
                :key="opt.value"
                :label="opt.label"
                :value="opt.value"
              />
            </el-select>

            <el-select
              v-model="summaryReceiverId"
              placeholder="请选择领药人"
              size="default"
              style="width: 160px"
            >
              <el-option
                v-for="opt in receiverOptions"
                :key="opt.value"
                :label="opt.label"
                :value="opt.value"
              />
            </el-select>

            <div class="right-panel__toolbar-spacer" />

            <el-button @click="handleRejectSummary" :disabled="!canRejectSummary" type="danger" plain>
              驳回
            </el-button>
            <el-button type="primary" :loading="summaryDispensing" :disabled="!canSummaryDispense" @click="handleConfirmSummaryDispense">
              <el-icon><Check /></el-icon>
              确认发药
            </el-button>
          </div>

          <div class="right-panel__content">
            <div v-if="!selectedSummaryId" class="right-panel__empty">
              <div class="right-panel__empty-icon">
                <el-icon :size="64"><Document /></el-icon>
              </div>
              <span class="right-panel__empty-text">请从左侧选择汇总领药单</span>
            </div>

            <SummaryDispenseDetail
              v-else
              :summary-no="selectedSummary?.summaryNo || ''"
              :receiver-name="selectedSummary?.receiverName || ''"
              :create-time="selectedSummary?.createTime || ''"
              :items="summaryItems"
            />
          </div>
        </div>
      </div>
    </div>

    <!-- 驳回弹窗 -->
    <el-dialog v-model="rejectDialogVisible" title="驳回汇总单" width="420px" :close-on-click-modal="false">
      <el-form>
        <el-form-item label="驳回原因">
          <el-input v-model="rejectReason" type="textarea" :rows="3" placeholder="请输入驳回原因" />
        </el-form-item>
      </el-form>
      <template #footer>
        <el-button @click="rejectDialogVisible = false">取消</el-button>
        <el-button type="danger" :loading="rejecting" @click="handleConfirmReject">确认驳回</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { User, Document, List, Check, Refresh } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

import DispenseCenterPanel from './components/DispenseCenterPanel.vue'
import SummaryDispenseDetail from './components/SummaryDispenseDetail.vue'
import type { DispensePrescriptionBlock } from './type'
import { SUMMARY_STATUS_MAP } from './constants'

import {
  loadInpatientDispenseInit,
  pageInpatientDispenseQueue,
  pageInpatientMedicationDispenseSlips,
  saveInpatientDispensePreparation,
  dispenseInpatientMedications,
  pagePendingMedicationSummarySlips,
  getMedicationSummaryDispenseDetail,
  dispenseSummaryMedications,
  rejectSummaryDispense,
  adaptPatientSummary
} from '@/apis/pharmacy/dispense/inpatient'
import type { MedicationSummarySlipDto, InpatientMedicationDispenseSlipDto } from '@/apis/pharmacy/dispense/inpatient/type'

// ===== 模式 =====
const dispenseMode = ref<'patient' | 'summary'>('patient')

// ===== 患者模式状态 =====
const patientScope = ref<'pending' | 'completed'>('pending')
const patientKeyword = ref('')
const selectedPatientId = ref<string | null>(null)
const patientSummaries = ref<PatientSummary[]>([])
const patientPrescriptionBlocks = ref<DispensePrescriptionBlock[]>([])
const patientQueueLoading = ref(false)
const patientDetailLoading = ref(false)
const dispensing = ref(false)
const prescriptionType = ref<'western' | 'herbal'>('western')

const selectedDispenserId = ref('')
const dispenserOptions = ref<{ label: string; value: string }[]>([])
const receiverOptions = ref<{ label: string; value: string }[]>([])

const patientCenterRef = ref<InstanceType<typeof DispenseCenterPanel> | null>(null)

const patientSelectedIds = ref<string[]>([])

function handlePatientSelectionChange(ids: string[]) {
  patientSelectedIds.value = ids
}

const canPatientDispense = computed(() => {
  return selectedPatientId.value && selectedDispenserId.value && patientSelectedIds.value.length > 0 && patientScope.value === 'pending'
})

// ===== 汇总模式状态 =====
const summaryScope = ref<'pending' | 'completed'>('pending')
const summaryKeyword = ref('')
const selectedSummaryId = ref<string | null>(null)
const summarySlips = ref<MedicationSummarySlipDto[]>([])
const selectedSummary = ref<MedicationSummarySlipDto | null>(null)
const summaryQueueLoading = ref(false)
const summaryDispensing = ref(false)

const summaryDispenserId = ref('')
const summaryReceiverId = ref('')

const rejectDialogVisible = ref(false)
const rejectReason = ref('')
const rejecting = ref(false)

const summaryItems = computed(() => {
  if (!selectedSummary.value) return []
  return selectedSummary.value.items.map(item => ({
    key: item.dispenseId,
    itemName: item.itemName,
    spec: item.spec,
    quantity: item.quantity,
    batchNo: item.batchNo,
    totalPrice: item.totalPrice,
    pharmacy: item.pharmacy,
    factory: item.factory
  }))
})

const canSummaryDispense = computed(() => {
  return selectedSummaryId.value && summaryDispenserId.value && summaryReceiverId.value && summaryScope.value === 'pending'
})

const canRejectSummary = computed(() => {
  return selectedSummaryId.value && summaryScope.value === 'pending'
})

// ===== 初始化 =====
async function loadInit() {
  try {
    const init = await loadInpatientDispenseInit()
    dispenserOptions.value = init.dispenserOptions
    receiverOptions.value = init.receiverOptions
  } catch {
    ElMessage.error('加载初始化数据失败')
  }
}

// ===== 患者模式 =====
async function loadPatientQueue() {
  patientQueueLoading.value = true
  try {
    const result = await pageInpatientDispenseQueue({
      status: patientScope.value,
      keyword: patientKeyword.value,
      pageNum: 1,
      pageSize: 50
    })
    patientSummaries.value = result.records.map(adaptPatientSummary)
  } catch {
    ElMessage.error('加载患者队列失败')
    patientSummaries.value = []
  } finally {
    patientQueueLoading.value = false
  }
}

async function handleSelectPatient(patient: PatientSummary) {
  selectedPatientId.value = String(patient.id)
  patientCenterRef.value?.clearSelection()
  patientSelectedIds.value = []
  patientDetailLoading.value = true
  try {
    const slips = await pageInpatientMedicationDispenseSlips(selectedPatientId.value)
    patientPrescriptionBlocks.value = toBlocks(slips)
  } catch {
    ElMessage.error('加载处方明细失败')
    patientPrescriptionBlocks.value = []
  } finally {
    patientDetailLoading.value = false
  }
}

function toBlocks(slips: InpatientMedicationDispenseSlipDto[]): DispensePrescriptionBlock[] {
  return slips.map(slip => ({
    slipId: slip.slipId,
    rxNo: slip.rxNo,
    rxType: slip.rxType,
    status: slip.status,
    items: slip.items.map(item => ({
      key: item.dispenseId,
      rxNo: slip.rxNo,
      status: item.status,
      itemName: item.itemName,
      quantity: item.quantity,
      spec: item.spec,
      batchNo: item.batchNo,
      totalPrice: item.totalPrice,
      pharmacy: item.pharmacy,
      traceCode: item.traceCode,
      factory: item.factory,
      sourceStatus: item.sourceStatus,
      dispenseId: item.dispenseId
    }))
  }))
}

function handleRefreshPatient() {
  if (selectedPatientId.value) {
    const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
    if (patient) {
      handleSelectPatient(patient)
    }
  }
  loadPatientQueue()
}

async function handleConfirmPatientDispense() {
  if (!selectedPatientId.value) {
    ElMessage.warning('请先选择患者')
    return
  }
  if (!selectedDispenserId.value) {
    ElMessage.warning('请选择发药人')
    return
  }
  if (patientSelectedIds.value.length === 0) {
    ElMessage.warning('请勾选要发的药品')
    return
  }

  dispensing.value = true
  try {
    const items = patientSelectedIds.value.map(id => {
      let foundItem: { quantity: number; batchNo: string } | undefined
      for (const block of patientPrescriptionBlocks.value) {
        const found = block.items.find((item: any) => item.key === id || item.dispenseId === id)
        if (found) {
          foundItem = found
          break
        }
      }
      return {
        dispenseId: id,
        dispenseQuantity: foundItem?.quantity ?? 1,
        lotNumber: foundItem?.batchNo
      }
    })

    await saveInpatientDispensePreparation({
      summaryNo: selectedPatientId.value!,
      items
    })

    const result = await dispenseInpatientMedications({
      summaryNo: selectedPatientId.value!,
      items,
      practitionerId: selectedDispenserId.value || undefined
    })

    if (result.success) {
      ElMessage.success(result.message || '发药成功')
      await loadPatientQueue()
      const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
      if (patient) {
        await handleSelectPatient(patient)
      }
    } else {
      ElMessage.error(result.message || '发药失败')
    }
  } catch {
    ElMessage.error('发药失败，请重试')
  } finally {
    dispensing.value = false
  }
}

// ===== 汇总模式 =====
async function loadSummaryQueue() {
  summaryQueueLoading.value = true
  try {
    const result = await pagePendingMedicationSummarySlips({
      status: summaryScope.value,
      keyword: summaryKeyword.value,
      pageNum: 1,
      pageSize: 50
    })
    summarySlips.value = result.records
  } catch {
    ElMessage.error('加载汇总单失败')
    summarySlips.value = []
  } finally {
    summaryQueueLoading.value = false
  }
}

async function handleSelectSummary(slip: MedicationSummarySlipDto) {
  selectedSummaryId.value = slip.summaryId
  try {
    const detail = await getMedicationSummaryDispenseDetail(slip.summaryId)
    if (detail) {
      selectedSummary.value = detail
      summaryReceiverId.value = detail.receiverId
    }
  } catch {
    ElMessage.error('加载汇总明细失败')
    selectedSummary.value = null
  }
}

async function handleConfirmSummaryDispense() {
  if (!selectedSummaryId.value) {
    ElMessage.warning('请先选择汇总单')
    return
  }
  if (!summaryDispenserId.value) {
    ElMessage.warning('请选择发药人')
    return
  }
  if (!summaryReceiverId.value) {
    ElMessage.warning('请选择领药人')
    return
  }

  summaryDispensing.value = true
  try {
    const items = (selectedSummary.value?.items || [])
      .filter(item => item.status !== 'completed')
      .map(item => ({
        dispenseId: item.dispenseId,
        dispenseQuantity: item.quantity,
        lotNumber: item.batchNo
      }))

    const result = await dispenseSummaryMedications({
      summaryNo: selectedSummary.value?.summaryNo || '',
      items,
      practitionerId: summaryDispenserId.value || undefined,
      recPractitionerId: summaryReceiverId.value || undefined
    })
    if (result.success) {
      ElMessage.success(result.message || '发药成功')
      await loadSummaryQueue()
      selectedSummaryId.value = null
      selectedSummary.value = null
    } else {
      ElMessage.error(result.message || '发药失败')
    }
  } catch {
    ElMessage.error('发药失败，请重试')
  } finally {
    summaryDispensing.value = false
  }
}

function handleRejectSummary() {
  if (!selectedSummaryId.value) return
  rejectReason.value = ''
  rejectDialogVisible.value = true
}

async function handleConfirmReject() {
  if (!rejectReason.value.trim()) {
    ElMessage.warning('请输入驳回原因')
    return
  }
  if (!selectedSummaryId.value) return

  rejecting.value = true
  try {
    const result = await rejectSummaryDispense({
      summaryNo: selectedSummary.value?.summaryNo || '',
      rejectReason: rejectReason.value
    })
    if (result.success) {
      ElMessage.success('已驳回')
      rejectDialogVisible.value = false
      await loadSummaryQueue()
      selectedSummaryId.value = null
      selectedSummary.value = null
    } else {
      ElMessage.error(result.message || '驳回失败')
    }
  } catch {
    ElMessage.error('驳回失败')
  } finally {
    rejecting.value = false
  }
}

// ===== 辅助 =====
function summaryStatusTagType(status: string): 'primary' | 'success' | 'warning' | 'info' | 'danger' {
  const map: Record<string, 'primary' | 'success' | 'warning' | 'info' | 'danger'> = {
    pending: 'warning',
    completed: 'success',
    rejected: 'danger'
  }
  return map[status] || 'info'
}

// ===== 监听 =====
watch(patientScope, () => {
  selectedPatientId.value = null
  patientPrescriptionBlocks.value = []
  loadPatientQueue()
})

watch(summaryScope, () => {
  selectedSummaryId.value = null
  selectedSummary.value = null
  loadSummaryQueue()
})

watch(dispenseMode, (mode) => {
  if (mode === 'patient' && patientSummaries.value.length === 0) {
    loadPatientQueue()
  }
  if (mode === 'summary' && summarySlips.value.length === 0) {
    loadSummaryQueue()
  }
})

onMounted(async () => {
  await loadInit()
  loadPatientQueue()
})
</script>

<style scoped>
.inpatient-dispense-page {
  display: flex;
  flex: 1;
  flex-direction: column;
  min-height: 0;
}

.mode-bar {
  display: flex;
  align-items: center;
  padding: 8px 16px;
  background: #ffffff;
  border-bottom: 1px solid #e8e8e8;
}

.patient-mode,
.summary-mode {
  display: flex;
  flex: 1;
  min-height: 0;
}

.page__left {
  width: 320px;
  border-right: 1px solid #e8e8e8;
  background: #ffffff;
}

.page__right {
  flex: 1;
  min-height: 0;
  background: #f5f5f5;
}

.left-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
}

.left-panel__header {
  display: flex;
  align-items: center;
  padding: 16px;
  border-bottom: 1px solid #e8e8e8;
}

.left-panel__header-icon {
  font-size: 18px;
  color: #1890ff;
  margin-right: 8px;
}

.left-panel__header-title {
  font-size: 16px;
  font-weight: 600;
  color: #333;
}

.left-panel__tabs {
  display: flex;
  padding: 12px 16px;
  gap: 8px;
  border-bottom: 1px solid #f0f0f0;
}

.left-panel__tab-btn {
  flex: 1;
  padding: 8px 0;
  font-size: 14px;
  background: transparent;
  border: none;
  color: #666;
  cursor: pointer;
  border-radius: 4px;
  transition: all 0.2s;
}

.left-panel__tab-btn:hover {
  background: #f0f0f0;
}

.left-panel__tab-btn--active {
  background: #e6f7ff;
  color: #1890ff;
  font-weight: 500;
}

.left-panel__search {
  padding: 12px 16px;
}

.left-panel__patient-list,
.left-panel__summary-list {
  flex: 1;
  overflow-y: auto;
  padding: 0 16px 16px;
}

.left-panel__empty {
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 40px 0;
  color: #999;
  font-size: 14px;
}

.summary-card {
  padding: 12px;
  border: 1px solid #e8e8e8;
  border-radius: 6px;
  margin-bottom: 8px;
  cursor: pointer;
  transition: all 0.2s;
}

.summary-card:hover {
  border-color: #1890ff;
}

.summary-card--active {
  border-color: #1890ff;
  background: #e6f7ff;
}

.summary-card__header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 8px;
}

.summary-card__no {
  font-size: 14px;
  font-weight: 600;
  color: #333;
}

.summary-card__info {
  font-size: 13px;
  color: #666;
  margin-top: 4px;
}

.summary-card__label {
  color: #999;
}

.summary-card__value {
  color: #333;
}

.right-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
}

.right-panel__header {
  display: flex;
  align-items: center;
  padding: 16px;
  background: #ffffff;
  border-bottom: 1px solid #e8e8e8;
}

.right-panel__header-icon {
  font-size: 18px;
  color: #1890ff;
  margin-right: 8px;
}

.right-panel__header-title {
  font-size: 16px;
  font-weight: 600;
  color: #333;
}

.right-panel__toolbar {
  display: flex;
  align-items: center;
  padding: 12px 16px;
  background: #ffffff;
  border-bottom: 1px solid #e8e8e8;
  gap: 12px;
}

.right-panel__toolbar-spacer {
  flex: 1;
}

.right-panel__content {
  flex: 1;
  min-height: 0;
  padding: 16px;
}

.right-panel__empty {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%;
  color: #999;
}

.right-panel__empty-icon {
  margin-bottom: 16px;
}

.right-panel__empty-text {
  font-size: 14px;
}
</style>
