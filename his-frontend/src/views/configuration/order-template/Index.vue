<template>
  <BasicCrudPage
    title="组套模板"
    description="维护常用药品、检查和治疗项目的组合。当前页面先维护组套头信息和项目摘要，后续可接入明细编辑器。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    :resolve-enabled="(row) => row.status === 'ENABLED'"
    dialog-width="820px"
  />
</template>

<script setup lang="ts">
import type { MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createOrderTemplate,
  getOrderTemplateDetail,
  pageOrderTemplates,
  removeOrderTemplate,
  setOrderTemplateEnabled,
  updateOrderTemplate
} from '@/apis/configuration/order-template'
import type { OrderTemplateSavePayload } from '@/apis/configuration/order-template'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageOrderTemplates,
  detail: getOrderTemplateDetail,
  create: (data: Record<string, unknown>) => createOrderTemplate(data as OrderTemplateSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateOrderTemplate(id, data as OrderTemplateSavePayload),
  remove: removeOrderTemplate,
  setEnabled: setOrderTemplateEnabled
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'code', label: '组套编码', width: '150px' },
  { prop: 'name', label: '组套名称', width: '180px' },
  { prop: 'templateType', label: '组套类型', width: '110px' },
  { prop: 'departmentName', label: '适用科室', width: '130px' },
  { prop: 'itemSummary', label: '项目摘要', 'min-width': 220, 'show-overflow-tooltip': true },
  { prop: 'statusText', label: '状态', width: '90px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('code', '组套编码', '请输入组套编码'),
  requiredInput('name', '组套名称', '请输入组套名称'),
  select('templateType', '组套类型', [
    { label: '普通组套', value: 'GENERAL' },
    { label: '中药组套', value: 'TCM' },
    { label: '计价组套', value: 'PRICING' }
  ]),
  input('departmentName', '适用科室', '请输入适用科室'),
  input('itemSummary', '项目摘要', '用顿号或逗号分隔项目'),
  select('status', '启用状态', [
    { label: '启用', value: 'ENABLED' },
    { label: '停用', value: 'DISABLED' }
  ]),
  input('description', '说明', '请输入组套说明')
]

const defaultForm = {
  code: '',
  name: '',
  templateType: 'GENERAL',
  departmentName: '',
  itemSummary: '',
  status: 'ENABLED',
  description: ''
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
</script>
