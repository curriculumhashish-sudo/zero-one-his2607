import { expect, describe, it, beforeAll, afterAll } from 'vitest'
import { withSetup } from '../test-apis-settings'
import { loginAccount } from '@/apis/login'

/**
 * 登录测试
 * 测试指令 pnpm test:unit login
 */
describe('login', () => {
  let app: any
  beforeAll(() => {
    app = withSetup()[1]
  })
  afterAll(() => {
    app.unmount()
  })
  // 测试调用登录接口
  it('login', async () => {
    const result = await loginAccount({ username: 'admin', password: '123456' })
    expect(result).toHaveProperty('accessToken')
    expect(result.user.userName).toBe('admin')
  })
})
