export type InpatientDoctorStationApiMode = 'mock' | 'real'

const configuredMode = import.meta.env.VITE_INPATIENT_DOCTOR_STATION_API_MODE

function resolveInpatientDoctorStationApiMode(
  mode: string | undefined
): InpatientDoctorStationApiMode {
  if (mode === 'real' || mode === 'mock') return mode

  if (import.meta.env.DEV) {
    const reason = mode
      ? `未知的住院医生站接口模式“${mode}”`
      : '未配置住院医生站接口模式'
    console.warn(`${reason}，已安全回退为 mock。请使用 real 或 mock。`)
  }
  return 'mock'
}

/** 住院医生站唯一的接口模式来源。修改环境变量后需要重启 Vite。 */
export const inpatientDoctorStationApiMode =
  resolveInpatientDoctorStationApiMode(configuredMode)

/** 是否启用住院医生站真实接口。 */
export const isInpatientDoctorStationRealApiEnabled =
  inpatientDoctorStationApiMode === 'real'

/**
 * 当前后端接口契约提供的住院医生站能力。
 * 文书类型和医嘱目录尚无真实接口，Real 模式下必须明确禁用。
 */
export const inpatientDoctorStationCapabilities = Object.freeze({
  medicalRecordDocumentTypes: !isInpatientDoctorStationRealApiEnabled,
  /** Real mode reuses the outpatient doctor-station catalog endpoint. */
  medicalOrderCatalog: true
})

export function createUnsupportedInpatientDoctorStationCapabilityError(
  capabilityName: string
): Error {
  return new Error(`后端暂未提供${capabilityName}接口，Real 模式下无法使用该功能`)
}
