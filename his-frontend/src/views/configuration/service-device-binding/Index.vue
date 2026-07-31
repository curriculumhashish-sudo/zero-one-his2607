<template>
  <BasicCrudPage
    title="诊疗耗材绑定"
    description="维护诊疗项目需要自动带出的耗材、规格、数量和单位，为开单或执行阶段提供耗材依据。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
  />
</template>

<script setup lang="ts">
import type {
  MyFormInputNumberProps,
  MyFormItemAttr,
  MyFormSelectProps
} from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createServiceDeviceBinding,
  pageServiceDeviceBindings,
  removeServiceDeviceBinding,
  updateServiceDeviceBinding
} from '@/apis/configuration/service-device-binding'
import type { ServiceDeviceBindingSavePayload } from '@/apis/configuration/service-device-binding'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageServiceDeviceBindings,
  create: (data: Record<string, unknown>) =>
    createServiceDeviceBinding(data as ServiceDeviceBindingSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateServiceDeviceBinding(id, data as ServiceDeviceBindingSavePayload),
  remove: removeServiceDeviceBinding
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'serviceName', label: '诊疗项目', width: '170px' },
  { prop: 'serviceCategory', label: '项目类别', width: '110px' },
  { prop: 'deviceName', label: '耗材名称', width: '180px' },
  { prop: 'specification', label: '规格', width: '140px' },
  { prop: 'quantity', label: '数量', width: '90px', align: 'center' },
  { prop: 'unit', label: '单位', width: '80px', align: 'center' },
  { prop: 'enabled', label: '启用状态', width: '100px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('serviceName', '诊疗项目', '请输入诊疗项目名称'),
  input('serviceCategory', '项目类别', '例如：治疗、检查'),
  requiredInput('deviceName', '耗材名称', '请输入耗材名称'),
  input('specification', '规格', '请输入耗材规格'),
  {
    type: 'number',
    prop: 'quantity',
    label: '数量',
    fprops: { min: 1, max: 999 } as MyFormInputNumberProps
  },
  select(
    'unit',
    '单位',
    ['个', '套', '包', '支'].map((value) => ({ label: value, value }))
  ),
  booleanSelect('enabled', '启用状态')
]

const defaultForm = {
  serviceName: '',
  serviceCategory: '治疗',
  deviceName: '',
  specification: '',
  quantity: 1,
  unit: '个',
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
