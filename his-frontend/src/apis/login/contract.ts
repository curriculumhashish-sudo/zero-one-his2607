import * as legacy from './index'

export type SelectDepartmentContractRequest = {
  organizationUnitId?: string
}

export type SelectDepartmentContractResponse = void

export async function selectDepartmentContract(
  request: SelectDepartmentContractRequest
): Promise<SelectDepartmentContractResponse> {
  await legacy.selectDepartment(String(request.organizationUnitId))
  return undefined as unknown as SelectDepartmentContractResponse
}
