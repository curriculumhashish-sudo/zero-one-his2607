<script setup lang="ts">
import { computed, ref } from 'vue'
import { ElMessage } from 'element-plus'

import StationTriPaneLayout from '@/components/common/StationTriPaneLayout.vue'
import type { DateRangeValue } from '@/components/common/type'
import PatientInfoBar from '@/components/patient/PatientInfoBar.vue'
import PatientQueuePanel from '@/components/patient/PatientQueuePanel.vue'
import type { PatientSummary } from '@/components/patient/type'

const allPatients: PatientSummary[] = [
  {
    id: 'outpatient-001',
    name: '张明',
    gender: '男',
    age: 38,
    patientType: 'outpatient',
    serviceType: '普通门诊',
    patientNo: 'MR20260720001',
    visitNo: 'OP20260720001',
    queueNumber: 'A012',
    visitTime: '2026-07-20 09:30',
    departmentName: '心血管内科',
    status: '候诊中',
    statusTone: 'warning',
    paymentType: '职工医保'
  },
  {
    id: 'emergency-002',
    name: '李华',
    gender: '女',
    age: 72,
    patientType: 'emergency',
    serviceType: '急诊',
    patientNo: 'MR20260720002',
    visitNo: 'ER20260720008',
    queueNumber: 'E003',
    visitTime: '2026-07-20 09:42',
    departmentName: '急诊医学科',
    status: '待处置',
    statusTone: 'danger',
    paymentType: '自费',
    priority: '优先',
    riskHint: '青霉素过敏，高跌倒风险'
  },
  {
    id: 'inpatient-003',
    name: '王芳',
    gender: '女',
    age: '56岁',
    patientType: 'inpatient',
    serviceType: '08床',
    patientNo: 'ZY20260088',
    visitNo: 'IP20260088',
    visitTime: '2026-07-18 14:20',
    departmentName: '呼吸内科',
    status: '在院',
    statusTone: 'success',
    paymentType: '居民医保'
  },
  {
    id: 'outpatient-004',
    name: '赵强',
    gender: '男',
    age: 45,
    patientType: 'outpatient',
    serviceType: '专家门诊',
    patientNo: 'MR20260720004',
    visitNo: 'OP20260720004',
    queueNumber: 'A018',
    visitTime: '2026-07-20 10:05',
    departmentName: '骨科',
    status: '已接诊',
    statusTone: 'primary',
    paymentType: '自费'
  },
  {
    id: 'unregistered-005',
    name: null,
    gender: null,
    age: null,
    patientType: 'outpatient',
    queueNumber: 'B006',
    visitNo: 'OP20260720005',
    visitTime: '2026-07-20 10:16',
    status: '待登记',
    statusTone: 'info'
  }
]

const visiblePatients = ref<PatientSummary[]>([...allPatients])
const selectedId = ref<string | number | null>(allPatients[0]!.id)
const loading = ref(false)
const leftCollapsed = ref(false)
const rightCollapsed = ref(false)
const detailVisible = ref(false)
const activeKeyword = ref('')
const activeDateRange = ref<DateRangeValue>(null)
const queueVersion = ref(0)
const actionLogs = ref<string[]>(['演示页面已加载，默认选中张明'])

const selectedPatient = computed(
  () => visiblePatients.value.find((patient) => patient.id === selectedId.value) ?? null
)

const selectedDescriptionItems = computed(() => {
  const patient = selectedPatient.value
  if (!patient) return []

  return [
    { label: '患者 ID', value: patient.id },
    { label: '患者类型', value: patient.patientType },
    { label: '患者号', value: patient.patientNo },
    { label: '就诊号', value: patient.visitNo },
    { label: '就诊时间', value: patient.visitTime },
    { label: '科室', value: patient.departmentName },
    { label: '状态', value: patient.status },
    { label: '支付类型', value: patient.paymentType }
  ].filter((item) => item.value !== null && item.value !== undefined && item.value !== '')
})

const workspaceRows = [
  { time: '09:35', category: '工作区占位', content: '中间业务区内容示例 01' },
  { time: '09:42', category: '工作区占位', content: '中间业务区内容示例 02' },
  { time: '09:50', category: '工作区占位', content: '中间业务区内容示例 03' },
  { time: '10:03', category: '工作区占位', content: '中间业务区内容示例 04' },
  { time: '10:16', category: '工作区占位', content: '中间业务区内容示例 05' },
  { time: '10:28', category: '工作区占位', content: '中间业务区内容示例 06' }
]

function addLog(message: string) {
  const time = new Date().toLocaleTimeString('zh-CN', { hour12: false })
  actionLogs.value.unshift(`${time} ${message}`)
  actionLogs.value = actionLogs.value.slice(0, 12)
}

function applyFilters() {
  const keyword = activeKeyword.value.toLowerCase()
  const startDate = activeDateRange.value?.[0]?.slice(0, 10)
  const endDate = activeDateRange.value?.[1]?.slice(0, 10)

  visiblePatients.value = allPatients.filter((patient) => {
    const matchesKeyword =
      !keyword ||
      [patient.name, patient.patientNo, patient.visitNo]
        .filter(Boolean)
        .some((value) => String(value).toLowerCase().includes(keyword))

    const visitDate = patient.visitTime?.slice(0, 10)
    const matchesDate =
      !startDate || !endDate || !visitDate || (visitDate >= startDate && visitDate <= endDate)

    return matchesKeyword && matchesDate
  })

  if (!visiblePatients.value.some((patient) => patient.id === selectedId.value)) {
    selectedId.value = null
  }
}

function searchPatients(keyword: string) {
  activeKeyword.value = keyword
  applyFilters()
  addLog(`PatientQueuePanel search：${keyword || '清空关键词'}`)
}

function changeDate(value: DateRangeValue) {
  activeDateRange.value = value
  applyFilters()
  addLog(`PatientQueuePanel dateChange：${value?.join(' 至 ') || '清空日期'}`)
}

function refreshPatients() {
  if (loading.value) return

  loading.value = true
  addLog('PatientQueuePanel refresh：开始模拟刷新')
  window.setTimeout(() => {
    applyFilters()
    loading.value = false
    addLog('模拟刷新完成')
  }, 600)
}

function selectPatient(patient: PatientSummary) {
  selectedId.value = patient.id
  addLog(`PatientQueuePanel select：${patient.name || '无档案登记'}`)
}

function clearSelection() {
  selectedId.value = null
  addLog('清空当前患者，PatientInfoBar 展示占位')
}

function showEmptyQueue() {
  visiblePatients.value = []
  selectedId.value = null
  addLog('清空队列，PatientQueuePanel 展示 EmptyState')
}

function resetDemo() {
  visiblePatients.value = [...allPatients]
  selectedId.value = allPatients[0]!.id
  activeKeyword.value = ''
  activeDateRange.value = null
  queueVersion.value += 1
  leftCollapsed.value = false
  rightCollapsed.value = false
  addLog('恢复四组件默认演示状态')
}

function printPatient(patient: PatientSummary) {
  ElMessage.info(`模拟打印：${patient.visitNo || patient.patientNo || patient.id}`)
  addLog(`PatientInfoBar actions：打印 ${patient.name || '无档案登记'}`)
}

function acceptPatient(patient: PatientSummary) {
  ElMessage.success(`模拟操作成功：${patient.name || '无档案登记'}`)
  addLog(`PatientInfoBar actions：处理 ${patient.name || '无档案登记'}`)
}
</script>

<template>
  <div class="patient-demo-page">
    <header class="demo-hero">
      <div>
        <div class="demo-hero__eyebrow">工作站基础组件联动</div>
        <h2>四组件演示页面</h2>
        <p>
          StationTriPaneLayout、PatientQueuePanel、PatientCard 和 PatientInfoBar
          组合成一个可交互工作站。
        </p>
      </div>

      <div class="demo-hero__actions">
        <el-button size="small" @click="leftCollapsed = !leftCollapsed">
          {{ leftCollapsed ? '展开左栏' : '折叠左栏' }}
        </el-button>
        <el-button size="small" @click="rightCollapsed = !rightCollapsed">
          {{ rightCollapsed ? '展开右栏' : '折叠右栏' }}
        </el-button>
        <el-button size="small" @click="clearSelection">无患者</el-button>
        <el-button size="small" @click="showEmptyQueue">空队列</el-button>
        <el-button size="small" type="primary" @click="resetDemo">恢复演示</el-button>
      </div>
    </header>

    <div class="station-demo">
      <StationTriPaneLayout
        v-model:left-collapsed="leftCollapsed"
        v-model:right-collapsed="rightCollapsed"
        left-width="400px"
        right-width="300px"
        :gap="10"
      >
        <template #left>
          <PatientQueuePanel
            :key="queueVersion"
            :patients="visiblePatients"
            :loading="loading"
            :selected-id="selectedId"
            @search="searchPatients"
            @date-change="changeDate"
            @refresh="refreshPatients"
            @select="selectPatient"
          />
        </template>

        <template #main>
          <section class="main-workspace">
            <header class="pane-heading">
              <div>
                <span class="pane-heading__eyebrow">main slot</span>
                <h3>中间业务区</h3>
              </div>
              <el-tag effect="plain">独立滚动</el-tag>
            </header>

            <PatientInfoBar :patient="selectedPatient">
              <template #actions="{ patient }">
                <el-button size="small" @click="printPatient(patient)">打印</el-button>
                <el-button size="small" @click="detailVisible = true">详情</el-button>
                <el-button size="small" type="primary" @click="acceptPatient(patient)">
                  模拟操作
                </el-button>
              </template>
            </PatientInfoBar>

            <div class="workspace-section">
              <div class="section-heading">
                <div>
                  <h4>当前患者数据</h4>
                  <p>由页面根据 selectedId 计算，中间业务区不依赖队列内部状态。</p>
                </div>
              </div>

              <el-descriptions v-if="selectedPatient" :column="2" border>
                <el-descriptions-item
                  v-for="item in selectedDescriptionItems"
                  :key="item.label"
                  :label="item.label"
                >
                  {{ item.value }}
                </el-descriptions-item>
              </el-descriptions>
              <el-empty
                v-else
                description="未选择患者，中间业务区显示自己的空状态"
                :image-size="72"
              />
            </div>

            <div class="workspace-section">
              <div class="section-heading">
                <div>
                  <h4>业务内容占位</h4>
                  <p>这里只演示布局，不包含患者、医嘱或收费逻辑。</p>
                </div>
              </div>

              <el-table :data="workspaceRows" border>
                <el-table-column prop="time" label="时间" width="90" />
                <el-table-column prop="category" label="类型" width="130" />
                <el-table-column prop="content" label="内容" min-width="220" />
              </el-table>
            </div>
          </section>
        </template>

        <template #right>
          <aside class="assistant-pane">
            <header class="pane-heading">
              <div>
                <span class="pane-heading__eyebrow">right slot</span>
                <h3>右侧辅助区</h3>
              </div>
              <el-tag type="info" effect="plain">独立滚动</el-tag>
            </header>

            <section class="assistant-section">
              <h4>状态快照</h4>
              <dl class="state-list">
                <div>
                  <dt>左栏</dt>
                  <dd>{{ leftCollapsed ? '已折叠' : '已展开' }}</dd>
                </div>
                <div>
                  <dt>右栏</dt>
                  <dd>{{ rightCollapsed ? '已折叠' : '已展开' }}</dd>
                </div>
                <div>
                  <dt>队列数量</dt>
                  <dd>{{ visiblePatients.length }}</dd>
                </div>
                <div>
                  <dt>当前患者</dt>
                  <dd>{{ selectedPatient?.name || '未选择' }}</dd>
                </div>
              </dl>
            </section>

            <section class="assistant-section">
              <div class="section-heading section-heading--inline">
                <h4>事件记录</h4>
                <el-button link type="primary" @click="actionLogs = []">清空</el-button>
              </div>

              <div v-if="actionLogs.length" class="event-log">
                <div
                  v-for="(log, index) in actionLogs"
                  :key="`${log}-${index}`"
                  class="event-log__item"
                >
                  {{ log }}
                </div>
              </div>
              <el-empty v-else description="暂无事件" :image-size="52" />
            </section>

            <section class="assistant-section">
              <h4>建议测试</h4>
              <ol class="test-list">
                <li>点击 PatientCard 切换患者。</li>
                <li>输入关键词并点击查询。</li>
                <li>选择日期范围观察 dateChange。</li>
                <li>模拟刷新并观察 loading。</li>
                <li>清空队列检查 EmptyState。</li>
                <li>折叠左右栏后分别恢复。</li>
              </ol>
            </section>
          </aside>
        </template>
      </StationTriPaneLayout>
    </div>

    <el-dialog v-model="detailVisible" title="PatientInfoBar 操作插槽演示" width="520px">
      <el-descriptions v-if="selectedPatient" :column="1" border>
        <el-descriptions-item label="姓名">
          {{ selectedPatient.name || '无档案登记' }}
        </el-descriptions-item>
        <el-descriptions-item label="患者号">
          {{ selectedPatient.patientNo || '-' }}
        </el-descriptions-item>
        <el-descriptions-item label="就诊号">
          {{ selectedPatient.visitNo || '-' }}
        </el-descriptions-item>
        <el-descriptions-item label="风险提示">
          {{ selectedPatient.riskHint || '无' }}
        </el-descriptions-item>
      </el-descriptions>
      <template #footer>
        <el-button @click="detailVisible = false">关闭</el-button>
      </template>
    </el-dialog>
  </div>
</template>

<style scoped>
.patient-demo-page {
  display: flex;
  box-sizing: border-box;
  height: calc(100vh - var(--sample-header-height) - 20px);
  min-width: 0;
  min-height: 720px;
  flex-direction: column;
  gap: 8px;
  overflow: hidden;
}

.demo-hero {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 8px 12px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 12px;
  background:
    radial-gradient(circle at 88% 20%, var(--el-color-primary-light-8), transparent 32%),
    var(--el-bg-color);
}

.demo-hero__eyebrow,
.pane-heading__eyebrow {
  color: var(--el-color-primary);
  font-size: 11px;
  font-weight: 700;
  letter-spacing: 0.08em;
  text-transform: uppercase;
}

.demo-hero h2,
.pane-heading h3,
.section-heading h4,
.assistant-section h4 {
  margin: 0;
  color: var(--el-text-color-primary);
}

.demo-hero h2 {
  margin-top: 1px;
  font-size: 18px;
}

.demo-hero p,
.section-heading p {
  margin: 2px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.demo-hero__actions {
  display: flex;
  flex-shrink: 0;
  flex-wrap: wrap;
  justify-content: flex-end;
  gap: 6px;
}

.station-demo {
  flex: 1;
  min-width: 0;
  min-height: 0;
}

.main-workspace,
.assistant-pane {
  display: flex;
  box-sizing: border-box;
  min-width: 0;
  min-height: 100%;
  flex-direction: column;
  gap: 14px;
  padding: 14px;
}

.pane-heading,
.section-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.pane-heading {
  flex-shrink: 0;
}

.pane-heading h3 {
  margin-top: 2px;
  font-size: 17px;
}

.workspace-section,
.assistant-section {
  padding: 14px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 8px;
  background: var(--el-bg-color);
}

.section-heading {
  margin-bottom: 12px;
}

.section-heading h4,
.assistant-section h4 {
  font-size: 14px;
}

.section-heading--inline {
  margin-bottom: 8px;
}

.state-list {
  display: grid;
  gap: 8px;
  margin: 10px 0 0;
}

.state-list > div {
  display: flex;
  justify-content: space-between;
  gap: 12px;
  padding-bottom: 7px;
  border-bottom: 1px dashed var(--el-border-color-lighter);
  font-size: 12px;
}

.state-list dt {
  color: var(--el-text-color-secondary);
}

.state-list dd {
  margin: 0;
  color: var(--el-text-color-primary);
  font-weight: 600;
  text-align: right;
}

.event-log {
  display: flex;
  flex-direction: column;
  overflow: hidden;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: 6px;
}

.event-log__item {
  padding: 7px 8px;
  color: var(--el-text-color-regular);
  background: var(--el-fill-color-extra-light);
  font-family: Consolas, Monaco, monospace;
  font-size: 11px;
  line-height: 1.45;
  overflow-wrap: anywhere;
}

.event-log__item + .event-log__item {
  border-top: 1px solid var(--el-border-color-lighter);
}

.test-list {
  display: grid;
  gap: 7px;
  margin: 10px 0 0;
  padding-left: 20px;
  color: var(--el-text-color-regular);
  font-size: 12px;
}

.test-list li::marker {
  color: var(--el-color-primary);
  font-weight: 700;
}

@media (max-width: 900px) {
  .patient-demo-page {
    height: auto;
    min-height: 900px;
    overflow: visible;
  }

  .demo-hero {
    align-items: flex-start;
    flex-direction: column;
  }

  .demo-hero__actions {
    justify-content: flex-start;
  }

  .station-demo {
    min-height: 720px;
  }
}
</style>
