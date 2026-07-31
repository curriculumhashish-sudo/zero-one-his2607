<template>
  <BasicCrudPage
    title="权限管理"
    description="维护权限标识、显示名称、上级权限和授权范围；角色可据此分配功能访问权限。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
  />
</template>

<script setup lang="ts">
import type { MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createPermission,
  pagePermissions,
  removePermission,
  updatePermission
} from '@/apis/system/permission'
import type { PermissionSavePayload } from '@/apis/system/permission'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pagePermissions,
  create: (data: Record<string, unknown>) => createPermission(data as PermissionSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updatePermission(id, data as PermissionSavePayload),
  remove: removePermission
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'displayName', label: '权限名称', width: '150px' },
  { prop: 'name', label: '权限标识', width: '190px' },
  { prop: 'parentName', label: '上级权限', width: '130px' },
  { prop: 'providerName', label: '提供方', width: '90px', align: 'center' },
  { prop: 'grantable', label: '可授权', width: '90px', align: 'center' },
  { prop: 'description', label: '说明', 'min-width': 180, 'show-overflow-tooltip': true }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '权限标识', '例如：System.User'),
  requiredInput('displayName', '权限名称', '请输入权限名称'),
  input('parentName', '上级权限', '例如：System'),
  input('description', '说明', '请输入权限用途说明'),
  select('grantable', '可授权', [
    { label: '是', value: true },
    { label: '否', value: false }
  ])
]

const defaultForm = {
  name: '',
  displayName: '',
  parentName: '',
  description: '',
  grantable: true
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
  options: Array<{ label: string; value: boolean }>
): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps
  }
}
</script>
