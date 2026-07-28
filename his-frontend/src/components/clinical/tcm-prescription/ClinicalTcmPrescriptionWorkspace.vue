<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { Delete, Plus, Refresh, Select } from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'

import {
  cancelStopTcmPrescription,
  deleteTcmPrescription,
  listTcmPrescriptionHistory,
  listTcmPrescriptions,
  pageTcmHerbCatalog,
  saveTcmPrescriptionDraft,
  signTcmPrescription,
  stopTcmPrescription,
  withdrawTcmPrescription,
  type SaveTcmPrescriptionDTO,
  type TcmHerbCatalogItemVO,
  type TcmPrescriptionActionDTO,
  type TcmPrescriptionHistoryVO,
  type TcmPrescriptionVO
} from '@/apis/clinical/tcm-prescription'
import EmptyState from '@/components/common/EmptyState.vue'
import TcmHerbSelectorDialog from './TcmHerbSelectorDialog.vue'
import type {
  ClinicalTcmPrescriptionContext,
  ClinicalTcmPrescriptionDraft,
  ClinicalTcmPrescriptionLine,
  ClinicalTcmPrescriptionWorkspaceExpose,
  TcmHerbCatalogLoader
} from './type'

defineOptions({ name: 'ClinicalTcmPrescriptionWorkspace' })

const props = withDefaults(
  defineProps<{
    context?: ClinicalTcmPrescriptionContext | null
    patientSelected?: boolean
    accepted?: boolean
    readonly?: boolean
  }>(),
  {
    context: null,
    patientSelected: false,
    accepted: false,
    readonly: false
  }
)

const emit = defineEmits<{
  dirtyChange: [dirty: boolean]
  loadingChange: [loading: boolean]
}>()

const prescriptions = ref<ClinicalTcmPrescriptionDraft[]>([])
const historyRows = ref<TcmPrescriptionHistoryVO[]>([])
const activeKey = ref('')
const savedSnapshot = ref('[]')
const loading = ref(false)
const mutationLoading = ref(false)
const error = ref('')
const selectorVisible = ref(false)
let latestRequestId = 0

const activePrescription = computed(
  () => prescriptions.value.find((item) => item.key === activeKey.value) ?? null
)
const supportsTherapyType = computed(() => props.context?.scope === 'inpatient')
const editable = computed(
  () =>
    Boolean(props.context) &&
    props.accepted &&
    !props.readonly &&
    activePrescription.value?.status === 'DRAFT'
)
const existingCatalogIds = computed(
  () => activePrescription.value?.lines.map((line) => line.catalogId) ?? []
)
const dirty = computed(
  () =>
    Boolean(props.context) &&
    !loading.value &&
    serializePrescriptions(prescriptions.value) !== savedSnapshot.value
)
const totalAmount = computed(() => {
  const prescription = activePrescription.value
  if (!prescription) return 0
  return prescription.lines.reduce(
    (total, line) => total + line.dose * line.retailPrice * prescription.doseCount,
    0
  )
})

const methodOptions = ['水煎服', '冲服', '泡服', '煎汤外洗']
const frequencyOptions = ['每日一剂', '每日两次', '隔日一剂', '必要时']
const decoctionOptions = ['自煎', '代煎', '免煎颗粒']
const specialDecoctionOptions = ['', '先煎', '后下', '包煎', '另煎', '烊化', '冲服']

function serializePrescriptions(items: ClinicalTcmPrescriptionDraft[]) {
  return JSON.stringify(
    items.map((item) => ({
      id: item.id,
      therapyType: item.therapyType,
      syndromeName: item.syndromeName,
      doseCount: item.doseCount,
      method: item.method,
      frequency: item.frequency,
      days: item.days,
      decoctionMethod: item.decoctionMethod,
      remark: item.remark,
      status: item.status,
      lines: item.lines.map((line) => ({
        id: line.id,
        catalogId: line.catalogId,
        dose: line.dose,
        doseUnit: line.doseUnit,
        decoctionMethod: line.decoctionMethod,
        orderNo: line.orderNo
      }))
    }))
  )
}

function toDraft(record: TcmPrescriptionVO): ClinicalTcmPrescriptionDraft {
  return {
    key: record.id,
    id: record.id,
    prescriptionNo: record.prescriptionNo,
    therapyType: record.therapyType,
    syndromeName: record.syndromeName,
    doseCount: record.doseCount,
    method: record.method,
    frequency: record.frequency,
    days: record.days,
    decoctionMethod: record.decoctionMethod,
    remark: record.remark,
    status: record.status,
    statusName: record.statusName,
    lines: record.lines
      .slice()
      .sort((left, right) => left.orderNo - right.orderNo)
      .map((line) => ({ ...line, key: line.id })),
    updatedAt: record.updatedAt,
    version: record.version
  }
}

function createDraft(): ClinicalTcmPrescriptionDraft {
  const key = `draft-tcm-${Date.now()}-${prescriptions.value.length}`
  return {
    key,
    therapyType: supportsTherapyType.value ? 'LONG_TERM' : undefined,
    syndromeName: '',
    doseCount: 7,
    method: '水煎服',
    frequency: '每日一剂',
    days: 7,
    decoctionMethod: '自煎',
    status: 'DRAFT',
    statusName: '待签发',
    lines: [],
    version: 0
  }
}

function reset() {
  latestRequestId += 1
  prescriptions.value = []
  historyRows.value = []
  activeKey.value = ''
  savedSnapshot.value = '[]'
  loading.value = false
  mutationLoading.value = false
  error.value = ''
}

async function reload() {
  const context = props.context
  if (!context) {
    reset()
    return
  }
  const requestId = ++latestRequestId
  loading.value = true
  error.value = ''

  try {
    const [current, history] = await Promise.all([
      listTcmPrescriptions(context),
      listTcmPrescriptionHistory({
        patientId: context.patientId,
        currentEncounterId: context.encounterId
      })
    ])
    if (requestId !== latestRequestId) return
    const mapped = current.map(toDraft)
    prescriptions.value = mapped
    historyRows.value = history
    if (!mapped.some((item) => item.key === activeKey.value)) {
      activeKey.value = mapped[0]?.key ?? ''
    }
    savedSnapshot.value = serializePrescriptions(mapped)
  } catch (caught) {
    if (requestId !== latestRequestId) return
    error.value = caught instanceof Error ? caught.message : '中医处方加载失败'
  } finally {
    if (requestId === latestRequestId) loading.value = false
  }
}

const loadHerbCatalog: TcmHerbCatalogLoader = async (query, context) => {
  const stationContext = props.context
  if (!stationContext || context.signal.aborted) return { rows: [], total: 0 }
  const result = await pageTcmHerbCatalog({
    scope: stationContext.scope,
    pageIndex: query.pageIndex,
    pageSize: query.pageSize,
    keyword: query.keyword || undefined
  })
  if (context.signal.aborted) return { rows: [], total: 0 }
  return { rows: result.rows ?? [], total: result.total }
}

function addPrescription() {
  if (!props.accepted || props.readonly) return
  const draft = createDraft()
  prescriptions.value = [...prescriptions.value, draft]
  activeKey.value = draft.key
}

function addHerbs(items: TcmHerbCatalogItemVO[]) {
  const prescription = activePrescription.value
  if (!prescription || !editable.value) return
  const existing = new Set(prescription.lines.map((line) => line.catalogId))
  const additions = items
    .filter((item) => !existing.has(item.id))
    .map<ClinicalTcmPrescriptionLine>((item, index) => ({
      key: `draft-herb-${prescription.key}-${item.id}`,
      catalogId: item.id,
      code: item.code,
      name: item.name,
      specification: item.specification,
      dose: item.defaultDose,
      doseUnit: item.doseUnit,
      decoctionMethod: item.defaultDecoctionMethod,
      retailPrice: item.retailPrice,
      orderNo: prescription.lines.length + index + 1
    }))
  prescription.lines.push(...additions)
}

function removeHerb(key: string) {
  const prescription = activePrescription.value
  if (!prescription || !editable.value) return
  prescription.lines = prescription.lines
    .filter((line) => line.key !== key)
    .map((line, index) => ({ ...line, orderNo: index + 1 }))
}

function validate(prescription: ClinicalTcmPrescriptionDraft) {
  if (!prescription.syndromeName.trim()) return '请填写中医病证'
  if (supportsTherapyType.value && !prescription.therapyType) return '请选择长期或临时处方'
  if (prescription.doseCount <= 0) return '付数必须大于 0'
  if (!prescription.method) return '请选择用法'
  if (!prescription.frequency) return '请选择频次'
  if (prescription.days <= 0) return '疗程天数必须大于 0'
  if (prescription.lines.length === 0) return '请至少添加一味中药饮片'
  const invalidLine = prescription.lines.find((line) => line.dose <= 0)
  if (invalidLine) return `${invalidLine.name}的单剂用量必须大于 0`
  return ''
}

function changedDrafts() {
  const currentSnapshot = JSON.parse(savedSnapshot.value) as Array<Record<string, unknown>>
  const savedById = new Map(currentSnapshot.map((item) => [item.id, item]))
  return prescriptions.value.filter((prescription) => {
    if (prescription.status !== 'DRAFT') return false
    if (!prescription.id) return true
    const current = JSON.parse(serializePrescriptions([prescription]))[0] as Record<string, unknown>
    return JSON.stringify(current) !== JSON.stringify(savedById.get(prescription.id))
  })
}

function validateDirtyPrescriptions() {
  for (const prescription of changedDrafts()) {
    const validationError = validate(prescription)
    if (validationError) {
      activeKey.value = prescription.key
      return validationError
    }
  }
  return ''
}

function toSaveDTO(
  prescription: ClinicalTcmPrescriptionDraft,
  context: ClinicalTcmPrescriptionContext
): SaveTcmPrescriptionDTO {
  return {
    scope: context.scope,
    encounterId: context.encounterId,
    patientId: context.patientId,
    id: prescription.id,
    therapyType: context.scope === 'inpatient' ? prescription.therapyType : undefined,
    syndromeName: prescription.syndromeName.trim(),
    doseCount: prescription.doseCount,
    method: prescription.method,
    frequency: prescription.frequency,
    days: prescription.days,
    decoctionMethod: prescription.decoctionMethod,
    remark: prescription.remark?.trim() || undefined,
    lines: prescription.lines.map((line, index) => ({
      id: line.id,
      catalogId: line.catalogId,
      code: line.code,
      name: line.name,
      specification: line.specification,
      dose: line.dose,
      doseUnit: line.doseUnit,
      decoctionMethod: line.decoctionMethod,
      retailPrice: line.retailPrice,
      orderNo: index + 1
    })),
    version: prescription.version
  }
}

async function save(showMessage = true) {
  const context = props.context
  if (!context || mutationLoading.value) return false
  const pendingDrafts = changedDrafts()
  if (pendingDrafts.length === 0) return true

  for (const prescription of pendingDrafts) {
    const validationError = validate(prescription)
    if (validationError) {
      activeKey.value = prescription.key
      if (showMessage) ElMessage.warning(validationError)
      return false
    }
  }

  mutationLoading.value = true
  try {
    let nextActiveKey = activePrescription.value?.id ?? activeKey.value
    for (const prescription of pendingDrafts) {
      const saved = await saveTcmPrescriptionDraft(toSaveDTO(prescription, context))
      if (prescription.key === activeKey.value) nextActiveKey = saved.id
    }
    activeKey.value = nextActiveKey
    await reload()
    if (showMessage) ElMessage.success('中医处方保存成功')
    return true
  } catch (caught) {
    ElMessage.error(caught instanceof Error ? caught.message : '中医处方保存失败')
    return false
  } finally {
    mutationLoading.value = false
  }
}

function actionDTO(
  prescription: ClinicalTcmPrescriptionDraft,
  context: ClinicalTcmPrescriptionContext
): TcmPrescriptionActionDTO {
  if (!prescription.id) throw new Error('请先保存中医处方')
  return {
    scope: context.scope,
    encounterId: context.encounterId,
    patientId: context.patientId,
    id: prescription.id,
    version: prescription.version
  }
}

async function performAction(action: 'delete' | 'sign' | 'withdraw' | 'stop' | 'cancelStop') {
  let prescription = activePrescription.value
  const context = props.context
  if (!prescription || !context || mutationLoading.value) return

  if (action === 'sign' && dirty.value) {
    if (!(await save(false))) return
    prescription = activePrescription.value
    if (!prescription) return
  } else if (dirty.value && prescription.id) {
    ElMessage.warning('请先保存当前中医处方修改')
    return
  }

  const labels = {
    delete: '删除',
    sign: '签发',
    withdraw: '撤回',
    stop: '停嘱',
    cancelStop: '取消停嘱'
  }
  try {
    await ElMessageBox.confirm(`确定要${labels[action]}当前中医处方吗？`, `${labels[action]}处方`, {
      type: action === 'delete' || action === 'stop' ? 'warning' : 'info',
      confirmButtonText: `确定${labels[action]}`,
      cancelButtonText: '取消'
    })
  } catch {
    return
  }

  if (action === 'delete' && !prescription.id) {
    prescriptions.value = prescriptions.value.filter((item) => item.key !== prescription!.key)
    activeKey.value = prescriptions.value[0]?.key ?? ''
    return
  }

  mutationLoading.value = true
  try {
    const data = actionDTO(prescription, context)
    if (action === 'delete') await deleteTcmPrescription(data)
    else if (action === 'sign') await signTcmPrescription(data)
    else if (action === 'withdraw') await withdrawTcmPrescription(data)
    else if (action === 'stop') await stopTcmPrescription(data)
    else await cancelStopTcmPrescription(data)
    await reload()
    ElMessage.success(`${labels[action]}成功`)
  } catch (caught) {
    ElMessage.error(caught instanceof Error ? caught.message : `${labels[action]}中医处方失败`)
  } finally {
    mutationLoading.value = false
  }
}

function statusTagType(status: ClinicalTcmPrescriptionDraft['status']) {
  if (status === 'DRAFT') return 'primary'
  if (status === 'SIGNED') return 'success'
  if (status === 'STOPPED') return 'danger'
  return 'info'
}

watch(
  () => [props.context?.scope, props.context?.encounterId, props.context?.patientId],
  () => void reload(),
  { immediate: true }
)
watch(dirty, (value) => emit('dirtyChange', value), { immediate: true })
watch(
  () => loading.value || mutationLoading.value,
  (value) => emit('loadingChange', value),
  { immediate: true }
)

defineExpose<ClinicalTcmPrescriptionWorkspaceExpose>({
  save,
  reload,
  isDirty: () => dirty.value,
  validate: validateDirtyPrescriptions
})
</script>

<template>
  <section class="clinical-tcm-prescription">
    <EmptyState
      v-if="!patientSelected"
      title="请选择患者"
      description="选择患者后可查看和维护中医处方"
    />

    <div v-else-if="loading" class="clinical-tcm-prescription__skeleton">
      <el-skeleton :rows="10" animated />
    </div>

    <EmptyState
      v-else-if="error"
      title="中医处方加载失败"
      :description="error"
      action-text="重试"
      @action="reload"
    />

    <div v-else class="clinical-tcm-prescription__layout">
      <aside class="clinical-tcm-prescription__sidebar">
        <header>
          <strong>中医处方</strong>
          <el-button
            type="primary"
            link
            :icon="Plus"
            :disabled="!accepted || readonly"
            @click="addPrescription"
          >
            新增
          </el-button>
        </header>

        <div class="clinical-tcm-prescription__prescriptions">
          <button
            v-for="prescription in prescriptions"
            :key="prescription.key"
            type="button"
            :class="{ 'is-active': prescription.key === activeKey }"
            @click="activeKey = prescription.key"
          >
            <span>{{ prescription.prescriptionNo || '新处方' }}</span>
            <el-tag :type="statusTagType(prescription.status)" size="small">
              {{ prescription.statusName }}
            </el-tag>
            <small>{{ prescription.syndromeName || '未填写病证' }}</small>
          </button>
        </div>

        <div class="clinical-tcm-prescription__history">
          <div class="clinical-tcm-prescription__history-title">历史处方</div>
          <div v-if="historyRows.length === 0" class="clinical-tcm-prescription__history-empty">
            暂无历史处方
          </div>
          <article v-for="history in historyRows" :key="history.id">
            <strong>{{ history.prescriptionNo }}</strong>
            <span>{{ history.summary }}</span>
            <small>{{ history.herbNames.join('、') }}</small>
          </article>
        </div>
      </aside>

      <main class="clinical-tcm-prescription__main">
        <EmptyState
          v-if="!activePrescription"
          title="暂无中医处方"
          :description="
            accepted && !readonly ? '点击“新增”开始开立中医处方' : '当前没有可查看的中医处方'
          "
          :action-text="accepted && !readonly ? '新增处方' : ''"
          @action="addPrescription"
        />

        <template v-else>
          <header class="clinical-tcm-prescription__toolbar">
            <div>
              <strong>{{ activePrescription.prescriptionNo || '中医处方草稿' }}</strong>
              <el-tag :type="statusTagType(activePrescription.status)">
                {{ activePrescription.statusName }}
              </el-tag>
              <span v-if="dirty" class="is-dirty">有未保存修改</span>
            </div>
            <div>
              <el-button :icon="Refresh" :disabled="mutationLoading || dirty" @click="reload">
                刷新
              </el-button>
              <el-button
                v-if="editable"
                type="primary"
                :icon="Select"
                :loading="mutationLoading"
                :disabled="!dirty"
                @click="save()"
              >
                保存
              </el-button>
              <el-button
                v-if="activePrescription.status === 'DRAFT' && !readonly"
                type="success"
                :disabled="!accepted || mutationLoading"
                @click="performAction('sign')"
              >
                签发
              </el-button>
              <el-button
                v-if="activePrescription.status === 'SIGNED' && !readonly"
                type="warning"
                plain
                :disabled="mutationLoading"
                @click="performAction('withdraw')"
              >
                撤回
              </el-button>
              <el-button
                v-if="
                  supportsTherapyType &&
                  activePrescription.status === 'SIGNED' &&
                  activePrescription.therapyType === 'LONG_TERM' &&
                  !readonly
                "
                type="warning"
                :disabled="mutationLoading"
                @click="performAction('stop')"
              >
                停嘱
              </el-button>
              <el-button
                v-if="
                  supportsTherapyType &&
                  activePrescription.status === 'STOPPED' &&
                  activePrescription.therapyType === 'LONG_TERM' &&
                  !readonly
                "
                type="warning"
                plain
                :disabled="mutationLoading"
                @click="performAction('cancelStop')"
              >
                取消停嘱
              </el-button>
              <el-button
                v-if="activePrescription.status === 'DRAFT' && !readonly"
                type="danger"
                plain
                :icon="Delete"
                :disabled="mutationLoading"
                @click="performAction('delete')"
              >
                删除
              </el-button>
            </div>
          </header>

          <el-form class="clinical-tcm-prescription__form" label-position="top">
            <el-form-item v-if="supportsTherapyType" label="医嘱类型">
              <el-radio-group v-model="activePrescription.therapyType" :disabled="!editable">
                <el-radio-button value="LONG_TERM">长期</el-radio-button>
                <el-radio-button value="TEMPORARY">临时</el-radio-button>
              </el-radio-group>
            </el-form-item>
            <el-form-item label="中医病证">
              <el-input
                v-model="activePrescription.syndromeName"
                :disabled="!editable"
                placeholder="例如：气虚血瘀证"
              />
            </el-form-item>
            <el-form-item label="付数">
              <el-input-number
                v-model="activePrescription.doseCount"
                :min="1"
                :precision="0"
                :disabled="!editable"
              />
            </el-form-item>
            <el-form-item label="用法">
              <el-select
                v-model="activePrescription.method"
                :disabled="!editable"
                allow-create
                filterable
              >
                <el-option v-for="item in methodOptions" :key="item" :label="item" :value="item" />
              </el-select>
            </el-form-item>
            <el-form-item label="频次">
              <el-select
                v-model="activePrescription.frequency"
                :disabled="!editable"
                allow-create
                filterable
              >
                <el-option
                  v-for="item in frequencyOptions"
                  :key="item"
                  :label="item"
                  :value="item"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="疗程">
              <el-input-number
                v-model="activePrescription.days"
                :min="1"
                :precision="0"
                :disabled="!editable"
              />
            </el-form-item>
            <el-form-item label="煎药方式">
              <el-select v-model="activePrescription.decoctionMethod" :disabled="!editable">
                <el-option
                  v-for="item in decoctionOptions"
                  :key="item"
                  :label="item"
                  :value="item"
                />
              </el-select>
            </el-form-item>
            <el-form-item label="备注">
              <el-input
                v-model="activePrescription.remark"
                :disabled="!editable"
                placeholder="服药说明"
              />
            </el-form-item>
          </el-form>

          <section class="clinical-tcm-prescription__herbs">
            <header>
              <div>
                <strong>中药饮片</strong>
                <el-tag size="small" effect="plain">
                  {{ activePrescription.lines.length }} 味
                </el-tag>
              </div>
              <el-button
                v-if="editable"
                type="primary"
                plain
                :icon="Plus"
                @click="selectorVisible = true"
              >
                添加饮片
              </el-button>
            </header>

            <el-table :data="activePrescription.lines" empty-text="暂无饮片，请点击“添加饮片”">
              <el-table-column prop="name" label="饮片" min-width="150">
                <template #default="{ row }">
                  <strong>{{ row.name }}</strong>
                  <small class="clinical-tcm-prescription__spec">{{ row.specification }}</small>
                </template>
              </el-table-column>
              <el-table-column prop="dose" label="单剂用量" width="150">
                <template #default="{ row }">
                  <div class="clinical-tcm-prescription__dose">
                    <el-input-number
                      v-model="row.dose"
                      :min="0.1"
                      :precision="1"
                      :controls="false"
                      :disabled="!editable"
                    />
                    <span>{{ row.doseUnit }}</span>
                  </div>
                </template>
              </el-table-column>
              <el-table-column prop="decoctionMethod" label="特殊煎法" width="140">
                <template #default="{ row }">
                  <el-select v-model="row.decoctionMethod" :disabled="!editable">
                    <el-option
                      v-for="item in specialDecoctionOptions"
                      :key="item || 'none'"
                      :label="item || '无'"
                      :value="item"
                    />
                  </el-select>
                </template>
              </el-table-column>
              <el-table-column label="总量" width="110" align="right">
                <template #default="{ row }">
                  {{ Number(row.dose * activePrescription.doseCount).toFixed(1) }}{{ row.doseUnit }}
                </template>
              </el-table-column>
              <el-table-column label="金额" width="110" align="right">
                <template #default="{ row }">
                  ¥{{
                    Number(row.dose * row.retailPrice * activePrescription.doseCount).toFixed(2)
                  }}
                </template>
              </el-table-column>
              <el-table-column v-if="editable" label="操作" width="70" align="center">
                <template #default="{ row }">
                  <el-button type="danger" link :icon="Delete" @click="removeHerb(row.key)" />
                </template>
              </el-table-column>
            </el-table>

            <footer>
              <span
                >共 {{ activePrescription.lines.length }} 味，{{
                  activePrescription.doseCount
                }}
                付</span
              >
              <strong>预计金额：¥{{ totalAmount.toFixed(2) }}</strong>
            </footer>
          </section>
        </template>
      </main>
    </div>

    <TcmHerbSelectorDialog
      v-model:visible="selectorVisible"
      :load-page="loadHerbCatalog"
      :existing-catalog-ids="existingCatalogIds"
      @confirm="addHerbs"
      @load-error="ElMessage.error('中药饮片目录加载失败，请重试')"
    />
  </section>
</template>

<style scoped>
.clinical-tcm-prescription {
  box-sizing: border-box;
  width: 100%;
  min-width: 0;
  min-height: 100%;
  color: var(--el-text-color-primary, #303133);
  background: var(--el-bg-color, #fff);
}

.clinical-tcm-prescription__skeleton {
  padding: 20px;
}

.clinical-tcm-prescription__layout {
  display: grid;
  min-height: 100%;
  grid-template-columns: 210px minmax(0, 1fr);
}

.clinical-tcm-prescription__sidebar {
  border-right: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-extra-light);
}

.clinical-tcm-prescription__sidebar > header,
.clinical-tcm-prescription__toolbar,
.clinical-tcm-prescription__herbs > header,
.clinical-tcm-prescription__herbs > footer {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}

.clinical-tcm-prescription__sidebar > header {
  height: 48px;
  padding: 0 12px;
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.clinical-tcm-prescription__prescriptions {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 10px;
}

.clinical-tcm-prescription__prescriptions button {
  display: grid;
  grid-template-columns: minmax(0, 1fr) auto;
  gap: 5px;
  padding: 10px;
  border: 1px solid var(--el-border-color-light);
  border-radius: 8px;
  background: var(--el-bg-color);
  color: var(--el-text-color-primary);
  text-align: left;
  cursor: pointer;
}

.clinical-tcm-prescription__prescriptions button.is-active {
  border-color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
}

.clinical-tcm-prescription__prescriptions small {
  grid-column: 1 / -1;
  color: var(--el-text-color-secondary);
}

.clinical-tcm-prescription__history {
  border-top: 1px solid var(--el-border-color-lighter);
}

.clinical-tcm-prescription__history-title,
.clinical-tcm-prescription__history-empty,
.clinical-tcm-prescription__history article {
  padding: 10px 12px;
}

.clinical-tcm-prescription__history-title {
  font-weight: 700;
}

.clinical-tcm-prescription__history-empty {
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.clinical-tcm-prescription__history article {
  display: flex;
  flex-direction: column;
  gap: 3px;
  border-top: 1px solid var(--el-border-color-extra-light);
  font-size: 12px;
}

.clinical-tcm-prescription__history article span,
.clinical-tcm-prescription__history article small {
  color: var(--el-text-color-secondary);
}

.clinical-tcm-prescription__main {
  min-width: 0;
  padding: 14px;
}

.clinical-tcm-prescription__toolbar > div {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 8px;
}

.clinical-tcm-prescription__toolbar .is-dirty {
  color: var(--el-color-warning);
  font-size: 13px;
}

.clinical-tcm-prescription__form {
  display: grid;
  grid-template-columns: repeat(4, minmax(130px, 1fr));
  gap: 0 12px;
  margin-top: 14px;
  padding: 14px;
  border-radius: 8px;
  background: var(--el-fill-color-extra-light);
}

.clinical-tcm-prescription__form :deep(.el-form-item) {
  margin-bottom: 10px;
}

.clinical-tcm-prescription__form :deep(.el-select),
.clinical-tcm-prescription__form :deep(.el-input-number) {
  width: 100%;
}

.clinical-tcm-prescription__herbs {
  margin-top: 14px;
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  border-radius: 8px;
}

.clinical-tcm-prescription__herbs > header,
.clinical-tcm-prescription__herbs > footer {
  padding: 10px 12px;
}

.clinical-tcm-prescription__herbs > header > div {
  display: flex;
  align-items: center;
  gap: 8px;
}

.clinical-tcm-prescription__herbs > footer {
  border-top: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-extra-light);
}

.clinical-tcm-prescription__spec {
  display: block;
  color: var(--el-text-color-secondary);
}

.clinical-tcm-prescription__dose {
  display: flex;
  align-items: center;
  gap: 6px;
}

.clinical-tcm-prescription__dose :deep(.el-input-number) {
  width: 96px;
}

@media (max-width: 1200px) {
  .clinical-tcm-prescription__layout {
    grid-template-columns: 180px minmax(0, 1fr);
  }

  .clinical-tcm-prescription__form {
    grid-template-columns: repeat(2, minmax(130px, 1fr));
  }
}
</style>
