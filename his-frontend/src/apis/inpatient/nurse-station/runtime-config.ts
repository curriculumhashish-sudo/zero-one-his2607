/** 住院护士站支持的接口来源。默认使用 Mock，避免未配置时意外请求真实后端。 */
export type NurseStationApiMode = 'mock' | 'real'

/** 统一读取护士站接口模式；只有显式填写 real 才会访问后端。 */
export const nurseStationApiMode: NurseStationApiMode =
  import.meta.env.VITE_NURSE_STATION_API_MODE === 'real' ? 'real' : 'mock'

/** 所有住院护士站真实适配器共享的代理前缀或网关地址。 */
export const realApiBaseURL = import.meta.env.VITE_NURSE_STATION_API_URL || '/api'

/** 页面门面使用的统一 Real 判断。 */
export const isNurseStationRealApiEnabled = nurseStationApiMode === 'real'
