import { beforeEach, describe, expect, it, vi } from 'vitest'
import {
  listMedicationCatalog,
  removeMedicationCatalog,
  setMedicationCatalogEnabled
} from './index'
import { setApiProfile } from './profile'
import { setDiagnosisEnabled } from './diagnosis'
import { removeEquipment, setEquipmentEnabled } from './equipment'
import { setFrequencyEnabled } from './frequency'
import { removeMedication, setMedicationEnabled } from './medication'
import { removeSample, setSampleEnabled } from './sample'
import { removeTreatment, setTreatmentEnabled } from './treatment'

const http = vi.hoisted(() => ({
  get: vi.fn(),
  post: vi.fn(),
  put: vi.fn(),
  patch: vi.fn(),
  delete: vi.fn()
}))

vi.mock('./client', () => ({ useProfiledHttp: () => http }))

describe('C6/J4 路由分发', () => {
  beforeEach(() => {
    window.localStorage.clear()
    vi.clearAllMocks()
    for (const method of Object.values(http)) {
      method.mockResolvedValue({ code: 10000, message: 'success', data: {} })
    }
  })

  it('J4 使用各模块的 J4 状态接口', async () => {
    setApiProfile('j4')
    await setMedicationEnabled('m1', true)
    await setFrequencyEnabled('f1', false)
    await setEquipmentEnabled('e1', true)
    await setDiagnosisEnabled('d1', false)
    await setTreatmentEnabled('t1', true)

    expect(http.put).toHaveBeenCalledWith('/medicine/change-status/1', {
      ids: ['m1'],
      size: 1
    })
    expect(http.put).toHaveBeenCalledWith('/frequency/modify-frequency-status', {
      id: 'f1',
      activeFlag: 'MA=='
    })
    expect(http.put).toHaveBeenCalledWith('/equipment/modify-equipment-status', {
      ids: ['e1'],
      activeFlag: true
    })
    expect(http.put).toHaveBeenCalledWith('/diagnosis/modify-batch-status', {
      ids: ['d1'],
      status: 'INACTIVE'
    })
    expect(http.put).toHaveBeenCalledWith('/treatment/modify-treatment-status', {
      id: 't1',
      statusEnum: 'ACTIVE'
    })
  })

  it('C6 使用各模块的 C6 删除和状态接口', async () => {
    setApiProfile('c6')
    await setFrequencyEnabled('f1', true)
    await setEquipmentEnabled('e1', false)
    await removeEquipment('e1')
    await setSampleEnabled('s1', true)
    await removeSample('s1')
    await setTreatmentEnabled('t1', false)
    await removeTreatment('t1')

    expect(http.put).toHaveBeenCalledWith('/frequency/modify-status', undefined, {
      params: { id: 'f1', status: 1 }
    })
    expect(http.put).toHaveBeenCalledWith('/equipment/status', {
      ids: ['e1'],
      status: 0
    })
    expect(http.delete).toHaveBeenCalledWith('/equipment/delete', { ids: ['e1'] })
    expect(http.put).toHaveBeenCalledWith('/sample/switch-status', {
      ids: ['s1'],
      statusEnum: 'ACTIVE',
      status: 1
    })
    expect(http.delete).toHaveBeenCalledWith('/sample/remove-sample', { id: 's1' })
    expect(http.post).toHaveBeenCalledWith('/treatment/modify-status', {
      ids: ['t1'],
      status: 0
    })
    expect(http.delete).toHaveBeenCalledWith('/treatment/remove-treatment', ['t1'])
  })

  it('J4 删除不自动尝试 C6', async () => {
    setApiProfile('j4')
    await removeMedication('m1')
    expect(http.delete).toHaveBeenCalledTimes(1)
    expect(http.delete).toHaveBeenCalledWith('/medicine/delete-medicine', { ids: ['m1'] })
  })

  it('药品目录列表按当前模式选择 C6 或 J4', async () => {
    setApiProfile('c6')
    await listMedicationCatalog({ pageIndex: 1, pageSize: 10 })
    expect(http.get).toHaveBeenCalledWith('/Drug', {
      keywords: '',
      name: '',
      kind: '',
      pageIndex: 1,
      pageSize: 10
    })

    vi.clearAllMocks()
    http.get.mockResolvedValue({ code: 10000, message: 'success', data: { rows: [], total: 0 } })
    setApiProfile('j4')
    await listMedicationCatalog({ pageIndex: 1, pageSize: 10 })
    expect(http.get).toHaveBeenCalledWith('/medicine/query-medicine-list', {
      keyword: '',
      categoryCode: '',
      activeFlag: undefined,
      pageIndex: 1,
      pageSize: 10
    })
  })

  it('药品目录写操作只使用当前模式的接口', async () => {
    setApiProfile('c6')
    http.get.mockResolvedValueOnce({
      code: 10000,
      message: 'success',
      data: { id: 'catalog-1', version: 3 }
    })
    await setMedicationCatalogEnabled('catalog-1', false)
    expect(http.patch).toHaveBeenCalledWith('/api/v2/medicines/status', {
      items: [{ id: 'catalog-1', version: 3 }],
      status: 'INACTIVE'
    })

    vi.clearAllMocks()
    http.delete.mockResolvedValue({ code: 10000, message: 'success', data: {} })
    setApiProfile('j4')
    await removeMedicationCatalog('catalog-1')
    expect(http.delete).toHaveBeenCalledTimes(1)
    expect(http.delete).toHaveBeenCalledWith('/medicine/delete-medicine', {
      ids: ['catalog-1']
    })
  })
})
