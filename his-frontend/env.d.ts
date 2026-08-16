/// <reference types="vite/client" />

// .env 环境变量声明
interface ImportMetaEnv {
  readonly VITE_ASSISTANT_SERVER_URL?: string
  /** 应用标题 */
  readonly VITE_APP_TITLE: string
  /** 后端接口请求超时毫秒数 */
  readonly VITE_API_TIMEOUT: number
  /** 后端接口请求地址 */
  readonly VITE_API_URL: string
  /** 默认基础数据接口来源 */
  readonly VITE_API_PROFILE?: 'c6' | 'j4'
  /** C6 接口请求地址，未配置时使用 VITE_API_URL */
  readonly VITE_C6_API_URL?: string
  /** J4 接口请求地址，未配置时使用 VITE_API_URL */
  readonly VITE_J4_API_URL?: string
  /** 是否允许接口异常时使用 Mock 数据 */
  readonly VITE_USE_MOCK?: string
  /** 验证码请求地址 */
  readonly VITE_CAPTCHA_URL: string
  /** 验证码请求前缀 */
  readonly VITE_CAPTCHA_PREFIX: string
  /** 客户端id */
  readonly VITE_CLIENT_ID: string
  /** 认证服务器地址 */
  /** 是否启用页面登录拦截 */
  readonly VITE_AUTH_GUARD_ENABLED?: string
  /** C1 收费工作站本地开发代理 */
  readonly VITE_C1_CHARGE_SERVER_URL?: string
    /** 是否启用页面登录拦截 */
    /** C1 收费工作站本地开发代理 */
  readonly VITE_AUTH_SERVER_URL: string
  /** 住院护士站接口模式：real 使用后端，mock 使用内存数据 */
  readonly VITE_NURSE_STATION_API_MODE?: 'real' | 'mock'
  /** 住院护士站后端代理前缀或网关地址 */
  readonly VITE_NURSE_STATION_API_URL?: string
  /** 门诊护士站后端代理前缀或网关地址 */
  readonly VITE_OUTPATIENT_NURSE_STATION_API_URL?: string
  /** 门诊护士站接口模式：real 使用后端，mock 使用内存数据 */
  readonly VITE_OUTPATIENT_NURSE_STATION_API_MODE?: 'real' | 'mock'
  /** 住院医生站接口模式：real 使用后端，mock 使用内存数据 */
  readonly VITE_INPATIENT_DOCTOR_STATION_API_MODE?: 'real' | 'mock'
  /** 住院医生站后端代理前缀或网关地址 */
  readonly VITE_INPATIENT_DOCTOR_STATION_API_URL?: string
  /** 门诊医生站接口模式：real 使用后端，mock 使用内存数据 */
  readonly VITE_OUTPATIENT_DOCTOR_STATION_API_MODE?: 'real' | 'mock'
  /** 门诊医生站后端代理前缀或网关地址 */
  readonly VITE_OUTPATIENT_DOCTOR_STATION_API_URL?: string
}

interface TianaiCaptchaInstance {
  init(): void
  destroyWindow(): void
  reloadCaptcha(): void
}

declare global {
  interface Window {
    initTAC(
      resourcePath: string,
      config: Record<string, unknown>,
      style: Record<string, unknown>
    ): Promise<TianaiCaptchaInstance>
  }
}

declare module '*.vue' {
  import { DefineComponent } from 'vue'
  const component: DefineComponent<{}, {}, any>
  export default component
}

import 'vue-router'
declare module 'vue-router' {
  interface RouteMeta {
    /** 标签页名称 */
    label?: string
    /** 是否可匿名访问 */
    anonymous?: boolean
  }
}
