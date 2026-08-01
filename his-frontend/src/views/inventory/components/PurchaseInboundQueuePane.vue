<template>
  <div class="purchase-inbound-queue-pane">
    <!-- ==================== 检索区 ==================== -->
    <div class="search-area" @keydown.enter="handleSearch">
      <!-- 普通输入框：独占一行，无左侧字段，框内靠左放大镜图标 -->
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
        start-placeholder="开始"
        end-placeholder="结束"
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
          <el-icon :size="17"><RefreshLeft /></el-icon>
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

    <!-- ==================== 队列区 ==================== -->
    <div class="queue-area">
      <!-- 空状态 -->
      <EmptyState
        v-if="isEmpty && !loading"
        title="暂无单据"
        :action-text="addButtonText"
        @action="handleCreate"
      />

      <!-- 卡片队列 -->
      <DocumentQueueList
        v-else
        :orders="orders"
        :selected-order="selectedOrder"
        :cabinet-list-options="cabinetListOptions"
        :practitioner-option="practitionerOption"
        :location-context-id="locationContextId"
        :show-supplier="showSupplier"
        :status-map="statusMap"
        @select="handleSelect"
        @add="handleCreate"
      />
    </div>

    <!-- ==================== 新增区 ==================== -->
    <div class="add-area">
<<<<<<< HEAD
      <el-button type="primary" size="default" style="width: 100%" @click="handleCreate">
=======
      <el-button type="primary" size="default" style="width: 100%" :disabled="addDisabled" @click="handleCreate">
>>>>>>> remotes/origin/f1-cat
        {{ addButtonText }}
      </el-button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { Search, RefreshLeft } from '@element-plus/icons-vue'
import EmptyState from '@/components/common/EmptyState.vue'
import DocumentQueueList from '@/components/warehouse/DocumentQueueList.vue'

// ==================== 类型 ====================

interface ApiOptionItem {
  value?: string | number
  label?: string
  name?: string
  id?: string | number
}

/** 检索参数 */
export interface QueueSearchParams {
  checkNo?: string // 盘点单号
  pageSize: number // 每页显示条数
  checkType?: string // 盘点类型
  startDate?: string // 开始时间
  endDate?: string // 结束时间
  pageIndex: number // 当前页码
}

// ==================== Props ====================

const props = withDefaults(
  defineProps<{
    addButtonText: string
    cabinetListOptions?: Record<string, unknown>[]
    dropdown1Options: { label: string; value: string }[]
    dropdown1Placeholder: string
    dropdown2Options: { label: string; value: string }[]
    dropdown2Placeholder: string
    locationContextId?: number | string
    practitionerOption?: ApiOptionItem[]
    showDropdown2: boolean
    showSupplier?: boolean
    /** 父组件传入的单据列表 */
    orders?: Record<string, unknown>[]
    /** 加载状态 */
    loading?: boolean
    /** 当前选中单据 */
    selectedOrder?: Record<string, unknown> | null
<<<<<<< HEAD
=======
    /** 禁用新增按钮（编辑中状态不允许新增） */
    addDisabled?: boolean
>>>>>>> remotes/origin/f1-cat
  }>(),
  {
    cabinetListOptions: () => [],
    practitionerOption: () => [],
    showSupplier: false,
    orders: () => [],
    loading: false,
<<<<<<< HEAD
    selectedOrder: null
=======
    selectedOrder: null,
    addDisabled: false
>>>>>>> remotes/origin/f1-cat
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  add: []
  select: [order: Record<string, unknown>]
  search: [params: QueueSearchParams]
  reset: []
}>()

// ==================== 状态映射 ====================

const statusMap: {
  value: number
  label: string
  type: 'primary' | 'success' | 'warning' | 'danger' | 'info'
}[] = [
  { value: 0, label: '编辑中', type: 'primary' },
<<<<<<< HEAD
  { value: 1, label: '待审批', type: 'warning' },
  { value: 2, label: '已完成', type: 'success' },
  { value: 3, label: '已驳回', type: 'danger' }
=======
  { value: 1, label: '待审核', type: 'warning' },
  { value: 2, label: '审核中', type: 'warning' },
  { value: 3, label: '已撤回', type: 'info' },
  { value: 4, label: '同意', type: 'success' },
  { value: 5, label: '驳回', type: 'danger' }
>>>>>>> remotes/origin/f1-cat
]

// ==================== 检索条件 ====================

const keyword = ref('')
/** el-date-picker daterange 返回 Date 对象数组 */
const dateRange = ref<[Date, Date] | null>(null)
const dropdown1Value = ref('')
const dropdown2Value = ref('')

// ==================== 计算属性 ====================

const isEmpty = computed(() => !props.orders || props.orders.length === 0)

// ==================== 方法 ====================

/** Date 转为 YYYY-MM-DD 字符串（本地时间，避免 UTC 偏移一天） */
function formatDate(d: Date): string {
  const y = d.getFullYear()
  const m = String(d.getMonth() + 1).padStart(2, '0')
  const day = String(d.getDate()).padStart(2, '0')
  return `${y}-${m}-${day}`
}

function handleSearch() {
  emit('search', {
    checkNo: keyword.value,
    pageSize: 10,
    checkType: dropdown1Value.value,
    startDate: dateRange.value ? formatDate(dateRange.value[0]) : undefined,
    endDate: dateRange.value ? formatDate(dateRange.value[1]) : undefined,
    pageIndex: 1
  })
}

function handleReset() {
  keyword.value = ''
  dateRange.value = null
  dropdown1Value.value = ''
  dropdown2Value.value = ''
  emit('reset')
}

function handleSelect(order: Record<string, unknown>) {
  emit('select', order)
}

function handleCreate() {
  emit('add')
}
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
