<script setup lang="ts">
import { computed } from 'vue'
import { Calendar, User } from '@element-plus/icons-vue'
import type { PatientSummary } from './type'

defineOptions({
  name: 'PatientInfoBar'
})

interface PatientInfoBarProps {
  patient?: PatientSummary | null
  /** 医生站摘要模式只展示对应场景要求的挂号/住院信息。 */
  displayMode?: 'default' | 'outpatient-registration' | 'inpatient-registration'
  /** 电话和接诊医生来自就诊上下文，不强行塞进公共患者摘要模型。 */
  phone?: string | null
  doctorName?: string | null
}

const props = withDefaults(defineProps<PatientInfoBarProps>(), {
  patient: null,
  displayMode: 'default',
  phone: '',
  doctorName: ''
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

const registrationTime = computed(() => {
  const value = props.patient?.visitTime?.trim() ?? ''
  // 标准时间去掉年份，与参考界面的“08-02 07:30”保持一致。
  return /^\d{4}-\d{2}-\d{2}/.test(value) ? value.slice(5, 16) : value
})

const registrationRecordNo = computed(() => {
  const patient = props.patient
  if (!patient) return ''
  return props.displayMode === 'inpatient-registration'
    ? patient.visitNo?.trim()
    : patient.patientNo?.trim()
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
    <div
      v-if="patient && displayMode !== 'default'"
      class="patient-info-bar__outpatient"
    >
      <div class="patient-info-bar__outpatient-primary">
        <div class="patient-info-bar__outpatient-person">
          <el-icon class="patient-info-bar__outpatient-icon"><User /></el-icon>
          <span class="patient-info-bar__outpatient-label">患者：</span>
          <strong class="patient-info-bar__name">{{ displayName }}</strong>
          <strong v-if="patient.gender" class="patient-info-bar__outpatient-gender">
            {{ patient.gender }}
          </strong>
          <span v-if="ageText" class="patient-info-bar__outpatient-value">{{ ageText }}</span>
        </div>
        <div
          v-if="displayMode === 'outpatient-registration'"
          class="patient-info-bar__outpatient-field"
        >
          <span class="patient-info-bar__outpatient-label">电话：</span>
          <span class="patient-info-bar__outpatient-value">{{ phone || '—' }}</span>
        </div>
        <div class="patient-info-bar__outpatient-field">
          <span class="patient-info-bar__outpatient-label">病历号：</span>
          <strong class="patient-info-bar__outpatient-value">
            {{ registrationRecordNo || '—' }}
          </strong>
        </div>
        <el-tag v-if="patient.serviceType" type="primary" effect="light">
          {{ patient.serviceType }}
        </el-tag>
      </div>

      <div class="patient-info-bar__outpatient-secondary">
        <div class="patient-info-bar__outpatient-field">
          <el-icon class="patient-info-bar__outpatient-icon"><Calendar /></el-icon>
          <span class="patient-info-bar__outpatient-label">挂号时间：</span>
          <span class="patient-info-bar__outpatient-value">
            {{ registrationTime || '—' }}
          </span>
        </div>
        <div class="patient-info-bar__outpatient-field">
          <span class="patient-info-bar__outpatient-label">医生：</span>
          <strong class="patient-info-bar__outpatient-value">{{ doctorName || '—' }}</strong>
        </div>
      </div>

      <div v-if="$slots.actions" class="patient-info-bar__actions">
        <slot name="actions" :patient="patient" />
      </div>
    </div>

    <div v-else-if="patient" class="patient-info-bar__layout">
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

      <div v-if="patient.priority" class="patient-info-bar__risks">
        <el-tag v-if="patient.priority" type="warning" size="small" effect="light" round>
          {{ patient.priority }}
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
  padding: 8px 12px;
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

.patient-info-bar__outpatient {
  display: flex;
  width: 100%;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 8px 16px;
}

.patient-info-bar__outpatient-primary,
.patient-info-bar__outpatient-secondary {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 10px;
  white-space: nowrap;
}

.patient-info-bar__outpatient-person,
.patient-info-bar__outpatient-field {
  display: inline-flex;
  flex-shrink: 0;
  align-items: center;
  gap: 3px;
}

.patient-info-bar__outpatient-person {
  gap: 5px;
}

.patient-info-bar__outpatient-primary {
  flex: 1 1 auto;
  overflow: hidden;
}

.patient-info-bar__outpatient-secondary {
  flex-shrink: 0;
  padding-left: 16px;
  border-left: 1px solid var(--el-border-color-lighter);
}

.patient-info-bar__outpatient-icon {
  flex-shrink: 0;
  font-size: 20px;
}

.patient-info-bar__outpatient-label {
  flex-shrink: 0;
  color: var(--el-text-color-primary);
  font-size: 14px;
  font-weight: 600;
}

.patient-info-bar__outpatient-gender,
.patient-info-bar__outpatient-value {
  flex-shrink: 0;
  color: var(--el-text-color-primary);
  font-size: 14px;
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
  .patient-info-bar__outpatient {
    align-items: flex-start;
    flex-direction: column;
    gap: 6px;
  }

  .patient-info-bar__outpatient-primary {
    width: 100%;
    flex-wrap: wrap;
    overflow: visible;
  }

  .patient-info-bar__outpatient-secondary {
    width: 100%;
    padding-top: 6px;
    padding-left: 0;
    border-top: 1px solid var(--el-border-color-lighter);
    border-left: 0;
  }

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
