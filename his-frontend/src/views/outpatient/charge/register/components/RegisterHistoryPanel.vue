<script setup lang="ts">
import type { RegisterHistoryRecord } from '@/apis/outpatient/charge/register'

import DataTablePanel from '@/components/common/DataTablePanel.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import { REGISTER_STATUS_OPTIONS } from '../constants'

defineProps<{
  loading: boolean
  records: RegisterHistoryRecord[]
  total: number
  page: number
  pageSize: number
}>()
const emit = defineEmits<{
  cancel: [record: RegisterHistoryRecord]
  print: [record: RegisterHistoryRecord]
  refresh: []
  'update:page': [value: number]
  'update:pageSize': [value: number]
}>()

function requestPrint(source: unknown) {
  emit('print', source as RegisterHistoryRecord)
}

function requestCancel(source: unknown) {
  emit('cancel', source as RegisterHistoryRecord)
}
</script>

<template>
  <DataTablePanel
    :data="records"
    :loading="loading"
    :total="total"
    :page="page"
    :page-size="pageSize"
    min-height="280px"
    @update:page="emit('update:page', $event)"
    @update:page-size="emit('update:pageSize', $event)"
  >
    <el-table-column prop="startTime" label="就诊时间" width="165" />
    <el-table-column prop="patientName" label="患者" width="100" />
    <el-table-column prop="organizationName" label="科室" min-width="120" />
    <el-table-column prop="doctorName" label="医生" width="100" />
    <el-table-column prop="serviceTypeName" label="号别" width="100" />
    <el-table-column label="金额" width="90"
      ><template #default="{ row }">¥{{ Number(row.amount).toFixed(2) }}</template></el-table-column
    >
    <el-table-column label="状态" width="90"
      ><template #default="{ row }"
        ><StatusDisplay :value="row.statusEnum" :options="REGISTER_STATUS_OPTIONS" /></template
    ></el-table-column>
    <el-table-column label="操作" width="145" fixed="right"
      ><template #default="{ row }"
        ><el-button
          type="primary"
          link
          :disabled="row.statusEnum === 'CANCELLED'"
          @click="requestPrint(row)"
          >小票</el-button
        ><el-button type="danger" link :disabled="!row.canCancel" @click="requestCancel(row)"
          >退号</el-button
        ></template
      ></el-table-column
    >
  </DataTablePanel>
</template>
