type LoginContext = {
  organizationId?: unknown
  departmentId?: unknown
}

function normalizedId(value: unknown) {
  return typeof value === 'string' || typeof value === 'number' ? String(value).trim() : ''
}

export function resolveScheduleOrganizationId(
  rawLoginContext: string | null,
  defaultOrganizationId = ''
) {
  try {
    const context = JSON.parse(rawLoginContext ?? '{}') as LoginContext
    return (
      normalizedId(context.organizationId) ||
      normalizedId(context.departmentId) ||
      normalizedId(defaultOrganizationId)
    )
  } catch {
    return normalizedId(defaultOrganizationId)
  }
}
