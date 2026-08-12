import { beforeEach, describe, expect, it, vi } from 'vitest'

const get = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get })
}))

import { listDoctorStationDiagnosis } from './index'

describe('诊断字典真实查询接口', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('使用 GET 请求传递分页参数和去除空白后的关键词', async () => {
    const rows = [{ id: 'diag-I10', code: 'I10', name: '原发性高血压', typeName: '西医诊断' }]
    get.mockResolvedValue({ code: 10000, message: 'success', data: { rows, total: 1 } })

    await expect(listDoctorStationDiagnosis({ keyword: ' I10 ', pageIndex: 2, pageSize: 20 })).resolves.toEqual({
      rows,
      total: 1
    })
    expect(get).toHaveBeenCalledWith('/doctor-station/page-diagnosis-dictionary', {
      keyword: 'I10',
      pageIndex: 2,
      pageSize: 20
    })
  })

  it('空关键词不发送，并拒绝异常业务响应和错误数据结构', async () => {
    get.mockResolvedValueOnce({ code: 9999, message: '查询失败' })
    await expect(listDoctorStationDiagnosis({ keyword: '  ', pageIndex: 1, pageSize: 10 })).rejects.toThrow(
      '查询失败'
    )
    expect(get).toHaveBeenLastCalledWith('/doctor-station/page-diagnosis-dictionary', {
      pageIndex: 1,
      pageSize: 10
    })

    get.mockResolvedValueOnce({ code: 10000, message: 'success', data: null })
    await expect(listDoctorStationDiagnosis({ pageIndex: 1, pageSize: 10 })).rejects.toThrow(
      '诊断字典返回数据格式错误'
    )
  })

  it('网络异常直接向上抛出，不回退 Mock', async () => {
    get.mockRejectedValue(new Error('Network Error'))

    await expect(listDoctorStationDiagnosis({ pageIndex: 1, pageSize: 10 })).rejects.toThrow('Network Error')
  })
})
