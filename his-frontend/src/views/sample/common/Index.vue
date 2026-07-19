<template>
  <div class="common-demo-page">
    <header class="demo-header">
      <div>
        <h2>公共组件交互示例</h2>
        <p>按开发清单展示公共组件，示例数据仅用于验证组件展示和事件联动。</p>
      </div>
    </header>

    <div class="component-overview">
      <div v-for="item in componentOverview" :key="item.code" class="overview-item">
        <span class="overview-code">{{ item.code }}</span>
        <strong>{{ item.name }}</strong>
        <span>{{ item.description }}</span>
      </div>
    </div>

    <el-tabs v-model="activeTab" class="demo-tabs">
      <el-tab-pane label="C01–C03 查询列表" name="query">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>查询、日期范围与数据表格</h3>
              <p>展示查询条件外壳、日期范围输入和受控分页表格的典型组合。</p>
            </div>
            <el-tag effect="plain">SearchPanel + DateRangeField + DataTablePanel</el-tag>
          </div>

          <SearchPanel
            v-model:collapsed="queryCollapsed"
            :model="searchModel"
            collapsible
            label-width="90px"
            @search="handleSearch"
            @reset="resetSearch"
          >
            <el-row :gutter="16">
              <el-col :span="6">
                <el-form-item label="关键字">
                  <el-input v-model="searchModel.keyword" placeholder="姓名或病历号" clearable />
                </el-form-item>
              </el-col>
              <el-col :span="6">
                <el-form-item label="状态">
                  <el-select v-model="searchModel.status" placeholder="全部状态" clearable>
                    <el-option label="待诊" value="待诊" />
                    <el-option label="诊疗中" value="诊疗中" />
                    <el-option label="已完成" value="已完成" />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="就诊日期">
                  <DateRangeField />
                </el-form-item>
              </el-col>
            </el-row>
          </SearchPanel>

          <DataTablePanel
            v-model:page="tablePage"
            v-model:page-size="tablePageSize"
            :data="filteredPatients"
            :total="filteredPatients.length"
            min-height="360px"
            show-index
            selectable
          >
            <el-table-column prop="recordNo" label="病历号" width="130" />
            <el-table-column prop="name" label="患者姓名" width="120" />
            <el-table-column prop="department" label="就诊科室" width="130" />
            <el-table-column prop="doctor" label="接诊医生" width="120" />
            <el-table-column prop="date" label="就诊日期" width="130" />
            <el-table-column label="状态" width="110">
              <template #default="scope">
                <StatusDisplay :value="scope.row.status" :options="statusOptions" />
              </template>
            </el-table-column>
            <el-table-column prop="diagnosis" label="诊断摘要" min-width="200" />
          </DataTablePanel>
        </section>
      </el-tab-pane>

      <el-tab-pane label="C04–C08 分页状态操作" name="actions">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>分页、空状态、工具栏、行操作与状态展示</h3>
              <p>集中展示列表页最常用的操作区、行内动作和状态标签。</p>
            </div>
            <el-tag effect="plain"
              >PaginationBar + EmptyState + ActionToolbar + RowActionGroup + StatusDisplay</el-tag
            >
          </div>

          <ActionToolbar
            title="门诊记录"
            description="工具栏只抛出动作事件，具体业务由页面处理。"
            :actions="toolbarActions"
            :selection-count="selectedPatients.length"
            :max-visible="3"
            @action="handleToolbarAction"
          />

          <DataTablePanel
            v-model:page="actionTablePage"
            v-model:page-size="actionTablePageSize"
            :data="patients"
            :total="patients.length"
            min-height="320px"
            selectable
            show-index
            @selectable="selectedPatients = $event"
          >
            <el-table-column prop="recordNo" label="病历号" width="140" />
            <el-table-column prop="name" label="患者姓名" width="110" />
            <el-table-column prop="department" label="科室" width="120" />
            <el-table-column label="状态" width="120">
              <template #default="{ row }">
                <StatusDisplay :value="row.status" :options="statusOptions" show-description />
              </template>
            </el-table-column>
            <el-table-column prop="diagnosis" label="诊断摘要" min-width="180" />
            <el-table-column label="行操作" width="190" fixed="right">
              <template #default="{ row }">
                <RowActionGroup :actions="rowActions" :row="row" @action="handleRowAction" />
              </template>
            </el-table-column>
          </DataTablePanel>

          <div class="utility-demo-grid">
            <section class="utility-demo-item">
              <h4>PaginationBar 独立分页</h4>
              <PaginationBar
                v-model:page="simplePage"
                v-model:page-size="simplePageSize"
                :total="128"
                small
              />
            </section>
            <section class="utility-demo-item">
              <h4>EmptyState 空状态</h4>
              <EmptyState
                compact
                title="暂无候诊患者"
                description="当前筛选条件下没有可展示记录"
                action-text="新增登记"
                show-refresh
                @action="lastActionText = '点击了空状态主操作'"
                @refresh="lastActionText = '点击了空状态刷新'"
              />
            </section>
            <section class="utility-demo-item">
              <h4>StatusDisplay 状态集合</h4>
              <div class="status-demo-list">
                <StatusDisplay
                  v-for="option in statusOptions"
                  :key="option.value"
                  :value="option.value"
                  :options="statusOptions"
                  show-description
                />
              </div>
            </section>
          </div>

          <el-alert
            v-if="lastActionText"
            :title="lastActionText"
            type="info"
            show-icon
            :closable="false"
          />
        </section>
      </el-tab-pane>

      <el-tab-pane label="C09–C11 侧栏详情外壳" name="shells">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>侧栏外壳、表单分区与详情面板</h3>
              <p>展示 C09 作为基础侧栏容器时的标题、搜索、滚动主体、固定底部和折叠能力。</p>
            </div>
            <el-switch v-model="shellCollapsed" active-text="折叠侧栏" />
          </div>

          <div class="shell-demo-layout">
            <SidePanelShell
              v-model:collapsed="shellCollapsed"
              title="C09 侧栏外壳"
              :width="300"
              :empty="shellRecords.length === 0"
              empty-description="暂无侧栏记录"
              collapsible
            >
              <template #search>
                <el-input v-model="shellKeyword" placeholder="搜索侧栏内容" clearable />
              </template>

              <div class="shell-record-list">
                <button
                  v-for="item in filteredShellRecords"
                  :key="item.key"
                  type="button"
                  class="shell-record-item"
                  @click="lastActionText = `侧栏选择：${item.title}`"
                >
                  <strong>{{ item.title }}</strong>
                  <span>{{ item.description }}</span>
                </button>
              </div>

              <template #footer> 侧栏底部固定区 · {{ filteredShellRecords.length }} 条 </template>
            </SidePanelShell>

            <DetailPanel title="C11 详情面板" :readonly="readonly">
              <div class="shell-detail-content">
                <FormSection :min-table-height="220">
                  <template #filter>
                    <div class="detail-toolbar">
                      <strong>C10 表单分区</strong>
                      <StatusDisplay value="诊疗中" :options="statusOptions" />
                    </div>
                  </template>
                  <template #table>
                    <el-form label-width="90px">
                      <el-row :gutter="16">
                        <el-col :span="12">
                          <el-form-item label="分区字段">
                            <el-input model-value="页面提供字段" readonly />
                          </el-form-item>
                        </el-col>
                        <el-col :span="12">
                          <el-form-item label="只读状态">
                            <el-input :model-value="readonly ? '只读' : '编辑中'" readonly />
                          </el-form-item>
                        </el-col>
                      </el-row>
                    </el-form>
                  </template>
                </FormSection>

                <EmptyState
                  compact
                  title="C09 空状态也由外壳承载"
                  description="侧栏内容为空时由 SidePanelShell 统一显示 emptyDescription"
                />
              </div>
            </DetailPanel>
          </div>
        </section>
      </el-tab-pane>

      <el-tab-pane label="C12 摘要组件" name="summary">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>DetailSummary 详情摘要</h3>
              <p>支持响应式列数、跨列、空值占位、加载/空状态，以及标题和值插槽。</p>
            </div>
            <el-radio-group v-model="summaryColumns" size="small">
              <el-radio-button :value="2">两列</el-radio-button>
              <el-radio-button :value="3">三列</el-radio-button>
              <el-radio-button :value="4">四列</el-radio-button>
            </el-radio-group>
          </div>

          <DetailSummary title="患者基本信息" :items="patientSummary" :column="summaryColumns">
            <template #extra>
              <el-tag type="success">资料完整</el-tag>
            </template>
            <template #value-status="{ item }">
              <el-tag type="primary" effect="plain">{{ item.value }}</el-tag>
            </template>
          </DetailSummary>

          <div class="summary-variants">
            <DetailSummary title="无边框摘要" :items="compactSummary" :column="2" :border="false" />
            <DetailSummary title="空状态" :items="[]" :column="2" />
          </div>
        </section>
      </el-tab-pane>

      <el-tab-pane label="C13–C14 主从详情" name="master-detail">
        <section class="demo-section">
          <div class="section-heading">
            <div>
              <h3>分类导航、记录侧栏与详情联动</h3>
              <p>左侧选择业务分类，中间选择记录，右侧展示摘要和明细，所有选中状态均由页面受控。</p>
            </div>
            <el-tag effect="plain">SideNavigator + RecordSidebar + DetailPanel</el-tag>
          </div>

          <div class="master-detail-demo">
            <SideNavigator
              v-model="selectedCategoryKey"
              v-model:collapsed="navigatorCollapsed"
              v-model:keyword="navigatorKeyword"
              title="病历分类"
              :items="categoryNodes"
              :width="240"
              default-expand-all
              collapsible
              @select="selectedCategoryLabel = $event.label"
            />

            <RecordSidebar
              v-model="selectedRecordKey"
              v-model:collapsed="recordSidebarCollapsed"
              v-model:keyword="recordKeyword"
              title="就诊记录"
              :items="filteredRecords"
              :width="300"
              collapsible
            >
              <template #footer="{ count }">
                当前分类：{{ selectedCategoryLabel }} · {{ count }} 条
              </template>
            </RecordSidebar>

            <DetailPanel
              :title="`${selectedRecord.title} · 就诊详情`"
              :readonly="readonly"
              @edit="readonly = false"
              @save="readonly = true"
              @cancel="readonly = true"
            >
              <div class="detail-content">
                <DetailSummary :items="selectedRecordSummary" :column="2">
                  <template #value-status="{ item }">
                    <el-tag type="success" effect="plain">{{ item.value }}</el-tag>
                  </template>
                </DetailSummary>

                <FormSection :min-table-height="280">
                  <template #filter>
                    <div class="detail-toolbar">
                      <strong>诊疗明细</strong>
                      <el-segmented v-model="detailFilter" :options="['全部', '医嘱', '检查']" />
                    </div>
                  </template>
                  <template #table>
                    <el-table :data="filteredDetailRows" border height="100%">
                      <el-table-column prop="date" label="日期" width="120" />
                      <el-table-column prop="type" label="类型" width="100" />
                      <el-table-column prop="content" label="诊疗内容" min-width="220" />
                      <el-table-column prop="doctor" label="执行人" width="110" />
                    </el-table>
                  </template>
                </FormSection>
              </div>
            </DetailPanel>
          </div>
        </section>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup lang="ts">
import { computed, reactive, ref } from 'vue'
import { Download, Plus, Refresh, Upload } from '@element-plus/icons-vue'
import ActionToolbar from '@/components/common/ActionToolbar.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import DateRangeField from '@/components/common/DateRangeField.vue'
import DetailPanel from '@/components/common/DetailPanel.vue'
import DetailSummary from '@/components/common/DetailSummary.vue'
import EmptyState from '@/components/common/EmptyState.vue'
import FormSection from '@/components/common/FormSection.vue'
import PaginationBar from '@/components/common/PaginationBar.vue'
import RecordSidebar from '@/components/common/RecordSidebar.vue'
import RowActionGroup from '@/components/common/RowActionGroup.vue'
import SearchPanel from '@/components/common/SearchPanel.vue'
import SideNavigator from '@/components/common/SideNavigator.vue'
import SidePanelShell from '@/components/common/SidePanelShell.vue'
import StatusDisplay from '@/components/common/StatusDisplay.vue'
import type {
  ActionToolbarItem,
  DetailSummaryItem,
  RecordSidebarItem,
  RowActionItem,
  SideNavigatorNode,
  StatusDisplayOption
} from '@/components/common/type'

interface PatientRow {
  recordNo: string
  name: string
  department: string
  doctor: string
  date: string
  status: string
  diagnosis: string
}

const componentOverview = [
  { code: 'C01', name: 'SearchPanel', description: '查询区外壳' },
  { code: 'C02', name: 'DateRangeField', description: '日期范围输入' },
  { code: 'C03', name: 'DataTablePanel', description: '表格与分页容器' },
  { code: 'C04', name: 'PaginationBar', description: '独立受控分页' },
  { code: 'C05', name: 'EmptyState', description: '空状态展示' },
  { code: 'C06', name: 'ActionToolbar', description: '页面/批量操作栏' },
  { code: 'C07', name: 'RowActionGroup', description: '表格行操作组' },
  { code: 'C08', name: 'StatusDisplay', description: '统一状态展示' },
  { code: 'C09', name: 'SidePanelShell', description: '侧栏基础外壳' },
  { code: 'C10', name: 'FormSection', description: '筛选与表格分区' },
  { code: 'C11', name: 'DetailPanel', description: '详情页外壳' },
  { code: 'C12', name: 'DetailSummary', description: '摘要信息' },
  { code: 'C13', name: 'RecordSidebar', description: '记录选择侧栏' },
  { code: 'C14', name: 'SideNavigator', description: '分类树导航' }
]

const activeTab = ref('query')
const queryCollapsed = ref(false)
const tablePage = ref(1)
const tablePageSize = ref(10)
const actionTablePage = ref(1)
const actionTablePageSize = ref(10)
const simplePage = ref(1)
const simplePageSize = ref(10)
const selectedPatients = ref<PatientRow[]>([])
const lastActionText = ref('')
const shellCollapsed = ref(false)
const shellKeyword = ref('')
const searchModel = reactive({ keyword: '', status: '' })

const patients: PatientRow[] = [
  {
    recordNo: 'MR20260719001',
    name: '张明',
    department: '呼吸内科',
    doctor: '陈医生',
    date: '2026-07-19',
    status: '诊疗中',
    diagnosis: '上呼吸道感染'
  },
  {
    recordNo: 'MR20260719002',
    name: '李华',
    department: '普外科',
    doctor: '周医生',
    date: '2026-07-19',
    status: '待诊',
    diagnosis: '腹部不适待查'
  },
  {
    recordNo: 'MR20260718006',
    name: '王芳',
    department: '儿科',
    doctor: '刘医生',
    date: '2026-07-18',
    status: '已完成',
    diagnosis: '常规复诊'
  },
  {
    recordNo: 'MR20260718009',
    name: '赵敏',
    department: '心内科',
    doctor: '吴医生',
    date: '2026-07-18',
    status: '已完成',
    diagnosis: '高血压随访'
  }
]

const filteredPatients = computed(() => {
  const keyword = searchModel.keyword.trim()
  return patients.filter(
    (item) =>
      (!keyword || item.name.includes(keyword) || item.recordNo.includes(keyword)) &&
      (!searchModel.status || item.status === searchModel.status)
  )
})

function handleSearch() {
  tablePage.value = 1
}

function resetSearch() {
  searchModel.keyword = ''
  searchModel.status = ''
  tablePage.value = 1
}

const statusOptions: StatusDisplayOption[] = [
  { value: '待诊', label: '待诊', type: 'warning', description: '等待接诊' },
  { value: '诊疗中', label: '诊疗中', type: 'primary', description: '医生处理中' },
  { value: '已完成', label: '已完成', type: 'success', description: '诊疗闭环' }
]

const toolbarActions: ActionToolbarItem[] = [
  { key: 'create', label: '新增', type: 'primary', icon: Plus },
  { key: 'refresh', label: '刷新', icon: Refresh },
  { key: 'export', label: '导出', icon: Download },
  { key: 'import', label: '导入', icon: Upload },
  { key: 'archive', label: '归档', type: 'warning', disabled: true }
]

const rowActions: RowActionItem[] = [
  { key: 'view', label: '查看', type: 'primary' },
  { key: 'edit', label: '编辑' },
  { key: 'print', label: '打印' },
  { key: 'void', label: '作废', type: 'danger', divided: true }
]

const shellRecords = [
  { key: 'search', title: '搜索插槽', description: '顶部搜索区由页面传入' },
  { key: 'body', title: '滚动主体', description: '内容超高时只滚动中间区域' },
  { key: 'footer', title: '固定底部', description: '统计和按钮可固定在底部' }
]

const filteredShellRecords = computed(() => {
  const keyword = shellKeyword.value.trim()
  return keyword
    ? shellRecords.filter((item) =>
        [item.title, item.description].some((value) => value.includes(keyword))
      )
    : shellRecords
})

function handleToolbarAction(action: ActionToolbarItem) {
  lastActionText.value = `工具栏动作：${action.label}`
}

function handleRowAction(action: RowActionItem, row: unknown) {
  const patient = row as PatientRow
  lastActionText.value = `行操作：${patient.name} - ${action.label}`
}

const summaryColumns = ref(4)
const patientSummary: DetailSummaryItem[] = [
  { key: 'name', label: '患者姓名', value: '张明' },
  { key: 'gender', label: '性别', value: '男' },
  { key: 'age', label: '年龄', value: 42 },
  { key: 'status', label: '就诊状态', value: '诊疗中' },
  { key: 'recordNo', label: '病历号', value: 'MR20260719001' },
  { key: 'department', label: '就诊科室', value: '呼吸内科' },
  { key: 'doctor', label: '接诊医生', value: '陈医生' },
  { key: 'allergy', label: '过敏史', value: null }
]
const compactSummary: DetailSummaryItem[] = [
  { key: 'createdAt', label: '建档时间', value: '2026-07-19 09:30' },
  { key: 'source', label: '数据来源', value: '门诊工作站' },
  { key: 'remark', label: '备注', value: '摘要项支持跨列显示', span: 2 }
]

const categoryNodes: SideNavigatorNode[] = [
  {
    key: 'medical-record',
    label: '病历文书',
    children: [
      { key: 'outpatient', label: '门诊病历' },
      { key: 'inpatient', label: '住院记录' },
      { key: 'discharge', label: '出院小结' }
    ]
  },
  {
    key: 'clinical-data',
    label: '临床资料',
    children: [
      { key: 'orders', label: '医嘱记录' },
      { key: 'exam', label: '检查报告' },
      { key: 'lab', label: '检验报告' }
    ]
  }
]

const records: RecordSidebarItem[] = [
  {
    key: 1,
    title: '2026-07-19 门诊',
    subtitle: '呼吸内科 · 陈医生',
    description: '上呼吸道感染',
    status: '诊疗中'
  },
  {
    key: 2,
    title: '2026-06-03 复诊',
    subtitle: '呼吸内科 · 陈医生',
    description: '恢复情况良好',
    status: '已完成'
  },
  {
    key: 3,
    title: '2026-04-21 体检',
    subtitle: '健康管理中心 · 刘医生',
    description: '常规健康检查',
    status: '已完成'
  }
]

const selectedCategoryKey = ref<string | number | null>('outpatient')
const selectedCategoryLabel = ref('门诊病历')
const navigatorKeyword = ref('')
const navigatorCollapsed = ref(false)
const selectedRecordKey = ref<string | number | null>(1)
const recordKeyword = ref('')
const recordSidebarCollapsed = ref(false)
const readonly = ref(true)
const detailFilter = ref('全部')

const filteredRecords = computed(() => {
  const keyword = recordKeyword.value.trim()
  return keyword
    ? records.filter((item) =>
        [item.title, item.subtitle, item.description].some((value) => value?.includes(keyword))
      )
    : records
})

const selectedRecord = computed(
  () => records.find((item) => item.key === selectedRecordKey.value) ?? records[0]!
)

const selectedRecordSummary = computed<DetailSummaryItem[]>(() => [
  { key: 'record', label: '就诊记录', value: selectedRecord.value.title },
  { key: 'department', label: '科室/医生', value: selectedRecord.value.subtitle },
  { key: 'status', label: '状态', value: selectedRecord.value.status }
])

const detailRows = [
  { date: '2026-07-19', type: '医嘱', content: '口服药物治疗，每日三次', doctor: '陈医生' },
  { date: '2026-07-19', type: '检查', content: '血常规检查', doctor: '检验科' },
  { date: '2026-07-19', type: '医嘱', content: '三日后门诊复查', doctor: '陈医生' }
]

const filteredDetailRows = computed(() =>
  detailFilter.value === '全部'
    ? detailRows
    : detailRows.filter((item) => item.type === detailFilter.value)
)
</script>

<style scoped>
.common-demo-page {
  box-sizing: border-box;
  min-height: calc(100vh - var(--sample-header-height) - 20px);
  padding: 4px;
}

.demo-header,
.section-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
}

.demo-header h2,
.section-heading h3 {
  margin: 0;
  color: var(--el-text-color-primary);
}

.demo-header p,
.section-heading p {
  margin: 6px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.component-overview {
  display: grid;
  grid-template-columns: repeat(4, minmax(150px, 1fr));
  gap: 10px;
  margin: 16px 0;
}

.overview-item {
  display: flex;
  flex-direction: column;
  gap: 4px;
  padding: 12px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.overview-item strong {
  color: var(--el-text-color-primary);
  font-size: 13px;
}

.overview-item > span:last-child {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.overview-code {
  color: var(--el-color-primary);
  font-size: 12px;
  font-weight: 700;
}

.demo-tabs {
  padding: 0 16px 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.demo-section {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.summary-variants {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 16px;
}

.utility-demo-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 16px;
}

.shell-demo-layout {
  display: flex;
  gap: 12px;
  height: 520px;
  min-width: 0;
  min-height: 0;
}

.shell-record-list {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.shell-record-item {
  display: flex;
  flex-direction: column;
  gap: 4px;
  width: 100%;
  padding: 10px 12px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  color: var(--el-text-color-regular);
  font: inherit;
  text-align: left;
  cursor: pointer;
}

.shell-record-item:hover {
  border-color: var(--el-color-primary-light-7);
  color: var(--el-color-primary);
}

.shell-record-item span {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.shell-detail-content {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.utility-demo-item {
  min-width: 0;
  padding: 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.utility-demo-item h4 {
  margin: 0 0 12px;
  color: var(--el-text-color-primary);
  font-size: 14px;
}

.status-demo-list {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.master-detail-demo {
  display: flex;
  gap: 12px;
  height: 620px;
  min-width: 0;
  min-height: 0;
}

.detail-content {
  display: flex;
  flex-direction: column;
  gap: 12px;
  min-height: 100%;
}

.detail-toolbar {
  display: flex;
  align-items: center;
  justify-content: space-between;
}

:deep(.query-shell .el-form-item) {
  margin-bottom: 12px;
}

@media (max-width: 1400px) {
  .component-overview {
    grid-template-columns: repeat(3, minmax(150px, 1fr));
  }
}

@media (max-width: 900px) {
  .component-overview,
  .utility-demo-grid,
  .summary-variants {
    grid-template-columns: 1fr;
  }

  .master-detail-demo {
    height: auto;
    min-height: 600px;
    overflow-x: auto;
  }

  .shell-demo-layout {
    height: auto;
    min-height: 520px;
    overflow-x: auto;
  }
}
</style>
