<script setup lang="ts">
import { computed } from 'vue'

import type { DoctorPatientContextVO } from '@/apis/outpatient/doctor-station'
import DescriptionList from '@/components/common/DescriptionList.vue'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'

import { DOCTOR_VISIT_STATUS_OPTIONS } from '../constants'

defineOptions({
  name: 'PatientContextPanel'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    context?: DoctorPatientContextVO | null
    loading?: boolean
    error?: string
  }>(),
  {
    patientSelected: false,
    context: null,
    loading: false,
    error: ''
  }
)

const emit = defineEmits<{
  retry: []
}>()

// DescriptionList 接收普通键值对象，这里复制一份以保持明确的组件边界。
const descriptionData = computed<Record<string, unknown>>(() =>
  props.context ? { ...props.context } : {}
)

const descriptionItems: DescriptionItem[] = [
  { prop: 'visitStatus', label: '就诊状态' },
  { prop: 'attendingDoctorName', label: '接诊医生' },
  { prop: 'acceptedAt', label: '接诊时间' },
  { prop: 'chiefComplaint', label: '分诊主诉' },
  { prop: 'allergyHistory', label: '过敏史' },
  { prop: 'pastMedicalHistory', label: '既往病史' },
  { prop: 'phone', label: '联系电话' },
  { prop: 'emergencyContact', label: '紧急联系人' },
  { prop: 'triageNote', label: '分诊备注' }
]
</script>

<template>
  <section class="patient-context-panel">
    <EmptyState
      v-if="!patientSelected"
      compact
      title="辅助信息"
      description="选择患者后查看就诊详情"
    />

    <!--
      加载时直接显示骨架屏，不使用半透明 v-loading 遮罩。
      遮罩会覆盖已有文字，在部分 Chrome 显示环境中造成整块内容发白。
    -->
    <el-skeleton v-else-if="loading" :rows="8" animated />

    <EmptyState
      v-else-if="error"
      compact
      title="就诊信息加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <DescriptionList
      v-else-if="context"
      :data="descriptionData"
      :items="descriptionItems"
      :column="1"
      title="就诊信息"
      label-width="88px"
      size="small"
      dense
    >
      <template #value-visitStatus>
        <StatusDisplay :value="context.visitStatus" :options="DOCTOR_VISIT_STATUS_OPTIONS" round />
      </template>
    </DescriptionList>
  </section>
</template>

<style scoped>
.patient-context-panel {
  box-sizing: border-box;
  width: 100%;
  min-height: 100%;
  padding: 12px;
  color: #303133;
  background: var(--el-bg-color);
}

/* 明确设置详情标题、标签和值的颜色，避免继承到过浅的宿主页面颜色。 */
.patient-context-panel :deep(.el-descriptions__title) {
  color: #303133;
  font-weight: 600;
}

.patient-context-panel :deep(.el-descriptions__label) {
  color: #606266 !important;
  font-weight: 500;
}

.patient-context-panel :deep(.el-descriptions__content),
.patient-context-panel :deep(.description-list__value) {
  color: #303133 !important;
}
</style>
