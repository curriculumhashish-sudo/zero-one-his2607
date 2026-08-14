<template>
  <BasicCrudPage
    title="药房项目配置"
    description="维护开立组织对应的药品、耗材和设备分发配置。"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="860px"
  />
</template>

<script setup lang="ts">
import type { MyFormInputNumberProps, MyFormItemAttr, MyFormSelectProps } from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import { pageMedicationLocationRules, saveMedicationLocationRules } from '@/apis/configuration/medication-location'
import type { MedicationLocationRuleRecord, MedicationLocationRuleSavePayload } from '@/apis/configuration/medication-location'
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudRecord, BasicCrudService } from '@/views/basic/components/basic-crud'

const organizationId = import.meta.env.VITE_DEV_ORGANIZATION_ID || ''

const service = {
  page: (query: { pageIndex: number; pageSize: number; keyword?: string }) =>
    pageMedicationLocationRules({ ...query, organizationId }),
  async create(data: Record<string, unknown>) {
    const rows = await loadAllRules()
    const row = { ...(data as unknown as MedicationLocationRuleRecord), id: '' }
    await saveRows([...rows, row])
    return { ...row, id: `new-${Date.now()}` } as unknown as BasicCrudRecord
  },
  async update(id: string, data: Record<string, unknown>) {
    const rows = await loadAllRules()
    const next = rows.map((row) => row.id === id ? { ...(data as unknown as MedicationLocationRuleRecord), id } : row)
    await saveRows(next)
    return { ...(data as unknown as MedicationLocationRuleRecord), id } as unknown as BasicCrudRecord
  },
  async remove(id: string) {
    const rows = await loadAllRules()
    await saveRows(rows.filter((row) => row.id !== id))
  }
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'organizationName', label: '开立组织', width: '150px' },
  { prop: 'itemTable', label: '项目表', width: '210px' },
  { prop: 'distributionCategoryCode', label: '分发分类', width: '120px' },
  { prop: 'defLocationName', label: '默认分发地点', width: '160px' },
  { prop: 'itemDefinitionName', label: '项目名称', width: '170px' },
  { prop: 'priority', label: '优先级', width: '90px' }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('organizationId', '开立组织 ID', '请输入组织机构 ID'),
  requiredInput('itemTable', '项目表', '例如 med_medication_definition'),
  select('distributionCategoryCode', '分发分类', [
    { label: '西药', value: '1' }, { label: '中药', value: '2' }, { label: '耗材/设备', value: '11' }
  ]),
  requiredInput('defLocationId', '默认分发地点 ID', '请输入地点 ID'),
  input('itemDefinitionId', '具体项目 ID', '留空表示整类配置'),
  input('methodCode', '分发方式编码', '可选'),
  numberInput('priority', '优先级', 1),
  numberInput('displayOrder', '显示顺序', 0),
  input('startTime', '开始时间', 'HH:mm'),
  input('endTime', '结束时间', 'HH:mm'),
  input('remark', '备注', '可选')
]

const defaultForm = {
  organizationId, itemTable: 'med_medication_definition', distributionCategoryCode: '1', defLocationId: '',
  itemDefinitionId: '', methodCode: '', priority: 1, displayOrder: 0, startTime: '00:00', endTime: '23:59', remark: ''
}

async function loadAllRules() {
  const result = await pageMedicationLocationRules({ organizationId, pageIndex: 1, pageSize: 1000 })
  return result.rows as MedicationLocationRuleRecord[]
}
async function saveRows(rows: MedicationLocationRuleRecord[]) {
  const payload: MedicationLocationRuleSavePayload = { organizationId, checkedItems: rows }
  await saveMedicationLocationRules(payload)
}
function input(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { type: 'input', prop, label, fprops: { placeholder, clearable: true } }
}
function requiredInput(prop: string, label: string, placeholder: string): MyFormItemAttr {
  return { ...input(prop, label, placeholder), required: true, rules: [{ required: true, message: placeholder, trigger: 'blur' }] }
}
function numberInput(prop: string, label: string, min: number): MyFormItemAttr {
  return { type: 'number', prop, label, fprops: { min, max: 999999 } as MyFormInputNumberProps }
}
function select(prop: string, label: string, options: Array<{ label: string; value: string }>): MyFormItemAttr {
  return { type: 'select', prop, label, fprops: { placeholder: `请选择${label}`, options } as MyFormSelectProps }
}
</script>
