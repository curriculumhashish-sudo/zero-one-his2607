export interface PageDispenseQueueParams {
  startTime?: string
  endTime?: string
  keyword?: string
  dispenseStatusEnumIn?: ('PREPARATION' | 'IN_PROGRESS' | 'COMPLETED')[]
  pageNo?: number
  pageSize?: number
  status?: string
  pageNum?: number
}

export interface PendingDispenseEncounterQueueRowDto {
  encounterId: string
  patientId: string
  patientName: string
  genderText: string
  ageText: string
  visitNo: string
  prescriptionCount: number
  registerTime: string
  dispenseStatus: 'PREPARATION' | 'IN_PROGRESS' | 'COMPLETED'
  gender?: string
  age?: string | number
  departmentName?: string
  doctorName?: string
  visitTime?: string
  slipCount?: number
  status?: string
  serviceType?: string
  paymentType?: string
  patientNo?: string
}

export interface PageDispenseQueueResult {
  total: number
  pageNum: number
  pageSize: number
  records: PendingDispenseEncounterQueueRowDto[]
  rows?: PendingDispenseEncounterQueueRowDto[]
}

export interface PagePatientQueueResult {
  total: number
  pageNum: number
  pageSize: number
  records: {
    id: string | number
    name: string
    gender: string
    age: string | number
    serviceType: string
    patientNo: string
    visitNo: string
    visitTime: string
    departmentName: string
    status: string
    statusTone: string
    paymentType: string
  }[]
}

export interface PageDispenseSlipsParams {
  encounterId: string
  dispenseStatusEnumIn?: ('PREPARATION' | 'IN_PROGRESS' | 'COMPLETED')[]
  pageNo?: number
  pageSize?: number
}

export interface EncounterMedicationDispenseSlipRowDto {
  dispenseId: string
  prescriptionNo: string
  rxType?: 'western' | 'herbal'
  medicationName: string
  specText: string
  quantity: number
  unitName: string
  dosageText: string
  frequencyText: string
  routeText: string
  days: number
  totalPrice: number
  statusEnum:
    | 'PENDING'
    | 'PREPARATION'
    | 'IN_PROGRESS'
    | 'PARTIAL'
    | 'COMPLETED'
    | 'BLOCKED'
    | 'ERROR'
  pharmacyName: string
  lotNumber?: string
  traceNo?: string
  factoryName?: string
  inventoryDetailList?: {
    pharmacyOrganizationId: string
    pharmacyName: string
    lotNumber: string
    availableQuantity: number
  }[]
}

export interface DispenseSlipItemDto {
  itemId: string
  slipId: string
  drugId: string
  itemName: string
  spec: string
  dosage: string
  route: string
  frequency: string
  days: number
  quantity: number
  unit: string
  price: number
  totalAmount: number
  status: string
  inventoryLots: InventoryLotDto[]
  selectedInventoryId: string
  selectedLotNo: string
  pickedQty: number
}

export interface DispenseSlipDto {
  slipId: string
  encounterId: string
  patientId: string
  patientName: string
  patientNo: string
  visitNo: string
  doctorName: string
  departmentName: string
  createTime: string
  status: string
  items: DispenseSlipItemDto[]
}

export interface InventoryLotDto {
  inventoryId: string
  inventoryName: string
  lotNo: string
  batchNo: string
  expiryDate: string
  availableQty: number
  unit: string
  price: number
}

export interface PharmacyDispenserDto {
  dispenserId: string
  name: string
  code: string
}

export interface TraceCodeConfirmResult {
  success: boolean
  drugName: string
  lotNo: string
  expiryDate: string
}

export interface PrintPrescriptionParams {
  encounterId: string
  prescriptionIds: string[]
}

export interface SavePreparationParams {
  encounterId: string
  preparerPractitionerId: string
  lines: {
    dispenseId: string
    dispenseLocationId: string
    lotNumber?: string | null
    quantity: string
    traceNo?: string | null
  }[]
}

export interface ConfirmDispenseParams {
  encounterId: string
  contractBusNo?: string
  lines: {
    dispenseId: string
  }[]
}

export interface PractitionerOptionDto {
  practitionerId: number | string
  displayName: string
}

export interface PharmacyOptionDto {
  pharmacyId: string
  pharmacyName: string
}
