<template>
  <div class="medical-template-page">
    <div class="page-header">
      <h2>病历模板设计器</h2>
      <p class="page-desc">拖拽元数据字段到正文，设计病历模板内容</p>
    </div>
    <div class="editor-container">
      <MedicalTemplateEditor
        :template-info="templateInfo"
        :content="content"
        @save="handleSave"
        @cancel="handleCancel"
        @copy="handleCopy"
      />
    </div>
    <div v-if="savedTemplate" class="save-result">
      <div class="result-box">
        <h4>保存结果</h4>
        <pre class="result-code">{{ JSON.stringify(savedTemplate, null, 2) }}</pre>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import MedicalTemplateEditor from '@/components/medical-template-editor/MedicalTemplateEditor.vue'
import {
  type MedicalTemplate,
  type MedicalTemplateInfo,
  createMedicalTemplateInfo
} from '@/components/medical-template-editor/type'

const templateInfo = ref<MedicalTemplateInfo>(
  createMedicalTemplateInfo({ name: '首次病程记录', code: 'PN_FIRST' })
)
const content = ref(
  '<h1>首次病程记录</h1><p>诊号：<span data-metadata-variable data-key="caseNo" data-label="诊号">[诊号]</span></p><p>患者姓名：<span data-metadata-variable data-key="patientName" data-label="患者姓名">[患者姓名]</span></p><p>性别：<span data-metadata-variable data-key="gender" data-label="性别" data-readonly="true">[性别]</span>，年龄：<span data-metadata-variable data-key="age" data-label="年龄" data-readonly="true">[年龄]</span>岁</p><p>病历号：<span data-metadata-variable data-key="medicalRecordNo" data-label="病历号" data-readonly="true">[病历号]</span></p><p>科室：<span data-metadata-variable data-key="department" data-label="科室" data-readonly="true">[科室]</span></p><p>医师姓名：<span data-metadata-variable data-key="doctorName" data-label="医师姓名" data-readonly="true">[医师姓名]</span></p><p>入院日期：<span data-metadata-variable data-key="admissionDate" data-label="入院日期" data-readonly="true">[入院日期]</span></p><p>电话：<span data-metadata-variable data-key="phone" data-label="电话">[电话]</span></p><p>病例特点：</p><p>初步诊断：<span data-metadata-variable data-key="diagnosis" data-label="初步诊断">[初步诊断]</span></p><p>诊疗计划：</p>'
)
const savedTemplate = ref<MedicalTemplate | null>(null)

function handleSave(template: MedicalTemplate) {
  savedTemplate.value = template
  templateInfo.value = { ...template.info }
  content.value = template.content
}

function handleCancel() {
  if (savedTemplate.value) {
    templateInfo.value = { ...savedTemplate.value.info }
    content.value = savedTemplate.value.content
  }
}

function handleCopy(template: MedicalTemplate) {
  savedTemplate.value = template
  templateInfo.value = { ...template.info }
  content.value = template.content
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

.editor-container {
  flex: 1;
  background: #fff;
  border-radius: 8px;
  overflow: hidden;
}

.save-result {
  margin-top: 16px;
}

.result-box {
  background: #fff;
  border-radius: 8px;
  padding: 16px;
}

.result-box h4 {
  margin: 0 0 12px 0;
  font-size: 14px;
  font-weight: 600;
  color: #333;
}

.result-code {
  margin: 0;
  padding: 12px;
  background: #f5f5f5;
  border-radius: 6px;
  font-size: 12px;
  overflow-x: auto;
  max-height: 200px;
  overflow-y: auto;
}
</style>
