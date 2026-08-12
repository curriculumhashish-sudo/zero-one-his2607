export type NurseStationTemperatureWindowRange = '14d' | '3d' | '7d'

export type NurseStationTemperatureTimeSlot = '02' | '06' | '10' | '14' | '18' | '22'

export type NurseStationTemperatureSpecialEventCode =
  | 'ADMISSION'
  | 'DEATH'
  | 'DELIVERY'
  | 'DISCHARGE'
  | 'DRUG_COOLING'
  | 'EXTERNAL_EXAM'
  | 'PHYSICAL_COOLING'
  | 'PULSE_DEFICIT'
  | 'REFUSED'
  | 'SURGERY'
  | 'TEMP_NOT_RISING'
  | 'TRANSFER'

export interface NurseStationTemperatureSpecialEventVO {
  code: NurseStationTemperatureSpecialEventCode
  displayText: string
  eventTime?: string
  cooledTemperature?: string
}

export interface NurseStationTemperatureSlotVO {
  timeSlot: NurseStationTemperatureTimeSlot
  vitalSignId?: string
  temperature?: string
  pulse?: string
  heartRate?: string
  respiratory?: string
  systolicPressure?: string
  diastolicPressure?: string
  specialEvents: NurseStationTemperatureSpecialEventVO[]
}

export interface NurseStationTemperatureGridDayVO {
  recordDate: string
  slots: NurseStationTemperatureSlotVO[]
}

export interface NurseStationTemperatureSheetQueryDTO {
  encounterId: string
  patientId: string
  windowRange: NurseStationTemperatureWindowRange
  focusRecordDate?: string
}

export interface NurseStationTemperatureSheetDataVO {
  gridDays: NurseStationTemperatureGridDayVO[]
}

export type NurseStationTemperatureSaveSlotDTO = NurseStationTemperatureSlotVO

export interface NurseStationSaveTemperatureSheetDTO {
  encounterId: string
  patientId: string
  recordDate: string
  slots: NurseStationTemperatureSaveSlotDTO[]
}

export interface NurseStationTemperatureSaveSlotResultVO {
  timeSlot: NurseStationTemperatureTimeSlot
  success: boolean
  message: string
  slot?: NurseStationTemperatureSlotVO
}

// This result is a frontend aggregation of the backend single-slot responses.
export interface NurseStationSaveTemperatureSheetResultVO {
  success: boolean
  savedCount: number
  nextTimeSlot?: NurseStationTemperatureTimeSlot
  results: NurseStationTemperatureSaveSlotResultVO[]
}
