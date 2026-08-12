<script setup lang="ts">
import { computed, ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { storeToRefs } from 'pinia'
import { Search } from '@element-plus/icons-vue'
import * as ElIcons from '@element-plus/icons-vue'
import type { WorkbenchAction, WorkbenchData } from '@/apis/workbench'
import { loadWorkbenchData } from '@/apis/workbench'
import { useUserStore } from '@/stores/user'

defineOptions({ name: 'WorkbenchHome' })

const router = useRouter()
const userStore = useUserStore()
const { user } = storeToRefs(userStore)
const searchKeyword = ref('')
const loading = ref(false)
const data = ref<WorkbenchData>({
  notices: [],
  actions: [],
  statistics: []
})

const filteredGroups = computed(() => {
  const kw = (searchKeyword.value || '').trim().toLowerCase()
  const list = kw
    ? data.value.actions.filter((a) => a.title.toLowerCase().includes(kw) || a.group.toLowerCase().includes(kw))
    : data.value.actions
  const groups = new Map<string, WorkbenchAction[]>()
  for (const item of list) {
    if (!groups.has(item.group)) groups.set(item.group, [])
    groups.get(item.group)!.push(item)
  }
  return Array.from(groups.entries()).map(([groupName, items]) => ({
    name: groupName,
    items
  }))
})

async function loadData() {
  if (loading.value) return
  loading.value = true
  try {
    data.value = await loadWorkbenchData(user.value?.roles ?? [])
  } finally {
    loading.value = false
  }
}

function handleNavigate(path: string) {
  if (!path) return
  router.push(path)
}

function getIconComponent(iconName: string) {
  return (ElIcons as Record<string, unknown>)[iconName] || ElIcons.Menu
}

function getStatTrendClass(trendType?: string) {
  if (trendType === 'up') return 'stat-trend--up'
  if (trendType === 'down') return 'stat-trend--down'
  return 'stat-trend--flat'
}

function getNoticeTagType(level: string): 'info' | 'warning' | 'danger' | 'success' {
  const map: Record<string, 'info' | 'warning' | 'danger' | 'success'> = {
    info: 'info',
    warning: 'warning',
    danger: 'danger',
    success: 'success'
  }
  return map[level] || 'info'
}

onMounted(() => {
  loadData()
})
</script>

<template>
  <div class="workbench-home">
    <div class="workbench-home__toolbar">
      <el-input
        v-model="searchKeyword"
        class="workbench-home__search"
        clearable
        placeholder="搜索菜单"
        :prefix-icon="Search"
      />
    </div>

    <div class="workbench-home__main">
      <div v-loading="loading" class="workbench-home__left">
        <div class="stat-grid">
          <div v-for="stat in data.statistics" :key="stat.key" class="stat-card">
            <div class="stat-card__label">{{ stat.label }}</div>
            <div class="stat-card__value">{{ stat.value }}</div>
            <div :class="['stat-trend', getStatTrendClass(stat.trendType)]">
              {{ stat.trend || '' }}
            </div>
          </div>
        </div>

        <div v-if="filteredGroups.length > 0" class="groups">
          <div v-for="group in filteredGroups" :key="group.name" class="group">
            <div class="group__title">{{ group.name }}</div>
            <div class="group__grid">
              <div
                v-for="action in group.items"
                :key="action.id"
                class="shortcut-card"
                @click="handleNavigate(action.path)"
              >
                <div class="shortcut-card__icon">
                  <el-icon :size="24">
                    <component :is="getIconComponent(action.icon)" />
                  </el-icon>
                </div>
                <div class="shortcut-card__title">{{ action.title }}</div>
              </div>
            </div>
          </div>
        </div>

        <el-empty
          v-else-if="!loading"
          class="workbench-home__empty"
          :description="data.actions.length > 0 ? '未找到匹配的菜单' : '暂无可用快捷入口'"
        />
      </div>

      <div class="workbench-home__right">
        <div class="notice-panel">
          <div class="notice-panel__header">
            <span class="notice-panel__title">通知公告</span>
          </div>
          <div class="notice-panel__body">
            <div v-if="data.notices.length === 0" class="notice-empty">暂无通知</div>
            <div v-for="notice in data.notices" :key="notice.id" class="notice-item">
              <el-tag :type="getNoticeTagType(notice.level)" size="small" effect="light">
                {{ notice.level === 'info' ? '信息' : notice.level === 'warning' ? '警告' : notice.level === 'danger' ? '危险' : '成功' }}
              </el-tag>
              <span class="notice-item__title">{{ notice.title }}</span>
              <span class="notice-item__time">{{ notice.createdAt }}</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.workbench-home {
  display: flex;
  flex-direction: column;
  height: 100%;
  padding: 20px 24px;
  gap: 20px;
  overflow-y: auto;
}

.workbench-home__toolbar {
  flex-shrink: 0;
}

.workbench-home__search {
  width: min(360px, 100%);
}

.workbench-home__main {
  display: flex;
  gap: 20px;
  flex: 1;
  min-height: 0;
}

.workbench-home__left {
  flex: 1;
  min-width: 0;
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.workbench-home__right {
  width: 320px;
  flex-shrink: 0;
}

.stat-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
  gap: 16px;
}

.stat-card {
  background: #fff;
  border-radius: 8px;
  padding: 16px 20px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
}

.stat-card__label {
  font-size: 13px;
  color: #909399;
  margin-bottom: 8px;
}

.stat-card__value {
  font-size: 28px;
  font-weight: 600;
  color: var(--el-text-color-primary);
}

.stat-trend {
  font-size: 12px;
  margin-top: 6px;
}

.stat-trend--up {
  color: #67c23a;
}

.stat-trend--down {
  color: #f56c6c;
}

.stat-trend--flat {
  color: #909399;
}

.groups {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.group__title {
  font-size: 15px;
  font-weight: 600;
  color: var(--el-text-color-primary);
  margin-bottom: 12px;
  padding-left: 8px;
  border-left: 3px solid var(--el-color-primary);
}

.group__grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(120px, 1fr));
  gap: 12px;
}

.shortcut-card {
  background: #fff;
  border-radius: 8px;
  padding: 16px 12px;
  text-align: center;
  cursor: pointer;
  transition: box-shadow 0.2s, transform 0.15s;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
}

.shortcut-card:hover {
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.1);
  transform: translateY(-2px);
}

.shortcut-card__icon {
  color: var(--el-color-primary);
  margin-bottom: 8px;
}

.shortcut-card__title {
  font-size: 13px;
  color: var(--el-text-color-regular);
}

.notice-panel {
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
  height: 100%;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.notice-panel__header {
  padding: 14px 16px;
  border-bottom: 1px solid #ebeef5;
}

.notice-panel__title {
  font-size: 15px;
  font-weight: 600;
  color: var(--el-text-color-primary);
}

.notice-panel__body {
  flex: 1;
  overflow-y: auto;
  padding: 8px 0;
}

.notice-empty {
  padding: 20px;
  text-align: center;
  color: #909399;
  font-size: 13px;
}

.notice-item {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 10px 16px;
  border-bottom: 1px solid #f5f5f5;
}

.notice-item:last-child {
  border-bottom: none;
}

.notice-item__title {
  flex: 1;
  font-size: 13px;
  color: var(--el-text-color-regular);
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.notice-item__time {
  font-size: 12px;
  color: #c0c4cc;
  flex-shrink: 0;
}

.workbench-home__empty {
  flex: 1;
  padding: 48px 0;
}
</style>
