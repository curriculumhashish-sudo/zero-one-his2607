<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  deleteNurseStationNursingRecord,
  queryNurseStationNursingRecords,
  saveNurseStationNursingRecords,
  type NurseStationNursingRecordPaperHeaderVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'
import NursingRecordPaperTable from './components/NursingRecordPaperTable.vue'
import {
  applySaveResults,
  createEmptyNursingRecordRow,
  mapViewRowToSaveItem,
  mergeRecordsWithWeekSkeleton,
  type NursingRecordViewRow
} from './model'

defineOptions({ name: 'NurseStationNursingRecord' })

const props = defineProps<{
  /** 护理记录是单患者业务，正常情况下数组只包含左侧选中的一位患者。 */
  patients: NurseStationPatientSummary[]
}>()

const rows = ref<NursingRecordViewRow[]>([])
const paperHeader = ref<NurseStationNursingRecordPaperHeaderVO | null>(null)
const loading = ref(false)
const saving = ref(false)
let requestSerial = 0

const selectedPatient = computed(() => (props.patients.length === 1 ? props.patients[0] : null))
const canOperate = computed(() => Boolean(selectedPatient.value))
const unsavedCount = computed(() => rows.value.filter((row) => row.isDirty).length)

/** 计算本周周一和周日，查询与工具栏文案共用同一时间范围。 */
const weekRange = computed(() => {
  const today = dayjs()
  const weekday = today.day()
  const monday = today.subtract(weekday === 0 ? 6 : weekday - 1, 'day').startOf('day')
  const sunday = monday.add(6, 'day').endOf('day')
  return {
    from: monday.format('YYYY-MM-DD HH:mm:ss'),
    to: sunday.format('YYYY-MM-DD HH:mm:ss'),
    label: `本周 ${monday.format('MM-DD')} ~ ${sunday.format('MM-DD')}`
  }
})

function createIdempotencyKey(action: string) {
  return `${action}-${Date.now()}-${Math.random().toString(36).slice(2, 10)}`
}

/**
 * 切换患者时加载当周真实记录，再在页面补足 14 行纸质单骨架。
 * requestSerial 会丢弃较慢的旧请求，防止快速切换患者时串数据。
 */
async function loadRecords() {
  const serial = ++requestSerial
  rows.value = []
  paperHeader.value = null
  const patient = selectedPatient.value
  if (!patient) return

  loading.value = true
  try {
    const result = await queryNurseStationNursingRecords({
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      recordedAtFrom: weekRange.value.from,
      recordedAtTo: weekRange.value.to,
      sortAsc: true
    })
    if (serial !== requestSerial) return
    paperHeader.value = result.paperHeader
    rows.value = mergeRecordsWithWeekSkeleton(result.rows)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '加载护理记录失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

watch(selectedPatient, () => void loadRecords(), { immediate: true })

/**
 * 子组件不直接修改 props，而是把字段变化交回父组件处理。
 * 集中更新可以确保每次编辑都会标记 isDirty，保存按钮不会漏掉变化。
 */
function handleUpdateField(clientRowId: string, field: keyof NursingRecordViewRow, value: string) {
  const row = rows.value.find((item) => item.clientRowId === clientRowId)
  if (!row) return
  Object.assign(row, { [field]: value, isDirty: true })
}

/** 首次进入日期时间单元格时自动补入当前日期和分钟。 */
function handleEnsureDatetime(clientRowId: string) {
  const row = rows.value.find((item) => item.clientRowId === clientRowId)
  if (!row) return
  let changed = false
  if (!row.recordDate) {
    row.recordDate = dayjs().format('YYYY-MM-DD')
    changed = true
  }
  if (!row.recordTime) {
    row.recordTime = dayjs().format('HH:mm')
    changed = true
  }
  if (changed) row.isDirty = true
}

/** 新增额外时点行，并沿用最近一次护士签名以减少重复录入。 */
function handleAddRow() {
  const latestSignature = [...rows.value]
    .reverse()
    .find((row) => row.nurseSignature.trim())?.nurseSignature
  rows.value.push(
    createEmptyNursingRecordRow({
      rowType: 'extra',
      isDirty: true,
      recordDate: dayjs().format('YYYY-MM-DD'),
      recordTime: dayjs().format('HH:mm'),
      nurseSignature: latestSignature ?? ''
    })
  )
}

/** 保存所有已修改且包含护理内容的行，空白骨架不会发送到后端。 */
async function handleSaveAll() {
  const patient = selectedPatient.value
  if (!patient) {
    ElMessage.warning('请先选择一位患者')
    return
  }

  const dirtyRows = rows.value.filter((row) => row.isDirty)
  const records = dirtyRows
    .map(mapViewRowToSaveItem)
    .filter((item): item is NonNullable<typeof item> => Boolean(item))
  if (!records.length) {
    ElMessage.info('没有需要保存的护理记录')
    return
  }
  if (records.some((item) => !/^\d{4}-\d{2}-\d{2}$/.test(item.recordDate))) {
    ElMessage.warning('请按 MM-DD 填写记录日期')
    return
  }
  if (records.some((item) => !/^\d{2}:\d{2}$/.test(item.recordTime))) {
    ElMessage.warning('请按 HH:mm 填写记录时间')
    return
  }

  saving.value = true
  try {
    const result = await saveNurseStationNursingRecords({
      idempotencyKey: createIdempotencyKey('save-nursing-record'),
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      records
    })
    rows.value = applySaveResults(rows.value, result.results)
    if (result.savedCount) ElMessage.success(`已保存 ${result.savedCount} 条护理记录`)
    if (!result.success) {
      const failed = result.results.filter((item) => !item.success)
      ElMessage.warning(failed[0]?.message ?? '部分护理记录保存失败')
    }
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '保存护理记录失败')
  } finally {
    saving.value = false
  }
}

/**
 * 未保存的额外行直接从页面移除；已保存行先调用删除接口。
 * 骨架行不显示删除按钮，符合“只能清空、不能删除纸质表格槽位”的约束。
 */
async function handleDeleteRow(row: NursingRecordViewRow) {
  const patient = selectedPatient.value
  if (row.rowType !== 'extra' || !patient) return
  if (!row.vitalSignId) {
    rows.value = rows.value.filter((item) => item.clientRowId !== row.clientRowId)
    return
  }

  try {
    await ElMessageBox.confirm('确认删除这条护理记录？删除操作将保留审计记录。', '删除护理记录', {
      type: 'warning',
      confirmButtonText: '确定删除',
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  const result = await deleteNurseStationNursingRecord({
    idempotencyKey: createIdempotencyKey('delete-nursing-record'),
    encounterId: patient.encounterId,
    patientId: patient.patientId,
    vitalSignId: row.vitalSignId,
    expectedVersion: row.version ?? 0
  })
  if (!result.success) {
    ElMessage.warning(result.message)
    return
  }
  rows.value = rows.value.filter((item) => item.clientRowId !== row.clientRowId)
  ElMessage.success('护理记录已删除')
}
</script>

<template>
  <section class="nursing-record-module">
    <el-empty v-if="!canOperate" description="请先从左侧选择一位患者" />
    <div v-else v-loading="loading" class="nursing-record-content">
      <NursingRecordPaperTable
        :paper-header="paperHeader"
        :rows="rows"
        :saving="saving"
        :unsaved-count="unsavedCount"
        :week-range-label="weekRange.label"
        @add-row="handleAddRow"
        @delete-row="handleDeleteRow"
        @ensure-datetime="handleEnsureDatetime"
        @save-all="handleSaveAll"
        @update-field="handleUpdateField"
      />
    </div>
  </section>
</template>

<style scoped>
.nursing-record-module {
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #eef1f5;
  color: var(--nurse-text-regular, #606266);
}

.nursing-record-content {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
}

.nursing-record-module > :deep(.el-empty) {
  min-height: 0;
  flex: 1;
}
</style>
