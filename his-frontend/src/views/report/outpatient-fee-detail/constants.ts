export const tradeDirectionOptions = [
  { label: '全部', value: 'ALL' },
  { label: '收费', value: 'CHARGE' },
  { label: '退费', value: 'REFUND' }
]

export const feeDetailOptions = {
  departments: [
    { value: 'dept-internal', label: '心血管内科' },
    { value: 'dept-surgery', label: '普通外科' },
    { value: 'dept-pediatrics', label: '儿科' }
  ],
  cashiers: [
    { value: 'cashier-1', label: '李芳' },
    { value: 'cashier-2', label: '王强' }
  ],
  feeCategories: [
    { value: 'MEDICATION', label: '药品' },
    { value: 'ACTIVITY', label: '诊疗' },
    { value: 'DEVICE', label: '耗材' }
  ],
  subCategories: [
    { value: 'WESTERN_MEDICINE', label: '西药' },
    { value: 'EXAMINATION', label: '检查' },
    { value: 'CONSUMABLE', label: '医用耗材' }
  ]
}

export const summaryFields = [
  ['原价合计', 'baseAmountTotal'],
  ['优惠合计', 'discountAmountTotal'],
  ['应收合计', 'receivableAmountTotal'],
  ['折后合计', 'discountedAmountTotal'],
  ['行金额合计', 'lineAmountTotal'],
  ['实收合计', 'actualReceivedAmountTotal'],
  ['医保合计', 'insuranceAmountTotal'],
  ['自费合计', 'selfPayAmountTotal']
] as const
