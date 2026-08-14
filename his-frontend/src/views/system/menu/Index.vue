<template>
  <BasicCrudPage
    title="菜单管理"
    description="维护导航菜单的名称、路径、组件位置、显示状态和启停状态；后端菜单接入后可直接替换此 Mock 数据。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    :resolve-enabled="(row) => Boolean(row.enabled)"
    dialog-width="820px"
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
  createMenu,
  getMenuDetail,
  pageMenus,
  removeMenu,
  setMenuEnabled,
  updateMenu
} from '@/apis/system/menu'
import type { MenuSavePayload } from '@/apis/system/menu'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageMenus,
  detail: getMenuDetail,
  create: (data: Record<string, unknown>) => createMenu(data as MenuSavePayload),
  update: (id: string, data: Record<string, unknown>) => updateMenu(id, data as MenuSavePayload),
  remove: removeMenu,
  setEnabled: setMenuEnabled
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'name', label: '菜单名称', width: '130px' },
  { prop: 'parentName', label: '上级菜单', width: '120px' },
  { prop: 'path', label: '菜单路径', width: '190px' },
  { prop: 'component', label: '页面组件', width: '180px', 'show-overflow-tooltip': true },
  { prop: 'sortNo', label: '排序', width: '80px', align: 'center' },
  { prop: 'visible', label: '显示', width: '80px', align: 'center' },
  { prop: 'enabled', label: '状态', width: '80px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '菜单名称', '请输入菜单名称'),
  input('parentName', '上级菜单', '一级菜单留空'),
  input('path', '菜单路径', '例如：/system/user'),
  input('component', '页面组件', '例如：system/user/Index'),
  input('icon', '图标名称', '例如：IconUser'),
  {
    type: 'number',
    prop: 'sortNo',
    label: '排序号',
    fprops: { min: 0, max: 9999 } as MyFormInputNumberProps
  },
  select('visible', '是否显示'),
  select('enabled', '启用状态'),
  input('description', '说明', '请输入菜单说明')
]

const defaultForm = {
  name: '',
  parentName: '',
  path: '',
  component: '',
  icon: '',
  sortNo: 0,
  visible: true,
  enabled: true,
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
