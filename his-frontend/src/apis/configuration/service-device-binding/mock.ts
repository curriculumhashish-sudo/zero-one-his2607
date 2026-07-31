import { createCrudStore } from '@/apis/basic/crud-mock'
import type { ServiceDeviceBindingRecord, ServiceDeviceBindingSavePayload } from './type'

const store = createCrudStore<ServiceDeviceBindingRecord>(
  'service-device-binding',
  [
    {
      id: 'service-device-binding-001',
      serviceName: '静脉输液',
      serviceCategory: '治疗',
      deviceName: '一次性输液器',
      specification: '带针型',
      quantity: 1,
      unit: '套',
      enabled: true
    },
    {
      id: 'service-device-binding-002',
      serviceName: '换药',
      serviceCategory: '治疗',
      deviceName: '无菌敷料包',
      specification: '常规型',
      quantity: 1,
      unit: '包',
      enabled: true
    }
  ],
  ['serviceName', 'serviceCategory', 'deviceName', 'specification']
)

function normalize(payload: ServiceDeviceBindingSavePayload): ServiceDeviceBindingSavePayload {
  if (!payload.serviceName.trim() || !payload.deviceName.trim())
    throw new Error('诊疗项目和耗材名称不能为空')
  return {
    ...payload,
    serviceName: payload.serviceName.trim(),
    serviceCategory: payload.serviceCategory.trim(),
    deviceName: payload.deviceName.trim(),
    specification: payload.specification.trim(),
    unit: payload.unit.trim(),
    quantity: Math.max(1, Number(payload.quantity) || 1)
  }
}

export async function mockCreateServiceDeviceBinding(payload: ServiceDeviceBindingSavePayload) {
  return store.create(normalize(payload))
}

export async function mockUpdateServiceDeviceBinding(
  id: string,
  payload: ServiceDeviceBindingSavePayload
) {
  return store.update(id, normalize(payload))
}

export async function mockInitServiceDeviceBinding() {
  return { serviceCategories: ['治疗', '检查', '检验', '手术'], units: ['个', '套', '包', '支'] }
}

export { store as serviceDeviceBindingStore }
