import { describe, expect, it } from 'vitest'

import { parseLoginContextResponse } from '@/apis/login'

describe('登录上下文长 ID', () => {
  it('在 JSON 解析前把超出安全整数范围的登录上下文 ID 保留为字符串', () => {
    const response = parseLoginContextResponse(
      '{"code":10000,"message":"成功","data":[{"id":1912345678901234567,"tenantId":1812345678901234567,"displayName":"测试医院"}]}'
    ) as {
      data: Array<{ id: string; tenantId: string; displayName: string }>
    }

    expect(response.data[0]).toEqual({
      id: '1912345678901234567',
      tenantId: '1812345678901234567',
      displayName: '测试医院'
    })
  })

  it('不改变普通数字和非字符串响应', () => {
    expect(parseLoginContextResponse('{"code":10000,"data":[{"id":1001}]}')).toEqual({
      code: 10000,
      data: [{ id: 1001 }]
    })

    const response = { code: 10000, data: [] }
    expect(parseLoginContextResponse(response)).toBe(response)
  })
})
