<script setup lang="ts">
import { ref, watch } from 'vue'
import { Printer } from '@element-plus/icons-vue'

import { pagePatientExecutionHistory } from '@/apis/outpatient/nurse-station'
import type {
  NurseConsumableItemVO,
  NurseExecutionItemVO,
  NurseExecutionRecordVO,
  NursePrintDocumentType
} from '@/apis/outpatient/nurse-station'
import EmptyState from '@/components/common/EmptyState.vue'

import { NURSE_PRINT_OPTIONS } from '../print-targets'

defineOptions({
  name: 'NurseRightSidebar'
})

interface Props {
  executionItems?: NurseExecutionItemVO[]
  items?: NurseConsumableItemVO[]
  loading?: boolean
  printingType?: '' | NursePrintDocumentType
  canPrint?: boolean
  selectedCount?: number
  patientId?: string
}

const props = withDefaults(defineProps<Props>(), {
  executionItems: () => [],
  items: () => [],
  loading: false,
  printingType: '',
  canPrint: false,
  selectedCount: 0,
  patientId: ''
})

const emit = defineEmits<{
  print: [documentType: NursePrintDocumentType]
}>()

const activeTab = ref<'supplies' | 'print' | 'records'>('supplies')

// -- 执行记录（独立分页查询）-------------------------------------------
const records = ref<NurseExecutionRecordVO[]>([])
const recordsLoading = ref(false)
const recordsPageIndex = ref(1)
const recordsPageSize = ref(10)
const recordsTotal = ref(0)
let recordsRequestId = 0

async function loadRecords() {
  if (!props.patientId) {
    records.value = []
    recordsTotal.value = 0
    return
  }
  const requestId = ++recordsRequestId
  recordsLoading.value = true
  try {
    const result = await pagePatientExecutionHistory(props.patientId, {
      pageIndex: recordsPageIndex.value,
      pageSize: recordsPageSize.value
    })
    if (requestId !== recordsRequestId || activeTab.value !== 'records') return
    records.value = result.rows ?? []
    recordsTotal.value = result.total
  } catch {
    if (requestId === recordsRequestId) {
      records.value = []
      recordsTotal.value = 0
    }
  } finally {
    if (requestId === recordsRequestId) {
      recordsLoading.value = false
    }
  }
}

function handleRecordsPageChange(page: number) {
  recordsPageIndex.value = page
  void loadRecords()
}

function handleRecordsSizeChange(size: number) {
  recordsPageSize.value = size
  recordsPageIndex.value = 1
  void loadRecords()
}

// 切换到"执行记录"标签 或 患者变化时重新加载
watch(
  () => [activeTab.value, props.patientId] as const,
  ([tab, patientId]) => {
    if (tab === 'records' && patientId) {
      recordsPageIndex.value = 1
      void loadRecords()
    }
  }
)
</script>

<template>
  <section class="nurse-right-sidebar">
    <el-tabs v-model="activeTab" class="nurse-right-sidebar__tabs" stretch>
      <el-tab-pane label="耗材项目" name="supplies">
        <div v-loading="loading" class="nurse-right-sidebar__pane">
          <EmptyState
            v-if="!loading && items.length === 0"
            compact
            title="暂无耗材"
            description="当前患者没有待展示的耗材"
          />
          <div v-else class="nurse-right-sidebar__scroll">
            <article v-for="item in items" :key="item.dispenseItemId" class="supply-card">
              <div class="supply-card__header">
                <strong>{{ item.materialName }}</strong>
                <el-tag :type="item.status === 'DISPENSED' ? 'success' : 'warning'" size="small">
                  {{ item.statusName }}
                </el-tag>
              </div>
              <div class="supply-card__meta">{{ item.specification }}</div>
              <div class="supply-card__footer">
                <span>{{ item.roomName }}</span>
                <strong>{{ item.quantity }} {{ item.unitName }}</strong>
              </div>
            </article>
          </div>
        </div>
      </el-tab-pane>

      <el-tab-pane label="打印" name="print">
        <div class="nurse-right-sidebar__pane">
          <div class="print-hint">
            <el-icon><Printer /></el-icon>
            <span>优先打印勾选项目；未勾选时打印全部待执行项目。</span>
          </div>
          <div class="print-action-grid">
            <el-button
              v-for="(option, index) in NURSE_PRINT_OPTIONS"
              :key="option.type"
              :type="index === 0 ? 'primary' : 'default'"
              :loading="printingType === option.type"
              :disabled="!canPrint"
              @click="emit('print', option.type)"
            >
              {{ option.label }}
            </el-button>
          </div>
          <p class="print-selection-summary">当前已选择 {{ selectedCount }} 项</p>
        </div>
      </el-tab-pane>

      <el-tab-pane label="执行记录" name="records">
        <div v-loading="recordsLoading" class="nurse-right-sidebar__pane">
          <EmptyState
            v-if="!patientId"
            compact
            title="未选择患者"
            description="请先选择一位患者查看执行记录"
          />
          <EmptyState
            v-else-if="!recordsLoading && records.length === 0"
            compact
            title="暂无执行记录"
            description="当前患者还没有执行记录"
          />
          <template v-else>
            <div class="nurse-right-sidebar__scroll">
              <article
                v-for="item in records"
                :key="`${item.encounterId}-${item.itemId}`"
                class="execution-record"
              >
                <div class="execution-record__header">
                  <strong>{{ item.orderContent }}</strong>
                  <el-tag type="success" size="small">{{ item.statusName }}</el-tag>
                </div>
                <div v-if="item.specification" class="execution-record__specification">
                  {{ item.specification }}
                </div>
                <dl>
                  <div>
                    <dt>就诊号</dt>
                    <dd>{{ item.visitNo }}</dd>
                  </div>
                  <div>
                    <dt>执行人</dt>
                    <dd>{{ item.executorName || '—' }}</dd>
                  </div>
                  <div>
                    <dt>执行时间</dt>
                    <dd>{{ item.executedAt || '—' }}</dd>
                  </div>
                  <div>
                    <dt>执行次数</dt>
                    <dd>{{ item.executedCount }}/{{ item.plannedCount }}</dd>
                  </div>
                </dl>
              </article>
            </div>
            <el-pagination
              v-if="recordsTotal > recordsPageSize"
              class="execution-record__pagination"
              layout="prev, pager, next"
              small
              :page-size="recordsPageSize"
              :total="recordsTotal"
              :current-page="recordsPageIndex"
              @current-change="handleRecordsPageChange"
            />
          </template>
        </div>
      </el-tab-pane>
    </el-tabs>
  </section>
</template>

<style scoped>
.nurse-right-sidebar {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
}

.nurse-right-sidebar__tabs {
  display: flex;
  height: 100%;
  min-height: 0;
  flex: 1;
  flex-direction: column;
}

.nurse-right-sidebar__tabs :deep(.el-tabs__header) {
  flex-shrink: 0;
  margin: 0;
  padding: 0 12px;
}

.nurse-right-sidebar__tabs :deep(.el-tabs__item) {
  height: 54px;
  padding: 0 8px;
  font-size: 14px;
  font-weight: 600;
}

.nurse-right-sidebar__tabs :deep(.el-tabs__content),
.nurse-right-sidebar__tabs :deep(.el-tab-pane) {
  height: 100%;
  min-height: 0;
}

.nurse-right-sidebar__tabs :deep(.el-tabs__content) {
  flex: 1;
}

.nurse-right-sidebar__pane {
  display: flex;
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  flex-direction: column;
  padding: 12px;
}

.nurse-right-sidebar__scroll {
  min-height: 0;
  flex: 1;
  overflow-y: auto;
}

.supply-card,
.execution-record {
  padding: 12px;
  margin-bottom: 10px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 8px;
  background: var(--el-fill-color-extra-light);
}

.supply-card__header,
.supply-card__footer,
.execution-record__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
}

.supply-card__meta,
.supply-card__footer,
.execution-record__specification,
.execution-record dl {
  color: var(--el-text-color-regular, #606266);
  font-size: 13px;
}

.supply-card__header strong,
.supply-card__footer strong,
.execution-record__header strong {
  color: var(--el-text-color-primary, #303133);
  font-weight: 600;
}

.supply-card__meta {
  margin: 8px 0;
}

.print-hint {
  display: flex;
  align-items: flex-start;
  gap: 8px;
  padding: 10px;
  margin-bottom: 12px;
  border-radius: 8px;
  color: var(--el-text-color-secondary);
  background: var(--el-fill-color-light);
  font-size: 13px;
  line-height: 1.5;
}

.print-hint .el-icon {
  flex-shrink: 0;
  margin-top: 2px;
  color: var(--el-color-primary);
  font-size: 18px;
}

.print-action-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
}

.print-action-grid .el-button {
  width: 100%;
  min-width: 0;
  min-height: 36px;
  padding: 6px;
  margin: 0;
  white-space: normal;
}

.print-selection-summary {
  margin: 12px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 12px;
  text-align: center;
}

.execution-record__specification {
  margin-top: 6px;
}

.execution-record dl {
  margin: 10px 0 0;
}

.execution-record dl > div {
  display: grid;
  grid-template-columns: 64px minmax(0, 1fr);
  gap: 6px;
  margin-top: 5px;
}

.execution-record dt,
.execution-record dd {
  margin: 0;
}

.execution-record dd {
  color: var(--el-text-color-regular);
}

.execution-record__pagination {
  display: flex;
  justify-content: center;
  padding-top: 8px;
  margin-top: auto;
}
</style>
