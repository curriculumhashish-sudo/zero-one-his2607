<template>
  <div class="outpatient-return-medicine-page">
    <div class="page__left">
      <div class="left-panel">
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

        <div class="left-panel__search">
          <el-input
            v-model="searchKeyword"
            placeholder="姓名/排队号"
            clearable
            @keyup.enter="handleSearch"
            prefix-icon="Search"
          />
        </div>

        <div class="left-panel__date">
          <el-date-picker
            v-model="selectedDate"
            type="date"
            placeholder="选择日期"
            value-format="YYYY-MM-DD"
          />
        </div>

        <div class="left-panel__patient-list">
          <div v-if="patientSummaries.length === 0" class="left-panel__empty">
            <span class="left-panel__empty-text">暂无匹配患者</span>
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
          <span class="right-panel__header-title">退药明细（药品）</span>
        </div>

        <div class="right-panel__toolbar">
          <el-button class="right-panel__toolbar-btn" type="primary" @click="handleBatchReturn">
            确认退药
          </el-button>
        </div>

        <div class="right-panel__content">
          <div v-if="!selectedPatientId" class="right-panel__empty">
            <div class="right-panel__empty-icon">
              <svg width="120" height="120" viewBox="0 0 120 120" fill="none" xmlns="http://www.w3.org/2000/svg">
                <rect x="20" y="40" width="80" height="60" rx="4" fill="#f5f5f5"/>
                <rect x="35" y="55" width="50" height="8" rx="2" fill="#e0e0e0"/>
                <rect x="35" y="70" width="40" height="6" rx="1" fill="#e0e0e0"/>
                <rect x="35" y="82" width="45" height="6" rx="1" fill="#e0e0e0"/>
                <rect x="45" y="20" width="30" height="25" rx="2" fill="#f5f5f5"/>
                <rect x="50" y="28" width="20" height="4" rx="1" fill="#e0e0e0"/>
                <rect x="52" y="36" width="16" height="3" rx="1" fill="#e0e0e0"/>
              </svg>
            </div>
            <span class="right-panel__empty-text">请从左侧选择退药患者</span>
          </div>

          <ReturnMedicineCenterPanel
            v-else
            ref="centerPanelRef"
            :prescription-blocks="prescriptionBlocks"
            :loading="loading"
            @update:reasons="handleReasonsUpdate"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted, watch } from 'vue'
import { debounce } from 'lodash-es'
import { User, Document } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'
import PatientCard from '@/components/patient/PatientCard.vue'
import type { PatientSummary } from '@/components/patient/type'
import ReturnMedicineCenterPanel from './components/ReturnMedicineCenterPanel.vue'

import {
  pageOutpatientReturnMedicinePatientQueue,
  listOutpatientReturnMedicineSlips,
  returnOutpatientMedicine,
  adaptPatientSummary
} from '@/apis/pharmacy/return-medicine/outpatient'
import { adaptReturnMedicineBlocks } from './adapter'
import type { ReturnMedicinePrescriptionBlock } from './type'

const queueScope = ref<'pending' | 'completed'>('pending')
const searchKeyword = ref('')
const selectedDate = ref('')
const selectedPatientId = ref<string | null>(null)
const patientSummaries = ref<PatientSummary[]>([])
const prescriptionBlocks = ref<ReturnMedicinePrescriptionBlock[]>([])
const loading = ref(false)
const returnReasons = ref<Record<string, string>>({})

const centerPanelRef = ref<InstanceType<typeof ReturnMedicineCenterPanel> | null>(null)

const debouncedLoadPatients = debounce(loadPatients, 300)

async function loadPatients() {
  loading.value = true
  try {
    const result = await pageOutpatientReturnMedicinePatientQueue({
      status: queueScope.value,
      keyword: searchKeyword.value
    })
    patientSummaries.value = result.records.map(adaptPatientSummary)
  } finally {
    loading.value = false
  }
}

async function handleSelectPatient(patient: PatientSummary) {
  selectedPatientId.value = String(patient.id)
  loading.value = true
  try {
    const slips = await listOutpatientReturnMedicineSlips(String(patient.id))
    prescriptionBlocks.value = adaptReturnMedicineBlocks(slips)
  } finally {
    loading.value = false
  }
}

function handleSearch() {
  debouncedLoadPatients()
}

function handleReasonsUpdate(reasons: Record<string, string>) {
  returnReasons.value = { ...reasons }
}

async function handleBatchReturn() {
  const selectedIds = centerPanelRef.value?.getSelectedIds() || []
  if (selectedIds.length === 0) {
    ElMessage.warning('请先勾选需要退药的药品')
    return
  }

  const itemsWithReasons = selectedIds.map(id => {
    const reason = returnReasons.value[id] || '患者申请退药'
    return { itemId: id, reason }
  })

  loading.value = true
  try {
    const result = await returnOutpatientMedicine({
      encounterId: selectedPatientId.value!,
      dispenserId: '',
      items: itemsWithReasons
    })
    if (result.success) {
      ElMessage.success(result.message)
      await loadPatients()
      const patient = patientSummaries.value.find(p => String(p.id) === selectedPatientId.value)
      if (patient) {
        await handleSelectPatient(patient)
      }
    } else {
      ElMessage.error(result.message)
    }
  } finally {
    loading.value = false
  }
}

watch(queueScope, loadPatients)

onMounted(loadPatients)
</script>

<style scoped>
.outpatient-return-medicine-page {
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

.left-panel__date {
  padding: 0 16px 12px;
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
}

.left-panel__empty-text {
  font-size: 14px;
  color: #999;
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
}

.right-panel__toolbar-btn {
  font-size: 14px;
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