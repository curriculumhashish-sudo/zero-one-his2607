<script setup lang="ts">
import { computed } from 'vue'
import type { PatientSummary } from './type'

defineOptions({
  name: 'PatientInfoBar'
})

interface PatientInfoBarProps {
  patient?: PatientSummary | null
}

const props = withDefaults(defineProps<PatientInfoBarProps>(), {
  patient: null
})

defineSlots<{
  actions?(props: { patient: PatientSummary }): any
}>()

const displayName = computed(() => props.patient?.name?.trim() || '无档案登记')

const ageText = computed(() => {
  const age = props.patient?.age
  if (age === null || age === undefined || age === '') return ''

  const text = String(age).trim()
  if (!text) return ''
  return /[岁月天]/.test(text) ? text : `${text}岁`
})

const identityMeta = computed(() => {
  if (!props.patient) return ''

  return [props.patient.gender?.trim(), ageText.value].filter(Boolean).join(' · ')
})

const detailItems = computed(() => {
  const patient = props.patient
  if (!patient) return []

  return [
    { label: '患者号', value: patient.patientNo?.trim() },
    { label: '就诊号', value: patient.visitNo?.trim() },
    { label: '科室', value: patient.departmentName?.trim() },
    { label: '就诊时间', value: patient.visitTime?.trim() }
  ].filter((item): item is { label: string; value: string } => Boolean(item.value))
})
</script>

<template>
  <section class="patient-info-bar" aria-label="患者信息">
    <div v-if="patient" class="patient-info-bar__layout">
      <div class="patient-info-bar__main">
        <div class="patient-info-bar__identity">
          <span class="patient-info-bar__label">患者</span>
          <strong class="patient-info-bar__name">{{ displayName }}</strong>
          <span v-if="identityMeta" class="patient-info-bar__identity-meta">
            {{ identityMeta }}
          </span>
        </div>

        <el-tag v-if="patient.serviceType" size="small" effect="light">
          {{ patient.serviceType }}
        </el-tag>
        <el-tag
          v-if="patient.status"
          :type="patient.statusTone ?? 'info'"
          size="small"
          effect="light"
        >
          {{ patient.status }}
        </el-tag>
        <el-tag v-if="patient.paymentType" type="primary" size="small" effect="plain">
          {{ patient.paymentType }}
        </el-tag>
      </div>

      <div v-if="detailItems.length" class="patient-info-bar__details">
        <span v-for="item in detailItems" :key="item.label" class="patient-info-bar__detail">
          <span class="patient-info-bar__label">{{ item.label }}</span>
          <span class="patient-info-bar__detail-value" :title="item.value">
            {{ item.value }}
          </span>
        </span>
      </div>

      <div v-if="patient.riskHint || patient.priority" class="patient-info-bar__risks">
        <el-tag v-if="patient.priority" type="warning" size="small" effect="light" round>
          {{ patient.priority }}
        </el-tag>
        <el-tag
          v-if="patient.riskHint"
          class="patient-info-bar__risk-tag"
          type="danger"
          size="small"
          effect="light"
          round
          :title="patient.riskHint"
        >
          {{ patient.riskHint }}
        </el-tag>
      </div>

      <div v-if="$slots.actions" class="patient-info-bar__actions">
        <slot name="actions" :patient="patient" />
      </div>
    </div>

    <div v-else class="patient-info-bar__empty" role="status">
      <span class="patient-info-bar__empty-title">当前未选择患者</span>
      <span class="patient-info-bar__empty-description">请先从患者列表中选择一位患者</span>
    </div>
  </section>
</template>

<style scoped>
.patient-info-bar {
  container-type: inline-size;
  min-height: 56px;
  padding: 10px 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 8px;
  color: var(--el-text-color-primary);
  background: var(--el-bg-color);
}

.patient-info-bar__layout {
  display: flex;
  width: 100%;
  min-width: 0;
  align-items: center;
  gap: 12px 20px;
}

.patient-info-bar__main,
.patient-info-bar__identity,
.patient-info-bar__detail,
.patient-info-bar__risks,
.patient-info-bar__actions {
  display: flex;
  min-width: 0;
  align-items: center;
}

.patient-info-bar__main {
  flex-shrink: 0;
  flex-wrap: wrap;
  gap: 8px;
}

.patient-info-bar__identity {
  gap: 8px;
}

.patient-info-bar__label {
  flex-shrink: 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.patient-info-bar__name {
  max-width: 160px;
  overflow: hidden;
  font-size: 16px;
  font-weight: 600;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-info-bar__identity-meta {
  color: var(--el-text-color-regular);
  font-size: 14px;
  white-space: nowrap;
}

.patient-info-bar__details {
  display: grid;
  flex: 1;
  grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
  gap: 6px 18px;
}

.patient-info-bar__detail {
  width: 100%;
  max-width: none;
  gap: 5px;
  font-size: 13px;
}

.patient-info-bar__detail-value {
  overflow: hidden;
  font-weight: 500;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-info-bar__risks {
  max-width: 280px;
  flex-shrink: 1;
  gap: 6px;
}

.patient-info-bar__risk-tag {
  min-width: 0;
}

.patient-info-bar__risk-tag :deep(.el-tag__content) {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-info-bar__actions {
  flex-shrink: 0;
  gap: 8px;
  padding-left: 16px;
  border-left: 1px solid var(--el-border-color-lighter);
}

.patient-info-bar__empty {
  display: flex;
  width: 100%;
  align-items: baseline;
  justify-content: center;
  gap: 10px;
  color: var(--el-text-color-secondary);
}

.patient-info-bar__empty-title {
  color: var(--el-text-color-regular);
  font-size: 14px;
  font-weight: 600;
}

.patient-info-bar__empty-description {
  font-size: 13px;
}

@container (max-width: 820px) {
  .patient-info-bar__layout {
    flex-wrap: wrap;
  }

  .patient-info-bar__details {
    order: 4;
    flex-basis: 100%;
  }

  .patient-info-bar__actions {
    margin-left: auto;
  }
}

@container (max-width: 520px) {
  .patient-info-bar__main,
  .patient-info-bar__details {
    flex-basis: 100%;
  }

  .patient-info-bar__details {
    grid-template-columns: 1fr;
  }

  .patient-info-bar__actions {
    margin-left: 0;
    padding-left: 0;
    border-left: 0;
  }

  .patient-info-bar__empty {
    align-items: center;
    flex-direction: column;
    gap: 2px;
  }
}
</style>
