import { describe, expect, it } from 'vitest'
import { resolveScheduleOrganizationId } from './context'

describe('resolveScheduleOrganizationId', () => {
  it('优先使用登录上下文中的 organizationId', () => {
    expect(
      resolveScheduleOrganizationId(
        JSON.stringify({ organizationId: 'organization-1', departmentId: 'department-1' }),
        'default-1'
      )
    ).toBe('organization-1')
  })

  it('兼容登录流程实际保存的 departmentId', () => {
    expect(
      resolveScheduleOrganizationId(JSON.stringify({ departmentId: 'department-1' }), 'default-1')
    ).toBe('department-1')
  })

  it('未登录联调时使用开发环境默认科室', () => {
    expect(resolveScheduleOrganizationId(null, 'default-1')).toBe('default-1')
    expect(resolveScheduleOrganizationId('{invalid-json', 'default-1')).toBe('default-1')
  })
})
