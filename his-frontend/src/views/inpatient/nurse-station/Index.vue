<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'

import NurseStationModulePlaceholder from './components/NurseStationModulePlaceholder.vue'
import NurseStationPatientPanel from './components/NurseStationPatientPanel.vue'
import CostDetail from './cost-detail/Index.vue'
import DispenseSummary from './dispense-summary/Index.vue'
import InOutTransfer from './in-out-transfer/Index.vue'
import InpatientBilling from './inpatient-billing/Index.vue'
import NursingRecord from './nursing-record/Index.vue'
import OrderAudit from './order-audit/Index.vue'
import OrderExecute from './order-execute/Index.vue'
import TemperatureSheet from './temperature-sheet/Index.vue'
import {
  DEFAULT_NURSE_STATION_TAB,
  getNurseStationOperationConfig,
  NURSE_STATION_OPERATION_CONFIGS
} from './constants'
import type { NurseStationOperationTab } from './type'
import { useNurseStationPatientWorkbench } from './use-nurse-station-patient-workbench'

defineOptions({
  name: 'InpatientNurseStation'
})

const activeOperationTab = ref<NurseStationOperationTab>(DEFAULT_NURSE_STATION_TAB)

const activeOperationConfig = computed(() => {
  const config = getNurseStationOperationConfig(activeOperationTab.value)
  if (!config) throw new Error(`未配置护士站业务页签：${activeOperationTab.value}`)
  return config
})

const {
  bootstrapPatientWorkbench,
  errorMessage,
  keyword,
  loading: patientLoading,
  normalizeSelection,
  pageIndex,
  pageSize,
  patients,
  refreshPatients,
  selectedPatientIds,
  statistics: patientStatistics,
  taskFilter,
  total: patientTotal
} = useNurseStationPatientWorkbench(() => activeOperationConfig.value.selectionPolicy)

const selectedPatients = computed(() => {
  const selectedIds = new Set(selectedPatientIds.value)
  return patients.value.filter((patient) => selectedIds.has(patient.encounterId))
})

function handleOperationTabChange(tab: NurseStationOperationTab) {
  activeOperationTab.value = tab
  normalizeSelection()
}

onMounted(() => void bootstrapPatientWorkbench())
</script>

<template>
  <main class="nurse-station-page">
    <div class="nurse-station-workbench">
      <NurseStationPatientPanel
        v-model:selected-patient-ids="selectedPatientIds"
        v-model:task-filter="taskFilter"
        v-model:keyword="keyword"
        v-model:page-index="pageIndex"
        class="nurse-station-workbench__patient"
        :patients="patients"
        :statistics="patientStatistics"
        :selection-policy="activeOperationConfig.selectionPolicy"
        :loading="patientLoading"
        :error-message="errorMessage"
        :page-size="pageSize"
        :total="patientTotal"
        :enable-patient-drag="activeOperationTab === '入出转管理'"
        @refresh="refreshPatients"
      />

      <section class="nurse-station-workbench__operation" aria-label="护士站业务工作区">
        <nav class="operation-tabs" aria-label="护士站业务导航">
          <button
            v-for="item in NURSE_STATION_OPERATION_CONFIGS"
            :key="item.id"
            type="button"
            class="operation-tabs__item"
            :class="{
              'is-active': activeOperationTab === item.id
            }"
            :aria-current="activeOperationTab === item.id ? 'page' : undefined"
            @click="handleOperationTabChange(item.id)"
          >
            {{ item.id }}
          </button>
        </nav>

        <div class="nurse-station-workbench__content">
          <KeepAlive>
            <InOutTransfer
              v-if="activeOperationTab === '入出转管理'"
              v-model:selected-patient-ids="selectedPatientIds"
              :patients="patients"
              @patients-changed="refreshPatients"
            />
            <OrderAudit
              v-else-if="activeOperationTab === '医嘱校对'"
              :patients="selectedPatients"
              @patients-changed="refreshPatients"
            />
            <OrderExecute
              v-else-if="activeOperationTab === '医嘱执行'"
              :patients="selectedPatients"
              @patients-changed="refreshPatients"
            />
            <NursingRecord
              v-else-if="activeOperationTab === '护理记录'"
              :patients="selectedPatients"
            />
            <TemperatureSheet
              v-else-if="activeOperationTab === '体温单'"
              :patients="selectedPatients"
            />
            <InpatientBilling
              v-else-if="activeOperationTab === '住院记账'"
              :patients="selectedPatients"
            />
            <CostDetail
              v-else-if="activeOperationTab === '费用明细'"
              :patients="selectedPatients"
            />
            <DispenseSummary
              v-else-if="activeOperationTab === '汇总领药'"
              :patients="selectedPatients"
            />
          </KeepAlive>
          <NurseStationModulePlaceholder
            v-if="
              ![
                '入出转管理',
                '医嘱校对',
                '医嘱执行',
                '护理记录',
                '体温单',
                '住院记账',
                '费用明细',
                '汇总领药'
              ].includes(activeOperationTab)
            "
            :key="activeOperationTab"
            :config="activeOperationConfig"
            :selected-patient-count="selectedPatientIds.length"
          />
        </div>
      </section>
    </div>
  </main>
</template>

<style scoped>
.nurse-station-page {
  /* 页面级文字色供当前和后续护士站模块统一继承。 */
  --nurse-text-primary: #303133;
  --nurse-text-regular: #606266;
  --nurse-text-secondary: #6b7280;

  box-sizing: border-box;
  width: 100%;
  height: calc(100% - var(--el-tabs-header-height, 40px));
  min-width: 0;
  min-height: 0;
  padding: 8px;
  overflow: hidden;
  background: var(--el-fill-color-lighter);
  color: var(--nurse-text-regular);
}

/*
 * Chrome 会继承非标准的 -webkit-text-fill-color，而且普通 color 不能覆盖它。
 * 护士站的业务卡片包含大量状态色，因此这里统一要求每个后代使用自己的 currentColor：
 * 既能阻断外层浅色填充继续向下继承，也不会破坏按钮、警告、成功和禁用状态的原有颜色。
 */
.nurse-station-page :deep(*) {
  -webkit-text-fill-color: currentColor;
}

.nurse-station-workbench {
  display: grid;
  width: 100%;
  height: 100%;
  min-width: 0;
  min-height: 0;
  grid-template-columns: minmax(310px, 22%) minmax(0, 1fr);
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  box-shadow: var(--el-box-shadow-lighter);
}

.nurse-station-workbench__patient,
.nurse-station-workbench__operation {
  min-width: 0;
  min-height: 0;
}

.nurse-station-workbench__operation {
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.operation-tabs {
  display: flex;
  min-height: 48px;
  overflow-x: auto;
  border-bottom: 1px solid var(--el-border-color);
  background: var(--el-bg-color);
  scrollbar-width: thin;
}

.operation-tabs__item {
  position: relative;
  display: inline-flex;
  min-width: 104px;
  flex: 1 0 auto;
  align-items: center;
  justify-content: center;
  padding: 0 12px;
  cursor: pointer;
  border: 0;
  background: transparent;
  color: var(--nurse-text-regular);
  font: inherit;
  font-size: 14px;
  transition:
    color 0.2s ease,
    background-color 0.2s ease;
}

.operation-tabs__item:hover {
  color: var(--el-color-primary);
  background: var(--el-fill-color-light);
}

.operation-tabs__item.is-active {
  color: var(--el-color-primary);
  font-weight: 600;
}

.operation-tabs__item.is-active::after {
  position: absolute;
  right: 16px;
  bottom: 0;
  left: 16px;
  height: 2px;
  border-radius: 2px;
  background: var(--el-color-primary);
  content: '';
}

.operation-tabs__item.is-disabled {
  cursor: not-allowed;
  color: var(--el-text-color-placeholder);
}

.operation-tabs__item.is-disabled:hover {
  background: transparent;
  color: var(--el-text-color-placeholder);
}

.nurse-station-workbench__content {
  min-height: 0;
  flex: 1;
  overflow: hidden;
}

@media (max-width: 1366px) {
  .nurse-station-page {
    padding: 6px;
  }

  .nurse-station-workbench {
    grid-template-columns: minmax(300px, 25%) minmax(0, 1fr);
  }

  .operation-tabs__item {
    min-width: 96px;
    padding-inline: 10px;
  }
}
</style>
