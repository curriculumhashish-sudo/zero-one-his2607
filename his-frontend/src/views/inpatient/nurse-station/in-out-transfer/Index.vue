<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue'
import { Refresh } from '@element-plus/icons-vue'
import { ElMessage } from 'element-plus'

import {
  assignNurseStationBed,
  queryNurseStationBedBoard,
  type NurseStationBedBoardItemVO,
  type NurseStationBedLocationVO
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'
import { groupNurseStationBedBoard, isNurseStationBedAvailable } from './bed-board'
import BedAssignmentDialog from './components/BedAssignmentDialog.vue'
import DischargePanel from './components/DischargePanel.vue'

defineOptions({ name: 'NurseStationInOutTransfer' })

type Panel = 'admission' | 'discharge'
type BedFilter = 'all' | 'empty' | 'occupied'

const props = defineProps<{
  patients: NurseStationPatientSummary[]
  selectedPatientIds: string[]
}>()

const emit = defineEmits<{
  'update:selectedPatientIds': [value: string[]]
  patientsChanged: []
}>()

const PATIENT_DRAG_MIME = 'application/x-his-nurse-patient'
const activePanel = ref<Panel>('admission')
const bedFilter = ref<BedFilter>('all')
const boardItems = ref<NurseStationBedBoardItemVO[]>([])
const boardLoading = ref(false)
const actionLoading = ref(false)
const hoverBedId = ref('')
const assignmentDialogVisible = ref(false)
const assignmentPatient = ref<NurseStationPatientSummary | null>(null)
const assignmentBed = ref<NurseStationBedLocationVO | null>(null)
let boardRequestSerial = 0

const visibleBoardItems = computed(() => {
  if (activePanel.value !== 'admission' || bedFilter.value === 'all') return boardItems.value
  return boardItems.value.filter((item) =>
    bedFilter.value === 'empty' ? !item.occupant : Boolean(item.occupant)
  )
})
const wardGroups = computed(() => groupNurseStationBedBoard(visibleBoardItems.value))
const selectedDischargePatient = computed(() => {
  const selectedId = props.selectedPatientIds[0]
  return (
    boardItems.value.find((item) => item.occupant?.encounterId === selectedId)?.occupant ?? null
  )
})
const unassignedPatients = computed(() => props.patients.filter((patient) => !patient.bedId))

function buildBoardQuery() {
  return {
    panel: activePanel.value === 'discharge' ? ('DISCHARGE' as const) : ('ADMISSION' as const)
  }
}

async function loadBedBoard() {
  const serial = ++boardRequestSerial
  const requestedPanel = activePanel.value
  const query = buildBoardQuery()
  boardLoading.value = true
  try {
    const result = await queryNurseStationBedBoard(query)
    // 用户快速切换床位预约或出院时，只允许最后一次请求更新床位图。
    if (serial !== boardRequestSerial || activePanel.value !== requestedPanel) return
    boardItems.value = result.items
    if (requestedPanel === 'discharge') {
      const availableIds = new Set(
        result.items.flatMap((item) => (item.occupant ? [item.occupant.encounterId] : []))
      )
      if (!availableIds.has(props.selectedPatientIds[0] ?? '')) {
        emit(
          'update:selectedPatientIds',
          result.items[0]?.occupant?.encounterId ? [result.items[0].occupant.encounterId] : []
        )
      }
    }
  } catch (error) {
    if (serial !== boardRequestSerial) return
    ElMessage.error(error instanceof Error ? error.message : '加载床位看板失败')
  } finally {
    if (serial === boardRequestSerial) boardLoading.value = false
  }
}

watch(activePanel, () => void loadBedBoard())

onMounted(() => void loadBedBoard())

function selectOccupant(encounterId: string) {
  emit('update:selectedPatientIds', [encounterId])
}

function readDraggedEncounterId(event: DragEvent) {
  const raw =
    event.dataTransfer?.getData(PATIENT_DRAG_MIME) || event.dataTransfer?.getData('text/plain')
  if (!raw) return ''
  try {
    return JSON.parse(raw).encounterId ?? raw
  } catch {
    return raw
  }
}

function dropPatientOnBed(event: DragEvent, target: NurseStationBedBoardItemVO) {
  if (activePanel.value !== 'admission') return
  if (actionLoading.value) return
  hoverBedId.value = ''
  if (!isNurseStationBedAvailable(target.bed) && !target.occupant) {
    ElMessage.warning('该床位当前不可用')
    return
  }
  const encounterId = readDraggedEncounterId(event)
  const source = props.patients.find((patient) => patient.encounterId === encounterId)
  if (!source) return
  if (source.bedId) {
    ElMessage.warning('该患者已有床位，不能重复预约')
    return
  }
  if (target.occupant) {
    ElMessage.warning('待预约患者只能拖入空床')
    return
  }
  assignmentPatient.value = source
  assignmentBed.value = target.bed
  assignmentDialogVisible.value = true
}

async function confirmAssignment() {
  if (!assignmentPatient.value || !assignmentBed.value) return
  if (actionLoading.value) return
  actionLoading.value = true
  try {
    await assignNurseStationBed({
      encounterId: assignmentPatient.value.encounterId,
      patientId: assignmentPatient.value.patientId,
      targetBedId: assignmentBed.value.bedId
    })
    assignmentDialogVisible.value = false
    ElMessage.success('床位预约成功')
    emit('patientsChanged')
    await loadBedBoard()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '床位预约失败，请刷新后重试')
  } finally {
    actionLoading.value = false
  }
}

async function handleDischargeChanged() {
  emit('patientsChanged')
  await loadBedBoard()
}
</script>

<template>
  <section v-loading="boardLoading || actionLoading" class="atd-module">
    <header class="atd-module__switch">
      <el-segmented
        v-model="activePanel"
        :options="[
          { label: '床位预约', value: 'admission' },
          { label: '出院', value: 'discharge' }
        ]"
      />
      <el-button :icon="Refresh" circle title="刷新床位图" @click="loadBedBoard" />
    </header>

    <template v-if="activePanel === 'admission'">
      <div class="atd-module__toolbar">
        <div>
          <span class="toolbar-label">床位</span>
          <el-radio-group v-model="bedFilter" size="small">
            <el-radio-button value="all">全部</el-radio-button>
            <el-radio-button value="empty">空床</el-radio-button>
            <el-radio-button value="occupied">占床</el-radio-button>
          </el-radio-group>
        </div>
        <span class="toolbar-hint">从左侧拖动待预约患者至空床</span>
      </div>

      <el-alert
        v-if="unassignedPatients.length"
        type="warning"
        show-icon
        :closable="false"
        class="atd-module__alert"
      >
        <template #title>
          待预约床位 {{ unassignedPatients.length }} 人：{{
            unassignedPatients.map((item) => item.patientName).join('、')
          }}
        </template>
      </el-alert>
    </template>

    <div class="bed-board-scroll">
      <template v-if="wardGroups.length">
        <section v-for="ward in wardGroups" :key="ward.wardId" class="bed-ward">
          <h2>{{ ward.wardName }}</h2>
          <div v-for="house in ward.houses" :key="house.houseId" class="bed-house">
            <header>
              <strong>{{ house.houseName }}</strong>
              <span>{{ house.beds.length }} 床</span>
            </header>
            <div class="bed-grid">
              <div
                v-for="item in house.beds"
                :key="item.bed.bedId"
                class="bed-drop-zone"
                :class="{ 'is-hover': hoverBedId === item.bed.bedId }"
                @dragover.prevent="hoverBedId = item.bed.bedId"
                @dragleave="hoverBedId = ''"
                @drop.prevent="dropPatientOnBed($event, item)"
              >
                <article
                  v-if="item.occupant"
                  class="bed-card bed-card--occupied"
                  :class="{ 'is-selected': selectedPatientIds.includes(item.occupant.encounterId) }"
                  tabindex="0"
                  @click="selectOccupant(item.occupant.encounterId)"
                  @keydown.enter="selectOccupant(item.occupant.encounterId)"
                >
                  <span class="bed-card__stripe" />
                  <div class="bed-card__top">
                    <strong>{{ item.bed.bedName }}</strong>
                    <el-tag size="small" effect="plain">{{
                      item.occupant.statusEnumText || '在院'
                    }}</el-tag>
                  </div>
                  <div class="bed-card__name">{{ item.occupant.patientName }}</div>
                  <div class="bed-card__meta">
                    {{ item.occupant.genderEnumText || '-' }} {{ item.occupant.ageText || '-' }}
                  </div>
                </article>
                <article
                  v-else
                  class="bed-card bed-card--empty"
                  :class="{ 'is-disabled': item.bed.operationalEnum === 'DISABLED' }"
                >
                  <span class="bed-card__stripe" />
                  <strong>{{ item.bed.bedName }}</strong>
                  <span>{{ item.bed.operationalEnum === 'DISABLED' ? '停用' : '空床' }}</span>
                </article>
              </div>
            </div>
          </div>
        </section>
      </template>
      <el-empty v-else description="当前范围暂无可展示床位" />
    </div>

    <DischargePanel
      v-if="activePanel === 'discharge'"
      :patient="selectedDischargePatient"
      @changed="handleDischargeChanged"
    />

    <BedAssignmentDialog
      v-model="assignmentDialogVisible"
      :patient="assignmentPatient"
      :bed="assignmentBed"
      :submitting="actionLoading"
      @confirm="confirmAssignment"
    />
  </section>
</template>

<style scoped>
.atd-module {
  display: flex;
  height: 100%;
  min-height: 0;
  color: var(--nurse-text-regular);
  flex-direction: column;
  overflow: hidden;
  background: var(--el-fill-color-lighter);
}

.atd-module__switch,
.atd-module__toolbar {
  display: flex;
  min-height: 48px;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 0 14px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.atd-module__toolbar {
  min-height: 46px;
}

.toolbar-label {
  margin-right: 10px;
  color: var(--nurse-text-regular);
  font-size: 13px;
}

.toolbar-hint {
  color: var(--nurse-text-secondary);
  font-size: 12px;
}

.atd-module__alert {
  margin: 8px 12px 0;
  width: auto;
}

.bed-board-scroll {
  min-height: 220px;
  flex: 1;
  overflow: auto;
  padding: 12px;
}

.bed-ward {
  margin-bottom: 14px;
}

.bed-ward > h2 {
  margin: 0 0 8px;
  color: var(--nurse-text-primary);
  font-size: 16px;
  font-weight: 600;
}

.bed-house {
  margin-bottom: 10px;
  overflow: hidden;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
}

.bed-house > header {
  display: flex;
  min-height: 36px;
  align-items: center;
  justify-content: space-between;
  padding: 0 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-extra-light);
  color: var(--nurse-text-primary);
  font-size: 13px;
}

.bed-house > header strong {
  font-weight: 500;
}

.bed-house > header span {
  color: var(--nurse-text-regular);
}

.bed-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
  gap: 10px;
  padding: 10px;
}

.bed-drop-zone {
  min-width: 0;
  border-radius: var(--el-border-radius-base);
  transition: box-shadow 0.2s ease;
}

.bed-drop-zone.is-hover {
  box-shadow: 0 0 0 2px var(--el-color-primary-light-3);
}

.bed-card {
  position: relative;
  box-sizing: border-box;
  min-height: 104px;
  overflow: hidden;
  padding: 10px 10px 8px 14px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-bg-color);
  color: var(--nurse-text-primary);
}

.bed-card__stripe {
  position: absolute;
  inset: 0 auto 0 0;
  width: 4px;
  background: var(--el-color-success);
}

.bed-card--occupied {
  cursor: grab;
}

.bed-card--occupied:hover,
.bed-card--occupied.is-selected {
  border-color: var(--el-color-primary-light-5);
  background: var(--el-color-primary-light-9);
}

.bed-card__top {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
}

.bed-card__top strong {
  color: var(--nurse-text-primary);
  font-weight: 500;
}

.bed-card__name {
  margin-top: 10px;
  color: var(--nurse-text-primary);
  font-weight: 600;
}

.bed-card__meta {
  margin-top: 6px;
  color: var(--nurse-text-secondary);
  font-size: 12px;
}

.bed-card--empty {
  display: flex;
  align-items: center;
  justify-content: space-between;
  border-style: dashed;
  color: var(--nurse-text-regular);
}

.bed-card--empty .bed-card__stripe {
  background: var(--el-color-info-light-5);
}

.bed-card--empty.is-disabled {
  background: var(--el-fill-color-light);
  color: var(--el-text-color-placeholder);
}

@media (max-width: 1366px) {
  .bed-grid {
    grid-template-columns: repeat(auto-fill, minmax(136px, 1fr));
  }

  .toolbar-hint {
    display: none;
  }
}
</style>
