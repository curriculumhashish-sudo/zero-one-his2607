import { beforeEach, describe, expect, it, vi } from 'vitest'

const httpMock = vi.hoisted(() => ({
  get: vi.fn()
}))

vi.mock('@/plugins/http', () => ({
  useHttp: () => httpMock
}))

import { realPageInpatientMedicalOrderCatalog } from './orders-catalog-real'

describe('住院医嘱目录真实接口适配', () => {
  beforeEach(() => {
    vi.clearAllMocks()
  })

  it('复用门诊医生站目录接口并转换为住院目录模型', async () => {
    httpMock.get.mockResolvedValue({
      data: {
        pageIndex: 1,
        pageSize: 20,
        total: 1,
        pages: 1,
        rows: [
          {
            id: 'drug-1',
            itemCode: 'DRUG-001',
            itemName: '阿司匹林',
            orderType: 'medication',
            categoryName: '西药',
            specification: '100mg*30片',
            defaultDose: 100,
            defaultUnit: 'mg',
            defaultFrequency: '每日一次',
            defaultRoute: '口服',
            defaultDays: 7,
            defaultQuantity: 1,
            quantityUnit: '盒'
          }
        ]
      }
    })

    const result = await realPageInpatientMedicalOrderCatalog({
      pageIndex: 1,
      pageSize: 20,
      keyword: '阿司匹林'
    })

    expect(httpMock.get).toHaveBeenCalledWith(
      '/c2-opdoctor/doctor-station/page-advice-order-catalog',
      {
        pageIndex: 1,
        pageSize: 20,
        keyword: '阿司匹林'
      },
      { baseURL: '/api' }
    )
    expect(result.rows?.[0]).toMatchObject({
      id: 'drug-1',
      code: 'DRUG-001',
      name: '阿司匹林',
      category: 'DRUG',
      defaultTherapyType: 'TEMPORARY'
    })
  })
})
