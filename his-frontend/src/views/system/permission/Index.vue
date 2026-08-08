<template>
  <BasicCrudPage
    title="权限管理"
    description="维护 J1 权限标识、显示名称、上级权限和说明。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
  />
</template>

<script setup lang="ts">
import type { MyFormItemAttr } from '@/components/myform/type'
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
  { prop: 'description', label: '说明', 'min-width': 180, 'show-overflow-tooltip': true }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '权限标识', '例如：System.User'),
  requiredInput('displayName', '权限名称', '请输入权限名称'),
  input('parentName', '上级权限', '例如：System'),
  input('description', '说明', '请输入权限用途说明')
]

const defaultForm = {
  name: '',
  displayName: '',
  parentName: '',
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
</script>
