import { flushPromises, mount } from '@vue/test-utils'
import { afterEach, describe, expect, it, vi } from 'vitest'

import * as nurseStationApi from '@/apis/inpatient/nurse-station'
import type { NurseStationBedBoardVO } from '@/apis/inpatient/nurse-station'

import Index from './Index.vue'

function createDeferred<T>() {
  let resolve!: (value: T) => void
  const promise = new Promise<T>((done) => {
    resolve = done
  })
  return { promise, resolve }
}

function createBoardResult(label: string): NurseStationBedBoardVO {
  return {
    items: [
      {
        bed: {
          bedId: `bed-${label}`,
          bedName: `${label}床`,
          wardId: `ward-${label}`,
          wardName: `${label}疗区`,
          houseId: `house-${label}`,
          houseName: `${label}病房`,
          displayOrder: 1,
          operationalEnum: 'AVAILABLE',
          version: 1
        },
        occupant: null
      }
    ]
  }
}

afterEach(() => {
  vi.restoreAllMocks()
})

describe('入出转床位看板请求保护', () => {
  it('快速切换筛选时只展示最后一次请求结果', async () => {
    const firstRequest = createDeferred<NurseStationBedBoardVO>()
    const secondRequest = createDeferred<NurseStationBedBoardVO>()
    vi.spyOn(nurseStationApi, 'queryNurseStationBedBoard')
      .mockImplementationOnce(() => firstRequest.promise)
      .mockImplementationOnce(() => secondRequest.promise)

    const wrapper = mount(Index, {
      props: {
        patients: [],
        selectedPatientIds: [],
        selectedWardIds: [],
        selectedHouseIds: []
      }
    })
    await flushPromises()

    await wrapper.setProps({ selectedWardIds: ['ward-latest'] })
    secondRequest.resolve(createBoardResult('最新'))
    await flushPromises()
    expect(wrapper.text()).toContain('最新疗区')

    // 较早请求最后才返回，也不能把已经显示的最新筛选结果覆盖掉。
    firstRequest.resolve(createBoardResult('过期'))
    await flushPromises()
    expect(wrapper.text()).toContain('最新疗区')
    expect(wrapper.text()).not.toContain('过期疗区')
  })
})
