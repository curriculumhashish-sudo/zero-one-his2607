<template>
  <div class="date-range-field">
    <el-date-picker
      :model-value="modelValue"
      type="daterange"
      value-format="YYYY-MM-DD"
      range-separator="至"
      start-placeholder="开始日期"
      end-placeholder="结束日期"
      :shortcuts="shortcuts"
      style="width: 100%"
      @update:model-value="emit('update:modelValue', $event)"
    />
  </div>
</template>

<script lang="ts" setup>
import dayjs from 'dayjs'

// ==================== Props & Emits ====================

withDefaults(
  defineProps<{
    modelValue?: [string, string] | null
  }>(),
  { modelValue: null }
)

const emit = defineEmits<{
  'update:modelValue': [value: [string, string] | null]
}>()

// ==================== 快捷日期 ====================

const shortcuts = [
  {
    text: '最近7天',
    value: () => {
      const end = dayjs().format('YYYY-MM-DD')
      const start = dayjs().subtract(6, 'day').format('YYYY-MM-DD')
      return [start, end]
    }
  },
  {
    text: '最近30天',
    value: () => {
      const end = dayjs().format('YYYY-MM-DD')
      const start = dayjs().subtract(29, 'day').format('YYYY-MM-DD')
      return [start, end]
    }
  },
  {
    text: '本月',
    value: () => {
      const end = dayjs().format('YYYY-MM-DD')
      const start = dayjs().startOf('month').format('YYYY-MM-DD')
      return [start, end]
    }
  }
]
</script>

<style scoped>
.date-range-field {
  width: 100%;
}
</style>
