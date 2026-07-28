<template>
  <BasicCrudPage
    title="病区/病房/床位"
    description="统一维护病区、病房和床位层级，并支持启用、停用"
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
  createWardBed,
  pageWardBeds,
  removeWardBed,
  setWardBedEnabled,
  updateWardBed
} from '@/apis/basic/ward-bed'
import type { WardBedSavePayload } from '@/apis/basic/ward-bed'

const service = {
  page: pageWardBeds,
  create: (data: Record<string, unknown>) => createWardBed(data as WardBedSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateWardBed(id, data as WardBedSavePayload),
  remove: removeWardBed,
  setEnabled: setWardBedEnabled
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'busNo', label: '位置编码', width: '110px' },
  { prop: 'name', label: '名称', 'min-width': 150 },
  { prop: 'formEnumText', label: '类型', width: '90px' },
  { prop: 'organizationName', label: '关联科室', width: '120px' },
  { prop: 'parentName', label: '上级位置', width: '150px' },
  { prop: 'operationalEnumText', label: '使用情况', width: '100px' },
  { prop: 'statusEnumText', label: '状态', width: '80px' },
  { prop: 'displayOrder', label: '排序', width: '70px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  select('formEnum', '类型', [
    { label: '病区', value: 'WARD' },
    { label: '病房', value: 'HOUSE' },
    { label: '床位', value: 'BED' }
  ], true),
  requiredInput('name', '名称'),
  select('organizationId', '关联科室', [
    { label: '内科', value: 'dept-001' },
    { label: '外科', value: 'dept-002' }
  ], true),
  select('parentId', '上级位置', [
    { label: '无上级', value: '' },
    { label: '内科一病区', value: 'ward-001' },
    { label: '内科101病房', value: 'room-001' },
    { label: '外科一病区', value: 'ward-002' }
  ]),
  {
    type: 'number',
    prop: 'displayOrder',
    label: '显示顺序',
    fprops: { min: 0 } as MyFormInputNumberProps
  },
  select('statusEnum', '状态', [
    { label: '启用', value: 'ACTIVE' },
    { label: '停用', value: 'INACTIVE' }
  ], true),
  select('operationalEnum', '使用情况', [
    { label: '未使用', value: 'NOT_IN_USE' },
    { label: '使用中', value: 'IN_USE' }
  ], true)
]

const defaultForm = {
  name: '',
  formEnum: 'WARD',
  organizationId: 'dept-001',
  parentId: '',
  displayOrder: 0,
  statusEnum: 'ACTIVE',
  operationalEnum: 'NOT_IN_USE'
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
function select(
  prop: string,
  label: string,
  options: Array<{ label: string; value: string }>,
  required = false
): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    required,
    rules: required ? [{ required: true, message: `请选择${label}`, trigger: 'change' }] : undefined,
    fprops: { placeholder: `请选择${label}`, options, clearable: !required } as MyFormSelectProps
  }
}
</script>
