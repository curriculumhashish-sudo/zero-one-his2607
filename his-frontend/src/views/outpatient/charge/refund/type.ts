export interface RefundPageQuery {
  keyword: string
  status: 'all' | 'pending' | 'refunded'
  page: number
  pageSize: number
}
