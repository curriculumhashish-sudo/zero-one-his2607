import type { AxiosError, AxiosResponse, InternalAxiosRequestConfig } from 'axios'
import HttpClient, { type JsonVO } from '@/apis/http'
import { useUserStore } from '@/stores/user'

/** 自定义请求配置（带重试标记） */
type RetryableRequestConfig = InternalAxiosRequestConfig & { __retried?: boolean }

/** 是否正在刷新 token */
let isRefreshing = false
/** 等待 token 刷新完成的请求队列 */
let pendingQueue: Array<{ resolve: (v: any) => void; reject: (e: any) => void; config: RetryableRequestConfig }> = []

/** 读取 localStorage 里的 Authorization 头 */
function buildAuthHeader(): string | null {
  const token = localStorage.getItem('token')
  if (!token) return null
  const tokenHead = (localStorage.getItem('tokenHead') || 'Bearer').trim()
  return tokenHead ? `${tokenHead} ${token}` : token
}

/** 判断响应数据是否表示 token 过期 */
function isJwtExpired(data: unknown): boolean {
  if (typeof data === 'string') return data.indexOf('Jwt expired') >= 0 || data.indexOf('expired') >= 0
  if (data && typeof data === 'object' && 'data' in data) {
    const inner = (data as JsonVO<unknown>).data
    if (typeof inner === 'string') return inner.indexOf('Jwt expired') >= 0 || inner.indexOf('expired') >= 0
  }
  return false
}

/**
 * 创建带 401 自动刷新 token 能力的 J2 HttpClient
 * 用于 J2 各服务（workbench/sysmgr/personcenter）独立代理客户端
 */
export function createJ2HttpClient(baseURL: string): HttpClient {
  // 先声明，在拦截器闭包里引用；构造完成后赋值
  let client: HttpClient

  client = new HttpClient(
    { baseURL },
    {
      requestInterceptor: (config) => {
        const auth = buildAuthHeader()
        if (auth) config.headers['Authorization'] = auth
        return config
      },
      responseInterceptor: async (response: AxiosResponse<JsonVO<any>>) => {
        // 业务层面 401：HTTP 200 但 body.code === 401
        const data = response.data
        if (data && typeof data === 'object' && 'code' in data && data.code === 401 && isJwtExpired(data)) {
          return refreshTokenAndRetry(response.config as RetryableRequestConfig, client.getInstance())
        }
        return Promise.resolve(response)
      },
      responseErrorInterceptor: (error: AxiosError) => {
        // HTTP 层面 401
        if (error?.response?.status === 401) {
          const config = (error.config || {}) as RetryableRequestConfig
          return refreshTokenAndRetry(config, client.getInstance())
        }
        return Promise.reject(error)
      }
    }
  )

  return client
}

/** 刷新 token 并重试原始请求 */
async function refreshTokenAndRetry(
  config: RetryableRequestConfig,
  instance: ReturnType<HttpClient['getInstance']>
): Promise<any> {
  // 已重试过的请求不再重试，防止死循环
  if (config.__retried) {
    return Promise.reject(new Error('Token 刷新后请求仍失败'))
  }
  config.__retried = true

  // 如果正在刷新，排队等待
  if (isRefreshing) {
    return new Promise((resolve, reject) => {
      pendingQueue.push({ resolve, reject, config })
    })
  }

  isRefreshing = true
  try {
    const store = useUserStore()
    await store.reloadToken()
    isRefreshing = false

    // 用新 token 重试原始请求
    const auth = buildAuthHeader()
    if (auth) config.headers['Authorization'] = auth

    // 处理排队请求
    const queued = pendingQueue
    pendingQueue = []
    queued.forEach(({ config: cfg, resolve, reject }) => {
      const h = buildAuthHeader()
      if (h) cfg.headers['Authorization'] = h
      instance.request(cfg).then(resolve).catch(reject)
    })

    return instance.request(config)
  } catch (e) {
    isRefreshing = false
    pendingQueue = []
    return Promise.reject(e)
  }
}
