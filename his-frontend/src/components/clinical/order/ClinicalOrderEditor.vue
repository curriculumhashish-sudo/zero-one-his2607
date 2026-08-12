<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { Plus } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'

import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import type { AdviceColumn } from '@/components/medical/type'

import type { AdviceScenarioAdapter } from './adapter'
import OrderSelectorDialog from './OrderSelectorDialog.vue'
import type {
  ClinicalOrderCandidate,
  ClinicalOrderCatalogLoader,
  ClinicalOrderCategory,
  ClinicalOrderContext,
  ClinicalOrderDraft,
  ClinicalOrderStatus
} from './type'
import {
  calculateSuggestedOrderQuantity,
  groupClinicalOrders,
  normalizeOrphanClinicalOrderGroups,
  ungroupClinicalOrders,
  updateClinicalOrder
} from './utils'

defineOptions({ name: 'ClinicalOrderEditor' })

const props = withDefaults(
  defineProps<{
    modelValue?: ClinicalOrderDraft[]
    context?: ClinicalOrderContext | null
    adapter: AdviceScenarioAdapter
    loadCatalog?: ClinicalOrderCatalogLoader
    patientSelected?: boolean
    accepted?: boolean
    readonly?: boolean
    loading?: boolean
    actionLoading?: boolean
    error?: string
    dirty?: boolean
    /** 是否由场景页面在变更后自动保存。 */
    autoSave?: boolean
    /** 是否展示当前医嘱查询条件。 */
    showFilters?: boolean
    selectable?: boolean
    canEditOrder?: (row: ClinicalOrderDraft) => boolean
    selectionDisabledRow?: (row: ClinicalOrderDraft) => boolean
    showStopActions?: boolean
    stopDisabled?: boolean
    cancelStopDisabled?: boolean
    showDeleteAction?: boolean
    showGroupActions?: boolean
    showHistoryAction?: boolean
    showCreateAction?: boolean
    showQuantityColumn?: boolean
    showEditFields?: boolean
    updatedAt?: string
    keyword?: string
    category?: '' | ClinicalOrderCategory
    status?: '' | ClinicalOrderStatus
    page?: number
    pageSize?: number
    total?: number
  }>(),
  {
    modelValue: () => [],
    context: null,
    loadCatalog: undefined,
    patientSelected: false,
    accepted: false,
    readonly: false,
    loading: false,
    actionLoading: false,
    error: '',
    dirty: false,
    autoSave: false,
    showFilters: true,
    selectable: true,
    canEditOrder: () => true,
    selectionDisabledRow: () => false,
    showStopActions: false,
    stopDisabled: true,
    cancelStopDisabled: true,
    showDeleteAction: true,
    showGroupActions: true,
    showHistoryAction: true,
    showCreateAction: true,
    showQuantityColumn: true,
    showEditFields: true,
    updatedAt: '',
    keyword: '',
    category: '',
    status: '',
    page: 1,
    pageSize: 10,
    total: 0
  }
)

const emit = defineEmits<{
  'update:modelValue': [orders: ClinicalOrderDraft[]]
  change: [orders: ClinicalOrderDraft[]]
  retry: []
  catalogError: [error: unknown]
  queryChange: [
    patch: Partial<{
      pageIndex: number
      pageSize: number
      keyword: string
      category: '' | ClinicalOrderCategory
      status: '' | ClinicalOrderStatus
    }>,
    resetPage?: boolean
  ]
  sign: [rows: ClinicalOrderDraft[]]
  withdraw: [rows: ClinicalOrderDraft[]]
  selectionChange: [rows: ClinicalOrderDraft[]]
  stop: [rows: ClinicalOrderDraft[]]
  cancelStop: [rows: ClinicalOrderDraft[]]
  openHistory: []
}>()

const selectorVisible = ref(false)
const selectedRows = ref<ClinicalOrderDraft[]>([])
const keywordInput = ref(props.keyword)
const canDisplayOrders = computed(() => props.accepted || props.readonly)
const existingCatalogIds = computed(() =>
  props.modelValue.filter((item) => item.status !== 'WITHDRAWN').map((item) => item.catalogId)
)
const selectedAreDrafts = computed(
  () => selectedRows.value.length > 0 && selectedRows.value.every((item) => item.status === 'DRAFT')
)
const selectedAreSigned = computed(
  () =>
    selectedRows.value.length > 0 && selectedRows.value.every((item) => item.status === 'SIGNED')
)
// The outpatient backend currently signs and withdraws medication orders only.
const selectedAreMedicationOrders = computed(
  () =>
    selectedRows.value.length > 0 && selectedRows.value.every((item) => item.category === 'DRUG')
)
const canDeleteSelected = computed(() => !props.actionLoading && selectedAreDrafts.value)
const canGroupSelected = computed(
  () => !props.actionLoading && selectedRows.value.length >= 2 && selectedAreDrafts.value
)
const canUngroupSelected = computed(
  () =>
    !props.actionLoading &&
    selectedAreDrafts.value &&
    selectedRows.value.some((item) => Boolean(item.groupId))
)

watch(
  () => props.keyword,
  (value) => {
    keywordInput.value = value
  }
)
watch(
  () => props.modelValue,
  (rows) => {
    const validKeys = new Set(rows.map((item) => item.key))
    selectedRows.value = selectedRows.value.filter((item) => validKeys.has(item.key))
  }
)

const allColumns: AdviceColumn[] = [
  { prop: 'order', label: '医嘱内容', minWidth: 220, fixed: 'left' },
  { prop: 'categoryName', label: '分类', width: 82, align: 'center' },
  { prop: 'status', label: '状态', width: 82, align: 'center' },
  { prop: 'group', label: '组别', width: 68, align: 'center' },
  // 内部编辑器实际需要 520px，列宽必须与内容宽度一致，避免覆盖后续列。
  { prop: 'usage', label: '剂量 / 用法', minWidth: 540 },
  { prop: 'quantity', label: '开立数量', width: 150 },
  { prop: 'remark', label: '备注', minWidth: 150 }
]

const columns = computed(() =>
  props.showQuantityColumn ? allColumns : allColumns.filter((column) => column.prop !== 'quantity')
)

const categoryOptions: Array<{ label: string; value: ClinicalOrderCategory }> = [
  { label: '药品', value: 'DRUG' },
  { label: '检验', value: 'LAB' },
  { label: '检查', value: 'EXAM' },
  { label: '治疗', value: 'TREATMENT' }
]
const statusOptions: Array<{ label: string; value: ClinicalOrderStatus }> = [
  { label: '未签发', value: 'DRAFT' },
  { label: '已签发', value: 'SIGNED' },
  { label: '已撤回', value: 'WITHDRAWN' }
]
const frequencyOptions = ['每日一次', '每日两次', '每日三次', '每晚一次', '必要时']
const routeOptions = ['口服', '静脉滴注', '静脉注射', '皮下注射', '肌肉注射', '外用']

const groupLabels = computed(() => {
  const labels = new Map<string, number>()
  let sequence = 0
  for (const order of props.modelValue) {
    if (order.groupId && !labels.has(order.groupId)) labels.set(order.groupId, ++sequence)
  }
  return labels
})

function commit(orders: ClinicalOrderDraft[]) {
  emit('update:modelValue', orders)
  emit('change', orders)
}

function addOrders(candidates: ClinicalOrderCandidate[]) {
  if (!props.context) return
  const existingIds = new Set(props.modelValue.map((item) => item.catalogId))
  const additions = candidates
    .filter((candidate) => {
      if (existingIds.has(candidate.id)) return false
      existingIds.add(candidate.id)
      return true
    })
    .map((candidate) => props.adapter.createDraft(candidate, props.context!))
  commit([...props.modelValue.map((item) => ({ ...item })), ...additions])
}

function asOrder(row: unknown) {
  return row as ClinicalOrderDraft
}

function isDraft(row: unknown) {
  return asOrder(row).status === 'DRAFT'
}

function isOrderEditable(row: unknown) {
  const order = asOrder(row)
  return props.showEditFields && !props.readonly && isDraft(order) && props.canEditOrder(order)
}

function updateOrder(row: unknown, patch: Partial<ClinicalOrderDraft>) {
  const order = asOrder(row)
  const next = { ...order, ...patch }
  if (order.category === 'DRUG' && ('dose' in patch || 'frequency' in patch || 'days' in patch)) {
    const suggestedQuantity = calculateSuggestedOrderQuantity(next)
    if (suggestedQuantity !== undefined) next.quantity = suggestedQuantity
  }
  commit(updateClinicalOrder(props.modelValue, order.key, next))
}

function onSelectionChange(rows: unknown[]) {
  selectedRows.value = rows.map(asOrder)
  emit('selectionChange', selectedRows.value)
}

function removeSelected() {
  if (!requireDraftSelection('删除')) return
  const keys = new Set(selectedRows.value.map((item) => item.key))
  commit(
    normalizeOrphanClinicalOrderGroups(
      props.modelValue.filter((item) => !keys.has(item.key)).map((item) => ({ ...item }))
    )
  )
}

function requireDraftSelection(actionName: string, minimum = 1) {
  if (selectedRows.value.length < minimum) {
    ElMessage.warning(
      minimum > 1 ? `${actionName}至少需要选择 ${minimum} 条医嘱` : `请选择需要${actionName}的医嘱`
    )
    return false
  }
  if (selectedRows.value.some((item) => item.status !== 'DRAFT')) {
    ElMessage.warning(`只有未签发医嘱可以${actionName}`)
    return false
  }
  return true
}

function groupSelected() {
  if (!requireDraftSelection('并组', 2)) return
  commit(
    groupClinicalOrders(
      props.modelValue,
      selectedRows.value.map((item) => item.key),
      `draft-group-${Date.now()}`
    )
  )
}

function ungroupSelected() {
  if (!requireDraftSelection('拆组')) return
  if (selectedRows.value.every((item) => !item.groupId)) {
    ElMessage.warning('选中的医嘱尚未并组')
    return
  }
  commit(
    ungroupClinicalOrders(
      props.modelValue,
      selectedRows.value.map((item) => item.key)
    )
  )
}

function statusLabel(status: ClinicalOrderStatus) {
  return statusOptions.find((item) => item.value === status)?.label ?? status
}

function statusTagType(status: ClinicalOrderStatus) {
  if (status === 'SIGNED') return 'success'
  if (status === 'WITHDRAWN') return 'info'
  return 'warning'
}

function submitSearch() {
  emit('queryChange', { keyword: keywordInput.value }, true)
}

function resetFilters() {
  keywordInput.value = ''
  emit('queryChange', { keyword: '', category: '', status: '' }, true)
}
</script>

<template>
  <section class="clinical-order-editor">
    <EmptyState
      v-if="!patientSelected"
      title="请选择患者"
      description="从患者队列选择患者后，可查看本次医嘱"
    />

    <div v-else-if="loading" class="clinical-order-editor__skeleton">
      <el-skeleton :rows="9" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="医嘱加载失败"
      :description="error"
      action-text="重试"
      @action="emit('retry')"
    />

    <EmptyState
      v-else-if="!canDisplayOrders"
      title="请先接诊患者"
      :description="
        autoSave ? '接诊后可新增和编辑医嘱，修改会自动保存' : '接诊后才能新增、编辑并保存本次医嘱'
      "
    />

    <div v-else class="clinical-order-editor__content">
      <header class="clinical-order-editor__toolbar">
        <div class="clinical-order-editor__title">
          <strong>本次医嘱</strong>
          <el-tag size="small" effect="plain">{{ total }}</el-tag>
          <span v-if="readonly" class="is-readonly">已完诊，只读</span>
          <span v-else-if="dirty" class="is-dirty">
            {{ autoSave ? '待自动保存' : '有未保存修改' }}
          </span>
          <span v-else class="is-saved">{{ autoSave ? '已自动保存' : '内容已保存' }}</span>
        </div>

        <div class="clinical-order-editor__actions">
          <span v-if="updatedAt" class="clinical-order-editor__time">
            最近保存：{{ updatedAt }}
          </span>
          <span>已选 {{ selectedRows.length }} 条</span>
          <template v-if="!readonly">
            <el-button
              v-if="showDeleteAction"
              :disabled="!canDeleteSelected"
              @click="removeSelected"
              >删除</el-button
            >
            <template v-if="showGroupActions">
              <el-button :disabled="!canGroupSelected" @click="groupSelected">并组</el-button>
              <el-button :disabled="!canUngroupSelected" @click="ungroupSelected">拆组</el-button>
            </template>
            <el-button
              type="success"
              :loading="actionLoading"
              :disabled="actionLoading || !selectedAreDrafts || !selectedAreMedicationOrders"
              @click="emit('sign', selectedRows)"
            >
              签发
            </el-button>
            <el-button
              type="warning"
              :loading="actionLoading"
              :disabled="actionLoading || !selectedAreSigned || !selectedAreMedicationOrders"
              @click="emit('withdraw', selectedRows)"
            >
              撤回
            </el-button>
            <template v-if="showStopActions">
              <el-button
                type="warning"
                :loading="actionLoading"
                :disabled="actionLoading || stopDisabled"
                @click="emit('stop', selectedRows)"
              >
                停嘱
              </el-button>
              <el-button
                type="warning"
                plain
                :loading="actionLoading"
                :disabled="actionLoading || cancelStopDisabled"
                @click="emit('cancelStop', selectedRows)"
              >
                取消停嘱
              </el-button>
            </template>
          </template>
          <el-button v-if="showHistoryAction" @click="emit('openHistory')">历史医嘱</el-button>
          <el-button
            v-if="!readonly && showCreateAction"
            type="primary"
            :icon="Plus"
            :disabled="!loadCatalog || !context"
            @click="selectorVisible = true"
          >
            新增医嘱
          </el-button>
        </div>
      </header>

      <div v-if="showFilters" class="clinical-order-editor__filters">
        <el-input
          v-model="keywordInput"
          clearable
          placeholder="医嘱名称、编码或备注"
          @keyup.enter="submitSearch"
        />
        <el-select
          :model-value="category"
          clearable
          placeholder="全部分类"
          @update:model-value="emit('queryChange', { category: $event || '' }, true)"
        >
          <el-option
            v-for="item in categoryOptions"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
        <el-select
          :model-value="status"
          clearable
          placeholder="全部状态"
          @update:model-value="emit('queryChange', { status: $event || '' }, true)"
        >
          <el-option
            v-for="item in statusOptions"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
        <el-button type="primary" @click="submitSearch">查询</el-button>
        <el-button @click="resetFilters">重置</el-button>
      </div>

      <AdviceOrderTable
        :rows="modelValue"
        :columns="columns"
        :readonly="readonly"
        :selectable="selectable"
        :disabled-row="(row) => selectionDisabledRow(asOrder(row))"
        height="100%"
        :group-by="(row) => asOrder(row).groupId || ''"
        empty-text="暂无符合条件的医嘱"
        class="clinical-order-editor__table"
        @selection-change="onSelectionChange"
      >
        <template #cell-categoryName="{ row }">
          <el-tag type="info" effect="light">{{ asOrder(row).categoryName }}</el-tag>
        </template>

        <template #cell-status="{ row }">
          <el-tag :type="statusTagType(asOrder(row).status)" effect="light">
            {{ asOrder(row).statusName || statusLabel(asOrder(row).status) }}
          </el-tag>
        </template>

        <template #cell-group="{ row }">
          <el-tag v-if="asOrder(row).groupId" size="small" effect="plain">
            组 {{ groupLabels.get(asOrder(row).groupId!) }}
          </el-tag>
          <span v-else>-</span>
        </template>

        <template #cell-usage="{ row }">
          <div
            v-if="asOrder(row).category === 'DRUG' && showEditFields"
            class="clinical-order-editor__usage"
          >
            <el-input-number
              :model-value="Number(asOrder(row).dose || 0)"
              :min="0"
              :precision="2"
              :controls="false"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { dose: $event ?? undefined })"
            />
            <el-input
              :model-value="asOrder(row).unit"
              placeholder="单位"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { unit: $event })"
            />
            <el-select
              :model-value="asOrder(row).route"
              filterable
              allow-create
              placeholder="用法"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { route: $event })"
            >
              <el-option v-for="item in routeOptions" :key="item" :label="item" :value="item" />
            </el-select>
            <el-select
              :model-value="asOrder(row).frequency"
              filterable
              allow-create
              placeholder="频次"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { frequency: $event })"
            >
              <el-option v-for="item in frequencyOptions" :key="item" :label="item" :value="item" />
            </el-select>
            <el-input-number
              :model-value="Number(asOrder(row).days || 0)"
              :min="1"
              :precision="0"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { days: $event ?? undefined })"
            />
            <span>天</span>
          </div>
          <span
            v-else-if="asOrder(row).category === 'DRUG'"
            class="clinical-order-editor__usage-text"
          >
            {{
              [
                asOrder(row).dose,
                asOrder(row).unit,
                asOrder(row).route,
                asOrder(row).frequency,
                asOrder(row).days ? `${asOrder(row).days}天` : ''
              ]
                .filter(Boolean)
                .join(' / ') || '—'
            }}
          </span>
          <span v-else class="clinical-order-editor__non-drug">按项目执行</span>
        </template>

        <template #cell-quantity="{ row }">
          <div class="clinical-order-editor__quantity">
            <el-input-number
              :model-value="asOrder(row).quantity"
              :min="1"
              :precision="0"
              :controls="false"
              :disabled="!isOrderEditable(row)"
              @update:model-value="updateOrder(row, { quantity: $event ?? undefined })"
            />
            <span>{{ asOrder(row).quantityUnit || '项' }}</span>
          </div>
        </template>

        <template #cell-remark="{ row }">
          <el-input
            v-if="showEditFields"
            :model-value="asOrder(row).remark"
            clearable
            placeholder="备注"
            :disabled="!isOrderEditable(row)"
            @update:model-value="updateOrder(row, { remark: $event })"
          />
          <span v-else>{{ asOrder(row).remark || '—' }}</span>
        </template>

        <template #summary>
          <el-alert
            v-if="!readonly"
            :title="
              autoSave
                ? '新增、修改、删除和分组会在校验通过后自动提交；签发和撤回前也会确认草稿已经保存。'
                : '新增、修改、删除和分组先保存在页面草稿中，点击医生站保存后统一提交。签发和撤回会先保存草稿。'
            "
            type="info"
            :closable="false"
            show-icon
          />
        </template>
      </AdviceOrderTable>

      <PaginationBar
        :page="page"
        :page-size="pageSize"
        :total="total"
        :disabled="loading || actionLoading"
        :page-sizes="[10, 20, 50]"
        :layout="['total', 'sizes', 'prev', 'pager', 'next']"
        @change="emit('queryChange', { pageIndex: $event.page, pageSize: $event.pageSize }, false)"
      />
    </div>

    <OrderSelectorDialog
      v-if="loadCatalog && !readonly"
      v-model:visible="selectorVisible"
      :load-page="loadCatalog"
      :existing-catalog-ids="existingCatalogIds"
      @confirm="addOrders"
      @load-error="emit('catalogError', $event)"
    />
  </section>
</template>

<style scoped>
.clinical-order-editor {
  display: flex;
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  height: 100%;
  min-height: 0;
  justify-content: center;
  color: #303133;
  background: #ffffff;
}

.clinical-order-editor__skeleton,
.clinical-order-editor__content {
  box-sizing: border-box;
  width: 100%;
  padding: 16px;
}

.clinical-order-editor__content {
  display: flex;
  height: 100%;
  min-height: 0;
  overflow: hidden;
  flex-direction: column;
  gap: 12px;
}

.clinical-order-editor__toolbar,
.clinical-order-editor__title,
.clinical-order-editor__actions,
.clinical-order-editor__filters,
.clinical-order-editor__quantity {
  display: flex;
  align-items: center;
}

.clinical-order-editor__toolbar {
  justify-content: space-between;
  gap: 16px;
}

.clinical-order-editor__title,
.clinical-order-editor__actions,
.clinical-order-editor__quantity {
  gap: 10px;
}

.clinical-order-editor__filters {
  flex-wrap: wrap;
  gap: 8px;
}

.clinical-order-editor__filters > :deep(.el-input) {
  width: 240px;
}

.clinical-order-editor__filters > :deep(.el-select) {
  width: 128px;
}

.clinical-order-editor__actions {
  flex-wrap: wrap;
  justify-content: flex-end;
  color: #606266;
  font-size: 13px;
}

.clinical-order-editor__title strong {
  color: #303133;
  font-weight: 700;
}

.clinical-order-editor__title .is-readonly,
.clinical-order-editor__time,
.clinical-order-editor__non-drug {
  color: #606266;
  font-size: 12px;
}

.clinical-order-editor__title .is-dirty {
  color: var(--el-color-warning);
  font-size: 13px;
}

.clinical-order-editor__title .is-saved {
  color: var(--el-color-success);
  font-size: 13px;
}

.clinical-order-editor__table {
  --el-table-text-color: #303133;
  --el-table-header-text-color: #606266;
  --el-table-header-bg-color: #f5f7fa;
  width: 100%;
  min-height: 0;
  flex: 1;
}

.clinical-order-editor__usage {
  display: grid;
  min-width: 520px;
  grid-template-columns: 72px 64px 120px 120px 72px 22px;
  align-items: center;
  gap: 6px;
}

.clinical-order-editor__quantity {
  min-width: 124px;
}

.clinical-order-editor__quantity :deep(.el-input-number) {
  width: 96px;
  min-width: 96px;
}

.clinical-order-editor__quantity :deep(.el-input__inner) {
  text-align: center;
}

.clinical-order-editor__usage :deep(.el-input-number),
.clinical-order-editor__usage :deep(.el-select),
.clinical-order-editor__usage :deep(.el-input) {
  width: 100%;
}

@media (max-width: 1100px) {
  .clinical-order-editor__toolbar {
    align-items: flex-start;
    flex-direction: column;
  }

  .clinical-order-editor__actions {
    width: 100%;
    justify-content: flex-start;
  }
}
</style>
