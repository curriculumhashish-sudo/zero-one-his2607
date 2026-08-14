type LooseRecord = Record<string, any>

const J6_SUB_CATEGORY_LABELS: Record<string, string> = {
  DRUG: '药品',
  ACTIVITY: '诊疗',
  DEVICE: '耗材'
}

export function getJ6EncounterRows(response: LooseRecord): LooseRecord[] {
  if (Array.isArray(response.content)) return response.content
  if (Array.isArray(response.rows)) return response.rows
  return []
}

export function getJ6SubCategoryDisplay(line: LooseRecord): string | undefined {
  const display = String(line.subCategoryDisplay ?? '').trim()
  if (display) return display
  const code = String(line.subCategoryCode ?? '').trim()
  return J6_SUB_CATEGORY_LABELS[code] ?? (code || undefined)
}

export function filterJ6EncounterRows(
  rows: LooseRecord[],
  feeCategoryCodes?: string[],
  subCategoryCodes?: string[]
): LooseRecord[] {
  const feeCategorySet = new Set((feeCategoryCodes ?? []).map(String))
  const subCategorySet = new Set((subCategoryCodes ?? []).map(String))

  return rows
    .map((row) => ({
      ...row,
      lines: (Array.isArray(row.lines) ? row.lines : []).filter((line: LooseRecord) => {
        const feeCategoryMatched =
          !feeCategorySet.size || feeCategorySet.has(String(line.feeCategoryCode ?? ''))
        const subCategoryMatched =
          !subCategorySet.size || subCategorySet.has(String(line.subCategoryCode ?? ''))
        return feeCategoryMatched && subCategoryMatched
      })
    }))
    .filter((row) => row.lines.length > 0)
}

export function filterJ6EncounterRowsByDepartment(
  rows: LooseRecord[],
  departmentName?: string
): LooseRecord[] {
  const selectedName = String(departmentName ?? '').trim()
  if (!selectedName) return rows
  return rows.filter((row) => String(row.registrationDepartmentName ?? '').trim() === selectedName)
}

export function getJ6DepartmentOptions(
  rows: LooseRecord[]
): Array<{ value: string; label: string }> {
  const names = new Set<string>()
  rows.forEach((row) => {
    const name = String(row.registrationDepartmentName ?? '').trim()
    if (name) names.add(name)
  })
  return [...names].map((name) => ({ value: name, label: name }))
}

export function getJ6SubCategoryOptions(
  rows: LooseRecord[],
  feeCategoryCodes?: string[]
): Array<{ value: string; label: string }> {
  const categoryRows = filterJ6EncounterRows(rows, feeCategoryCodes)
  const options = new Map<string, string>()
  categoryRows.forEach((row) => {
    row.lines.forEach((line: LooseRecord) => {
      const value = String(line.subCategoryCode ?? '').trim()
      if (!value) return
      options.set(value, getJ6SubCategoryDisplay(line) ?? value)
    })
  })
  return [...options].map(([value, label]) => ({ value, label }))
}

export function getJ6CashierOptions(rows: LooseRecord[]): Array<{ value: string; label: string }> {
  const options = new Map<string, string>()
  rows.forEach((row) => {
    ;(Array.isArray(row.lines) ? row.lines : []).forEach((line: LooseRecord) => {
      const value = String(line.cashierId ?? '').trim()
      if (!value) return
      const label = String(line.cashierName ?? '').trim() || value
      options.set(value, label)
    })
  })
  return [...options].map(([value, label]) => ({ value, label }))
}

export function getJ6MatchingSubCategoryCodes(
  rows: LooseRecord[],
  feeCategoryCodes?: string[],
  subCategoryCodes?: string[]
): string[] {
  const codes = new Set<string>()
  filterJ6EncounterRows(rows, feeCategoryCodes, subCategoryCodes).forEach((row) => {
    row.lines.forEach((line: LooseRecord) => {
      const code = String(line.subCategoryCode ?? '').trim()
      if (code) codes.add(code)
    })
  })
  return [...codes]
}
