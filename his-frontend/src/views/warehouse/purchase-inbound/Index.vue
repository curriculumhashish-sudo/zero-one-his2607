<template>
  <DocumentWorkstationShell :has-selection="true">
    <!-- 队列头部 -->
    <template #queue-header>
      <span>历史盘点单</span>
    </template>

    <!-- 左侧 Queue 区 -->
    <template #queue>
      <PurchaseInboundQueuePane
        dropdown1-placeholder="盘点药房"
        @select="selectedOrder = $event"
        @create="selectedOrder = null"
      />
    </template>

    <!-- 右侧 Main 区 -->
    <template #main>
      <PurchaseInboundMainPane
        :order-id="selectedOrder?.id"
        title="药房盘点单"
        :extra-search-fields="mainSearchFields"
      />
    </template>
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import PurchaseInboundQueuePane from './components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from './components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from './components/PurchaseInboundMainPane.vue'
import { loadPurchaseInboundInit } from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder } from '@/apis/warehouse/purchase-inbound/type'

// ==================== 选中状态 ====================

const selectedOrder = ref<PurchaseInboundOrder | null>(null)

// ==================== Main 区检索配置 ====================

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
    options: []
  },
  {
    label: '项目类型',
    prop: 'itemType',
    type: 'select',
    placeholder: '请选择',
    options: [
      { label: '药品', value: '药品' },
      { label: '耗材', value: '耗材' }
    ]
  }
])

onMounted(async () => {
  try {
    const data = await loadPurchaseInboundInit()
    // 同步盘点药房选项到 Main 检索区
    const pharmacyField = mainSearchFields.find((f) => f.prop === 'pharmacy')
    if (pharmacyField) pharmacyField.options = data.pharmacyOptions
  } catch {
    // 初始化失败不影响页面渲染
  }
})
</script>
