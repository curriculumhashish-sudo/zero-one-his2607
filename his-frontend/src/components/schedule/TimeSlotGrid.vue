<template>
  <section class="time-slot-grid" :aria-busy="loading">
    <div v-if="loading" class="time-slot-grid__state"><el-skeleton :rows="2" animated /></div>
    <div v-else-if="slots.length === 0" class="time-slot-grid__state">
      <slot name="empty">{{ emptyText }}</slot>
    </div>
    <template v-else>
      <section v-for="group in groups" :key="group.key" class="time-slot-grid__group">
        <header v-if="group.key" class="time-slot-grid__group-header">
          <slot name="groupHeader" :group="group.key" :slots="group.slots">{{ group.key }}</slot>
        </header>
        <div class="time-slot-grid__items">
          <button
            v-for="slot in group.slots"
            :key="slot.key"
            type="button"
            class="time-slot-grid__item"
            :class="[
              `is-${statusType(slot)}`,
              {
                'is-selected': modelValue === slot.key,
                'is-disabled': !isSelectable(slot),
                'is-readonly': readonly
              }
            ]"
            :aria-disabled="!isSelectable(slot)"
            @click="select(slot)"
          >
            <slot name="item" :data="slot" :selectable="isSelectable(slot)" :status-text="statusText(slot)">
              <strong class="time-slot-grid__time">{{ labelOf(slot) }}</strong>
              <span v-if="showCapacity" class="time-slot-grid__capacity">{{ capacityText(slot) }}</span>
              <small class="time-slot-grid__status">{{ statusText(slot) }}</small>
            </slot>
          </button>
        </div>
      </section>
    </template>
    <footer v-if="$slots.footer" class="time-slot-grid__footer"><slot name="footer" /></footer>
  </section>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import {
  DEFAULT_TIME_SLOT_STATUS_MAP,
  type ScheduleKey,
  type TimeSlot,
  type TimeSlotStatusMap
} from './type'

/**
 * C29 TimeSlotGrid —— 号源/时段选择网格。
 *
 * 受控单选：号源由页面注入，可选性由 statusMap.selectable 或剩余量决定。
 * 组件不请求预约接口、不写死业务号源。
 */
defineOptions({ name: 'TimeSlotGrid' })

const props = withDefaults(
  defineProps<{
    /** 号源列表 */
    slots?: TimeSlot[]
    /** 当前选中号源 key（受控） */
    modelValue?: ScheduleKey | null
    /** 分组：字段名或取值函数；不传则不分组 */
    groupBy?: ((slot: TimeSlot) => string) | string
    /** 状态映射（文案/语义色/是否可选），覆盖内置默认 */
    statusMap?: TimeSlotStatusMap
    /** 加载中 */
    loading?: boolean
    /** 只读：可选项也不再响应选择 */
    readonly?: boolean
    /** 是否展示容量/剩余信息 */
    showCapacity?: boolean
    /** 空数据文案 */
    emptyText?: string
  }>(),
  {
    slots: () => [],
    modelValue: null,
    groupBy: undefined,
    statusMap: () => ({}),
    loading: false,
    readonly: false,
    showCapacity: true,
    emptyText: '暂无可选时段'
  }
)

const emit = defineEmits<{
  'update:modelValue': [value: ScheduleKey]
  select: [slot: TimeSlot]
  disabledClick: [slot: TimeSlot]
}>()

/** 合并内置状态与页面覆盖 */
const mergedStatusMap = computed<TimeSlotStatusMap>(() => ({
  ...DEFAULT_TIME_SLOT_STATUS_MAP,
  ...props.statusMap
}))

const groups = computed(() => {
  const items = new Map<string, TimeSlot[]>()
  for (const slot of props.slots) {
    let key = ''
    if (typeof props.groupBy === 'function') key = props.groupBy(slot)
    else if (typeof props.groupBy === 'string') key = String(slot[props.groupBy as keyof TimeSlot] ?? '')
    const list = items.get(key)
    if (list) list.push(slot)
    else items.set(key, [slot])
  }
  return [...items].map(([key, slots]) => ({ key, slots }))
})

function statusKey(slot: TimeSlot): string {
  return slot.status ?? 'available'
}

function statusText(slot: TimeSlot): string {
  const key = statusKey(slot)
  return mergedStatusMap.value[key]?.text ?? key
}

function statusType(slot: TimeSlot): string {
  return mergedStatusMap.value[statusKey(slot)]?.type ?? 'default'
}

/** 可选性：优先看剩余量，其次 statusMap.selectable，缺省仅 available 可选 */
function isSelectable(slot: TimeSlot): boolean {
  if (slot.remaining !== undefined && slot.remaining <= 0) return false
  const meta = mergedStatusMap.value[statusKey(slot)]
  if (meta?.selectable !== undefined) return meta.selectable
  return statusKey(slot) === 'available'
}

function labelOf(slot: TimeSlot): string {
  if (slot.label) return slot.label
  if (slot.start && slot.end) return `${slot.start} - ${slot.end}`
  return slot.start ?? slot.end ?? String(slot.key)
}

function capacityText(slot: TimeSlot): string {
  if (slot.remaining !== undefined) {
    return `剩余 ${slot.remaining}${slot.capacity !== undefined ? ` / ${slot.capacity}` : ''}`
  }
  return slot.capacity !== undefined ? `容量 ${slot.capacity}` : '容量待定'
}

function select(slot: TimeSlot) {
  if (props.readonly || !isSelectable(slot)) {
    emit('disabledClick', slot)
    return
  }
  emit('update:modelValue', slot.key)
  emit('select', slot)
}

defineExpose({ groups })
</script>

<style scoped>
.time-slot-grid {
  display: grid;
  gap: 12px;
}

.time-slot-grid__group {
  display: grid;
  gap: 8px;
}

.time-slot-grid__group-header {
  font-weight: 600;
  color: var(--el-text-color-primary);
}

.time-slot-grid__items {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(132px, 1fr));
  gap: 8px;
}

.time-slot-grid__item {
  display: grid;
  gap: 4px;
  min-height: 82px;
  padding: 10px 12px;
  color: var(--el-text-color-regular);
  text-align: left;
  cursor: pointer;
  background: var(--el-bg-color);
  border: 1px solid var(--el-border-color-light);
  border-radius: 8px;
  transition:
    border-color 0.15s ease,
    box-shadow 0.15s ease,
    background-color 0.15s ease;
}

.time-slot-grid__item:hover:not(:disabled):not(.is-disabled):not(.is-readonly) {
  border-color: var(--el-color-primary-light-5);
  box-shadow: 0 2px 8px rgb(0 0 0 / 6%);
}

.time-slot-grid__item.is-selected {
  border-color: var(--el-color-primary);
  background: var(--el-color-primary-light-9);
}

.time-slot-grid__time {
  font-size: 15px;
  font-weight: 600;
  line-height: 1.3;
  color: var(--el-text-color-primary);
}

.time-slot-grid__status {
  margin-top: auto;
  color: var(--el-color-success);
}

.time-slot-grid__item.is-full .time-slot-grid__status,
.time-slot-grid__item.is-danger .time-slot-grid__status {
  color: var(--el-color-danger);
}

.time-slot-grid__item.is-warning .time-slot-grid__status,
.time-slot-grid__item.is-stopped .time-slot-grid__status {
  color: var(--el-color-warning);
}

.time-slot-grid__item.is-info .time-slot-grid__status {
  color: var(--el-color-info);
}

.time-slot-grid__item.is-disabled {
  cursor: not-allowed;
  opacity: 0.6;
}

.time-slot-grid__capacity {
  font-size: 12px;
  color: var(--el-text-color-secondary);
}

.time-slot-grid__state {
  padding: 22px;
  color: var(--el-text-color-secondary);
  text-align: center;
}
</style>
