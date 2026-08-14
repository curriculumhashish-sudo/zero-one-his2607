<template>
  <div class="medical-template-page">
    <div class="page-header">
      <h2>病历模板渲染演示</h2>
      <p class="page-desc">医生使用模板编写病历：加载模板数据后，在渲染结果基础上完善病历内容</p>
    </div>

    <div class="demo-layout">
      <div class="data-section">
        <div class="section-header">
          <h3>患者数据</h3>
          <span class="section-hint">修改数据后实时查看渲染效果</span>
        </div>
        <div class="data-form">
          <div class="form-grid">
            <div class="form-item" v-for="(value, key) in patientData" :key="key">
              <label
                >{{ getFieldLabel(key) }}
                <span v-if="isReadOnly(key)" class="readonly-badge">只读</span></label
              >
              <input
                v-model="patientData[key]"
                class="data-input"
                :class="{ 'readonly-input': isReadOnly(key) }"
                :disabled="isReadOnly(key)"
                :placeholder="`输入${getFieldLabel(key)}`"
              />
            </div>
          </div>
        </div>
        <div class="metadata-info">
          <h4>模板变量列表</h4>
          <p class="hint">从模板中提取的元数据变量：</p>
          <div class="variable-list">
            <span v-for="key in extractedKeys" :key="key" class="variable-tag">
              {{ getFieldLabel(key) }} ({{ key }})
            </span>
          </div>
        </div>
      </div>

      <div class="render-section">
        <MedicalTemplateEditor
          :content="renderedContent"
          :show-metadata="false"
          :show-template-info="false"
          @save="handleSave"
        />
      </div>
    </div>

    <div class="code-section">
      <div class="section-header">
        <h3>使用示例</h3>
      </div>
      <pre class="code-block">{{ renderCodeExample }}</pre>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import MedicalTemplateEditor from '@/components/medical-template-editor/MedicalTemplateEditor.vue'
import {
  type MedicalTemplateData,
  type MedicalTemplate,
  renderTemplate,
  extractMetadataKeys,
  DEFAULT_METADATA_LIST
} from '@/components/medical-template-editor/type'

const templateContent = ref(
  '<h1>首次病程记录</h1><p>诊号：<span data-metadata-variable data-key="caseNo" data-label="诊号">[诊号]</span></p><p>患者姓名：<span data-metadata-variable data-key="patientName" data-label="患者姓名">[患者姓名]</span></p><p>性别：<span data-metadata-variable data-key="gender" data-label="性别" data-readonly="true">[性别]</span>，年龄：<span data-metadata-variable data-key="age" data-label="年龄" data-readonly="true">[年龄]</span>岁</p><p>病历号：<span data-metadata-variable data-key="medicalRecordNo" data-label="病历号" data-readonly="true">[病历号]</span></p><p>科室：<span data-metadata-variable data-key="department" data-label="科室" data-readonly="true">[科室]</span></p><p>医师姓名：<span data-metadata-variable data-key="doctorName" data-label="医师姓名" data-readonly="true">[医师姓名]</span></p><p>入院日期：<span data-metadata-variable data-key="admissionDate" data-label="入院日期" data-readonly="true">[入院日期]</span></p><p>电话：<span data-metadata-variable data-key="phone" data-label="电话">[电话]</span></p><p>病例特点：</p><p>初步诊断：<span data-metadata-variable data-key="diagnosis" data-label="初步诊断">[初步诊断]</span></p><p>诊疗计划：</p>'
)

const patientData = ref<MedicalTemplateData>({
  caseNo: '20240101001',
  patientName: '张三',
  gender: '男',
  age: 35,
  idCard: '110101198901011234',
  medicalRecordNo: 'MR20240101001',
  phone: '13800138000',
  department: '内科',
  doctorName: '李医生',
  admissionDate: '2024-01-01',
  diagnosis: '上呼吸道感染'
})

const extractedKeys = computed(() => {
  return extractMetadataKeys(templateContent.value)
})

const renderedContent = computed(() => {
  return renderTemplate(templateContent.value, patientData.value)
})

const renderCodeExample = computed(() => {
  return `import { renderTemplate, extractMetadataKeys } from '@/components/medical-template-editor/type'

// 模板内容（来自模板设计器）
const templateContent = \`${templateContent.value.substring(0, 150)}...\`

// 患者数据
const patientData = ${JSON.stringify(patientData.value, null, 2)}

// 提取模板中使用的变量
const keys = extractMetadataKeys(templateContent)
console.log('模板变量:', keys)

// 渲染模板
const renderedHtml = renderTemplate(templateContent, patientData)
console.log('渲染结果:', renderedHtml)`
})

function getFieldLabel(key: string): string {
  const item = DEFAULT_METADATA_LIST.find((m) => m.key === key)
  return item?.label || key
}

function isReadOnly(key: string): boolean {
  const item = DEFAULT_METADATA_LIST.find((m) => m.key === key)
  return item?.readOnly || false
}

function handleSave(template: MedicalTemplate) {
  console.log('保存病历:', template)
  alert('病历已保存！')
}
</script>

<style scoped>
.medical-template-page {
  height: 100%;
  display: flex;
  flex-direction: column;
  padding: 16px;
  background: #f5f5f5;
}

.page-header {
  margin-bottom: 16px;
}

.page-header h2 {
  margin: 0 0 8px 0;
  font-size: 20px;
  color: #333;
}

.page-desc {
  margin: 0;
  font-size: 13px;
  color: #999;
}

.demo-layout {
  flex: 1;
  display: flex;
  gap: 16px;
  overflow: hidden;
  margin-bottom: 16px;
}

.data-section {
  width: 320px;
  display: flex;
  flex-direction: column;
  background: #fff;
  border-radius: 8px;
  overflow-y: auto;
}

.render-section {
  flex: 1;
  display: flex;
  flex-direction: column;
  background: #fff;
  border-radius: 8px;
  overflow: hidden;
}

.render-section :deep(.medical-template-editor) {
  height: 100%;
  padding: 0;
  background: transparent;
}

.section-header {
  padding: 12px 16px;
  border-bottom: 1px solid #eee;
  background: #fafafa;
}

.section-header h3 {
  margin: 0 0 4px 0;
  font-size: 14px;
  font-weight: 600;
  color: #333;
}

.section-hint {
  font-size: 12px;
  color: #999;
}

.data-form {
  padding: 16px;
}

.form-grid {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.form-item {
  display: flex;
  flex-direction: column;
}

.form-item label {
  font-size: 12px;
  color: #666;
  margin-bottom: 4px;
  font-weight: 500;
}

.data-input {
  padding: 8px 10px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 13px;
  transition: border-color 0.2s;
}

.data-input:focus {
  outline: none;
  border-color: #409eff;
}

.readonly-input {
  background: #f5f5f5;
  color: #909399;
  cursor: not-allowed;
}

.readonly-badge {
  font-size: 10px;
  color: #909399;
  background: #f5f5f5;
  padding: 1px 4px;
  border-radius: 2px;
  margin-left: 4px;
}

.metadata-info {
  padding: 0 16px 16px 16px;
}

.metadata-info h4 {
  margin: 0 0 8px 0;
  font-size: 13px;
  font-weight: 600;
  color: #333;
}

.hint {
  margin: 0 0 8px 0;
  font-size: 12px;
  color: #999;
}

.variable-list {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}

.variable-tag {
  padding: 4px 10px;
  background: #e8f4fd;
  color: #1e88e5;
  border-radius: 4px;
  font-size: 12px;
}

.code-section {
  background: #fff;
  border-radius: 8px;
  overflow: hidden;
  max-height: 200px;
}

.code-block {
  margin: 0;
  padding: 16px;
  background: #2d2d2d;
  color: #ccc;
  font-size: 12px;
  overflow: auto;
  font-family: monospace;
  max-height: calc(100% - 50px);
}
</style>
