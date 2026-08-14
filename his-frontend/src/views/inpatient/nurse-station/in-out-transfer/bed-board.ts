import type {
  NurseStationBedBoardItemVO,
  NurseStationBedLocationVO
} from '@/apis/inpatient/nurse-station'

export interface NurseStationBedHouseGroup {
  houseId: string
  houseName: string
  beds: NurseStationBedBoardItemVO[]
}

export interface NurseStationBedWardGroup {
  wardId: string
  wardName: string
  houses: NurseStationBedHouseGroup[]
}

/** 将扁平床位接口数据整理为“病区 → 病房 → 床位”的页面结构。 */
export function groupNurseStationBedBoard(items: NurseStationBedBoardItemVO[]) {
  const wardMap = new Map<string, NurseStationBedWardGroup>()
  for (const item of items) {
    const { bed } = item
    let ward = wardMap.get(bed.wardId)
    if (!ward) {
      ward = { wardId: bed.wardId, wardName: bed.wardName, houses: [] }
      wardMap.set(bed.wardId, ward)
    }
    let house = ward.houses.find((row) => row.houseId === bed.houseId)
    if (!house) {
      house = { houseId: bed.houseId, houseName: bed.houseName, beds: [] }
      ward.houses.push(house)
    }
    house.beds.push(item)
  }
  return [...wardMap.values()]
}

export function isNurseStationBedAvailable(bed: NurseStationBedLocationVO) {
  return bed.operationalEnum === 'AVAILABLE'
}
