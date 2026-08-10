<script setup lang="ts">
import { ref, watch } from 'vue'

import { pagePatientExecutionHistory } from '@/apis/outpatient/nurse-station'
import type {
  NurseConsumableItemVO,
  NurseExecutionItemVO,
  NurseExecutionRecordVO
} from '@/apis/outpatient/nurse-station'
import EmptyState from '@/components/common/EmptyState.vue'

import { executionStatusTagType } from '../adapter'

defineOptions({
  name: 'NurseRightSidebar'
})

interface Props {
  executionItems?: NurseExecutionItemVO[]
  items?: NurseConsumableItemVO[]
  loading?: boolean
  patientId?: string
}

const props = withDefaults(defineProps<Props>(), {
  executionItems: () => [],
  items: () => [],
  loading: false,
  patientId: ''
})

const activeTab = ref<'supplies' | 'records'>('supplies')

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
            <article
              v-for="(item, index) in items"
              :key="`${item.deviceDefId}-${index}`"
              class="supply-card"
            >
              <div class="supply-card__header">
                <strong>{{ item.supportInfo || item.deviceDefId || '—' }}</strong>
                <el-tag size="small">{{ item.statusEnum || '—' }}</el-tag>
              </div>
              <div v-if="item.usageInstruction" class="supply-card__meta">
                用法说明：{{ item.usageInstruction }}
              </div>
              <div v-if="item.lotNumber" class="supply-card__meta">批号：{{ item.lotNumber }}</div>
              <div class="supply-card__footer">
                <span>发放时间：{{ item.dispenseTime || '—' }}</span>
                <strong>{{ item.quantity ?? '—' }} {{ item.unitCode || '' }}</strong>
              </div>
            </article>
          </div>
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
                  <strong>{{ item.itemName }}</strong>
                  <el-tag :type="executionStatusTagType(item.status)" size="small">
                    {{ item.statusName }}
                  </el-tag>
                </div>
                <div v-if="item.remark" class="execution-record__specification">
                  {{ item.remark }}
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
                    <dd>{{ item.executionCount }}</dd>
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
