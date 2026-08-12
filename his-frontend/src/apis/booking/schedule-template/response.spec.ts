import { describe, expect, it } from 'vitest'

import { savedEntityId } from './save-response'

describe('J8 schedule-template save response', () => {
  it('reads an id returned as a string', () => {
    expect(savedEntityId('2087102334622539777')).toBe('2087102334622539777')
  })

  it('reads an id returned in an object and supports update fallback', () => {
    expect(savedEntityId({ id: 'policy-1' })).toBe('policy-1')
    expect(savedEntityId({}, 'existing-1')).toBe('existing-1')
  })
})
