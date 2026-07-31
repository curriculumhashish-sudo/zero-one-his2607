export interface PrepayPageQuery {
  keyword: string
  admissionDate: string | null
  admissionStartDate: string | null
  admissionEndDate: string | null
  page: number
  pageSize: number
}
