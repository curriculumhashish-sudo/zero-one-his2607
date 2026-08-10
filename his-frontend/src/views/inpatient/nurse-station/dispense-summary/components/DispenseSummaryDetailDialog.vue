<script setup lang="ts">
import { computed } from 'vue'

import type {
  NurseStationDispenseSummaryHeaderVO,
  NurseStationDispenseSummaryLineVO
} from '@/apis/inpatient/nurse-station'

defineOptions({ name: 'DispenseSummaryDetailDialog' })

const props = withDefaults(
  defineProps<{
    header: NurseStationDispenseSummaryHeaderVO | null
    lines: NurseStationDispenseSummaryLineVO[]
    loading?: boolean
  }>(),
  { loading: false }
)

const visible = defineModel<boolean>('visible', { required: true })

/**
 * Element Plus 的标签颜色只负责辅助识别，状态文字本身仍保留，避免用户仅靠颜色判断。
 */
const statusTagType = computed(() => {
  if (props.header?.statusEnum === 'DISPENSED') return 'success'
  if (props.header?.statusEnum === 'SUMMARIZED') return 'warning'
  return 'info'
})

const statusText = computed(() => {
  if (props.header?.statusEnum === 'SUMMARIZED') return '待发送'
  if (props.header?.statusEnum === 'WAIT_REVIEW') return '已发送'
  return '已发药'
})
</script>

<template>
  <el-dialog
    v-model="visible"
    class="dispense-detail-dialog"
    width="820px"
    destroy-on-close
    :close-on-click-modal="false"
  >
    <template #header>
      <div class="dialog-title">
        <strong>汇总单详情</strong>
        <el-tag v-if="header" size="small" :type="statusTagType">{{ statusText }}</el-tag>
      </div>
    </template>

    <div v-loading="loading" class="dialog-body">
      <section v-if="header" class="summary-header-card">
        <h3>{{ header.summaryNo }}</h3>
        <dl>
          <div>
            <dt>计划发药时间</dt>
            <dd>{{ header.plannedDispenseTime }}</dd>
          </div>
          <div>
            <dt>发药时间</dt>
            <dd>{{ header.dispenseTime || '—' }}</dd>
          </div>
          <div>
            <dt>目标位置</dt>
            <dd>{{ header.desLocationId || '—' }}</dd>
          </div>
        </dl>
      </section>

      <section class="summary-line-card">
        <header>
          <strong>药品明细</strong>
          <span>共 {{ lines.length }} 项</span>
        </header>
        <el-table :data="lines" border stripe max-height="360" empty-text="暂无药品明细">
          <el-table-column type="index" label="序号" width="60" align="center" />
          <el-table-column prop="name" label="药品名称" min-width="180" show-overflow-tooltip />
          <el-table-column label="数量" width="160" align="center">
            <template #default="{ row }">{{ row.quantity }} {{ row.unitCode }}</template>
          </el-table-column>
        </el-table>
      </section>
    </div>

    <template #footer>
      <el-button @click="visible = false">关闭</el-button>
    </template>
  </el-dialog>
</template>

<style scoped>
.dialog-title {
  display: flex;
  align-items: center;
  gap: 10px;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 16px;
}

.dialog-body {
  display: flex;
  min-height: 120px;
  flex-direction: column;
  gap: 14px;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.summary-header-card,
.summary-line-card {
  padding: 12px 14px;
  border: 1px solid #e4e7ed;
  border-radius: 8px;
  background: #fff;
}

.summary-header-card {
  background: #f5f7fa;
}

.summary-header-card h3 {
  margin: 0 0 10px;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 18px;
}

.summary-header-card dl {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 10px 16px;
  margin: 0;
}

.summary-header-card dl > div {
  display: flex;
  min-width: 0;
  gap: 8px;
}

.summary-header-card dt {
  flex: 0 0 auto;
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
}

.summary-header-card dd {
  margin: 0;
  overflow: hidden;
  color: #303133;
  -webkit-text-fill-color: #303133;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.summary-line-card > header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 8px;
}

.summary-line-card > header strong {
  color: #111827;
  -webkit-text-fill-color: #111827;
}

.summary-line-card > header span {
  color: #6b7280;
  -webkit-text-fill-color: #6b7280;
}

.summary-line-card :deep(.el-table),
.summary-line-card :deep(.el-table th.el-table__cell),
.summary-line-card :deep(.el-table td.el-table__cell) {
  color: #303133;
  -webkit-text-fill-color: #303133;
}
</style>
