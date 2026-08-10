// @vitest-environment jsdom

import { flushPromises, mount } from '@vue/test-utils'
import { ElDialog, ElMessageBox } from 'element-plus'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import type {
  ClinicalDiagnosisDictionaryLoader,
  ClinicalDiagnosisItem
} from '@/components/clinical/diagnosis'
import type { PatientSummary } from '@/components/patient/type'

import InpatientAdmissionDialog from './InpatientAdmissionDialog.vue'

const apiMocks = vi.hoisted(() => ({
  checkInpatientAdmission: vi.fn(),
  listInpatientAdmissionWards: vi.fn(),
  loadInpatientAdmissionOptions: vi.fn(),
  requestInpatientAdmission: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<Record<string, unknown>>()),
  ...apiMocks
}))

const patient: PatientSummary = {
  id: 'enc-dialog-test',
  name: '张建国',
  gender: '男',
  age: 56,
  patientNo: 'MR20260001',
  visitNo: 'OP20260001',
  departmentName: '心血管内科',
  serviceType: '普通门诊',
  status: '在诊'
}

const context = {
  encounterId: 'enc-dialog-test',
  patientId: 'patient-dialog-test',
  visitStatus: 'in-progress' as const,
  visitStatusName: '在诊',
  attendingDoctorName: '李尔王',
  acceptedAt: '2026-07-26 08:10',
  chiefComplaint: '胸闷',
  pastMedicalHistory: '高血压',
  phone: '138****1001',
  emergencyContact: '家属'
}

const diagnoses: ClinicalDiagnosisItem[] = [
  {
    key: 'diagnosis-saved-1',
    recordId: 'diagnosis-saved-1',
    catalogId: 'diagnosis-hypertension',
    code: 'I10',
    name: '高血压',
    typeName: '西医诊断',
    primary: true
  }
]

const loadDiagnosisDictionary: ClinicalDiagnosisDictionaryLoader = vi.fn(async () => ({
  rows: [
    {
      id: 'diagnosis-hypertension',
      code: 'I10',
      name: '高血压',
      typeName: '西医诊断'
    }
  ],
  total: 1
}))

const formOptions = {
  departmentOptions: [
    { value: 'inpatient-dept-cardio', label: '心血管内科', code: 'I-CARDIO' },
    { value: 'inpatient-dept-neuro', label: '神经内科', code: 'I-NEURO' }
  ],
  admissionTypeOptions: [{ value: 'ELECTIVE', label: '择期住院' }],
  admissionWayOptions: [{ value: 'OUTPATIENT_TRANSFER', label: '门诊转住院' }],
  priorityOptions: [{ value: 'NORMAL', label: '普通' }]
}

/**
 * 单元测试只关心 v-model 和 change 事件。
 * 使用轻量桩避免手动触发真实 ElSelect 内部监听器造成递归更新。
 */
const selectStub = {
  name: 'ElSelect',
  props: ['modelValue', 'disabled', 'loading', 'placeholder'],
  emits: ['update:modelValue', 'change'],
  template: '<div class="el-select-stub"><slot /></div>'
}

async function mountOpenedDialog() {
  const wrapper = mount(InpatientAdmissionDialog, {
    props: {
      modelValue: true,
      patient,
      context,
      diagnoses,
      loadDiagnosisDictionary
    },
    global: {
      stubs: {
        teleport: true,
        ElSelect: selectStub,
        ElOption: true
      }
    }
  })

  wrapper.getComponent(ElDialog).vm.$emit('open')
  await flushPromises()
  return wrapper
}

function findButtonByText(wrapper: ReturnType<typeof mount>, text: string) {
  return wrapper.findAll('button').find((button) => button.text().includes(text))
}

describe('InpatientAdmissionDialog', () => {
  beforeEach(() => {
    apiMocks.checkInpatientAdmission.mockResolvedValue({ exists: false })
    apiMocks.loadInpatientAdmissionOptions.mockResolvedValue(formOptions)
    apiMocks.listInpatientAdmissionWards.mockResolvedValue([
      {
        value: 'ward-cardio-1',
        label: '心内一病区',
        departmentId: 'inpatient-dept-cardio'
      }
    ])
    apiMocks.requestInpatientAdmission.mockResolvedValue({
      success: true,
      applicationId: 'application-test',
      inpatientEncounterId: 'inpatient-test',
      inpatientEncounterNo: 'IP202607260001',
      status: 'PLANNED',
      statusName: '待住院登记'
    })
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('展示患者摘要，并预填当前已保存主诊断', async () => {
    const wrapper = await mountOpenedDialog()

    expect(wrapper.text()).toContain('张建国')
    expect(wrapper.text()).toContain('李尔王')
    expect(wrapper.findComponent({ name: 'RemoteSelectField' }).props('modelValue')).toBe(
      'diagnosis-hypertension'
    )
  })

  it('切换入院科室时清空病区并重新加载', async () => {
    const wrapper = await mountOpenedDialog()
    let selects = wrapper.findAllComponents({ name: 'ElSelect' })
    const departmentSelect = selects[0]!

    departmentSelect.vm.$emit('update:modelValue', 'inpatient-dept-cardio')
    departmentSelect.vm.$emit('change', 'inpatient-dept-cardio')
    await flushPromises()

    selects = wrapper.findAllComponents({ name: 'ElSelect' })
    const wardSelect = selects[1]!
    wardSelect.vm.$emit('update:modelValue', 'ward-cardio-1')
    await wrapper.vm.$nextTick()
    expect(wardSelect.props('modelValue')).toBe('ward-cardio-1')

    departmentSelect.vm.$emit('update:modelValue', 'inpatient-dept-neuro')
    departmentSelect.vm.$emit('change', 'inpatient-dept-neuro')
    await flushPromises()

    expect(apiMocks.listInpatientAdmissionWards).toHaveBeenLastCalledWith('inpatient-dept-neuro')
    expect(wrapper.findAllComponents({ name: 'ElSelect' })[1]?.props('modelValue')).toBe('')
  })

  it('未选择入院科室时不提交，填写后提交完整申请', async () => {
    const wrapper = await mountOpenedDialog()
    const submitButton = findButtonByText(wrapper, '提交申请')
    expect(submitButton).toBeTruthy()

    await submitButton!.trigger('click')
    await flushPromises()
    expect(apiMocks.requestInpatientAdmission).not.toHaveBeenCalled()

    const departmentSelect = wrapper.findAllComponents({ name: 'ElSelect' })[0]!
    departmentSelect.vm.$emit('update:modelValue', 'inpatient-dept-cardio')
    departmentSelect.vm.$emit('change', 'inpatient-dept-cardio')
    await flushPromises()

    await submitButton!.trigger('click')
    await flushPromises()

    expect(apiMocks.requestInpatientAdmission).toHaveBeenCalledWith(
      expect.objectContaining({
        encounterId: context.encounterId,
        patientId: context.patientId,
        targetDepartmentId: 'inpatient-dept-cardio',
        admissionWayCode: 'OUTPATIENT_TRANSFER',
        admissionDiagnoses: [
          expect.objectContaining({
            catalogId: 'diagnosis-hypertension',
            diagnosisRecordId: 'diagnosis-saved-1',
            primary: true
          })
        ]
      })
    )
    expect(wrapper.emitted('submitted')?.[0]?.[0]).toEqual(
      expect.objectContaining({ status: 'PLANNED' })
    )
    expect(wrapper.emitted('update:modelValue')).toContainEqual([false])
  })

  it('已有住院申请时展示提示并禁用提交', async () => {
    apiMocks.checkInpatientAdmission.mockResolvedValue({
      exists: true,
      inpatientEncounterNo: 'IP202607260099',
      status: 'PLANNED',
      statusName: '待住院登记'
    })
    const wrapper = await mountOpenedDialog()

    expect(wrapper.text()).toContain('IP202607260099')
    expect(findButtonByText(wrapper, '提交申请')?.attributes('disabled')).toBeDefined()
  })

  it('关闭已修改的申请表单前进行二次确认', async () => {
    // Element Plus 当前版本把 MessageBoxData 声明成对象与字符串字面量的交叉类型，
    // 测试只关心 Promise 成功分支，因此使用 never 绕过这个上游类型问题。
    const confirm = vi.spyOn(ElMessageBox, 'confirm').mockResolvedValue('confirm' as never)
    const wrapper = await mountOpenedDialog()
    const departmentSelect = wrapper.findAllComponents({ name: 'ElSelect' })[0]!
    departmentSelect.vm.$emit('update:modelValue', 'inpatient-dept-cardio')
    departmentSelect.vm.$emit('change', 'inpatient-dept-cardio')
    await flushPromises()

    await findButtonByText(wrapper, '取消')!.trigger('click')
    await flushPromises()

    expect(confirm).toHaveBeenCalledOnce()
    expect(wrapper.emitted('update:modelValue')).toContainEqual([false])
  })
})
