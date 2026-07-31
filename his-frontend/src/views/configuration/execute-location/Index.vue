<template>
  <BasicCrudPage
    title="执行地点"
    description="维护检查、检验和治疗项目应由哪个科室执行，为开单后的执行流转提供基础数据。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="820px"
  />
</template>

<script setup lang="ts">
import type { MyFormDatePickerProps, MyFormItemAttr } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createExecuteLocation,
  pageExecuteLocations,
  removeExecuteLocation,
  updateExecuteLocation
} from '@/apis/configuration/execute-location'
import type { ExecuteLocationSavePayload } from '@/apis/configuration/execute-location'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageExecuteLocations,
  create: (data: Record<string, unknown>) =>
    createExecuteLocation(data as ExecuteLocationSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateExecuteLocation(id, data as ExecuteLocationSavePayload),
  remove: removeExecuteLocation
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'departmentName', label: '执行科室', width: '140px' },
  { prop: 'itemCode', label: '项目编码', width: '140px' },
  { prop: 'itemName', label: '项目名称', width: '170px' },
  { prop: 'serviceCategory', label: '项目类别', width: '120px' },
  { prop: 'startDate', label: '生效日期', width: '130px' },
  { prop: 'endDate', label: '失效日期', width: '130px' },
  { prop: 'description', label: '说明', 'min-width': 170, 'show-overflow-tooltip': true }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('departmentName', '执行科室', '请输入执行科室'),
  requiredInput('itemCode', '项目编码', '请输入执行项目编码'),
  requiredInput('itemName', '项目名称', '请输入执行项目名称'),
  input('serviceCategory', '项目类别', '例如：检验、检查、治疗'),
  date('startDate', '生效日期'),
  date('endDate', '失效日期'),
  input('description', '说明', '请输入配置说明')
]

const defaultForm = {
  departmentName: '',
  itemCode: '',
  itemName: '',
  serviceCategory: '',
  startDate: '',
  endDate: '',
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

function date(prop: string, label: string): MyFormItemAttr {
  return {
    type: 'date',
    prop,
    label,
    fprops: {
      type: 'date',
      'value-format': 'YYYY-MM-DD',
      placeholder: `请选择${label}`
    } as MyFormDatePickerProps
  }
}
</script>
