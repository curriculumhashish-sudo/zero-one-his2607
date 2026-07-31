<template>
  <BasicCrudPage
    title="取药科室"
    description="维护科室开立项目后对应的取药地点。保存时按当前规则集整体提交，符合原型接口的保存方式。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="780px"
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
  pageMedicationLocationRules,
  saveMedicationLocationRules
} from '@/apis/configuration/medication-location'
import type { MedicationLocationRuleRecord } from '@/apis/configuration/medication-location'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudRecord, BasicCrudService } from '@/views/basic/components/basic-crud'

const service = {
  page: pageMedicationLocationRules,
  async create(data: Record<string, unknown>) {
    const rows = await loadAllRules()
    const id = crypto.randomUUID()
    const saved = await saveMedicationLocationRules([
      ...rows,
      { ...(data as unknown as MedicationLocationRuleRecord), id }
    ])
    return findSavedRule(saved, id)
  },
  async update(id: string, data: Record<string, unknown>) {
    const rows = await loadAllRules()
    const saved = await saveMedicationLocationRules(
      rows.map((row) =>
        row.id === id ? { ...(data as unknown as MedicationLocationRuleRecord), id } : row
      )
    )
    return findSavedRule(saved, id)
  },
  async remove(id: string) {
    const rows = await loadAllRules()
    await saveMedicationLocationRules(rows.filter((row) => row.id !== id))
  }
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'departmentName', label: '开立科室', width: '140px' },
  { prop: 'itemType', label: '项目类型', width: '110px' },
  { prop: 'itemName', label: '项目名称', width: '180px' },
  { prop: 'pharmacyName', label: '取药地点', width: '150px' },
  { prop: 'routeName', label: '给药途径', width: '110px' },
  { prop: 'priority', label: '优先级', width: '90px', align: 'center' },
  { prop: 'enabled', label: '启用状态', width: '100px', align: 'center' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('departmentName', '开立科室', '请输入开立科室'),
  select('itemType', '项目类型', [
    { label: '西药', value: 'DRUG' },
    { label: '中药', value: 'HERBAL' },
    { label: '耗材', value: 'DEVICE' }
  ]),
  requiredInput('itemName', '项目名称', '请输入项目名称'),
  requiredInput('pharmacyName', '取药地点', '请输入药房或库房名称'),
  input('routeName', '给药途径', '例如：口服、外用'),
  {
    type: 'number',
    prop: 'priority',
    label: '优先级',
    fprops: { min: 1, max: 99 } as MyFormInputNumberProps
  },
  booleanSelect('enabled', '启用状态')
]

const defaultForm = {
  departmentName: '',
  itemType: 'DRUG',
  itemName: '',
  pharmacyName: '',
  routeName: '口服',
  priority: 1,
  enabled: true
}

async function loadAllRules() {
  const firstPage = await pageMedicationLocationRules({ pageIndex: 1, pageSize: 1000, keyword: '' })
  return firstPage.rows
}

function findSavedRule(rows: MedicationLocationRuleRecord[], id: string): BasicCrudRecord {
  const row = rows.find((item) => item.id === id)
  if (!row) throw new Error('取药科室规则保存失败')
  return row as unknown as BasicCrudRecord
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
