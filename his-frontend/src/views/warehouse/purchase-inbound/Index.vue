<template>
  <DocumentWorkstationShell :has-selection="Boolean(selectedOrder)">
    <template #queue-header>
      <span>历史盘点单</span>
    </template>

    <template #queue>
      <PurchaseInboundQueuePane
        ref="queueRef"
        :selected-id="selectedOrder?.id"
        dropdown1-placeholder="盘点药房"
        dropdown2-placeholder="项目类型"
        show-dropdown2
        add-button-text="新增盘点单"
        @select="handleSelect"
        @create="handleCreate"
      />
    </template>

    <template #main>
      <PurchaseInboundMainPane
        :selected-order="selectedOrder"
        title="药房盘点单"
        :extra-search-fields="mainSearchFields"
        @changed="handleChanged"
        @removed="handleRemoved"
      />
    </template>
  </DocumentWorkstationShell>
</template>

<script setup lang="ts">
import { onMounted, reactive, ref } from 'vue'
import DocumentWorkstationShell from '@/components/warehouse/DocumentWorkstationShell.vue'
import { loadPurchaseInboundInit } from '@/apis/warehouse/purchase-inbound'
import type { PurchaseInboundOrder } from '@/apis/warehouse/purchase-inbound'
import PurchaseInboundQueuePane from './components/PurchaseInboundQueuePane.vue'
import PurchaseInboundMainPane from './components/PurchaseInboundMainPane.vue'
import type { ExtraSearchField } from './components/PurchaseInboundMainPane.vue'

const selectedOrder = ref<PurchaseInboundOrder | null>(null)
const queueRef = ref<InstanceType<typeof PurchaseInboundQueuePane> | null>(null)

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
    defaultValue: '药品',
    options: [
      { label: '药品', value: '药品' },
      { label: '耗材', value: '耗材' }
    ]
  }
])

function handleSelect(order: PurchaseInboundOrder) {
  selectedOrder.value = order
}

function handleCreate() {
  // 未保存的新单不写入历史队列，保存成功后再由接口返回正式 ID 和单据号。
  selectedOrder.value = {
    id: '',
    docNo: '',
    date: '',
    handler: '',
    counterparty: '',
    status: 0,
    type: '',
    category: '药品'
  }
}

async function handleChanged(order: PurchaseInboundOrder) {
  selectedOrder.value = order
  await queueRef.value?.refresh()
}

async function handleRemoved() {
  selectedOrder.value = null
  await queueRef.value?.refresh()
}

onMounted(async () => {
  try {
    const data = await loadPurchaseInboundInit()
    const pharmacyField = mainSearchFields.find((field) => field.prop === 'pharmacy')
    if (pharmacyField) pharmacyField.options = data.pharmacyOptions
  } catch {
    // 队列会展示接口加载错误；主区域保留空选项，避免初始化失败阻断页面渲染。
  }
})
</script>
