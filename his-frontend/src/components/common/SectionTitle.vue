<script setup lang="ts">
defineOptions({
  name: 'SectionTitle'
})

interface SectionTitleProps {
  title: string
  description?: string
  dense?: boolean
  bordered?: boolean
  level?: 2 | 3 | 4 | 5 | 6
}

withDefaults(defineProps<SectionTitleProps>(), {
  description: '',
  dense: false,
  bordered: false,
  level: 3
})
</script>

<template>
  <header
    class="section-title"
    :class="{
      'section-title--dense': dense,
      'section-title--bordered': bordered
    }"
  >
    <div class="section-title__main">
      <span class="section-title__indicator" aria-hidden="true" />

      <slot name="icon" />

      <div class="section-title__content">
        <component :is="`h${level}`" class="section-title__heading">
          <slot>{{ title }}</slot>
        </component>

        <p v-if="description || $slots.description" class="section-title__description">
          <slot name="description">{{ description }}</slot>
        </p>
      </div>
    </div>

    <div v-if="$slots.actions" class="section-title__actions">
      <slot name="actions" />
    </div>
  </header>
</template>

<style scoped>
.section-title {
  display: flex;
  min-width: 0;
  align-items: flex-start;
  justify-content: space-between;
  gap: 16px;
  padding: 12px 0;
}

.section-title--dense {
  gap: 10px;
  padding: 6px 0;
}

.section-title--bordered {
  border-bottom: 1px solid var(--el-border-color-lighter);
}

.section-title__main {
  display: flex;
  min-width: 0;
  align-items: flex-start;
  gap: 8px;
}

.section-title__indicator {
  width: 3px;
  height: 16px;
  flex: none;
  margin-top: 2px;
  border-radius: 2px;
  background: var(--el-color-primary);
}

.section-title--dense .section-title__indicator {
  height: 14px;
}

.section-title__content {
  min-width: 0;
}

.section-title__heading {
  overflow: hidden;
  margin: 0;
  color: var(--el-text-color-primary);
  font-size: 16px;
  font-weight: 600;
  line-height: 20px;
  overflow-wrap: anywhere;
}

.section-title--dense .section-title__heading {
  font-size: 14px;
  line-height: 18px;
}

.section-title__description {
  margin: 4px 0 0;
  color: var(--el-text-color-secondary);
  font-size: 13px;
  line-height: 18px;
}

.section-title__actions {
  display: flex;
  flex: none;
  flex-wrap: wrap;
  align-items: center;
  justify-content: flex-end;
  gap: 8px;
}
</style>

