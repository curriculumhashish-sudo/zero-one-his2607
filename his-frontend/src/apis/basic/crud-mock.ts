export interface BasicPageQuery {
  pageIndex: number
  pageSize: number
  keyword?: string
}

export interface BasicPageResult<T> {
  pageIndex: number
  pageSize: number
  total: number
  pages: number
  rows: T[]
}

type IdentifiableRecord = { id: string }

function clone<T>(value: T): T {
  return structuredClone(value)
}

export function createCrudStore<T extends IdentifiableRecord>(
  prefix: string,
  initialRows: T[],
  searchFields: Array<keyof T>
) {
  const rows = initialRows.map(clone)
  let sequence = rows.length

  const requireRow = (id: string) => {
    const row = rows.find((item) => item.id === id)
    if (!row) throw new Error('记录不存在或已被删除')
    return row
  }

  return {
    async page(query: BasicPageQuery): Promise<BasicPageResult<T>> {
      const keyword = query.keyword?.trim().toLowerCase() ?? ''
      const filtered = keyword
        ? rows.filter((row) =>
            searchFields.some((field) =>
              String(row[field] ?? '')
                .toLowerCase()
                .includes(keyword)
            )
          )
        : rows
      const pageIndex = Math.max(1, query.pageIndex)
      const pageSize = Math.max(1, query.pageSize)
      const start = (pageIndex - 1) * pageSize
      return {
        pageIndex,
        pageSize,
        total: filtered.length,
        pages: Math.ceil(filtered.length / pageSize),
        rows: filtered.slice(start, start + pageSize).map(clone)
      }
    },

    async list(): Promise<T[]> {
      return rows.map(clone)
    },

    async detail(id: string): Promise<T> {
      return clone(requireRow(id))
    },

    async create(payload: Omit<T, 'id'>): Promise<T> {
      sequence += 1
      const row = { ...clone(payload), id: `${prefix}-${String(sequence).padStart(3, '0')}` } as T
      rows.unshift(row)
      return clone(row)
    },

    async update(id: string, payload: Partial<Omit<T, 'id'>>): Promise<T> {
      const row = requireRow(id)
      Object.assign(row, clone(payload), { id })
      return clone(row)
    },

    async remove(id: string): Promise<void> {
      const index = rows.findIndex((item) => item.id === id)
      if (index < 0) throw new Error('记录不存在或已被删除')
      rows.splice(index, 1)
    }
  }
}
