<template>
  <section class="emr-template-page">
    <el-tabs v-model="activeTab" type="border-card">
      <el-tab-pane label="文书类型" name="document-type">
        <BasicCrudPage
          title="文书类型"
          description="维护 C7 病历模板使用的文书类型。"
          :service="documentTypeService"
          :columns="documentTypeColumns"
          :form-items="documentTypeFormItems"
          :default-form="documentTypeDefaultForm"
        />
      </el-tab-pane>
      <el-tab-pane label="病历模板" name="template">
        <BasicCrudPage
          title="病历模板"
          description="维护病历模板基本信息和模板正文。"
          :service="templateService"
          :columns="templateColumns"
          :form-items="templateFormItems"
          :default-form="templateDefaultForm"
          dialog-width="900px"
        />
      </el-tab-pane>
      <el-tab-pane label="元数据字段" name="metadata">
        <BasicCrudPage
          title="元数据字段"
          description="维护病历模板可引用的元数据字段。"
          :service="metadataService"
          :columns="metadataColumns"
          :form-items="metadataFormItems"
          :default-form="metadataDefaultForm"
          dialog-width="900px"
        />
      </el-tab-pane>
    </el-tabs>
  </section>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import type { MyFormInputNumberProps, MyFormInputProps, MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
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
import type { DocumentTypeSavePayload, EmrTemplateSavePayload, MetadataFieldSavePayload } from '@/apis/configuration/emr-template'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const activeTab = ref('document-type')

const documentTypeService = {
  page: pageDocumentTypes,
  detail: getDocumentTypeDetail,
  create: (data: Record<string, unknown>) => createDocumentType(data as DocumentTypeSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateDocumentType(id, data as DocumentTypeSavePayload),
  remove: removeDocumentType
} as unknown as BasicCrudService

const templateService = {
  page: pageEmrTemplates,
  detail: getEmrTemplateDetail,
  create: (data: Record<string, unknown>) => createEmrTemplate(data as EmrTemplateSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateEmrTemplate(id, data as EmrTemplateSavePayload),
  remove: removeEmrTemplate
} as unknown as BasicCrudService

const metadataService = {
  page: pageMetadataFields,
  detail: getMetadataFieldDetail,
  create: (data: Record<string, unknown>) => createMetadataField(data as MetadataFieldSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateMetadataField(id, data as MetadataFieldSavePayload),
  remove: removeMetadataField
} as unknown as BasicCrudService

const documentTypeColumns: MyTableColumn[] = [
  { prop: 'code', label: '类型编码', width: '170px' },
  { prop: 'name', label: '类型名称', width: '170px' },
  { prop: 'defaultTemplateId', label: '默认模板 ID', width: '180px' },
  { prop: 'isEnabled', label: '启用状态', width: '100px' },
  { prop: 'sortOrder', label: '排序', width: '80px' }
]

const templateColumns: MyTableColumn[] = [
  { prop: 'code', label: '模板编码', width: '160px' },
  { prop: 'name', label: '模板名称', width: '180px' },
  { prop: 'documentTypeCode', label: '文书类型编码', width: '160px' },
  { prop: 'scopeLevel', label: '使用范围', width: '120px' },
  { prop: 'encounterClassCode', label: '就诊类别', width: '120px' },
  { prop: 'isEnabled', label: '启用状态', width: '100px' }
]

const metadataColumns: MyTableColumn[] = [
  { prop: 'fieldKey', label: '字段标识', width: '170px' },
  { prop: 'label', label: '显示名称', width: '150px' },
  { prop: 'dataType', label: '数据类型', width: '110px' },
  { prop: 'valueSource', label: '取值来源', width: '130px' },
  { prop: 'sourcePath', label: '来源路径', width: '180px' },
  { prop: 'isEnabled', label: '启用状态', width: '100px' }
]

const enabledOptions = [{ label: '启用', value: 1 }, { label: '停用', value: 0 }]

const documentTypeFormItems: MyFormItemAttr[] = [
  requiredInput('code', '类型编码', '请输入文书类型编码'),
  requiredInput('name', '类型名称', '请输入文书类型名称'),
  input('defaultTemplateId', '默认模板 ID', '可选'),
  numberInput('sortOrder', '排序', 0),
  select('isEnabled', '启用状态', enabledOptions)
]

const templateFormItems: MyFormItemAttr[] = [
  requiredInput('name', '模板名称', '请输入模板名称'),
  requiredInput('code', '模板编码', '请输入模板编码'),
  requiredInput('documentTypeCode', '文书类型编码', '请输入文书类型编码'),
  input('organizationId', '组织机构 ID', '按范围填写'),
  numberInput('encounterClassEnum', '就诊类别枚举', 0),
  numberInput('encounterTypeEnum', '就诊类型枚举', 0),
  input('scopeLevel', '使用范围', 'HOSPITAL / DEPARTMENT / PERSONAL'),
  input('ownerPractitionerId', '责任医生 ID', '个人模板时填写'),
  input('encounterClassCode', '就诊类别编码', '例如 OUTPATIENT'),
  textarea('content', '模板正文', '请输入模板正文或 JSON 内容'),
  select('isEnabled', '启用状态', enabledOptions)
]

const metadataFormItems: MyFormItemAttr[] = [
  requiredInput('fieldKey', '字段标识', '请输入字段标识'),
  requiredInput('label', '显示名称', '请输入显示名称'),
  requiredInput('dataType', '数据类型', '例如 string'),
  select('editableDefault', '默认可编辑', enabledOptions),
  requiredInput('valueSource', '取值来源', '例如 PATIENT'),
  requiredInput('sourcePath', '来源路径', '例如 patient.name'),
  textarea('sourceConfig', '来源配置', '可选 JSON 文本'),
  input('defaultValue', '默认值', '可选'),
  requiredInput('maskRule', '脱敏规则', 'NONE / ID_CARD / PHONE'),
  input('formatPattern', '格式化规则', '可选'),
  numberInput('sortOrder', '排序', 0),
  select('isEnabled', '启用状态', enabledOptions),
  input('remark', '备注', '可选')
]

const documentTypeDefaultForm = { code: '', name: '', defaultTemplateId: '', isEnabled: 1, sortOrder: 0 }
const templateDefaultForm = {
  name: '', code: '', documentTypeCode: '', organizationId: '', encounterClassEnum: 0,
  encounterTypeEnum: 0, content: '{}', isEnabled: 1, scopeLevel: 'HOSPITAL',
  ownerPractitionerId: '', encounterClassCode: 'OUTPATIENT'
}
const metadataDefaultForm = {
  fieldKey: '', label: '', dataType: 'string', editableDefault: 1, valueSource: '', sourcePath: '',
  sourceConfig: '', defaultValue: '', maskRule: 'NONE', formatPattern: '', sortOrder: 0, isEnabled: 1, remark: ''
}

function input(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder, clearable: true } }
}
function textarea(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { type: 'textarea', rows: 4, placeholder } as MyFormInputProps }
}
function requiredInput(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { ...input(prop, label, placeholder), required: true, rules: [{ required: true, message: placeholder, trigger: 'blur' }] }
}
function numberInput(prop: string, label: string, min: number): MyFormItemAttr {
  return { type: 'number', prop, label, fprops: { min, max: 999999 } as MyFormInputNumberProps }
}
function select(prop: string, label: string, options: Array<{ label: string; value: string | number }>): MyFormItemAttr {
  return { type: 'select', prop, label, fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps }
}
</script>

<style scoped>
.emr-template-page { min-height: 100%; padding: 16px; box-sizing: border-box; }
:deep(.el-tabs__content) { padding: 0; }
</style>
