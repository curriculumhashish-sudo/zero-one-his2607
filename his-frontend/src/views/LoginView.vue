<script setup lang="ts">
import type {
  CaptchaResult,
  LoginOrganizationOption,
  LoginResult,
  OrganizationInfo,
  TenantInfo
} from '@/apis/login'

import { computed, reactive, ref } from 'vue'
import { ElMessage } from 'element-plus'
import { ArrowLeft, Lock, OfficeBuilding, User } from '@element-plus/icons-vue'
import { useRoute, useRouter } from 'vue-router'

import {
  getCaptcha,
  loadAvailableDepartments,
  loadAvailableHospitals,
  loadAvailableTenants,
  loginAccount,
  selectDepartment,
  selectHospital,
  selectTenant,
  verifyCaptcha
} from '@/apis/login'
import PolygonBackground from '@/components/background/PolygonBackground.vue'
import ArithmeticCaptcha from '@/components/captcha/ArithmeticCaptcha.vue'
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'
import { useTabStore } from '@/stores/tab'
import { useUserStore } from '@/stores/user'

defineOptions({ name: 'LoginView' })

type LoginStep = 'account' | 'tenant' | 'hospital' | 'department'

const router = useRouter()
const route = useRoute()
const userStore = useUserStore()
const tabStore = useTabStore()
const formData = reactive({ username: '', password: '' })
const step = ref<LoginStep>('account')
const loading = ref(false)
const isDevelopment = import.meta.env.DEV
const enableVerify = ref(true)
const useArithmeticCaptcha = import.meta.env.VITE_CAPTCHA_MODE === 'arithmetic'
const showCaptcha = ref(false)
const loginResult = ref<LoginResult>()
const tenants = ref<TenantInfo[]>([])
const hospitals = ref<LoginOrganizationOption[]>([])
const departments = ref<OrganizationInfo[]>([])
const captchaData = ref<CaptchaResult>()
const captchaImage = ref('')
const pendingCaptcha = ref<{ id: string; code: string }>()
const selectedTenantId = ref<number | string>()
const selectedHospitalId = ref<number | string>()
const selectedDepartmentId = ref<string>()

const stepTitle = computed(() => ({ account: '账号登录', tenant: '选择租户', hospital: '选择医院', department: '选择机构（科室）' })[step.value])
const currentStep = computed(() => ({ account: 0, tenant: 1, hospital: 2, department: 3 })[step.value])

async function submitAccount() {
  if (loading.value) return
  if (!formData.username.trim() || !formData.password) {
    ElMessage.warning('请输入账号和密码')
    return
  }
  if (!enableVerify.value) {
    await completeAccountLogin()
    return
  }
  try {
    loading.value = true
    captchaData.value = await getCaptcha()
    captchaImage.value = captchaData.value.imageBase64 ?? ''
    pendingCaptcha.value = undefined
    showCaptcha.value = true
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '验证码加载失败')
  } finally {
    loading.value = false
  }
}

async function refreshCaptcha() {
  try {
    captchaData.value = await getCaptcha()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '验证码加载失败')
  }
}

async function handleCaptchaSuccess(payload: string | { id: string; code: string }) {
  const id = typeof payload === 'string' ? payload : payload.id
  const code = typeof payload === 'string' ? (useArithmeticCaptcha ? payload : '') : payload.code
  try {
    loading.value = true
    await verifyCaptcha(code ? { id, code } : { id })
    pendingCaptcha.value = { id, code }
    await completeAccountLogin()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '验证码验证失败')
  } finally {
    loading.value = false
  }
}

async function completeAccountLogin() {
  loading.value = true
  try {
    const result = await loginAccount({
      username: formData.username.trim(),
      password: formData.password,
      ...(pendingCaptcha.value?.code
        ? { captchaUuid: pendingCaptcha.value.id, code: pendingCaptcha.value.code }
        : {})
    })
    loginResult.value = result
    userStore.setToken(result)
    tenants.value = await loadAvailableTenants()
    if (!tenants.value.length) throw new Error('当前账号没有可用租户')
    if (tenants.value.length === 1) {
      selectedTenantId.value = tenants.value[0]!.id
      await confirmTenant()
    } else {
      step.value = 'tenant'
    }
  } catch (error) {
    step.value = 'account'
    ElMessage.error(error instanceof Error ? error.message : '登录失败，请重试')
  } finally {
    loading.value = false
  }
}

async function confirmTenant() {
  if (!selectedTenantId.value) return
  loading.value = true
  try {
    await selectTenant(selectedTenantId.value)
    userStore.setLoginContext({ tenantId: selectedTenantId.value, hospitalId: null, departmentId: null })
    hospitals.value = await loadAvailableHospitals()
    if (!hospitals.value.length) {
      departments.value = await loadAvailableDepartments()
      if (!departments.value.length) {
        await finishLogin()
        return
      }
      if (departments.value.length === 1) {
        selectedDepartmentId.value = departments.value[0]!.id
        loading.value = false
        await confirmDepartment()
      } else {
        step.value = 'department'
      }
      return
    }
    if (!hospitals.value.length) throw new Error('当前租户没有可用医院')
    if (hospitals.value.length === 1) {
      selectedHospitalId.value = hospitals.value[0]!.id
      loading.value = false
      await confirmHospital()
    } else {
      step.value = 'hospital'
    }
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '租户选择失败，请重试')
  } finally {
    loading.value = false
  }
}

async function confirmHospital() {
  if (!selectedHospitalId.value) return
  loading.value = true
  try {
    await selectHospital(selectedHospitalId.value)
    userStore.setLoginContext({ hospitalId: selectedHospitalId.value, departmentId: null })
    departments.value = await loadAvailableDepartments()
    if (!departments.value.length) {
      await finishLogin()
      return
    }
    if (departments.value.length === 1) {
      selectedDepartmentId.value = departments.value[0]!.id
      loading.value = false
      await confirmDepartment()
    } else {
      step.value = 'department'
    }
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '医院选择失败，请重试')
  } finally {
    loading.value = false
  }
}

async function confirmDepartment() {
  if (!selectedDepartmentId.value) return
  loading.value = true
  try {
    await selectDepartment(selectedDepartmentId.value)
    userStore.setLoginContext({ departmentId: selectedDepartmentId.value })
    await finishLogin()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '机构（科室）选择失败，请重试')
  } finally {
    loading.value = false
  }
}

async function finishLogin() {
  await Promise.all([userStore.loadUser(), userStore.loadMenus()])
  userStore.setLoaded(true)
  tabStore.reset()
  // 以当前用户返回的首页路径为准；没有返回时使用系统首页。
  // `/home` 会由主路由统一重定向到仪表盘，避免登录页和主框架使用两套首页入口。
  const redirect = typeof route?.query?.redirect === 'string' ? route.query.redirect : ''
  const targetPath = redirect.startsWith('/') && !redirect.startsWith('//') ? redirect : ''
  const homePath = userStore.getUser?.homePath
  await router.replace(targetPath || (homePath && homePath.startsWith('/') ? homePath : '/home'))
  ElMessage.success('登录成功')
}

function backStep() {
  if (step.value === 'department') step.value = 'hospital'
  else if (step.value === 'hospital') step.value = 'tenant'
  else step.value = 'account'
}
</script>

<template>
  <main class="login-page">
    <PolygonBackground />
    <section class="login-panel">
      <header class="login-panel__header">
        <div class="brand-mark">01</div>
        <div><h1>零壹 HIS</h1><p>{{ stepTitle }}</p></div>
      </header>

      <el-steps :active="currentStep" finish-status="success" simple class="login-steps">
        <el-step title="账号" />
        <el-step title="租户" />
        <el-step title="医院" />
        <el-step title="科室" />
      </el-steps>

      <el-form v-if="step === 'account'" :model="formData" class="login-form" @submit.prevent="submitAccount">
        <el-form-item><el-input v-model="formData.username" autocomplete="username" placeholder="账号" :prefix-icon="User" clearable @keyup.enter="submitAccount" /></el-form-item>
        <el-form-item><el-input v-model="formData.password" autocomplete="current-password" type="password" placeholder="密码" :prefix-icon="Lock" show-password @keyup.enter="submitAccount" /></el-form-item>
        <div v-if="isDevelopment" class="captcha-switch"><span>登录验证码</span><el-switch v-model="enableVerify" /></div>
        <el-button native-type="button" type="primary" class="primary-action" :loading="loading" @click="submitAccount">登录</el-button>
      </el-form>

      <div v-else class="context-form">
        <el-select v-if="step === 'tenant'" v-model="selectedTenantId" placeholder="请选择租户" size="large"><el-option v-for="item in tenants" :key="item.id" :label="item.name" :value="item.id" /></el-select>
        <el-select v-if="step === 'hospital'" v-model="selectedHospitalId" placeholder="请选择医院" size="large"><el-option v-for="item in hospitals" :key="item.id" :label="item.displayName ?? item.name" :value="item.id" /></el-select>
        <el-select v-if="step === 'department'" v-model="selectedDepartmentId" placeholder="请选择机构（科室）" size="large"><el-option v-for="item in departments" :key="item.id" :label="item.displayName" :value="item.id" :disabled="!item.isEnabled" /></el-select>
        <div class="context-actions">
          <el-button :icon="ArrowLeft" :disabled="loading" @click="backStep">返回</el-button>
          <el-button v-if="step === 'tenant'" type="primary" :loading="loading" :disabled="!selectedTenantId" @click="confirmTenant">确认租户</el-button>
          <el-button v-if="step === 'hospital'" type="primary" :loading="loading" :disabled="!selectedHospitalId" @click="confirmHospital">确认医院</el-button>
          <el-button v-if="step === 'department'" type="primary" :loading="loading" :disabled="!selectedDepartmentId" @click="confirmDepartment">进入系统</el-button>
        </div>
      </div>

      <div class="login-panel__footer"><OfficeBuilding /><span>零壹医院信息系统</span></div>
    </section>
    <TianaiCaptcha
      v-if="!useArithmeticCaptcha"
      v-model="showCaptcha"
      mode="popup"
      type="WORD_IMAGE_CLICK"
      :captcha="captchaData"
      @refresh="refreshCaptcha"
      @success="handleCaptchaSuccess"
      @error="ElMessage.error('验证码加载失败，请重试')"
    />
    <ArithmeticCaptcha
      v-else
      v-model="showCaptcha"
      :image="captchaImage"
      @success="handleCaptchaSuccess"
      @error="ElMessage.error('验证码验证失败，请重试')"
    />
  </main>
</template>

<style scoped>
.login-page { display: grid; min-height: 100vh; place-items: center; overflow: hidden; color: #111827; }
.login-panel { position: relative; z-index: 2; width: min(92vw, 560px); padding: 32px; border: 1px solid rgb(255 255 255 / 70%); border-radius: 10px; background: rgb(255 255 255 / 94%); box-shadow: 0 18px 50px rgb(15 23 42 / 18%); }
.login-panel__header { display: flex; align-items: center; gap: 14px; margin-bottom: 22px; }
.brand-mark { display: grid; width: 48px; height: 48px; place-items: center; border-radius: 8px; background: #1d4ed8; color: #fff; font-size: 20px; font-weight: 800; }
.login-panel__header h1 { margin: 0; color: #111827; font-size: 24px; letter-spacing: 0; }
.login-panel__header p { margin: 4px 0 0; color: #4b5563; font-size: 14px; }
.login-steps { margin-bottom: 26px; }
.login-form, .context-form { display: flex; flex-direction: column; gap: 14px; }
.login-form :deep(.el-form-item) { margin-bottom: 0; }
.login-form :deep(.el-input__wrapper), .context-form :deep(.el-select__wrapper) { min-height: 46px; }
.captcha-switch { display: flex; align-items: center; justify-content: space-between; color: #374151; font-size: 14px; }
.primary-action { width: 100%; height: 46px; font-weight: 700; }
.context-form :deep(.el-select) { width: 100%; }
.context-actions { display: flex; justify-content: space-between; gap: 12px; }
.login-panel__footer { display: flex; align-items: center; justify-content: center; gap: 6px; margin-top: 24px; color: #6b7280; font-size: 13px; }
.login-panel__footer :deep(svg) { width: 16px; height: 16px; flex: none; }
@media (max-width: 560px) { .login-panel { padding: 24px 18px; } .login-steps :deep(.el-step__title) { font-size: 12px; } }
</style>
