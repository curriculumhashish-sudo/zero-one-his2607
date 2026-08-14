import type { InventoryAmountComparisonResponse } from './type'

type LooseRecord = Record<string, any>

function inventoryDirection(value: unknown): 'INBOUND' | 'OUTBOUND' {
  return /inbound|^in$|入库/i.test(String(value ?? '')) ? 'INBOUND' : 'OUTBOUND'
}

export function toInventory(data: LooseRecord): InventoryAmountComparisonResponse {
  if (Array.isArray(data?.categories) && Array.isArray(data?.series)) {
    return {
      categories: data.categories.map(String),
      series: data.series.map((item: LooseRecord) => ({
        name: String(item.name ?? ''),
        direction: inventoryDirection(item.direction ?? item.type ?? item.stack),
        values: Array.isArray(item.values) ? item.values.map(Number) : []
      })),
      unit: String(data.unit ?? '金额')
    }
  }
  if (Array.isArray(data?.xaxis) && Array.isArray(data?.series)) {
    return {
      categories: data.xaxis.map(String),
      series: data.series.map((item: LooseRecord) => ({
        name: String(item.name ?? ''),
        direction: inventoryDirection(item.stack ?? item.direction ?? item.name),
        values: Array.isArray(item.data) ? item.data.map(Number) : []
      })),
      unit: String(data.unit ?? '金额')
    }
  }
  const rows = Array.isArray(data)
    ? data
    : Array.isArray(data?.data)
      ? data.data
      : Array.isArray(data?.rows)
        ? data.rows
        : []
  const categories = [
    ...new Set(rows.map((item: LooseRecord) => String(item.period ?? item.name ?? '')))
  ]
  const hasDirection = rows.some((item: LooseRecord) => item.type !== undefined)
  const inbound = hasDirection
    ? rows.filter((item: LooseRecord) => /in|入/.test(String(item.type ?? '')))
    : rows
  const outbound = hasDirection
    ? rows.filter((item: LooseRecord) => /out|出/.test(String(item.type ?? '')))
    : rows
  return {
    categories,
    series: [
      {
        name: '入库',
        direction: 'INBOUND',
        values: categories.map((category) =>
          Number(
            inbound.find((item: LooseRecord) => String(item.period ?? item.name) === category)
              ?.totalAmount ??
              inbound.find((item: LooseRecord) => String(item.period ?? item.name) === category)
                ?.allmoney ??
              0
          )
        )
      },
      {
        name: '出库',
        direction: 'OUTBOUND',
        values: categories.map((category) =>
          Number(
            outbound.find((item: LooseRecord) => String(item.period ?? item.name) === category)
              ?.totalAmount ??
              outbound.find((item: LooseRecord) => String(item.period ?? item.name) === category)
                ?.alloutmoney ??
              outbound.find((item: LooseRecord) => String(item.period ?? item.name) === category)
                ?.outmoney ??
              0
          )
        )
      }
    ],
    unit: String(data?.unit ?? '金额')
  }
}
