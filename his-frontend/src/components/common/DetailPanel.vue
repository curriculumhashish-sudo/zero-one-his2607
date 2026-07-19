<template>
  <section class="detail-panel" :aria-busy="loading">
    <header class="detail-panel__header">
      <div class="detail-panel__title">
        <slot name="title">
          {{ title }}
        </slot>
      </div>

      <div class="detail-panel__actions">
        <slot name="actions" :readonly="readonly">
          <el-button v-if="readonly && !empty" type="primary" @click="emit('edit')">
            编辑
          </el-button>
        </slot>
      </div>
    </header>

    <main v-loading="loading" class="detail-panel__body">
      <div class="detail-panel__content">
        <el-empty v-if="empty && !loading" :description="emptyDescription" />
        <slot v-else-if="!loading" :readonly="readonly" />
      </div>

      <footer
        v-if="$slots.footer || (!readonly && !empty)"
        class="detail-panel__footer"
        :class="{ 'is-sticky': stickyFooter }"
      >
        <slot name="footer" :readonly="readonly">
          <el-button @click="emit('cancel')">取消</el-button>
          <el-button type="primary" @click="emit('save')">保存</el-button>
        </slot>
      </footer>
    </main>
  </section>
</template>

<script setup lang="ts">
interface DetailPanelProps {
  title?: string
  loading?: boolean
  readonly?: boolean
  empty?: boolean
  emptyDescription?: string
  stickyFooter?: boolean
}

withDefaults(defineProps<DetailPanelProps>(), {
  title: '',
  loading: false,
  readonly: true,
  empty: false,
  emptyDescription: '请选择一条记录',
  stickyFooter: true
})

const emit = defineEmits<{
  save: []
  cancel: []
  edit: []
}>()
</script>

<style scoped>
.detail-panel {
  display: flex;
  flex: 1;
  flex-direction: column;
  box-sizing: border-box;
  height: 100%;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
  border: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.detail-panel__header {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  justify-content: space-between;
  min-height: 48px;
  padding: 0 16px;
  border-bottom: 1px solid var(--el-border-color-light);
}

.detail-panel__title {
  min-width: 0;
  overflow: hidden;
  font-size: 16px;
  font-weight: 600;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.detail-panel__actions {
  display: flex;
  flex-shrink: 0;
  align-items: center;
  gap: 8px;
}

.detail-panel__body {
  position: relative;
  display: flex;
  flex: 1;
  flex-direction: column;
  min-height: 0;
  overflow: auto;
}

.detail-panel__content {
  flex: 1 0 auto;
  padding: 16px;
}

.detail-panel__footer {
  z-index: 1;
  display: flex;
  flex: 0 0 auto;
  justify-content: flex-end;
  gap: 8px;
  padding: 12px 16px;
  border-top: 1px solid var(--el-border-color-light);
  background: var(--el-bg-color);
}

.detail-panel__footer.is-sticky {
  position: sticky;
  bottom: 0;
}
</style>
