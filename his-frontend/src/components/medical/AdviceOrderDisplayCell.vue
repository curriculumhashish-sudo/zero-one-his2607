<template>
  <article class="advice-cell" :class="{ 'is-compact': compact, 'is-group': isGroup }" :style="cellStyle" @click="emit('click', order)">
    <!-- 成组括线版式：groupItems 非空时启用，左侧竖括线拢住多药，底部共用行 -->
    <template v-if="isGroup">
      <div class="advice-cell__group-list">
        <div v-for="(item, index) in groupLines" :key="index" class="advice-cell__group-line">
          <slot name="line" :item="item.raw" :index="index">
            <span v-if="item.title" class="advice-cell__drug-name">{{ item.title }}</span>
            <span v-if="item.dose" class="advice-cell__drug-dose">{{ item.dose }}</span>
          </slot>
        </div>
      </div>
      <div v-if="sharedText || showStatus || $slots.tags" class="advice-cell__group-shared">
        <slot name="shared" :order="order">{{ sharedText }}</slot>
        <slot name="tags" :order="order"><el-tag v-if="showStatus" size="small" effect="plain">{{ statusMap[statusText] ?? statusText ?? '未知状态' }}</el-tag></slot>
      </div>
      <div v-if="showRemark && remarkText" class="advice-cell__warning"><slot name="remark" :order="order">{{ remarkText }}</slot></div>
    </template>

    <!-- 单条医嘱版式（向后兼容原字段映射） -->
    <template v-else>
      <div class="advice-cell__primary" :title="primaryText"><slot name="name" :order="order">{{ primaryText || '未命名医嘱' }}</slot></div>
      <div v-if="details" class="advice-cell__secondary" :title="details"><slot name="details" :order="order" :details="details">{{ details }}</slot></div>
      <div v-if="showStatus || $slots.tags" class="advice-cell__tags"><slot name="tags" :order="order"><el-tag v-if="showStatus" size="small" effect="plain">{{ statusMap[statusText] ?? statusText ?? '未知状态' }}</el-tag></slot></div>
      <div v-if="showRemark && remarkText" class="advice-cell__warning"><slot name="remark" :order="order">{{ remarkText }}</slot></div>
    </template>
    <slot name="extra" :order="order" />
  </article>
</template>
<script setup lang="ts">
import { computed } from 'vue'
import type { AdviceOrder } from './type'
const props = withDefaults(defineProps<{ order: AdviceOrder; fields?: Partial<Record<'name' | 'specification' | 'dose' | 'unit' | 'frequency' | 'route' | 'days' | 'remark' | 'status', keyof AdviceOrder>>; compact?: boolean; maxLines?: number; showRemark?: boolean; showStatus?: boolean; statusMap?: Record<string, string> }>(), { fields: () => ({}), compact: false, maxLines: 2, showRemark: true, showStatus: false, statusMap: () => ({}) })
const emit = defineEmits<{ click: [order: AdviceOrder]; action: [order: AdviceOrder, action: string] }>()
function value(name: keyof AdviceOrder, source: AdviceOrder = props.order) { const key = props.fields[name as keyof typeof props.fields] ?? name; return source[key] }
function text(value: unknown) { return value === null || value === undefined || value === '' ? '' : String(value) }
const primaryText = computed(() => text(value('name')))
const statusText = computed(() => text(value('status')))
const remarkText = computed(() => text(value('remark')))
const details = computed(() => detailOf(props.order))
const cellStyle = computed(() => ({ '--advice-max-lines': String(Math.max(1, props.maxLines)) }))
function detailOf(source: AdviceOrder) { const specification = text(value('specification', source)); const dose = [text(value('dose', source)), text(value('unit', source))].filter(Boolean).join(''); return [specification, dose, text(value('frequency', source)), text(value('route', source)), text(value('days', source)) ? `${text(value('days', source))}天` : ''].filter(Boolean).join(' · ') }

/** 成组：groupItems 非空即启用括线版式 */
const isGroup = computed(() => Array.isArray(props.order.groupItems) && props.order.groupItems.length > 0)

/** 每味药一行：题名「名称【规格】（药房）」+ 剂量「X单位/次，共Y单位」 */
const groupLines = computed(() =>
  (props.order.groupItems ?? []).map((item) => {
    const name = text(value('name', item))
    const spec = text(value('specification', item))
    const pharmacy = text(item.pharmacy)
    let title = name
    if (spec) title += `【${spec}】`
    if (pharmacy) title += `（${pharmacy}）`
    const single = [text(value('dose', item)), text(value('unit', item))].filter(Boolean).join('')
    const dose = single ? `${single}/次` : ''
    return { title, dose, raw: item }
  })
)

/** 组内共用行：用法、频次、天数，取首项，缺省片段自动省略 */
const sharedText = computed(() => {
  const first = props.order.groupItems?.[0]
  if (!first) return ''
  const usage = text(value('route', first))
  const freq = text(value('frequency', first))
  const days = text(value('days', first))
  const dayPart = days ? `（${days}天）` : ''
  if (usage && freq) return `${usage}，${freq}${dayPart}`
  if (usage) return `${usage}${dayPart}`
  if (freq) return `${freq}${dayPart}`
  return dayPart
})
</script>
<style scoped>
.advice-cell { display: grid; gap: 3px; min-width: 0; cursor: pointer; }
.advice-cell__primary,.advice-cell__warning { overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }
.advice-cell__primary { color: var(--el-text-color-primary); font-weight: 600; }
.advice-cell__secondary { display: -webkit-box; overflow: hidden; color: var(--el-text-color-secondary); font-size: 12px; -webkit-box-orient: vertical; -webkit-line-clamp: var(--advice-max-lines); }
.advice-cell__warning { color: var(--el-color-warning); font-size: 12px; }
.advice-cell__tags { display: flex; gap: 4px; }
.advice-cell.is-compact { gap: 1px; }
.advice-cell.is-compact .advice-cell__warning { display: none; }

/* 成组括线版式 */
.advice-cell__group-list { position: relative; padding-left: 12px; padding-bottom: 4px; line-height: 1.5; border-bottom: 1px dashed var(--el-border-color); }
.advice-cell__group-list::before { content: ''; position: absolute; top: 0; bottom: 0; left: 0; width: 0; border-left: 1px solid var(--el-text-color-secondary); }
.advice-cell__group-list::after { content: ''; position: absolute; top: 0; left: 0; width: 10px; border-top: 1px solid var(--el-text-color-secondary); }
.advice-cell__group-line:last-child::after { content: ''; position: absolute; left: 0; bottom: 0; width: 10px; border-bottom: 1px solid var(--el-text-color-secondary); }
.advice-cell__group-line { display: flex; align-items: baseline; justify-content: space-between; gap: 12px; min-width: 0; }
.advice-cell__drug-name { flex: 1 1 auto; min-width: 0; font-size: 14px; font-weight: 500; color: var(--el-text-color-primary); }
.advice-cell__drug-dose { flex: 0 0 auto; font-size: 12px; text-align: right; white-space: nowrap; color: var(--el-text-color-regular); }
.advice-cell__group-shared { display: flex; flex-wrap: wrap; gap: 6px; align-items: center; margin-top: 2px; font-size: 12px; color: var(--el-color-primary); }
</style>
