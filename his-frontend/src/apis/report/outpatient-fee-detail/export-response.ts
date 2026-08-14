import type { ExportOutpatientFeeDetailResponse } from './type'

export function normalizeExportResponse(
  response: ExportOutpatientFeeDetailResponse
): ExportOutpatientFeeDetailResponse {
  const body = response.body?.trim()
  if (body && /^https?:\/\//i.test(body)) {
    return { ...response, body: undefined, downloadUrl: body }
  }
  return { ...response, body }
}
