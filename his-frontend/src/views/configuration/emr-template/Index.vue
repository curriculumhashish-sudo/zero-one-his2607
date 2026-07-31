<template>
  <section class="emr-template-page">
    <el-tabs v-model="activeTab" type="border-card">
      <el-tab-pane label="文书类型" name="document-type">
        <BasicCrudPage
          title="文书类型"
          description="用于给病历模板分类，模板创建时按此类型选择适用文书。"
          :service="documentTypeService"
          :columns="documentTypeColumns"
          :form-items="documentTypeFormItems"
          :default-form="documentTypeDefaultForm"
        />
      </el-tab-pane>
      <el-tab-pane label="病历模板" name="template">
        <BasicCrudPage
          title="病历模板"
          description="维护可复用的病历内容；实际编辑器可在后续接入富文本组件替换“模板内容”字段。"
          :service="templateService"
          :columns="templateColumns"
          :form-items="templateFormItems"
          :default-form="templateDefaultForm"
          dialog-width="860px"
        />
      </el-tab-pane>
      <el-tab-pane label="元数据字段" name="metadata">
        <BasicCrudPage
          title="元数据字段"
          description="维护可被病历模板引用的变量字段，例如患者姓名、就诊日期和诊断。"
          :service="metadataService"
          :columns="metadataColumns"
          :form-items="metadataFormItems"
          :default-form="metadataDefaultForm"
        />
      </el-tab-pane>
    </el-tabs>
  </section>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import type { MyFormInputProps, MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createDocumentType,
  createEmrTemplate,
  createMetadataField,
  getDocumentTypeDetail,
  getEmrTemplateDetail,
  getMetadataFieldDetail,
  pageDocumentTypes,
  pageEmrTemplates,
  pageMetadataFields,
  removeDocumentType,
  removeEmrTemplate,
  removeMetadataField,
  updateDocumentType,
  updateEmrTemplate,
  updateMetadataField
} from '@/apis/configuration/emr-template'
import type {
  DocumentTypeSavePayload,
  EmrTemplateSavePayload,
  MetadataFieldSavePayload
} from '@/apis/configuration/emr-template'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const activeTab = ref('document-type')

const documentTypeService = {
  page: pageDocumentTypes,
  detail: getDocumentTypeDetail,
  create: (data: Record<string, unknown>) => createDocumentType(data as DocumentTypeSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateDocumentType(id, data as DocumentTypeSavePayload),
  remove: removeDocumentType
} as unknown as BasicCrudService

const templateService = {
  page: pageEmrTemplates,
  detail: getEmrTemplateDetail,
  create: (data: Record<string, unknown>) => createEmrTemplate(data as EmrTemplateSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateEmrTemplate(id, data as EmrTemplateSavePayload),
  remove: removeEmrTemplate
} as unknown as BasicCrudService

const metadataService = {
  page: pageMetadataFields,
  detail: getMetadataFieldDetail,
  create: (data: Record<string, unknown>) => createMetadataField(data as MetadataFieldSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateMetadataField(id, data as MetadataFieldSavePayload),
  remove: removeMetadataField
} as unknown as BasicCrudService

const documentTypeColumns: MyTableColumn[] = [
  { prop: 'code', label: '类型编码', width: '170px' },
  { prop: 'name', label: '类型名称', width: '160px' },
  { prop: 'description', label: '说明', 'min-width': 260, 'show-overflow-tooltip': true }
]

const templateColumns: MyTableColumn[] = [
  { prop: 'code', label: '模板编码', width: '150px' },
  { prop: 'name', label: '模板名称', width: '180px' },
  { prop: 'documentTypeName', label: '文书类型', width: '140px' },
  { prop: 'applicableScope', label: '适用范围', width: '120px' },
  { prop: 'enabled', label: '启用状态', width: '100px', align: 'center' }
]

const metadataColumns: MyTableColumn[] = [
  { prop: 'code', label: '字段编码', width: '170px' },
  { prop: 'name', label: '字段名称', width: '150px' },
  { prop: 'dataType', label: '数据类型', width: '120px' },
  { prop: 'defaultValue', label: '默认值', width: '150px' },
  { prop: 'enabled', label: '启用状态', width: '100px', align: 'center' },
  { prop: 'description', label: '说明', 'min-width': 180, 'show-overflow-tooltip': true }
]

const documentTypeFormItems: MyFormItemAttr[] = [
  requiredInput('code', '类型编码', '请输入文书类型编码'),
  requiredInput('name', '类型名称', '请输入文书类型名称'),
  input('description', '说明', '请输入文书类型说明')
]

const templateFormItems: MyFormItemAttr[] = [
  requiredInput('code', '模板编码', '请输入病历模板编码'),
  requiredInput('name', '模板名称', '请输入病历模板名称'),
  select('documentTypeCode', '文书类型', [
    { label: '门诊病历', value: 'OUTPATIENT' },
    { label: '入院记录', value: 'ADMISSION' },
    { label: '病程记录', value: 'PROGRESS' }
  ]),
  input('applicableScope', '适用范围', '例如：门诊、住院'),
  {
    type: 'input',
    prop: 'content',
    label: '模板内容',
    fprops: { type: 'textarea', rows: 5, placeholder: '请输入模板内容' } as MyFormInputProps
  },
  booleanSelect('enabled', '启用状态')
]

const metadataFormItems: MyFormItemAttr[] = [
  requiredInput('code', '字段编码', '请输入元数据字段编码'),
  requiredInput('name', '字段名称', '请输入元数据字段名称'),
  select('dataType', '数据类型', [
    { label: '文本', value: 'TEXT' },
    { label: '数字', value: 'NUMBER' },
    { label: '日期', value: 'DATE' },
    { label: '布尔值', value: 'BOOLEAN' }
  ]),
  input('defaultValue', '默认值', '请输入默认值'),
  input('description', '说明', '请输入字段说明'),
  booleanSelect('enabled', '启用状态')
]

const documentTypeDefaultForm = { code: '', name: '', description: '' }
const templateDefaultForm = {
  code: '',
  name: '',
  documentTypeCode: 'OUTPATIENT',
  applicableScope: '门诊',
  content: '',
  enabled: true
}
const metadataDefaultForm = {
  code: '',
  name: '',
  dataType: 'TEXT',
  defaultValue: '',
  description: '',
  enabled: true
}

function input(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder, clearable: true } }
}

function requiredInput(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return {
    ...input(prop, label, placeholder),
    required: true,
    rules: [{ required: true, message: placeholder, trigger: 'blur' }]
  }
}

function select(
  prop: string,
  label: string,
  options: Array<{ label: string; value: string }>
): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps
  }
}

function booleanSelect(prop: string, label: string): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    fprops: {
      placeholder: `请选择${label}`,
      options: [
        { label: '启用', value: true },
        { label: '停用', value: false }
      ]
    } as MyFormSelectProps
  }
}
</script>

<style scoped>
.emr-template-page {
  min-height: 100%;
  padding: 16px;
  box-sizing: border-box;
}

:deep(.el-tabs__content) {
  padding: 0;
}
</style>
