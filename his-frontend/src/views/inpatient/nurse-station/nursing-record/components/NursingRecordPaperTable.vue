<script setup lang="ts">
import { ref } from 'vue'
import { Delete } from '@element-plus/icons-vue'

import type { NurseStationNursingRecordPaperHeaderVO } from '@/apis/inpatient/nurse-station'

import type { NursingRecordViewRow } from '../model'

defineOptions({ name: 'NursingRecordPaperTable' })

const props = defineProps<{
  /** 纸质单顶部患者信息。 */
  paperHeader: NurseStationNursingRecordPaperHeaderVO | null
  /** 包含已保存记录和前端空白骨架的全部行。 */
  rows: NursingRecordViewRow[]
  /** 保存按钮是否正在等待 Mock 或真实后端。 */
  saving: boolean
  /** 当前尚未保存的行数。 */
  unsavedCount: number
  /** 当前周日期范围说明。 */
  weekRangeLabel: string
}>()

const emit = defineEmits<{
  addRow: []
  deleteRow: [row: NursingRecordViewRow]
  ensureDatetime: [clientRowId: string]
  saveAll: []
  updateField: [clientRowId: string, field: keyof NursingRecordViewRow, value: string]
}>()

const isFullscreen = ref(false)

/** 完整日期在纸质单窄列中只显示 MM-DD。 */
function formatDateShort(date: string) {
  const match = date.match(/^\d{4}-(\d{2})-(\d{2})$/)
  return match ? `${match[1]}-${match[2]}` : date
}

/**
 * 把 MM-DD 输入还原成完整日期。
 * 已有行沿用原年份，新行使用当前年份；真正的日期合法性由保存前和后端再次校验。
 */
function updateDate(row: NursingRecordViewRow, value: string) {
  const match = value.match(/^(\d{1,2})-(\d{1,2})$/)
  if (!match) {
    emit('updateField', row.clientRowId, 'recordDate', value)
    return
  }
  const year = row.recordDate.slice(0, 4) || String(new Date().getFullYear())
  emit(
    'updateField',
    row.clientRowId,
    'recordDate',
    `${year}-${match[1]?.padStart(2, '0')}-${match[2]?.padStart(2, '0')}`
  )
}

/** 从原生 input 事件安全读取字符串，避免在模板中重复类型断言。 */
function inputValue(event: Event) {
  return (event.target as HTMLInputElement | HTMLTextAreaElement).value
}
</script>

<template>
  <teleport to="body" :disabled="!isFullscreen">
    <section class="paper-record" :class="{ 'is-fullscreen': isFullscreen }">
      <header class="paper-record__toolbar">
        <div class="paper-record__toolbar-main">
          <span>{{ props.weekRangeLabel }}</span>
          <span v-if="props.unsavedCount" class="paper-record__unsaved">
            未保存 {{ props.unsavedCount }} 条
          </span>
        </div>
        <div class="paper-record__toolbar-actions">
          <el-button size="small" plain @click="isFullscreen = !isFullscreen">
            {{ isFullscreen ? '退出全屏' : '全屏' }}
          </el-button>
          <el-button size="small" type="primary" plain @click="emit('addRow')"> 新增行 </el-button>
          <el-button size="small" type="primary" :loading="props.saving" @click="emit('saveAll')">
            保存
          </el-button>
        </div>
      </header>

      <div class="paper-record__viewport">
        <div class="paper-record__sheet">
          <header class="paper-record__doc-head">
            <h2>护 理 记 录 单</h2>
            <div v-if="props.paperHeader" class="patient-info-line">
              <span>姓名：{{ props.paperHeader.patientName }}</span>
              <span>性别：{{ props.paperHeader.genderText || '' }}</span>
              <span>年龄：{{ props.paperHeader.ageText || '' }}</span>
              <span>病室：{{ props.paperHeader.wardName || '' }}</span>
              <span>床号：{{ props.paperHeader.bedName || '' }}</span>
              <span>病历号：{{ props.paperHeader.encounterBusNo || '' }}</span>
              <span>入院日期：{{ props.paperHeader.admissionDate || '' }}</span>
              <span>出院日期：{{ props.paperHeader.dischargeDate || '' }}</span>
            </div>
          </header>

          <table class="paper-record__table">
            <colgroup>
              <col class="col-datetime" />
              <col class="col-vital" span="5" />
              <col class="col-consciousness" />
              <col class="col-pupil" span="2" />
              <col class="col-io-name" />
              <col class="col-io-amount" />
              <col class="col-io-name" />
              <col class="col-io-amount" />
              <col class="col-io-color" />
              <col class="col-observation" />
              <col class="col-signature" />
            </colgroup>
            <thead>
              <tr>
                <th rowspan="2">日期<br />时间</th>
                <th>体温</th>
                <th>脉搏</th>
                <th>心率</th>
                <th>呼吸</th>
                <th>血压</th>
                <th rowspan="2">意识</th>
                <th colspan="2">瞳孔</th>
                <th colspan="2">入量</th>
                <th colspan="3">出量</th>
                <th rowspan="2">病情观察及护理</th>
                <th rowspan="2">护士签名</th>
              </tr>
              <tr>
                <th>℃</th>
                <th>次/分</th>
                <th>次/分</th>
                <th>次/分</th>
                <th>mmHg</th>
                <th>左</th>
                <th>右</th>
                <th>名称</th>
                <th>ml</th>
                <th>名称</th>
                <th>ml</th>
                <th>颜色</th>
              </tr>
            </thead>
            <tbody>
              <tr
                v-for="row in props.rows"
                :key="row.clientRowId"
                class="paper-record__row"
                :class="{ 'is-dirty': row.isDirty }"
              >
                <td class="col-datetime">
                  <input
                    class="paper-field paper-field--date"
                    type="text"
                    :value="formatDateShort(row.recordDate)"
                    @input="updateDate(row, inputValue($event))"
                  />
                  <input
                    class="paper-field paper-field--time"
                    type="text"
                    :value="row.recordTime"
                    @focus="emit('ensureDatetime', row.clientRowId)"
                    @input="emit('updateField', row.clientRowId, 'recordTime', inputValue($event))"
                  />
                  <button
                    v-if="row.rowType === 'extra'"
                    type="button"
                    class="paper-row-delete"
                    title="删除此行"
                    @click="emit('deleteRow', row)"
                  >
                    <el-icon :size="11"><Delete /></el-icon>
                  </button>
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.temperature"
                    @input="emit('updateField', row.clientRowId, 'temperature', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.pulse"
                    @input="emit('updateField', row.clientRowId, 'pulse', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.heartRate"
                    @input="emit('updateField', row.clientRowId, 'heartRate', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.respiratory"
                    @input="emit('updateField', row.clientRowId, 'respiratory', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.bloodPressure"
                    @input="
                      emit('updateField', row.clientRowId, 'bloodPressure', inputValue($event))
                    "
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.consciousness"
                    @input="
                      emit('updateField', row.clientRowId, 'consciousness', inputValue($event))
                    "
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.pupilLeft"
                    @input="emit('updateField', row.clientRowId, 'pupilLeft', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.pupilRight"
                    @input="emit('updateField', row.clientRowId, 'pupilRight', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field paper-field--left"
                    :value="row.intakeName"
                    @input="emit('updateField', row.clientRowId, 'intakeName', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.intakeAmount"
                    @input="
                      emit('updateField', row.clientRowId, 'intakeAmount', inputValue($event))
                    "
                  />
                </td>
                <td>
                  <input
                    class="paper-field paper-field--left"
                    :value="row.outputName"
                    @input="emit('updateField', row.clientRowId, 'outputName', inputValue($event))"
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.outputAmount"
                    @input="
                      emit('updateField', row.clientRowId, 'outputAmount', inputValue($event))
                    "
                  />
                </td>
                <td>
                  <input
                    class="paper-field paper-field--left"
                    :value="row.outputColorCharacter"
                    @input="
                      emit(
                        'updateField',
                        row.clientRowId,
                        'outputColorCharacter',
                        inputValue($event)
                      )
                    "
                  />
                </td>
                <td class="col-observation">
                  <textarea
                    class="paper-field paper-field--textarea"
                    rows="1"
                    :value="row.conditionMeasure"
                    @input="
                      emit('updateField', row.clientRowId, 'conditionMeasure', inputValue($event))
                    "
                  />
                </td>
                <td>
                  <input
                    class="paper-field"
                    :value="row.nurseSignature"
                    @input="
                      emit('updateField', row.clientRowId, 'nurseSignature', inputValue($event))
                    "
                  />
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </section>
  </teleport>
</template>

<style scoped>
.paper-record {
  display: flex;
  min-width: 0;
  min-height: 0;
  flex: 1;
  flex-direction: column;
  color: var(--nurse-text-primary, #303133);
}

.paper-record.is-fullscreen {
  position: fixed;
  inset: 0;
  z-index: 3000;
  width: 100vw;
  height: 100vh;
  background: #eef1f5;
}

.paper-record__toolbar {
  display: flex;
  min-height: 46px;
  flex: 0 0 auto;
  align-items: center;
  justify-content: space-between;
  gap: 8px;
  padding: 6px 10px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-bg-color);
}

.paper-record__toolbar-main,
.paper-record__toolbar-actions {
  display: flex;
  align-items: center;
  gap: 8px;
}

.paper-record__toolbar-main {
  color: var(--nurse-text-primary, #303133);
  font-size: 13px;
}

.paper-record__unsaved {
  color: #b45309;
  font-weight: 600;
}

.paper-record__viewport {
  min-width: 0;
  min-height: 0;
  flex: 1;
  overflow: auto;
  background: #fff;
}

.paper-record__sheet {
  box-sizing: border-box;
  min-width: 1080px;
  min-height: 100%;
  padding: 12px;
  background: #fff;
  color: #111827;
}

.paper-record__doc-head {
  margin-bottom: 6px;
  color: #111827;
}

.paper-record__doc-head h2 {
  margin: 0 0 7px;
  color: #111827;
  font-size: 27px;
  font-weight: 700;
  letter-spacing: 12px;
  line-height: 1.2;
  text-align: center;
}

.patient-info-line {
  display: flex;
  min-height: 24px;
  flex-wrap: wrap;
  align-items: center;
  gap: 6px 14px;
  color: #1f2937;
  font-size: 13px;
}

.paper-record__table {
  width: 100%;
  table-layout: fixed;
  border: 1px solid #374151;
  border-collapse: collapse;
  color: #111827;
}

.paper-record__table th,
.paper-record__table td {
  padding: 0;
  border: 1px solid #374151;
  color: #111827;
  text-align: center;
  vertical-align: middle;
}

.paper-record__table th {
  min-height: 33px;
  padding: 5px 3px;
  background: #f8fafc;
  font-size: 13px;
  font-weight: 500;
  line-height: 1.25;
}

.paper-record__table td {
  position: relative;
  box-sizing: border-box;
  height: 33px;
  min-height: 33px;
  background: #fff;
}

.paper-record__row.is-dirty td {
  background: #fff9e8;
}

.col-datetime {
  width: 66px;
}

.col-vital {
  width: 55px;
}

.col-consciousness {
  width: 55px;
}

.col-pupil {
  width: 45px;
}

.col-io-name {
  width: 70px;
}

.col-io-amount {
  width: 48px;
}

.col-io-color {
  width: 70px;
}

.col-observation {
  width: 245px;
}

.col-signature {
  width: 80px;
}

.paper-field {
  box-sizing: border-box;
  display: block;
  width: 100%;
  height: 30px;
  margin: 0;
  padding: 0 3px;
  border: 0;
  outline: 0;
  background: transparent;
  color: #111827;
  font: inherit;
  font-size: 13px;
  line-height: 30px;
  text-align: center;
  -webkit-text-fill-color: #111827;
}

.paper-field:focus {
  background: #eff6ff;
  box-shadow: inset 0 0 0 1px var(--el-color-primary);
}

.paper-field--date,
.paper-field--time {
  height: 15px;
  padding: 0 12px 0 2px;
  line-height: 15px;
}

.paper-field--left {
  overflow: hidden;
  text-align: left;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.paper-field--textarea {
  height: 31px;
  min-height: 31px;
  padding: 4px;
  overflow: hidden;
  line-height: 1.2;
  resize: none;
  text-align: left;
}

.paper-row-delete {
  position: absolute;
  top: 1px;
  right: 1px;
  display: flex;
  width: 13px;
  height: 13px;
  align-items: center;
  justify-content: center;
  padding: 0;
  cursor: pointer;
  border: 0;
  background: transparent;
  color: #b91c1c;
}

@media print {
  .paper-record__toolbar,
  .paper-row-delete {
    display: none;
  }

  .paper-record__viewport {
    overflow: visible;
  }

  .paper-record__sheet {
    min-width: 0;
    padding: 0;
  }
}
</style>
