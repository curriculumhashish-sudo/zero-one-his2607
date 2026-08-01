<script setup lang="ts">
import { computed, ref } from 'vue'
import { DArrowLeft, DArrowRight, Refresh, Search } from '@element-plus/icons-vue'

import { NURSE_STATION_QUEUE_SCOPE_OPTIONS, NURSE_STATION_TASK_FILTER_OPTIONS } from '../constants'
import type {
  NurseStationHouseOption,
  NurseStationPatientStatistics,
  NurseStationPatientSummary,
  NurseStationQueueScope,
  NurseStationSelectionPolicy,
  NurseStationTaskFilter,
  NurseStationWardOption
} from '../type'

defineOptions({
  name: 'NurseStationPatientPanel'
})

interface Props {
  patients: NurseStationPatientSummary[]
  statistics: NurseStationPatientStatistics
  selectedPatientIds: string[]
  selectionPolicy: NurseStationSelectionPolicy
  queueScope: NurseStationQueueScope
  taskFilter: NurseStationTaskFilter
  keyword: string
  wardOptions: NurseStationWardOption[]
  houseOptions: NurseStationHouseOption[]
  selectedWardIds: string[]
  selectedHouseIds: string[]
  /** 入出转页签中允许将患者卡片拖到床位图。 */
  enablePatientDrag?: boolean
  loading?: boolean
  errorMessage?: string
}

const props = withDefaults(defineProps<Props>(), {
  loading: false,
  errorMessage: '',
  enablePatientDrag: false
})

const emit = defineEmits<{
  'update:selectedPatientIds': [value: string[]]
  'update:queueScope': [value: NurseStationQueueScope]
  'update:taskFilter': [value: NurseStationTaskFilter]
  'update:keyword': [value: string]
  'update:selectedWardIds': [value: string[]]
  'update:selectedHouseIds': [value: string[]]
  refresh: []
}>()

const spatialFilterExpanded = ref(true)

const selectedIdSet = computed(() => new Set(props.selectedPatientIds))

const isRecallListMode = computed(() => props.queueScope === 'settled-discharged')

function handleQueueScopeChange(value: NurseStationQueueScope) {
  emit('update:queueScope', value)
  if (value === 'settled-discharged') {
    emit('update:selectedPatientIds', [])
  }
}

function handleTaskFilterChange(value: NurseStationTaskFilter) {
  emit('update:taskFilter', value)
}

function handleWardIdsChange(values: Array<string | number | boolean>) {
  emit('update:selectedWardIds', values.map(String))
}

function handleHouseIdsChange(values: Array<string | number | boolean>) {
  emit('update:selectedHouseIds', values.map(String))
}

function togglePatient(patientId: string, checked?: boolean) {
  if (isRecallListMode.value) return

  if (props.selectionPolicy === 'single') {
    emit('update:selectedPatientIds', checked === false ? [] : [patientId])
    return
  }

  const next = new Set(props.selectedPatientIds)
  const shouldSelect = checked ?? !next.has(patientId)
  if (shouldSelect) next.add(patientId)
  else next.delete(patientId)
  emit('update:selectedPatientIds', [...next])
}

function formatBalance(patient: NurseStationPatientSummary) {
  if (patient.availableBalance === undefined) return '--'
  return `${patient.availableBalance.toFixed(2)} 元`
}

function handlePatientDragStart(event: DragEvent, patient: NurseStationPatientSummary) {
  if (!props.enablePatientDrag || isRecallListMode.value || !event.dataTransfer) return
  const payload = JSON.stringify({ encounterId: patient.encounterId })
  event.dataTransfer.effectAllowed = 'move'
  event.dataTransfer.setData('application/x-his-nurse-patient', payload)
  event.dataTransfer.setData('text/plain', patient.encounterId)
  // 拖拽入口同时将患者设为当前主患者，保持左右两区上下文一致。
  emit('update:selectedPatientIds', [patient.encounterId])
}
</script>

<template>
  <aside class="patient-panel" aria-label="住院患者列表">
    <header class="patient-panel__statistics">
      <div class="statistic statistic--total">
        <span class="statistic__label">总人数</span>
        <strong class="statistic__value">{{ statistics.totalCount }}</strong>
      </div>
      <div class="statistic statistic--audit">
        <span class="statistic__label">待校对</span>
        <strong class="statistic__value">{{ statistics.pendingAuditCount }}</strong>
      </div>
      <div class="statistic statistic--execute">
        <span class="statistic__label">待执行</span>
        <strong class="statistic__value">{{ statistics.pendingExecuteCount }}</strong>
      </div>
    </header>

    <div class="patient-panel__search">
      <div class="patient-panel__search-row">
        <el-input
          :model-value="keyword"
          clearable
          placeholder="姓名/病区/床号模糊搜索"
          @update:model-value="emit('update:keyword', $event)"
          @keyup.enter="emit('refresh')"
        >
          <template #prefix>
            <el-icon><Search /></el-icon>
          </template>
        </el-input>
        <el-button :icon="Refresh" circle title="刷新患者列表" @click="emit('refresh')" />
      </div>
      <div class="patient-panel__search-row">
        <el-select
          :model-value="queueScope"
          aria-label="患者范围"
          @update:model-value="handleQueueScopeChange"
        >
          <el-option
            v-for="item in NURSE_STATION_QUEUE_SCOPE_OPTIONS"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
        <el-select
          :model-value="taskFilter"
          aria-label="任务状态"
          :disabled="isRecallListMode"
          @update:model-value="handleTaskFilterChange"
        >
          <el-option
            v-for="item in NURSE_STATION_TASK_FILTER_OPTIONS"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
      </div>
    </div>

    <div class="patient-panel__body">
      <section
        class="patient-panel__spatial"
        :class="{ 'is-collapsed': !spatialFilterExpanded }"
        aria-label="病区病房筛选"
      >
        <div class="patient-panel__spatial-toolbar">
          <el-button
            link
            type="primary"
            :title="spatialFilterExpanded ? '收起病区病房' : '展开病区病房'"
            @click="spatialFilterExpanded = !spatialFilterExpanded"
          >
            <el-icon>
              <DArrowLeft v-if="spatialFilterExpanded" />
              <DArrowRight v-else />
            </el-icon>
          </el-button>
        </div>
        <div v-show="spatialFilterExpanded" class="patient-panel__spatial-content">
          <h3>病区</h3>
          <el-checkbox-group
            :model-value="selectedWardIds"
            class="patient-panel__filter-group"
            @update:model-value="handleWardIdsChange"
          >
            <el-checkbox v-for="ward in wardOptions" :key="ward.id" :value="ward.id">
              {{ ward.name }}
            </el-checkbox>
          </el-checkbox-group>
          <el-empty v-if="wardOptions.length === 0" :image-size="42" description="暂无病区" />
          <el-divider />
          <h3>病房</h3>
          <el-checkbox-group
            :model-value="selectedHouseIds"
            class="patient-panel__filter-group"
            @update:model-value="handleHouseIdsChange"
          >
            <el-checkbox v-for="house in houseOptions" :key="house.id" :value="house.id">
              {{ house.name }}
            </el-checkbox>
          </el-checkbox-group>
          <el-empty v-if="houseOptions.length === 0" :image-size="42" description="请先选择病区" />
        </div>
      </section>

      <div class="patient-panel__divider" />

      <section v-loading="loading" class="patient-panel__list" aria-live="polite">
        <el-alert
          v-if="errorMessage"
          class="patient-panel__error"
          :title="errorMessage"
          type="error"
          show-icon
          :closable="false"
        />
        <el-empty
          v-if="patients.length === 0"
          :description="isRecallListMode ? '暂无已出院结算患者' : '暂无符合条件的在院患者'"
        />

        <div v-else class="patient-card-list">
          <article
            v-for="patient in patients"
            :key="patient.encounterId"
            class="patient-card"
            :class="{
              'is-selected': selectedIdSet.has(patient.encounterId),
              'is-draggable': enablePatientDrag && !isRecallListMode
            }"
            :draggable="enablePatientDrag && !isRecallListMode"
            @click="togglePatient(patient.encounterId)"
            @dragstart="handlePatientDragStart($event, patient)"
          >
            <div class="patient-card__header">
              <el-checkbox
                :model-value="selectedIdSet.has(patient.encounterId)"
                :disabled="isRecallListMode"
                @click.stop
                @change="togglePatient(patient.encounterId, Boolean($event))"
              />
              <strong>{{ patient.patientName }}</strong>
              <span>{{ patient.genderText || '-' }} / {{ patient.ageText || '-' }}</span>
            </div>
            <div class="patient-card__meta">
              <el-tag size="small" effect="plain">{{ patient.statusText || '在院' }}</el-tag>
              <span :class="{ 'is-warning': patient.lowBalanceWarning }">
                余额 {{ formatBalance(patient) }}
              </span>
            </div>
            <div class="patient-card__diagnosis" :title="patient.diagnosisSummary">
              诊断：{{ patient.diagnosisSummary || '-' }}
            </div>
            <div class="patient-card__footer">
              <span>
                {{ patient.wardName || '-' }} | {{ patient.houseName || '-' }} |
                {{ patient.bedName || '未分床' }}
              </span>
              <span>
                <em>校 {{ patient.pendingAuditCount }}</em>
                <em>执 {{ patient.pendingExecuteCount }}</em>
              </span>
            </div>
          </article>
        </div>
      </section>
    </div>
  </aside>
</template>

<style scoped>
.patient-panel {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  border-right: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.patient-panel__statistics {
  display: grid;
  min-height: 48px;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.statistic {
  display: flex;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 6px;
  padding: 0 10px;
}

.statistic + .statistic {
  border-left: 1px solid var(--el-border-color-lighter);
}

.statistic__label {
  color: var(--el-text-color-secondary);
  font-size: 12px;
  white-space: nowrap;
}

.statistic__value {
  color: var(--el-color-primary);
  font-size: 20px;
}

.statistic--audit .statistic__value {
  color: var(--el-color-warning);
}

.statistic--execute .statistic__value {
  color: var(--el-color-danger);
}

.patient-panel__search {
  display: flex;
  flex-direction: column;
  gap: 8px;
  padding: 8px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.patient-panel__search-row {
  display: flex;
  min-width: 0;
  gap: 8px;
}

.patient-panel__search-row .el-input,
.patient-panel__search-row .el-select {
  min-width: 0;
  flex: 1;
}

.patient-panel__body {
  display: flex;
  min-height: 0;
  flex: 1;
}

.patient-panel__spatial {
  display: flex;
  width: 112px;
  min-width: 112px;
  flex-direction: column;
  overflow: hidden;
  padding: 6px;
  transition: width 0.2s ease;
}

.patient-panel__spatial.is-collapsed {
  width: 34px;
  min-width: 34px;
  padding-inline: 2px;
}

.patient-panel__spatial-toolbar {
  display: flex;
  justify-content: flex-end;
}

.patient-panel__spatial.is-collapsed .patient-panel__spatial-toolbar {
  justify-content: center;
}

.patient-panel__spatial-content {
  min-height: 0;
  flex: 1;
  overflow: auto;
}

.patient-panel__spatial-content h3 {
  margin: 4px 0 0;
  color: var(--el-text-color-primary);
  font-size: 13px;
}

.patient-panel__filter-group {
  display: flex;
  flex-direction: column;
  gap: 2px;
  padding: 4px 0;
}

.patient-panel__filter-group :deep(.el-checkbox) {
  height: auto;
  margin-right: 0;
  white-space: normal;
}

.patient-panel__filter-group :deep(.el-checkbox__label) {
  overflow: hidden;
  padding-left: 5px;
  font-size: 12px;
  text-overflow: ellipsis;
}

.patient-panel__spatial-content :deep(.el-empty) {
  padding-block: 10px;
}

.patient-panel__spatial-content :deep(.el-empty__description) {
  margin-top: 4px;
}

.patient-panel__spatial-content :deep(.el-empty__description p) {
  font-size: 11px;
}

.patient-panel__spatial-content .el-divider {
  margin-block: 4px;
}

.patient-panel__divider {
  width: 1px;
  margin-block: 8px;
  background: var(--el-border-color-lighter);
}

.patient-panel__list {
  min-width: 0;
  min-height: 0;
  flex: 1;
  overflow: auto;
  padding: 6px;
}

.patient-panel__list > :deep(.el-empty) {
  height: 100%;
}

.patient-panel__error {
  margin-bottom: 8px;
}

.patient-card-list {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.patient-card {
  display: flex;
  min-height: 112px;
  cursor: pointer;
  flex-direction: column;
  gap: 6px;
  padding: 9px 10px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  transition:
    border-color 0.2s ease,
    box-shadow 0.2s ease,
    background-color 0.2s ease;
}

.patient-card:hover,
.patient-card.is-selected {
  border-color: var(--el-color-primary-light-5);
  background: var(--el-color-primary-light-9);
}

.patient-card.is-draggable {
  cursor: grab;
}

.patient-card.is-draggable:active {
  cursor: grabbing;
}

.patient-card.is-selected {
  box-shadow: 0 0 0 1px var(--el-color-primary-light-5);
}

.patient-card__header,
.patient-card__meta,
.patient-card__footer {
  display: flex;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
}

.patient-card__header strong {
  flex: 1;
  color: var(--el-text-color-primary);
}

.patient-card__header > span,
.patient-card__meta,
.patient-card__diagnosis,
.patient-card__footer {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.patient-card__meta .is-warning {
  color: var(--el-color-warning);
  font-weight: 600;
}

.patient-card__diagnosis {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-card__footer {
  margin-top: auto;
}

.patient-card__footer > span:first-child {
  min-width: 0;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-card__footer > span:last-child {
  display: flex;
  flex-shrink: 0;
  gap: 8px;
}

.patient-card__footer em {
  color: var(--el-color-warning);
  font-style: normal;
}

.patient-card__footer em:last-child {
  color: var(--el-color-danger);
}

@media (max-width: 1280px) {
  .statistic {
    flex-direction: column;
    justify-content: center;
    gap: 2px;
  }

  .patient-panel__spatial {
    width: 96px;
    min-width: 96px;
  }
}
</style>
