<template>
  <BasicCrudPage
    title="患者"
    description="参考患者主档维护姓名、证件、联系方式、血型、地址及联系人信息"
    :service="service"
    :columns="columns"
    :form-items="formItems"
    :default-form="defaultForm"
    dialog-width="820px"
  />
</template>

<script setup lang="ts">
import BasicCrudPage from '@/views/basic/components/BasicCrudPage.vue'
import type { BasicCrudService } from '@/views/basic/components/basic-crud'
import type {
  MyFormDatePickerProps,
  MyFormInputNumberProps,
  MyFormItemAttr,
  MyFormSelectProps
} from '@/components/myform/type'
import type { MyTableColumn } from '@/components/mytable/type'
import {
  createPatient,
  getPatientDetail,
  pagePatients,
  removePatient,
  updatePatient
} from '@/apis/basic/patient'
import type { PatientSavePayload } from '@/apis/basic/patient'

const service = {
  page: pagePatients,
  detail: getPatientDetail,
  create: (data: Record<string, unknown>) => createPatient(data as PatientSavePayload),
  update: (id: string, data: Record<string, unknown>) =>
    updatePatient(id, data as PatientSavePayload),
  remove: removePatient
} as unknown as BasicCrudService

const columns: MyTableColumn[] = [
  { prop: 'busNo', label: '病历号', width: '120px' },
  { prop: 'name', label: '姓名', width: '100px' },
  { prop: 'age', label: '年龄', width: '70px', align: 'center' },
  { prop: 'genderEnumText', label: '性别', width: '70px', align: 'center' },
  { prop: 'phone', label: '电话', width: '140px' },
  { prop: 'idCard', label: '证件号', 'min-width': 190, 'show-overflow-tooltip': true },
  { prop: 'birthDate', label: '出生日期', width: '120px' },
  { prop: 'address', label: '详细地址', 'min-width': 180, 'show-overflow-tooltip': true }
]

const formItems: MyFormItemAttr[] = [
  requiredInput('name', '姓名', '请输入姓名'),
  select('genderEnum', '性别', [
    { label: '男', value: 'MALE' },
    { label: '女', value: 'FEMALE' },
    { label: '未知', value: 'UNKNOWN' }
  ], true),
  requiredInput('idCard', '证件号', '请输入居民身份证或其他证件号'),
  input('phone', '电话', '请输入联系电话'),
  number('age', '年龄', 0, 150),
  date('birthDate', '出生日期'),
  input('nationalityCode', '国籍', '如 CN'),
  input('workCompany', '工作单位', '请输入工作单位'),
  select('maritalStatusEnum', '婚姻状况', [
    { label: '未婚', value: 'UNMARRIED' },
    { label: '已婚', value: 'MARRIED' },
    { label: '未知', value: 'UNKNOWN' }
  ]),
  select('bloodAbo', 'ABO 血型', ['A', 'B', 'AB', 'O', '未知'].map((value) => ({ label: value, value }))),
  select('bloodRh', 'Rh 血型', [
    { label: '阳性', value: 'POSITIVE' },
    { label: '阴性', value: 'NEGATIVE' },
    { label: '未知', value: 'UNKNOWN' }
  ]),
  input('addressProvince', '省份', '请输入省份'),
  input('addressCity', '城市', '请输入城市'),
  input('addressDistrict', '区县', '请输入区县'),
  input('address', '详细地址', '请输入详细地址'),
  input('linkName', '联系人姓名', '请输入紧急联系人'),
  input('linkTelcom', '联系人电话', '请输入联系人电话'),
  select('activeFlag', '状态', [
    { label: '启用', value: true },
    { label: '停用', value: false }
  ])
]

const defaultForm = {
  name: '',
  genderEnum: 'UNKNOWN',
  age: 0,
  birthDate: '',
  phone: '',
  idCard: '',
  nationalityCode: 'CN',
  workCompany: '',
  maritalStatusEnum: 'UNKNOWN',
  bloodAbo: '未知',
  bloodRh: 'UNKNOWN',
  addressProvince: '',
  addressCity: '',
  addressDistrict: '',
  address: '',
  linkName: '',
  linkTelcom: '',
  activeFlag: true
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
  options: Array<{ label: string; value: string | boolean }>,
  required = false
): MyFormItemAttr {
  return {
    type: 'select',
    prop,
    label,
    required,
    rules: required ? [{ required: true, message: `请选择${label}`, trigger: 'change' }] : undefined,
    fprops: { placeholder: `请选择${label}`, clearable: true, options } as MyFormSelectProps
  }
}
function number(prop: string, label: string, min: number, max: number): MyFormItemAttr {
  return {
    type: 'number',
    prop,
    label,
    fprops: { min, max, placeholder: `请输入${label}` } as MyFormInputNumberProps
  }
}
function date(prop: string, label: string): MyFormItemAttr {
  return {
    type: 'date',
    prop,
    label,
    fprops: {
      type: 'date',
      'value-format': 'YYYY-MM-DD',
      placeholder: `请选择${label}`
    } as MyFormDatePickerProps
  }
}
</script>
