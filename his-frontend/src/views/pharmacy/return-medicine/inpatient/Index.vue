<template>
  <div class="inpatient-return-medicine-page">
    <div class="page__left">
      <div class="left-panel">
        <div class="left-panel__contract">
          <el-select
            v-model="insuranceContract"
            placeholder="请选择"
            style="width: 100%"
            size="default"
          >
            <el-option
              v-for="option in insuranceContractOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
        <div class="left-panel__divider" />
        <div class="left-panel__header">
          <el-icon class="left-panel__header-icon"><User /></el-icon>
          <span class="left-panel__header-title">退药患者</span>
        </div>
        <div class="left-panel__tabs">
          <el-button
            class="left-panel__tab-btn"
            :class="{ 'left-panel__tab-btn--active': queueScope === 'pending' }"
            @click="queueScope = 'pending'"
          >
            待退药
          </el-button>
          <el-button
            class="left-panel__tab-btn"
            :class="{ 'left-panel__tab-btn--active': queueScope === 'completed' }"
            @click="queueScope = 'completed'"
          >
            已退药
          </el-button>
        </div>
        <div class="left-panel__divider" />
        <div class="left-panel__ward">
          <el-select
            v-model="selectedWard"
            placeholder="全部病区"
            style="width: 100%"
            size="default"
          >
            <el-option
              v-for="option in wardOptions"
              :key="option.value"
              :label="option.label"
              :value="option.value"
            />
          </el-select>
        </div>
        <div class="left-panel__search">
          <el-input
            v-model="patientKeyword"
            placeholder="姓名/住院号/床号"
            clearable
            @keyup.enter="loadPatients"
            prefix-icon="Search"
          />
        </div>
        <div class="left-panel__patient-list">
          <PatientCard
            v-for="patient in patientSummaries"
            :key="patient.id"
            :patient="patient"
            :selected="selectedPatientId === String(patient.id)"
            @click="handleSelectPatient(patient)"
          />
          <div v-if="patientSummaries.length === 0 && !queueLoading" class="left-panel__empty">
            <span>暂无匹配患者</span>
          </div>
        </div>
      </div>
    </div>
    <div class="page__right">
      <div class="right-panel">
        <div class="right-panel__header">
          <el-icon class="right-panel__header-icon"><Document /></el-icon>
          <span class="right-panel__header-title">退药明细（药品）</span>
        </div>
        <div class="right-panel__toolbar">
          <el-button class="right-panel__toolbar-btn" type="primary" @click="handleConfirmReturn">
            确认退药
          </el-button>
        </div>
        <div class="right-panel__content">
          <div v-if="!selectedPatientId" class="right-panel__empty">
            <div class="right-panel__empty-icon">
              <el-icon :size="64"><Document /></el-icon>
            </div>
            <span class="right-panel__empty-text">请从左侧选择退药患者</span>
          </div>
          <div v-else class="right-panel__table-wrapper">
            <el-table
              ref="returnTableRef"
              :data="returnItems"
              row-key="key"
              :loading="detailLoading"
              @selection-change="handleSelectionChange"
              style="width: 100%;"
            >
              <el-table-column
                type="selection"
                width="44"
                :selectable="canSelectRow"
              />
              <el-table-column
                prop="itemName"
                label="药品名称"
                minWidth="160"
              />
              <el-table-column
                prop="spec"
                label="规格"
                width="120"
              />
              <el-table-column
                prop="quantity"
                label="实退数量"
                width="100"
              />
              <el-table-column
                prop="unit"
                label="单位"
                width="60"
              />
              <el-table-column
                prop="batchNo"
                label="批次号"
                width="100"
              />
              <el-table-column
                prop="totalPrice"
                label="金额"
                width="100"
              />
              <el-table-column
                prop="pharmacy"
                label="发放库房"
                width="150"
              />
              <el-table-column
                prop="factory"
                label="厂家"
                minWidth="160"
              />
              <el-table-column
                prop="status"
                label="状态"
                width="100"
              >
                <template #default="{ row }">
                  <el-tag :type="getReturnStatusType(row.status)">
                    {{ INPATIENT_RETURN_STATUS_MAP[row.status] }}
                  </el-tag>
                </template>
              </el-table-column>
            </el-table>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { User, Document } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'

import {
  loadInpatientReturnMedicineInit,
  pageInpatientReturnPatientQueue,
  getInpatientReturnMedicineDetail,
  confirmInpatientMedicineReturn,
  adaptPatientSummary
} from '@/apis/pharmacy/return-medicine/inpatient'
import { INPATIENT_RETURN_STATUS_MAP } from './constants'
import type { InpatientReturnMedicineItemRow } from './type'

const queueScope = ref<'pending' | 'completed'>('pending')
const selectedPatientId = ref<string | null>(null)
const insuranceContract = ref('')
const selectedWard = ref('')
const insuranceContractOptions = ref<{ label: string; value: string }[]>([])
const wardOptions = ref<{ label: string; value: string }[]>([])
const patientSummaries = ref<PatientSummary[]>([])
const returnItems = ref<InpatientReturnMedicineItemRow[]>([])
const queueLoading = ref(false)
const detailLoading = ref(false)
const patientKeyword = ref('')
const selectedRowKeys = ref<string[]>([])

const returnTableRef = ref<any>(null)

const currentPatientSummary = computed(() => {
  return patientSummaries.value.find(p => p.id === selectedPatientId.value)
})

function getReturnStatusType(status: string) {
  const map: Record<string, 'primary' | 'success' | 'warning' | 'info' | 'danger'> = {
    'refundable': 'success',
    'partially-refundable': 'warning',
    'non-refundable': 'danger',
    'refunded': 'info'
  }
  return map[status] || 'info'
}

function canSelectRow(row: InpatientReturnMedicineItemRow) {
  return row.status === 'refundable' || row.status === 'partially-refundable'
}

async function initPage() {
  const initData = await loadInpatientReturnMedicineInit()
  insuranceContractOptions.value = initData.insuranceContractOptions
  wardOptions.value = initData.wardOptions || []
  await loadPatients()
}

async function loadPatients() {
  queueLoading.value = true
  try {
    const result = await pageInpatientReturnPatientQueue({
      status: queueScope.value,
      keyword: patientKeyword.value
    })
    patientSummaries.value = result.records.map(adaptPatientSummary)
  } finally {
    queueLoading.value = false
  }
}

async function handleSelectPatient(patient: PatientSummary) {
  selectedPatientId.value = String(patient.id)
  detailLoading.value = true
  try {
    const detail = await getInpatientReturnMedicineDetail(String(patient.id))
    if (detail) {
      returnItems.value = detail.items.map(item => ({
        key: item.itemId,
        ...item
      }))
    } else {
      returnItems.value = []
    }
  } finally {
    detailLoading.value = false
    selectedRowKeys.value = []
  }
}

async function handleConfirmReturn() {
  if (selectedRowKeys.value.length === 0) {
    ElMessage.warning('请先选择要退药的药品')
    return
  }
  try {
    const items = selectedRowKeys.value.map(key => {
      const item = returnItems.value.find(i => i.key === key)
      return { itemId: item?.itemId || key, quantity: item?.quantity || 0 }
    })
    const result = await confirmInpatientMedicineReturn({
      encounterId: selectedPatientId.value!,
      items
    })
    if (result.success) {
      ElMessage.success(result.message)
      await loadPatients()
      const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
      if (patient) {
        await handleSelectPatient(patient)
      }
      selectedRowKeys.value = []
      returnTableRef.value?.clearSelection()
    } else {
      ElMessage.error(result.message)
    }
  } catch {
    ElMessage.error('退药失败')
  }
}

function handleSelectionChange(selected: any[]) {
  selectedRowKeys.value = selected.map((row: any) => String(row.key))
}

watch(queueScope, loadPatients)

onMounted(initPage)
</script>

<style scoped>
.inpatient-return-medicine-page {
  display: flex;
  flex: 1;
  min-height: 0;
  gap: 8px;
}

.page__left {
  width: 320px;
  flex-shrink: 0;
}

.page__right {
  flex: 1;
  min-width: 0;
}

.left-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #ffffff;
  border-radius: 8px;
  overflow: hidden;
}

.left-panel__contract {
  padding: 12px;
  border-bottom: 1px solid #e5e7eb;
}

.left-panel__divider {
  height: 1px;
  background: #e5e7eb;
}

.left-panel__header {
  display: flex;
  align-items: center;
  padding: 12px;
  gap: 8px;
  background: #f9fafb;
  border-bottom: 1px solid #e5e7eb;
}

.left-panel__header-icon {
  color: #3b82f6;
}

.left-panel__header-title {
  font-size: 14px;
  font-weight: 600;
  color: #1f2937;
}

.left-panel__tabs {
  display: flex;
  padding: 8px 12px;
  gap: 8px;
}

.left-panel__tab-btn {
  flex: 1;
  padding: 6px;
  font-size: 13px;
  border-radius: 6px;
  background: #ffffff;
  border: 1px solid #e5e7eb;
  color: #6b7280;
  transition: all 0.2s;
}

.left-panel__tab-btn:hover {
  border-color: #3b82f6;
}

.left-panel__tab-btn--active {
  background: #eff6ff;
  border-color: #3b82f6;
  color: #3b82f6;
}

.left-panel__ward {
  padding: 8px 12px;
}

.left-panel__search {
  padding: 0 12px 8px;
}

.left-panel__patient-list {
  flex: 1;
  overflow-y: auto;
  padding: 8px;
}

.left-panel__empty {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 120px;
  color: #9ca3af;
  font-size: 13px;
}

.right-panel {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #ffffff;
  border-radius: 8px;
  overflow: hidden;
}

.right-panel__header {
  display: flex;
  align-items: center;
  padding: 16px 20px;
  gap: 8px;
  border-bottom: 1px solid #e5e7eb;
}

.right-panel__header-icon {
  color: #3b82f6;
}

.right-panel__header-title {
  font-size: 16px;
  font-weight: 600;
  color: #1f2937;
}

.right-panel__toolbar {
  display: flex;
  align-items: center;
  padding: 12px 20px;
  gap: 8px;
  border-bottom: 1px solid #e5e7eb;
}

.right-panel__toolbar-btn {
  padding: 6px 16px;
  font-size: 13px;
  border-radius: 6px;
}

.right-panel__content {
  flex: 1;
  overflow: hidden;
  padding: 12px;
}

.right-panel__empty {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%;
  color: #9ca3af;
}

.right-panel__empty-icon {
  margin-bottom: 16px;
}

.right-panel__empty-text {
  font-size: 14px;
}

.right-panel__table-wrapper {
  height: 100%;
  overflow: auto;
}
</style>