// @vitest-environment node

import { readFileSync } from 'node:fs'
import { fileURLToPath } from 'node:url'

import { describe, expect, it } from 'vitest'

const source = readFileSync(fileURLToPath(new URL('./Index.vue', import.meta.url)), 'utf8')

describe('数据统计页面区域', () => {
  it('三个小组统一只展示三个统计区域，不请求日/月流量趋势', () => {
    expect(source).not.toContain('title="访问流量趋势"')
    expect(source).not.toContain('title="月访问量"')
    expect(source).not.toContain('getTrafficTrend')
    expect(source).not.toContain('getMonthlyVisitCount')
    expect(source).not.toContain('getWeeklyOutpatientTrend')
    expect(source).not.toContain('getMonthlyOutpatientTrend')
    expect(source.match(/<StatisticChartCard/g)).toHaveLength(3)
  })
})
