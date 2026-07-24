<template>
  <div class="purchase-inbound-queue-pane">
    <div class="search-area" @keydown.enter="handleSearch">
      <el-input
        v-model="keyword"
        placeholder="单据号检索"
        clearable
        :prefix-icon="Search"
        size="default"
      />

      <el-date-picker
        v-model="dateRange"
        type="daterange"
        range-separator="至"
        start-placeholder="开始日期"
        end-placeholder="结束日期"
        size="default"
        style="width: 100%"
        @change="handleSearch"
      />

      <div class="search-area__row">
        <el-select
          v-model="dropdown1Value"
          :placeholder="dropdown1Placeholder"
          clearable
          size="default"
          style="flex: 1"
          @change="handleSearch"
        >
          <el-option
            v-for="option in dropdown1Options"
            :key="option.value"
            :label="option.label"
            :value="option.value"
          />
        </el-select>
        <el-button size="default" @click="handleReset">
          <el-icon><Refresh /></el-icon>
          &nbsp;重置
        </el-button>
      </div>

      <el-select
        v-if="showDropdown2"
        v-model="dropdown2Value"
        :placeholder="dropdown2Placeholder"
        clearable
        size="default"
        style="width: 100%"
        @change="handleSearch"
      >
        <el-option
          v-for="option in dropdown2Options"
          :key="option.value"
          :label="option.label"
          :value="option.value"
        />
      </el-select>
    </div>

    <div class="queue-area">
      <EmptyState
        v-if="isEmpty && !loading"
        title="暂无盘点单"
        :action-text="addButtonText"
        @action="handleCreate"
      />
      <DocumentQueueList
        v-else
        v-model:selected-key="selectedKey"
        :orders="orders"
        :loading="loading"
        :error="error"
        row-key="id"
        empty-text="暂无盘点单"
        @select="handleSelect"
        @refresh="handleSearch"
        @retry="handleSearch"
      />
    </div>

    <div class="add-area">
      <el-button type="primary" size="default" style="width: 100%" @click="handleCreate">
        {{ addButtonText }}
      </el-button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue'
import { Refresh, Search } from '@element-plus/icons-vue'
import EmptyState from '@/components/common/EmptyState.vue'
import DocumentQueueList from '@/components/warehouse/DocumentQueueList.vue'
import {
  loadPurchaseInboundInit,
  pagePurchaseInboundOrders
} from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder } from '@/apis/warehouse/purchase-inbound'

const props = withDefaults(
  defineProps<{
    selectedId?: string
    dropdown1Placeholder?: string
    dropdown2Placeholder?: string
    showDropdown2?: boolean
    addButtonText?: string
  }>(),
  {
    selectedId: '',
    dropdown1Placeholder: '请选择药房',
    dropdown2Placeholder: '请选择项目类型',
    showDropdown2: false,
    addButtonText: '新增盘点单'
  }
)

const emit = defineEmits<{
  create: []
  select: [order: PurchaseInboundOrder]
}>()

const keyword = ref('')
const dateRange = ref<[Date, Date] | null>(null)
const dropdown1Value = ref('')
const dropdown2Value = ref('')
const dropdown1Options = ref<{ label: string; value: string }[]>([])
const dropdown2Options = ref<{ label: string; value: string }[]>([])
const orders = ref<PurchaseInboundOrder[]>([])
const loading = ref(false)
const error = ref<string | boolean>(false)
const selectedKey = ref('')
const isEmpty = computed(() => orders.value.length === 0)

function formatDate(date: Date): string {
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')
  return `${year}-${month}-${day}`
}

async function handleSearch() {
  loading.value = true
  error.value = false
  try {
    const result = await pagePurchaseInboundOrders({
      keyword: keyword.value,
      dateRange: dateRange.value
        ? [formatDate(dateRange.value[0]), formatDate(dateRange.value[1])]
        : null,
      type: dropdown1Value.value,
      category: dropdown2Value.value
    })
    orders.value = result.rows
  } catch {
    error.value = '加载失败'
  } finally {
    loading.value = false
  }
}

function handleReset() {
  keyword.value = ''
  dateRange.value = null
  dropdown1Value.value = ''
  dropdown2Value.value = ''
  selectedKey.value = ''
  void handleSearch()
}

function handleSelect(order: PurchaseInboundOrder) {
  emit('select', order)
}

function handleCreate() {
  // 新单由父页面持有，避免未保存数据混入接口返回的历史队列。
  selectedKey.value = ''
  emit('create')
}

async function loadInit() {
  try {
    const data = await loadPurchaseInboundInit()
    dropdown1Options.value = data.pharmacyOptions
    dropdown2Options.value = data.categoryOptions
  } catch {
    // 初始化选项失败不阻断列表查询，用户仍可通过单据号和日期检索。
  }
}

watch(
  () => props.selectedId,
  (id) => {
    selectedKey.value = id || ''
  },
  { immediate: true }
)

onMounted(() => {
  void loadInit()
  void handleSearch()
})

defineExpose({ refresh: handleSearch })
</script>

<style scoped>
.purchase-inbound-queue-pane {
  display: flex;
  flex-direction: column;
  height: 100%;
  gap: 12px;
}

.search-area {
  display: flex;
  flex-direction: column;
  gap: 10px;
  padding: 16px;
  background: var(--el-bg-color);
  border-radius: var(--el-border-radius-base);
}

.search-area__row {
  display: flex;
  align-items: center;
  gap: 8px;
}

.queue-area {
  flex: 1;
  min-height: 0;
  overflow: hidden;
}

.add-area {
  flex-shrink: 0;
  padding: 0 15px;
}
</style>
