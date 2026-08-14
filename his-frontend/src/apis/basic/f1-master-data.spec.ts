import { describe, expect, it } from 'vitest'
import {
  createPatient,
  getPatientDetail,
  loadPatientNameOptions,
  pagePatients,
  removePatient,
  updatePatient
} from './patient'
import { pageWardBeds, setWardBedEnabled } from './ward-bed'
import { pageWarehousePharmacies } from './warehouse-pharmacy'
import { getSupplierDetail, pageSuppliers, setSuppliersEnabled } from './supplier'
import { pageChargeTypes, setChargeTypeEnabled } from './charge-type'
import { pageRegistrationTypes } from './registration-type'

describe('F1 基础数据模拟接口', () => {
  it('患者接口支持名称、分页、详情、新增、修改和删除六种能力', async () => {
    expect((await loadPatientNameOptions()).length).toBeGreaterThan(0)
    const before = await pagePatients({ pageIndex: 1, pageSize: 20, keyword: '' })
    const created = await createPatient({
      name: '测试患者',
      genderEnum: 'UNKNOWN',
      age: 20,
      birthDate: '2006-01-01',
      phone: '',
      idCard: 'TEST-ID',
      nationalityCode: 'CN',
      workCompany: '',
      maritalStatusEnum: 'UNKNOWN',
      bloodAbo: '未知',
      bloodRh: 'UNKNOWN',
      addressProvince: '',
      addressCity: '',
      addressDistrict: '',
      address: '',
      linkName: '',
      linkTelcom: '',
      activeFlag: true
    })
    expect((await getPatientDetail(created.id)).name).toBe('测试患者')
    await updatePatient(created.id, { ...created, name: '修改患者' })
    expect((await getPatientDetail(created.id)).name).toBe('修改患者')
    await removePatient(created.id)
    expect((await pagePatients({ pageIndex: 1, pageSize: 20, keyword: '' })).total).toBe(
      before.total
    )
  })

  it('其余五个页面均有可查询数据并支持需要的启停操作', async () => {
    const ward = await pageWardBeds({ pageIndex: 1, pageSize: 10, keyword: '' })
    await setWardBedEnabled(ward.rows[0]!.id, false)
    expect((await pageWardBeds({ pageIndex: 1, pageSize: 10, keyword: ward.rows[0]!.name })).rows[0]?.statusEnum).toBe('INACTIVE')

    expect((await pageWarehousePharmacies({ pageIndex: 1, pageSize: 10, keyword: '' })).total).toBeGreaterThan(0)
    const suppliers = await pageSuppliers({ pageIndex: 1, pageSize: 10, keyword: '' })
    await setSuppliersEnabled([suppliers.rows[0]!.id], false)
    expect((await getSupplierDetail(suppliers.rows[0]!.id)).statusEnum).toBe('INACTIVE')
    const charges = await pageChargeTypes({ pageIndex: 1, pageSize: 10, keyword: '' })
    await setChargeTypeEnabled(charges.rows[0]!.id, false)
    expect((await pageChargeTypes({ pageIndex: 1, pageSize: 10, keyword: '' })).rows[0]?.activeFlag).toBe(0)
    expect((await pageRegistrationTypes({ pageIndex: 1, pageSize: 10, keyword: '' })).total).toBeGreaterThan(0)
  })
})
