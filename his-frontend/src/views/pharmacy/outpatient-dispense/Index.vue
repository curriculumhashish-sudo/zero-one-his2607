<!-- src/views/pharmacy/outpatient-dispense/Index.vue -->
<template>
  <div class="outpatient-dispense-page">
    <div class="outpatient-dispense-page__layout">
      <!-- ===== 左侧：待发药患者面板 ===== -->
      <aside class="left-panel">
        <!-- 医保契约 -->
        <div class="left-panel__section">
          <span class="left-panel__label">医保契约</span>
          <el-select
            v-model="insuranceContract"
            placeholder="请选择"
            style="width: 100%"
            size="default"
          >
            <el-option label="市医保（H220104024...）" value="H220104024" />
          </el-select>
        </div>

        <div class="left-panel__divider" />

        <!-- 待发药/已发药单选 -->
        <div class="left-panel__section">
          <div class="left-panel__radio-group">
            <button
              class="left-panel__radio-btn"
              :class="{ 'left-panel__radio-btn--active': queueScope === 'pending' }"
              @click="queueScope = 'pending'"
            >
              <span
                class="left-panel__radio-dot"
                :class="{ 'left-panel__radio-dot--checked': queueScope === 'pending' }"
              />
              待发药
            </button>
            <button
              class="left-panel__radio-btn"
              :class="{ 'left-panel__radio-btn--active': queueScope === 'completed' }"
              @click="queueScope = 'completed'"
            >
              <span
                class="left-panel__radio-dot"
                :class="{ 'left-panel__radio-dot--checked': queueScope === 'completed' }"
              />
              已发药
            </button>
          </div>
        </div>

        <div class="left-panel__divider" />

        <!-- 日期选择 -->
        <div class="left-panel__section">
          <span class="left-panel__label">日期</span>
          <el-date-picker
            v-model="dateRange"
            type="daterange"
            range-separator="~"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
            value-format="YYYY-MM-DD"
            style="width: 100%"
            size="default"
          />
        </div>

        <div class="left-panel__divider" />

        <!-- 搜索行 -->
        <div class="left-panel__section">
          <div class="left-panel__search-row">
            <el-input
              v-model="keyword"
              placeholder="姓名/排队号"
              clearable
              size="default"
              @keyup.enter="handleSearch"
            >
              <template #prefix>
                <el-icon><Search /></el-icon>
              </template>
            </el-input>
            <el-button
              class="left-panel__refresh-btn"
              :icon="Refresh"
              circle
              @click="loadPatients"
            />
          </div>
        </div>

        <!-- 患者列表 -->
        <div class="left-panel__list">
          <!-- 空状态 -->
          <div v-if="patients.length === 0 && !queueLoading" class="left-panel__empty">
            <div class="left-panel__empty-icon">
              <el-icon :size="48"><UserFilled /></el-icon>
            </div>
            <span class="left-panel__empty-text">暂无匹配患者</span>
          </div>

          <!-- 加载中 -->
          <div v-else-if="queueLoading" class="left-panel__loading">
            <el-icon class="left-panel__loading-icon" :size="24"><Loading /></el-icon>
            <span>加载中...</span>
          </div>

          <!-- 患者列表 -->
          <el-scrollbar v-else class="left-panel__scroll">
            <div
              v-for="patient in patients"
              :key="patient.id"
              class="left-panel__item"
              :class="{ 'left-panel__item--selected': patient.id === selectedPatientId }"
              @click="handleSelectPatient(patient)"
            >
              <div class="left-panel__item-header">
                <span class="left-panel__item-name">{{ patient.patientName }}</span>
                <span
                  v-if="patient.status === 'completed'"
                  class="left-panel__item-badge left-panel__item-badge--done"
                  >已发</span
                >
                <span v-else class="left-panel__item-badge left-panel__item-badge--pending"
                  >待发</span
                >
              </div>
              <div class="left-panel__item-info">
                <span>{{ patient.visitNo }}</span>
                <span class="left-panel__item-sep">|</span>
                <span>{{ patient.genderText }} {{ patient.ageText }}</span>
              </div>
            </div>
          </el-scrollbar>
        </div>
      </aside>

      <!-- ===== 右侧：处方列表面板 ===== -->
      <section class="right-panel">
        <!-- 面板标题 -->
        <div class="right-panel__header">
          <span class="right-panel__header-bar" />
          <span class="right-panel__header-title">处方列表</span>
        </div>

        <!-- 操作按钮行 -->
        <div class="right-panel__toolbar">
          <div class="right-panel__toolbar-left">
            <el-button class="toolbar-btn toolbar-btn--light" @click="handleBatchDispense">
              批量发药
            </el-button>
            <el-button class="toolbar-btn" @click="openTraceDialog"> 扫追溯码 </el-button>
            <el-button class="toolbar-btn" @click="handleViewPrescription"> 处方查看 </el-button>
            <el-button class="toolbar-btn" @click="handlePrintClick"> 处方打印 </el-button>

            <span class="right-panel__toolbar-sep" />

            <el-button
              class="toolbar-btn toolbar-btn--prescription-type"
              :class="{ 'toolbar-btn--prescription-type-active': prescriptionType === 'western' }"
              @click="prescriptionType = 'western'"
            >
              西药/成药
            </el-button>
            <el-button
              class="toolbar-btn toolbar-btn--prescription-type"
              :class="{ 'toolbar-btn--prescription-type-active': prescriptionType === 'chinese' }"
              @click="prescriptionType = 'chinese'"
            >
              中药饮片
            </el-button>

            <span class="right-panel__toolbar-sep" />

            <el-select
              v-model="selectedDispenserId"
              placeholder="调配药师"
              style="width: 140px"
              size="default"
            >
              <el-option
                v-for="option in dispenserOptions"
                :key="option.value"
                :label="option.label"
                :value="option.value"
              />
            </el-select>

            <el-select
              v-model="selectedStorehouseId"
              placeholder="库管"
              style="width: 120px"
              size="default"
            >
              <el-option label="主库" value="main" />
              <el-option label="分库" value="branch" />
            </el-select>
          </div>
        </div>

        <!-- 处方内容区域 -->
        <div class="right-panel__content">
          <!-- 未选择患者：空状态 -->
          <div v-if="!selectedPatientId" class="right-panel__empty">
            <div class="right-panel__empty-icon">
              <el-icon :size="64"><Document /></el-icon>
            </div>
            <span class="right-panel__empty-text">请从左侧选择患者</span>
          </div>

          <!-- 已选择患者：处方表格 -->
          <DispenseCenterPanel
            v-else
            ref="centerPanelRef"
            :prescription-blocks="prescriptionBlocks"
            :loading="detailLoading"
            :queue-scope="queueScope"
          />
        </div>
      </section>
    </div>

    <!-- 追溯码弹窗 -->
    <TraceCodeDialog
      v-model:visible="traceDialogVisible"
      :lines="allPrescriptionItems"
      :patient-name="currentPatientInfo?.patientName || ''"
      @confirm="handleTraceConfirm"
    />
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import { Refresh, Search, UserFilled, Loading, Document } from '@element-plus/icons-vue'
import { useDebounceFn } from '@vueuse/core'

import DispenseCenterPanel from './components/DispenseCenterPanel.vue'
import TraceCodeDialog from './components/TraceCodeDialog.vue'

import {
  loadDispensePageInit,
  pageDispensePatientQueue,
  listPatientPrescriptions,
  saveDispensePreparation,
  confirmMedicationDispense,
  printPrescription
} from '@/apis/pharmacy/dispense/outpatient'

import { toDispensePatient, toPrescriptionBlocks, decodeInventoryPick } from './adapter'

import type {
  DispenseQueuePatient,
  DispensePrescriptionBlock,
  DispensePrescriptionItemRow
} from './type'
import type { PrintActionContext } from '@/components/common/PrintActionPanel.vue'

// ============ 状态 ============
const patients = ref<DispenseQueuePatient[]>([])
const selectedPatientId = ref<string | number | null>(null)
const queueLoading = ref(false)
const detailLoading = ref(false)
const queueScope = ref<'pending' | 'completed'>('pending')
const prescriptionType = ref<'western' | 'chinese'>('western')
const keyword = ref('')
const dateRange = ref<[string, string] | null>(null)
const insuranceContract = ref('H220104024')
const selectedStorehouseId = ref('main')

const prescriptionBlocks = ref<DispensePrescriptionBlock[]>([])
const dispenserOptions = ref<{ label: string; value: string }[]>([])
const selectedDispenserId = ref<string | null>(null)

const traceDialogVisible = ref(false)
const traceCodes = ref<Record<string, string>>({})

const dispenseLoading = ref(false)

// ============ 引用 ============
const centerPanelRef = ref<InstanceType<typeof DispenseCenterPanel> | null>(null)

// ============ 计算属性 ============
const currentPatient = computed(() => {
  const id = selectedPatientId.value
  if (!id) return null
  return patients.value.find((p) => p.id === id) ?? null
})

const currentPatientInfo = computed(() => {
  const p = currentPatient.value
  if (!p) return null
  return {
    id: p.id,
    patientId: p.patientId,
    patientName: p.patientName,
    genderText: p.genderText,
    ageText: p.ageText,
    visitNo: p.visitNo,
    name: p.name,
    gender: p.gender,
    age: p.age
  }
})

const allPrescriptionItems = computed(() => {
  const items: DispensePrescriptionItemRow[] = []
  for (const block of prescriptionBlocks.value) {
    items.push(...block.items)
  }
  return items
})

// ============ 方法 ============

/** 加载患者队列 */
async function loadPatients() {
  queueLoading.value = true
  try {
    const statusEnum: ('PREPARATION' | 'IN_PROGRESS' | 'COMPLETED')[] =
      queueScope.value === 'pending' ? ['PREPARATION', 'IN_PROGRESS'] : ['COMPLETED']
    const [startDay, endDay] = dateRange.value || [
      new Date().toISOString().slice(0, 10),
      new Date().toISOString().slice(0, 10)
    ]
    const result = await pageDispensePatientQueue({
      startTime: `${startDay} 00:00:00`,
      endTime: `${endDay} 23:59:59`,
      keyword: keyword.value || undefined,
      dispenseStatusEnumIn: statusEnum,
      pageNo: 1,
      pageSize: 50
    })
    patients.value = (result.rows || []).map(toDispensePatient)

    if (selectedPatientId.value && !patients.value.some((p) => p.id === selectedPatientId.value)) {
      selectedPatientId.value = null
      prescriptionBlocks.value = []
    }
  } catch {
    ElMessage.error('加载患者队列失败')
  } finally {
    queueLoading.value = false
  }
}

const debouncedLoadPatients = useDebounceFn(loadPatients, 400)

/** 加载处方明细 */
async function loadPrescriptions(encounterId: string | number) {
  const id = String(encounterId)
  detailLoading.value = true
  try {
    const rows = await listPatientPrescriptions(id)
    prescriptionBlocks.value = toPrescriptionBlocks(rows)
  } catch {
    ElMessage.error('加载处方失败')
  } finally {
    detailLoading.value = false
  }
}

/** 选择患者 */
function handleSelectPatient(patient: DispenseQueuePatient) {
  selectedPatientId.value = patient.id
  prescriptionBlocks.value = []
  traceCodes.value = {}
  centerPanelRef.value?.clearSelection()
  loadPrescriptions(patient.id)
}

/** 搜索 */
function handleSearch() {
  debouncedLoadPatients()
}

/** 批量发药 */
function handleBatchDispense() {
  const selectedIds = centerPanelRef.value?.getSelectedIds() || []
  if (selectedIds.length === 0) {
    ElMessage.warning('请先勾选需要发药的药品')
    return
  }
  handleDispense({ dispenseIds: selectedIds })
}

/** 打开追溯码弹窗 */
function openTraceDialog() {
  const items = allPrescriptionItems.value.filter((item) => item.checked)
  if (items.length === 0) {
    ElMessage.warning('请先勾选需要发药的药品')
    return
  }
  traceDialogVisible.value = true
}

/** 追溯码确认 */
function handleTraceConfirm(codes: Record<string, string>) {
  traceCodes.value = { ...traceCodes.value, ...codes }
  ElMessage.success('追溯码录入完成')
}

/** 查看处方 */
function handleViewPrescription() {}

/** 执行发药 */
async function handleDispense(params: { dispenseIds: string[] }) {
  if (dispenseLoading.value) return

  const enc = selectedPatientId.value
  if (!enc) {
    ElMessage.warning('请先选择患者')
    return
  }
  const encounterId = String(enc)

  if (!selectedDispenserId.value) {
    ElMessage.warning('请选择调配药师')
    return
  }

  if (params.dispenseIds.length === 0) {
    ElMessage.warning('请先勾选需要发药的药品')
    return
  }

  const items = allPrescriptionItems.value.filter((item) => params.dispenseIds.includes(item.id))

  const missingTrace = items.some((item) => {
    const hasTrace = traceCodes.value[item.id] || item.traceCode
    return !hasTrace
  })

  if (missingTrace) {
    try {
      await ElMessageBox.confirm('部分药品未录入追溯码，是否继续发药？', '提示', {
        type: 'warning'
      })
    } catch {
      return
    }
  }

  try {
    await ElMessageBox.confirm('确认对所选处方执行发药？', '发药确认', {
      type: 'warning'
    })
  } catch {
    return
  }

  const prepLines: {
    dispenseId: string
    dispenseLocationId: string
    lotNumber?: string | null
    quantity: string
    traceNo?: string | null
  }[] = []

  for (const item of items) {
    const pickValue = item.selectedInventoryPick || ''
    let dispenseLocationId = ''
    let lotNumber: string | undefined

    if (pickValue) {
      try {
        const decoded = decodeInventoryPick(pickValue)
        dispenseLocationId = decoded.pharmacyOrganizationId
        lotNumber = decoded.lotNumber
      } catch {}
    }

    if (!dispenseLocationId) {
      ElMessage.error(`请为「${item.itemName}」选择发放库房`)
      return
    }

    prepLines.push({
      dispenseId: item.id,
      dispenseLocationId,
      lotNumber,
      quantity: String(item.quantity),
      traceNo: traceCodes.value[item.id] || item.traceCode || undefined
    })
  }

  dispenseLoading.value = true
  try {
    await saveDispensePreparation({
      encounterId,
      preparerPractitionerId: selectedDispenserId.value,
      lines: prepLines
    })

    const result = await confirmMedicationDispense({
      dispenseIds: params.dispenseIds,
      practitionerId: selectedDispenserId.value || undefined
    })

    if (result.failCount > 0) {
      ElMessage.warning(result.failMessage || `发药完成：成功 ${result.successCount} 条，失败 ${result.failCount} 条`)
    } else {
      ElMessage.success('发药成功')
    }

    traceCodes.value = {}
    await loadPrescriptions(enc)
    await loadPatients()

    centerPanelRef.value?.clearSelection()
  } catch {
    ElMessage.error('发药失败')
  } finally {
    dispenseLoading.value = false
  }
}

/** 打印点击 */
function handlePrintClick() {}

/** 打印 */
// eslint-disable-next-line @typescript-eslint/no-unused-vars
async function handlePrint(context: PrintActionContext) {
  return printPrescription(context)
}

// ============ 生命周期 ============
onMounted(async () => {
  console.log('[D组门诊发药] 页面已加载，当前版本：两栏布局')
  console.log('[D组门诊发药] 当前文件路径:', import.meta.url)
  try {
    const initData = await loadDispensePageInit()
    dispenserOptions.value = initData.dispensers.map((d) => ({
      label: d.displayName,
      value: String(d.practitionerId)
    }))
    if (dispenserOptions.value.length > 0) {
      selectedDispenserId.value = dispenserOptions.value[0]?.value ?? null
    }
  } catch {}

  const today = new Date().toISOString().slice(0, 10)
  dateRange.value = [today, today]
  await loadPatients()
})

// ============ 监听 ============
watch(selectedPatientId, (newVal, oldVal) => {
  if (newVal !== oldVal) {
    traceCodes.value = {}
  }
})

watch(queueScope, () => {
  selectedPatientId.value = null
  prescriptionBlocks.value = []
  traceCodes.value = {}
  centerPanelRef.value?.clearSelection()
  loadPatients()
})

watch(dateRange, () => {
  loadPatients()
})
</script>

<style scoped>
/* ================================================================
   CSS 变量 — "三蓝" 色系
   ================================================================ */
.outpatient-dispense-page {
  --color-primary: #1677ff;
  --color-primary-light: #e6f7ff;
  --color-primary-border: #91d5ff;
  --color-primary-dark: #0958d9;
  --color-text-primary: #333333;
  --color-text-secondary: #888888;
  --color-text-placeholder: #bfbfbf;
  --color-border: #e5e7eb;
  --color-border-light: #f0f0f0;
  --color-bg: #ffffff;
  --color-bg-hover: #f5f7fa;
  --color-bg-page: #fafafa;

  height: 100%;
  background: var(--color-bg-page);
  font-family:
    -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
}

/* ================================================================
   左右两栏布局
   ================================================================ */
.outpatient-dispense-page__layout {
  display: flex;
  height: 100%;
  min-height: 0;
}

/* ================================================================
   左侧面板 — 待发药患者
   ================================================================ */
.left-panel {
  flex: 0 0 320px;
  width: 320px;
  height: 100%;
  display: flex;
  flex-direction: column;
  background: var(--color-bg);
  border-right: 1px solid var(--color-border);
}

.left-panel__section {
  padding: 12px 16px;
}

.left-panel__label {
  display: block;
  font-size: 13px;
  color: var(--color-text-secondary);
  margin-bottom: 6px;
  font-weight: 500;
}

.left-panel__divider {
  height: 1px;
  background: var(--color-border-light);
  margin: 0;
}

/* 单选按钮组 */
.left-panel__radio-group {
  display: flex;
  gap: 0;
  border: 1px solid var(--color-border);
  border-radius: 6px;
  overflow: hidden;
}

.left-panel__radio-btn {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 6px;
  padding: 8px 0;
  border: none;
  background: var(--color-bg);
  color: var(--color-text-secondary);
  font-size: 13px;
  cursor: pointer;
  transition: all 0.2s ease;
  font-family: inherit;
}

.left-panel__radio-btn:first-child {
  border-right: 1px solid var(--color-border);
}

.left-panel__radio-btn:hover {
  background: var(--color-bg-hover);
}

.left-panel__radio-btn--active {
  background: var(--color-primary-light);
  color: var(--color-primary);
  font-weight: 500;
}

.left-panel__radio-dot {
  display: inline-block;
  width: 14px;
  height: 14px;
  border-radius: 50%;
  border: 2px solid var(--color-border);
  background: var(--color-bg);
  transition: all 0.2s ease;
  flex-shrink: 0;
}

.left-panel__radio-dot--checked {
  border-color: var(--color-primary);
  background: var(--color-primary);
  box-shadow: inset 0 0 0 2px var(--color-bg);
}

/* 搜索行 */
.left-panel__search-row {
  display: flex;
  align-items: center;
  gap: 8px;
}

.left-panel__search-row :deep(.el-input) {
  flex: 1;
}

.left-panel__refresh-btn {
  width: 32px;
  height: 32px;
  padding: 0;
  border: 1px solid var(--color-border);
  background: var(--color-bg);
  color: var(--color-text-secondary);
  flex-shrink: 0;
}

.left-panel__refresh-btn:hover {
  color: var(--color-primary);
  border-color: var(--color-primary);
}

/* 患者列表 */
.left-panel__list {
  flex: 1;
  min-height: 0;
  position: relative;
}

.left-panel__empty,
.left-panel__loading {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%;
  min-height: 200px;
  color: var(--color-text-placeholder);
  gap: 12px;
}

.left-panel__empty-icon {
  color: var(--color-border);
}

.left-panel__empty-text {
  font-size: 14px;
  color: var(--color-text-placeholder);
}

.left-panel__loading-icon {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}

.left-panel__scroll {
  height: 100%;
}

/* 患者列表项 */
.left-panel__item {
  padding: 12px 16px;
  cursor: pointer;
  transition: all 0.15s ease;
  border-left: 3px solid transparent;
  border-bottom: 1px solid var(--color-border-light);
}

.left-panel__item:hover {
  background: var(--color-bg-hover);
}

.left-panel__item--selected {
  background: var(--color-primary-light);
  border-left-color: var(--color-primary);
}

.left-panel__item-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 4px;
}

.left-panel__item-name {
  font-size: 14px;
  font-weight: 500;
  color: var(--color-text-primary);
}

.left-panel__item-badge {
  font-size: 11px;
  padding: 1px 8px;
  border-radius: 10px;
  font-weight: 400;
  line-height: 18px;
}

.left-panel__item-badge--pending {
  background: #fff7e6;
  color: #d48806;
  border: 1px solid #ffd591;
}

.left-panel__item-badge--done {
  background: #f6ffed;
  color: #389e0d;
  border: 1px solid #b7eb8f;
}

.left-panel__item-info {
  font-size: 12px;
  color: var(--color-text-secondary);
  display: flex;
  align-items: center;
  gap: 6px;
}

.left-panel__item-sep {
  color: var(--color-border);
}

/* ================================================================
   右侧面板 — 处方列表
   ================================================================ */
.right-panel {
  flex: 1;
  display: flex;
  flex-direction: column;
  height: 100%;
  min-width: 0;
  background: var(--color-bg);
}

/* 面板标题 */
.right-panel__header {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 14px 20px;
  border-bottom: 1px solid var(--color-border);
  flex-shrink: 0;
}

.right-panel__header-bar {
  display: inline-block;
  width: 4px;
  height: 18px;
  border-radius: 2px;
  background: var(--color-primary);
  flex-shrink: 0;
}

.right-panel__header-title {
  font-size: 15px;
  font-weight: 600;
  color: var(--color-text-primary);
  letter-spacing: 0.5px;
}

/* 工具栏 */
.right-panel__toolbar {
  padding: 10px 20px;
  border-bottom: 1px solid var(--color-border-light);
  flex-shrink: 0;
}

.right-panel__toolbar-left {
  display: flex;
  align-items: center;
  gap: 8px;
  flex-wrap: wrap;
}

.right-panel__toolbar-sep {
  width: 1px;
  height: 22px;
  background: var(--color-border);
  margin: 0 4px;
}

/* 工具栏按钮 */
.toolbar-btn {
  height: 32px;
  padding: 4px 15px;
  font-size: 13px;
  border: 1px solid var(--color-border);
  background: var(--color-bg);
  color: var(--color-text-primary);
  border-radius: 6px;
  transition: all 0.2s ease;
}

.toolbar-btn:hover {
  color: var(--color-primary);
  border-color: var(--color-primary);
}

.toolbar-btn--light {
  color: var(--color-primary);
  border-color: var(--color-primary-border);
  background: var(--color-primary-light);
}

.toolbar-btn--light:hover {
  color: var(--color-primary-dark);
  border-color: var(--color-primary);
  background: #bae0ff;
}

/* 处方类型切换按钮 */
.toolbar-btn--prescription-type {
  border-radius: 6px;
}

.toolbar-btn--prescription-type-active {
  background: var(--color-primary);
  color: #ffffff;
  border-color: var(--color-primary);
  font-weight: 500;
}

.toolbar-btn--prescription-type-active:hover {
  background: var(--color-primary-dark);
  border-color: var(--color-primary-dark);
  color: #ffffff;
}

/* 内容区域 */
.right-panel__content {
  flex: 1;
  min-height: 0;
  overflow: hidden;
}

/* 空状态 */
.right-panel__empty {
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 16px;
}

.right-panel__empty-icon {
  color: var(--color-border);
}

.right-panel__empty-text {
  font-size: 14px;
  color: var(--color-text-placeholder);
  letter-spacing: 0.5px;
}
</style>
