<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import dayjs from 'dayjs'
import { ElMessage } from 'element-plus'

import {
  queryNurseStationTemperatureSheet,
  saveNurseStationTemperatureSheet,
  type NurseStationTemperatureDailySupplementVO,
  type NurseStationTemperatureGridDayVO,
  type NurseStationTemperatureSheetHeaderVO,
  type NurseStationTemperatureSpecialEventCode,
  type NurseStationTemperatureTimeSlot
} from '@/apis/inpatient/nurse-station'

import type { NurseStationPatientSummary } from '../type'

defineOptions({ name: 'NurseStationTemperatureSheet' })

const props = defineProps<{
  /** 体温单只能查看一位患者，因此正常情况下数组中只有左侧勾选的一位患者。 */
  patients: NurseStationPatientSummary[]
}>()

interface TemperatureFillRow {
  timeSlot: NurseStationTemperatureTimeSlot
  vitalSignId?: string
  version?: number
  temperature: string
  pulse: string
  heartRate: string
  respiratory: string
  systolicPressure: string
  diastolicPressure: string
  specialEventCode?: NurseStationTemperatureSpecialEventCode
  eventTime: string
  cooledTemperature: string
}

const timeSlots: NurseStationTemperatureTimeSlot[] = ['02', '06', '10', '14', '18', '22']
const timedEventCodes: NurseStationTemperatureSpecialEventCode[] = [
  'ADMISSION',
  'TRANSFER',
  'DELIVERY',
  'DISCHARGE',
  'DEATH'
]
const chartMarkerEventCodes: NurseStationTemperatureSpecialEventCode[] = [
  'ADMISSION',
  'TRANSFER',
  'SURGERY',
  'DELIVERY',
  'DISCHARGE',
  'DEATH'
]
const specialEventOptions: Array<{
  label: string
  value: NurseStationTemperatureSpecialEventCode
}> = [
  { label: '转科', value: 'TRANSFER' },
  { label: '手术', value: 'SURGERY' },
  { label: '分娩', value: 'DELIVERY' },
  { label: '出院', value: 'DISCHARGE' },
  { label: '死亡', value: 'DEATH' },
  { label: '外出检查', value: 'OUT_FOR_EXAM' },
  { label: '拒测体温', value: 'REFUSE_TEMP' },
  { label: '物理降温', value: 'PHYSICAL_COOLING' },
  { label: '药物降温', value: 'MEDICATION_COOLING' },
  { label: '体温不升', value: 'TEMP_NOT_RISING' }
]

const selectedPatient = computed(() => (props.patients.length === 1 ? props.patients[0] : null))
const loading = ref(false)
const saving = ref(false)
const pageIndex = ref(0)
const totalPages = ref(1)
const paperHeader = ref<NurseStationTemperatureSheetHeaderVO | null>(null)
const gridDays = ref<NurseStationTemperatureGridDayVO[]>([])
const hospitalizationDays = ref<Record<string, number>>({})
const postoperativeDays = ref<Record<string, number>>({})
const drawerVisible = ref(false)
const fullscreen = ref(false)
const editDate = ref(dayjs().format('YYYY-MM-DD'))
const fillRows = ref<TemperatureFillRow[]>([])
const dailySupplement = ref<NurseStationTemperatureDailySupplementVO>({})
let requestSerial = 0

const chartWidth = 1060
const chartHeight = 340
const plotLeft = 64
const plotRight = 18
const plotTop = 32
const plotBottom = 38
const plotWidth = chartWidth - plotLeft - plotRight
const plotHeight = chartHeight - plotTop - plotBottom

/** 将七天数据摊平成 42 个时次，供曲线和表尾共用，避免两处各自计算后错位。 */
const flatSlots = computed(() =>
  gridDays.value.flatMap((day) =>
    day.slots.map((slot) => ({ recordDate: day.recordDate, ...slot }))
  )
)

const chartColumns = computed(() => Math.max(1, flatSlots.value.length))
const xForIndex = (index: number) => plotLeft + ((index + 0.5) * plotWidth) / chartColumns.value
const yForTemperature = (value: string) => plotTop + ((42 - Number(value)) / (42 - 34)) * plotHeight
const yForPulse = (value: string) => plotTop + ((180 - Number(value)) / (180 - 40)) * plotHeight

/**
 * 只连接相邻且都有值的测量点。若中间漏测，曲线在该处断开，防止页面误导护士认为
 * 两个间隔很远的数值属于连续监测结果。
 */
function buildSegments(field: 'heartRate' | 'pulse' | 'temperature') {
  return flatSlots.value.slice(0, -1).flatMap((slot, index) => {
    const next = flatSlots.value[index + 1]
    const first = slot[field]
    const second = next?.[field]
    if (!first || !second) return []
    const yMapper = field === 'temperature' ? yForTemperature : yForPulse
    return [
      {
        x1: xForIndex(index),
        y1: yMapper(first),
        x2: xForIndex(index + 1),
        y2: yMapper(second)
      }
    ]
  })
}

const temperatureSegments = computed(() => buildSegments('temperature'))
const pulseSegments = computed(() => buildSegments('pulse'))
const heartRateSegments = computed(() => buildSegments('heartRate'))
const temperaturePoints = computed(() =>
  flatSlots.value.flatMap((slot, index) =>
    slot.temperature
      ? [{ x: xForIndex(index), y: yForTemperature(slot.temperature), value: slot.temperature }]
      : []
  )
)
const pulsePoints = computed(() =>
  flatSlots.value.flatMap((slot, index) =>
    slot.pulse ? [{ x: xForIndex(index), y: yForPulse(slot.pulse), value: slot.pulse }] : []
  )
)
const heartRatePoints = computed(() =>
  flatSlots.value.flatMap((slot, index) =>
    slot.heartRate
      ? [{ x: xForIndex(index), y: yForPulse(slot.heartRate), value: slot.heartRate }]
      : []
  )
)

/** 降温后复测点向右偏移半小时，即相邻四小时时次横向距离的八分之一。 */
const coolingPoints = computed(() => {
  const offset = plotWidth / chartColumns.value / 8
  return flatSlots.value.flatMap((slot, index) =>
    slot.specialEvents.flatMap((event) =>
      slot.temperature && event.cooledTemperature
        ? [
            {
              key: `${slot.recordDate}-${slot.timeSlot}-${event.code}-cooling`,
              x1: xForIndex(index),
              y1: yForTemperature(slot.temperature),
              x2: xForIndex(index) + offset,
              y2: yForTemperature(event.cooledTemperature)
            }
          ]
        : []
    )
  )
})

const specialEventMarkers = computed(() =>
  flatSlots.value.flatMap((slot, index) =>
    slot.specialEvents
      .filter((event) => chartMarkerEventCodes.includes(event.code))
      .map((event, eventIndex) => ({
        key: `${slot.recordDate}-${slot.timeSlot}-${event.code}`,
        x: xForIndex(index),
        y: 13 + eventIndex * 14,
        text: event.eventTime ? `${event.displayText}${event.eventTime}` : event.displayText
      }))
  )
)

function createEmptyFillRow(timeSlot: NurseStationTemperatureTimeSlot): TemperatureFillRow {
  return {
    timeSlot,
    temperature: '',
    pulse: '',
    heartRate: '',
    respiratory: '',
    systolicPressure: '',
    diastolicPressure: '',
    eventTime: '',
    cooledTemperature: ''
  }
}

function createIdempotencyKey() {
  return `save-temperature-${Date.now()}-${Math.random().toString(36).slice(2, 10)}`
}

/** 快速切换患者或翻页时，只接受最后一次请求返回的数据，防止旧请求覆盖新患者。 */
async function loadSheet() {
  const serial = ++requestSerial
  const patient = selectedPatient.value
  paperHeader.value = null
  gridDays.value = []
  if (!patient) return

  loading.value = true
  try {
    const result = await queryNurseStationTemperatureSheet({
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      windowRange: '7d',
      pageIndex: pageIndex.value
    })
    if (serial !== requestSerial) return
    paperHeader.value = result.paperHeader
    gridDays.value = result.gridDays
    hospitalizationDays.value = result.hospitalizationDaysByDate
    postoperativeDays.value = result.postoperativeDaysByDate
    totalPages.value = result.pagination.totalPages
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '加载体温单失败')
  } finally {
    if (serial === requestSerial) loading.value = false
  }
}

watch(
  selectedPatient,
  () => {
    pageIndex.value = 0
    void loadSheet()
  },
  { immediate: true }
)

function changePage(direction: 'newer' | 'older') {
  const next = direction === 'older' ? pageIndex.value + 1 : pageIndex.value - 1
  if (next < 0 || next >= totalPages.value) return
  pageIndex.value = next
  void loadSheet()
}

/** 把选中日期已经保存的数据复制进录入表，编辑时即可携带记录 ID 和版本号。 */
function hydrateEditor(recordDate: string) {
  const day = gridDays.value.find((item) => item.recordDate === recordDate)
  fillRows.value = timeSlots.map((timeSlot) => {
    const slot = day?.slots.find((item) => item.timeSlot === timeSlot)
    if (!slot) return createEmptyFillRow(timeSlot)
    const event = slot.specialEvents[0]
    return {
      timeSlot,
      vitalSignId: slot.vitalSignId,
      version: slot.version,
      temperature: slot.temperature ?? '',
      pulse: slot.pulse ?? '',
      heartRate: slot.heartRate ?? '',
      respiratory: slot.respiratory ?? '',
      systolicPressure: slot.systolicPressure ?? '',
      diastolicPressure: slot.diastolicPressure ?? '',
      specialEventCode: event?.code,
      eventTime: event?.eventTime ?? '',
      cooledTemperature: event?.cooledTemperature ?? ''
    }
  })
  dailySupplement.value = { ...(day?.dailySupplement ?? {}) }
}

function openEditor(recordDate?: string) {
  editDate.value = recordDate ?? gridDays.value.at(-1)?.recordDate ?? dayjs().format('YYYY-MM-DD')
  hydrateEditor(editDate.value)
  drawerVisible.value = true
}

function changeEditDate(delta: number) {
  editDate.value = dayjs(editDate.value).add(delta, 'day').format('YYYY-MM-DD')
  hydrateEditor(editDate.value)
}

/** 复制上一时次的生命体征，减少连续测量时的重复输入；记录 ID 和版本绝不复制。 */
function copyPreviousRow(index: number) {
  if (index <= 0) return
  const previous = fillRows.value[index - 1]
  const current = fillRows.value[index]
  // 数组来自固定六时次，但仍做边界保护，避免未来调整时次后访问到不存在的行。
  if (!previous || !current) return
  current.temperature = previous.temperature
  current.pulse = previous.pulse
  current.heartRate = previous.heartRate
  current.respiratory = previous.respiratory
  current.systolicPressure = previous.systolicPressure
  current.diastolicPressure = previous.diastolicPressure
}

function rowHasContent(row: TemperatureFillRow) {
  return [
    row.temperature,
    row.pulse,
    row.heartRate,
    row.respiratory,
    row.systolicPressure,
    row.diastolicPressure,
    row.specialEventCode
  ].some(Boolean)
}

async function saveSheet() {
  const patient = selectedPatient.value
  if (!patient) return
  const rows = fillRows.value.filter(rowHasContent)
  const invalidTimedEvent = rows.find(
    (row) =>
      row.specialEventCode && timedEventCodes.includes(row.specialEventCode) && !row.eventTime
  )
  if (invalidTimedEvent) {
    ElMessage.warning(`${invalidTimedEvent.timeSlot} 时的特殊事件需要填写发生时间`)
    return
  }

  saving.value = true
  try {
    const result = await saveNurseStationTemperatureSheet({
      idempotencyKey: createIdempotencyKey(),
      encounterId: patient.encounterId,
      patientId: patient.patientId,
      recordDate: editDate.value,
      slots: rows.map((row) => ({
        timeSlot: row.timeSlot,
        vitalSignId: row.vitalSignId,
        expectedVersion: row.version,
        temperature: row.temperature || undefined,
        pulse: row.pulse || undefined,
        heartRate: row.heartRate || undefined,
        respiratory: row.respiratory || undefined,
        systolicPressure: row.systolicPressure || undefined,
        diastolicPressure: row.diastolicPressure || undefined,
        specialEvents: row.specialEventCode
          ? [
              {
                code: row.specialEventCode,
                displayText:
                  specialEventOptions.find((option) => option.value === row.specialEventCode)
                    ?.label ?? row.specialEventCode,
                eventTime: row.eventTime || undefined,
                cooledTemperature: row.cooledTemperature || undefined
              }
            ]
          : []
      })),
      dailySupplement: { ...dailySupplement.value }
    })
    if (result.savedCount) ElMessage.success(`已保存 ${result.savedCount} 个测量时次`)
    if (!result.success) {
      ElMessage.warning(result.results.find((item) => !item.success)?.message ?? '部分时次保存失败')
    }
    await loadSheet()
    hydrateEditor(editDate.value)
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '保存体温单失败')
  } finally {
    saving.value = false
  }
}
</script>

<template>
  <section class="temperature-sheet-module" :class="{ 'is-fullscreen': fullscreen }">
    <el-empty v-if="!selectedPatient" description="请先从左侧选择一位患者" />

    <div v-else v-loading="loading" class="temperature-sheet-layout">
      <aside class="sheet-actions" aria-label="体温单操作">
        <el-button :disabled="pageIndex >= totalPages - 1" @click="changePage('older')">
          上一页
        </el-button>
        <el-button :disabled="pageIndex === 0" @click="changePage('newer')">下一页</el-button>
        <el-button @click="fullscreen = !fullscreen">{{
          fullscreen ? '退出全屏' : '全屏'
        }}</el-button>
        <el-button type="primary" @click="openEditor()">填写体温</el-button>
      </aside>

      <div class="paper-scroll">
        <article class="temperature-paper" data-testid="temperature-sheet-paper">
          <header class="paper-heading">
            <div class="hospital-name">示范医院</div>
            <h2>体 温 单</h2>
            <div class="paper-number">第 {{ pageIndex + 1 }} 页</div>
          </header>

          <dl class="patient-header">
            <div>
              <dt>姓名</dt>
              <dd>{{ paperHeader?.patientName }}</dd>
            </div>
            <div>
              <dt>性别</dt>
              <dd>{{ paperHeader?.genderText || '—' }}</dd>
            </div>
            <div>
              <dt>年龄</dt>
              <dd>{{ paperHeader?.ageText || '—' }}</dd>
            </div>
            <div>
              <dt>科室</dt>
              <dd>{{ paperHeader?.wardName || '—' }}</dd>
            </div>
            <div>
              <dt>床号</dt>
              <dd>{{ paperHeader?.bedName || '—' }}</dd>
            </div>
            <div>
              <dt>住院号</dt>
              <dd>{{ paperHeader?.encounterBusNo || '—' }}</dd>
            </div>
          </dl>

          <div class="sheet-grid-head">
            <div class="head-label">日期</div>
            <button
              v-for="day in gridDays"
              :key="day.recordDate"
              type="button"
              class="day-cell day-cell--clickable"
              @click="openEditor(day.recordDate)"
            >
              {{ dayjs(day.recordDate).format('MM-DD') }}
            </button>
            <div class="head-label">住院日数</div>
            <div v-for="day in gridDays" :key="`in-${day.recordDate}`" class="day-cell">
              {{ hospitalizationDays[day.recordDate] }}
            </div>
            <div class="head-label">术后日数</div>
            <div v-for="day in gridDays" :key="`post-${day.recordDate}`" class="day-cell">
              {{ postoperativeDays[day.recordDate] || '' }}
            </div>
            <div class="head-label">时间</div>
            <div v-for="day in gridDays" :key="`times-${day.recordDate}`" class="time-group">
              <span v-for="timeSlot in timeSlots" :key="timeSlot">{{ timeSlot }}</span>
            </div>
          </div>

          <div class="chart-wrap" data-testid="temperature-chart">
            <svg
              class="temperature-chart"
              :viewBox="`0 0 ${chartWidth} ${chartHeight}`"
              role="img"
              aria-label="七天体温和脉搏曲线"
            >
              <defs>
                <pattern
                  id="small-grid"
                  width="23.285"
                  height="16.875"
                  patternUnits="userSpaceOnUse"
                >
                  <path
                    d="M 23.285 0 L 0 0 0 16.875"
                    fill="none"
                    stroke="#dbe4ee"
                    stroke-width="1"
                  />
                </pattern>
              </defs>
              <rect
                :x="plotLeft"
                :y="plotTop"
                :width="plotWidth"
                :height="plotHeight"
                fill="url(#small-grid)"
                stroke="#94a3b8"
              />
              <line
                v-for="dayIndex in 8"
                :key="`day-line-${dayIndex}`"
                :x1="plotLeft + ((dayIndex - 1) * plotWidth) / 7"
                :x2="plotLeft + ((dayIndex - 1) * plotWidth) / 7"
                :y1="plotTop"
                :y2="plotTop + plotHeight"
                stroke="#64748b"
                stroke-width="1.2"
              />
              <g v-for="degree in 9" :key="`degree-${degree}`">
                <text x="10" :y="plotTop + ((degree - 1) * plotHeight) / 8 + 5" class="axis-label">
                  {{ 43 - degree }}℃
                </text>
              </g>
              <text x="7" :y="chartHeight - 8" class="axis-note">蓝：体温</text>
              <text x="875" :y="chartHeight - 8" class="axis-note axis-note--pulse">
                红：脉搏/心率
              </text>

              <line
                v-for="(segment, index) in temperatureSegments"
                :key="`temperature-line-${index}`"
                v-bind="segment"
                class="temperature-line"
              />
              <circle
                v-for="(point, index) in temperaturePoints"
                :key="`temperature-point-${index}`"
                :cx="point.x"
                :cy="point.y"
                r="4"
                class="temperature-point"
              />
              <line
                v-for="(segment, index) in pulseSegments"
                :key="`pulse-line-${index}`"
                v-bind="segment"
                class="pulse-line"
              />
              <circle
                v-for="(point, index) in pulsePoints"
                :key="`pulse-point-${index}`"
                :cx="point.x"
                :cy="point.y"
                r="4"
                class="pulse-point"
              />
              <line
                v-for="(segment, index) in heartRateSegments"
                :key="`heart-rate-line-${index}`"
                v-bind="segment"
                class="heart-rate-line"
              />
              <circle
                v-for="(point, index) in heartRatePoints"
                :key="`heart-rate-point-${index}`"
                :cx="point.x"
                :cy="point.y"
                r="4"
                class="heart-rate-point"
              />
              <g v-for="point in coolingPoints" :key="point.key">
                <line
                  :x1="point.x1"
                  :y1="point.y1"
                  :x2="point.x2"
                  :y2="point.y2"
                  class="cooling-line"
                />
                <circle :cx="point.x2" :cy="point.y2" r="4" class="cooling-point" />
              </g>
              <text
                v-for="marker in specialEventMarkers"
                :key="marker.key"
                :x="marker.x"
                :y="marker.y"
                class="event-marker"
              >
                {{ marker.text }}
              </text>
            </svg>
          </div>

          <div class="paper-footer" data-testid="temperature-footer">
            <div class="footer-label">呼吸（次/分）</div>
            <div
              v-for="slot in flatSlots"
              :key="`resp-${slot.recordDate}-${slot.timeSlot}`"
              class="slot-cell"
            >
              {{ slot.respiratory }}
            </div>

            <div class="footer-label">血压（mmHg）</div>
            <div v-for="day in gridDays" :key="`bp-${day.recordDate}`" class="daily-value">
              <span
                v-for="slot in day.slots.filter((item) => item.systolicPressure)"
                :key="slot.timeSlot"
              >
                {{ slot.systolicPressure }}/{{ slot.diastolicPressure }}
              </span>
            </div>

            <template
              v-for="item in [
                ['大便（次）', 'stoolCount'],
                ['小便（次）', 'urineCount'],
                ['总入量（ml）', 'intake'],
                ['尿量（ml）', 'urine'],
                ['其他出量（ml）', 'other'],
                ['体重（kg）', 'weight'],
                ['身高（cm）', 'height']
              ]"
              :key="item[1]"
            >
              <div class="footer-label">{{ item[0] }}</div>
              <div
                v-for="day in gridDays"
                :key="`${item[1]}-${day.recordDate}`"
                class="daily-value"
              >
                {{ day.dailySupplement[item[1] as keyof NurseStationTemperatureDailySupplementVO] }}
              </div>
            </template>
          </div>
        </article>
      </div>
    </div>

    <el-drawer
      v-model="drawerVisible"
      direction="btt"
      size="72%"
      class="temperature-fill-drawer"
      :with-header="false"
    >
      <div class="drawer-heading">
        <div>
          <h3>体温单录入</h3>
          <p>{{ paperHeader?.patientName }} · {{ paperHeader?.bedName }}</p>
        </div>
        <div class="date-switcher">
          <el-button @click="changeEditDate(-1)">前一天</el-button>
          <el-date-picker
            v-model="editDate"
            type="date"
            value-format="YYYY-MM-DD"
            format="YYYY-MM-DD"
            aria-label="体温单记录日期"
            @change="hydrateEditor(editDate)"
          />
          <el-button @click="changeEditDate(1)">后一天</el-button>
        </div>
        <div>
          <el-button @click="drawerVisible = false">取消</el-button>
          <el-button type="primary" :loading="saving" @click="saveSheet">保存</el-button>
        </div>
      </div>

      <div class="fill-table-scroll">
        <table class="fill-table">
          <thead>
            <tr>
              <th>时次</th>
              <th>体温℃</th>
              <th>脉搏</th>
              <th>心率</th>
              <th>呼吸</th>
              <th>收缩压</th>
              <th>舒张压</th>
              <th>特殊事件</th>
              <th>事件时间</th>
              <th>降温后℃</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(row, index) in fillRows" :key="row.timeSlot">
              <th>{{ row.timeSlot }} 时</th>
              <td><el-input v-model="row.temperature" placeholder="36.5" /></td>
              <td><el-input v-model="row.pulse" /></td>
              <td><el-input v-model="row.heartRate" /></td>
              <td><el-input v-model="row.respiratory" /></td>
              <td><el-input v-model="row.systolicPressure" /></td>
              <td><el-input v-model="row.diastolicPressure" /></td>
              <td>
                <el-select v-model="row.specialEventCode" clearable placeholder="无">
                  <el-option
                    v-for="option in specialEventOptions"
                    :key="option.value"
                    :label="option.label"
                    :value="option.value"
                  />
                </el-select>
              </td>
              <td>
                <el-time-picker
                  v-model="row.eventTime"
                  value-format="HH:mm"
                  format="HH:mm"
                  placeholder="HH:mm"
                  :disabled="
                    !row.specialEventCode || !timedEventCodes.includes(row.specialEventCode)
                  "
                />
              </td>
              <td>
                <el-input
                  v-model="row.cooledTemperature"
                  :disabled="
                    !row.specialEventCode ||
                    !['PHYSICAL_COOLING', 'MEDICATION_COOLING'].includes(row.specialEventCode)
                  "
                />
              </td>
              <td>
                <el-button
                  link
                  type="primary"
                  :disabled="index === 0"
                  @click="copyPreviousRow(index)"
                >
                  复制上次
                </el-button>
              </td>
            </tr>
          </tbody>
        </table>

        <section class="daily-supplement">
          <h4>当日补充项目</h4>
          <div class="supplement-grid">
            <label>大便（次）<el-input v-model="dailySupplement.stoolCount" /></label>
            <label>小便（次）<el-input v-model="dailySupplement.urineCount" /></label>
            <label>总入量（ml）<el-input v-model="dailySupplement.intake" /></label>
            <label>尿量（ml）<el-input v-model="dailySupplement.urine" /></label>
            <label>其他出量（ml）<el-input v-model="dailySupplement.other" /></label>
            <label>体重（kg）<el-input v-model="dailySupplement.weight" /></label>
            <label>身高（cm）<el-input v-model="dailySupplement.height" /></label>
            <label>腹围（cm）<el-input v-model="dailySupplement.abdominalCircumference" /></label>
          </div>
        </section>
      </div>
    </el-drawer>
  </section>
</template>

<style scoped>
.temperature-sheet-module {
  display: flex;
  height: 100%;
  min-width: 0;
  min-height: 0;
  flex-direction: column;
  overflow: hidden;
  background: #eef1f5;
  color: #303133;
  -webkit-text-fill-color: currentColor;
}

.temperature-sheet-module.is-fullscreen {
  position: fixed;
  z-index: 2000;
  inset: 0;
  padding: 12px;
}

.temperature-sheet-layout {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
}

.sheet-actions {
  display: flex;
  width: 92px;
  flex: 0 0 92px;
  flex-direction: column;
  gap: 10px;
  padding: 16px 10px;
  border-right: 1px solid #dcdfe6;
  background: #fff;
}

.sheet-actions :deep(.el-button) {
  width: 100%;
  margin: 0;
}

.paper-scroll {
  min-width: 0;
  min-height: 0;
  flex: 1;
  padding: 14px;
  overflow: auto;
}

.temperature-paper {
  box-sizing: border-box;
  min-width: 1100px;
  max-width: 1380px;
  margin: 0 auto;
  padding: 18px 22px 28px;
  border: 1px solid #cbd5e1;
  background: #fff;
  box-shadow: 0 2px 10px rgb(15 23 42 / 8%);
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-family: 'SimSun', 'Songti SC', serif;
}

.paper-heading {
  position: relative;
  display: grid;
  grid-template-columns: 1fr auto 1fr;
  align-items: end;
  margin-bottom: 12px;
  text-align: center;
}

.paper-heading h2 {
  margin: 0;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 28px;
  font-weight: 700;
  letter-spacing: 12px;
}

.hospital-name,
.paper-number {
  padding-bottom: 4px;
  color: #374151;
  -webkit-text-fill-color: #374151;
  font-size: 14px;
}

.hospital-name {
  text-align: left;
}
.paper-number {
  text-align: right;
}

.patient-header {
  display: grid;
  grid-template-columns: repeat(6, minmax(0, 1fr));
  margin: 0 0 10px;
  border-top: 1px solid #64748b;
  border-left: 1px solid #64748b;
}

.patient-header > div {
  display: flex;
  min-width: 0;
  border-right: 1px solid #64748b;
  border-bottom: 1px solid #64748b;
}

.patient-header dt,
.patient-header dd {
  margin: 0;
  padding: 7px 6px;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 13px;
}

.patient-header dt {
  flex: 0 0 auto;
  background: #f8fafc;
}
.patient-header dd {
  min-width: 0;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.sheet-grid-head {
  display: grid;
  grid-template-columns: 64px repeat(7, minmax(0, 1fr));
  border-top: 1px solid #64748b;
  border-left: 1px solid #64748b;
}

.head-label,
.day-cell,
.time-group {
  min-height: 28px;
  border-right: 1px solid #64748b;
  border-bottom: 1px solid #64748b;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 12px;
  line-height: 28px;
  text-align: center;
}

.head-label {
  background: #f8fafc;
  font-weight: 600;
}

.day-cell--clickable {
  padding: 0;
  cursor: pointer;
  background: #fff;
  font: inherit;
}

.day-cell--clickable:hover {
  background: #ecf5ff;
  color: #1d4ed8;
}

.time-group {
  display: grid;
  grid-template-columns: repeat(6, 1fr);
}

.time-group span:not(:last-child) {
  border-right: 1px solid #dbe4ee;
}

.chart-wrap {
  overflow: hidden;
  border-right: 1px solid #64748b;
  border-bottom: 1px solid #64748b;
  border-left: 1px solid #64748b;
}

.temperature-chart {
  display: block;
  width: 100%;
  height: 340px;
  background: #fff;
}
.axis-label,
.axis-note {
  fill: #334155;
  font-family: Arial, sans-serif;
  font-size: 11px;
}
.axis-note--pulse {
  fill: #dc2626;
}
.temperature-line {
  stroke: #2563eb;
  stroke-width: 2;
}
.temperature-point {
  fill: #fff;
  stroke: #2563eb;
  stroke-width: 2;
}
.pulse-line {
  stroke: #dc2626;
  stroke-width: 2;
}
.pulse-point {
  fill: #dc2626;
  stroke: #dc2626;
}
.heart-rate-line {
  stroke: #dc2626;
  stroke-dasharray: 5 3;
  stroke-width: 1.6;
}
.heart-rate-point {
  fill: #fff;
  stroke: #dc2626;
  stroke-width: 2;
}
.cooling-line {
  stroke: #dc2626;
  stroke-dasharray: 3 2;
  stroke-width: 1.5;
}
.cooling-point {
  fill: #fff;
  stroke: #dc2626;
  stroke-width: 1.5;
}
.event-marker {
  fill: #dc2626;
  font-family: 'Microsoft YaHei', sans-serif;
  font-size: 10px;
  text-anchor: middle;
}

.paper-footer {
  display: grid;
  grid-template-columns: 64px repeat(42, minmax(0, 1fr));
  border-left: 1px solid #64748b;
}

.footer-label,
.slot-cell,
.daily-value {
  min-height: 30px;
  border-right: 1px solid #64748b;
  border-bottom: 1px solid #64748b;
  color: #111827;
  -webkit-text-fill-color: #111827;
  font-size: 11px;
  text-align: center;
}

.footer-label {
  grid-column: span 1;
  display: flex;
  align-items: center;
  justify-content: center;
  background: #f8fafc;
  font-weight: 600;
}

.slot-cell {
  line-height: 30px;
}

.daily-value {
  display: flex;
  grid-column: span 6;
  align-items: center;
  justify-content: center;
  gap: 6px;
}

.drawer-heading {
  display: grid;
  grid-template-columns: 1fr auto 1fr;
  align-items: center;
  gap: 16px;
  padding: 0 0 14px;
  border-bottom: 1px solid #dcdfe6;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.drawer-heading h3 {
  margin: 0 0 4px;
  font-size: 18px;
}
.drawer-heading p {
  margin: 0;
  color: #606266;
  -webkit-text-fill-color: #606266;
}
.drawer-heading > :last-child {
  text-align: right;
}
.date-switcher {
  display: flex;
  align-items: center;
  gap: 8px;
}
.fill-table-scroll {
  height: calc(100% - 64px);
  overflow: auto;
}

.fill-table {
  width: 100%;
  min-width: 1320px;
  margin-top: 14px;
  border-collapse: collapse;
  table-layout: fixed;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.fill-table th,
.fill-table td {
  padding: 7px 5px;
  border: 1px solid #dcdfe6;
  text-align: center;
}
.fill-table th {
  background: #f5f7fa;
  font-weight: 600;
}
.fill-table th:first-child {
  width: 58px;
}
.fill-table th:nth-child(8) {
  width: 130px;
}
.fill-table th:nth-child(9) {
  width: 132px;
}
.fill-table th:last-child {
  width: 82px;
}
.fill-table :deep(.el-input__inner) {
  color: #303133;
  -webkit-text-fill-color: #303133;
  text-align: center;
}

.daily-supplement {
  margin-top: 16px;
  padding: 14px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  background: #fafafa;
  color: #303133;
  -webkit-text-fill-color: #303133;
}

.daily-supplement h4 {
  margin: 0 0 12px;
}
.supplement-grid {
  display: grid;
  grid-template-columns: repeat(4, minmax(180px, 1fr));
  gap: 12px;
}
.supplement-grid label {
  display: grid;
  grid-template-columns: 92px minmax(0, 1fr);
  align-items: center;
  gap: 6px;
  font-size: 13px;
}

.temperature-sheet-module > :deep(.el-empty) {
  min-height: 0;
  flex: 1;
}

@media (max-width: 1366px) {
  .sheet-actions {
    width: 78px;
    flex-basis: 78px;
    padding-inline: 7px;
  }
  .paper-scroll {
    padding: 10px;
  }
  .temperature-paper {
    padding-inline: 16px;
  }
  .supplement-grid {
    grid-template-columns: repeat(3, minmax(180px, 1fr));
  }
}
</style>
