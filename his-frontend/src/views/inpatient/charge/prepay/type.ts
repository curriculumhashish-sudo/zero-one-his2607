export interface PrepayPageQuery {
  keyword: string
  admissionDate: string | null
  admissionStartDate: string | null
  admissionEndDate: string | null
  pageIndex: number
  pageSize: number
}
