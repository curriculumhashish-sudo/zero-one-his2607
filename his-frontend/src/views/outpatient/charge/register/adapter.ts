import type { BookableSlotHint, RegisterTicketResult } from '@/apis/outpatient/charge/register'

export type ResidentIdGender = 'FEMALE' | 'MALE'

export function calculateAge(birthDate: string, referenceDate = new Date()) {
  if (!birthDate) return null
  const [yearText, monthText, dayText] = birthDate.split('-')
  const year = Number.parseInt(yearText ?? '', 10)
  const month = Number.parseInt(monthText ?? '', 10)
  const day = Number.parseInt(dayText ?? '', 10)
  const parsed = new Date(year, month - 1, day)
  if (
    !Number.isInteger(year) ||
    !Number.isInteger(month) ||
    !Number.isInteger(day) ||
    parsed.getFullYear() !== year ||
    parsed.getMonth() !== month - 1 ||
    parsed.getDate() !== day
  ) {
    return null
  }
  let age = referenceDate.getFullYear() - year
  const birthdayPassed =
    referenceDate.getMonth() + 1 > month ||
    (referenceDate.getMonth() + 1 === month && referenceDate.getDate() >= day)
  if (!birthdayPassed) age -= 1
  return age >= 0 ? age : null
}

/** 与参考原型一致：解析 18 位居民身份证，不校验末位校验码。 */
export function parseResidentIdCard(value: string) {
  const idCard = value.trim().toUpperCase()
  if (!/^\d{17}[\dX]$/.test(idCard)) return null
  const birthText = idCard.slice(6, 14)
  const birthDate = `${birthText.slice(0, 4)}-${birthText.slice(4, 6)}-${birthText.slice(6, 8)}`
  const age = calculateAge(birthDate)
  if (age == null) return null
  const gender: ResidentIdGender = Number(idCard.charAt(16)) % 2 === 1 ? 'MALE' : 'FEMALE'
  return { age, birthDate, gender }
}

export function formatLocalDate(date = new Date()) {
  const year = date.getFullYear()
  const month = String(date.getMonth() + 1).padStart(2, '0')
  const day = String(date.getDate()).padStart(2, '0')
  return `${year}-${month}-${day}`
}

export function slotTimeLabel(slot: BookableSlotHint) {
  return `${slot.slotStart?.slice(11, 16) ?? '--:--'}-${slot.slotEnd?.slice(11, 16) ?? '--:--'}`
}

export function toTicketDetail(ticket: RegisterTicketResult | null) {
  if (!ticket?.slip) return null
  const slip = ticket.slip
  return {
    slipNo: slip.slipNo,
    patientName: slip.patientName,
    gender: slip.genderEnumText,
    department: slip.registerDepartmentName ?? slip.organizationName,
    doctor: slip.doctorName,
    serviceType: slip.serviceTypeName,
    feeNature: slip.feeNatureText,
    total: slip.totalAmountText,
    chargeTime: slip.chargeTimeText,
    cashier: slip.cashierName,
    printCount: ticket.printCount
  }
}
