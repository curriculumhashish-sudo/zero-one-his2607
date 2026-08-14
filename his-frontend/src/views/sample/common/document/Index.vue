<script setup lang="ts">
import { computed, ref } from 'vue'

import DescriptionList from '@/components/common/DescriptionList.vue'
import type { DescriptionItem } from '@/components/common/DescriptionList.vue'
import PdfPreviewPanel from '@/components/common/PdfPreviewPanel.vue'
import PrintActionPanel from '@/components/common/PrintActionPanel.vue'
import type {
  PrintActionContext,
  PrinterOption,
  PrintTemplateOption
} from '@/components/common/PrintActionPanel.vue'
import RichTextEditorField from '@/components/common/RichTextEditorField.vue'
import TreeFilterPanel from '@/components/common/TreeFilterPanel.vue'
import type { TreeNodeData, TreeProps } from '@/components/common/TreeFilterPanel.vue'

const activeTab = ref('editor')
const editorReadonly = ref(false)
const editorContent = ref(
  '<h3>门诊病历摘要</h3><p>患者主诉咳嗽三天，无明显发热。</p><p><strong>处理意见：</strong>完善血常规检查，按医嘱用药。</p>'
)
const editorLog = ref('')

const samplePdf =
  'data:application/pdf;base64,JVBERi0xLjQKMSAwIG9iago8PC9UeXBlL0NhdGFsb2cvUGFnZXMgMiAwIFI+PmVuZG9iagoyIDAgb2JqCjw8L1R5cGUvUGFnZXMvS2lkc1szIDAgUl0vQ291bnQgMT4+ZW5kb2JqCjMgMCBvYmoKPDwvVHlwZS9QYWdlL1BhcmVudCAyIDAgUi9NZWRpYUJveFswIDAgNjEyIDc5Ml0+PmVuZG9iagp4cmVmCjAgNAowMDAwMDAwMDAwIDY1NTM1IGYgCjAwMDAwMDAwMDkgMDAwMDAgbiAKMDAwMDAwMDA1OCAwMDAwMCBuIAowMDAwMDAwMTE1IDAwMDAwIG4gCnRyYWlsZXIKPDwvU2l6ZSA0L1Jvb3QgMSAwIFI+PgpzdGFydHhyZWYKMjA0CiUlRU9G'
const pdfSource = ref<string | null>(samplePdf)
const pdfLog = ref('')
const printTemplateId = ref<string | number | null>('outpatient-record')
const printerName = ref<string | null>('clinic-a4')
const printLog = ref('')
const printTemplates: PrintTemplateOption[] = [
  { value: 'outpatient-record', label: '门诊病历模板', description: 'A4 纵向' },
  { value: 'prescription', label: '处方笺模板', description: 'A5 横向' }
]
const printers: PrinterOption[] = [
  { value: 'clinic-a4', label: '诊室 A4 打印机', status: 'online' },
  { value: 'pharmacy', label: '药房打印机', status: 'offline' }
]

function waitForPrintDemo(signal: AbortSignal) {
  return new Promise<void>((resolve, reject) => {
    const timer = window.setTimeout(resolve, 500)
    signal.addEventListener(
      'abort',
      () => {
        window.clearTimeout(timer)
        reject(new DOMException('操作已取消', 'AbortError'))
      },
      { once: true }
    )
  })
}

async function previewDocument(context: PrintActionContext) {
  await waitForPrintDemo(context.signal)
  return `已生成模板 ${context.templateValue} 的预览`
}

async function printDocument(context: PrintActionContext) {
  await waitForPrintDemo(context.signal)
  return `已发送到打印机 ${context.printerValue}`
}

function handlePrintSuccess(type: 'preview' | 'print', result: unknown) {
  printLog.value = `${type === 'preview' ? '预览' : '打印'}成功：${String(result)}`
}

const detailDataset = ref<'patient' | 'order'>('patient')
const patientDetail = {
  recordNo: 'MZ20260721001',
  name: '张明',
  gender: '男',
  age: 42,
  department: '呼吸内科',
  status: '诊疗中',
  allergy: '',
  diagnosis: '上呼吸道感染'
}
const orderDetail = {
  orderNo: 'RK20260721008',
  supplier: '示例医药有限公司',
  warehouse: '西药库',
  operator: '李药师',
  status: '待审核',
  amount: 2860.5,
  remark: '冷链药品单独验收'
}

const patientItems: DescriptionItem[] = [
  { prop: 'recordNo', label: '病历号' },
  { prop: 'name', label: '患者姓名' },
  { prop: 'gender', label: '性别' },
  { prop: 'age', label: '年龄', formatter: (value) => `${value} 岁` },
  { prop: 'department', label: '就诊科室' },
  { prop: 'status', label: '就诊状态' },
  { prop: 'allergy', label: '过敏史' },
  { prop: 'diagnosis', label: '诊断摘要', span: 2 }
]
const orderItems: DescriptionItem[] = [
  { prop: 'orderNo', label: '单据号' },
  { prop: 'supplier', label: '供应商' },
  { prop: 'warehouse', label: '目标库房' },
  { prop: 'operator', label: '经办人' },
  { prop: 'status', label: '审核状态' },
  {
    prop: 'amount',
    label: '单据金额',
    formatter: (value) => `¥${Number(value).toFixed(2)}`
  },
  { prop: 'remark', label: '备注', span: 2 }
]
const currentDetail = computed(() =>
  detailDataset.value === 'patient' ? patientDetail : orderDetail
)
const currentDetailItems = computed(() =>
  detailDataset.value === 'patient' ? patientItems : orderItems
)

const departmentProps: TreeProps = {
  label: 'title',
  children: 'nodes',
  disabled: 'disabled'
}
const departmentTree: TreeNodeData[] = [
  {
    id: 'outpatient',
    title: '门诊科室',
    nodes: [
      { id: 'cardiology', title: '心内科' },
      { id: 'respiratory', title: '呼吸内科' },
      { id: 'surgery', title: '普通外科' }
    ]
  },
  {
    id: 'inpatient',
    title: '住院病区',
    nodes: [
      { id: 'ward-a', title: '内科一病区' },
      { id: 'ward-b', title: '外科二病区' }
    ]
  }
]

const templateProps: TreeProps = {
  label: 'name',
  children: 'items'
}
const templateTree: TreeNodeData[] = [
  {
    code: 'outpatient-record',
    name: '门诊病历',
    items: [
      { code: 'first-visit', name: '初诊病历模板' },
      { code: 'follow-up', name: '复诊病历模板' }
    ]
  },
  {
    code: 'inpatient-record',
    name: '住院文书',
    items: [
      { code: 'admission', name: '入院记录模板' },
      { code: 'course', name: '病程记录模板' },
      { code: 'discharge', name: '出院小结模板' }
    ]
  }
]

const departmentKey = ref<string | number | null>('respiratory')
const departmentKeyword = ref('')
const templateKey = ref<string | number | null>('first-visit')
const templateKeyword = ref('')
const treeLog = ref('')

function handleTreeSelect(data: TreeNodeData, labelField: string) {
  treeLog.value = `当前选择：${String(data[labelField] ?? '')}`
}
</script>

<template>
  <div class="document-demo">
    <header class="document-demo__header">
      <div>
        <h2>C 组文档展示与通用筛选组件</h2>
        <p>展示 C32–C36 的受控数据、状态切换、字段配置和字段映射能力。</p>
      </div>
      <router-link to="/sample/common">
        <el-button>返回公共组件总览</el-button>
      </router-link>
    </header>

    <div class="document-demo__overview">
      <div v-for="item in [
        ['C32', 'RichTextEditorField', '受控富文本字段'],
        ['C33', 'PdfPreviewPanel', 'PDF 预览面板'],
        ['C34', 'PrintActionPanel', '打印操作面板'],
        ['C35', 'DescriptionList', '配置化详情列表'],
        ['C36', 'TreeFilterPanel', '字段可映射树筛选']
      ]" :key="item[0]" class="document-demo__overview-item">
        <span>{{ item[0] }}</span>
        <strong>{{ item[1] }}</strong>
        <small>{{ item[2] }}</small>
      </div>
    </div>

    <el-tabs v-model="activeTab" class="document-demo__tabs">
      <el-tab-pane label="C32 富文本" name="editor">
        <section class="document-demo__section">
          <div class="document-demo__heading">
            <div>
              <h3>富文本编辑与只读复用</h3>
              <p>内容由页面通过 v-model 控制，组件不保存病历或报告。</p>
            </div>
            <el-switch v-model="editorReadonly" active-text="只读模式" />
          </div>
          <RichTextEditorField
            v-model="editorContent"
            :readonly="editorReadonly"
            min-height="260px"
            placeholder="请输入病历或报告内容"
            @focus="editorLog = '编辑器获得焦点'"
            @blur="editorLog = '编辑器失去焦点'"
            @change="editorLog = '页面已接收到内容变化'"
          />
          <el-alert
            :title="editorLog || `当前 HTML 长度：${editorContent.length}`"
            type="info"
            :closable="false"
          />
        </section>
      </el-tab-pane>

      <el-tab-pane label="C33/C34 PDF 与打印" name="pdf">
        <section class="document-demo__section">
          <div class="document-demo__heading">
            <div>
              <h3>PDF 预览与打印操作</h3>
              <p>C33 接收 PDF 数据源；C34 接收模板、打印机以及页面注入的预览/打印函数。</p>
            </div>
            <el-switch
              :model-value="Boolean(pdfSource)"
              active-text="提供 PDF"
              @update:model-value="pdfSource = $event ? samplePdf : null"
            />
          </div>

          <div class="document-demo__pdf-grid">
            <div>
              <h4>C33 PdfPreviewPanel</h4>
              <PdfPreviewPanel
                :source="pdfSource"
                file-name="检查报告示例.pdf"
                height="420px"
                :openable="false"
                @load="pdfLog = 'C33 PDF 加载完成'"
                @retry="pdfLog = 'C33 执行重新加载'"
              />
            </div>
            <div>
              <h4>C34 PrintActionPanel</h4>
              <PrintActionPanel
                v-model:template-value="printTemplateId"
                v-model:printer-value="printerName"
                :templates="printTemplates"
                :printers="printers"
                :preview="previewDocument"
                :print="printDocument"
                @success="handlePrintSuccess"
              />
              <p v-if="printLog" class="document-demo__log">{{ printLog }}</p>
            </div>
          </div>
          <p v-if="pdfLog" class="document-demo__log">{{ pdfLog }}</p>
        </section>
      </el-tab-pane>

      <el-tab-pane label="C35 详情列表" name="description">
        <section class="document-demo__section">
          <div class="document-demo__heading">
            <div>
              <h3>同一组件展示不同业务实体</h3>
              <p>字段、格式化、跨列和空值占位全部由页面配置。</p>
            </div>
            <el-radio-group v-model="detailDataset" size="small">
              <el-radio-button value="patient">患者详情</el-radio-button>
              <el-radio-button value="order">入库单详情</el-radio-button>
            </el-radio-group>
          </div>
          <DescriptionList
            :data="currentDetail"
            :items="currentDetailItems"
            :title="detailDataset === 'patient' ? '患者基本信息' : '入库单基本信息'"
            :column="2"
            empty-text="未记录"
          >
            <template #value-status="{ value }">
              <el-tag type="primary" effect="plain">{{ value }}</el-tag>
            </template>
          </DescriptionList>
        </section>
      </el-tab-pane>

      <el-tab-pane label="C36 树筛选" name="tree">
        <section class="document-demo__section">
          <div class="document-demo__heading">
            <div>
              <h3>两种字段结构共用树筛选</h3>
              <p>科室树使用 id/title/nodes，模板树使用 code/name/items。</p>
            </div>
            <el-tag effect="plain">字段映射验证</el-tag>
          </div>
          <div class="document-demo__tree-grid">
            <div class="document-demo__tree-card">
              <h4>科室树</h4>
              <TreeFilterPanel
                v-model:current-key="departmentKey"
                v-model:filter-text="departmentKeyword"
                :data="departmentTree"
                :props="departmentProps"
                node-key="id"
                default-expand-all
                @select="handleTreeSelect($event, 'title')"
              />
            </div>
            <div class="document-demo__tree-card">
              <h4>病历模板树</h4>
              <TreeFilterPanel
                v-model:current-key="templateKey"
                v-model:filter-text="templateKeyword"
                :data="templateTree"
                :props="templateProps"
                node-key="code"
                default-expand-all
                @select="handleTreeSelect($event, 'name')"
              >
                <template #node="{ data }">
                  <span class="document-demo__template-node">
                    <span>{{ data.name }}</span>
                    <el-tag v-if="!data.items" size="small" effect="plain">模板</el-tag>
                  </span>
                </template>
              </TreeFilterPanel>
            </div>
          </div>
          <p v-if="treeLog" class="document-demo__log">{{ treeLog }}</p>
        </section>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<style scoped>
.document-demo {
  display: flex;
  flex-direction: column;
  gap: 16px;
  padding: 16px;
}

.document-demo__header,
.document-demo__heading {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: 16px;
}

.document-demo h2,
.document-demo h3,
.document-demo h4,
.document-demo p {
  margin: 0;
}

.document-demo h2,
.document-demo h3,
.document-demo h4,
.document-demo__overview-item strong {
  color: var(--el-text-color-primary);
}

.document-demo__header p,
.document-demo__heading p,
.document-demo__overview-item small {
  margin-top: 5px;
  color: var(--el-text-color-secondary);
}

.document-demo__overview {
  display: grid;
  grid-template-columns: repeat(5, minmax(150px, 1fr));
  gap: 10px;
}

.document-demo__overview-item,
.document-demo__tree-card {
  padding: 12px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.document-demo__overview-item {
  display: flex;
  flex-direction: column;
  gap: 3px;
}

.document-demo__overview-item > span {
  color: var(--el-color-primary);
  font-size: 12px;
  font-weight: 700;
}

.document-demo__tabs {
  padding: 0 16px 16px;
  border: 1px solid var(--el-border-color-light);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.document-demo__section {
  display: flex;
  flex-direction: column;
  gap: 14px;
}

.document-demo__pdf-grid,
.document-demo__tree-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 16px;
}

.document-demo__pdf-grid h4,
.document-demo__tree-card h4 {
  margin-bottom: 10px;
}

.document-demo__tree-card {
  min-height: 360px;
}

.document-demo__tree-card :deep(.tree-filter-panel__node-label),
.document-demo__template-node {
  display: flex;
  flex: 1;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  color: var(--el-text-color-primary);
}

.document-demo__log {
  padding: 8px 12px;
  border-radius: var(--el-border-radius-base);
  background: var(--el-color-primary-light-9);
  color: var(--el-color-primary);
  font-size: 13px;
}

@media (max-width: 1000px) {
  .document-demo__overview {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }

  .document-demo__pdf-grid,
  .document-demo__tree-grid {
    grid-template-columns: 1fr;
  }
}
</style>
