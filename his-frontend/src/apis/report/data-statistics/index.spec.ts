import { describe, expect, it } from 'vitest'

import { toInventory } from './inventory-response'
import { getStatisticsProvider, setStatisticsProvider } from './index'

describe('J8 inventory statistics response', () => {
  it('normalizes Chinese inventory directions returned by J8', () => {
    expect(
      toInventory({
        categories: ['2026-08'],
        series: [
          { name: '药品', direction: '入库', values: [2520] },
          { name: '药品', direction: '出库', values: [160] }
        ],
        unit: '元'
      }).series
    ).toEqual([
      { name: '药品', direction: 'INBOUND', values: [2520] },
      { name: '药品', direction: 'OUTBOUND', values: [160] }
    ])
  })
})

describe('C1 inventory statistics response', () => {
  it('uses total inbound and outbound amounts instead of unit prices', () => {
    expect(
      toInventory({
        rows: [
          {
            name: '药品',
            onemoney: 10,
            allmoney: 100,
            outmoney: 12,
            alloutmoney: 60
          }
        ]
      }).series
    ).toEqual([
      { name: '入库', direction: 'INBOUND', values: [100] },
      { name: '出库', direction: 'OUTBOUND', values: [60] }
    ])
  })

  it('switches the shared statistics page between J8, J6 and C1 providers', () => {
    setStatisticsProvider('c1')
    expect(getStatisticsProvider()).toBe('c1')
    setStatisticsProvider('j6')
    expect(getStatisticsProvider()).toBe('j6')
    setStatisticsProvider('j8')
    expect(getStatisticsProvider()).toBe('j8')
  })
})
