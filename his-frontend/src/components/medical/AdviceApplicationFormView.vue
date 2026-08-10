<script setup lang="ts">
import { computed } from 'vue'

import type { DoctorAdviceApplicationFormVO } from '@/apis/outpatient/doctor-station'

defineOptions({ name: 'AdviceApplicationFormView' })

const props = defineProps<{
  data: DoctorAdviceApplicationFormVO
}>()

/** 空字段统一显示横线，避免打印出来的表格出现不易理解的空白。 */
function display(value: string | number | null | undefined) {
  const text = value == null ? '' : String(value).trim()
  return text || '—'
}

const genderAge = computed(() => `${display(props.data.genderName)} / ${props.data.age}岁`)
</script>

<template>
  <article class="advice-application-form-view">
    <header class="advice-application-form-view__heading">
      <span>{{ data.hospitalName }}</span>
      <h2>门诊{{ data.categoryName }}申请单</h2>
      <small>申请单号：{{ data.applicationNo }}</small>
    </header>

    <dl class="advice-application-form-view__meta">
      <div>
        <dt>姓名</dt>
        <dd>{{ display(data.patientName) }}</dd>
      </div>
      <div>
        <dt>性别/年龄</dt>
        <dd>{{ genderAge }}</dd>
      </div>
      <div>
        <dt>病历号</dt>
        <dd>{{ display(data.patientNo) }}</dd>
      </div>
      <div>
        <dt>门诊号</dt>
        <dd>{{ display(data.visitNo) }}</dd>
      </div>
      <div>
        <dt>申请科室</dt>
        <dd>{{ display(data.departmentName) }}</dd>
      </div>
      <div>
        <dt>联系电话</dt>
        <dd>{{ display(data.phone) }}</dd>
      </div>
      <div class="is-wide">
        <dt>临床诊断</dt>
        <dd>{{ display(data.diagnosisNames) }}</dd>
      </div>
      <div class="is-wide">
        <dt>主诉</dt>
        <dd>{{ display(data.chiefComplaint) }}</dd>
      </div>
    </dl>

    <section class="advice-application-form-view__request">
      <h3>申请项目</h3>
      <table>
        <thead>
          <tr>
            <th>项目编码</th>
            <th>项目名称</th>
            <th>规格</th>
            <th>数量</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>{{ display(data.itemCode) }}</td>
            <td>{{ display(data.itemName) }}</td>
            <td>{{ display(data.specification) }}</td>
            <td>{{ data.quantity }} {{ data.quantityUnit }}</td>
          </tr>
        </tbody>
      </table>
      <p><strong>备注：</strong>{{ display(data.remark) }}</p>
    </section>

    <footer class="advice-application-form-view__footer">
      <span>申请医生：{{ display(data.attendingDoctorName) }}</span>
      <span>申请时间：{{ display(data.requestedAt) }}</span>
      <span>执行人员：____________</span>
      <span>执行时间：____________</span>
    </footer>
  </article>
</template>

<style scoped>
.advice-application-form-view {
  box-sizing: border-box;
  width: min(100%, 760px);
  min-height: 560px;
  margin: 0 auto;
  padding: 28px 36px;
  border: 1px solid #c0c4cc;
  background: #fff;
  color: #303133;
  box-shadow: 0 4px 16px rgb(0 0 0 / 8%);
  font-family: 'Microsoft YaHei', sans-serif;
}

.advice-application-form-view__heading {
  padding-bottom: 12px;
  border-bottom: 2px solid #303133;
  text-align: center;
}

.advice-application-form-view__heading span,
.advice-application-form-view__heading small {
  display: block;
}

.advice-application-form-view__heading h2 {
  margin: 5px 0;
  font-size: 28px;
  letter-spacing: 5px;
}

.advice-application-form-view__heading small,
.advice-application-form-view__meta dt {
  color: #606266;
}

.advice-application-form-view__meta {
  display: grid;
  margin: 0;
  padding: 16px 0;
  border-bottom: 1px solid #909399;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px 24px;
}

.advice-application-form-view__meta div {
  display: grid;
  min-width: 0;
  grid-template-columns: 76px minmax(0, 1fr);
}

.advice-application-form-view__meta .is-wide {
  grid-column: 1 / -1;
}

.advice-application-form-view__meta dd {
  min-width: 0;
  margin: 0;
  overflow-wrap: anywhere;
}

.advice-application-form-view__request {
  min-height: 230px;
  padding: 18px 0;
}

.advice-application-form-view__request h3 {
  margin: 0 0 12px;
  font-size: 17px;
}

.advice-application-form-view__request table {
  width: 100%;
  border-collapse: collapse;
}

.advice-application-form-view__request th,
.advice-application-form-view__request td {
  padding: 10px;
  border: 1px solid #909399;
  text-align: left;
}

.advice-application-form-view__request p {
  margin: 14px 0 0;
}

.advice-application-form-view__footer {
  display: grid;
  padding-top: 14px;
  border-top: 1px dashed #909399;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 12px 24px;
  font-size: 13px;
}

@media (max-width: 640px) {
  .advice-application-form-view {
    padding: 20px;
  }

  .advice-application-form-view__meta,
  .advice-application-form-view__footer {
    grid-template-columns: 1fr;
  }

  .advice-application-form-view__meta .is-wide {
    grid-column: auto;
  }
}

@media print {
  .advice-application-form-view {
    width: 100%;
    min-height: 0;
    padding: 12mm 14mm;
    border: 0;
    box-shadow: none;
  }
}
</style>
