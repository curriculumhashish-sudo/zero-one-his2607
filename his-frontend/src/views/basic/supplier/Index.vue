<template>
  <BasicCrudPage
    title="供应商"
    description="维护药品、器械及综合供应商的联系信息与启停状态"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
  />
</template>

<script setup lang="ts">
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'
import type { MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createSupplier,
  getSupplierDetail,
  pageSuppliers,
  removeSupplier,
  setSuppliersEnabled,
  updateSupplier
} from '@/apis/basic/supplier'
import type { SupplierSavePayload } from '@/apis/basic/supplier'

const service = {
  page: pageSuppliers,
  detail: getSupplierDetail,
  create: (data: Record<string, unknown>) => createSupplier(data as SupplierSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateSupplier(id, data as SupplierSavePayload),
  remove: removeSupplier,
  setEnabled: async (id: string, enabled: boolean) => (await setSuppliersEnabled([id], enabled))[0]!
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'busNo', label: '供应商编号', width: '130px' },
  { prop: 'name', label: '供应商名称', 'min-width': 190 },
  { prop: 'typeEnumText', label: '类型', width: '100px' },
  { prop: 'organizationName', label: '所属部门', width: '120px' },
  { prop: 'address', label: '地址', 'min-width': 180, 'show-overflow-tooltip': true },
  { prop: 'phone', label: '电话', width: '140px' },
  { prop: 'email', label: '邮箱', width: '180px', 'show-overflow-tooltip': true },
  { prop: 'statusEnumText', label: '状态', width: '80px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('busNo', '供应商编号'),
  requiredInput('name', '供应商名称'),
  requiredInput('creditCode', '统一信用代码'),
  select('organizationId', '所属部门', [
    { label: '当前组织', value: import.meta.env.VITE_DEV_ORGANIZATION_ID || '' }
  ], true),
  select('typeEnum', '类型', [
    { label: '企业', value: '1' },
    { label: '个体', value: '2' }
  ], true),
  input('address', '地址'),
  input('phone', '电话'),
  input('email', '邮箱'),
  requiredInput('pyStr', '拼音'),
  requiredInput('wbStr', '五笔'),
  select('statusEnum', '状态', [
    { label: '启用', value: 'ACTIVE' },
    { label: '停用', value: 'INACTIVE' }
  ])
]

const defaultForm = {
  busNo: '',
  name: '',
  creditCode: '',
  organizationId: import.meta.env.VITE_DEV_ORGANIZATION_ID || '',
  typeEnum: '1',
  address: '',
  phone: '',
  email: '',
  pyStr: '',
  wbStr: '',
  statusEnum: 'ACTIVE'
}

function input(prop: string, label: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder: `请输入${label}`, clearable: true } }
}
function requiredInput(prop: string, label: string): MyFormItemAttr {
  return {
    ...input(prop, label),
    required: true,
    rules: [{ required: true, message: `请输入${label}`, trigger: 'blur' }]
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
    fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps
  }
}
</script>
