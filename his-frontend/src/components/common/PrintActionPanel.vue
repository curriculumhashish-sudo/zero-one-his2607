<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, shallowRef, watch } from 'vue'

defineOptions({ name: 'PrintActionPanel' })

export type PrintActionValue = string | number
export type PrintActionType = 'preview' | 'print'

export interface PrintTemplateOption {
  value: PrintActionValue
  label: string
  disabled?: boolean
  description?: string
}

export interface PrinterOption {
  value: string
  label: string
  disabled?: boolean
  status?: 'online' | 'offline' | 'unknown'
  description?: string
}

export interface PrintActionContext {
  templateValue: PrintActionValue
  printerValue: string | null
  signal: AbortSignal
}

export type PrintActionHandler = (context: PrintActionContext) => unknown | Promise<unknown>
export type PrinterLoader = (context: { signal: AbortSignal }) => PrinterOption[] | Promise<PrinterOption[]>

interface PrintActionPanelProps {
  templateValue?: PrintActionValue | null
  printerValue?: string | null
  templates?: PrintTemplateOption[]
  printers?: PrinterOption[]
  preview?: PrintActionHandler
  print?: PrintActionHandler
  loadPrinters?: PrinterLoader
  loading?: boolean
  disabled?: boolean
  requirePrinter?: boolean
  autoLoadPrinters?: boolean
  title?: string
  templateLabel?: string
  printerLabel?: string
  previewText?: string
  printText?: string
  emptyPrinterText?: string
}

const props = withDefaults(defineProps<PrintActionPanelProps>(), {
  templateValue: null,
  printerValue: null,
  templates: () => [],
  printers: () => [],
  preview: undefined,
  print: undefined,
  loadPrinters: undefined,
  loading: false,
  disabled: false,
  requirePrinter: true,
  autoLoadPrinters: true,
  title: '打印设置',
  templateLabel: '打印模板',
  printerLabel: '打印机',
  previewText: '打印预览',
  printText: '开始打印',
  emptyPrinterText: '暂无可用打印机'
})

const emit = defineEmits<{
  'update:templateValue': [value: PrintActionValue | null]
  'update:printerValue': [value: string | null]
  preview: [context: PrintActionContext]
  print: [context: PrintActionContext]
  success: [type: PrintActionType, result: unknown]
  error: [error: unknown, type: PrintActionType | 'loadPrinters']
  retry: [type: PrintActionType | 'loadPrinters']
  printersLoaded: [printers: PrinterOption[]]
}>()

const loadedPrinters = shallowRef<PrinterOption[]>(props.printers)
const executing = shallowRef<PrintActionType | 'loadPrinters' | null>(null)
const errorMessage = shallowRef('')
const lastFailedAction = shallowRef<PrintActionType | 'loadPrinters' | null>(null)

let controller: AbortController | undefined
let sequence = 0

watch(
  () => props.printers,
  (printers) => {
    if (executing.value !== 'loadPrinters') loadedPrinters.value = printers
  }
)

const busy = computed(() => props.loading || executing.value !== null)
const printerRequiredButMissing = computed(
  () => props.requirePrinter && !props.printerValue
)
const canPreview = computed(
  () => !props.disabled && !busy.value && props.templateValue != null && Boolean(props.preview)
)
const canPrint = computed(
  () =>
    !props.disabled &&
    !busy.value &&
    props.templateValue != null &&
    !printerRequiredButMissing.value &&
    Boolean(props.print)
)

function cancelCurrent() {
  controller?.abort()
  controller = undefined
}

function createContext(signal: AbortSignal): PrintActionContext | null {
  if (props.templateValue == null) return null
  return {
    templateValue: props.templateValue,
    printerValue: props.printerValue,
    signal
  }
}

function normalizeError(error: unknown, fallback: string) {
  return error instanceof Error && error.message ? error.message : fallback
}

async function runAction(type: PrintActionType) {
  const handler = type === 'preview' ? props.preview : props.print
  if (!handler || props.disabled || busy.value) return
  if (props.templateValue == null) {
    errorMessage.value = '请先选择打印模板'
    return
  }
  if (type === 'print' && printerRequiredButMissing.value) {
    errorMessage.value = '请先选择打印机'
    return
  }

  cancelCurrent()
  const currentController = new AbortController()
  controller = currentController
  const currentSequence = ++sequence
  const context = createContext(currentController.signal)
  if (!context) return

  executing.value = type
  errorMessage.value = ''
  lastFailedAction.value = null
  if (type === 'preview') emit('preview', context)
  else emit('print', context)

  try {
    const result = await handler(context)
    if (currentController.signal.aborted || currentSequence !== sequence) return
    emit('success', type, result)
  } catch (error) {
    if (currentController.signal.aborted || currentSequence !== sequence) return
    errorMessage.value = normalizeError(
      error,
      type === 'preview' ? '打印预览失败' : '打印失败'
    )
    lastFailedAction.value = type
    emit('error', error, type)
  } finally {
    if (currentSequence === sequence) {
      executing.value = null
      controller = undefined
    }
  }
}

async function reloadPrinters() {
  if (!props.loadPrinters || props.disabled || busy.value) return

  cancelCurrent()
  const currentController = new AbortController()
  controller = currentController
  const currentSequence = ++sequence
  executing.value = 'loadPrinters'
  errorMessage.value = ''
  lastFailedAction.value = null

  try {
    const printers = await props.loadPrinters({ signal: currentController.signal })
    if (currentController.signal.aborted || currentSequence !== sequence) return
    loadedPrinters.value = printers
    emit('printersLoaded', printers)

    if (
      props.printerValue &&
      !printers.some((printer) => printer.value === props.printerValue)
    ) {
      emit('update:printerValue', null)
    }
  } catch (error) {
    if (currentController.signal.aborted || currentSequence !== sequence) return
    errorMessage.value = normalizeError(error, '打印机加载失败')
    lastFailedAction.value = 'loadPrinters'
    emit('error', error, 'loadPrinters')
  } finally {
    if (currentSequence === sequence) {
      executing.value = null
      controller = undefined
    }
  }
}

function retry() {
  const action = lastFailedAction.value
  if (!action) return
  emit('retry', action)
  if (action === 'loadPrinters') void reloadPrinters()
  else void runAction(action)
}

function updateTemplate(value: PrintActionValue | null) {
  errorMessage.value = ''
  emit('update:templateValue', value)
}

function updatePrinter(value: string | null) {
  errorMessage.value = ''
  emit('update:printerValue', value)
}

onMounted(() => {
  if (props.autoLoadPrinters && props.loadPrinters) void reloadPrinters()
})

onBeforeUnmount(() => {
  cancelCurrent()
  sequence += 1
})

defineExpose({
  preview: () => runAction('preview'),
  print: () => runAction('print'),
  reloadPrinters,
  retry,
  abort: cancelCurrent
})
</script>

<template>
  <section class="print-action-panel" :aria-busy="busy">
    <header class="print-action-panel__header">
      <slot name="header" :busy="busy">
        <div>
          <h3>{{ title }}</h3>
          <p>模板、打印机和执行函数均由当前页面提供。</p>
        </div>
      </slot>
    </header>

    <div class="print-action-panel__fields">
      <slot name="fields">
        <el-form-item :label="templateLabel" required>
          <el-select
            :model-value="templateValue"
            :disabled="disabled || busy"
            placeholder="请选择打印模板"
            clearable
            @update:model-value="updateTemplate"
          >
            <el-option
              v-for="item in templates"
              :key="item.value"
              :label="item.label"
              :value="item.value"
              :disabled="item.disabled"
            >
              <span>{{ item.label }}</span>
              <small v-if="item.description" class="print-action-panel__option-description">
                {{ item.description }}
              </small>
            </el-option>
          </el-select>
        </el-form-item>

        <el-form-item :label="printerLabel" :required="requirePrinter">
          <div class="print-action-panel__printer-field">
            <el-select
              :model-value="printerValue"
              :disabled="disabled || busy"
              :placeholder="emptyPrinterText"
              clearable
              @update:model-value="updatePrinter"
            >
              <el-option
                v-for="item in loadedPrinters"
                :key="item.value"
                :label="item.label"
                :value="item.value"
                :disabled="item.disabled || item.status === 'offline'"
              >
                <span>{{ item.label }}</span>
                <el-tag
                  v-if="item.status"
                  size="small"
                  :type="item.status === 'online' ? 'success' : item.status === 'offline' ? 'danger' : 'info'"
                >
                  {{ item.status === 'online' ? '在线' : item.status === 'offline' ? '离线' : '未知' }}
                </el-tag>
              </el-option>
            </el-select>
            <el-button
              v-if="loadPrinters"
              :loading="executing === 'loadPrinters'"
              :disabled="disabled || busy"
              @click="reloadPrinters"
            >
              刷新
            </el-button>
          </div>
        </el-form-item>
      </slot>
    </div>

    <el-alert
      v-if="errorMessage"
      :title="errorMessage"
      type="error"
      show-icon
      :closable="false"
    >
      <template #default>
        <el-button type="primary" link :disabled="busy" @click="retry">重试</el-button>
      </template>
    </el-alert>

    <footer class="print-action-panel__actions">
      <slot
        name="actions"
        :busy="busy"
        :can-preview="canPreview"
        :can-print="canPrint"
        :preview="() => runAction('preview')"
        :print="() => runAction('print')"
      >
        <el-button
          :loading="executing === 'preview'"
          :disabled="!canPreview"
          @click="runAction('preview')"
        >
          {{ previewText }}
        </el-button>
        <el-button
          type="primary"
          :loading="executing === 'print'"
          :disabled="!canPrint"
          @click="runAction('print')"
        >
          {{ printText }}
        </el-button>
      </slot>
    </footer>
  </section>
</template>

<style scoped>
.print-action-panel {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 14px;
  padding: 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.print-action-panel__header h3,
.print-action-panel__header p {
  margin: 0;
}

.print-action-panel__header h3 {
  color: var(--el-text-color-primary);
  font-size: 16px;
}

.print-action-panel__header p {
  margin-top: 5px;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.print-action-panel__fields {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 12px 16px;
}

.print-action-panel__fields :deep(.el-form-item) {
  margin-bottom: 0;
}

.print-action-panel__fields :deep(.el-select) {
  width: 100%;
}

.print-action-panel__printer-field {
  display: flex;
  width: 100%;
  min-width: 0;
  gap: 8px;
}

.print-action-panel__option-description {
  margin-left: 8px;
  color: var(--el-text-color-secondary);
}

.print-action-panel__actions {
  display: flex;
  justify-content: flex-end;
  gap: 8px;
  padding-top: 12px;
  border-top: 1px dashed var(--el-border-color-light);
}

@media (max-width: 700px) {
  .print-action-panel__fields {
    grid-template-columns: 1fr;
  }
}
</style>
