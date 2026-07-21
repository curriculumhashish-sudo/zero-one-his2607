<template>
  <article class="advice-cell" :class="{ 'is-compact': compact }" @click="emit('click', order)">
    <div class="advice-cell__primary" :title="primaryText"><slot name="name" :order="order">{{ primaryText || '未命名医嘱' }}</slot></div>
    <div v-if="details" class="advice-cell__secondary" :title="details"><slot name="details" :order="order" :details="details">{{ details }}</slot></div>
    <div v-if="showStatus || $slots.tags" class="advice-cell__tags"><slot name="tags" :order="order"><el-tag v-if="showStatus" size="small" effect="plain">{{ statusMap[statusText] ?? statusText ?? '未知状态' }}</el-tag></slot></div>
    <div v-if="showRemark && remarkText" class="advice-cell__warning"><slot name="remark" :order="order">{{ remarkText }}</slot></div>
    <slot name="extra" :order="order" />
  </article>
</template>
<script setup lang="ts">
import { computed } from 'vue'
import type { AdviceOrder } from './type'
const props = withDefaults(defineProps<{ order: AdviceOrder; fields?: Partial<Record<'name' | 'specification' | 'dose' | 'unit' | 'frequency' | 'route' | 'days' | 'remark' | 'status', keyof AdviceOrder>>; compact?: boolean; maxLines?: number; showRemark?: boolean; showStatus?: boolean; statusMap?: Record<string, string> }>(), { fields: () => ({}), compact: false, maxLines: 2, showRemark: true, showStatus: false, statusMap: () => ({}) })
const emit = defineEmits<{ click: [order: AdviceOrder]; action: [order: AdviceOrder, action: string] }>()
function value(name: keyof AdviceOrder) { const key = props.fields[name as keyof typeof props.fields] ?? name; return props.order[key] }
function text(value: unknown) { return value === null || value === undefined || value === '' ? '' : String(value) }
const primaryText = computed(() => text(value('name')))
const statusText = computed(() => text(value('status')))
const remarkText = computed(() => text(value('remark')))
const details = computed(() => { const specification = text(value('specification')); const dose = [text(value('dose')), text(value('unit'))].filter(Boolean).join(''); return [specification, dose, text(value('frequency')), text(value('route')), text(value('days')) ? `${text(value('days'))}天` : ''].filter(Boolean).join(' · ') })
</script>
<style scoped>
.advice-cell { display: grid; gap: 3px; min-width: 0; cursor: pointer; }.advice-cell__primary,.advice-cell__secondary,.advice-cell__warning { overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }.advice-cell__primary { color: var(--el-text-color-primary); font-weight: 600; }.advice-cell__secondary { color: var(--el-text-color-secondary); font-size: 12px; }.advice-cell__warning { color: var(--el-color-warning); font-size: 12px; }.advice-cell__tags { display: flex; gap: 4px; }.advice-cell.is-compact { gap: 1px; }.advice-cell.is-compact .advice-cell__warning { display: none; }
</style>
