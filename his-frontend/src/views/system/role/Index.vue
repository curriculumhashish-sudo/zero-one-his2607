<template>
  <BasicCrudPage
    title="角色管理"
    description="维护角色编码、角色名称和可分配权限；用户管理页面选择的角色将由后端统一关联。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="820px"
  />
</template>

<script setup lang="ts">
import type { MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import { createRole, getRoleDetail, pageRoles, removeRole, updateRole } from '@/apis/system/role'
import type { RoleSavePayload } from '@/apis/system/role'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const permissionOptions = [
  { label: '用户管理', value: 'System.User' },
  { label: '角色管理', value: 'System.Role' },
  { label: '菜单管理', value: 'System.Menu' },
  { label: '门诊开单', value: 'Outpatient.Order' },
  { label: '药房发药', value: 'Pharmacy.Dispense' }
]

const service = {
  page: pageRoles,
  detail: getRoleDetail,
  create: (data: Record<string, unknown>) => createRole(data as RoleSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateRole(id, data as RoleSavePayload),
  remove: removeRole
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'displayName', label: '角色名称', width: '140px' },
  { prop: 'name', label: '角色编码', width: '140px' },
  { prop: 'permissionNames', label: '权限范围', 'min-width': 240, 'show-overflow-tooltip': true },
  { prop: 'isDefault', label: '默认角色', width: '100px', align: 'center' },
  { prop: 'description', label: '说明', 'min-width': 180, 'show-overflow-tooltip': true }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '角色编码', '请输入角色编码'),
  requiredInput('displayName', '角色名称', '请输入角色名称'),
  {
    type: 'select',
    prop: 'permissionIds',
    label: '权限范围',
    fprops: {
      placeholder: '请选择可分配权限',
      clearable: true,
      multiple: true,
      filterable: true,
      options: permissionOptions
    } as MyFormSelectProps
  },
  select('isDefault', '默认角色'),
  input('description', '说明', '请输入角色说明')
]

const defaultForm = {
  name: '',
  displayName: '',
  description: '',
  permissionIds: [],
  isDefault: false
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

function select(prop: string, label: string): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    fprops: {
      placeholder: `请选择${label}`,
      options: [
        { label: '是', value: true },
        { label: '否', value: false }
      ]
    } as MyFormSelectProps
  }
}
</script>
