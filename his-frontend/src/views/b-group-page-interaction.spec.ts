// @vitest-environment jsdom

import { defineComponent } from 'vue'
import { flushPromises, shallowMount } from '@vue/test-utils'
import { beforeEach, describe, expect, it, vi } from 'vitest'
import { ElMessageBox } from 'element-plus'

const loginApi = vi.hoisted(() => ({
  getCaptcha: vi.fn(),
  verifyCaptcha: vi.fn(),
  loginAccount: vi.fn(),
  loadAvailableTenants: vi.fn(),
  selectTenant: vi.fn(),
  loadAvailableHospitals: vi.fn(),
  selectHospital: vi.fn(),
  loadAvailableDepartments: vi.fn(),
  selectDepartment: vi.fn()
}))

const chargeApi = vi.hoisted(() => ({
  pageChargePatientQueue: vi.fn(),
  getPendingChargeDetail: vi.fn(),
  preSettleOutpatientCharge: vi.fn(),
  settleOutpatientCharge: vi.fn(),
  switchChargeInsuranceMode: vi.fn(),
  queryPaymentResult: vi.fn(),
  recoverSettlement: vi.fn(),
  getChargeReceipt: vi.fn()
}))

const refundApi = vi.hoisted(() => ({
  pageRefundPatientQueue: vi.fn(),
  getRefundableExecutionDetail: vi.fn(),
  verifyOutpatientRefund: vi.fn(),
  refundOutpatientCharge: vi.fn(),
  switchRetainedItemsInsuranceMode: vi.fn(),
  queryRefundResult: vi.fn(),
  recoverRefund: vi.fn()
}))

const userStore = vi.hoisted(() => ({
  setToken: vi.fn(),
  setLoginContext: vi.fn(),
  loadUser: vi.fn(),
  loadMenus: vi.fn(),
  setLoaded: vi.fn()
}))
const tabStore = vi.hoisted(() => ({ reset: vi.fn() }))
const router = vi.hoisted(() => ({ replace: vi.fn() }))

vi.mock('@/apis/login', () => loginApi)
vi.mock('@/apis/outpatient/charge/charge', () => chargeApi)
vi.mock('@/apis/outpatient/charge/refund', () => refundApi)
vi.mock('@/stores/user', () => ({ useUserStore: () => userStore }))
vi.mock('@/stores/tab', () => ({ useTabStore: () => tabStore }))
vi.mock('vue-router', () => ({ useRouter: () => router }))

import LoginView from './LoginView.vue'
import OutpatientChargePage from './outpatient/charge/charge/Index.vue'
import OutpatientRefundPage from './outpatient/charge/refund/Index.vue'

const ButtonStub = defineComponent({
  name: 'ElButton',
  props: ['disabled', 'loading'],
  emits: ['click'],
  template: '<button :disabled="disabled" @click="$emit(\'click\')"><slot /></button>'
})
const FormStub = defineComponent({
  name: 'ElForm',
  template: '<form><slot /></form>'
})
const FormItemStub = defineComponent({
  name: 'ElFormItem',
  template: '<div><slot /></div>'
})
const CaptchaStub = defineComponent({
  name: 'TianaiCaptcha',
  props: ['modelValue'],
  emits: ['update:modelValue', 'success', 'error'],
  template: '<div class="captcha-stub" />'
})
const ToolbarStub = defineComponent({
  name: 'ActionToolbar',
  props: ['actions'],
  emits: ['action'],
  template: '<div class="toolbar-stub" />'
})
const PatientQueueStub = defineComponent({
  name: 'PatientQueuePanel',
  emits: ['search', 'date-change', 'refresh', 'select'],
  template: '<div class="patient-queue-stub" />'
})
const DataTableStub = defineComponent({
  name: 'DataTablePanel',
  props: ['data'],
  emits: ['selectable'],
  template: '<div class="data-table-stub"><slot /></div>'
})
const PaginationStub = defineComponent({
  name: 'PaginationBar',
  template: '<div class="pagination-stub" />'
})
const SettlementDialogStub = defineComponent({
  name: 'SettlementDialog',
  emits: ['confirm'],
  setup(_, { expose }) {
    expose({ open: vi.fn(), close: vi.fn() })
    return () => null
  }
})
const RefundDialogStub = defineComponent({
  name: 'RefundSettlementDialog',
  emits: ['confirm'],
  setup(_, { expose }) {
    expose({ open: vi.fn(), close: vi.fn() })
    return () => null
  }
})

const patient = {
  id: 'enc-1',
  encounterId: 'enc-1',
  patientNo: 'P-1',
  visitNo: 'V-1',
  name: '测试患者',
  gender: '男',
  age: 30,
  departmentName: '心内科',
  visitTime: '2026-08-04 09:00:00',
  status: 'pending',
  paymentType: '自费'
}

const commonPageStubs = {
  ActionToolbar: ToolbarStub,
  PatientQueuePanel: PatientQueueStub,
  DataTablePanel: DataTableStub,
  PaginationBar: PaginationStub,
  PatientInfoBar: true,
  DetailSummary: true,
  EmptyState: true,
  SettlementDialog: SettlementDialogStub,
  RefundSettlementDialog: RefundDialogStub,
  PrintActionPanel: true,
  StatusDisplay: true,
  ElAlert: true,
  ElButton: ButtonStub,
  ElSegmented: true,
  ElDatePicker: true,
  ElDialog: true,
  ElDescriptions: true,
  ElDescriptionsItem: true,
  ElTable: true,
  ElTableColumn: true,
  ElTag: true
}

beforeEach(() => {
  vi.clearAllMocks()
  loginApi.getCaptcha.mockResolvedValue({ id: 'captcha-1' })
  loginApi.verifyCaptcha.mockResolvedValue(undefined)
  loginApi.loginAccount.mockResolvedValue({
    accessToken: 'token',
    refreshToken: 'refresh',
    expiresIn: 3600,
    user: { id: 1 },
    configuration: { currentUser: { roles: ['reporter'] } }
  })
  loginApi.loadAvailableTenants.mockResolvedValue([{ id: 1, name: '租户' }])
  loginApi.selectTenant.mockResolvedValue(undefined)
  loginApi.loadAvailableHospitals.mockResolvedValue([{ id: 'hospital-1', name: '医院' }])
  loginApi.selectHospital.mockResolvedValue(undefined)
  loginApi.loadAvailableDepartments.mockResolvedValue([])
  loginApi.selectDepartment.mockResolvedValue(undefined)
  userStore.loadUser.mockResolvedValue(undefined)
  userStore.loadMenus.mockResolvedValue(undefined)
  router.replace.mockResolvedValue(undefined)

  chargeApi.pageChargePatientQueue.mockResolvedValue({ rows: [patient], total: 1 })
  chargeApi.getPendingChargeDetail.mockResolvedValue({
    encounterId: 'enc-1',
    patient,
    items: [],
    insuranceMode: 'self-pay',
    summary: { totalAmount: 0, insuranceAmount: 0, selfPayAmount: 0 }
  })
  chargeApi.preSettleOutpatientCharge.mockResolvedValue({ paymentDraftId: 'draft-1', displayAmount: 0 })
  refundApi.pageRefundPatientQueue.mockResolvedValue({ rows: [], total: 0 })
})

describe('B组页面关键点击交互', () => {
  it('登录按钮始终先打开图片验证码，验证码成功后才提交账号', async () => {
    const wrapper = shallowMount(LoginView, {
      global: { stubs: { PolygonBackground: true, TianaiCaptcha: CaptchaStub, ElButton: ButtonStub, ElForm: FormStub, ElFormItem: FormItemStub, ElInput: true, ElSteps: true, ElStep: true, ElSelect: true, ElOption: true } }
    })
    const vm = wrapper.vm as unknown as { formData: { username: string; password: string } }
    vm.formData.username = 'admin'
    vm.formData.password = '123456'
    await wrapper.get('.primary-action').trigger('click')
    await flushPromises()
    expect(loginApi.getCaptcha).toHaveBeenCalledOnce()
    expect(loginApi.loginAccount).not.toHaveBeenCalled()

    wrapper.findComponent(CaptchaStub).vm.$emit('success', 'captcha-1')
    await flushPromises()
    expect(loginApi.verifyCaptcha).toHaveBeenCalledWith({ id: 'captcha-1' })
    expect(loginApi.loginAccount).toHaveBeenCalledWith({ username: 'admin', password: '123456' })
  })

  it('门诊收费队列点击加载时发送统一 queueTabCode 和分页字段', async () => {
    const wrapper = shallowMount(OutpatientChargePage, {
      global: { stubs: commonPageStubs, directives: { loading: () => undefined } }
    })
    await flushPromises()
    expect(chargeApi.pageChargePatientQueue).toHaveBeenCalledWith({
      keyword: '',
      startDate: null,
      endDate: null,
      queueTabCode: 'PENDING',
      pageIndex: 1,
      pageSize: 10
    })
    wrapper.findComponent(PatientQueueStub).vm.$emit('select', { id: 'enc-1' })
    await flushPromises()
    expect(chargeApi.getPendingChargeDetail).toHaveBeenCalledWith('enc-1')
  })

  it('门诊退费队列点击加载时发送统一 queueTabCode 和分页字段', async () => {
    shallowMount(OutpatientRefundPage, {
      global: { stubs: commonPageStubs, directives: { loading: () => undefined } }
    })
    await flushPromises()
    expect(refundApi.pageRefundPatientQueue).toHaveBeenCalledWith({
      keyword: '',
      startDate: null,
      endDate: null,
      queueTabCode: 'PENDING',
      pageIndex: 1,
      pageSize: 10
    })
  })
})
