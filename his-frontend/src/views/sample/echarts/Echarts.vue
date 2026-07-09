<template>
  <!-- 为 ECharts 准备一个 DOM 用于渲染图表 -->
  <div ref="chartRef" class="chart-container"></div>
</template>
<script setup lang="ts">
import { onMounted, ref, onUnmounted, nextTick } from 'vue'
// 引入 echarts 核心模块，核心模块提供了 echarts 使用必须要的接口。
import * as echarts from 'echarts/core'
// 引入柱状图图表，图表后缀都为 Chart
import { BarChart } from 'echarts/charts'
// 引入标题，提示框，直角坐标系，数据集，内置数据转换器组件，组件后缀都为 Component
import {
  TitleComponent,
  TooltipComponent,
  GridComponent,
  DatasetComponent,
  TransformComponent
} from 'echarts/components'
// 标签自动布局、全局过渡动画等特性
import { LabelLayout, UniversalTransition } from 'echarts/features'
// 引入 Canvas 渲染器，注意引入 CanvasRenderer 或者 SVGRenderer 是必须的一步
import { CanvasRenderer } from 'echarts/renderers'
// 引入 dark 主题
import 'echarts/theme/dark'
// 注册必须的组件
echarts.use([
  TitleComponent,
  TooltipComponent,
  GridComponent,
  DatasetComponent,
  TransformComponent,
  BarChart,
  LabelLayout,
  UniversalTransition,
  CanvasRenderer
])

const chartRef = ref<HTMLElement | null>(null)
let myChart: echarts.ECharts | null = null

// 处理窗口缩放
const handleResize = () => {
  myChart?.resize()
}

// 在 mounted 中初始化图表
onMounted(async () => {
  // 确保 DOM 已经完全渲染并具有宽高
  await nextTick()

  if (!chartRef.value) return

  // 初始化图表
  myChart = echarts.init(chartRef.value, 'dark')

  // 设置配置项
  myChart.setOption({
    title: {
      text: 'ECharts 入门示例',
      textStyle: {
        color: '#fff' // 标题颜色
      }
    },
    tooltip: {
      trigger: 'axis'
    },
    legend: {
      textStyle: {
        color: '#ccc'
      }
    },
    grid: {
      left: '3%',
      right: '4%',
      bottom: '3%',
      containLabel: true
    },
    xAxis: {
      data: ['衬衫', '羊毛衫', '雪纺衫', '裤子', '高跟鞋', '袜子'],
      axisLabel: {
        color: '#ccc', // X 轴文字颜色
        fontSize: 14
      },
      axisLine: {
        lineStyle: {
          color: '#555' // X 轴线颜色
        }
      }
    },
    yAxis: {
      axisLabel: {
        color: '#ccc', // Y 轴文字颜色
        fontSize: 14
      },
      splitLine: {
        lineStyle: {
          color: '#333' // 网格线颜色
        }
      }
    },
    series: [
      {
        name: '1月销量',
        type: 'bar',
        data: [5, 20, 36, 10, 10, 20],
        itemStyle: {
          borderRadius: [4, 4, 0, 0]
        }
      },
      {
        name: '2月销量',
        type: 'bar',
        data: [15, 15, 30, 16, 19, 11],
        itemStyle: {
          borderRadius: [4, 4, 0, 0]
        }
      }
    ]
  })

  // 监听窗口缩放
  window.addEventListener('resize', handleResize)
})

onUnmounted(() => {
  // 销毁实例
  myChart?.dispose()
  window.removeEventListener('resize', handleResize)
})
</script>

<style scoped>
.chart-container {
  width: 100%;
  height: 400px;
  min-height: 300px;
}
</style>
