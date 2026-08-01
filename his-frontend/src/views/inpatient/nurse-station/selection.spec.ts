import { describe, expect, it } from 'vitest'

import { normalizeSelectedPatientIds } from './selection'

describe('住院护士站患者选择收敛', () => {
  it('清理不在当前队列中的患者并去重', () => {
    expect(
      normalizeSelectedPatientIds(
        ['encounter-1', 'encounter-2', 'encounter-1', 'missing'],
        ['encounter-1', 'encounter-2'],
        'multiple'
      )
    ).toEqual(['encounter-1', 'encounter-2'])
  })

  it('从多选页签切到单选页签时仅保留第一名患者', () => {
    expect(
      normalizeSelectedPatientIds(
        ['encounter-2', 'encounter-1'],
        ['encounter-1', 'encounter-2'],
        'single'
      )
    ).toEqual(['encounter-2'])
  })
})
