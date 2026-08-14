<script setup lang="ts">
import { computed } from 'vue'

import type { DoctorPatientContextVO, DoctorVisitStatus } from '@/apis/outpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'
import type { PatientSummary } from '@/components/patient/type'

defineOptions({
  name: 'PatientContextPanel'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    patient?: PatientSummary | null
    context?: DoctorPatientContextVO | null
    loading?: boolean
    error?: string
  }>(),
  {
    patientSelected: false,
    patient: null,
    context: null,
    loading: false,
    error: ''
  }
)

const emit = defineEmits<{
  retry: []
}>()

const displayName = computed(() => props.patient?.name?.trim() || '无档案登记')

const ageText = computed(() => {
  const age = props.patient?.age
  if (age === null || age === undefined || age === '') return '-'
  const text = String(age).trim()
  return /[岁月天]/.test(text) ? text : `${text}岁`
})

const patientMeta = computed(() =>
  [
    props.patient?.gender?.trim() || '-',
    ageText.value,
    props.patient?.serviceType?.trim() || '-'
  ].join(' · ')
)

const statusType = computed(() => {
  const statusTypeMap: Record<DoctorVisitStatus, 'success' | 'warning' | 'info'> = {
    arrived: 'warning',
    'in-progress': 'success',
    finished: 'info'
  }
  return statusTypeMap[props.context?.visitStatus ?? 'arrived']
})
</script>

<template>
  <section class="patient-context-panel">
    <EmptyState
      v-if="!patientSelected"
      compact
      title="请选择患者"
      description="从左侧患者列表中选择一位患者"
    />

    <!-- 骨架屏不会像半透明 loading 遮罩一样让 Chrome 中的文字变浅。 -->
    <el-skeleton v-else-if="loading" :rows="8" animated />

    <EmptyState
      v-else-if="error"
      compact
      title="就诊信息加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!context"
      compact
      title="请先接诊患者"
      description="点击顶部接诊按钮后查看本次就诊信息"
    />

    <article v-else-if="patient && context" class="patient-context-card">
      <header class="patient-context-card__header">
        <div class="patient-context-card__headline">
          <strong class="patient-context-card__name">{{ displayName }}</strong>
          <el-tag :type="statusType" effect="dark" round>
            {{ context.visitStatusName }}
          </el-tag>
        </div>
        <div class="patient-context-card__meta">{{ patientMeta }}</div>
      </header>

      <dl class="patient-context-card__details">
        <div class="patient-context-card__row">
          <dt>病历号</dt>
          <dd>{{ patient.patientNo || '—' }}</dd>
        </div>
        <div class="patient-context-card__row">
          <dt>性别</dt>
          <dd>{{ patient.gender || '—' }}</dd>
        </div>
        <div class="patient-context-card__row">
          <dt>费用性质</dt>
          <dd>
            <el-tag type="primary" effect="dark" round>
              {{ patient.paymentType || '—' }}
            </el-tag>
          </dd>
        </div>
        <div class="patient-context-card__row">
          <dt>就诊时间</dt>
          <dd>{{ patient.visitTime || '—' }}</dd>
        </div>
        <div class="patient-context-card__row">
          <dt>挂号科室</dt>
          <dd>{{ patient.departmentName || '—' }}</dd>
        </div>
        <div class="patient-context-card__row">
          <dt>电话号码</dt>
          <dd>{{ context.phone || '—' }}</dd>
        </div>
      </dl>
    </article>
  </section>
</template>

<style scoped>
.patient-context-panel {
  box-sizing: border-box;
  width: 100%;
  min-height: 100%;
  padding: 4px;
  color: #1f2937;
  background: var(--el-bg-color);
}

.patient-context-card {
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: 6px;
  background: var(--el-bg-color);
}

.patient-context-card__header {
  padding: 12px 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.patient-context-card__headline {
  display: flex;
  min-width: 0;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.patient-context-card__name {
  min-width: 0;
  overflow: hidden;
  color: #111827;
  font-size: 18px;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.patient-context-card__headline :deep(.el-tag) {
  flex-shrink: 0;
  padding-right: 14px;
  padding-left: 14px;
  font-size: 14px;
  font-weight: 600;
}

.patient-context-card__meta {
  margin-top: 6px;
  color: #1f2937;
  font-size: 14px;
}

.patient-context-card__details {
  margin: 0;
  padding: 0 16px;
}

.patient-context-card__row {
  display: flex;
  min-height: 52px;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.patient-context-card__row:last-child {
  border-bottom: 0;
}

.patient-context-card__row dt,
.patient-context-card__row dd {
  margin: 0;
}

.patient-context-card__row dt {
  flex-shrink: 0;
  color: #909399;
  font-size: 14px;
}

.patient-context-card__row dd {
  min-width: 0;
  overflow-wrap: anywhere;
  color: #111827;
  font-size: 14px;
  text-align: right;
}

.patient-context-card__row :deep(.el-tag) {
  padding-right: 12px;
  padding-left: 12px;
  font-size: 14px;
  font-weight: 600;
}
</style>
