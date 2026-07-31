<script setup lang="ts">
import type { FormInstance, FormRules } from 'element-plus'
import type {
  BookingOption,
  ScheduleRecord
} from '@/apis/booking/outpatient-schedule'
import type { ScheduleSavePayload } from '../type'

import { computed, nextTick, reactive, ref } from 'vue'

import MyDialog from '@/components/mydialog/MyDialog.vue'
import RemoteSelectField from '@/components/common/RemoteSelectField.vue'

const props = defineProps<{
  departments: BookingOption[]
  doctors: BookingOption[]
  rooms: BookingOption[]
  saving?: boolean
  loadDepartments: (keyword: string, context: { signal: AbortSignal }) => Promise<BookingOption[]>
  loadDoctors: (
    keyword: string,
    context: { signal: AbortSignal },
    departmentId?: string | null
  ) => Promise<BookingOption[]>
}>()

const emit = defineEmits<{
  submit: [id: string | undefined, payload: ScheduleSavePayload]
}>()

const dialog = ref<{ openDialog: () => void; closeDialog: () => void }>()
const formRef = ref<FormInstance>()
const editingId = ref<string>()

const form = reactive<ScheduleSavePayload>({
  departmentId: '',
  doctorId: '',
  roomId: '',
  date: '',
  startTime: '08:00',
  endTime: '12:00',
  capacity: 20,
  publish: false
})

const title = computed(() => (editingId.value ? '编辑门诊排班' : '新增门诊排班'))
const filteredDoctors = computed(() =>
  props.doctors.filter((item) => !form.departmentId || item.departmentId === form.departmentId)
)

const rules: FormRules<ScheduleSavePayload> = {
  departmentId: [{ required: true, message: '请选择科室', trigger: 'change' }],
  doctorId: [{ required: true, message: '请选择医生', trigger: 'change' }],
  roomId: [{ required: true, message: '请选择诊室', trigger: 'change' }],
  date: [{ required: true, message: '请选择排班日期', trigger: 'change' }],
  startTime: [{ required: true, message: '请选择开始时间', trigger: 'change' }],
  endTime: [{ required: true, message: '请选择结束时间', trigger: 'change' }]
}

function reset(record?: ScheduleRecord, date?: string) {
  editingId.value = record?.id
  Object.assign(form, {
    departmentId: record?.departmentId ?? '',
    doctorId: record?.doctorId ?? '',
    roomId: record?.roomId ?? '',
    date: record?.date ?? date ?? '',
    startTime: record?.startTime ?? '08:00',
    endTime: record?.endTime ?? '12:00',
    capacity: record?.capacity ?? 20,
    publish: record?.status === 'published'
  })
  nextTick(() => formRef.value?.clearValidate())
}

function open(record?: ScheduleRecord, date?: string) {
  reset(record, date)
  dialog.value?.openDialog()
}

function close() {
  dialog.value?.closeDialog()
}

async function submit() {
  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid || form.endTime <= form.startTime || props.saving) return
  emit('submit', editingId.value, { ...form })
}

function handleDepartmentChange() {
  const doctor = props.doctors.find((item) => item.value === form.doctorId)
  if (doctor && doctor.departmentId !== form.departmentId) form.doctorId = ''
}

defineExpose({ open, close })
</script>

<template>
  <MyDialog
    ref="dialog"
    :data="form"
    :title="title"
    width="680px"
    align-center
    :close-on-click-modal="false"
  >
    <el-form ref="formRef" :model="form" :rules="rules" label-width="92px" class="schedule-form">
      <el-form-item label="科室" prop="departmentId">
        <RemoteSelectField
          v-model="form.departmentId"
          :load-options="loadDepartments"
          :initial-options="departments"
          value-key="value"
          label-key="label"
          placeholder="搜索并选择科室"
          @change="handleDepartmentChange"
        />
      </el-form-item>
      <el-form-item label="医生" prop="doctorId">
        <RemoteSelectField
          v-model="form.doctorId"
          :load-options="(keyword, context) => loadDoctors(keyword, context, form.departmentId)"
          :initial-options="filteredDoctors"
          value-key="value"
          label-key="label"
          placeholder="搜索并选择医生"
        />
      </el-form-item>
      <el-form-item label="诊室" prop="roomId">
        <el-select v-model="form.roomId" placeholder="请选择诊室" style="width: 100%">
          <el-option
            v-for="room in rooms"
            :key="room.value"
            :label="room.label"
            :value="room.value"
          />
        </el-select>
      </el-form-item>
      <div class="schedule-form__grid">
        <el-form-item label="日期" prop="date">
          <el-date-picker
            v-model="form.date"
            type="date"
            value-format="YYYY-MM-DD"
            placeholder="选择日期"
            style="width: 100%"
          />
        </el-form-item>
        <el-form-item label="时间" required>
          <div class="schedule-form__time">
            <el-time-select
              v-model="form.startTime"
              start="07:00"
              step="00:30"
              end="20:00"
              placeholder="开始"
            />
            <span>至</span>
            <el-time-select
              v-model="form.endTime"
              start="07:30"
              step="00:30"
              end="21:00"
              placeholder="结束"
            />
          </div>
          <div v-if="form.endTime <= form.startTime" class="schedule-form__error">
            结束时间必须晚于开始时间
          </div>
        </el-form-item>
        <el-form-item label="总号数" prop="capacity">
          <el-input-number
            v-model="form.capacity"
            :min="1"
            :max="500"
            controls-position="right"
            style="width: 100%"
          />
        </el-form-item>
      </div>
      <el-form-item label="保存后发布">
        <el-switch v-model="form.publish" active-text="发布" inactive-text="保存草稿" />
      </el-form-item>
    </el-form>
    <template #footer>
      <el-button :disabled="saving" @click="close">取消</el-button>
      <el-button type="primary" :loading="saving" @click="submit">保存</el-button>
    </template>
  </MyDialog>
</template>

<style scoped>
.schedule-form {
  padding: 4px 12px 0 0;
}
.schedule-form__grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 0 12px;
}
.schedule-form__time {
  display: flex;
  width: 100%;
  align-items: center;
  gap: 8px;
}
.schedule-form__time :deep(.el-select) {
  flex: 1;
  min-width: 0;
}
.schedule-form__error {
  color: var(--el-color-danger);
  font-size: 12px;
  line-height: 1.4;
}
@media (max-width: 700px) {
  .schedule-form__grid {
    grid-template-columns: 1fr;
  }
}
</style>
