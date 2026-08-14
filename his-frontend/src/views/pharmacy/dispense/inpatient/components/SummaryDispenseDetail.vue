<template>
  <div class="summary-dispense-detail">
    <div class="summary-dispense-detail__header">
      <span class="summary-dispense-detail__label">汇总单号：</span>
      <span class="summary-dispense-detail__value">{{ summaryNo }}</span>
      <span class="summary-dispense-detail__sep" />
      <span class="summary-dispense-detail__label">领药人：</span>
      <span class="summary-dispense-detail__value">{{ receiverName }}</span>
      <span class="summary-dispense-detail__sep" />
      <span class="summary-dispense-detail__label">创建时间：</span>
      <span class="summary-dispense-detail__value">{{ createTime }}</span>
    </div>
    <div class="summary-dispense-detail__table">
      <el-table
        :data="items"
        row-key="key"
        style="width: 100%;"
      >
        <el-table-column
          prop="itemName"
          label="药品名称"
          minWidth="160"
        />
        <el-table-column
          prop="spec"
          label="规格"
          width="120"
        />
        <el-table-column
          prop="quantity"
          label="数量"
          width="80"
        />
        <el-table-column
          prop="batchNo"
          label="批次号"
          width="100"
        />
        <el-table-column
          prop="totalPrice"
          label="金额"
          width="100"
        />
        <el-table-column
          prop="pharmacy"
          label="发放库房"
          width="150"
        />
        <el-table-column
          prop="factory"
          label="厂家"
          minWidth="160"
        />
      </el-table>
    </div>
  </div>
</template>

<script setup lang="ts">
defineProps<{
  summaryNo: string
  receiverName: string
  createTime: string
  items: {
    key: string
    itemName: string
    spec: string
    quantity: number
    batchNo: string
    totalPrice: number
    pharmacy: string
    factory: string
  }[]
}>()
</script>

<style scoped>
.summary-dispense-detail {
  display: flex;
  flex-direction: column;
  flex: 1;
  min-height: 0;
}

.summary-dispense-detail__header {
  display: flex;
  align-items: center;
  padding: 12px 20px;
  background: #fafafa;
  border-bottom: 1px solid #e5e7eb;
  flex-wrap: wrap;
  gap: 8px;
}

.summary-dispense-detail__label {
  color: #6b7280;
}

.summary-dispense-detail__value {
  font-weight: 500;
}

.summary-dispense-detail__sep {
  width: 1px;
  height: 14px;
  background: #e5e7eb;
  margin: 0 12px;
}

.summary-dispense-detail__table {
  flex: 1;
  min-height: 0;
  overflow: auto;
  padding: 12px 20px;
}
</style>
