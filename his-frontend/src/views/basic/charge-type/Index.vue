<template>
  <BasicCrudPage
    title="费用性质"
    description="维护自费、医保、公费等费用性质及显示顺序"
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
  createChargeType,
  pageChargeTypes,
  removeChargeType,
  setChargeTypeEnabled,
  updateChargeType
} from '@/apis/basic/charge-type'
import type { ChargeTypeSavePayload } from '@/apis/basic/charge-type'

const service = {
  page: pageChargeTypes,
  create: (data: Record<string, unknown>) => createChargeType(data as ChargeTypeSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateChargeType(id, data as ChargeTypeSavePayload),
  remove: removeChargeType,
  setEnabled: setChargeTypeEnabled
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'busNo', label: '编码', width: '130px' },
  { prop: 'name', label: '名称', 'min-width': 200 },
  { prop: 'categoryEnumText', label: '分类', width: '130px' },
  { prop: 'displayOrder', label: '显示顺序', width: '100px', align: 'center' },
  { prop: 'activeFlagText', label: '状态', width: '90px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('busNo', '编码'),
  requiredInput('name', '名称'),
  {
    type: 'number',
    prop: 'displayOrder',
    label: '显示顺序',
    fprops: { min: 0 } as MyFormInputNumberProps
  },
  {
    type: 'select',
    prop: 'categoryEnum',
    label: '分类',
    required: true,
    rules: [{ required: true, message: '请选择分类', trigger: 'change' }],
    fprops: {
      placeholder: '请选择分类',
      options: [
        { label: '自费', value: 'SELF_PAY' },
        { label: '医保', value: 'MEDICAL_INSURANCE' },
        { label: '公费', value: 'PUBLIC_PAY' }
      ]
    } as MyFormSelectProps
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

const defaultForm = {
  busNo: '',
  name: '',
  displayOrder: 0,
  categoryEnum: 'SELF_PAY',
  activeFlag: 1
}

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
