<script setup lang="ts">
import type { EChartsOption } from 'echarts'

import { nextTick, onBeforeUnmount, onMounted, ref, watch } from 'vue'
import { Refresh } from '@element-plus/icons-vue'
import * as echarts from 'echarts'

const props = withDefaults(defineProps<{
  title: string
  option?: EChartsOption
  loading?: boolean
  error?: string
  empty?: boolean
}>(), { loading: false, error: '', empty: false })
const emit = defineEmits<{
  retry: []
  select: [payload: { name: string; value: unknown }]
}>()
const chartRef = ref<HTMLElement>()
let chart: echarts.ECharts | undefined
let observer: ResizeObserver | undefined

function render() {
  if (!chart || !props.option || props.empty || props.error) return
  chart.setOption(props.option, true)
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
watch(() => props.option, render, { deep: true })
onBeforeUnmount(() => {
  observer?.disconnect()
  chart?.dispose()
})
</script>

<template>
  <section class="chart-card">
    <div class="chart-card__header"><h3>{{ title }}</h3><span v-if="loading" class="chart-card__status">加载中…</span></div>
    <div v-if="error" class="chart-card__state chart-card__state--error"><span>{{ error }}</span><el-button link type="primary" :icon="Refresh" @click="emit('retry')">重试</el-button></div>
    <div v-else-if="empty && !loading" class="chart-card__state">暂无数据</div>
    <div v-else ref="chartRef" class="chart-card__canvas" v-loading="loading" />
  </section>
</template>

<style scoped>
.chart-card { min-height: 300px; padding: 16px; border: 1px solid #e5e7eb; border-radius: 10px; background: #fff; box-shadow: 0 2px 10px rgb(15 23 42 / 5%); }
.chart-card__header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 8px; }
.chart-card__header h3 { margin: 0; color: #1f2937; font-size: 16px; }
.chart-card__status { color: #6b7280; font-size: 12px; }
.chart-card__canvas { width: 100%; height: 250px; }
.chart-card__state { display: flex; height: 250px; align-items: center; justify-content: center; gap: 12px; color: #6b7280; }
.chart-card__state--error { flex-direction: column; color: #b91c1c; }
</style>
