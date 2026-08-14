<!-- src/views/pharmacy/outpatient-dispense/components/TraceCodeDialog.vue -->
<template>
  <el-dialog
    v-model="dialogVisible"
    title="批量扫描追溯码"
    width="920px"
    destroy-on-close
    :close-on-click-modal="false"
    @opened="handleDialogOpened"
    @close="handleCancel"
  >
    <div class="trace-code-dialog">
      <div class="trace-code-dialog__header">
        <span v-if="patientName">患者：{{ patientName }} · </span>
        追溯码为 <strong>{{ TRACE_CODE_LENGTH }}</strong> 位
        <span class="trace-code-dialog__badge">
          已绑定 {{ boundCount }} / {{ lines.length }}
        </span>
      </div>

      <!-- 扫码输入 -->
      <el-input
        ref="scanInputRef"
        v-model="scanInput"
        clearable
        class="trace-code-dialog__input"
        placeholder="扫描或粘贴追溯码（支持多条，每行一条），按回车提交"
        @keydown.enter.prevent="handleScanEnter"
      />

      <!-- 扫码流水 + 药品列表 -->
      <div class="trace-code-dialog__body">
        <!-- 左侧：流水 -->
        <div class="trace-code-dialog__log">
          <div class="trace-code-dialog__log-title">扫码流水</div>
          <div class="trace-code-dialog__log-list">
            <div v-if="scanLog.length === 0" class="trace-code-dialog__log-empty">
              暂无记录，请开始扫码
            </div>
            <ul v-else class="trace-code-dialog__log-items">
              <li
                v-for="item in scanLog"
                :key="item.id"
                class="trace-code-dialog__log-item"
              >
                <span class="trace-code-dialog__log-time">{{ item.timeLabel }}</span>
                <StatusDisplay
                  :status="item.status"
                  :status-map="logStatusMap"
                  :tag-type-map="logTagTypeMap"
                  size="small"
                />
                <span class="trace-code-dialog__log-code">{{ item.code }}</span>
                <span class="trace-code-dialog__log-message">{{ item.message }}</span>
              </li>
            </ul>
          </div>
        </div>

        <!-- 右侧：药品追溯码列表 -->
        <div class="trace-code-dialog__list">
          <div class="trace-code-dialog__list-title">药品与追溯码</div>
          <el-table :data="lines" border size="small" max-height="320" row-key="id">
            <el-table-column prop="rxNo" label="处方号" width="128" show-overflow-tooltip />
            <el-table-column prop="itemName" label="项目名称" min-width="120" show-overflow-tooltip />
            <el-table-column prop="spec" label="规格" width="100" show-overflow-tooltip />
            <el-table-column label="追溯码" min-width="140">
              <template #default="{ row }">
                <span
                  :class="
                    isTraceEmpty(effectiveTrace(row))
                      ? 'trace-code-dialog__trace-empty'
                      : 'trace-code-dialog__trace-filled'
                  "
                >
                  {{ isTraceEmpty(effectiveTrace(row)) ? '待扫' : effectiveTrace(row) }}
                </span>
              </template>
            </el-table-column>
            <el-table-column label="操作" width="72" align="center">
              <template #default="{ row }">
                <el-button
                  link
                  type="primary"
                  :disabled="isTraceEmpty(effectiveTrace(row))"
                  @click="clearLineTrace(row)"
                >
                  清除
                </el-button>
              </template>
            </el-table-column>
          </el-table>
        </div>
      </div>
    </div>

    <template #footer>
      <el-button @click="handleCancel">取消</el-button>
      <el-button type="primary" @click="handleConfirm">确定</el-button>
    </template>
  </el-dialog>
</template>

<script setup lang="ts">
import { ref, watch, nextTick, computed } from 'vue'
import {
  ElDialog,
  ElInput,
  ElTable,
  ElTableColumn,
  ElButton,
  ElMessage,
} from 'element-plus'
import StatusDisplay from '@/components/common/StatusDisplay.vue'

import type { DispensePrescriptionItemRow } from '../type'

// ============ Props ============
const props = defineProps<{
  visible: boolean
  lines: DispensePrescriptionItemRow[]
  patientName?: string
}>()

// ============ Emits ============
const emit = defineEmits<{
  'update:visible': [value: boolean]
  confirm: [traceCodes: Record<string, string>]
}>()

// ============ 常量 ============
const TRACE_CODE_LENGTH = 20

// ============ 状态 ============
const dialogVisible = ref(false)
const scanInput = ref('')
const scanInputRef = ref<InstanceType<typeof ElInput> | null>(null)

/** 打开时的快照，取消时还原 */
const snapshotTrace = ref<Record<string, string>>({})

/** 当前追溯码 */
const traceCodes = ref<Record<string, string>>({})

/** 扫码日志 */
interface ScanLogItem {
  id: string
  code: string
  status: 'ok' | 'duplicate' | 'invalid' | 'not_found'
  message: string
  timeLabel: string
}
const scanLog = ref<ScanLogItem[]>([])

// ============ 计算属性 ============
/** 已绑定数量 */
const boundCount = computed(() => {
  return props.lines.filter((row) => !isTraceEmpty(effectiveTrace(row))).length
})

/** 日志状态映射 */
const logStatusMap = {
  ok: '成功',
  duplicate: '重复',
  invalid: '格式错误',
  not_found: '未找到',
}

const logTagTypeMap = {
  ok: 'success',
  duplicate: 'warning',
  invalid: 'danger',
  not_found: 'info',
}

// ============ 方法 ============
/** 判断追溯码是否为空 */
function isTraceEmpty(value: string | undefined): boolean {
  const t = (value || '').trim()
  return t === '' || t === '—'
}

/** 获取有效追溯码 */
function effectiveTrace(row: unknown): string {
  const r = row as { id: string | number; traceCode?: string }
  const id = String(r.id)
  const v = traceCodes.value[id]
  if (v && v.trim() !== '') return v.trim()
  return String(r.traceCode || '').trim()
}

/** 推入日志 */
function pushLog(code: string, status: ScanLogItem['status'], message: string) {
  const timeLabel = new Date().toLocaleTimeString('zh-CN', { hour12: false })
  scanLog.value = [
    {
      id: `${Date.now()}-${Math.random().toString(16).slice(2, 8)}`,
      code,
      status,
      message,
      timeLabel,
    },
    ...scanLog.value,
  ].slice(0, 80)
}

/** 模拟解析追溯码 */
function resolveTraceCodeMock(code: string): { lineId: string; drugId: string; traceCode: string } | null {
  // 模拟解析：取后4位作为匹配标识
  const suffix = code.slice(-4)
  // 尝试匹配药品
  for (const row of props.lines) {
    if (row.id.endsWith(suffix) || row.itemName.includes(suffix)) {
      return {
        lineId: row.id,
        drugId: row.id,
        traceCode: code,
      }
    }
  }
  return null
}

/** 绑定单条追溯码 */
function tryBindSingleCode(code: string): 'bound' | 'duplicate' | 'invalid' | 'not_found' {
  const trimmed = code.trim()
  if (trimmed.length !== TRACE_CODE_LENGTH) {
    pushLog(trimmed || '(空)', 'invalid', `须为 ${TRACE_CODE_LENGTH} 位`)
    return 'invalid'
  }

  const resolved = resolveTraceCodeMock(trimmed)
  if (!resolved) {
    pushLog(trimmed, 'not_found', '库中未找到该追溯码')
    ElMessage.warning('未找到该追溯码')
    return 'not_found'
  }

  const { lineId, traceCode } = resolved
  const row = props.lines.find((r) => r.id === lineId)
  if (!row) {
    pushLog(trimmed, 'not_found', '解析结果与当前列表不匹配')
    return 'not_found'
  }

  // 检查是否重复
  const boundElsewhere = props.lines.some(
    (r) => r.id !== lineId && !isTraceEmpty(effectiveTrace(r)) && effectiveTrace(r) === traceCode,
  )
  if (boundElsewhere) {
    pushLog(traceCode, 'duplicate', '该追溯码已绑定在其他药品行')
    ElMessage.warning('追溯码重复')
    return 'duplicate'
  }

  // 绑定
  traceCodes.value = { ...traceCodes.value, [lineId]: traceCode }
  pushLog(traceCode, 'ok', `已绑定：${row.itemName}`)
  return 'bound'
}

/** 处理扫码输入 */
function handleScanEnter() {
  const raw = scanInput.value
  scanInput.value = ''

  if (!raw.trim()) {
    if (boundCount.value === props.lines.length && props.lines.length > 0) {
      handleConfirm()
    }
    focusInput()
    return
  }

  const codes = raw
    .replaceAll('\r\n', '\n')
    .replaceAll('\r', '\n')
    .split('\n')
    .map((s) => s.trim())
    .filter(Boolean)

  for (const code of codes) {
    tryBindSingleCode(code)
  }

  nextTick(() => {
    if (boundCount.value === props.lines.length && props.lines.length > 0) {
      ElMessage.success('追溯码已全部绑定')
      handleConfirm()
      return
    }
    focusInput()
  })
}

/** 聚焦输入框 */
function focusInput() {
  nextTick(() => {
    scanInputRef.value?.focus?.()
  })
}

/** 清除某行追溯码 */
function clearLineTrace(row: unknown) {
  const r = row as { id: string | number; itemName?: string }
  const id = String(r.id)
  // eslint-disable-next-line @typescript-eslint/no-unused-vars
  const { [id]: _removed, ...rest } = traceCodes.value
  traceCodes.value = rest
  pushLog('—', 'ok', `已清除：${r.itemName || ''}`)
}

/** 取消 */
function handleCancel() {
  traceCodes.value = { ...snapshotTrace.value }
  dialogVisible.value = false
  emit('update:visible', false)
}

/** 确定 */
function handleConfirm() {
  emit('confirm', { ...traceCodes.value })
  dialogVisible.value = false
  emit('update:visible', false)
}

/** 弹窗打开 */
function handleDialogOpened() {
  // 初始化
  traceCodes.value = {}
  for (const row of props.lines) {
    if (row.traceCode) {
      traceCodes.value[row.id] = row.traceCode
    }
  }
  snapshotTrace.value = { ...traceCodes.value }
  scanLog.value = []
  scanInput.value = ''
  focusInput()
}

// ============ 监听 ============
watch(
  () => props.visible,
  (val) => {
    dialogVisible.value = val
  },
)
</script>

<style scoped>
.trace-code-dialog {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.trace-code-dialog__header {
  font-size: 13px;
  color: var(--el-text-color-secondary);
}

.trace-code-dialog__badge {
  margin-left: 8px;
  font-weight: 600;
  color: var(--el-text-color-primary);
}

.trace-code-dialog__input {
  font-family: monospace;
}

.trace-code-dialog__input :deep(.el-input__inner) {
  font-family: monospace;
}

.trace-code-dialog__body {
  display: flex;
  gap: 12px;
  min-height: 280px;
}

.trace-code-dialog__log {
  flex: 1;
  display: flex;
  flex-direction: column;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 4px;
  padding: 8px 12px;
  background: var(--el-fill-color-blank);
  min-width: 0;
}

.trace-code-dialog__log-title {
  font-size: 12px;
  font-weight: 500;
  color: var(--el-text-color-secondary);
  margin-bottom: 4px;
}

.trace-code-dialog__log-list {
  flex: 1;
  overflow: auto;
  min-height: 0;
}

.trace-code-dialog__log-empty {
  padding: 20px 0;
  text-align: center;
  color: var(--el-text-color-placeholder);
  font-size: 13px;
}

.trace-code-dialog__log-items {
  list-style: none;
  margin: 0;
  padding: 0;
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.trace-code-dialog__log-item {
  display: flex;
  flex-wrap: wrap;
  gap: 4px 8px;
  font-size: 12px;
  align-items: center;
  padding: 2px 4px;
  border-radius: 4px;
}

.trace-code-dialog__log-item:hover {
  background: var(--el-fill-color-light);
}

.trace-code-dialog__log-time {
  color: var(--el-text-color-placeholder);
  font-size: 11px;
  flex-shrink: 0;
}

.trace-code-dialog__log-code {
  font-family: monospace;
  color: var(--el-text-color-primary);
}

.trace-code-dialog__log-message {
  color: var(--el-text-color-regular);
}

.trace-code-dialog__list {
  flex: 1.2;
  display: flex;
  flex-direction: column;
  min-width: 0;
}

.trace-code-dialog__list-title {
  font-size: 12px;
  font-weight: 500;
  color: var(--el-text-color-secondary);
  margin-bottom: 4px;
}

.trace-code-dialog__trace-empty {
  color: var(--el-text-color-placeholder);
}

.trace-code-dialog__trace-filled {
  font-family: monospace;
  color: var(--el-text-color-primary);
}
</style>
