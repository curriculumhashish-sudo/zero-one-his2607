<template>
  <BasicCrudPage
    title="库房/药房"
    description="维护中心药库、门诊药房和住院药房等库存位置"
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
  createWarehousePharmacy,
  pageWarehousePharmacies,
  removeWarehousePharmacy,
  setWarehousePharmacyEnabled,
  updateWarehousePharmacy
} from '@/apis/basic/warehouse-pharmacy'
import type { WarehousePharmacySavePayload } from '@/apis/basic/warehouse-pharmacy'

const service = {
  page: pageWarehousePharmacies,
  create: (data: Record<string, unknown>) =>
    createWarehousePharmacy(data as WarehousePharmacySavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updateWarehousePharmacy(id, data as WarehousePharmacySavePayload),
  remove: removeWarehousePharmacy,
  setEnabled: setWarehousePharmacyEnabled
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'busNo', label: '编码', width: '120px' },
  { prop: 'name', label: '名称', 'min-width': 200 },
  { prop: 'formEnumText', label: '类型', width: '100px' },
  { prop: 'statusEnumText', label: '状态', width: '100px' },
  { prop: 'displayOrder', label: '显示顺序', width: '100px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  {
    type: 'select',
    prop: 'formEnum',
    label: '类型',
    required: true,
    rules: [{ required: true, message: '请选择类型', trigger: 'change' }],
    fprops: {
      placeholder: '请选择库房或药房',
      options: [
        { label: '库房', value: 'WAREHOUSE' },
        { label: '药房', value: 'PHARMACY' }
      ]
    } as MyFormSelectProps
  },
  {
    type: 'input',
    prop: 'name',
    label: '名称',
    required: true,
    rules: [{ required: true, message: '请输入名称', trigger: 'blur' }],
    fprops: { placeholder: '请输入库房或药房名称', clearable: true }
  },
  {
    type: 'number',
    prop: 'displayOrder',
    label: '显示顺序',
    fprops: { min: 0 } as MyFormInputNumberProps
  },
  {
    type: 'select',
    prop: 'statusEnum',
    label: '状态',
    fprops: {
      options: [
        { label: '启用', value: 'ACTIVE' },
        { label: '停用', value: 'INACTIVE' }
      ]
    } as MyFormSelectProps
  }
]

const defaultForm = { name: '', formEnum: 'WAREHOUSE', displayOrder: 0, statusEnum: 'ACTIVE' }
</script>
