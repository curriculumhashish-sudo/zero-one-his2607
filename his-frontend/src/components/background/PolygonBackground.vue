<template>
  <canvas ref="canvasRef" class="polygon-background"></canvas>
</template>

<script setup lang="ts">
/**
 * PolygonBackground 组件
 * 提供一个动态的多边形背景，通过 Canvas 绘制并随窗口调整大小。
 */
import { ref, onMounted, onUnmounted } from 'vue'

// Canvas 元素的引用
const canvasRef = ref<HTMLCanvasElement | null>(null)

/**
 * 多边形接口定义
 */
interface Polygon {
  points: { x: number; y: number }[] // 多边形的顶点坐标数组
  color: string // 填充颜色
  opacity: number // 透明度
  speedX: number // X 轴移动速度
  speedY: number // Y 轴移动速度
}

// 存储所有多边形的响应式数组
const polygons = ref<Polygon[]>([])
// 多边形的总数
const polygonCount = 15
// 动画帧 ID，用于在卸载时取消动画
let animationFrameId: number

/**
 * 初始化多边形数据
 * @param width 画布宽度
 * @param height 画布高度
 */
const initPolygons = (width: number, height: number) => {
  const newPolygons: Polygon[] = []
  const colors = [
    'rgba(173, 216, 230, 0.3)', // LightBlue
    'rgba(224, 255, 255, 0.3)', // LightCyan
    'rgba(240, 248, 255, 0.3)', // AliceBlue
    'rgba(230, 230, 250, 0.3)', // Lavender
    'rgba(255, 240, 245, 0.3)' // LavenderBlush
  ]

  for (let i = 0; i < polygonCount; i++) {
    const sideCount = Math.floor(Math.random() * 3) + 3 // 随机生成 3 到 5 条边
    const radius = Math.random() * 100 + 50 // 随机半径
    const centerX = Math.random() * width // 随机中心 X 坐标
    const centerY = Math.random() * height // 随机中心 Y 坐标
    const points: { x: number; y: number }[] = []

    // 生成多边形的各个顶点
    for (let j = 0; j < sideCount; j++) {
      const angle = (j / sideCount) * Math.PI * 2
      points.push({
        x: centerX + Math.cos(angle) * radius,
        y: centerY + Math.sin(angle) * radius
      })
    }

    newPolygons.push({
      points,
      color: colors[Math.floor(Math.random() * colors.length)]!,
      opacity: Math.random() * 0.5 + 0.1,
      speedX: (Math.random() - 0.5) * 0.5,
      speedY: (Math.random() - 0.5) * 0.5
    })
  }
  polygons.value = newPolygons
}

/**
 * 绘制循环函数
 * @param ctx Canvas 绘图上下文
 * @param width 画布宽度
 * @param height 画布高度
 */
const draw = (ctx: CanvasRenderingContext2D, width: number, height: number) => {
  // 清除上一帧内容
  ctx.clearRect(0, 0, width, height)

  polygons.value.forEach((poly) => {
    if (poly.points.length === 0) return
    const firstPoint = poly.points[0]!

    // 开始绘制路径
    ctx.beginPath()
    ctx.moveTo(firstPoint.x, firstPoint.y)
    for (let i = 1; i < poly.points.length; i++) {
      const point = poly.points[i]!
      ctx.lineTo(point.x, point.y)
    }
    ctx.closePath()
    ctx.fillStyle = poly.color
    ctx.fill()

    // 更新位置并检查是否需要反弹
    let needsBounceX = false
    let needsBounceY = false

    poly.points.forEach((p) => {
      p.x += poly.speedX
      p.y += poly.speedY

      // 如果超出边界，设置反弹标记
      if (p.x < -100 || p.x > width + 100) needsBounceX = true
      if (p.y < -100 || p.y > height + 100) needsBounceY = true
    })

    // 执行反弹（改变速度方向）
    if (needsBounceX) poly.speedX *= -1
    if (needsBounceY) poly.speedY *= -1
  })

  // 请求下一帧动画
  animationFrameId = requestAnimationFrame(() => draw(ctx, width, height))
}

/**
 * 处理窗口尺寸变化
 * 更新 Canvas 尺寸并重新初始化多边形
 */
const handleResize = () => {
  if (!canvasRef.value) return
  const canvas = canvasRef.value
  canvas.width = window.innerWidth
  canvas.height = window.innerHeight
  initPolygons(canvas.width, canvas.height)
}

onMounted(() => {
  if (!canvasRef.value) return
  const canvas = canvasRef.value
  const ctx = canvas.getContext('2d')
  if (!ctx) return

  handleResize()
  window.addEventListener('resize', handleResize, { passive: true })
  draw(ctx, canvas.width, canvas.height)
})

onUnmounted(() => {
  // 组件卸载时移除监听器并取消动画，防止内存泄漏
  window.removeEventListener('resize', handleResize)
  cancelAnimationFrame(animationFrameId)
})
</script>

<style scoped>
.polygon-background {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  z-index: -1;
  background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
  touch-action: none;
}
</style>
