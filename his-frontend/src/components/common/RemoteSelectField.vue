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
const searchPending = shallowRef(false)
const hasLoaded = shallowRef(false)
const errorMessage = shallowRef('')
const lastKeyword = shallowRef('')

let debounceTimer: ReturnType<typeof setTimeout> | undefined
let requestController: AbortController | undefined
let requestSequence = 0
// 首次打开下拉框时，Element Plus 会先用空关键字触发一次远程查询。
// 只要用户还没有输入过关键字且当前已有选项，就直接复用现有列表。
let skipOpeningEmptySearch = true

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
    // initialOptions 是上层筛选后的当前候选集（例如当前科室的医生）。
    // 仅保留当前候选项和仍被选中的远程项，避免科室切换后混入旧科室医生。
    const selectedOptions = getSelectedOptions(options.value)
    options.value = mergeOptions(initialOptions, selectedOptions)
  },
  { immediate: true }
)

watch(
  modelValue,
  () => {
    // 上层切换筛选条件后通常会先清空已选医生；同步裁剪旧候选项。
    options.value = mergeOptions(props.initialOptions, getSelectedOptions(options.value))
  },
  { deep: true }
)

function cancelPendingRequest() {
  requestController?.abort()
  requestController = undefined
}

async function load(keyword: string) {
  searchPending.value = false
  const normalizedKeyword = keyword.trim()
  lastKeyword.value = normalizedKeyword
  errorMessage.value = ''

  if (normalizedKeyword.length < Math.max(0, props.minKeywordLength)) {
    cancelPendingRequest()
    options.value = [...props.initialOptions]
    loading.value = false
    hasLoaded.value = true
    skipOpeningEmptySearch = true
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
    hasLoaded.value = true
    if (!normalizedKeyword) skipOpeningEmptySearch = true
  } catch (error) {
    if (controller.signal.aborted || currentSequence !== requestSequence) return

    errorMessage.value = '选项加载失败，请重试'
    options.value = getSelectedOptions(mergeOptions(props.initialOptions, options.value))
    hasLoaded.value = true
    emit('loadError', error, normalizedKeyword)
  } finally {
    if (currentSequence === requestSequence) loading.value = false
  }
}

function search(keyword: string) {
  const normalizedKeyword = keyword.trim()

  // 打开下拉框时 Element Plus 可能会用空关键字触发一次 remote-method。
  // 已有选项时直接保留当前列表，避免列表先清空再重新加载造成闪烁。
  // 用户主动清空已输入的关键字时不走这里，仍然按空关键字重新查询完整列表。
  if (!normalizedKeyword && skipOpeningEmptySearch && options.value.length > 0) {
    if (debounceTimer) clearTimeout(debounceTimer)
    debounceTimer = undefined
    searchPending.value = false
    options.value = mergeOptions(props.initialOptions, options.value)
    return
  }

  if (normalizedKeyword) skipOpeningEmptySearch = false

  if (debounceTimer) clearTimeout(debounceTimer)

  // Element Plus 会在防抖期间先按新关键字过滤旧选项，
  // 此时也要显示加载状态，避免短暂出现“暂无匹配数据”。
  searchPending.value = true

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
  if (visible && options.value.length === 0 && !loading.value && !searchPending.value) {
    void load(lastKeyword.value)
  }
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
            <span>{{ errorMessage || (loading || searchPending || !hasLoaded ? loadingText : '暂无匹配数据') }}</span>
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
