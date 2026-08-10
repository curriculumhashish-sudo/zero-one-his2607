import type { CustomAxiosRequestConfig, JsonVO } from '@/apis/http'
import { useHttp } from '@/plugins/http'
import { getApiBaseUrl } from './profile'

function withProfile(config?: CustomAxiosRequestConfig): CustomAxiosRequestConfig {
  return { ...config, baseURL: getApiBaseUrl() }
}

export interface ProfiledHttpClient {
  get<T = unknown>(
    url: string,
    params?: unknown,
    config?: CustomAxiosRequestConfig
  ): Promise<JsonVO<T>>
  post<T = unknown>(
    url: string,
    data?: unknown,
    config?: CustomAxiosRequestConfig
  ): Promise<JsonVO<T>>
  put<T = unknown>(
    url: string,
    data?: unknown,
    config?: CustomAxiosRequestConfig
  ): Promise<JsonVO<T>>
  patch<T = unknown>(
    url: string,
    data?: unknown,
    config?: CustomAxiosRequestConfig
  ): Promise<JsonVO<T>>
  delete<T = unknown>(
    url: string,
    data?: unknown,
    config?: CustomAxiosRequestConfig
  ): Promise<JsonVO<T>>
}

/** 使用当前 C6/J4 运行时配置创建请求代理。 */
export function useProfiledHttp(): ProfiledHttpClient {
  const http = useHttp()
  return {
    get: (url, params, config) => http.get(url, params, withProfile(config)),
    post: (url, data, config) => http.post(url, data, withProfile(config)),
    put: (url, data, config) => http.put(url, data, withProfile(config)),
    patch: (url, data, config) => http.patch(url, data, withProfile(config)),
    delete: (url, data, config) => http.delete(url, data, withProfile(config))
  }
}
