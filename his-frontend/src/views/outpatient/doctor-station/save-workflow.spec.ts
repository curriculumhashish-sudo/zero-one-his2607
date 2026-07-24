import { describe, expect, it, vi } from 'vitest'

import { saveDirtyStationSections } from './save-workflow'

describe('doctor station save workflow', () => {
  it('一次调用会同时保存病历、诊断和医嘱三个已修改模块', async () => {
    const saveRecord = vi.fn(async () => true)
    const saveDiagnosis = vi.fn(async () => true)
    const saveOrders = vi.fn(async () => true)

    const result = await saveDirtyStationSections([
      { name: '病历', dirty: true, save: saveRecord },
      { name: '诊断', dirty: true, save: saveDiagnosis },
      { name: '医嘱', dirty: true, save: saveOrders }
    ])

    expect(saveRecord).toHaveBeenCalledOnce()
    expect(saveDiagnosis).toHaveBeenCalledOnce()
    expect(saveOrders).toHaveBeenCalledOnce()
    expect(result).toEqual({
      requested: ['病历', '诊断', '医嘱'],
      succeeded: ['病历', '诊断', '医嘱'],
      failed: []
    })
  })

  it('跳过没有修改的模块，避免产生无意义的保存请求', async () => {
    const saveRecord = vi.fn(async () => true)
    const saveDiagnosis = vi.fn(async () => true)

    await saveDirtyStationSections([
      { name: '病历', dirty: false, save: saveRecord },
      { name: '诊断', dirty: true, save: saveDiagnosis }
    ])

    expect(saveRecord).not.toHaveBeenCalled()
    expect(saveDiagnosis).toHaveBeenCalledOnce()
  })

  it('保留部分失败结果，方便页面给出准确的汇总提示', async () => {
    const result = await saveDirtyStationSections([
      { name: '病历', dirty: true, save: async () => true },
      { name: '诊断', dirty: true, save: async () => false },
      { name: '医嘱', dirty: true, save: async () => true }
    ])

    expect(result.succeeded).toEqual(['病历', '医嘱'])
    expect(result.failed).toEqual(['诊断'])
  })
})
