export interface ChargePageQuery {
  keyword: string
  status: 'all' | 'pending' | 'settled'
  page: number
  pageSize: number
}
