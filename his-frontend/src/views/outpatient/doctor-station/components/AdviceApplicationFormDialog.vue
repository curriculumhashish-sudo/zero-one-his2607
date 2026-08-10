<script setup lang="ts">
import { nextTick, ref } from 'vue'

import {
  printAdviceApplicationForm,
  type DoctorAdviceApplicationFormVO
} from '@/apis/outpatient/doctor-station'
import EmptyState from '@/components/common/EmptyState.vue'
import AdviceApplicationFormView from '@/components/medical/AdviceApplicationFormView.vue'

defineOptions({ name: 'AdviceApplicationFormDialog' })

const props = withDefaults(
  defineProps<{
    modelValue?: boolean
    serviceRequestIds?: string[]
  }>(),
  {
    modelValue: false,
    serviceRequestIds: () => []
  }
)

const emit = defineEmits<{
  'update:modelValue': [visible: boolean]
}>()

const forms = ref<DoctorAdviceApplicationFormVO[]>([])
const loading = ref(false)
const error = ref('')
let requestSequence = 0

/** 每个服务申请 ID 对应一张申请单，多选时按选择顺序连续展示。 */
async function loadForms() {
  const ids = [...new Set(props.serviceRequestIds.map((item) => item.trim()).filter(Boolean))]
  const currentRequest = ++requestSequence
  forms.value = []
  error.value = ''

  if (ids.length === 0) {
    error.value = '没有可打印的服务申请'
    return
  }

  loading.value = true
  try {
    const result = await Promise.all(
      ids.map((serviceRequestId) => printAdviceApplicationForm({ serviceRequestId }))
    )
    if (currentRequest !== requestSequence) return
    forms.value = result
  } catch (loadError) {
    if (currentRequest !== requestSequence) return
    error.value = loadError instanceof Error ? loadError.message : '申请单加载失败'
  } finally {
    if (currentRequest === requestSequence) loading.value = false
  }
}

async function handlePrint() {
  if (forms.value.length === 0) return
  // 等待按钮状态和 DOM 更新完成后再调用浏览器打印，避免少打印最后一张。
  await nextTick()
  window.print()
}

function handleClosed() {
  requestSequence += 1
  forms.value = []
  loading.value = false
  error.value = ''
}
</script>

<template>
  <el-dialog
    :model-value="modelValue"
    title="打印申请单"
    width="min(920px, calc(100vw - 32px))"
    append-to-body
    destroy-on-close
    :close-on-click-modal="false"
    class="advice-application-form-dialog"
    @open="loadForms"
    @closed="handleClosed"
    @update:model-value="emit('update:modelValue', $event)"
  >
    <div class="advice-application-form-dialog__content">
      <div v-if="loading" class="advice-application-form-dialog__skeleton" aria-busy="true">
        <el-skeleton :rows="14" animated />
      </div>

      <EmptyState
        v-else-if="error"
        title="申请单加载失败"
        :description="error"
        action-text="重试"
        @action="loadForms"
      />

      <template v-else-if="forms.length > 0">
        <div
          v-for="form in forms"
          :key="form.serviceRequestId"
          class="advice-application-form-dialog__sheet"
        >
          <AdviceApplicationFormView :data="form" />
        </div>
      </template>

      <EmptyState v-else title="暂无申请单" description="请选择已签发的检验、检查或治疗医嘱" />
    </div>

    <template #footer>
      <el-button @click="emit('update:modelValue', false)">关闭</el-button>
      <el-button type="primary" :disabled="loading || forms.length === 0" @click="handlePrint">
        打印
      </el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.advice-application-form-dialog__content {
  display: flex;
  max-height: min(720px, 76vh);
  min-height: 480px;
  overflow: auto;
  flex-direction: column;
  gap: 24px;
  padding: 12px;
  background: var(--el-fill-color-light);
  color: var(--el-text-color-primary);
}

.advice-application-form-dialog__skeleton {
  width: min(100%, 760px);
  margin: 0 auto;
  padding: 24px;
  background: var(--el-bg-color);
}

@media print {
  :global(body *) {
    visibility: hidden !important;
  }

  :global(.el-overlay:has(.advice-application-form-dialog)),
  :global(.advice-application-form-dialog),
  :global(.advice-application-form-dialog *) {
    visibility: visible !important;
  }

  :global(.el-overlay:has(.advice-application-form-dialog)) {
    position: absolute !important;
    inset: 0 !important;
    overflow: visible !important;
    background: #fff !important;
  }

  :global(.advice-application-form-dialog) {
    width: 100% !important;
    max-width: none !important;
    margin: 0 !important;
    padding: 0 !important;
    box-shadow: none !important;
  }

  :global(.advice-application-form-dialog .el-dialog__header),
  :global(.advice-application-form-dialog .el-dialog__footer) {
    display: none !important;
  }

  .advice-application-form-dialog__content {
    max-height: none;
    min-height: 0;
    overflow: visible;
    gap: 0;
    padding: 0;
    background: #fff;
  }

  .advice-application-form-dialog__sheet {
    break-after: page;
    page-break-after: always;
  }

  .advice-application-form-dialog__sheet:last-child {
    break-after: auto;
    page-break-after: auto;
  }
}
</style>
