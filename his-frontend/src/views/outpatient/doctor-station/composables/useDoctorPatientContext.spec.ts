import { effectScope } from 'vue'
import { afterEach, describe, expect, it, vi } from 'vitest'

import type { DoctorPatientContextVO } from '@/apis/outpatient/doctor-station'

import { useDoctorPatientContext } from './useDoctorPatientContext'

const { acceptPatientMock } = vi.hoisted(() => ({
  acceptPatientMock: vi.fn()
}))

vi.mock('@/apis/outpatient/doctor-station', async (importOriginal) => ({
  ...(await importOriginal<typeof import('@/apis/outpatient/doctor-station')>()),
  acceptPatient: acceptPatientMock
}))

function createContext(encounterId: string): DoctorPatientContextVO {
  return {
    encounterId,
    patientId: `patient-${encounterId}`,
    visitStatus: 'in-progress',
    visitStatusName: '在诊',
    phone: '138****0000'
  }
}

function createDeferred<T>() {
  let resolve!: (value: T) => void
  let reject!: (reason?: unknown) => void
  const promise = new Promise<T>((resolvePromise, rejectPromise) => {
    resolve = resolvePromise
    reject = rejectPromise
  })
  return { promise, resolve, reject }
}

describe('useDoctorPatientContext', () => {
  afterEach(() => {
    vi.clearAllMocks()
  })

  it('通过接诊入口加载并保存患者上下文', async () => {
    const scope = effectScope()
    const state = scope.run(() => useDoctorPatientContext())!
    const expected = createContext('encounter-1')
    acceptPatientMock.mockResolvedValueOnce(expected)

    await expect(state.acceptOrLoadPatient('encounter-1')).resolves.toEqual(expected)
    expect(acceptPatientMock).toHaveBeenCalledOnce()
    expect(state.patientContext.value).toEqual(expected)
    expect(state.patientAccepted.value).toBe(true)
    expect(state.contextError.value).toBe('')

    scope.stop()
  })

  it('快速切换患者时忽略旧患者的迟到响应', async () => {
    const scope = effectScope()
    const state = scope.run(() => useDoctorPatientContext())!
    const first = createDeferred<DoctorPatientContextVO>()
    const second = createDeferred<DoctorPatientContextVO>()
    acceptPatientMock.mockReturnValueOnce(first.promise).mockReturnValueOnce(second.promise)

    const firstRequest = state.acceptOrLoadPatient('encounter-1')
    const secondRequest = state.acceptOrLoadPatient('encounter-2')
    first.resolve(createContext('encounter-1'))

    await expect(firstRequest).resolves.toBeNull()
    expect(state.patientContext.value).toBeNull()

    const current = createContext('encounter-2')
    second.resolve(current)
    await expect(secondRequest).resolves.toEqual(current)
    expect(state.patientContext.value).toEqual(current)

    scope.stop()
  })

  it('加载失败时清空旧上下文并保留可重试错误', async () => {
    const scope = effectScope()
    const state = scope.run(() => useDoctorPatientContext())!
    acceptPatientMock.mockRejectedValueOnce(new Error('接诊服务不可用'))

    await expect(state.acceptOrLoadPatient('encounter-1')).rejects.toThrow('接诊服务不可用')
    expect(state.patientContext.value).toBeNull()
    expect(state.contextError.value).toBe('接诊服务不可用')
    expect(state.contextLoading.value).toBe(false)

    scope.stop()
  })
})
