<template>
  <BasicCrudPage
    title="执行地点"
    description="维护项目的执行组织、执行地点和生效规则。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="860px"
  />
</template>

<script setup lang="ts">
import type { MyFormInputNumberProps, MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import { createExecuteLocation, pageExecuteLocations, removeExecuteLocation, updateExecuteLocation } from '@/apis/configuration/execute-location'
import type { ExecuteLocationSavePayload } from '@/apis/configuration/execute-location'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageExecuteLocations,
  create: (data: Record<string, unknown>) => createExecuteLocation(data as ExecuteLocationSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateExecuteLocation(id, data as ExecuteLocationSavePayload),
  remove: removeExecuteLocation
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'organizationId', label: '开立组织 ID', width: '160px' },
  { prop: 'itemTable', label: '项目表', width: '210px' },
  { prop: 'distributionCategoryCode', label: '分发分类', width: '120px' },
  { prop: 'defLocationName', label: '默认执行地点', width: '160px' },
  { prop: 'priority', label: '优先级', width: '90px' },
  { prop: 'displayOrder', label: '排序', width: '90px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('organizationId', '开立组织 ID', '请输入组织机构 ID'),
  requiredInput('itemTable', '项目表', '例如 med_medication_definition'),
  select('distributionCategoryCode', '分发分类', [
    { label: '西药', value: '1' }, { label: '中药', value: '2' }, { label: '耗材/设备', value: '11' }
  ]),
  requiredInput('defLocationId', '默认执行地点 ID', '请输入 adm_location ID'),
  numberInput('priority', '优先级', 1),
  input('itemDefinitionId', '具体项目 ID', '留空表示整类生效'),
  input('methodCode', '执行方式编码', '可选'),
  input('startTime', '生效开始时间', 'HH:mm:ss'),
  input('endTime', '生效结束时间', 'HH:mm:ss'),
  numberInput('displayOrder', '排序', 0),
  input('remark', '备注', '可选')
]

const defaultForm = {
  organizationId: import.meta.env.VITE_DEV_ORGANIZATION_ID || '', itemTable: 'med_medication_definition',
  distributionCategoryCode: '1', defLocationId: '', priority: 1, itemDefinitionId: '', methodCode: '',
  startTime: '00:00:00', endTime: '23:59:59', displayOrder: 0, remark: ''
}

function input(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder, clearable: true } }
}
function requiredInput(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { ...input(prop, label, placeholder), required: true, rules: [{ required: true, message: placeholder, trigger: 'blur' }] }
}
function numberInput(prop: string, label: string, min: number): MyFormItemAttr {
  return { type: 'number', prop, label, fprops: { min, max: 999999 } as MyFormInputNumberProps }
}
function select(prop: string, label: string, options: Array<{ label: string; value: string }>): MyFormItemAttr {
  return { type: 'select', prop, label, fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps }
}
</script>
