// @vitest-environment jsdom

import { beforeEach, describe, expect, it } from 'vitest'

import {
  persistNavigationAuth,
  readNavigationAuth,
  resolveExternalNavigationUrl
} from './navigation-auth'

describe('页面跳转鉴权参数', () => {
  beforeEach(() => localStorage.clear())

  it('跳转时从当前会话携带 token 和 tokenHead', () => {
    localStorage.setItem('token', 'session-token')
    localStorage.setItem('tokenHead', 'Bearer')

    expect(readNavigationAuth()).toEqual({ token: 'session-token', tokenHead: 'Bearer' })
  })

  it('目标页优先接收 URL 中的新 token 并写入当前会话', () => {
    localStorage.setItem('token', 'old-token')

    expect(persistNavigationAuth('?token=new-token&tokenHead=JWT')).toEqual({
      token: 'new-token',
      tokenHead: 'JWT'
    })
    expect(localStorage.getItem('token')).toBe('new-token')
    expect(localStorage.getItem('tokenHead')).toBe('JWT')
  })

  it('跨端口预约登记跳转到配置的主收费应用', () => {
    expect(
      resolveExternalNavigationUrl(
        '/outpatient/charge/register?slotId=slot-1',
        'http://localhost:3000',
        'http://127.0.0.1:3002'
      )
    ).toBe('http://localhost:3000/outpatient/charge/register?slotId=slot-1')
    expect(
      resolveExternalNavigationUrl(
        '/outpatient/charge/register?slotId=slot-1',
        'http://localhost:3000',
        'http://localhost:3000'
      )
    ).toBe('')
  })
})
