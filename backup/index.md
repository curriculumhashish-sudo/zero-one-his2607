```vue
<template>
  <div class="warehouse-demo-page">
    <header class="demo-header">
      <div>
        <h3>采购入库工作站</h3>
      </div>
    </header>

    <!-- ==================== 状态控制面板 ==================== -->
    <section class="control-bar">
      <div class="control-bar__group">
        <span class="control-bar__label">状态切换：</span>
        <el-switch v-model="queueCollapsed" active-text="收起队列" inactive-text="展开队列" />
        <el-switch v-model="loading" active-text="加载中" inactive-text="空闲" />
        <el-switch v-model="readonly" active-text="只读" inactive-text="编辑" />
      </div>
      <div class="control-bar__group">
        <el-radio-group v-model="queueWidth" size="small">
          <el-radio-button :value="280" label="280px">窄队列</el-radio-button>
          <el-radio-button :value="360" label="360px">宽队列</el-radio-button>
          <el-radio-button :value="420" label="420px">超宽</el-radio-button>
        </el-radio-group>
      </div>
    </section>

    <!-- ==================== 单据工作站 ==================== -->
    <doc-shell
      v-model:queue-collapsed="queueCollapsed"
      :queue-width="queueWidth"
      :has-selection="!!selectedDoc"
      :loading="loading"
      :readonly="readonly"
      :collapsible="true"
      :gap="12"
      @collapse-change="onCollapseChange"
    >
      <!-- ===== Queue Header：单据列表标题 + 新建按钮 ===== -->
      <template #queue-header>
        <span class="queue-title">采购入库单</span>
        <el-button type="primary" size="small" :icon="Plus" link @click="createNew">
          新建
        </el-button>
      </template>

      <!-- ===== Queue Body：单据列表 ===== -->
      <template #queue>
        <div class="queue-list" role="listbox" aria-label="采购入库单列表">
          <button
            v-for="doc in documents"
            :key="doc.id"
            type="button"
            class="queue-item"
            :class="{ 'is-active': doc.id === selectedDoc?.id }"
            :aria-selected="doc.id === selectedDoc?.id"
            @click="selectDocument(doc)"
          >
            <span class="queue-item__main">
              <strong class="queue-item__title">{{ doc.code }}</strong>
              <span class="queue-item__subtitle">{{ doc.supplier }}</span>
              <span class="queue-item__desc">{{ doc.date }}</span>
            </span>
            <el-tag :type="statusType(doc.status)" size="small" effect="plain">
              {{ doc.status }}
            </el-tag>
          </button>
        </div>
      </template>

      <!-- ===== Main Header：单据标题 + 状态标签 + 操作 ===== -->
      <template #main-header>
        <div class="main-header-content">
          <div class="main-header__info">
            <strong class="main-header__title">
              {{ isNewDoc ? '新建采购入库单' : selectedDoc?.code }}
            </strong>
            <el-tag
              v-if="selectedDoc && !isNewDoc"
              :type="statusType(selectedDoc.status)"
              effect="plain"
              size="small"
            >
              {{ selectedDoc.status }}
            </el-tag>
            <el-tag v-if="isNewDoc" type="primary" effect="plain" size="small">新建</el-tag>
          </div>
          <div class="main-header__actions">
            <el-button v-if="!isNewDoc" text :icon="Edit" @click="readonly = false">编辑</el-button>
            <el-button text type="danger" :icon="Delete" @click="deleteDoc">删除</el-button>
          </div>
        </div>
      </template>

      <!-- ===== Main Content：单据详情 / 编辑表单 ===== -->
      <template #main-content>
        <div class="edit-area">
          <!-- 单据基础信息 -->
          <fieldset class="edit-area__block" :disabled="readonly">
            <legend>基础信息</legend>
            <el-row :gutter="16">
              <el-col :span="12">
                <el-form-item label="入库单号">
                  <el-input :model-value="currentForm.code" disabled />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="入库日期">
                  <el-date-picker
                    :model-value="currentForm.date"
                    type="date"
                    placeholder="选择日期"
                    style="width: 100%"
                  />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="供应商">
                  <el-select :model-value="currentForm.supplier" style="width: 100%">
                    <el-option label="华药集团" value="华药集团" />
                    <el-option label="九州通医药" value="九州通医药" />
                    <el-option label="国药控股" value="国药控股" />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="入库仓库">
                  <el-select :model-value="currentForm.warehouse" style="width: 100%">
                    <el-option label="西药库" value="西药库" />
                    <el-option label="中药库" value="中药库" />
                    <el-option label="器械库" value="器械库" />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="24">
                <el-form-item label="备注">
                  <el-input
                    :model-value="currentForm.remark"
                    type="textarea"
                    :rows="2"
                    placeholder="备注信息（可选）"
                  />
                </el-form-item>
              </el-col>
            </el-row>
          </fieldset>

          <!-- 明细列表 -->
          <fieldset class="edit-area__block" :disabled="readonly">
            <legend>入库明细</legend>
            <div class="edit-area__table-toolbar">
              <el-button size="small" :icon="Plus" @click="addDetailRow">添加行</el-button>
              <el-button
                size="small"
                :icon="Delete"
                :disabled="readonly || selectedDetails.length === 0"
                @click="removeDetailRows"
              >
                删除选中行
              </el-button>
            </div>
            <el-table
              :data="currentForm.details"
              border
              size="small"
              @selection-change="onDetailSelectionChange"
            >
              <el-table-column type="selection" width="44" />
              <el-table-column prop="drugCode" label="药品编码" width="140">
                <template #default="{ row }">
                  <el-input v-if="!readonly" v-model="row.drugCode" size="small" />
                  <span v-else>{{ row.drugCode }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="drugName" label="药品名称" min-width="160">
                <template #default="{ row }">
                  <el-input v-if="!readonly" v-model="row.drugName" size="small" />
                  <span v-else>{{ row.drugName }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="spec" label="规格" width="120">
                <template #default="{ row }">
                  <el-input v-if="!readonly" v-model="row.spec" size="small" />
                  <span v-else>{{ row.spec }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="unit" label="单位" width="80">
                <template #default="{ row }">
                  <el-select v-if="!readonly" v-model="row.unit" size="small">
                    <el-option label="盒" value="盒" />
                    <el-option label="瓶" value="瓶" />
                    <el-option label="支" value="支" />
                  </el-select>
                  <span v-else>{{ row.unit }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="quantity" label="数量" width="100">
                <template #default="{ row }">
                  <el-input-number
                    v-if="!readonly"
                    v-model="row.quantity"
                    :min="1"
                    size="small"
                    controls-position="right"
                    style="width: 100%"
                  />
                  <span v-else>{{ row.quantity }}</span>
                </template>
              </el-table-column>
              <el-table-column prop="price" label="单价(元)" width="120">
                <template #default="{ row }">
                  <el-input-number
                    v-if="!readonly"
                    v-model="row.price"
                    :min="0"
                    :precision="2"
                    size="small"
                    controls-position="right"
                    style="width: 100%"
                  />
                  <span v-else>{{ row.price?.toFixed(2) }}</span>
                </template>
              </el-table-column>
              <el-table-column label="金额(元)" width="120">
                <template #default="{ row }">
                  {{ (row.quantity * row.price).toFixed(2) }}
                </template>
              </el-table-column>
            </el-table>
          </fieldset>
        </div>
      </template>

      <!-- ===== Main Footer：操作按钮 ===== -->
      <template #main-footer>
        <span class="footer-summary">
          共 <strong>{{ currentForm.details.length }}</strong> 条明细 · 合计
          <strong class="footer-amount">{{ totalAmount.toFixed(2) }}</strong> 元
        </span>
        <el-button @click="cancelEdit">取消</el-button>
        <el-button v-if="isNewDoc" type="primary" :loading="loading" @click="simulateSave">
          保存并入库
        </el-button>
        <template v-else>
          <el-button v-if="!readonly" type="primary" :loading="loading" @click="simulateSave">
            保存
          </el-button>
        </template>
      </template>

      <!-- ===== Empty：无选中单据 ===== -->
      <template #empty>
        <div class="empty-custom">
          <el-empty
            description="请在左侧选择一份采购入库单，或点击「新建」创建单据"
            :image-size="120"
          />
          <el-button type="primary" :icon="Plus" @click="createNew">新建采购入库单</el-button>
        </div>
      </template>
    </doc-shell>

    <!-- ==================== 事件日志 ==================== -->
    <el-alert
      v-if="eventLog"
      :title="eventLog"
      type="info"
      show-icon
      :closable="false"
      class="demo-log"
    />
  </div>
</template>

<script setup lang="ts">
import { computed, reactive, ref } from 'vue'
import { Delete, Edit, Plus } from '@element-plus/icons-vue'
import DocShell from '@/components/warehouse/DocumentWorkstationShell.vue'

// ==================== 类型 ====================
type DocStatus = '待审核' | '已入库' | '已作废'

interface DetailRow {
  drugCode: string
  drugName: string
  spec: string
  unit: string
  quantity: number
  price: number
}

interface InboundDoc {
  id: number
  code: string
  supplier: string
  warehouse: string
  date: string
  status: DocStatus
  remark: string
  details: DetailRow[]
}

// ==================== 示例数据 ====================
const mockDetailsA: DetailRow[] = [
  {
    drugCode: 'YP001234',
    drugName: '阿莫西林胶囊',
    spec: '0.25g×24粒',
    unit: '盒',
    quantity: 200,
    price: 12.5
  },
  {
    drugCode: 'YP005678',
    drugName: '布洛芬缓释胶囊',
    spec: '0.3g×20粒',
    unit: '盒',
    quantity: 150,
    price: 28.0
  },
  {
    drugCode: 'YP009012',
    drugName: '头孢克肟分散片',
    spec: '50mg×12片',
    unit: '盒',
    quantity: 100,
    price: 45.6
  }
]

const mockDetailsB: DetailRow[] = [
  {
    drugCode: 'YP002345',
    drugName: '蒙脱石散',
    spec: '3g×10袋',
    unit: '盒',
    quantity: 300,
    price: 18.9
  },
  {
    drugCode: 'YP006789',
    drugName: '盐酸氨溴索口服液',
    spec: '100ml',
    unit: '瓶',
    quantity: 80,
    price: 32.0
  }
]

const mockDetailsC: DetailRow[] = [
  {
    drugCode: 'YP003456',
    drugName: '氯雷他定片',
    spec: '10mg×6片',
    unit: '盒',
    quantity: 120,
    price: 22.3
  },
  {
    drugCode: 'YP007890',
    drugName: '奥美拉唑肠溶胶囊',
    spec: '20mg×14粒',
    unit: '盒',
    quantity: 90,
    price: 56.8
  },
  {
    drugCode: 'YP010111',
    drugName: '葡萄糖注射液',
    spec: '500ml',
    unit: '瓶',
    quantity: 200,
    price: 6.5
  }
]

const defaultDocuments: InboundDoc[] = [
  {
    id: 1,
    code: 'RK-2026-00001',
    supplier: '华药集团',
    warehouse: '西药库',
    date: '2026-07-20',
    status: '已入库',
    remark: '月度常规采购',
    details: mockDetailsA
  },
  {
    id: 2,
    code: 'RK-2026-00002',
    supplier: '九州通医药',
    warehouse: '中药库',
    date: '2026-07-19',
    status: '待审核',
    remark: '',
    details: mockDetailsB
  },
  {
    id: 3,
    code: 'RK-2026-00003',
    supplier: '国药控股',
    warehouse: '器械库',
    date: '2026-07-18',
    status: '已作废',
    remark: '供应商取消发货',
    details: mockDetailsC
  }
]

const emptyDetailRow = (): DetailRow => ({
  drugCode: '',
  drugName: '',
  spec: '',
  unit: '盒',
  quantity: 1,
  price: 0
})

const blankForm = (): Omit<InboundDoc, 'id'> => ({
  code: `RK-2026-${String(Date.now()).slice(-5)}`,
  supplier: '',
  warehouse: '',
  date: new Date().toISOString().slice(0, 10),
  status: '待审核',
  remark: '',
  details: [emptyDetailRow()]
})

// ==================== 状态 ====================
const queueWidth = ref<string | number>(360)
const queueCollapsed = ref(false)
const loading = ref(false)
const readonly = ref(true)
const eventLog = ref('')

const documents = ref<InboundDoc[]>([...defaultDocuments])
const selectedDoc = ref<InboundDoc | null>(null)
const isNewDoc = ref(false)
const selectedDetails = ref<DetailRow[]>([])

/** 当前展示/编辑中的表单数据（选中单据的副本或新建的空白单据） */
const currentForm = reactive<Omit<InboundDoc, 'id'>>(blankForm())

// ==================== 计算属性 ====================
const totalAmount = computed(() =>
  currentForm.details.reduce((sum, row) => sum + row.quantity * row.price, 0)
)

// ==================== 方法 ====================
function statusType(status: DocStatus) {
  const map: Record<DocStatus, 'success' | 'warning' | 'danger'> = {
    已入库: 'success',
    待审核: 'warning',
    已作废: 'danger'
  }
  return map[status]
}

function fillForm(doc: InboundDoc) {
  currentForm.code = doc.code
  currentForm.supplier = doc.supplier
  currentForm.warehouse = doc.warehouse
  currentForm.date = doc.date
  currentForm.status = doc.status
  currentForm.remark = doc.remark
  currentForm.details = doc.details.map((d) => ({ ...d }))
}

function selectDocument(doc: InboundDoc) {
  isNewDoc.value = false
  selectedDoc.value = doc
  readonly.value = true
  fillForm(doc)
  eventLog.value = `选中单据：${doc.code}`
}

function createNew() {
  isNewDoc.value = true
  selectedDoc.value = null
  readonly.value = false
  const blank = blankForm()
  Object.assign(currentForm, blank)
  eventLog.value = '新建采购入库单'
}

function addDetailRow() {
  currentForm.details.push(emptyDetailRow())
}

function removeDetailRows() {
  if (selectedDetails.value.length === 0) return
  const keys = new Set(selectedDetails.value.map((d) => d.drugCode))
  currentForm.details = currentForm.details.filter(
    (d) => !keys.has(d.drugCode) || d.drugCode === ''
  )
  selectedDetails.value = []
  eventLog.value = `已移除 ${selectedDetails.value.length || keys.size} 行明细`
}

function onDetailSelectionChange(rows: DetailRow[]) {
  selectedDetails.value = rows
}

function simulateSave() {
  loading.value = true
  eventLog.value = '保存中...'

  setTimeout(() => {
    loading.value = false
    readonly.value = true
    eventLog.value = `保存成功：${currentForm.code}`
  }, 800)
}

function cancelEdit() {
  if (isNewDoc.value) {
    // 放弃新建：回到空态
    isNewDoc.value = false
    selectedDoc.value = null
    eventLog.value = '已放弃新建'
  } else if (selectedDoc.value) {
    // 还原原始数据
    fillForm(selectedDoc.value)
    readonly.value = true
    eventLog.value = '已取消编辑'
  }
}

function deleteDoc() {
  if (!selectedDoc.value || isNewDoc.value) return
  const doc = selectedDoc.value
  documents.value = documents.value.filter((d) => d.id !== doc.id)
  selectedDoc.value = null
  isNewDoc.value = false
  eventLog.value = `已删除单据：${doc.code}`
}

function onCollapseChange(collapsed: boolean) {
  eventLog.value = collapsed ? '队列已收起' : '队列已展开'
}
</script>

<style scoped>
/* ==================== 页面容器 ==================== */
.warehouse-demo-page {
  box-sizing: border-box;
  display: flex;
  flex-direction: column;
  gap: 12px;
  min-height: calc(100vh - var(--sample-header-height) - 20px);
  padding: 4px;
}

.demo-header h2 {
  margin: 0;
  color: var(--el-text-color-primary);
}

.demo-header p {
  margin: 6px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

/* ==================== 控制面板 ==================== */
.control-bar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  padding: 10px 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.control-bar__group {
  display: flex;
  align-items: center;
  gap: 12px;
}

.control-bar__label {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

/* ==================== Queue 区内容样式 ==================== */
.queue-title {
  font-weight: 600;
  font-size: 14px;
  color: var(--el-text-color-primary);
}

.queue-list {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 8px;
}

.queue-item {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 10px;
  width: 100%;
  padding: 10px 12px;
  border: 1px solid transparent;
  border-radius: var(--el-border-radius-base);
  background: transparent;
  color: var(--el-text-color-regular);
  font: inherit;
  text-align: left;
  cursor: pointer;
  transition:
    border-color 0.2s,
    background-color 0.2s,
    color 0.2s;
}

.queue-item:hover {
  border-color: var(--el-color-primary-light-7);
  background: var(--el-color-primary-light-9);
}

.queue-item.is-active {
  border-color: var(--el-color-primary-light-5);
  background: var(--el-color-primary-light-8);
  color: var(--el-color-primary);
}

.queue-item__main {
  display: flex;
  min-width: 0;
  flex-direction: column;
  gap: 4px;
}

.queue-item__title {
  font-size: 14px;
}

.queue-item__subtitle {
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.queue-item__desc {
  color: var(--el-text-color-placeholder);
  font-size: 12px;
}

/* ==================== Main Header 内容样式 ==================== */
.main-header-content {
  display: flex;
  align-items: center;
  justify-content: space-between;
  width: 100%;
}

.main-header__info {
  display: flex;
  align-items: center;
  gap: 8px;
  min-width: 0;
}

.main-header__title {
  font-size: 15px;
  color: var(--el-text-color-primary);
}

.main-header__actions {
  display: flex;
  align-items: center;
  gap: 4px;
  flex-shrink: 0;
}

/* ==================== Main Content 编辑区样式 ==================== */
.edit-area {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.edit-area__block {
  margin: 0;
  padding: 16px 16px 20px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
}

.edit-area__block legend {
  padding: 0 8px;
  font-weight: 600;
  font-size: 13px;
  color: var(--el-text-color-primary);
}

.edit-area__table-toolbar {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-bottom: 10px;
}

/* ==================== Main Footer 内容样式 ==================== */
.footer-summary {
  flex: 1;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.footer-summary strong {
  color: var(--el-text-color-regular);
}

.footer-amount {
  color: var(--el-color-danger) !important;
}

/* ==================== Empty 空态 ==================== */
.empty-custom {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 12px;
}

/* ==================== 事件日志 ==================== */
.demo-log {
  flex-shrink: 0;
}

/* ==================== 全局微调 ==================== */
:deep(.el-form-item) {
  margin-bottom: 14px;
}
</style>
```

