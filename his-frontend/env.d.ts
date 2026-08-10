/// <reference types="vite/client" />

// .env 环境变量声明
interface ImportMetaEnv {
  /** 应用标题 */
  readonly VITE_APP_TITLE: string
  /** 后端接口请求超时毫秒数 */
  readonly VITE_API_TIMEOUT: number
  /** 后端接口请求地址 */
  readonly VITE_API_URL: string
  /** 验证码请求地址 */
  readonly VITE_CAPTCHA_URL: string
  /** 验证码请求前缀 */
  readonly VITE_CAPTCHA_PREFIX: string
  /** 客户端id */
  readonly VITE_CLIENT_ID: string
  /** 认证服务器地址 */
    readonly VITE_AUTH_SERVER_URL: string
    /** 是否启用页面登录拦截 */
    readonly VITE_AUTH_GUARD_ENABLED: string
    /** C1 收费工作站本地开发代理 */
    readonly VITE_C1_CHARGE_SERVER_URL: string
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
