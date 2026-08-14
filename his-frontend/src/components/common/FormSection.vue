<template>
  <section class="form-section">
    <div v-if="$slots.filter" class="form-section__filter">
      <slot name="filter" />
    </div>

    <div
      v-loading="loading"
      class="form-section__table"
      :style="{ minHeight: normalizedMinTableHeight }"
    >
      <slot name="table">
        <slot />
      </slot>
    </div>
  </section>
</template>

<script setup lang="ts">
import { computed } from 'vue'

interface FormSectionProps {
  loading?: boolean
  minTableHeight?: string | number
}

const props = withDefaults(defineProps<FormSectionProps>(), {
  loading: false,
  minTableHeight: 240
})

const normalizedMinTableHeight = computed(() =>
  typeof props.minTableHeight === 'number' ? `${props.minTableHeight}px` : props.minTableHeight
)
</script>

<style scoped>
.form-section {
  display: flex;
  flex: 1;
  flex-direction: column;
  gap: 12px;
  box-sizing: border-box;
  min-width: 0;
  min-height: 0;
  overflow: hidden;
}

.form-section__filter {
  flex: 0 0 auto;
  min-width: 0;
}

.form-section__table {
  position: relative;
  flex: 1 1 auto;
  min-width: 0;
  overflow: auto;
}
</style>
