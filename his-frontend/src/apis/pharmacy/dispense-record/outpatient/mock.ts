import type {
  OutpatientDispenseRecordInitDTO,
  OutpatientDispenseRecordItemDTO,
  PageOutpatientDispenseRecordsParams,
  PageOutpatientDispenseRecordsResult,
  ExportOutpatientDispenseRecordsResult
} from './type'

function getMonthRange(): [string, string] {
  const now = new Date()
  const y = now.getFullYear()
  const m = String(now.getMonth() + 1).padStart(2, '0')
  const firstDay = `${y}-${m}-01`
  const lastDay = new Date(y, now.getMonth() + 1, 0)
    .getDate()
  const lastDayStr = String(lastDay).padStart(2, '0')
  return [firstDay, `${y}-${m}-${lastDayStr}`]
}

const initData = {
  pharmacyOptions: [
    { label: '门诊西药房', value: 'west' },
    { label: '门诊中药房', value: 'chinese' },
    { label: '门诊成药房', value: 'patent' }
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
  west: '门诊西药房',
  chinese: '门诊中药房',
  patent: '门诊成药房'
}

const dispensers: Record<string, string> = {
  u001: '张药师',
  u002: '李药师',
  u003: '王药师'
}

const departments = ['心血管内科', '呼吸内科', '消化内科', '神经内科', '骨科', '皮肤科']
const doctors = ['张医生', '李医生', '王医生', '赵医生', '刘医生']
const drugNames = [
  { name: '阿司匹林肠溶片', spec: '100mg*30片', unit: '盒', price: 12.5, dosage: '口服' },
  { name: '硝苯地平控释片', spec: '30mg*7片', unit: '盒', price: 28.0, dosage: '口服' },
  { name: '阿托伐他汀钙片', spec: '20mg*7片', unit: '盒', price: 35.8, dosage: '口服' },
  { name: '氯吡格雷片', spec: '75mg*7片', unit: '盒', price: 48.5, dosage: '口服' },
  { name: '二甲双胍缓释片', spec: '0.5g*60片', unit: '盒', price: 18.0, dosage: '口服' },
  { name: '头孢克肟胶囊', spec: '100mg*6粒', unit: '盒', price: 22.0, dosage: '口服' },
  { name: '布洛芬缓释胶囊', spec: '0.3g*20粒', unit: '盒', price: 15.5, dosage: '口服' },
  { name: '奥美拉唑肠溶胶囊', spec: '20mg*14粒', unit: '盒', price: 16.8, dosage: '口服' },
  { name: '复方甘草片', spec: '100片', unit: '瓶', price: 8.5, dosage: '口服' },
  { name: '蒙脱石散', spec: '3g*10袋', unit: '盒', price: 22.0, dosage: '冲服' }
]

const patientNames = ['陈小明', '李华', '王芳', '刘大伟', '赵晓敏', '孙强', '周丽', '吴刚', '郑华', '王建国']

let allRecords: OutpatientDispenseRecordItemDTO[] = []

function generateRecords(): OutpatientDispenseRecordItemDTO[] {
  const records: OutpatientDispenseRecordItemDTO[] = []
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
    const recordsPerDay = isToday ? 8 : 3 + Math.floor(Math.random() * 4)

    for (let i = 0; i < recordsPerDay; i++) {
      const recordType: 'DISPENSE' | 'REFUND' = Math.random() > 0.8 ? 'REFUND' : 'DISPENSE'
      const drug = drugNames[Math.floor(Math.random() * drugNames.length)]
      const patientIdx = Math.floor(Math.random() * patientNames.length)
      const deptIdx = Math.floor(Math.random() * departments.length)
      const docIdx = Math.floor(Math.random() * doctors.length)
      const pharmacyKeys = Object.keys(pharmacies)
      const pharmacyKey = pharmacyKeys[Math.floor(Math.random() * pharmacyKeys.length)]!
      const dispenserKeys = Object.keys(dispensers)
      const dispenserKey = dispenserKeys[Math.floor(Math.random() * dispenserKeys.length)]!
      const qty = Math.floor(Math.random() * 3) + 1
      const hour = String(8 + Math.floor(Math.random() * 10)).padStart(2, '0')
      const minute = String(Math.floor(Math.random() * 60)).padStart(2, '0')

      records.push({
        id: `R${String(idCounter).padStart(6, '0')}`,
        recordId: `REC${String(idCounter).padStart(6, '0')}`,
        prescriptionNo: `RX${dateStr.replace(/-/g, '')}${String(Math.floor(idCounter / 3) + 1).padStart(4, '0')}`,
        recordType,
        patientId: `P${String(patientIdx + 1).padStart(4, '0')}`,
        patientName: patientNames[patientIdx]!,
        outpatientNo: `MZ${dateStr.replace(/-/g, '')}${String(idCounter).padStart(4, '0')}`,
        departmentName: departments[deptIdx]!,
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
        dosage: `${drug!.dosage || '口服'} ${Math.floor(Math.random() * 3) + 1}${Math.random() > 0.5 ? '次/日' : 'ml/次'}`,
        summaryNo: recordType === 'DISPENSE' ? `SUM${dateStr.replace(/-/g, '')}${String(Math.floor(idCounter / 5) + 1).padStart(4, '0')}` : ''
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

export async function loadOutpatientDispenseRecordInit(): Promise<OutpatientDispenseRecordInitDTO> {
  await new Promise((r) => setTimeout(r, 200))
  return { ...initData }
}

export async function pageOutpatientDispenseRecords(
  params: PageOutpatientDispenseRecordsParams
): Promise<PageOutpatientDispenseRecordsResult> {
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
        r.outpatientNo.toLowerCase().includes(kw)
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

export async function exportOutpatientDispenseRecords(
  params: PageOutpatientDispenseRecordsParams
): Promise<ExportOutpatientDispenseRecordsResult> {
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
        r.outpatientNo.includes(params.patientKeyword!)
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
