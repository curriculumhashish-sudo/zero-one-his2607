<script setup lang="ts">
import { computed } from 'vue'

import type { PrescriptionSheetData } from './prescription-sheet-type'

defineOptions({ name: 'PrescriptionSheetView' })

const props = defineProps<{
  data: PrescriptionSheetData
}>()

function display(value: string | number | null | undefined) {
  const text = value == null ? '' : String(value).trim()
  return text || '—'
}

const genderAge = computed(() =>
  [display(props.data.gender), props.data.age == null ? '—' : `${props.data.age}岁`].join(' / ')
)

function formatMoney(value?: number) {
  return Number.isFinite(value) ? Number(value).toFixed(2) : '0.00'
}
</script>

<template>
  <article class="prescription-sheet-view">
    <header class="prescription-sheet-view__heading">
      <span>零壹医院</span>
      <h2>门诊处方笺</h2>
      <small>Prescription</small>
    </header>

    <dl class="prescription-sheet-view__meta">
      <div>
        <dt>姓名</dt>
        <dd>{{ display(data.patientName) }}</dd>
      </div>
      <div>
        <dt>性别/年龄</dt>
        <dd>{{ genderAge }}</dd>
      </div>
      <div>
        <dt>门诊号</dt>
        <dd>{{ display(data.patientId) }}</dd>
      </div>
      <div>
        <dt>处方号</dt>
        <dd>{{ display(data.prescriptionNo) }}</dd>
      </div>
      <div>
        <dt>科室</dt>
        <dd>{{ display(data.department) }}</dd>
      </div>
      <div>
        <dt>费用性质</dt>
        <dd>{{ display(data.costType) }}</dd>
      </div>
      <div>
        <dt>开方日期</dt>
        <dd>{{ display(data.date) }}</dd>
      </div>
      <div>
        <dt>开单医生</dt>
        <dd>{{ display(data.doctor) }}</dd>
      </div>
      <div class="is-wide">
        <dt>临床诊断</dt>
        <dd>{{ display(data.diagnosis) }}</dd>
      </div>
    </dl>

    <section class="prescription-sheet-view__rp">
      <strong>Rp.</strong>
      <div>{{ display(data.items) }}</div>
    </section>

    <div class="prescription-sheet-view__total">
      合计金额：<strong>¥{{ formatMoney(data.totalPrice) }}</strong>
    </div>

    <footer class="prescription-sheet-view__signatures">
      <span>划价：{{ display(data.priceChecker) }}</span>
      <span>调配：{{ display(data.pharmacist) }}</span>
      <span>发药：{{ display(data.dispenser) }}</span>
      <span>医师：{{ display(data.doctor) }}</span>
    </footer>
  </article>
</template>

<style scoped>
.prescription-sheet-view {
  box-sizing: border-box;
  width: min(100%, 760px);
  min-height: 560px;
  margin: 0 auto;
  padding: 28px 36px;
  border: 1px solid #c0c4cc;
  background: #ffffff;
  color: #303133;
  box-shadow: 0 4px 16px rgb(0 0 0 / 8%);
  font-family: 'Microsoft YaHei', sans-serif;
}

.prescription-sheet-view__heading {
  position: relative;
  padding-bottom: 12px;
  border-bottom: 2px solid #303133;
  text-align: center;
}

.prescription-sheet-view__heading span {
  display: block;
  font-size: 15px;
}

.prescription-sheet-view__heading h2 {
  margin: 4px 0 0;
  font-size: 28px;
  letter-spacing: 8px;
}

.prescription-sheet-view__heading small {
  color: #606266;
  letter-spacing: 2px;
  text-transform: uppercase;
}

.prescription-sheet-view__meta {
  display: grid;
  margin: 0;
  padding: 16px 0;
  border-bottom: 1px solid #909399;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px 24px;
}

.prescription-sheet-view__meta div {
  display: grid;
  min-width: 0;
  grid-template-columns: 76px minmax(0, 1fr);
}

.prescription-sheet-view__meta .is-wide {
  grid-column: 1 / -1;
}

.prescription-sheet-view__meta dt {
  color: #606266;
}

.prescription-sheet-view__meta dd {
  min-width: 0;
  margin: 0;
  overflow-wrap: anywhere;
}

.prescription-sheet-view__rp {
  display: grid;
  min-height: 250px;
  padding: 18px 4px;
  border-bottom: 1px solid #909399;
  grid-template-columns: 48px minmax(0, 1fr);
}

.prescription-sheet-view__rp strong {
  font-family: Georgia, serif;
  font-size: 24px;
  font-style: italic;
}

.prescription-sheet-view__rp div {
  line-height: 1.9;
  white-space: pre-wrap;
}

.prescription-sheet-view__total {
  padding: 12px 0;
  text-align: right;
}

.prescription-sheet-view__signatures {
  display: grid;
  padding-top: 12px;
  border-top: 1px dashed #c0c4cc;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 12px 24px;
  font-size: 13px;
}

@media (max-width: 640px) {
  .prescription-sheet-view {
    padding: 20px;
  }

  .prescription-sheet-view__meta,
  .prescription-sheet-view__signatures {
    grid-template-columns: 1fr;
  }

  .prescription-sheet-view__meta .is-wide {
    grid-column: auto;
  }
}
</style>
