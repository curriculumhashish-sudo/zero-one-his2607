import { describe, expect, it, vi } from 'vitest'

const post = vi.fn()
const put = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ post, put })
}))

import { realCreateAdviceOrder, realUpdateAdviceOrder } from './advice-real'

const item = {
  catalogId: 'catalog-1',
  code: 'ITEM-1',
  name: 'Test item',
  category: 'DRUG' as const,
  categoryName: 'Drug',
  quantity: 1,
  quantityUnit: 'box',
  orderNo: 1
}

describe('real outpatient advice version mapping', () => {
  it('sends encounterVersion without unsupported fields when creating', async () => {
    post.mockResolvedValueOnce({
      code: 10000,
      data: { id: 'order-1', encounterId: 'encounter-1', orderVersion: 1, encounterVersion: 4 }
    })

    await realCreateAdviceOrder({
      encounterId: 'encounter-1',
      encounterVersion: 3,
      item
    })

    expect(post).toHaveBeenCalledWith(
      '/c2-opdoctor/createOrder/add',
      expect.objectContaining({
        encounterId: 'encounter-1',
        encounterVersion: 3
      }),
      expect.anything()
    )
    expect(post.mock.calls[0]?.[1]).not.toHaveProperty('expenseType')
  })

  it('uses orderVersion instead of version when modifying', async () => {
    put.mockResolvedValueOnce({
      code: 10000,
      data: { id: 'order-1', encounterId: 'encounter-1', orderVersion: 2, encounterVersion: 4 }
    })

    await realUpdateAdviceOrder({
      encounterId: 'encounter-1',
      adviceId: 'order-1',
      orderVersion: 1,
      encounterVersion: 3,
      item
    })

    expect(put).toHaveBeenCalledWith(
      '/c2-opdoctor/createOrder/modify',
      expect.objectContaining({
        orderId: 'order-1',
        orderVersion: 1,
        encounterVersion: 3
      }),
      expect.anything()
    )
    expect(put.mock.calls[0]?.[1]).not.toHaveProperty('version')
  })
})
