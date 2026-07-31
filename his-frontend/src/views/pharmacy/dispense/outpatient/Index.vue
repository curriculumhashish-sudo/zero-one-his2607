<template>
  <div class="outpatient-dispense-page">
    <!-- 左侧：患者队列 -->
    <div class="page__left">
      <div class="left-panel">
        <div class="left-panel__header">
          <el-icon class="left-panel__header-icon"><User /></el-icon>
          <span class="left-panel__header-title">发药患者</span>
        </div>

        <div class="left-panel__tabs">
          <el-button
            class="left-panel__tab-btn"
            :class="{ 'left-panel__tab-btn--active': queueScope === 'pending' }"
            @click="queueScope = 'pending'"
          >
            待发药
          </el-button>
          <el-button
            class="left-panel__tab-btn"
            :class="{ 'left-panel__tab-btn--active': queueScope === 'completed' }"
            @click="queueScope = 'completed'"
          >
            已发药
          </el-button>
        </div>

        <div class="left-panel__search">
          <el-input
            v-model="searchKeyword"
            placeholder="姓名/就诊号"
            clearable
            prefix-icon="Search"
            @keyup.enter="loadPatients"
          />
        </div>

        <div class="left-panel__patient-list">
          <div v-if="patientSummaries.length === 0 && !queueLoading" class="left-panel__empty">
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

    <!-- 右侧：发药明细 -->
    <div class="page__right">
      <div class="right-panel">
        <div class="right-panel__header">
          <el-icon class="right-panel__header-icon"><Document /></el-icon>
          <span class="right-panel__header-title">发药明细（药品）</span>
        </div>

        <!-- 工具栏 -->
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
            <el-radio-button value="chinese">中药</el-radio-button>
          </el-radio-group>

          <div class="right-panel__toolbar-spacer" />

          <el-button @click="handleRefresh" :icon="Refresh">刷新</el-button>
          <el-button @click="traceDialogVisible = true" :disabled="!selectedPatientId">
            <el-icon><Camera /></el-icon>
            录入追溯码
          </el-button>
          <el-button type="primary" :loading="dispensing" :disabled="!canDispense" @click="handleConfirmDispense">
            <el-icon><Check /></el-icon>
            确认发药
          </el-button>
        </div>

        <!-- 明细区域 -->
        <div class="right-panel__content">
          <div v-if="!selectedPatientId" class="right-panel__empty">
            <div class="right-panel__empty-icon">
              <el-icon :size="64"><Document /></el-icon>
            </div>
            <span class="right-panel__empty-text">请从左侧选择发药患者</span>
          </div>

          <DispenseCenterPanel
            v-else
            ref="centerPanelRef"
            :prescription-blocks="prescriptionBlocks"
            :loading="detailLoading"
            :queue-scope="queueScope"
            :prescription-type="prescriptionType"
            @selection-change="handleSelectionChange"
          />
        </div>
      </div>
    </div>

    <!-- 追溯码弹窗 -->
    <TraceCodeDialog
      v-model:visible="traceDialogVisible"
      :lines="traceDialogLines"
      :patient-name="selectedPatientName"
      @confirm="handleTraceConfirm"
    />
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { User, Document, Camera, Check, Refresh } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

import DispenseCenterPanel from './components/DispenseCenterPanel.vue'
import TraceCodeDialog from './components/TraceCodeDialog.vue'
import type { DispensePrescriptionBlock } from './type'
import { toPrescriptionBlocks } from './adapter'

import {
  loadDispensePageInit,
  pageDispensePatientQueue,
  listPatientPrescriptions,
  saveDispensePreparation,
  confirmMedicationDispense,
  adaptPatientSummary
} from '@/apis/pharmacy/dispense/outpatient'

const queueScope = ref<'pending' | 'completed'>('pending')
const searchKeyword = ref('')
const selectedPatientId = ref<string | null>(null)
const patientSummaries = ref<PatientSummary[]>([])
const prescriptionBlocks = ref<DispensePrescriptionBlock[]>([])
const queueLoading = ref(false)
const detailLoading = ref(false)
const dispensing = ref(false)

const selectedDispenserId = ref('')
const dispenserOptions = ref<{ label: string; value: string }[]>([])
const prescriptionType = ref<'western' | 'chinese'>('western')
const traceDialogVisible = ref(false)
const traceCodes = ref<Record<string, string>>({})

const centerPanelRef = ref<InstanceType<typeof DispenseCenterPanel> | null>(null)
const selectedIds = ref<string[]>([])

const selectedPatientName = computed(() => {
  const p = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
  return p?.name || ''
})

const canDispense = computed(() => {
  return selectedPatientId.value && selectedDispenserId.value && selectedIds.value.length > 0 && queueScope.value === 'pending'
})

function handleSelectionChange(ids: string[]) {
  selectedIds.value = ids
}

const traceDialogLines = computed(() => {
  const lines: any[] = []
  for (const block of prescriptionBlocks.value) {
    for (const item of block.items) {
      if (item.medicationType === prescriptionType.value) {
        lines.push(item)
      }
    }
  }
  return lines
})

async function loadInit() {
  try {
    const init = await loadDispensePageInit()
    dispenserOptions.value = init.dispensers.map(d => ({
      label: d.displayName,
      value: String(d.practitionerId)
    }))
  } catch {
    ElMessage.error('加载初始化数据失败')
  }
}

async function loadPatients() {
  queueLoading.value = true
  try {
    const result = await pageDispensePatientQueue({
      keyword: searchKeyword.value,
      dispenseStatusEnumIn: queueScope.value === 'pending'
        ? ['PREPARATION', 'IN_PROGRESS']
        : ['COMPLETED'],
      pageNo: 1,
      pageSize: 50
    })
    patientSummaries.value = result.rows.map(adaptPatientSummary)
  } catch {
    ElMessage.error('加载患者队列失败')
    patientSummaries.value = []
  } finally {
    queueLoading.value = false
  }
}

async function handleSelectPatient(patient: PatientSummary) {
  selectedPatientId.value = String(patient.id)
  centerPanelRef.value?.clearSelection()
  traceCodes.value = {}
  detailLoading.value = true
  try {
    const rows = await listPatientPrescriptions(selectedPatientId.value)
    prescriptionBlocks.value = toPrescriptionBlocks(rows)
  } catch {
    ElMessage.error('加载处方明细失败')
    prescriptionBlocks.value = []
  } finally {
    detailLoading.value = false
  }
}

function handleRefresh() {
  if (selectedPatientId.value) {
    const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
    if (patient) {
      handleSelectPatient(patient)
    }
  }
  loadPatients()
}

async function handleConfirmDispense() {
  if (!selectedPatientId.value) {
    ElMessage.warning('请先选择患者')
    return
  }
  if (!selectedDispenserId.value) {
    ElMessage.warning('请选择发药人')
    return
  }
  if (selectedIds.value.length === 0) {
    ElMessage.warning('请勾选要发的药品')
    return
  }

  dispensing.value = true
  try {
    const lines = selectedIds.value.map(id => {
      const block = prescriptionBlocks.value.find(b => b.items.some(i => i.id === id))
      const item = block?.items.find(i => i.id === id)
      return {
        dispenseId: id,
        dispenseLocationId: item?.pharmacy || '',
        lotNumber: item?.batchNo || null,
        quantity: String(item?.quantity || 0),
        traceNo: traceCodes.value[id] || null
      }
    })

    await saveDispensePreparation({
      encounterId: selectedPatientId.value,
      preparerPractitionerId: selectedDispenserId.value,
      lines
    })

    const result = await confirmMedicationDispense({
      encounterId: selectedPatientId.value,
      lines: selectedIds.value.map(id => ({ dispenseId: id }))
    })

    ElMessage.success(result.message || '发药成功')
    await loadPatients()
    const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
    if (patient) {
      await handleSelectPatient(patient)
    }
  } catch {
    ElMessage.error('发药失败，请重试')
  } finally {
    dispensing.value = false
  }
}

function handleTraceConfirm(codes: Record<string, string>) {
  traceCodes.value = { ...traceCodes.value, ...codes }
  ElMessage.success('追溯码已保存')
}

watch(queueScope, () => {
  selectedPatientId.value = null
  prescriptionBlocks.value = []
  loadPatients()
})

onMounted(async () => {
  await loadInit()
  loadPatients()
})
</script>

<style scoped>
.outpatient-dispense-page {
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

.left-panel__patient-list {
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
