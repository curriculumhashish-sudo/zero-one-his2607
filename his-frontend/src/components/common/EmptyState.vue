<script setup lang="ts">
/**
 * EmptyState —— 统一空状态展示
 * 用于「暂无数据 / 暂无单据 / 暂无模板」等场景，统一图标、文案与操作区。
 * 纯展示组件：不含业务逻辑，操作通过事件回调给父组件；是否显示由父组件控制。
 */
withDefaults(
  defineProps<{
    title?: string // 主标题
    description?: string // 补充说明，空则不渲染
    /** 自定义图片 URL；不传则使用内置插画 */
    image?: string
    compact?: boolean // 紧凑模式，适合嵌入表格 / 侧栏
    actionText?: string // 主操作按钮文案，空则不显示该按钮
    showRefresh?: boolean // 是否显示「刷新」按钮
  }>(),
  {
    title: '暂无数据',
    description: '',
    image: '',
    compact: false,
    actionText: '',
    showRefresh: false,
  },
)

const emit = defineEmits<{
  action: [] // 点击主操作按钮
  refresh: [] // 点击刷新按钮
}>()

defineSlots<{
  /** 替换默认图片 / 插画 */
  image?: () => unknown
  /** 替换整个操作区 */
  action?: () => unknown
  /** 说明文字下方追加内容 */
  default?: () => unknown
}>()
</script>

<template>
  <div class="empty-state" :class="{ 'is-compact': compact }">
    <!-- 图片 / 插画区：优先使用 image 插槽，其次自定义图片，最后内置 SVG 插画 -->
    <div class="empty-state__image">
      <slot name="image">
        <img v-if="image" :src="image" alt="" class="empty-state__img" />
        <svg
          v-else
          class="empty-state__illustration"
          viewBox="0 0 64 64"
          fill="none"
          aria-hidden="true"
        >
          <rect x="10" y="18" width="44" height="30" rx="3" fill="#f0f2f5" stroke="#dcdfe6" />
          <path d="M10 26h44" stroke="#dcdfe6" />
          <rect x="16" y="32" width="20" height="3" rx="1.5" fill="#e0e3e9" />
          <rect x="16" y="39" width="12" height="3" rx="1.5" fill="#e0e3e9" />
          <circle cx="46" cy="44" r="12" fill="#fff" stroke="#dcdfe6" />
          <path d="M42 44h8M46 40v8" stroke="#c0c4cc" stroke-linecap="round" />
        </svg>
      </slot>
    </div>

    <!-- 标题与说明（均可为空时不渲染） -->
    <p v-if="title" class="empty-state__title">{{ title }}</p>
    <p v-if="description" class="empty-state__description">{{ description }}</p>

    <!-- 默认插槽：说明下方的追加内容 -->
    <slot />

    <!-- 操作区：无 action 插槽 / actionText / showRefresh 时整块不渲染，避免留白 -->
    <div v-if="$slots.action || actionText || showRefresh" class="empty-state__actions">
      <slot name="action">
        <button
          v-if="actionText"
          type="button"
          class="empty-state__btn empty-state__btn--primary"
          @click="emit('action')"
        >
          {{ actionText }}
        </button>
        <button
          v-if="showRefresh"
          type="button"
          class="empty-state__btn"
          @click="emit('refresh')"
        >
          刷新
        </button>
      </slot>
    </div>
  </div>
</template>

<style scoped>
.empty-state {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 40px 16px;
  color: #909399;
  text-align: center;
}

/* 紧凑模式：整体缩小内边距与图标尺寸 */
.empty-state.is-compact {
  padding: 16px 12px;
}

.empty-state__image {
  margin-bottom: 12px;
}

.empty-state.is-compact .empty-state__image {
  margin-bottom: 8px;
}

.empty-state__illustration {
  width: 96px;
  height: 96px;
}

.empty-state.is-compact .empty-state__illustration {
  width: 56px;
  height: 56px;
}

.empty-state__img {
  max-width: 160px;
  max-height: 160px;
  object-fit: contain;
}

.empty-state.is-compact .empty-state__img {
  max-width: 80px;
  max-height: 80px;
}

.empty-state__title {
  margin: 0;
  font-size: 14px;
  color: #606266;
}

.empty-state.is-compact .empty-state__title {
  font-size: 13px;
}

.empty-state__description {
  margin: 6px 0 0;
  font-size: 12px;
  color: #909399;
  line-height: 1.5;
}

/* 操作按钮区 */
.empty-state__actions {
  display: flex;
  gap: 8px;
  margin-top: 16px;
}

.empty-state.is-compact .empty-state__actions {
  margin-top: 10px;
}

.empty-state__btn {
  height: 32px;
  padding: 0 16px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  background: #fff;
  color: #606266;
  font-size: 14px;
  cursor: pointer;
}

.empty-state__btn:hover {
  color: #409eff;
  border-color: #c6e2ff;
}

/* 主操作按钮（高亮） */
.empty-state__btn--primary {
  background: #409eff;
  border-color: #409eff;
  color: #fff;
}

.empty-state__btn--primary:hover {
  background: #66b1ff;
  border-color: #66b1ff;
  color: #fff;
}
</style>
