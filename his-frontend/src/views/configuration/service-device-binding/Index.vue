<template>
  <BasicCrudPage
    title="耗材绑定"
    description="维护服务项目与耗材或设备的绑定关系。"
    :service="service"
    :columns="columns"
    :search-items="searchItems"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="860px"
  />
</template>

<script setup lang="ts">
import type { MyFormInputNumberProps, MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import { createServiceDeviceBinding, pageServiceDeviceBindings, removeServiceDeviceBinding, updateServiceDeviceBinding } from '@/apis/configuration/service-device-binding'
import type { ServiceDeviceBindingSavePayload } from '@/apis/configuration/service-device-binding'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageServiceDeviceBindings,
  create: (data: Record<string, unknown>) => createServiceDeviceBinding(data as ServiceDeviceBindingSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateServiceDeviceBinding(id, data as ServiceDeviceBindingSavePayload),
  remove: removeServiceDeviceBinding
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'typeName', label: '绑定类型', width: '130px' },
  { prop: 'devActName', label: '耗材/设备名称', width: '190px' },
  { prop: 'quantity', label: '数量', width: '90px' },
  { prop: 'unitName', label: '单位', width: '90px' },
  { prop: 'statusName', label: '状态', width: '100px' }
]

const searchItems: MyFormItemAttr[] = [
  input('sourceId', '来源 ID', '可选'),
  select('bindType', '绑定类型', [{ label: '用法', value: 'USAGE' }, { label: '活动', value: 'ACTIVITY' }])
]

const formItems: MyFormItemAttr[] = [
  select('typeCode', '绑定类型编码', [
    { label: '用法', value: 'USAGE' }, { label: '活动', value: 'ACTIVITY' }
  ], true),
  requiredInput('sourceId', '来源 ID', '请输入服务或活动 ID'),
  select('devActTable', '目标表', [
    { label: '设备定义', value: 'adm_device_definition' },
    { label: '活动定义', value: 'wkf_activity_definition' }
  ], true),
  requiredInput('devActId', '目标 ID', '请输入耗材或设备 ID'),
  numberInput('quantity', '数量', 0.01),
  select('unitCode', '单位编码', [
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '14', '71'
  ].map((value) => ({ label: value, value })), true),
  select('statusEnum', '状态', [{ label: '启用', value: 'ACTIVE' }, { label: '停用', value: 'INACTIVE' }], true)
]

const defaultForm = {
  typeCode: 'USAGE', sourceId: '', devActTable: 'adm_device_definition', devActId: '', quantity: 1,
  unitCode: '', statusEnum: 'ACTIVE'
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
function select(prop: string, label: string, options: Array<{ label: string; value: string }>, required = false): MyFormItemAttr {
  return {
    type: 'select', prop, label, required,
    fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps,
    ...(required ? { rules: [{ required: true, message: `请选择${label}`, trigger: 'change' }] } : {})
  }
}
</script>
