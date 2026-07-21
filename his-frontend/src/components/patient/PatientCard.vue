<script setup lang="ts">
import { computed } from 'vue'
import type { PatientStatusTone, PatientSummary } from './type'

defineOptions({
  name: 'PatientCard'
})

interface PatientCardProps {
  patient: PatientSummary
  selected?: boolean
}

const props = withDefaults(defineProps<PatientCardProps>(), {
  selected: false
})

const emit = defineEmits<{
  select: [patient: PatientSummary]
  view: [patient: PatientSummary]
}>()

const displayName = computed(() => props.patient.name?.trim() || '无档案登记')

const ageText = computed(() => {
  const age = props.patient.age
  if (age === null || age === undefined || age === '') return '-'

  const text = String(age).trim()
  if (!text) return '-'
  return /[岁月天]/.test(text) ? text : `${text}岁`
})

const metaText = computed(() => {
  const patient = props.patient
  return [
    patient.gender?.trim() || '-',
    ageText.value,
    patient.serviceType?.trim() || patient.priority?.trim() || '-'
  ].join(' · ')
})

const primaryNumber = computed(() => {
  const patient = props.patient
  if (patient.queueNumber !== null && patient.queueNumber !== undefined) {
    const queueNumber = String(patient.queueNumber).trim()
    if (queueNumber) return `排队号 ${queueNumber}`
  }

  return patient.visitNo?.trim() || patient.patientNo?.trim() || ''
})

const statusType = computed(() => {
  const toneMap: Record<PatientStatusTone, PatientStatusTone> = {
    primary: 'primary',
    success: 'success',
    warning: 'warning',
    danger: 'danger',
    info: 'info'
  }

  return toneMap[props.patient.statusTone ?? 'info']
})

function handleSelect() {
  emit('select', props.patient)
}

function handleView() {
  emit('view', props.patient)
}

function handleKeydown(event: KeyboardEvent) {
  if (event.key !== 'Enter' && event.key !== ' ') return
  event.preventDefault()
  handleSelect()
}
</script>

<template>
  <article
    class="patient-card"
    :class="{ 'patient-card--selected': selected }"
    role="button"
    tabindex="0"
    :aria-pressed="selected"
    @click="handleSelect"
    @keydown="handleKeydown"
  >
    <header class="patient-card__header">
      <span class="patient-card__name">{{ displayName }}</span>
      <span class="patient-card__meta" :title="metaText">{{ metaText }}</span>
    </header>

    <div class="patient-card__content">
      <div v-if="patient.visitTime" class="patient-card__row">
        <span class="patient-card__label">就诊时间</span>
        <span class="patient-card__value">{{ patient.visitTime }}</span>
      </div>
      <div v-if="patient.departmentName" class="patient-card__row">
        <span class="patient-card__label">科室</span>
        <span class="patient-card__value">{{ patient.departmentName }}</span>
      </div>
      <div v-if="patient.riskHint" class="patient-card__risk">
        {{ patient.riskHint }}
      </div>
    </div>

    <footer class="patient-card__footer">
      <span class="patient-card__number" :title="primaryNumber">{{ primaryNumber }}</span>

      <div class="patient-card__actions">
        <el-tag v-if="patient.paymentType" size="small" effect="light" round>
          {{ patient.paymentType }}
        </el-tag>
        <el-tag v-if="patient.status" :type="statusType" size="small" effect="light" round>
          {{ patient.status }}
        </el-tag>
        <el-button class="patient-card__view" link type="primary" @click.stop="handleView">
          查看
        </el-button>
      </div>
    </footer>
  </article>
</template>

<style scoped>
.patient-card {
  overflow: hidden;
  padding: 14px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 12px;
  background: var(--el-bg-color);
  cursor: pointer;
  outline: none;
  transition:
    border-color 0.2s ease,
    background-color 0.2s ease,
    box-shadow 0.2s ease;
}

.patient-card:hover {
  border-color: var(--el-color-primary-light-5);
  background: var(--el-fill-color-light);
}

.patient-card:focus-visible {
  box-shadow: 0 0 0 3px var(--el-color-primary-light-8);
}

.patient-card--selected {
  border-color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
  box-shadow: var(--el-box-shadow-light);
}

.patient-card__header,
.patient-card__footer,
.patient-card__row {
  display: flex;
  align-items: center;
}

.patient-card__header {
  justify-content: space-between;
  gap: 12px;
  margin-bottom: 12px;
}

.patient-card__name {
  flex-shrink: 0;
  color: var(--el-text-color-primary);
  font-size: 16px;
  font-weight: 600;
}

.patient-card__meta,
.patient-card__number,
.patient-card__value {
  min-width: 0;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-card__meta {
  color: var(--el-text-color-secondary);
  font-size: 12px;
  text-align: right;
}

.patient-card__content {
  min-height: 24px;
  color: var(--el-text-color-regular);
  font-size: 12px;
}

.patient-card__row {
  gap: 8px;
  margin-top: 6px;
}

.patient-card__label {
  flex: 0 0 52px;
  color: var(--el-text-color-secondary);
}

.patient-card__value {
  color: var(--el-text-color-primary);
  font-weight: 500;
}

.patient-card__risk {
  margin-top: 8px;
  padding: 5px 8px;
  border-radius: 6px;
  color: var(--el-color-warning-dark-2);
  background: var(--el-color-warning-light-9);
  line-height: 1.5;
}

.patient-card__footer {
  justify-content: space-between;
  gap: 10px;
  margin-top: 12px;
  padding-top: 12px;
  border-top: 1px dashed var(--el-border-color);
}

.patient-card__number {
  flex: 1;
  color: var(--el-text-color-primary);
  font-size: 12px;
  font-weight: 500;
}

.patient-card__actions {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  gap: 8px;
}

.patient-card__view {
  padding: 0;
}
</style>
