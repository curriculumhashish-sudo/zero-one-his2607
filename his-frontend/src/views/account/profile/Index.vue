<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useRoute } from 'vue-router'
import type { ProfileInfo } from '@/apis/account/profile'
import { getProfile } from '@/apis/account/profile'
import { useUserStore } from '@/stores/user'
import BasicSetting from './components/basic-setting.vue'
import PasswordSetting from './components/password-setting.vue'

defineOptions({ name: 'MyProfile' })

const route = useRoute()
const userStore = useUserStore()
const tabsValue = ref<string>('basic')

const tabs = ref([
  { label: '基本设置', value: 'basic' },
  { label: '修改密码', value: 'password' }
])

const profile = ref<ProfileInfo | null>(null)
const loading = ref(false)

function getLoginTimeFromToken(): string {
  try {
    const token = localStorage.getItem('token') || ''
    if (!token) return ''
    const parts = token.split('.')
    if (parts.length !== 3) return ''
    const payload = JSON.parse(atob(parts[1].replace(/-/g, '+').replace(/_/g, '/')))
    if (payload.iat) {
      const d = new Date(payload.iat * 1000)
      const pad = (n: number) => String(n).padStart(2, '0')
      return `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())} ${pad(d.getHours())}:${pad(d.getMinutes())}:${pad(d.getSeconds())}`
    }
  } catch { /* ignore */ }
  return ''
}

function buildFallbackProfile(): ProfileInfo {
  const u = userStore.user
  return {
    id: u?.id ?? '',
    username: u?.username ?? '',
    name: u?.realName ?? u?.username ?? '',
    avatar: u?.avatar ?? '',
    email: '',
    phone: '',
    gender: '',
    department: '',
    roles: u?.roles ?? [],
    createdAt: getLoginTimeFromToken()
  }
}

async function loadProfile() {
  loading.value = true
  try {
    profile.value = await getProfile()
  } catch {
    profile.value = buildFallbackProfile()
  } finally {
    loading.value = false
  }
}

onMounted(() => {
  const tab = typeof route.query.tab === 'string' ? route.query.tab.trim() : ''
  if (tab && tabs.value.some((item) => item.value === tab)) {
    tabsValue.value = tab
  }
  loadProfile()
})
</script>

<template>
  <div class="profile-page">
    <div v-loading="loading" class="profile-card">
      <div class="profile-header">
        <div class="profile-avatar">
          <el-avatar :size="80" :src="profile?.avatar">
            {{ profile?.name?.charAt(0) || 'U' }}
          </el-avatar>
        </div>
        <div class="profile-info">
          <h2 class="profile-name">{{ profile?.name || profile?.username || '未登录用户' }}</h2>
          <div class="profile-meta">
            <el-tag v-for="r in profile?.roles" :key="r" size="small" style="margin-right: 6px">
              {{ r }}
            </el-tag>
          </div>
          <div class="profile-desc">
            账号：{{ profile?.username || '-' }} · 科室：{{ profile?.department || '-' }} {{ profile?.createdAt ? '· 登录时间 ' + profile.createdAt : '' }}
          </div>
        </div>
      </div>

      <el-tabs v-model="tabsValue" class="profile-tabs">
        <el-tab-pane
          v-for="tab in tabs"
          :key="tab.value"
          :label="tab.label"
          :name="tab.value"
        >
          <BasicSetting v-if="tab.value === 'basic'" :profile="profile" @saved="loadProfile" />
          <PasswordSetting v-if="tab.value === 'password'" />
        </el-tab-pane>
      </el-tabs>
    </div>
  </div>
</template>

<style scoped>
.profile-page {
  padding: 16px;
}

.profile-card {
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
  padding: 24px;
  max-width: 900px;
}

.profile-header {
  display: flex;
  gap: 20px;
  align-items: center;
  margin-bottom: 24px;
  padding-bottom: 20px;
  border-bottom: 1px solid #ebeef5;
}

.profile-info {
  flex: 1;
}

.profile-name {
  margin: 0 0 8px;
  font-size: 20px;
  color: var(--el-text-color-primary);
}

.profile-meta {
  margin-bottom: 6px;
}

.profile-desc {
  font-size: 13px;
  color: #909399;
}

.profile-tabs {
  margin-top: 8px;
}
</style>
