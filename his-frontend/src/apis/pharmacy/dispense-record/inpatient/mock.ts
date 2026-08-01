import type {
  InpatientDispenseRecordInitDTO,
  InpatientDispenseRecordItemDTO,
  PageInpatientDispenseRecordsParams,
  PageInpatientDispenseRecordsResult,
  ExportInpatientDispenseRecordsResult
} from './type'

function getMonthRange(): [string, string] {
  const now = new Date()
  const y = now.getFullYear()
  const m = String(now.getMonth() + 1).padStart(2, '0')
  const firstDay = `${y}-${m}-01`
  const lastDay = new Date(y, now.getMonth() + 1, 0).getDate()
  const lastDayStr = String(lastDay).padStart(2, '0')
  return [firstDay, `${y}-${m}-${lastDayStr}`]
}

const initData: InpatientDispenseRecordInitDTO = {
  pharmacyOptions: [
    { label: '住院西药房', value: 'west' },
    { label: '住院中药房', value: 'chinese' }
  ],
  categoryOptions: [
    { label: '西药', value: 'western' },
    { label: '中成药', value: 'patent' },
    { label: '中草药', value: 'herb' }
  ],
  dispenserOptions: [
    { label: '张药师', value: 'u001' },
    { label: '李药师', value: 'u002' },
    { label: '王药师', value: 'u003' }
  ],
  recordTypeOptions: [
    { label: '发药', value: 'DISPENSE' },
    { label: '退药', value: 'REFUND' }
  ],
  defaultDateRange: getMonthRange()
}

const pharmacies: Record<string, string> = {
  west: '住院西药房',
  chinese: '住院中药房'
}

const dispensers: Record<string, string> = {
  u001: '张药师',
  u002: '李药师',
  u003: '王药师'
}

const departments = ['心血管内科', '呼吸内科', '消化内科', '神经内科', '骨科', '外科']
const wards = ['内科一区', '内科二区', '外科一区', '外科二区', '重症监护室']
const drugNames = [
  { name: '阿司匹林肠溶片', spec: '100mg*30片', unit: '盒', price: 12.5, dosage: '口服' },
  { name: '硝苯地平控释片', spec: '30mg*7片', unit: '盒', price: 28.0, dosage: '口服' },
  { name: '阿托伐他汀钙片', spec: '20mg*7片', unit: '盒', price: 35.8, dosage: '口服' },
  { name: '头孢呋辛钠', spec: '1.5g/支', unit: '支', price: 18.0, dosage: '静滴' },
  { name: '奥美拉唑肠溶胶囊', spec: '20mg*14粒', unit: '盒', price: 16.8, dosage: '口服' },
  { name: '氨溴索注射液', spec: '15mg/2ml', unit: '支', price: 9.5, dosage: '静推' },
  { name: '复方甘草片', spec: '100片', unit: '瓶', price: 8.5, dosage: '口服' },
  { name: '生理盐水', spec: '500ml', unit: '袋', price: 3.5, dosage: '静滴' },
  { name: '葡萄糖注射液', spec: '500ml', unit: '袋', price: 4.0, dosage: '静滴' },
  { name: '胰岛素注射液', spec: '300IU/3ml', unit: '支', price: 68.0, dosage: '皮下注射' }
]

const patientNames = ['陈建国', '李秀英', '王志强', '刘雅琴', '赵大海', '孙美丽', '周文斌', '吴秀英', '郑伟', '冯建华']

let allRecords: InpatientDispenseRecordItemDTO[] = []

function generateRecords(): InpatientDispenseRecordItemDTO[] {
  const records: InpatientDispenseRecordItemDTO[] = []
  const today = new Date()
  const [firstDay] = getMonthRange()
  const startDate = new Date(firstDay)

  let idCounter = 1
  for (let dayOffset = 0; dayOffset < today.getDate(); dayOffset++) {
    const currentDate = new Date(startDate)
    currentDate.setDate(currentDate.getDate() + dayOffset)
    if (currentDate > today) break

    const dateStr = currentDate.toISOString().slice(0, 10)
    const isToday = dayOffset === today.getDate() - 1
    const recordsPerDay = isToday ? 6 : 2 + Math.floor(Math.random() * 3)

    for (let i = 0; i < recordsPerDay; i++) {
      const recordType: 'DISPENSE' | 'REFUND' = Math.random() > 0.75 ? 'REFUND' : 'DISPENSE'
      const drug = drugNames[Math.floor(Math.random() * drugNames.length)]
      const patientIdx = Math.floor(Math.random() * patientNames.length)
      const deptIdx = Math.floor(Math.random() * departments.length)
      const wardIdx = Math.floor(Math.random() * wards.length)
      const bedNo = `${String(Math.floor(Math.random() * 20) + 1).padStart(2, '0')}`
      const pharmacyKeys = Object.keys(pharmacies)
      const pharmacyKey = pharmacyKeys[Math.floor(Math.random() * pharmacyKeys.length)]!
      const dispenserKeys = Object.keys(dispensers)
      const dispenserKey = dispenserKeys[Math.floor(Math.random() * dispenserKeys.length)]!
      const qty = Math.floor(Math.random() * 3) + 1
      const hour = String(8 + Math.floor(Math.random() * 12)).padStart(2, '0')
      const minute = String(Math.floor(Math.random() * 60)).padStart(2, '0')

      records.push({
        id: `R${String(idCounter).padStart(6, '0')}`,
        recordId: `REC${String(idCounter).padStart(6, '0')}`,
        prescriptionNo: `RX${dateStr.replace(/-/g, '')}${String(Math.floor(idCounter / 2) + 1).padStart(4, '0')}`,
        recordType,
        patientId: `P${String(patientIdx + 1).padStart(4, '0')}`,
        patientName: patientNames[patientIdx]!,
        inpatientNo: `ZY${dateStr.replace(/-/g, '')}${String(idCounter).padStart(4, '0')}`,
        departmentName: departments[deptIdx]!,
        wardName: wards[wardIdx]!,
        bedNo,
        dispenseTime: `${dateStr} ${hour}:${minute}:00`,
        pharmacyName: pharmacies[pharmacyKey]!,
        dispenserName: dispensers[dispenserKey]!,
        drugName: drug!.name,
        spec: drug!.spec,
        quantity: qty,
        unit: drug!.unit,
        price: drug!.price,
        amount: Math.round(drug!.price * qty * 100) / 100,
        batchNo: `B${dateStr.replace(/-/g, '')}${String(Math.floor(Math.random() * 100)).padStart(3, '0')}`,
        traceCode: `TC${String(idCounter).padStart(12, '0')}`,
        dosage: `${Math.floor(Math.random() * 3) + 1}次/日`,
        summaryNo: recordType === 'DISPENSE' ? `SUM${dateStr.replace(/-/g, '')}${String(Math.floor(idCounter / 4) + 1).padStart(4, '0')}` : ''
      })
      idCounter++
    }
  }

  return records.sort((a, b) => b.dispenseTime.localeCompare(a.dispenseTime))
}

function ensureRecordsInitialized() {
  if (allRecords.length === 0) {
    allRecords = generateRecords()
  }
}

export async function loadInpatientDispenseRecordInit(): Promise<InpatientDispenseRecordInitDTO> {
  await new Promise((r) => setTimeout(r, 200))
  return { ...initData }
}

export async function pageInpatientDispenseRecords(
  params: PageInpatientDispenseRecordsParams
): Promise<PageInpatientDispenseRecordsResult> {
  await new Promise((r) => setTimeout(r, 300))
  ensureRecordsInitialized()

  let filtered = [...allRecords]

  if (params.startDate) {
    filtered = filtered.filter((r) => r.dispenseTime >= `${params.startDate} 00:00:00`)
  }
  if (params.endDate) {
    filtered = filtered.filter((r) => r.dispenseTime <= `${params.endDate} 23:59:59`)
  }
  if (params.recordType) {
    filtered = filtered.filter((r) => r.recordType === params.recordType)
  }
  if (params.pharmacyId && pharmacies[params.pharmacyId]) {
    filtered = filtered.filter((r) => r.pharmacyName === pharmacies[params.pharmacyId!])
  }
  if (params.dispenserId && dispensers[params.dispenserId]) {
    filtered = filtered.filter((r) => r.dispenserName === dispensers[params.dispenserId!])
  }
  if (params.drugKeyword) {
    const kw = params.drugKeyword.toLowerCase()
    filtered = filtered.filter((r) => r.drugName.toLowerCase().includes(kw))
  }
  if (params.patientKeyword) {
    const kw = params.patientKeyword.toLowerCase()
    filtered = filtered.filter(
      (r) =>
        r.patientName.toLowerCase().includes(kw) ||
        r.inpatientNo.toLowerCase().includes(kw)
    )
  }

  const total = filtered.length
  const start = (params.pageIndex - 1) * params.pageSize
  const records = filtered.slice(start, start + params.pageSize)

  return {
    records,
    total,
    pageIndex: params.pageIndex,
    pageSize: params.pageSize
  }
}

export async function exportInpatientDispenseRecords(
  params: PageInpatientDispenseRecordsParams
): Promise<ExportInpatientDispenseRecordsResult> {
  await new Promise((r) => setTimeout(r, 500))
  ensureRecordsInitialized()

  let filtered = [...allRecords]
  if (params.startDate) {
    filtered = filtered.filter((r) => r.dispenseTime >= `${params.startDate} 00:00:00`)
  }
  if (params.endDate) {
    filtered = filtered.filter((r) => r.dispenseTime <= `${params.endDate} 23:59:59`)
  }
  if (params.recordType) {
    filtered = filtered.filter((r) => r.recordType === params.recordType)
  }
  if (params.drugKeyword) {
    filtered = filtered.filter((r) => r.drugName.includes(params.drugKeyword!))
  }
  if (params.patientKeyword) {
    filtered = filtered.filter(
      (r) =>
        r.patientName.includes(params.patientKeyword!) ||
        r.inpatientNo.includes(params.patientKeyword!)
    )
  }

  if (filtered.length === 0) {
    return { success: false, message: '导出数据为空，请调整筛选条件后重试' }
  }
  if (filtered.length > 5000) {
    return { success: false, message: `导出数据超过上限(5000条)，当前${filtered.length}条，请缩小筛选范围` }
  }

  return { success: true, message: `导出成功，共${filtered.length}条记录` }
}
