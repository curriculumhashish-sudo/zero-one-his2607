import { describe, expect, it } from 'vitest'
import { shallowMount } from '@vue/test-utils'

import AdviceOrderTable from '@/components/medical/AdviceOrderTable.vue'
import DispenseCenterPanel from './components/DispenseCenterPanel.vue'
import type { DispensePrescriptionBlock, DispensePrescriptionItemRow } from './type'

function createItem(id: string, inventoryValue: string): DispensePrescriptionItemRow {
  return {
    id,
    key: id,
    rxId: `rx-${id}`,
    rxNo: `RX-${id}`,
    isFirstInRx: true,
    isLastInRx: true,
    rxRowSpan: 1,
    itemName: `药品-${id}`,
    name: `药品-${id}`,
    spec: '10mg',
    specification: '10mg',
    quantity: 1,
    dosage: '10mg',
    days: 1,
    totalPrice: 1,
    sourceStatus: 'PENDING',
    status: 'pending',
    batchNo: 'LOT-1',
    traceCode: '',
    factory: '测试厂家',
    inventoryPickOptions: [{ label: '门诊药房（LOT-1）', value: inventoryValue }]
  } as DispensePrescriptionItemRow
}

describe('DispenseCenterPanel', () => {
  it('按处方类型过滤，并为唯一库存批次建立可用选择', async () => {
    const blocks: DispensePrescriptionBlock[] = [
      {
        id: 'rx-western',
        rxNo: 'RX-W',
        rxType: 'western',
        status: 'pending',
        items: [createItem('western-line', 'pharmacy-1|LOT-1')]
      },
      {
        id: 'rx-herbal',
        rxNo: 'RX-H',
        rxType: 'herbal',
        status: 'pending',
        items: [createItem('herbal-line', 'pharmacy-2|LOT-2')]
      }
    ]

    const wrapper = shallowMount(DispenseCenterPanel, {
      props: {
        prescriptionBlocks: blocks,
        prescriptionType: 'western',
        loading: false,
        queueScope: 'pending',
        dispenserOptions: [],
        selectedDispenserId: null
      }
    })
    const exposed = wrapper.vm as unknown as {
      getInventoryPick: (lineId: string) => string
    }

    expect(exposed.getInventoryPick('western-line')).toBe('pharmacy-1|LOT-1')
    expect(wrapper.findComponent(AdviceOrderTable).props('rows')).toEqual([
      expect.objectContaining({ id: 'western-line' })
    ])

    await wrapper.setProps({ prescriptionType: 'herbal' })

    expect(wrapper.findComponent(AdviceOrderTable).props('rows')).toEqual([
      expect.objectContaining({ id: 'herbal-line' })
    ])
  })
})
