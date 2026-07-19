<template>
  <div class="dashboard-container">
    <!-- 顶部：核心指标 -->
    <el-row :gutter="20" class="stat-row">
      <el-col :xs="24" :sm="12" :md="6" v-for="(item, index) in statCards" :key="index">
        <StatCard v-bind="item" :delay="index * 100" />
      </el-col>
    </el-row>
    <!-- 中部：常用功能 + 项目信息 + 系统动态 -->
    <el-row :gutter="20" class="content-row">
      <el-col :md="16" class="content-col-left">
        <FeatureGrid :items="quickLinks" />
        <ProjectInfo />
      </el-col>
      <el-col :md="8" class="content-col-right">
        <SystemTimeline :items="activities" />
      </el-col>
    </el-row>
    <!-- 底部：资源监控 -->
    <el-row :gutter="16" class="bottom-row">
      <el-col :span="24">
        <ResourceMonitor :items="resources" />
      </el-col>
    </el-row>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import {
  User,
  Monitor,
  Connection,
  ChatLineRound,
  Setting,
  PieChart,
  Files,
  Document,
  Collection,
  Box,
  Lock,
  Cpu,
  Odometer,
  DataLine
} from '@element-plus/icons-vue'

// Dashboard子组件
import StatCard from '@/components/dashboard/StatCard.vue'
import FeatureGrid from '@/components/dashboard/FeatureGrid.vue'
import ProjectInfo from '@/components/dashboard/ProjectInfo.vue'
import SystemTimeline from '@/components/dashboard/SystemTimeline.vue'
import ResourceMonitor from '@/components/dashboard/ResourceMonitor.vue'
import type { Activity, FeatureItem, ResourceItem, StatCardItem } from '@/components/dashboard/type'

// 顶部核心指标
const statCards = ref<StatCardItem[]>([
  {
    label: '在线用户',
    value: 1284,
    displayValue: 0,
    unit: '',
    trendValue: '12%',
    trendType: 'up',
    icon: User,
    color: '#409eff',
    bgColor: '#ecf5ff'
  },
  {
    label: '系统负载',
    value: 24.5,
    displayValue: 0,
    unit: '%',
    trendValue: '5%',
    trendType: 'down',
    icon: Monitor,
    color: '#67c23a',
    bgColor: '#f0f9eb'
  },
  {
    label: 'API 调用',
    value: 85420,
    displayValue: 0,
    unit: '',
    trendValue: '18%',
    trendType: 'up',
    icon: Connection,
    color: '#e6a23c',
    bgColor: '#fdf6ec'
  },
  {
    label: '待处理反馈',
    value: 12,
    displayValue: 0,
    unit: '',
    trendValue: '2%',
    trendType: 'up',
    icon: ChatLineRound,
    color: '#f56c6c',
    bgColor: '#fef0f0'
  }
])

// 底部资源监控
const resources = ref<ResourceItem[]>([
  { name: 'CPU', percentage: 28, displayValue: 0, color: '#409eff', icon: Cpu },
  { name: '内存', percentage: 45, displayValue: 0, color: '#67c23a', icon: Odometer },
  { name: '磁盘', percentage: 72, displayValue: 0, color: '#e6a23c', icon: DataLine },
  { name: '网络', percentage: 15, displayValue: 0, color: '#f56c6c', icon: PieChart }
])

/**
 * 以requestAnimationFrame驱动的数字增长动画（用于统计数值/进度条的）
 * @param target 目标值
 * @param currentRef 需要被更新的引用对象（形如 { value: number }）
 * @param duration 动画时长(ms)
 */
const animateNumber = (target: number, currentRef: { value: number }, duration: number = 1000) => {
  const start = 0
  const startTime = performance.now()

  const update = (now: number) => {
    const elapsed = now - startTime
    const progress = Math.min(elapsed / duration, 1)

    const easeProgress = progress === 1 ? 1 : 1 - Math.pow(2, -10 * progress)

    currentRef.value = start + (target - start) * easeProgress

    if (progress < 1) {
      requestAnimationFrame(update)
    } else {
      currentRef.value = target
    }
  }

  requestAnimationFrame(update)
}

onMounted(() => {
  // 核心指标数字：逐项错开增长动画
  statCards.value.forEach((card, index) => {
    const wrapper = {
      get value() {
        return card.displayValue
      },
      set value(v) {
        card.displayValue = v
      }
    }
    setTimeout(() => {
      animateNumber(card.value, wrapper, 1500)
    }, index * 100)
  })

  // 资源进度条：逐项错开增长动画
  resources.value.forEach((res, index) => {
    const wrapper = {
      get value() {
        return res.displayValue
      },
      set value(v) {
        res.displayValue = v
      }
    }
    setTimeout(
      () => {
        animateNumber(res.percentage, wrapper, 800)
      },
      100 + index * 50
    )
  })
})

// 常用功能入口
const quickLinks: FeatureItem[] = [
  {
    name: '报表统计',
    desc: '系统数据可视化分析',
    icon: PieChart,
    path: '/sample/echarts',
    bgColor: '#409eff'
  },
  {
    name: '文件中心',
    desc: '资源上传与分发管理',
    icon: Files,
    path: '/sample/upload',
    bgColor: '#67c23a'
  },
  {
    name: '内容编辑',
    desc: '富文本内容创作平台',
    icon: Document,
    path: '/sample/editor',
    bgColor: '#e6a23c'
  },
  {
    name: '工作流引擎',
    desc: '业务流程审批与建模',
    icon: Connection,
    path: '/sample/flow',
    bgColor: '#f56c6c'
  },
  {
    name: '表单设计',
    desc: '动态表单配置工具',
    icon: Collection,
    path: '/sample/form',
    bgColor: '#7232dd'
  },
  {
    name: '组件示例',
    desc: '基础与业务组件演示',
    icon: Box,
    path: '/sample/common',
    bgColor: '#07c160',
    navigable: true
  },
  {
    name: '系统设置',
    desc: '全局参数与功能配置',
    icon: Setting,
    path: '/home',
    bgColor: '#1989fa'
  },
  { name: '安全审计', desc: '访问日志与权限监控', icon: Lock, path: '/home', bgColor: '#ee0a24' }
]

// 系统动态数据
const activities: Activity[] = [
  {
    title: '部署提醒',
    content: '生产环境 v2.5.1 版本部署完成',
    time: '10:30',
    type: 'primary',
    color: '#409eff'
  },
  {
    title: '安全告警',
    content: '拦截到来自 192.168.1.105 的暴力破解尝试',
    time: '09:15',
    type: 'danger',
    color: '#f56c6c'
  },
  {
    title: '系统通知',
    content: '数据库自动备份任务执行成功',
    time: '04:00',
    type: 'success',
    color: '#67c23a'
  },
  {
    title: '工单提醒',
    content: '您有 3 个待处理的业务审核申请',
    time: '昨天',
    type: 'warning',
    color: '#e6a23c'
  }
]
</script>

<style scoped>
.dashboard-container {
  padding: 12px 16px;
  background-color: #f6f8fb;
  height: calc(100vh - 104px);
  overflow: hidden;
  box-sizing: border-box;
  display: flex;
  flex-direction: column;
}

.stat-row {
  margin-bottom: 12px;
}

.content-row {
  flex: 1;
  margin-bottom: 12px;
  min-height: 0;
}

.content-col-left,
.content-col-right {
  display: flex;
  flex-direction: column;
}

.bottom-row {
  margin-top: 12px;
}
</style>
