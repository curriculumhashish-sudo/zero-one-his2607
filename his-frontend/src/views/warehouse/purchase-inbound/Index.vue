<template>
  <DocumentWorkstationShell :has-selection="true">
    <!-- 左侧标题 -->
    <template #queue-header>
      <span class="queue-header-title">历史盘点单</span>
    </template>

    <!-- 左侧 Queue 区 -->
    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="盘点药房"
        :dropdown1-options="queueDropdown1Options"
        dropdown2-placeholder="项目类型"
        :dropdown2-options="queueDropdown2Options"
        :show-dropdown2="true"
        add-button-text="新增盘点单"
        @select="selectedOrder = $event"
        @create="selectedOrder = null"
        @reset="selectedOrder = null"
      />
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <PurchaseInboundMainPane
        :order-id="selectedOrder?.id"
        title="药房盘点单"
        :extra-search-fields="mainSearchFields"
        :form-columns="formColumns"
        :row-defaults="rowDefaults"
        :select-table-columns="selectTableColumns"
      />
    </template>
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import PurchaseInboundQueuePane from '@/views/warehouse/purchase-inbound/components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from '@/views/warehouse/purchase-inbound/components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from '@/views/warehouse/purchase-inbound/components/PurchaseInboundMainPane.vue'
import { getFormConfig } from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder } from '@/apis/warehouse/purchase-inbound/type'

// ==================== 药房列表（静态数据） ====================

const pharmacyOptions = [
  { label: '门诊西药房', value: '门诊西药房' },
  { label: '门诊中药房', value: '门诊中药房' },
  { label: '门诊耗材房', value: '门诊耗材房' },
  { label: '住院西药房', value: '住院西药房' },
  { label: '住院中药房', value: '住院中药房' },
  { label: '住院耗材房', value: '住院耗材房' }
]

const categoryOptions = [
  { label: '药品', value: '药品' },
  { label: '耗材', value: '耗材' }
]

// ==================== 选中状态 ====================

const selectedOrder = ref<PurchaseInboundOrder | null>(null)

// ==================== 左侧 Queue 区下拉菜单配置 ====================

const queueDropdown1Options = pharmacyOptions
const queueDropdown2Options = categoryOptions

// ==================== 右侧 Main 区检索配置 ====================

const mainSearchFields = reactive<ExtraSearchField[]>([
  {
    label: '单据号',
    prop: 'docNo',
    type: 'input',
    placeholder: '保存后生成',
    disabled: true
  },
  {
    label: '盘点药房',
    prop: 'pharmacy',
    type: 'select',
    placeholder: '请选择',
    options: pharmacyOptions
  },
  {
    label: '项目类型',
    prop: 'itemType',
    type: 'select',
    placeholder: '请选择',
    options: categoryOptions
  }
])

// ==================== Main 区表单配置（从 mock 加载） ====================

const formColumns = ref<any[]>([])
const rowDefaults = ref<Record<string, any>>({})
const selectTableColumns = ref<any[]>([])

onMounted(async () => {
  try {
    const config = await getFormConfig()
    // 父组件控制列项：过滤掉不需要的列即可
    formColumns.value = config.columns.filter(
      (c) => c.prop !== 'profitLossAmount' // 示例：排除"盈亏金额"列
    )
    rowDefaults.value = config.rowDefaults
    selectTableColumns.value = config.selectTableColumns
  } catch {
    // 加载失败使用默认空值
  }
})
</script>

<style scoped>
.queue-header-title {
  font-size: 16px;
  font-weight: 600;
  color: var(--el-text-color-primary);
  position: relative;
  padding-left: 10px;
}

.queue-header-title::before {
  position: absolute;
  top: 50%;
  left: 0;
  width: 3px;
  height: 14px;
  background: var(--el-color-primary);
  border-radius: 2px;
  content: '';
  transform: translateY(-50%);
}
</style>
