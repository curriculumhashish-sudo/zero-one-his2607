// @vitest-environment jsdom

import { flushPromises, mount } from '@vue/test-utils'
import dayjs from 'dayjs'
import { defineComponent } from 'vue'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import Index from './Index.vue'

const layoutStub = {
  template: '<div><slot name="left" /><slot name="main" /><slot name="right" /></div>'
}

const reloadTcmPrescription = vi.fn(async () => undefined)

const tcmWorkspaceStub = defineComponent({
  name: 'ClinicalTcmPrescriptionWorkspace',
  props: {
    context: { type: Object, default: null },
    patientSelected: Boolean,
    accepted: Boolean,
    readonly: Boolean,
    showSaveAction: { type: Boolean, default: true },
    showRefreshAction: Boolean,
    autoSave: Boolean
  },
  emits: ['dirtyChange', 'loadingChange'],
  setup(_props, { expose }) {
    expose({
      reload: reloadTcmPrescription,
      isDirty: () => false,
      validate: () => ''
    })
    return {}
  },
  template: '<div class="tcm-workspace-stub" />'
})

const stationStubs = {
  StationTriPaneLayout: layoutStub,
  DoctorQueueStatusFilter: true,
  PatientQueuePanel: true,
  PatientInfoBar: true,
  DoctorStationHeader: true,
  MedicalRecordPanel: true,
  ClinicalDiagnosisEditor: true,
  ClinicalOrderEditor: true,
  ClinicalTcmPrescriptionWorkspace: tcmWorkspaceStub,
  DoctorStationAssistantPanel: true,
  AdviceApplicationFormDialog: true,
  AdviceOrderHistoryDrawer: true,
  DiagnosisHistoryDrawer: true,
  InpatientAdmissionDialog: true,
  RefundApplicationDialog: true,
  PrescriptionSheetDialog: true,
  MedicalRecordHistoryDialog: true
}

function mountStation() {
  return mount(Index, {
    global: {
      stubs: stationStubs
    }
  })
}

describe('OutpatientDoctorStation composable assembly', () => {
  beforeEach(() => {
    vi.useFakeTimers()
    reloadTcmPrescription.mockClear()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('进入页面后加载队列，选择患者后由 composable 加载完整上下文', async () => {
    const wrapper = mountStation()

    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patients = queue.props('patients')
    expect(patients.length).toBeGreaterThan(0)
    expect(queue.props('date')).toBe(dayjs().format('YYYY-MM-DD'))
    expect(queue.props('showRefresh')).toBe(false)
    expect(queue.props('showDepartment')).toBe(false)
    expect(queue.props('showPrimaryNumber')).toBe(true)
    expect(queue.props('primaryNumberMode')).toBe('patient-no')
    expect(
      patients.every((patient: { paymentType?: string }) =>
        ['医保', '自费'].includes(patient.paymentType ?? '')
      )
    ).toBe(true)
    expect(
      patients.every((patient: { visitTime: string }) =>
        patient.visitTime.startsWith(dayjs().format('YYYY-MM-DD'))
      )
    ).toBe(true)

    queue.vm.$emit('select', patients[0])
    await vi.runAllTimersAsync()
    await flushPromises()

    const assistant = wrapper.getComponent({ name: 'DoctorStationAssistantPanel' })
    expect(assistant.props('context')?.encounterId).toBe(patients[0].id)
    expect(assistant.props('patient')?.id).toBe(patients[0].id)

    const patientInfo = wrapper.getComponent({ name: 'PatientInfoBar' })
    expect(patientInfo.props('displayMode')).toBe('outpatient-registration')
    expect(patientInfo.props('phone')).toBe(assistant.props('context')?.phone)
    expect(patientInfo.props('doctorName')).toBe(assistant.props('context')?.attendingDoctorName)

    wrapper.unmount()
  })

  it('切换查询日期后保留正在处理的患者，并加载所选单日的完整队列', async () => {
    const wrapper = mountStation()

    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const selectedPatient = queue.props('patients')[0]
    queue.vm.$emit('select', selectedPatient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const yesterday = dayjs().subtract(1, 'day').format('YYYY-MM-DD')
    queue.vm.$emit('single-date-change', yesterday)
    await vi.runAllTimersAsync()
    await flushPromises()

    expect(queue.props('date')).toBe(yesterday)
    expect(queue.props('patients').length).toBeGreaterThan(0)
    expect(
      queue
        .props('patients')
        .every((patient: { visitTime: string }) => patient.visitTime.startsWith(yesterday))
    ).toBe(true)

    const assistant = wrapper.getComponent({ name: 'DoctorStationAssistantPanel' })
    expect(assistant.props('context')?.encounterId).toBe(selectedPatient.id)

    wrapper.unmount()
  })

  it('进入历史病历页签后按患者加载列表，并按需打开只读详情', async () => {
    const wrapper = mountStation()

    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    expect(patient).toBeTruthy()

    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const assistant = wrapper.getComponent({ name: 'DoctorStationAssistantPanel' })
    assistant.vm.$emit('history-active-change', true)
    await vi.runAllTimersAsync()
    await flushPromises()

    expect(assistant.props('historyRows')).toHaveLength(10)
    expect(assistant.props('historyTotal')).toBe(12)

    assistant.vm.$emit('select-history', assistant.props('historyRows')[0])
    await vi.runAllTimersAsync()
    await flushPromises()

    const dialog = wrapper.getComponent({ name: 'MedicalRecordHistoryDialog' })
    expect(dialog.props('modelValue')).toBe(true)
    expect(dialog.props('record')?.encounterId).toBe(assistant.props('historyRows')[0].encounterId)

    wrapper.unmount()
  })

  it('医嘱编辑器隐藏查询栏、保留分页，并可按需打开患者历史医嘱', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const editor = wrapper.getComponent({ name: 'ClinicalOrderEditor' })
    expect(editor.props('showFilters')).toBe(false)
    expect(editor.props('updatedAt')).toBe('')
    expect(editor.props('page')).toBe(1)
    expect(editor.props('pageSize')).toBe(10)
    expect(editor.props('total')).toBe(2)

    editor.vm.$emit('open-history')
    await vi.runAllTimersAsync()
    await flushPromises()

    const drawer = wrapper.getComponent({ name: 'AdviceOrderHistoryDrawer' })
    expect(drawer.props('modelValue')).toBe(true)
    expect(drawer.props('encounters').length).toBeGreaterThan(0)
    expect(
      drawer
        .props('encounters')
        .every((item: { patientId: string }) => item.patientId === 'patient-1003')
    ).toBe(true)

    wrapper.unmount()
  })

  it('复用公共工作区加载门诊中医处方，并通过页签内保存按钮保存', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const workspace = wrapper.getComponent({ name: 'ClinicalTcmPrescriptionWorkspace' })
    expect(workspace.props('context')).toEqual({
      scope: 'outpatient',
      encounterId: 'enc-20260723-003',
      patientId: 'patient-1003'
    })
    expect(workspace.props('accepted')).toBe(true)
    expect(workspace.props('showSaveAction')).toBe(true)
    expect(workspace.props('showRefreshAction')).toBe(false)
    expect(workspace.props('autoSave')).toBe(false)

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    expect(header.props('tabs')).toContainEqual({ key: 'tcmPrescription', label: '中医处方' })
    wrapper.unmount()
  })

  it('保存病历下放到病历区域，头部展示全部流程操作且不再提供刷新动作', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const actionKeys = header.props('actions').map((item: { key: string }) => item.key)
    expect(actionKeys).toEqual([
      'accept',
      'complete',
      'leave',
      'refund',
      'prescription',
      'admission'
    ])
    expect(actionKeys).not.toContain('saveRecord')
    expect(actionKeys).not.toContain('refresh')
    expect(header.props('maxVisible')).toBe(actionKeys.length)

    const recordPanel = wrapper.getComponent({ name: 'MedicalRecordPanel' })
    expect(recordPanel.props('saving')).toBe(false)
    wrapper.unmount()
  })

  it('诊断编辑器可按需查询当前患者历史诊断', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const editor = wrapper.getComponent({ name: 'ClinicalDiagnosisEditor' })
    editor.vm.$emit('open-history')
    await vi.runAllTimersAsync()
    await flushPromises()

    const drawer = wrapper.getComponent({ name: 'DiagnosisHistoryDrawer' })
    expect(drawer.props('modelValue')).toBe(true)
    expect(drawer.props('rows').length).toBeGreaterThan(0)
    expect(
      drawer.props('rows').every((item: { patientId: string }) => item.patientId === 'patient-1003')
    ).toBe(true)

    drawer.vm.$emit('query-change', { keyword: '高血压' }, true)
    await vi.runAllTimersAsync()
    await flushPromises()
    expect(drawer.props('page')).toBe(1)
    expect(drawer.props('rows').length).toBeGreaterThan(0)
    expect(
      drawer.props('rows').every((item: { name: string }) => item.name.includes('高血压'))
    ).toBe(true)

    wrapper.unmount()
  })

  it('新增诊断满足唯一主诊断规则后自动提交', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-demo-in-progress')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const editor = wrapper.getComponent({ name: 'ClinicalDiagnosisEditor' })
    const diagnosis = {
      key: 'draft-auto-diagnosis',
      catalogId: 'diag-I209',
      code: 'I20.9',
      name: '心绞痛',
      typeName: '西医诊断',
      primary: false
    }
    const nextDiagnoses = [...editor.props('modelValue'), diagnosis]
    editor.vm.$emit('update:modelValue', nextDiagnoses)
    editor.vm.$emit('change', nextDiagnoses)
    await vi.runAllTimersAsync()
    await flushPromises()

    expect(editor.props('dirty')).toBe(false)
    expect(
      editor
        .props('modelValue')
        .find((item: { catalogId: string }) => item.catalogId === 'diag-I209')?.recordId
    ).toBeTruthy()
    wrapper.unmount()
  })

  it('新增、修改和删除医嘱草稿在校验通过后自动提交', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-010')
    expect(patient).toBeTruthy()
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const editor = wrapper.getComponent({ name: 'ClinicalOrderEditor' })
    const draft = {
      key: 'draft-unified-save',
      catalogId: 'catalog-unified-save',
      code: 'JY-TEST',
      name: '自动保存测试检验',
      category: 'LAB',
      categoryName: '检验',
      quantity: 1,
      quantityUnit: '次',
      status: 'DRAFT'
    }

    editor.vm.$emit('update:modelValue', [draft])
    editor.vm.$emit('change', [draft])
    await vi.runAllTimersAsync()
    await flushPromises()
    expect(editor.props('dirty')).toBe(false)
    expect(editor.props('modelValue')[0]?.recordId).toBeTruthy()

    const updated = [{ ...editor.props('modelValue')[0], quantity: 2 }]
    editor.vm.$emit('update:modelValue', updated)
    editor.vm.$emit('change', updated)
    await vi.runAllTimersAsync()
    await flushPromises()
    expect(editor.props('modelValue')[0]?.quantity).toBe(2)

    editor.vm.$emit('update:modelValue', [])
    editor.vm.$emit('change', [])
    await vi.runAllTimersAsync()
    await flushPromises()
    expect(editor.props('modelValue')).toEqual([])
    expect(editor.props('total')).toBe(0)

    wrapper.unmount()
  })

  it('在诊患者诊疗数据已保存时可以打开办理住院弹窗', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    expect(patient).toBeTruthy()

    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const admissionAction = header
      .props('actions')
      .find((action: { key: string }) => action.key === 'admission')
    expect(admissionAction?.disabled).toBe(false)

    header.vm.$emit('action', admissionAction)
    await wrapper.vm.$nextTick()

    expect(wrapper.getComponent({ name: 'InpatientAdmissionDialog' }).props('modelValue')).toBe(
      true
    )
    wrapper.unmount()
  })

  it('没有已保存主诊断时定位诊断页签且不打开办理住院弹窗', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-010')
    expect(patient).toBeTruthy()

    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const admissionAction = header
      .props('actions')
      .find((action: { key: string }) => action.key === 'admission')
    header.vm.$emit('action', admissionAction)
    await wrapper.vm.$nextTick()

    expect(header.props('activeTab')).toBe('diagnosis')
    expect(wrapper.getComponent({ name: 'InpatientAdmissionDialog' }).props('modelValue')).toBe(
      false
    )
    wrapper.unmount()
  })

  it('诊断存在未保存修改时阻止办理住院', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const diagnosisEditor = wrapper.getComponent({ name: 'ClinicalDiagnosisEditor' })
    const changedDiagnoses = diagnosisEditor.props('modelValue').map((diagnosis: object) => ({
      ...diagnosis
    }))
    changedDiagnoses[0] = {
      ...changedDiagnoses[0],
      name: `${changedDiagnoses[0].name}（修改未保存）`
    }
    diagnosisEditor.vm.$emit('update:modelValue', changedDiagnoses)
    await wrapper.vm.$nextTick()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const admissionAction = header
      .props('actions')
      .find((action: { key: string }) => action.key === 'admission')
    header.vm.$emit('action', admissionAction)
    await wrapper.vm.$nextTick()

    expect(header.props('activeTab')).toBe('diagnosis')
    expect(wrapper.getComponent({ name: 'InpatientAdmissionDialog' }).props('modelValue')).toBe(
      false
    )
    wrapper.unmount()
  })

  it('选中带病历号的患者后可以打开退费申请弹窗', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-003')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const refundAction = header
      .props('actions')
      .find((action: { key: string }) => action.key === 'refund')
    expect(refundAction?.disabled).toBe(false)

    header.vm.$emit('action', refundAction)
    await wrapper.vm.$nextTick()

    const dialog = wrapper.getComponent({ name: 'RefundApplicationDialog' })
    expect(dialog.props('modelValue')).toBe(true)
    expect(dialog.props('patient')?.patientNo).toBe('MR20260003')
    wrapper.unmount()
  })

  it('选中带病历号的患者后可以打开处方单弹窗', async () => {
    const wrapper = mountStation()
    await vi.runAllTimersAsync()
    await flushPromises()

    const queue = wrapper.getComponent({ name: 'PatientQueuePanel' })
    const patient = queue
      .props('patients')
      .find((item: { id: string }) => item.id === 'enc-20260723-008')
    queue.vm.$emit('select', patient)
    await vi.runAllTimersAsync()
    await flushPromises()

    const header = wrapper.getComponent({ name: 'DoctorStationHeader' })
    const prescriptionAction = header
      .props('actions')
      .find((action: { key: string }) => action.key === 'prescription')
    expect(prescriptionAction?.disabled).toBe(false)

    header.vm.$emit('action', prescriptionAction)
    await wrapper.vm.$nextTick()

    const dialog = wrapper.getComponent({ name: 'PrescriptionSheetDialog' })
    expect(dialog.props('modelValue')).toBe(true)
    expect(dialog.props('patient')?.patientNo).toBe('MR20260008')
    wrapper.unmount()
  })
})
