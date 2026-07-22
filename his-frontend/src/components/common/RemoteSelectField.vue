<script setup lang="ts" generic="T extends Record<string, unknown>">
import type { RemoteSelectLoadOptions, SelectValue } from './select-type'

import { computed, onBeforeUnmount, shallowRef, watch } from 'vue'

defineOptions({
  name: 'RemoteSelectField',
  inheritAttrs: false
})

interface RemoteSelectFieldProps<TOption> {
  loadOptions: RemoteSelectLoadOptions<TOption>
  initialOptions?: TOption[]
  valueKey?: keyof TOption & string | string
  labelKey?: keyof TOption & string | string
  multiple?: boolean
  disabled?: boolean
  clearable?: boolean
  filterable?: boolean
  debounce?: number
  minKeywordLength?: number
  placeholder?: string
  collapseTags?: boolean
  maxCollapseTags?: number
  optionDisabled?: (option: TOption) => boolean
}

const props = withDefaults(defineProps<RemoteSelectFieldProps<T>>(), {
  initialOptions: () => [],
  valueKey: 'value',
  labelKey: 'label',
  multiple: false,
  disabled: false,
  clearable: true,
  filterable: true,
  debounce: 300,
  minKeywordLength: 0,
  placeholder: '请选择',
  collapseTags: true,
  maxCollapseTags: 2,
  optionDisabled: undefined
})

const emit = defineEmits<{
  change: [value: SelectValue | SelectValue[] | null, selected: T | T[] | undefined]
  search: [keyword: string]
  loadError: [error: unknown, keyword: string]
  retry: [keyword: string]
}>()

const modelValue = defineModel<SelectValue | SelectValue[] | null>({ default: null })

const options = shallowRef<T[]>([])
const loading = shallowRef(false)
const errorMessage = shallowRef('')
const lastKeyword = shallowRef('')

let debounceTimer: ReturnType<typeof setTimeout> | undefined
let requestController: AbortController | undefined
let requestSequence = 0

function getOptionValue(option: T) {
  return option[props.valueKey] as SelectValue | undefined
}

function getOptionLabel(option: T) {
  const label = option[props.labelKey]
  return label == null ? '' : String(label)
}

function mergeOptions(primary: T[], secondary: T[]) {
  const merged = new Map<SelectValue, T>()

  for (const option of [...primary, ...secondary]) {
    const value = getOptionValue(option)
    if (value !== undefined && value !== null) merged.set(value, option)
  }

  return [...merged.values()]
}

function getSelectedOptions(candidates: T[]) {
  const values = Array.isArray(modelValue.value)
    ? new Set(modelValue.value)
    : new Set(modelValue.value == null ? [] : [modelValue.value])

  return candidates.filter((option) => {
    const value = getOptionValue(option)
    return value !== undefined && values.has(value)
  })
}

watch(
  () => props.initialOptions,
  (initialOptions) => {
    options.value = mergeOptions(initialOptions, options.value)
  },
  { immediate: true }
)

function cancelPendingRequest() {
  requestController?.abort()
  requestController = undefined
}

async function load(keyword: string) {
  const normalizedKeyword = keyword.trim()
  lastKeyword.value = normalizedKeyword
  errorMessage.value = ''

  if (normalizedKeyword.length < Math.max(0, props.minKeywordLength)) {
    cancelPendingRequest()
    options.value = [...props.initialOptions]
    loading.value = false
    return
  }

  cancelPendingRequest()
  const controller = new AbortController()
  requestController = controller
  const currentSequence = ++requestSequence
  loading.value = true
  emit('search', normalizedKeyword)

  try {
    const loadedOptions = await props.loadOptions(normalizedKeyword, {
      signal: controller.signal
    })

    if (controller.signal.aborted || currentSequence !== requestSequence) return
    const selectedOptions = getSelectedOptions(mergeOptions(props.initialOptions, options.value))
    options.value = mergeOptions(selectedOptions, loadedOptions)
  } catch (error) {
    if (controller.signal.aborted || currentSequence !== requestSequence) return

    errorMessage.value = '选项加载失败，请重试'
    options.value = getSelectedOptions(mergeOptions(props.initialOptions, options.value))
    emit('loadError', error, normalizedKeyword)
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function search(keyword: string) {
  if (debounceTimer) clearTimeout(debounceTimer)

  debounceTimer = setTimeout(() => {
    debounceTimer = undefined
    void load(keyword)
  }, Math.max(0, props.debounce))
}

function retry() {
  emit('retry', lastKeyword.value)
  void load(lastKeyword.value)
}

function findSelected(value: SelectValue | SelectValue[] | null) {
  if (Array.isArray(value)) {
    const selectedValues = new Set(value)
    return options.value.filter((option) => {
      const optionValue = getOptionValue(option)
      return optionValue !== undefined && selectedValues.has(optionValue)
    })
  }

  if (value === null || value === undefined) return undefined
  return options.value.find((option) => getOptionValue(option) === value)
}

function handleChange(value: SelectValue | SelectValue[] | null) {
  emit('change', value, findSelected(value))
}

function handleVisibleChange(visible: boolean) {
  if (visible && options.value.length === 0 && !loading.value) void load(lastKeyword.value)
}

const loadingText = computed(() => (errorMessage.value ? '加载失败' : '正在加载'))

onBeforeUnmount(() => {
  if (debounceTimer) clearTimeout(debounceTimer)
  cancelPendingRequest()
  requestSequence += 1
})

defineExpose({
  reload: retry,
  search: load
})
</script>

<template>
  <div class="remote-select-field">
    <el-select
      v-model="modelValue"
      v-bind="$attrs"
      class="remote-select-field__select"
      :multiple="multiple"
      :disabled="disabled"
      :clearable="clearable"
      :filterable="filterable"
      :remote="filterable"
      :remote-method="search"
      :loading="loading"
      :loading-text="loadingText"
      :placeholder="placeholder"
      :collapse-tags="multiple && collapseTags"
      :max-collapse-tags="maxCollapseTags"
      reserve-keyword
      @change="handleChange"
      @visible-change="handleVisibleChange"
    >
      <template v-if="$slots.prefix" #prefix>
        <slot name="prefix" />
      </template>

      <el-option
        v-for="option in options.filter((item) => getOptionValue(item) !== undefined)"
        :key="getOptionValue(option)"
        :label="getOptionLabel(option)"
        :value="getOptionValue(option)!"
        :disabled="optionDisabled?.(option)"
      >
        <slot name="option" :option="option">
          <span>{{ getOptionLabel(option) }}</span>
        </slot>
      </el-option>

      <template #empty>
        <slot name="empty" :loading="loading" :error="errorMessage" :retry="retry">
          <div class="remote-select-field__empty">
            <span>{{ errorMessage || '暂无匹配数据' }}</span>
            <el-button v-if="errorMessage" type="primary" link @click="retry">重试</el-button>
          </div>
        </slot>
      </template>
    </el-select>

    <div v-if="errorMessage" class="remote-select-field__error" role="alert">
      <span>{{ errorMessage }}</span>
      <el-button type="primary" link :disabled="loading" @click="retry">重新加载</el-button>
    </div>
  </div>
</template>

<style scoped>
.remote-select-field {
  display: flex;
  width: 100%;
  min-width: 0;
  flex-direction: column;
  gap: 4px;
}

.remote-select-field__select {
  width: 100%;
}

.remote-select-field__empty {
  display: flex;
  min-height: 54px;
  align-items: center;
  justify-content: center;
  gap: 8px;
  color: var(--el-text-color-secondary);
  font-size: 13px;
}

.remote-select-field__error {
  display: flex;
  align-items: center;
  gap: 6px;
  color: var(--el-color-danger);
  font-size: 12px;
}
</style>
