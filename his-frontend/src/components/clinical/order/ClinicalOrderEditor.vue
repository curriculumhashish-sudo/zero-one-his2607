<script setup lang="ts">
import { computed, ref } from 'vue'
import { ArrowDown, ArrowUp, Delete, Plus } from '@element-plus/icons-vue'

import EmptyState from '@/components/common/EmptyState.vue'
import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import type { AdviceColumn } from '@/components/medical/type'

import type { AdviceScenarioAdapter } from './adapter'
import OrderSelectorDialog from './OrderSelectorDialog.vue'
import type {
  ClinicalOrderCandidate,
  ClinicalOrderCatalogLoader,
  ClinicalOrderContext,
  ClinicalOrderDraft
} from './type'
import { moveClinicalOrder, removeClinicalOrder, updateClinicalOrder } from './utils'

defineOptions({ name: 'ClinicalOrderEditor' })

const props = withDefaults(
  defineProps<{
    /** 受控医嘱草稿列表 */
    modelValue?: ClinicalOrderDraft[]
    /** 当前诊疗上下文 */
    context?: ClinicalOrderContext | null
    /** 当前场景适配器 */
    adapter: AdviceScenarioAdapter
    /** 医嘱项目加载函数 */
    loadCatalog?: ClinicalOrderCatalogLoader
    patientSelected?: boolean
    accepted?: boolean
    readonly?: boolean
    loading?: boolean
    error?: string
    dirty?: boolean
    updatedAt?: string
  }>(),
  {
    modelValue: () => [],
    context: null,
    loadCatalog: undefined,
    patientSelected: false,
    accepted: false,
    readonly: false,
    loading: false,
    error: '',
    dirty: false,
    updatedAt: ''
  }
)

const emit = defineEmits<{
  'update:modelValue': [orders: ClinicalOrderDraft[]]
  change: [orders: ClinicalOrderDraft[]]
  retry: []
  catalogError: [error: unknown]
}>()

const selectorVisible = ref(false)
const canDisplayOrders = computed(() => props.accepted || props.readonly)
const existingCatalogIds = computed(() => props.modelValue.map((item) => item.catalogId))

const columns: AdviceColumn[] = [
  { prop: 'order', label: '医嘱内容', minWidth: 230 },
  { prop: 'categoryName', label: '分类', width: 90, align: 'center' },
  { prop: 'usage', label: '剂量 / 用法', minWidth: 280 },
  { prop: 'quantity', label: '开立数量', width: 150 },
  { prop: 'remark', label: '备注', minWidth: 160 }
]

const frequencyOptions = ['每日一次', '每日两次', '每日三次', '每晚一次', '必要时']
const routeOptions = ['口服', '静脉滴注', '静脉注射', '皮下注射', '肌肉注射', '外用']

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

function updateOrder(row: unknown, patch: Partial<ClinicalOrderDraft>) {
  commit(updateClinicalOrder(props.modelValue, asOrder(row).key, patch))
}

function removeOrder(row: unknown) {
  commit(removeClinicalOrder(props.modelValue, asOrder(row).key))
}

function moveOrder(row: unknown, direction: -1 | 1) {
  commit(moveClinicalOrder(props.modelValue, asOrder(row).key, direction))
}

function rowIndex(row: unknown) {
  return props.modelValue.findIndex((item) => item.key === asOrder(row).key)
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
      description="接诊后才能新增、编辑并保存本次医嘱"
    />

    <div v-else class="clinical-order-editor__content">
      <header class="clinical-order-editor__toolbar">
        <div class="clinical-order-editor__title">
          <strong>本次医嘱</strong>
          <el-tag size="small" effect="plain">{{ modelValue.length }}</el-tag>
          <span v-if="readonly" class="is-readonly">已完诊，只读</span>
          <span v-else-if="dirty" class="is-dirty">有未保存修改</span>
          <span v-else class="is-saved">内容已保存</span>
        </div>

        <div class="clinical-order-editor__actions">
          <span v-if="updatedAt" class="clinical-order-editor__time">
            最近保存：{{ updatedAt }}
          </span>
          <el-button
            v-if="!readonly"
            type="primary"
            :icon="Plus"
            :disabled="!loadCatalog || !context"
            @click="selectorVisible = true"
          >
            新增医嘱
          </el-button>
        </div>
      </header>

      <AdviceOrderTable
        :rows="modelValue"
        :columns="columns"
        :readonly="readonly"
        empty-text="暂无医嘱，请点击“新增医嘱”"
        class="clinical-order-editor__table"
      >
        <template #cell-categoryName="{ row }">
          <el-tag type="info" effect="light">{{ asOrder(row).categoryName }}</el-tag>
        </template>

        <template #cell-usage="{ row }">
          <div v-if="asOrder(row).category === 'DRUG'" class="clinical-order-editor__usage">
            <el-input-number
              :model-value="Number(asOrder(row).dose || 0)"
              :min="0"
              :precision="2"
              :controls="false"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { dose: $event ?? undefined })"
            />
            <el-input
              :model-value="asOrder(row).unit"
              placeholder="单位"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { unit: $event })"
            />
            <el-select
              :model-value="asOrder(row).route"
              filterable
              allow-create
              placeholder="用法"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { route: $event })"
            >
              <el-option v-for="item in routeOptions" :key="item" :label="item" :value="item" />
            </el-select>
            <el-select
              :model-value="asOrder(row).frequency"
              filterable
              allow-create
              placeholder="频次"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { frequency: $event })"
            >
              <el-option v-for="item in frequencyOptions" :key="item" :label="item" :value="item" />
            </el-select>
            <el-input-number
              :model-value="Number(asOrder(row).days || 0)"
              :min="1"
              :precision="0"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { days: $event ?? undefined })"
            />
            <span>天</span>
          </div>
          <span v-else class="clinical-order-editor__non-drug">按项目执行</span>
        </template>

        <template #cell-quantity="{ row }">
          <div class="clinical-order-editor__quantity">
            <el-input-number
              :model-value="asOrder(row).quantity"
              :min="1"
              :precision="0"
              :controls="false"
              :disabled="readonly"
              @update:model-value="updateOrder(row, { quantity: $event ?? undefined })"
            />
            <span>{{ asOrder(row).quantityUnit || '项' }}</span>
          </div>
        </template>

        <template #cell-remark="{ row }">
          <el-input
            :model-value="asOrder(row).remark"
            clearable
            placeholder="备注"
            :disabled="readonly"
            @update:model-value="updateOrder(row, { remark: $event })"
          />
        </template>

        <template v-if="!readonly" #actions="{ row }">
          <el-button
            link
            :icon="ArrowUp"
            :disabled="rowIndex(row) <= 0"
            @click="moveOrder(row, -1)"
          />
          <el-button
            link
            :icon="ArrowDown"
            :disabled="rowIndex(row) >= modelValue.length - 1"
            @click="moveOrder(row, 1)"
          />
          <el-button type="danger" link :icon="Delete" @click="removeOrder(row)" />
        </template>

        <template #summary>
          <el-alert
            v-if="!readonly"
            title="药品医嘱需要填写剂量、单位、用法、频次和天数；所有医嘱都需要填写开立数量。"
            type="info"
            :closable="false"
            show-icon
          />
        </template>
      </AdviceOrderTable>
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
  min-height: 100%;
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
  min-height: 0;
  flex-direction: column;
  gap: 12px;
}

.clinical-order-editor__toolbar,
.clinical-order-editor__title,
.clinical-order-editor__actions,
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
}

.clinical-order-editor__usage {
  display: grid;
  min-width: 520px;
  grid-template-columns: 72px 64px 120px 120px 72px 22px;
  align-items: center;
  gap: 6px;
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
  .clinical-order-editor__toolbar,
  .clinical-order-editor__actions {
    align-items: flex-start;
    flex-direction: column;
  }
}
</style>
