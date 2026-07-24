<template>
  <div class="purchase-inbound-queue-pane">
    <!-- ==================== 1. 历史进货单检索 ==================== -->
    <div class="search-area" @keydown.enter="handleSearch">
      <!-- 关键词查询框：独占一行，无左侧字段，框内靠左放大镜图标 -->
      <el-input
        v-model="keyword"
        placeholder="单据号检索"
        clearable
        :prefix-icon="Search"
        size="default"
      />

      <!-- 日期检索器：独占一行 -->
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

      <!-- 下拉菜单1 + 重置按钮：二者共占一行 -->
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
            v-for="opt in dropdown1Options"
            :key="opt.value"
            :label="opt.label"
            :value="opt.value"
          />
        </el-select>
        <el-button size="default" @click="handleReset">
          <el-icon><Refresh /></el-icon>
          &nbsp;重置
        </el-button>
      </div>

      <!-- 下拉菜单2：由父组件控制是否显示，独占一行 -->
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
          v-for="opt in dropdown2Options"
          :key="opt.value"
          :label="opt.label"
          :value="opt.value"
        />
      </el-select>
    </div>

    <!-- ==================== 2. 单据队列 ==================== -->
    <div class="queue-area">
      <!-- 空状态：EmptyState + 新增单据按钮（上下分布） -->
      <EmptyState
        v-if="isEmpty && !loading"
        title="暂无进货单"
        :action-text="addButtonText"
        @action="handleCreate"
      />

      <!-- 卡片队列 -->
      <DocumentQueueList
        v-else
        v-model:selected-key="selectedKey"
        :orders="orders"
        :loading="loading"
        :error="error"
        row-key="id"
        empty-text="暂无进货单"
        @select="handleSelect"
        @refresh="handleSearch"
        @retry="handleSearch"
      />
    </div>

    <!-- ==================== 3. 新增按钮：独占一行 ==================== -->
    <div class="add-area">
      <el-button type="primary" size="default" style="width: 100%" @click="handleCreate">
        {{ addButtonText }}
      </el-button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { Search, Refresh } from '@element-plus/icons-vue'
import EmptyState from '@/components/common/EmptyState.vue'
import DocumentQueueList from '@/components/warehouse/DocumentQueueList.vue'
import {
  loadPurchaseInboundInit,
  pagePurchaseInboundOrders
} from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder } from '@/apis/warehouse/purchase-inbound/type'

// ==================== Props ====================

withDefaults(
  defineProps<{
    /** 下拉菜单1占位文本 */
    dropdown1Placeholder?: string
    /** 下拉菜单2占位文本 */
    dropdown2Placeholder?: string
    /** 是否显示下拉菜单2 */
    showDropdown2?: boolean
    /** 新增按钮文本 */
    addButtonText?: string
  }>(),
  {
    dropdown1Placeholder: '请选择类型',
    dropdown2Placeholder: '请选择类别',
    showDropdown2: false,
    addButtonText: '新增进货单'
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  create: []
  select: [order: PurchaseInboundOrder]
}>()

// ==================== 检索条件 ====================

const keyword = ref('')
/** el-date-picker daterange 返回 Date 对象数组 */
const dateRange = ref<[Date, Date] | null>(null)
const dropdown1Value = ref('')
const dropdown2Value = ref('')

// ==================== 下拉选项（从接口加载） ====================

const dropdown1Options = ref<{ label: string; value: string }[]>([])
const dropdown2Options = ref<{ label: string; value: string }[]>([])

// ==================== 单据队列 ====================

const orders = ref<PurchaseInboundOrder[]>([])
const loading = ref(false)
const error = ref<string | boolean>(false)
const selectedKey = ref<string>('')

const isEmpty = computed(() => !orders.value || orders.value.length === 0)

/** 新增空单据自增计数器 */
let idCounter = 0

// ==================== 方法 ====================

/** Date 转为 YYYY-MM-DD 字符串（本地时间，避免 UTC 偏移一天） */
function formatDate(d: Date): string {
  const y = d.getFullYear()
  const m = String(d.getMonth() + 1).padStart(2, '0')
  const day = String(d.getDate()).padStart(2, '0')
  return `${y}-${m}-${day}`
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
  handleSearch()
  selectedKey.value = ''
}

function handleSelect(order: PurchaseInboundOrder) {
  emit('select', order)
}

function handleCreate() {
  idCounter++
  const newOrder: PurchaseInboundOrder = {
    id: `pi-new-${idCounter}`,
    docNo: '',
    date: '',
    handler: '',
    counterparty: '',
    status: 0
  }
  orders.value.unshift(newOrder)
  selectedKey.value = newOrder.id
  emit('create')
  emit('select', newOrder)
}

async function loadInit() {
  try {
    const data = await loadPurchaseInboundInit()
    dropdown1Options.value = data.pharmacyOptions
    dropdown2Options.value = data.categoryOptions
  } catch {
    // 初始化失败不影响搜索功能
  }
}

// ==================== 生命周期 ====================

onMounted(() => {
  loadInit()
  handleSearch()
})
</script>

<style scoped>
.purchase-inbound-queue-pane {
  display: flex;
  flex-direction: column;
  height: 100%;
  gap: 12px;
}

/* 检索区域 */
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

/* 单据队列区域 */
.queue-area {
  flex: 1;
  min-height: 0;
  overflow: hidden;
}

/* 新增按钮区域 */
.add-area {
  flex-shrink: 0;
  padding: 0 15px;
}
</style>
