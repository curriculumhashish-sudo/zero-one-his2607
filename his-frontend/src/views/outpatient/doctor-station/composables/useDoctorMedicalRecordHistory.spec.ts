// @vitest-environment jsdom

import { effectScope, ref } from 'vue'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import type { DoctorPatientContextVO } from '@/apis/outpatient/doctor-station'

import { useDoctorMedicalRecordHistory } from './useDoctorMedicalRecordHistory'

const patient1003: DoctorPatientContextVO = {
  encounterId: 'enc-20260723-003',
  patientId: 'patient-1003',
  visitStatus: 'in-progress',
  visitStatusName: '在诊',
  attendingDoctorName: '李尔王',
  acceptedAt: '2026-07-23 08:32',
  phone: '138****1003'
}

describe('useDoctorMedicalRecordHistory', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  it('进入历史页签后才加载，并支持搜索与分页', async () => {
    const scope = effectScope()
    const context = ref<DoctorPatientContextVO | null>(patient1003)
    const state = scope.run(() => useDoctorMedicalRecordHistory({ patientContext: context }))!

    expect(state.historyRows.value).toEqual([])

    state.setHistoryActive(true)
    await vi.runAllTimersAsync()
    expect(state.historyRows.value).toHaveLength(10)
    expect(state.historyTotal.value).toBe(12)

    state.searchHistory('高血压')
    await vi.runAllTimersAsync()
    expect(state.historyQuery.pageIndex).toBe(1)
    expect(state.historyRows.value.length).toBeGreaterThan(0)
    expect(state.historyRows.value.every((item) => item.diagnosis?.includes('高血压'))).toBe(true)

    state.searchHistory('')
    await vi.runAllTimersAsync()
    state.changeHistoryPage({ page: 2, pageSize: 10 })
    await vi.runAllTimersAsync()
    expect(state.historyRows.value).toHaveLength(2)

    scope.stop()
  })

  it('切换患者时清空旧列表、关闭详情，并忽略旧请求', async () => {
    const scope = effectScope()
    const context = ref<DoctorPatientContextVO | null>(patient1003)
    const state = scope.run(() => useDoctorMedicalRecordHistory({ patientContext: context }))!

    state.setHistoryActive(true)
    await vi.runAllTimersAsync()
    const detailRequest = state.openHistoryDetail(state.historyRows.value[0]!)
    await vi.runAllTimersAsync()
    await detailRequest
    expect(state.historyDialogVisible.value).toBe(true)

    state.updateHistoryKeyword('心')
    void state.loadHistoryList()
    context.value = {
      ...patient1003,
      encounterId: 'enc-20260723-008',
      patientId: 'patient-1008'
    }
    await vi.runAllTimersAsync()

    expect(state.historyDialogVisible.value).toBe(false)
    expect(state.historyRows.value.every((item) => item.encounterId.includes('1008'))).toBe(true)
    expect(state.historyTotal.value).toBe(2)

    scope.stop()
  })
})
