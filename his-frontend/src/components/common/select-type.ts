export type SelectValue = string | number

export interface RemoteSelectLoadContext {
  signal: AbortSignal
}

export type RemoteSelectLoadOptions<T> = (
  keyword: string,
  context: RemoteSelectLoadContext
) => Promise<T[]>

export interface SelectTableColumn<T extends Record<string, unknown>> {
  prop: keyof T & string | string
  label: string
  width?: number | string
  minWidth?: number | string
  align?: 'left' | 'center' | 'right'
  fixed?: boolean | 'left' | 'right'
  showOverflowTooltip?: boolean
  sortable?: boolean | 'custom'
  formatter?: (row: T, column: SelectTableColumn<T>) => string | number
}

export interface SelectTableQuery {
  keyword: string
  pageIndex: number
  pageSize: number
}

export interface SelectTableLoadContext {
  signal: AbortSignal
}

export interface SelectTablePage<T> {
  rows: T[]
  total: number
}

export type SelectTableLoadPage<T> = (
  query: SelectTableQuery,
  context: SelectTableLoadContext
) => Promise<SelectTablePage<T>>

