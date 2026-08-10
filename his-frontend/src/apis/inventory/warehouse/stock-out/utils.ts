/** 将 YYYY-MM-DD 补全为后端要求的 yyyy-MM-dd HH:mm:ss 格式 */
export function toDateTime(dateStr: string | undefined, isEnd: boolean): string | undefined {
  if (!dateStr) return undefined
  return `${dateStr} ${isEnd ? '23:59:59' : '00:00:00'}`
}

/** 后端 statusEnum 字符串 → 前端 status 数字 */
export const STATUS_MAP: Record<string, number> = {
  DRAFT: 0,
  PENDING_APPROVAL: 1,
  APPROVAL: 2,
  WITHDRAW: 3,
  AGREE: 4,
  REJECT: 5
}

/** 后端 statusEnum 字符串 → 前端 statusEnum_enumText */
export const STATUS_TEXT_MAP: Record<string, string> = {
  DRAFT: '编辑中',
  PENDING_APPROVAL: '待审核',
  APPROVAL: '审核中',
  WITHDRAW: '已撤回',
  AGREE: '同意',
  REJECT: '驳回'
}

/** 前端 status 数字 → 后端 statusEnum 字符串 */
export const STATUS_ENUM_MAP: Record<number, string> = {
  0: 'DRAFT',
  1: 'PENDING_APPROVAL',
  2: 'APPROVAL',
  3: 'WITHDRAW',
  4: 'AGREE',
  5: 'REJECT'
}
