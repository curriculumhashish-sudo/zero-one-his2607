// @vitest-environment jsdom

import { createPinia, setActivePinia } from 'pinia'
import { flushPromises, mount } from '@vue/test-utils'
import { beforeEach, describe, expect, it, vi } from 'vitest'

const mocks = vi.hoisted(() => ({
  push: vi.fn(),
  replace: vi.fn(),
  getCaptcha: vi.fn(),
  verifyCaptcha: vi.fn(),
  loginAccount: vi.fn(),
  loadAvailableTenants: vi.fn(),
  selectTenant: vi.fn(),
  loadAvailableHospitals: vi.fn(),
  selectHospital: vi.fn(),
  loadAvailableDepartments: vi.fn(),
  selectDepartment: vi.fn(),
  loadCurrentMenus: vi.fn(),
  loadCurrentUser: vi.fn(),
  logout: vi.fn()
}))

vi.mock('vue-router', async (importOriginal) => ({
  ...(await importOriginal<typeof import('vue-router')>()),
  useRouter: () => ({ push: mocks.push, replace: mocks.replace })
}))

vi.mock('@/apis/login', () => ({
  getCaptcha: mocks.getCaptcha,
  verifyCaptcha: mocks.verifyCaptcha,
  loginAccount: mocks.loginAccount,
  loadAvailableTenants: mocks.loadAvailableTenants,
  selectTenant: mocks.selectTenant,
  loadAvailableHospitals: mocks.loadAvailableHospitals,
  selectHospital: mocks.selectHospital,
  loadAvailableDepartments: mocks.loadAvailableDepartments,
  selectDepartment: mocks.selectDepartment,
  loadCurrentMenus: mocks.loadCurrentMenus,
  loadCurrentUser: mocks.loadCurrentUser,
  logout: mocks.logout
}))

import LoginView from '@/views/LoginView.vue'

describe('LoginView', () => {
  beforeEach(() => {
    localStorage.clear()
    setActivePinia(createPinia())
    vi.clearAllMocks()

    mocks.getCaptcha.mockResolvedValue({ id: 'captcha-1' })
    mocks.verifyCaptcha.mockResolvedValue(undefined)
    mocks.loginAccount.mockResolvedValue({
      accessToken: 'test-access-token',
      refreshToken: 'test-refresh-token',
      expiresIn: 7200,
      user: { id: 1, userName: 'admin' },
      configuration: { currentUser: { roles: [] } }
    })
    mocks.loadAvailableTenants.mockResolvedValue([{ id: 1001, name: '测试租户' }])
    mocks.selectTenant.mockResolvedValue({ id: 1001 })
    mocks.loadAvailableHospitals.mockResolvedValue([{ id: 'hospital-1', name: '测试医院' }])
    mocks.selectHospital.mockResolvedValue({ id: 'hospital-1' })
    mocks.loadAvailableDepartments.mockResolvedValue([])
    mocks.loadCurrentMenus.mockResolvedValue([])
    mocks.loadCurrentUser.mockResolvedValue({
      avatar: '',
      realName: '管理员',
      userId: '1',
      username: 'admin',
      desc: '',
      homePath: '/home',
      token: 'test-access-token'
    })
  })

  it('验证码成功后继续完成账号和登录上下文流程', async () => {
    const wrapper = mount(LoginView, {
      global: {
        stubs: {
          PolygonBackground: true,
          TianaiCaptcha: true,
          ElSteps: true,
          ElStep: true,
          ElForm: true,
          ElFormItem: true,
          ElInput: true,
          ElButton: true,
          ElSwitch: true,
          ElSelect: true,
          ElOption: true
        }
      }
    })

    const vm = wrapper.vm as unknown as {
      formData: { username: string; password: string }
      enableVerify: boolean
      submitAccount: () => Promise<void>
      handleCaptchaSuccess: (id: string) => Promise<void>
    }
    vm.formData.username = 'admin'
    vm.formData.password = '123456'
    vm.enableVerify = true

    await vm.submitAccount()
    await vm.handleCaptchaSuccess('captcha-1')
    await flushPromises()

    expect(mocks.verifyCaptcha).toHaveBeenCalledWith({ id: 'captcha-1' })
    expect(mocks.loginAccount).toHaveBeenCalledWith({ username: 'admin', password: '123456' })
    expect(mocks.selectTenant).toHaveBeenCalledWith(1001)
    expect(mocks.selectHospital).toHaveBeenCalledWith('hospital-1')
    expect(mocks.loadCurrentMenus).toHaveBeenCalledOnce()
    expect(mocks.loadCurrentUser).toHaveBeenCalledOnce()
    expect(mocks.replace).toHaveBeenCalledWith('/home')
  })
})
