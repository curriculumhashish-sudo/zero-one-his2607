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
          operationalEnum: 'AVAILABLE'
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
  it('加载不依赖前端拼装病区筛选的完整床位看板', async () => {
    const request = createDeferred<NurseStationBedBoardVO>()
    const querySpy = vi
      .spyOn(nurseStationApi, 'queryNurseStationBedBoard')
      .mockImplementationOnce(() => request.promise)

    const wrapper = mount(Index, {
      props: {
        patients: [],
        selectedPatientIds: []
      }
    })
    await flushPromises()

    request.resolve(createBoardResult('完整'))
    await flushPromises()
    expect(querySpy).toHaveBeenCalledWith({ panel: 'ADMISSION' })
    expect(wrapper.text()).toContain('完整疗区')
  })
})
