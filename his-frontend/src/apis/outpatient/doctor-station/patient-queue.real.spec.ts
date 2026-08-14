import dayjs from 'dayjs'
import { describe, expect, it, vi } from 'vitest'

const get = vi.fn()

vi.mock('@/plugins/http', () => ({
  useHttp: () => ({ get })
}))

import { listDoctorPatientQueue } from './index'

describe('门诊医生站患者队列 Mock 切换', () => {
  it('查询患者时使用 Mock 数据且不调用真实 HTTP', async () => {
    const result = await listDoctorPatientQueue({ visitDate: dayjs().format('YYYY-MM-DD') })

    expect(get).not.toHaveBeenCalled()
    expect(result.length).toBeGreaterThan(0)
    expect(result.every((item) => Boolean(item.patientId && item.encounterId))).toBe(true)
  })

  it('保留患者姓名和状态筛选能力', async () => {
    const visitDate = dayjs().format('YYYY-MM-DD')
    const all = await listDoctorPatientQueue({ visitDate })
    const target = all[0]!

    const result = await listDoctorPatientQueue({
      visitDate,
      patientName: ` ${target.patientName} `,
      visitStatus: target.visitStatus
    })

    expect(result).toContainEqual(target)
  })

  it('Mock 查询失败时保留错误提示', async () => {
    await expect(
      listDoctorPatientQueue({
        visitDate: dayjs().format('YYYY-MM-DD'),
        patientName: '失败'
      })
    ).rejects.toThrow('模拟患者队列加载失败')
  })
})
