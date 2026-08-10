<template>
  <BasicCrudPage
    title="医嘱组合模板"
    description="维护 C7 医嘱组合模板的表头和明细。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="900px"
  />
</template>

<script setup lang="ts">
import type { MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import { createOrderTemplate, getOrderTemplateDetail, pageOrderTemplates, removeOrderTemplate, updateOrderTemplate } from '@/apis/configuration/order-template'
import type { OrderTemplateSavePayload } from '@/apis/configuration/order-template'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageOrderTemplates,
  detail: getOrderTemplateDetail,
  create: (data: Record<string, unknown>) => createOrderTemplate(data as unknown as OrderTemplateSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateOrderTemplate(id, data as unknown as OrderTemplateSavePayload),
  remove: removeOrderTemplate
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'id', label: '模板 ID', width: '180px' },
  { prop: 'name', label: '模板名称', width: '180px' },
  { prop: 'description', label: '说明', width: '180px' },
  { prop: 'statusEnum', label: '状态', width: '100px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('templateCode', '模板编码', '请输入模板编码'),
  requiredInput('name', '模板名称', '请输入模板名称'),
  input('objectId', '绑定对象 ID', '可选'),
  select('bindingEnum', '绑定类型', [
    { label: '个人', value: 'PERSONAL' }, { label: '科室', value: 'DEPARTMENT' },
    { label: '全院', value: 'HOSPITAL' }, { label: '协议', value: 'AGREEMENT' }
  ], true),
  select('sceneEnum', '适用场景', [
    { label: '门诊', value: 'OUTPATIENT' }, { label: '住院', value: 'INPATIENT' }
  ], true),
  select('templateKindEnum', '模板种类', [
    { label: '普通', value: 'GENERAL' }, { label: '中药', value: 'TCM' }, { label: '计价', value: 'PRICING' }
  ], true),
  input('description', '说明', '可选'),
  input('remark', '备注', '可选'),
  input('lines', '明细内容', '请输入明细文字，系统会自动封装为一条药品明细')
]

const defaultForm = {
  objectId: import.meta.env.VITE_DEV_ORGANIZATION_ID || '', name: '', bindingEnum: 'DEPARTMENT', sceneEnum: 'OUTPATIENT',
  templateKindEnum: 'GENERAL', templateCode: '', description: '', sortWeight: 0, remark: '',
  lines: '默认明细',
  defaultChineseHerbsDoseQuantity: 0, defaultSufferingFlag: false
}

function input(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder, clearable: true } }
}
function requiredInput(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { ...input(prop, label, placeholder), required: true, rules: [{ required: true, message: placeholder, trigger: 'blur' }] }
}
function select(prop: string, label: string, options: Array<{ label: string; value: string }>, required = false): MyFormItemAttr {
  return {
    type: 'select', prop, label, required,
    fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps,
    ...(required ? { rules: [{ required: true, message: `请选择${label}`, trigger: 'change' }] } : {})
  }
}
</script>
