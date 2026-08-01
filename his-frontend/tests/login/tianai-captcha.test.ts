// @vitest-environment jsdom

import { mount } from '@vue/test-utils'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'

describe('TianaiCaptcha', () => {
  beforeEach(() => {
    vi.spyOn(HTMLCanvasElement.prototype, 'getContext').mockReturnValue(null)
  })

  afterEach(() => {
    vi.restoreAllMocks()
  })

  it('本地图片验证码输入正确字符后才通过验证', async () => {
    const wrapper = mount(TianaiCaptcha, {
      props: { mode: 'popup', modelValue: true },
      global: { stubs: { Teleport: true } }
    })

    const input = wrapper.get('input[aria-label="图片验证码输入"]')
    await input.setValue('XXXX')
    await wrapper.get('button.captcha-confirm').trigger('click')
    expect(wrapper.emitted('success')).toBeUndefined()

    const code = (wrapper.vm as unknown as { localCode: string }).localCode
    await input.setValue(code)
    await wrapper.get('button.captcha-confirm').trigger('click')
    expect(wrapper.emitted('success')).toHaveLength(1)
  })
})
