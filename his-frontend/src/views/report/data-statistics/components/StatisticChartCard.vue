<script setup lang="ts">
import type { EChartsOption } from 'echarts'

import { computed, nextTick, onBeforeUnmount, onMounted, ref, watch } from 'vue'
import { Refresh } from '@element-plus/icons-vue'
import * as echarts from 'echarts'

const props = withDefaults(
  defineProps<{
    title: string
    option?: EChartsOption
    loading?: boolean
    error?: string
    empty?: boolean
    emptyText?: string
  }>(),
  { loading: false, error: '', empty: false, emptyText: '暂无数据' }
)
const emit = defineEmits<{
  retry: []
  select: [payload: { name: string; value: unknown }]
}>()
const chartRef = ref<HTMLElement>()
let chart: echarts.ECharts | undefined
let observer: ResizeObserver | undefined
const hasChart = computed(() => Boolean(props.option) && !props.empty && !props.error)

async function render() {
  await nextTick()
  if (!chart) return
  if (!hasChart.value || !props.option) {
    chart.clear()
    return
  }
  chart.setOption(props.option, true)
  chart.resize()
}

onMounted(async () => {
  await nextTick()
  if (!chartRef.value) return
  chart = echarts.init(chartRef.value)
  chart.on('click', (params) => {
    emit('select', { name: String(params.name ?? ''), value: params.value })
  })
  observer = new ResizeObserver(() => chart?.resize())
  observer.observe(chartRef.value)
  render()
})
watch([() => props.option, () => props.empty, () => props.error], render, { deep: true })
onBeforeUnmount(() => {
  observer?.disconnect()
  chart?.dispose()
})
</script>

<template>
  <section class="chart-card">
    <div class="chart-card__header">
      <h3>{{ title }}</h3>
      <span v-if="loading" class="chart-card__status">加载中…</span>
    </div>
    <div ref="chartRef" v-show="hasChart" class="chart-card__canvas" v-loading="loading" />
    <div v-if="error" class="chart-card__state chart-card__state--error">
      <span>{{ error }}</span
      ><el-button link type="primary" :icon="Refresh" @click="emit('retry')">重试</el-button>
    </div>
    <div v-else-if="!loading && !hasChart" class="chart-card__state">{{ emptyText }}</div>
  </section>
</template>

<style scoped>
.chart-card {
  min-height: 300px;
  padding: 16px;
  border: 1px solid #e5e7eb;
  border-radius: 10px;
  background: #fff;
  box-shadow: 0 2px 10px rgb(15 23 42 / 5%);
}
.chart-card__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: 8px;
}
.chart-card__header h3 {
  margin: 0;
  color: #1f2937;
  font-size: 16px;
}
.chart-card__status {
  color: #6b7280;
  font-size: 12px;
}
.chart-card__canvas {
  width: 100%;
  height: 250px;
}
.chart-card__state {
  display: flex;
  height: 250px;
  align-items: center;
  justify-content: center;
  gap: 12px;
  color: #6b7280;
}
.chart-card__state--error {
  flex-direction: column;
  color: #b91c1c;
}
</style>
