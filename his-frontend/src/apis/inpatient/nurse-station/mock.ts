import type { PageDTO } from '../../type'
import type {
  NurseStationPatientFilterQuery,
  NurseStationPatientQueueItemVO,
  NurseStationPatientQueueQuery,
  NurseStationPatientStatisticsVO,
  NurseStationWardTreeVO
} from './type'

const wardTree: NurseStationWardTreeVO = {
  wardNodes: [
    {
      wardId: 'ward-internal',
      wardName: '内科疗区',
      displayOrder: 1,
      houseNodes: [
        { houseId: 'house-301', houseName: '301病房', displayOrder: 1 },
        { houseId: 'house-302', houseName: '302病房', displayOrder: 2 }
      ]
    },
    {
      wardId: 'ward-surgery',
      wardName: '外科疗区',
      displayOrder: 2,
      houseNodes: [
        { houseId: 'house-401', houseName: '401病房', displayOrder: 1 },
        { houseId: 'house-402', houseName: '402病房', displayOrder: 2 }
      ]
    },
    {
      wardId: 'ward-pediatrics',
      wardName: '儿科疗区',
      displayOrder: 3,
      houseNodes: [
        { houseId: 'house-501', houseName: '501病房', displayOrder: 1 },
        { houseId: 'house-502', houseName: '502病房', displayOrder: 2 }
      ]
    }
  ]
}

const patientRows: NurseStationPatientQueueItemVO[] = [
  {
    accountId: 'account-001',
    accountBalance: 1300,
    accountBalanceDisplayMode: 'BALANCE',
    accountBalanceSubText: '可用余额',
    accountBalanceSummaryText: '余额 1300.00 元',
    availableBalance: 1300,
    admissionTime: '2026-07-21 08:30:00',
    ageText: '26岁',
    bedId: 'bed-301-01',
    bedName: '01床',
    diagnosisSummary: '社区获得性肺炎',
    encounterBusNo: 'ZY202607210001',
    encounterId: 'encounter-001',
    genderEnum: 'FEMALE',
    genderEnumText: '女',
    houseId: 'house-301',
    houseName: '301病房',
    lowBalanceWarning: false,
    organizationId: 'dept-internal',
    organizationName: '住院内科',
    patientId: 'patient-001',
    patientName: '孙晓梅',
    pendingAuditCount: 2,
    pendingExecuteCount: 4,
    pinyinCode: 'SXM',
    primaryNurseName: '周护士',
    priorityEnum: 'NORMAL',
    recallEnabled: false,
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    wardId: 'ward-internal',
    wardName: '内科疗区',
    wubiCode: 'SXM'
  },
  {
    accountId: 'account-002',
    availableBalance: 80,
    ageText: '68岁',
    bedId: 'bed-301-02',
    bedName: '02床',
    diagnosisSummary: '慢性阻塞性肺疾病急性加重',
    encounterBusNo: 'ZY202607210002',
    encounterId: 'encounter-002',
    genderEnumText: '男',
    houseId: 'house-301',
    houseName: '301病房',
    lowBalanceWarning: true,
    organizationName: '住院内科',
    patientId: 'patient-002',
    patientName: '李建国',
    pendingAuditCount: 0,
    pendingExecuteCount: 3,
    pinyinCode: 'LJG',
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    wardId: 'ward-internal',
    wardName: '内科疗区'
  },
  {
    accountId: 'account-003',
    availableBalance: 2300,
    ageText: '45岁',
    bedId: 'bed-302-03',
    bedName: '03床',
    diagnosisSummary: '2型糖尿病',
    encounterBusNo: 'ZY202607220003',
    encounterId: 'encounter-003',
    genderEnumText: '女',
    houseId: 'house-302',
    houseName: '302病房',
    organizationName: '住院内科',
    patientId: 'patient-003',
    patientName: '王丽',
    pendingAuditCount: 1,
    pendingExecuteCount: 0,
    pinyinCode: 'WL',
    statusEnum: 'ON_HOLD',
    statusEnumText: '待出院',
    wardId: 'ward-internal',
    wardName: '内科疗区'
  },
  {
    accountId: 'account-004',
    availableBalance: 600,
    ageText: '37岁',
    bedId: 'bed-401-01',
    bedName: '01床',
    diagnosisSummary: '急性阑尾炎术后',
    encounterBusNo: 'ZY202607220004',
    encounterId: 'encounter-004',
    genderEnumText: '男',
    houseId: 'house-401',
    houseName: '401病房',
    organizationName: '住院外科',
    patientId: 'patient-004',
    patientName: '赵磊',
    pendingAuditCount: 4,
    pendingExecuteCount: 2,
    pinyinCode: 'ZL',
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    wardId: 'ward-surgery',
    wardName: '外科疗区'
  },
  {
    accountId: 'account-005',
    availableBalance: 1800,
    ageText: '54岁',
    bedId: 'bed-401-05',
    bedName: '05床',
    diagnosisSummary: '胆囊结石伴胆囊炎',
    encounterBusNo: 'ZY202607230005',
    encounterId: 'encounter-005',
    genderEnumText: '女',
    houseId: 'house-401',
    houseName: '401病房',
    organizationName: '住院外科',
    patientId: 'patient-005',
    patientName: '陈敏',
    pendingAuditCount: 0,
    pendingExecuteCount: 0,
    pinyinCode: 'CM',
    statusEnum: 'PENDING',
    statusEnumText: '待结算',
    wardId: 'ward-surgery',
    wardName: '外科疗区'
  },
  {
    accountId: 'account-006',
    availableBalance: 980,
    ageText: '29岁',
    bedId: 'bed-402-02',
    bedName: '02床',
    diagnosisSummary: '胫骨骨折',
    encounterBusNo: 'ZY202607230006',
    encounterId: 'encounter-006',
    genderEnumText: '男',
    houseId: 'house-402',
    houseName: '402病房',
    organizationName: '住院外科',
    patientId: 'patient-006',
    patientName: '周航',
    pendingAuditCount: 3,
    pendingExecuteCount: 5,
    pinyinCode: 'ZH',
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    wardId: 'ward-surgery',
    wardName: '外科疗区'
  },
  {
    accountId: 'account-007',
    availableBalance: 1500,
    ageText: '6岁',
    bedId: 'bed-501-01',
    bedName: '01床',
    diagnosisSummary: '支气管肺炎',
    encounterBusNo: 'ZY202607240007',
    encounterId: 'encounter-007',
    genderEnumText: '女',
    houseId: 'house-501',
    houseName: '501病房',
    organizationName: '住院儿科',
    patientId: 'patient-007',
    patientName: '何雨桐',
    pendingAuditCount: 1,
    pendingExecuteCount: 6,
    pinyinCode: 'HYT',
    statusEnum: 'IN_PROGRESS',
    statusEnumText: '在院',
    wardId: 'ward-pediatrics',
    wardName: '儿科疗区'
  },
  {
    accountId: 'account-008',
    availableBalance: 3200,
    ageText: '10岁',
    bedName: '',
    diagnosisSummary: '急性胃肠炎',
    encounterBusNo: 'ZY202607240008',
    encounterId: 'encounter-008',
    genderEnumText: '男',
    houseId: 'house-502',
    houseName: '502病房',
    organizationName: '住院儿科',
    patientId: 'patient-008',
    patientName: '吴子轩',
    pendingAuditCount: 0,
    pendingExecuteCount: 0,
    pinyinCode: 'WZX',
    statusEnum: 'REGISTERED',
    statusEnumText: '待入科',
    wardId: 'ward-pediatrics',
    wardName: '儿科疗区'
  },
  {
    accountId: 'account-009',
    availableBalance: 460,
    ageText: '73岁',
    bedName: '08床',
    diagnosisSummary: '高血压病',
    encounterBusNo: 'ZY202607150009',
    encounterId: 'encounter-009',
    endTime: '2026-07-29 10:00:00',
    genderEnumText: '男',
    houseId: 'house-302',
    houseName: '302病房',
    organizationName: '住院内科',
    patientId: 'patient-009',
    patientName: '郑伯安',
    pendingAuditCount: 0,
    pendingExecuteCount: 0,
    pinyinCode: 'ZBA',
    recallEnabled: true,
    statusEnum: 'COMPLETED',
    statusEnumText: '已结算',
    wardId: 'ward-internal',
    wardName: '内科疗区'
  },
  {
    accountId: 'account-010',
    availableBalance: 0,
    ageText: '61岁',
    bedName: '06床',
    diagnosisSummary: '腹股沟疝术后',
    encounterBusNo: 'ZY202607160010',
    encounterId: 'encounter-010',
    endTime: '2026-07-30 15:00:00',
    genderEnumText: '女',
    houseId: 'house-402',
    houseName: '402病房',
    organizationName: '住院外科',
    patientId: 'patient-010',
    patientName: '蒋春华',
    pendingAuditCount: 0,
    pendingExecuteCount: 0,
    pinyinCode: 'JCH',
    recallEnabled: true,
    statusEnum: 'DISCHARGED',
    statusEnumText: '已离院',
    wardId: 'ward-surgery',
    wardName: '外科疗区'
  }
]

/** 供同一护士站下的其他 Mock 领域共享患者单一事实源。 */
export function mockListNurseStationPatients() {
  return patientRows
}

/** 按就诊 ID 获取 Mock 患者。 */
export function mockFindNurseStationPatient(encounterId: string) {
  return patientRows.find((patient) => patient.encounterId === encounterId)
}

/** 更新 Mock 患者，模拟后端写操作后的读模型变化。 */
export function mockUpdateNurseStationPatient(
  encounterId: string,
  patch: Partial<NurseStationPatientQueueItemVO>
) {
  const patient = mockFindNurseStationPatient(encounterId)
  if (!patient) throw new Error('患者不存在')
  Object.assign(patient, patch)
  return patient
}

function parseIds(value?: string) {
  return new Set(
    (value ?? '')
      .split(',')
      .map((item) => item.trim())
      .filter(Boolean)
  )
}

function clonePatient(row: NurseStationPatientQueueItemVO) {
  return { ...row, diagnosisList: row.diagnosisList ? [...row.diagnosisList] : undefined }
}

function filterPatients(query: NurseStationPatientFilterQuery) {
  const wardIds = parseIds(query.wardIds || query.wardId)
  const houseIds = parseIds(query.houseIds || query.houseId)
  const keyword = query.keyword?.trim().toLowerCase() ?? ''
  const settledMode = query.nurseStationStatusFilter === 'SETTLED_DISCHARGED'

  return patientRows.filter((row) => {
    const isSettled = row.statusEnum === 'COMPLETED' || row.statusEnum === 'DISCHARGED'
    if (settledMode !== isSettled) return false
    if (houseIds.size > 0 && (!row.houseId || !houseIds.has(row.houseId))) return false
    if (houseIds.size === 0 && wardIds.size > 0 && (!row.wardId || !wardIds.has(row.wardId))) {
      return false
    }
    if (
      keyword &&
      ![
        row.patientName,
        row.wardName,
        row.houseName,
        row.bedName,
        row.encounterBusNo,
        row.pinyinCode,
        row.wubiCode
      ].some((value) => value?.toLowerCase().includes(keyword))
    ) {
      return false
    }
    if (query.nurseStationTaskFilter === 'pending-audit') {
      return (row.pendingAuditCount ?? 0) > 0
    }
    if (query.nurseStationTaskFilter === 'pending-execute') {
      return (row.pendingExecuteCount ?? 0) > 0
    }
    if (query.nurseStationTaskFilter === 'processed') {
      return (row.pendingAuditCount ?? 0) === 0 && (row.pendingExecuteCount ?? 0) === 0
    }
    return true
  })
}

function comparePatients(a: NurseStationPatientQueueItemVO, b: NurseStationPatientQueueItemVO) {
  const left = [a.wardId, a.houseId, a.bedName, a.patientName, a.encounterId]
  const right = [b.wardId, b.houseId, b.bedName, b.patientName, b.encounterId]
  for (let index = 0; index < left.length; index += 1) {
    const compared = String(left[index] ?? '').localeCompare(String(right[index] ?? ''), 'zh-CN')
    if (compared !== 0) return compared
  }
  return 0
}

/** Mock：加载当前护士站管理范围内的病区病房树。 */
export async function mockLoadNurseStationWardTree(): Promise<NurseStationWardTreeVO> {
  await Promise.resolve()
  return {
    wardNodes: wardTree.wardNodes.map((ward) => ({
      ...ward,
      houseNodes: ward.houseNodes.map((house) => ({ ...house }))
    }))
  }
}

/** Mock：获取与患者队列相同范围下的顶部统计。 */
export async function mockGetNurseStationPatientStatistics(
  query: NurseStationPatientFilterQuery
): Promise<NurseStationPatientStatisticsVO> {
  await Promise.resolve()
  const rows = filterPatients(query)
  return {
    totalCount: rows.length,
    pendingAuditCount: rows.reduce((sum, row) => sum + (row.pendingAuditCount ?? 0), 0),
    pendingExecuteCount: rows.reduce((sum, row) => sum + (row.pendingExecuteCount ?? 0), 0)
  }
}

/** Mock：分页查询患者队列。 */
export async function mockPageNurseStationPatientQueue(
  query: NurseStationPatientQueueQuery
): Promise<PageDTO<NurseStationPatientQueueItemVO>> {
  await Promise.resolve()
  const rows = filterPatients(query).sort(comparePatients)
  const pageIndex = Math.max(1, query.pageIndex)
  const pageSize = Math.max(1, query.pageSize)
  const offset = (pageIndex - 1) * pageSize
  return {
    pageIndex,
    pageSize,
    total: rows.length,
    pages: Math.ceil(rows.length / pageSize),
    rows: rows.slice(offset, offset + pageSize).map(clonePatient)
  }
}
