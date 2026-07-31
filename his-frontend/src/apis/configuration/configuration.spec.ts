import { describe, expect, it } from 'vitest'
import {
  createDocumentType,
  createEmrTemplate,
  createMetadataField,
  getDocumentTypeDetail,
  getEmrTemplateDetail,
  getMetadataFieldDetail,
  loadDocumentTypeNameOptions,
  loadMetadataFieldNameOptions,
  pageDocumentTypes,
  pageEmrTemplates,
  pageMetadataFields,
  removeDocumentType,
  removeEmrTemplate,
  removeMetadataField,
  updateDocumentType,
  updateEmrTemplate,
  updateMetadataField
} from './emr-template'
import {
  createExecuteLocation,
  loadExecuteDepartmentOptions,
  pageExecuteLocations,
  removeExecuteLocation,
  updateExecuteLocation
} from './execute-location'
import { pageMedicationLocationRules, saveMedicationLocationRules } from './medication-location'
import {
  createOrderTemplate,
  getOrderTemplateDetail,
  loadOrderTemplateNameOptions,
  pageOrderTemplates,
  removeOrderTemplate,
  setOrderTemplateEnabled,
  updateOrderTemplate
} from './order-template'
import {
  createServiceDeviceBinding,
  initServiceDeviceBinding,
  pageServiceDeviceBindings,
  removeServiceDeviceBinding,
  updateServiceDeviceBinding
} from './service-device-binding'

describe('配置管理接口', () => {
  it('病历模板配置的三类数据均支持 SVG 要求的接口能力', async () => {
    expect((await loadDocumentTypeNameOptions()).length).toBeGreaterThan(0)
    const documentType = await createDocumentType({
      code: 'TEST-DOC',
      name: '测试文书',
      description: ''
    })
    await updateDocumentType(documentType.id, { ...documentType, name: '测试文书（修改）' })
    expect((await getDocumentTypeDetail(documentType.id)).name).toContain('修改')
    expect(
      (await pageDocumentTypes({ pageIndex: 1, pageSize: 20, keyword: 'TEST-DOC' })).total
    ).toBe(1)

    const template = await createEmrTemplate({
      code: 'TEST-EMR',
      name: '测试病历模板',
      documentTypeCode: 'OUTPATIENT',
      applicableScope: '门诊',
      content: '<p>测试</p>',
      enabled: true
    })
    await updateEmrTemplate(template.id, { ...template, content: '<p>修改</p>' })
    expect((await getEmrTemplateDetail(template.id)).content).toContain('修改')
    expect(
      (await pageEmrTemplates({ pageIndex: 1, pageSize: 20, keyword: 'TEST-EMR' })).total
    ).toBe(1)

    expect((await loadMetadataFieldNameOptions()).length).toBeGreaterThan(0)
    const metadata = await createMetadataField({
      code: 'TEST_FIELD',
      name: '测试字段',
      dataType: 'TEXT',
      defaultValue: '',
      description: '',
      enabled: true
    })
    await updateMetadataField(metadata.id, { ...metadata, defaultValue: '默认值' })
    expect((await getMetadataFieldDetail(metadata.id)).defaultValue).toBe('默认值')
    expect(
      (await pageMetadataFields({ pageIndex: 1, pageSize: 20, keyword: 'TEST_FIELD' })).total
    ).toBe(1)

    await removeDocumentType(documentType.id)
    await removeEmrTemplate(template.id)
    await removeMetadataField(metadata.id)
  })

  it('其余四项配置支持对应的查询和维护能力', async () => {
    expect((await loadExecuteDepartmentOptions()).length).toBeGreaterThan(0)
    const executeLocation = await createExecuteLocation({
      departmentName: '检验科',
      itemCode: 'TEST-LAB',
      itemName: '测试检验',
      serviceCategory: '检验',
      startDate: '2026-01-01',
      endDate: '',
      description: ''
    })
    await updateExecuteLocation(executeLocation.id, {
      ...executeLocation,
      itemName: '修改测试检验'
    })
    expect(
      (await pageExecuteLocations({ pageIndex: 1, pageSize: 20, keyword: 'TEST-LAB' })).total
    ).toBe(1)
    await removeExecuteLocation(executeLocation.id)

    expect((await loadOrderTemplateNameOptions()).length).toBeGreaterThan(0)
    const orderTemplate = await createOrderTemplate({
      code: 'TEST-ORDER',
      name: '测试组套',
      templateType: 'GENERAL',
      departmentName: '测试科室',
      itemSummary: '测试项目',
      status: 'ENABLED',
      description: ''
    })
    await updateOrderTemplate(orderTemplate.id, { ...orderTemplate, name: '修改测试组套' })
    expect((await getOrderTemplateDetail(orderTemplate.id)).name).toContain('修改')
    expect((await setOrderTemplateEnabled(orderTemplate.id, false)).status).toBe('DISABLED')
    expect(
      (await pageOrderTemplates({ pageIndex: 1, pageSize: 20, keyword: 'TEST-ORDER' })).total
    ).toBe(1)
    await removeOrderTemplate(orderTemplate.id)

    const rules = await pageMedicationLocationRules({ pageIndex: 1, pageSize: 100, keyword: '' })
    const savedRules = await saveMedicationLocationRules([
      ...rules.rows,
      {
        id: '',
        departmentName: '测试科室',
        itemType: 'DRUG',
        itemName: '测试药品',
        pharmacyName: '测试药房',
        routeName: '口服',
        priority: 1,
        enabled: true
      }
    ])
    expect(savedRules.some((item) => item.itemName === '测试药品')).toBe(true)
    await saveMedicationLocationRules(rules.rows)

    expect((await initServiceDeviceBinding()).units).toContain('套')
    const binding = await createServiceDeviceBinding({
      serviceName: '测试治疗',
      serviceCategory: '治疗',
      deviceName: '测试耗材',
      specification: '',
      quantity: 1,
      unit: '套',
      enabled: true
    })
    await updateServiceDeviceBinding(binding.id, { ...binding, quantity: 2 })
    expect(
      (await pageServiceDeviceBindings({ pageIndex: 1, pageSize: 20, keyword: '测试治疗' })).total
    ).toBe(1)
    await removeServiceDeviceBinding(binding.id)
  })
})
