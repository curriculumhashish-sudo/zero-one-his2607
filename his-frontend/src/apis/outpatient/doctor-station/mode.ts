/** Shared Mock/Real switch for every outpatient doctor-station API. */
export type OutpatientDoctorStationApiMode = 'mock' | 'real'

const configuredMode = String(import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_MODE || '').toLowerCase()

/** Invalid or missing values deliberately fall back to Mock for safe local startup. */
export const outpatientDoctorStationApiMode: OutpatientDoctorStationApiMode =
  configuredMode === 'real' ? 'real' : 'mock'

export const isOutpatientDoctorStationRealApiEnabled =
  outpatientDoctorStationApiMode === 'real'

export const isOutpatientDoctorStationMockApiEnabled =
  !isOutpatientDoctorStationRealApiEnabled

/** All real outpatient doctor-station requests share this base URL. */
export const outpatientDoctorStationApiBaseURL =
  import.meta.env.VITE_OUTPATIENT_DOCTOR_STATION_API_URL || '/api'

