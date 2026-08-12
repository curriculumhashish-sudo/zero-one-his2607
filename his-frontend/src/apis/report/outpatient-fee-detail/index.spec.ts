// @vitest-environment jsdom

import { describe, expect, it } from 'vitest'
import { read, utils } from 'xlsx'

import { normalizeExportResponse } from './export-response'
import { createJ6ExportFile } from './index'
import {
  filterJ6EncounterRows,
  filterJ6EncounterRowsByDepartment,
  getJ6CashierOptions,
  getJ6DepartmentOptions,
  getJ6EncounterRows,
  getJ6MatchingSubCategoryCodes,
  getJ6SubCategoryDisplay,
  getJ6SubCategoryOptions
} from './j6-response'

describe('J6 门诊费用明细响应适配', () => {
  it('从正式响应字段 content 读取就诊分组', () => {
    const content = [{ encounterId: 'encounter-1', lines: [{ chargeItemId: 'item-1' }] }]
    expect(getJ6EncounterRows({ content, totalCount: 1 })).toBe(content)
  })

  it('兼容旧响应字段 rows', () => {
    const rows = [{ encounterId: 'encounter-legacy' }]
    expect(getJ6EncounterRows({ rows })).toBe(rows)
  })

  it('用 J6 实际编码补齐细分类显示名称', () => {
    expect(getJ6SubCategoryDisplay({ subCategoryCode: 'DRUG', subCategoryDisplay: null })).toBe(
      '药品'
    )
    expect(getJ6SubCategoryDisplay({ subCategoryCode: 'ACTIVITY', subCategoryDisplay: null })).toBe(
      '诊疗'
    )
  })

  const rows = [
    {
      encounterId: 'encounter-1',
      lines: [
        {
          feeCategoryCode: 'MEDICATION',
          subCategoryCode: '1001',
          subCategoryDisplay: '西药',
          cashierId: 'cashier-1',
          cashierName: '门诊医生'
        },
        {
          feeCategoryCode: 'ACTIVITY',
          subCategoryCode: '1008',
          subCategoryDisplay: '治疗',
          cashierId: 'cashier-1',
          cashierName: '门诊医生'
        }
      ]
    },
    {
      encounterId: 'encounter-2',
      lines: [
        {
          feeCategoryCode: 'ACTIVITY',
          subCategoryCode: 'ACTIVITY',
          subCategoryDisplay: null,
          cashierId: 'cashier-2',
          cashierName: 'admin'
        }
      ]
    }
  ]

  it('按明细真实返回的大类筛选，并保留命中的就诊分组', () => {
    expect(filterJ6EncounterRows(rows, ['ACTIVITY'])).toEqual([
      {
        encounterId: 'encounter-1',
        lines: [
          {
            feeCategoryCode: 'ACTIVITY',
            subCategoryCode: '1008',
            subCategoryDisplay: '治疗',
            cashierId: 'cashier-1',
            cashierName: '门诊医生'
          }
        ]
      },
      {
        encounterId: 'encounter-2',
        lines: [
          {
            feeCategoryCode: 'ACTIVITY',
            subCategoryCode: 'ACTIVITY',
            subCategoryDisplay: null,
            cashierId: 'cashier-2',
            cashierName: 'admin'
          }
        ]
      }
    ])
  })

  it('仅选择细分类时也只保留命中明细，供列表和导出共用', () => {
    expect(filterJ6EncounterRows(rows, undefined, ['1008'])).toEqual([
      {
        encounterId: 'encounter-1',
        lines: [
          {
            feeCategoryCode: 'ACTIVITY',
            subCategoryCode: '1008',
            subCategoryDisplay: '治疗',
            cashierId: 'cashier-1',
            cashierName: '门诊医生'
          }
        ]
      }
    ])
  })

  it('仅选择细分类后生成的 XLSX 只包含命中的明细行', async () => {
    const filteredRows = filterJ6EncounterRows(rows, undefined, ['1008'])
    const result = await createJ6ExportFile(filteredRows)
    const workbook = read(result.body, { type: 'base64' })
    const sheet = workbook.Sheets[workbook.SheetNames[0]!]
    expect(result.exportedRowCount).toBe(1)
    expect(utils.sheet_to_json(sheet!).length).toBe(1)
  })

  it('细分类选项来自当前数据，不假定不同月份使用相同编码', () => {
    expect(getJ6SubCategoryOptions(rows, ['ACTIVITY'])).toEqual([
      { value: '1008', label: '治疗' },
      { value: 'ACTIVITY', label: '诊疗' }
    ])
    expect(getJ6MatchingSubCategoryCodes(rows, ['ACTIVITY'])).toEqual(['1008', 'ACTIVITY'])
  })

  it('收费员选项使用明细中真实返回的编号和姓名', () => {
    expect(getJ6CashierOptions(rows)).toEqual([
      { value: 'cashier-1', label: '门诊医生' },
      { value: 'cashier-2', label: 'admin' }
    ])
  })

  it('科室选项使用就诊分组中的真实名称，并按名称筛选', () => {
    const departmentRows = [
      { encounterId: 'encounter-1', registrationDepartmentName: '门诊内科', lines: [{}] },
      { encounterId: 'encounter-2', registrationDepartmentName: '门诊外科', lines: [{}] }
    ]
    expect(getJ6DepartmentOptions(departmentRows)).toEqual([
      { value: '门诊内科', label: '门诊内科' },
      { value: '门诊外科', label: '门诊外科' }
    ])
    expect(filterJ6EncounterRowsByDepartment(departmentRows, '门诊内科')).toEqual([
      departmentRows[0]
    ])
  })
})

describe('门诊费用明细导出响应适配', () => {
  it('将 J8 返回的文件地址识别为下载地址，不再按 Base64 解码', () => {
    expect(
      normalizeExportResponse({
        fileName: '门诊费用明细.xlsx',
        contentType: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
        body: ' http://files.example.test/report.xlsx?token=test '
      })
    ).toEqual({
      fileName: '门诊费用明细.xlsx',
      contentType: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
      body: undefined,
      downloadUrl: 'http://files.example.test/report.xlsx?token=test'
    })
  })

  it('Base64 文件体保持原有下载方式', () => {
    expect(normalizeExportResponse({ body: 'UEsDBAoAAAAA' })).toEqual({
      body: 'UEsDBAoAAAAA'
    })
  })
})
