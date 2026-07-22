<script setup lang="ts">
import type {
  SelectTableColumn,
  SelectTableLoadContext,
  SelectTableQuery,
  SelectValue
} from '@/components/common/select-type'
import type { FileUploadHandler, FileUploadValue } from '@/components/common/upload-type'

import { computed, ref } from 'vue'

import FileUploadField from '@/components/common/FileUploadField.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'
import SectionTitle from '@/components/common/SectionTitle.vue'
import SelectTable from '@/components/common/SelectTable.vue'

interface DepartmentOption extends Record<string, unknown> {
  deptId: string
  deptName: string
}

interface DoctorOption extends Record<string, unknown> {
  userId: number
  realName: string
  departmentName: string
  disabled?: boolean
}

interface PatientRow extends Record<string, unknown> {
  id: string
  recordNo: string
  name: string
  gender: string
  department: string
}

interface MedicationRow extends Record<string, unknown> {
  id: number
  name: string
  specification: string
  manufacturer: string
  stock: number
}

const departments: DepartmentOption[] = [
  { deptId: 'D01', deptName: '心内科' },
  { deptId: 'D02', deptName: '呼吸内科' },
  { deptId: 'D03', deptName: '普通外科' },
  { deptId: 'D04', deptName: '儿科' }
]

const doctors: DoctorOption[] = [
  { userId: 101, realName: '王医生', departmentName: '心内科' },
  { userId: 102, realName: '李医生', departmentName: '呼吸内科' },
  { userId: 103, realName: '赵医生', departmentName: '普通外科', disabled: true }
]

const patients: PatientRow[] = [
  { id: 'P001', recordNo: 'MZ2026001', name: '张明', gender: '男', department: '心内科' },
  { id: 'P002', recordNo: 'MZ2026002', name: '李静', gender: '女', department: '呼吸内科' },
  { id: 'P003', recordNo: 'MZ2026003', name: '王强', gender: '男', department: '普通外科' }
]

const medications: MedicationRow[] = [
  { id: 1, name: '阿莫西林胶囊', specification: '0.25g×24粒', manufacturer: '示例药业', stock: 36 },
  { id: 2, name: '布洛芬缓释胶囊', specification: '0.3g×20粒', manufacturer: '示例制药', stock: 18 },
  { id: 3, name: '过期演示药品', specification: '10mg×10片', manufacturer: '示例药厂', stock: 0 }
]

const departmentId = ref<SelectValue | SelectValue[] | null>('D01')
const doctorId = ref<SelectValue | SelectValue[] | null>(null)
const patientId = ref<SelectValue | SelectValue[] | null>(null)
const medicationIds = ref<SelectValue | SelectValue[] | null>([])
const patientVisible = ref(false)
const medicationVisible = ref(false)
const selectedPatients = ref<PatientRow[]>([])
const selectedMedications = ref<MedicationRow[]>([])
const autoFiles = ref<FileUploadValue[]>([])
const manualFiles = ref<FileUploadValue[]>([])

const patientColumns: SelectTableColumn<PatientRow>[] = [
  { prop: 'recordNo', label: '病历号', width: 150 },
  { prop: 'name', label: '姓名', width: 100 },
  { prop: 'gender', label: '性别', width: 80, align: 'center' },
  { prop: 'department', label: '就诊科室', minWidth: 130 }
]

const medicationColumns: SelectTableColumn<MedicationRow>[] = [
  { prop: 'name', label: '药品名称', minWidth: 160 },
  { prop: 'specification', label: '规格', minWidth: 140 },
  { prop: 'manufacturer', label: '厂家', minWidth: 140 },
  { prop: 'stock', label: '库存', width: 90, align: 'right' }
]

function delay(milliseconds: number, signal: AbortSignal) {
  return new Promise<void>((resolve, reject) => {
    const timer = window.setTimeout(resolve, milliseconds)
    signal.addEventListener(
      'abort',
      () => {
        window.clearTimeout(timer)
        reject(new DOMException('请求已取消', 'AbortError'))
      },
      { once: true }
    )
  })
}

async function loadDepartments(keyword: string, context: { signal: AbortSignal }) {
  await delay(250, context.signal)
  return departments.filter((item) => item.deptName.includes(keyword))
}

async function loadDoctors(keyword: string, context: { signal: AbortSignal }) {
  await delay(250, context.signal)
  return doctors.filter(
    (item) => item.realName.includes(keyword) || item.departmentName.includes(keyword)
  )
}

function createLocalPageLoader<T extends Record<string, unknown>>(
  source: T[],
  fields: Array<keyof T & string>
) {
  return async (query: SelectTableQuery, context: SelectTableLoadContext) => {
    await delay(300, context.signal)
    const keyword = query.keyword.toLocaleLowerCase()
    const filtered = source.filter((row) =>
      fields.some((field) => String(row[field]).toLocaleLowerCase().includes(keyword))
    )
    const start = (query.pageIndex - 1) * query.pageSize
    return {
      rows: filtered.slice(start, start + query.pageSize),
      total: filtered.length
    }
  }
}

const loadPatients = createLocalPageLoader(patients, ['recordNo', 'name', 'department'])
const loadMedications = createLocalPageLoader(medications, ['name', 'specification', 'manufacturer'])

const uploadFile: FileUploadHandler = async (file, context) => {
  context.onProgress(20)
  await delay(250, context.signal)
  context.onProgress(70)
  await delay(250, context.signal)

  return {
    uid: file.uid,
    name: file.name,
    size: file.size,
    status: 'success',
    response: { fileId: `FILE-${file.uid}` }
  }
}

const patientSummary = computed(() => selectedPatients.value[0]?.name || '尚未选择')
const medicationSummary = computed(() =>
  selectedMedications.value.length
    ? selectedMedications.value.map((item) => item.name).join('、')
    : '尚未选择'
)
</script>

<template>
  <div class="foundation-demo">
    <header class="foundation-demo__header">
      <div>
        <h2>A 组基础公共组件</h2>
        <p>展示 C16、C17、C27、C30 在不同数据结构和交互模式下的复用方式。</p>
      </div>
      <router-link to="/sample/common">
        <el-button>返回公共组件总览</el-button>
      </router-link>
    </header>

    <section class="foundation-demo__card">
      <SectionTitle title="C16 RemoteSelectField" description="相同组件适配科室和医生两种字段结构">
        <template #actions>
          <el-tag effect="plain">远程查询 · 防抖 · 竞态保护</el-tag>
        </template>
      </SectionTitle>

      <div class="foundation-demo__grid">
        <label class="foundation-demo__field">
          <span>科室选择</span>
          <RemoteSelectField
            v-model="departmentId"
            :load-options="loadDepartments"
            :initial-options="departments"
            value-key="deptId"
            label-key="deptName"
            placeholder="请输入科室名称"
          />
        </label>

        <label class="foundation-demo__field">
          <span>医生选择</span>
          <RemoteSelectField
            v-model="doctorId"
            :load-options="loadDoctors"
            :initial-options="doctors"
            value-key="userId"
            label-key="realName"
            placeholder="请输入医生或科室"
            :option-disabled="(doctor) => Boolean(doctor.disabled)"
          >
            <template #option="{ option }">
              <span>{{ option.realName }} · {{ option.departmentName }}</span>
            </template>
          </RemoteSelectField>
        </label>
      </div>
    </section>

    <section class="foundation-demo__card">
      <SectionTitle title="C17 SelectTable" description="患者单选和药品多选共用搜索表格选择流程" dense bordered />

      <div class="foundation-demo__grid">
        <div class="foundation-demo__choice">
          <strong>患者：{{ patientSummary }}</strong>
          <el-button type="primary" @click="patientVisible = true">选择患者</el-button>
        </div>
        <div class="foundation-demo__choice">
          <strong>药品：{{ medicationSummary }}</strong>
          <el-button type="primary" @click="medicationVisible = true">选择药品</el-button>
        </div>
      </div>

      <SelectTable
        v-model="patientId"
        v-model:visible="patientVisible"
        title="选择患者"
        row-key="id"
        :columns="patientColumns"
        :load-page="loadPatients"
        :selected-rows="selectedPatients"
        @confirm="(_, rows) => (selectedPatients = rows)"
      />

      <SelectTable
        v-model="medicationIds"
        v-model:visible="medicationVisible"
        title="选择药品"
        row-key="id"
        selection-mode="multiple"
        :columns="medicationColumns"
        :load-page="loadMedications"
        :selected-rows="selectedMedications"
        :disabled-row="(row) => Number(row.stock) <= 0"
        @confirm="(_, rows) => (selectedMedications = rows)"
      >
        <template #cell-stock="{ row }">
          <el-tag :type="Number(row.stock) > 0 ? 'success' : 'danger'">
            {{ row.stock }}
          </el-tag>
        </template>
      </SelectTable>
    </section>

    <section class="foundation-demo__card">
      <SectionTitle title="C27 SectionTitle" description="普通、紧凑、说明和右侧操作均通过属性或插槽控制">
        <template #actions>
          <el-button type="primary" link>示例操作</el-button>
        </template>
      </SectionTitle>
      <SectionTitle title="紧凑分区标题" dense bordered>
        <template #description>适合单据或工作站内部的小分区</template>
      </SectionTitle>
    </section>

    <section class="foundation-demo__card">
      <SectionTitle title="C30 FileUploadField" description="自动上传和手动上传共用外部上传函数" />

      <div class="foundation-demo__grid">
        <div>
          <h4>自动上传</h4>
          <FileUploadField
            v-model="autoFiles"
            :upload="uploadFile"
            accept=".pdf,.doc,.docx"
            :max-size-m-b="10"
            :limit="3"
          />
        </div>
        <div>
          <h4>手动上传</h4>
          <FileUploadField
            v-model="manualFiles"
            :upload="uploadFile"
            accept=".pdf,.txt"
            :auto-upload="false"
            tip="选择后点击“开始上传”，用于批量确认场景"
          />
        </div>
      </div>
    </section>
  </div>
</template>

<style scoped>
.foundation-demo {
  display: flex;
  flex-direction: column;
  gap: 16px;
  padding: 20px;
}

.foundation-demo__header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 16px;
}

.foundation-demo__header h2,
.foundation-demo__header p,
.foundation-demo__card h4 {
  margin: 0;
}

.foundation-demo__header p {
  margin-top: 6px;
  color: var(--el-text-color-secondary);
}

.foundation-demo__card {
  padding: 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.foundation-demo__grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 20px;
}

.foundation-demo__field,
.foundation-demo__choice {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 8px;
}

.foundation-demo__field > span,
.foundation-demo__choice strong,
.foundation-demo__card h4 {
  color: var(--el-text-color-primary);
  font-size: 14px;
}

@media (max-width: 900px) {
  .foundation-demo__grid {
    grid-template-columns: 1fr;
  }

  .foundation-demo__header {
    flex-direction: column;
  }
}
</style>

