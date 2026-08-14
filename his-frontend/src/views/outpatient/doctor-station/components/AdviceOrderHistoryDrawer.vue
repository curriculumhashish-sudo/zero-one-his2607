<script setup lang="ts">
import { computed, ref, watch } from 'vue'

import type {
  DoctorAdviceOrderHistoryEncounterVO,
  DoctorAdviceOrderVO
} from '@/apis/outpatient/doctor-station'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import RecordSidebar from '@/components/common/RecordSidebar.vue'
import type { DetailSummaryItem, RecordSidebarItem } from '@/components/common/type'
import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import type { AdviceColumn } from '@/components/medical/type'
import type { ClinicalOrderDraft } from '@/components/clinical/order'

import { toClinicalOrder } from '../advice/outpatientAdviceAdapter'

defineOptions({ name: 'AdviceOrderHistoryDrawer' })

const props = withDefaults(
  defineProps<{
    modelValue?: boolean
    encounters?: DoctorAdviceOrderHistoryEncounterVO[]
    encounterTotal?: number
    encounterLoading?: boolean
    encounterError?: string
    encounterPage?: number
    encounterPageSize?: number
    encounterKeyword?: string
    selectedEncounter?: DoctorAdviceOrderHistoryEncounterVO | null
    orders?: DoctorAdviceOrderVO[]
    orderTotal?: number
    orderLoading?: boolean
    orderError?: string
    orderPage?: number
    orderPageSize?: number
    orderKeyword?: string
  }>(),
  {
    modelValue: false,
    encounters: () => [],
    encounterTotal: 0,
    encounterLoading: false,
    encounterError: '',
    encounterPage: 1,
    encounterPageSize: 10,
    encounterKeyword: '',
    selectedEncounter: null,
    orders: () => [],
    orderTotal: 0,
    orderLoading: false,
    orderError: '',
    orderPage: 1,
    orderPageSize: 10,
    orderKeyword: ''
  }
)

const emit = defineEmits<{
  'update:modelValue': [visible: boolean]
  selectEncounter: [item: DoctorAdviceOrderHistoryEncounterVO]
  encounterQueryChange: [
    patch: Partial<{ pageIndex: number; pageSize: number; keyword: string }>,
    resetPage?: boolean
  ]
  orderQueryChange: [
    patch: Partial<{
      pageIndex: number
      pageSize: number
      keyword: string
    }>,
    resetPage?: boolean
  ]
  retryEncounters: []
  retryOrders: []
}>()

const encounterKeywordInput = ref(props.encounterKeyword)
const orderKeywordInput = ref(props.orderKeyword)

watch(
  () => props.encounterKeyword,
  (value) => (encounterKeywordInput.value = value)
)
watch(
  () => props.orderKeyword,
  (value) => (orderKeywordInput.value = value)
)

function visitType(item: DoctorAdviceOrderHistoryEncounterVO) {
  return item.visitType || item.departmentName
}

function doctorName(item: DoctorAdviceOrderHistoryEncounterVO) {
  return item.doctorName || item.attendingDoctorName
}

function visitTime(item: DoctorAdviceOrderHistoryEncounterVO) {
  return item.visitTime || item.visitedAt
}

const sidebarItems = computed<RecordSidebarItem[]>(() =>
  props.encounters.map((item) => ({
    key: item.encounterId,
    title: visitType(item),
    subtitle: `${item.visitNo} · ${doctorName(item)}`,
    description: visitTime(item),
    status: `药品 ${item.drugCount ?? item.signedCount} / 检查 ${item.examCount ?? 0} / 材料 ${item.materialCount ?? 0}`
  }))
)

const clinicalOrders = computed(() => props.orders.map(toClinicalOrder))
const summaryItems = computed<DetailSummaryItem[]>(() => {
  const item = props.selectedEncounter
  if (!item) return []
  return [
    { key: 'visitNo', label: '就诊号', value: item.visitNo },
    { key: 'visitType', label: '就诊类型', value: visitType(item) },
    { key: 'doctor', label: '医生', value: doctorName(item) },
    { key: 'visitTime', label: '就诊时间', value: visitTime(item) },
    { key: 'diagnosis', label: '主诊断', value: item.mainDiagnosis || '-' },
    { key: 'drugCount', label: '药品数', value: item.drugCount ?? item.signedCount },
    { key: 'examCount', label: '检查数', value: item.examCount ?? 0 },
    { key: 'materialCount', label: '材料数', value: item.materialCount ?? 0 }
  ]
})

const columns: AdviceColumn[] = [
  { prop: 'order', label: '医嘱内容', minWidth: 210 },
  { prop: 'categoryName', label: '类型', width: 90, align: 'center' },
  { prop: 'quantity', label: '数量', width: 90 },
  { prop: 'executedCount', label: '执行次数', width: 100 },
  { prop: 'sampleType', label: '标本类型', width: 120 },
  { prop: 'executeHospital', label: '执行医院', minWidth: 150 },
  { prop: 'createTime', label: '开立时间', width: 160 },
  { prop: 'doctorName', label: '医生', width: 110 },
  { prop: 'diagnosis', label: '诊断', minWidth: 150 }
]

function selectSidebarItem(item: RecordSidebarItem) {
  const encounter = props.encounters.find((row) => row.encounterId === String(item.key))
  if (encounter) emit('selectEncounter', encounter)
}

function asHistoryOrder(row: unknown) {
  return row as ClinicalOrderDraft & DoctorAdviceOrderVO
}
</script>

<template>
  <el-drawer
    :model-value="modelValue"
    title="历史医嘱"
    size="86%"
    destroy-on-close
    @update:model-value="emit('update:modelValue', $event)"
  >
    <div class="advice-history">
      <aside class="advice-history__sidebar">
        <el-alert v-if="encounterError" :title="encounterError" type="error" :closable="false">
          <template #default>
            <el-button link type="primary" @click="emit('retryEncounters')">重试</el-button>
          </template>
        </el-alert>
        <RecordSidebar
          :model-value="selectedEncounter?.encounterId"
          :items="sidebarItems"
          title="既往就诊"
          width="100%"
          :loading="encounterLoading"
          :keyword="encounterKeywordInput"
          search-placeholder="就诊号、科室或医生"
          empty-description="暂无历史医嘱就诊"
          @update:keyword="encounterKeywordInput = $event"
          @search="emit('encounterQueryChange', { keyword: encounterKeywordInput }, true)"
          @select="selectSidebarItem"
        >
          <template #footer>
            <PaginationBar
              :page="encounterPage"
              :page-size="encounterPageSize"
              :total="encounterTotal"
              :disabled="encounterLoading"
              :page-sizes="[10, 20]"
              :layout="['total', 'prev', 'pager', 'next']"
              small
              @change="
                emit(
                  'encounterQueryChange',
                  { pageIndex: $event.page, pageSize: $event.pageSize },
                  false
                )
              "
            />
          </template>
        </RecordSidebar>
      </aside>

      <main class="advice-history__detail">
        <EmptyState
          v-if="!selectedEncounter"
          title="请选择历史就诊"
          description="从左侧选择一次既往就诊后查看正式医嘱"
        />
        <template v-else>
          <DetailSummary
            title="历史就诊摘要"
            :items="summaryItems"
            :column="3"
            label-width="88px"
          />

          <div class="advice-history__filters">
            <el-input
              v-model="orderKeywordInput"
              clearable
              placeholder="医嘱名称、编码或备注"
              @keyup.enter="emit('orderQueryChange', { keyword: orderKeywordInput }, true)"
            />
            <el-button
              type="primary"
              @click="emit('orderQueryChange', { keyword: orderKeywordInput }, true)"
            >
              查询
            </el-button>
          </div>

          <el-alert v-if="orderError" :title="orderError" type="error" :closable="false">
            <template #default>
              <el-button link type="primary" @click="emit('retryOrders')">重试</el-button>
            </template>
          </el-alert>

          <AdviceOrderTable
            :rows="clinicalOrders"
            :columns="columns"
            :loading="orderLoading"
            readonly
            :group-by="(row) => String(asHistoryOrder(row).groupId || '')"
            empty-text="该次就诊暂无符合条件的正式医嘱"
          >
            <template #cell-categoryName="{ row }">
              <el-tag type="info" effect="light">
                {{ asHistoryOrder(row).categoryName }}
              </el-tag>
            </template>
            <template #cell-quantity="{ row }">
              {{ asHistoryOrder(row).quantity }} {{ asHistoryOrder(row).quantityUnit }}
            </template>
            <template #cell-executedCount="{ row }">
              {{ asHistoryOrder(row).executedCount ?? 0 }}
            </template>
            <template #cell-sampleType="{ row }">
              {{ asHistoryOrder(row).sampleType || '-' }}
            </template>
            <template #cell-executeHospital="{ row }">
              {{ asHistoryOrder(row).executeHospital || '-' }}
            </template>
            <template #cell-createTime="{ row }">
              {{ asHistoryOrder(row).createTime || '-' }}
            </template>
            <template #cell-doctorName="{ row }">
              {{ asHistoryOrder(row).doctorName || '-' }}
            </template>
            <template #cell-diagnosis="{ row }">
              {{ asHistoryOrder(row).diagnosis || '-' }}
            </template>
          </AdviceOrderTable>

          <PaginationBar
            :page="orderPage"
            :page-size="orderPageSize"
            :total="orderTotal"
            :disabled="orderLoading"
            :page-sizes="[10, 20, 50]"
            :layout="['total', 'sizes', 'prev', 'pager', 'next']"
            @change="
              emit('orderQueryChange', { pageIndex: $event.page, pageSize: $event.pageSize }, false)
            "
          />
        </template>
      </main>
    </div>
  </el-drawer>
</template>

<style scoped>
.advice-history {
  display: grid;
  min-width: 0;
  height: calc(100vh - 120px);
  grid-template-columns: 320px minmax(0, 1fr);
  gap: 16px;
  color: #303133;
}

.advice-history__sidebar,
.advice-history__detail {
  min-width: 0;
  min-height: 0;
}

.advice-history__sidebar {
  overflow: auto;
  border-right: 1px solid var(--el-border-color-light);
  padding-right: 12px;
}

.advice-history__detail {
  display: flex;
  overflow: auto;
  flex-direction: column;
  gap: 12px;
}

.advice-history__filters {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}

.advice-history__filters :deep(.el-input) {
  width: 240px;
}

.advice-history__filters :deep(.el-select) {
  width: 132px;
}

@media (max-width: 1000px) {
  .advice-history {
    grid-template-columns: 260px minmax(0, 1fr);
  }
}
</style>
