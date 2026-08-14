<script setup lang="ts">
import type { FormInstance, FormRules } from 'element-plus'
import type {
  BookingOption,
  GenerateFormalSchedulesPayload
} from '@/apis/booking/outpatient-schedule'
import type { CalendarDateRange } from '@/components/schedule/type'

import { computed, nextTick, reactive, ref, watch } from 'vue'

import MyDialog from '@/components/mydialog/MyDialog.vue'

interface GenerateScheduleForm {
  templateKind: 'DAY' | 'WEEK'
  scheduleTemplateId: string
  weekScheduleTemplateId: string
  rangeStart: string
  rangeEnd: string
  autoPublish: boolean
  autoProvision: boolean
}

const props = defineProps<{
  dailyTemplates: BookingOption[]
  weeklyTemplates: BookingOption[]
  submitting?: boolean
}>()

const emit = defineEmits<{
  submit: [payload: Omit<GenerateFormalSchedulesPayload, 'organizationId'>]
}>()

const dialog = ref<{ openDialog: () => void; closeDialog: () => void }>()
const formRef = ref<FormInstance>()
const form = reactive<GenerateScheduleForm>({
  templateKind: 'DAY',
  scheduleTemplateId: '',
  weekScheduleTemplateId: '',
  rangeStart: '',
  rangeEnd: '',
  autoPublish: true,
  autoProvision: false
})

const selectedTemplateId = computed({
  get: () =>
    form.templateKind === 'DAY' ? form.scheduleTemplateId : form.weekScheduleTemplateId,
  set: (value: string) => {
    if (form.templateKind === 'DAY') form.scheduleTemplateId = value
    else form.weekScheduleTemplateId = value
  }
})
const currentTemplates = computed(() =>
  form.templateKind === 'DAY' ? props.dailyTemplates : props.weeklyTemplates
)

const rules: FormRules<GenerateScheduleForm> = {
  rangeStart: [{ required: true, message: '请选择开始日期', trigger: 'change' }],
  rangeEnd: [{ required: true, message: '请选择结束日期', trigger: 'change' }]
}

function reset(range: CalendarDateRange) {
  const templateKind = props.dailyTemplates.length ? 'DAY' : 'WEEK'
  Object.assign(form, {
    templateKind,
    scheduleTemplateId: props.dailyTemplates[0]?.value ?? '',
    weekScheduleTemplateId: props.weeklyTemplates[0]?.value ?? '',
    rangeStart: range.start,
    rangeEnd: range.end,
    autoPublish: true,
    autoProvision: false
  })
  nextTick(() => formRef.value?.clearValidate())
}

function open(range: CalendarDateRange) {
  reset(range)
  dialog.value?.openDialog()
}

function close() {
  dialog.value?.closeDialog()
}

function handleTemplateKindChange() {
  if (!selectedTemplateId.value) selectedTemplateId.value = currentTemplates.value[0]?.value ?? ''
}

async function submit() {
  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid || props.submitting) return
  if (!selectedTemplateId.value) {
    formRef.value?.validateField('templateKind').catch(() => undefined)
    return
  }
  if (form.rangeEnd < form.rangeStart) return
  emit('submit', {
    templateKind: form.templateKind,
    scheduleTemplateId: form.templateKind === 'DAY' ? form.scheduleTemplateId : undefined,
    weekScheduleTemplateId: form.templateKind === 'WEEK' ? form.weekScheduleTemplateId : undefined,
    rangeStart: form.rangeStart,
    rangeEnd: form.rangeEnd,
    autoPublish: form.autoPublish,
    autoProvision: form.autoProvision
  })
}

watch(
  () => form.autoPublish,
  (autoPublish) => {
    if (!autoPublish) form.autoProvision = false
  }
)

defineExpose({ open, close })
</script>

<template>
  <MyDialog
    ref="dialog"
    :data="form"
    title="生成正式排班"
    width="560px"
    align-center
    :close-on-click-modal="false"
  >
    <p class="generate-dialog__hint">
      将“排班设置”中的日排班或周排班模板批量生成为正式日程。
    </p>
    <el-form ref="formRef" :model="form" :rules="rules" label-width="96px">
      <el-form-item label="排班类型" required>
        <el-radio-group v-model="form.templateKind" @change="handleTemplateKindChange">
          <el-radio-button value="DAY">日排班</el-radio-button>
          <el-radio-button value="WEEK">周排班</el-radio-button>
        </el-radio-group>
      </el-form-item>
      <el-form-item
        :label="form.templateKind === 'DAY' ? '日排班' : '周排班'"
        required
      >
        <el-select
          v-model="selectedTemplateId"
          filterable
          :placeholder="form.templateKind === 'DAY' ? '请选择日排班' : '请选择周排班'"
          style="width: 100%"
        >
          <el-option
            v-for="item in currentTemplates"
            :key="item.value"
            :label="item.label"
            :value="item.value"
          />
        </el-select>
        <div v-if="!currentTemplates.length" class="generate-dialog__error">
          当前没有可用模板，请先到“排班设置”中维护并启用模板。
        </div>
      </el-form-item>
      <el-form-item label="生成区间" required>
        <div class="generate-dialog__range">
          <el-date-picker
            v-model="form.rangeStart"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="开始日期"
          />
          <span>至</span>
          <el-date-picker
            v-model="form.rangeEnd"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="结束日期"
          />
        </div>
        <div v-if="form.rangeStart && form.rangeEnd && form.rangeEnd < form.rangeStart" class="generate-dialog__error">
          结束日期不能早于开始日期
        </div>
        <div class="generate-dialog__tip">区间内逐日生成，已存在排班会自动跳过。</div>
      </el-form-item>
      <el-form-item label="自动发布">
        <el-switch v-model="form.autoPublish" />
        <span class="generate-dialog__switch-tip">
          {{ form.autoPublish ? '生成后直接发布' : '生成草稿，需后续手动发布' }}
        </span>
      </el-form-item>
      <el-form-item label="自动放号">
        <el-switch v-model="form.autoProvision" :disabled="!form.autoPublish" />
        <span class="generate-dialog__switch-tip">自动发布后才可自动放号</span>
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button :disabled="submitting" @click="close">取消</el-button>
      <el-button
        type="primary"
        :loading="submitting"
        :disabled="!selectedTemplateId || form.rangeEnd < form.rangeStart"
        @click="submit"
      >
        生成正式排班
      </el-button>
    </template>
  </MyDialog>
</template>

<style scoped>
.generate-dialog__hint {
  margin: 0 0 18px;
  color: var(--el-text-color-regular);
  font-size: 14px;
  line-height: 1.6;
}
.generate-dialog__range {
  display: grid;
  width: 100%;
  grid-template-columns: minmax(0, 1fr) auto minmax(0, 1fr);
  align-items: center;
  gap: 8px;
}
.generate-dialog__range :deep(.el-date-editor) {
  width: 100%;
}
.generate-dialog__error {
  width: 100%;
  color: var(--el-color-danger);
  font-size: 12px;
  line-height: 1.5;
}
.generate-dialog__tip,
.generate-dialog__switch-tip {
  color: var(--el-text-color-secondary);
  font-size: 12px;
  line-height: 1.5;
}
.generate-dialog__switch-tip {
  margin-left: 10px;
}
@media (max-width: 560px) {
  .generate-dialog__range {
    grid-template-columns: 1fr;
  }
  .generate-dialog__range > span {
    display: none;
  }
}
</style>
