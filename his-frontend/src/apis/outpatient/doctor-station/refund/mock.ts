import type { DoctorRefundApplicationDTO, DoctorRefundItemVO, DoctorRefundQuery } from './type'

/**
 * 模拟收费系统中可供医生查看的退费清单。
 *
 * 退费申请提交后只改变退费状态，不删除原收费记录，便于审计和防止重复申请。
 */
const refundItemsByPatientNo = new Map<string, DoctorRefundItemVO[]>([
  [
    'MR-DEMO-0001',
    [
      {
        patientName: '全功能演示患者',
        paymentNo: 'PAY-DEMO-0001',
        itemName: '血常规',
        quantity: '1 次',
        amount: '25.00',
        chargeStatus: '已收费',
        refundStatus: '未申请'
      },
      {
        patientName: '全功能演示患者',
        paymentNo: 'PAY-DEMO-0001',
        itemName: '十二导联心电图',
        quantity: '1 次',
        amount: '30.00',
        chargeStatus: '已支付',
        refundStatus: '可申请'
      },
      {
        patientName: '全功能演示患者',
        paymentNo: 'PAY-DEMO-0002',
        itemName: '心脏彩色多普勒超声',
        quantity: '1 次',
        amount: '160.00',
        chargeStatus: '未收费',
        refundStatus: '未申请'
      },
      {
        patientName: '全功能演示患者',
        paymentNo: 'PAY-DEMO-HISTORY',
        itemName: '血脂四项',
        quantity: '1 次',
        amount: '48.00',
        chargeStatus: '已收费',
        refundStatus: '已退费'
      }
    ]
  ],
  [
    'MR20260003',
    [
      {
        patientName: '王志强',
        paymentNo: 'PAY202608010001',
        itemName: '阿司匹林肠溶片',
        quantity: '20 片',
        amount: '18.60',
        chargeStatus: '已收费',
        refundStatus: '未申请'
      },
      {
        patientName: '王志强',
        paymentNo: 'PAY202608010001',
        itemName: '阿托伐他汀钙片',
        quantity: '14 片',
        amount: '35.20',
        chargeStatus: '已收费',
        refundStatus: '未申请'
      },
      {
        patientName: '王志强',
        paymentNo: 'PAY202608010002',
        itemName: '血常规检查',
        quantity: '1 次',
        amount: '25.00',
        chargeStatus: '已收费',
        refundStatus: '未申请'
      },
      {
        patientName: '王志强',
        paymentNo: 'PAY202607280009',
        itemName: '十二导联心电图',
        quantity: '1 次',
        amount: '30.00',
        chargeStatus: '已收费',
        refundStatus: '已退费'
      }
    ]
  ],
  [
    'MR20260005',
    [
      {
        patientName: '陈晨',
        paymentNo: 'PAY202608010005',
        itemName: '心肌酶谱',
        quantity: '1 次',
        amount: '72.00',
        chargeStatus: '已收费',
        refundStatus: '退费中'
      }
    ]
  ],
  [
    'MR20260008',
    [
      {
        patientName: '吴国平',
        paymentNo: 'PAY202608010008',
        itemName: '氯沙坦钾片',
        quantity: '14 片',
        amount: '28.40',
        chargeStatus: '已收费',
        refundStatus: '未申请'
      },
      {
        patientName: '吴国平',
        paymentNo: 'PAY202608010008',
        itemName: '动态血压监测',
        quantity: '1 次',
        amount: '180.00',
        chargeStatus: '未收费',
        refundStatus: '未申请'
      }
    ]
  ]
])

let refundApplicationSequence = 1

function delay(ms = 160) {
  return new Promise<void>((resolve) => setTimeout(resolve, ms))
}

function cloneItem(item: DoctorRefundItemVO): DoctorRefundItemVO {
  return { ...item }
}

function itemKey(item: DoctorRefundItemVO) {
  return [item.paymentNo, item.itemName, item.quantity, item.amount].join('|')
}

function amountToCents(value: string | number | undefined) {
  const amount = Number(value ?? 0)
  if (!Number.isFinite(amount)) throw new Error('退费明细中存在无效金额')
  return Math.round(amount * 100)
}

function isRefundable(item: DoctorRefundItemVO) {
  const chargeStatus = item.chargeStatus?.trim() ?? ''
  const refundStatus = item.refundStatus?.trim() ?? ''
  return (
    ['已收费', '已支付'].includes(chargeStatus) &&
    ['', '未申请', '未退费', '可申请'].includes(refundStatus)
  )
}

/** 模拟 GET /cyclectrl/refund_query。 */
export async function mockQueryDoctorRefundItems(
  query: DoctorRefundQuery
): Promise<DoctorRefundItemVO[]> {
  await delay()
  const patientNo = query.num?.trim()
  if (!patientNo) throw new Error('缺少患者病历号')
  return (refundItemsByPatientNo.get(patientNo) ?? []).map(cloneItem)
}

/**
 * 模拟 POST /cyclectrl/refund_apply。
 *
 * 服务端再次校验清单归属、勾选数量、金额和当前退费状态，避免旧页面或并发请求
 * 对同一收费项重复提交退费申请。
 */
export async function mockApplyDoctorRefund(data: DoctorRefundApplicationDTO): Promise<string> {
  await delay()

  const patientNo = data.patientNo?.trim()
  const selectedItems = data.apply ?? []
  if (!patientNo) throw new Error('缺少患者病历号')
  if (selectedItems.length === 0) throw new Error('请至少选择一项退费明细')
  if (data.applyNo !== selectedItems.length) throw new Error('退费勾选数量不一致，请刷新后重试')

  const selectedKeys = selectedItems.map(itemKey)
  if (new Set(selectedKeys).size !== selectedKeys.length) {
    throw new Error('退费明细存在重复项')
  }

  const selectedAmountCents = selectedItems.reduce(
    (sum, item) => sum + amountToCents(item.amount),
    0
  )
  if (selectedAmountCents !== amountToCents(data.applySum)) {
    throw new Error('退费合计金额不一致，请刷新后重试')
  }

  const storedItems = refundItemsByPatientNo.get(patientNo)
  if (!storedItems) throw new Error('未找到患者退费清单')

  const matchedItems = selectedKeys.map((key) => {
    const item = storedItems.find((candidate) => itemKey(candidate) === key)
    if (!item) throw new Error('退费明细已变化，请刷新后重试')
    if (!isRefundable(item)) throw new Error(`“${item.itemName || '收费项目'}”当前不可申请退费`)
    return item
  })

  matchedItems.forEach((item) => {
    item.refundStatus = '退费中'
  })

  refundApplicationSequence += 1
  return `RF${new Date().toISOString().slice(0, 10).replaceAll('-', '')}${String(
    refundApplicationSequence
  ).padStart(4, '0')}`
}
