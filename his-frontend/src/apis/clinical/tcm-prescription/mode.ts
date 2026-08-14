import { isInpatientDoctorStationRealApiEnabled } from '@/apis/inpatient/doctor-station/mode'
import { isOutpatientDoctorStationRealApiEnabled } from '@/apis/outpatient/doctor-station/mode'

import type { TcmPrescriptionStationScope } from './type'

/** 中医处方按工作站场景复用对应医生站的统一接口模式。 */
export function isTcmPrescriptionRealApiEnabled(scope: TcmPrescriptionStationScope) {
  return scope === 'outpatient'
    ? isOutpatientDoctorStationRealApiEnabled
    : isInpatientDoctorStationRealApiEnabled
}
