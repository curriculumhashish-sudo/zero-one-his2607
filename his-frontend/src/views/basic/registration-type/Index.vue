<template>
  <BasicCrudPage
    title="挂号类型"
    description="维护普通门诊、专家门诊、急诊等挂号类型与费用"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
  />
</template>

<script setup lang="ts">
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'
import type {
  MyFormInputNumberProps,
  MyFormItemAttr,
  MyFormSelectProps
} from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createRegistrationType,
  pageRegistrationTypes,
  removeRegistrationType,
  updateRegistrationType
} from '@/apis/basic/registration-type'
import type { RegistrationTypeSavePayload } from '@/apis/basic/registration-type'

const service = {
  page: pageRegistrationTypes,
  create: (data: Record<string, unknown>) =>
    createRegistrationType(data as RegistrationTypeSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateRegistrationType(id, data as RegistrationTypeSavePayload),
  remove: removeRegistrationType
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'name', label: '名称', 'min-width': 200 },
  { prop: 'ybNo', label: '医保编码', width: '140px' },
  { prop: 'busNo', label: '业务编码', width: '140px' },
  { prop: 'amount', label: '金额（元）', width: '110px', align: 'right' },
  { prop: 'activeFlagText', label: '状态', width: '90px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '名称'),
  requiredInput('ybNo', '医保编码'),
  {
    type: 'number',
    prop: 'amount',
    label: '金额',
    required: true,
    rules: [{ required: true, message: '请输入金额', trigger: 'change' }],
    fprops: { min: 0, step: 0.01, placeholder: '请输入金额' } as MyFormInputNumberProps
  },
  {
    type: 'select',
    prop: 'activeFlag',
    label: '状态',
    fprops: {
      options: [
        { label: '启用', value: 1 },
        { label: '停用', value: 0 }
      ]
    } as MyFormSelectProps
  }
]

const defaultForm = { name: '', ybNo: '', amount: 0, activeFlag: 1 }

function requiredInput(prop: string, label: string): MyFormItemAttr {
  return {
    type: 'input',
    prop,
    label,
    required: true,
    rules: [{ required: true, message: `请输入${label}`, trigger: 'blur' }],
    fprops: { placeholder: `请输入${label}`, clearable: true }
  }
}
</script>
