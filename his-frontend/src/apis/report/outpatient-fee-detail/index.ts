import { mockExportOutpatientFeeDetails, mockPageOutpatientFeeDetails } from './mock'
import type {
  ExportOutpatientFeeDetailParams,
  OutpatientFeeDetailAmountSummaryDto,
  PageOutpatientFeeDetailParams,
  PageOutpatientFeeDetailBackendResponse,
  PageOutpatientFeeDetailResponse,
  RequestContext
} from './type'

function normalizeSummary(response: PageOutpatientFeeDetailBackendResponse): OutpatientFeeDetailAmountSummaryDto {
  return {
    baseAmountTotal: response.baseAmountSum,
    discountAmountTotal: response.discountAmountSum,
    receivableAmountTotal: response.receivableAmountSum,
    discountedAmountTotal: response.discountedAmountSum,
    lineAmountTotal: response.lineAmountSum,
    actualReceivedAmountTotal: response.actualReceivedAmountSum,
    insuranceAmountTotal: response.insuranceAmountSum,
    selfPayAmountTotal: response.selfPayAmountSum
  }
}

export const pageOutpatientFeeDetails = (
  query: PageOutpatientFeeDetailParams,
  context?: RequestContext
): Promise<PageOutpatientFeeDetailResponse> =>
  mockPageOutpatientFeeDetails(query, context).then((response) => ({
    content: response.content,
    totalCount: response.totalCount,
    pageNo: response.pageNo,
    pageSize: response.pageSize,
    summary: normalizeSummary(response)
  }))

export const exportOutpatientFeeDetails = (
  query: ExportOutpatientFeeDetailParams,
  context?: RequestContext
) => mockExportOutpatientFeeDetails(query, context)

export type * from './type'
