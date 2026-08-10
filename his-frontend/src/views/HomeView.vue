<template>
  <div class="header-row">
    <!-- 应用信息栏 -->
    <div class="header-title" :class="{ 'is-collapsed': isCollapse }">
      <img v-show="!isCollapse" class="app-icon" src="/logo.jpg" :title="appName" />
      <el-text v-show="!isCollapse" class="app-name">{{ appName }}</el-text>
      <el-button
        v-show="isCollapse"
        link
        class="collapse-btn"
        icon="IconExpand"
        @click="isCollapse = !isCollapse"
      ></el-button>
      <el-button
        v-show="!isCollapse"
        link
        class="collapse-btn"
        icon="IconFold"
        @click="isCollapse = !isCollapse"
      ></el-button>
    </div>
    <!-- 导航栏 -->
    <div class="header-nav">
      <el-dropdown v-if="showApiProfileSwitch" trigger="click" @command="handleApiProfileCommand">
        <el-button class="api-profile-button" size="small" plain>
          接口：{{ apiProfileLabel }}
        </el-button>
        <template #dropdown>
          <el-dropdown-menu>
            <el-dropdown-item command="j4" :disabled="apiProfile === 'j4'">
              J4 接口
            </el-dropdown-item>
            <el-dropdown-item command="c6" :disabled="apiProfile === 'c6'">
              C6 接口
            </el-dropdown-item>
          </el-dropdown-menu>
        </template>
      </el-dropdown>
      <el-avatar :size="30" :src="user?.avatar" />{{ userInfo }}
      <el-dropdown trigger="click" @command="handleUserSettings">
        <el-icon :size="22" color="white">
          <IconSetting />
        </el-icon>
        <template #dropdown>
          <el-dropdown-menu>
            <el-dropdown-item command="profile">
              <el-icon :size="15"> <IconUser /> </el-icon>个人中心
            </el-dropdown-item>
            <el-dropdown-item command="logout">
              <el-icon :size="15"> <IconSwitchButton /> </el-icon>退出登录
            </el-dropdown-item>
          </el-dropdown-menu>
        </template>
      </el-dropdown>
    </div>
  </div>
  <div class="content-row" :class="{ 'is-collapsed': isCollapse }">
    <!-- 侧边菜单栏 -->
    <el-menu
      :collapse="isCollapse"
      :default-active="activeIndex"
      active-text-color="#409EFF"
      text-color="#fff"
      background-color="#545c64"
      unique-opened
      :collapse-transition="false"
      router
    >
      <el-menu-item :index="indexPath">
        <el-icon>
          <IconHomeFilled />
        </el-icon>
        <span>首页</span>
      </el-menu-item>
      <template v-for="item in menus" :key="item.id">
        <el-menu-item v-if="item.href && !item.children?.length" :index="item.href">
          <el-icon>
            <component :is="item.icon" />
          </el-icon>
          <span>{{ item.text }}</span>
        </el-menu-item>
        <el-sub-menu v-else :index="item.id + 'submenu'">
          <template #title>
            <el-icon>
              <component :is="item.icon" />
            </el-icon>
            <span>{{ item.text }}</span>
          </template>
          <el-menu-item-group>
            <el-menu-item v-for="i in item.children" :key="i.id" :index="i.href || ''">
              <el-icon>
                <component :is="i.icon" />
              </el-icon>
              {{ i.text }}
            </el-menu-item>
          </el-menu-item-group>
        </el-sub-menu>
      </template>
    </el-menu>
    <!-- 主内容区 -->
    <div class="main">
      <!-- 标签栏 -->
      <el-tabs
        v-model="activeIndex"
        type="border-card"
        :before-leave="beforeLeave"
        @tab-click="tabClick"
        @tab-remove="tabColse"
      >
        <!-- 首页标签页 -->
        <el-tab-pane :name="indexPath" style="height: 0">
          <template #label>
            <el-icon>
              <IconHomeFilled />
            </el-icon>
            <span style="padding-left: 5px">首页</span>
          </template>
        </el-tab-pane>
        <!-- 动态标签页 -->
        <el-tab-pane
          v-for="(item, index) in tabs"
          :key="index + 'tab'"
          :label="item.label"
          :name="item.path"
          closable
          style="height: 0"
        />
        <!-- 操作标签页 -->
        <el-tab-pane style="height: 0" name="tab-operation">
          <template #label>
            <el-dropdown trigger="click">
              <el-button class="operation-icon" type="info" link>
                <el-icon size="22">
                  <IconOperation />
                </el-icon>
              </el-button>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item
                    :disabled="tabs.length == 0"
                    icon="IconCloseBlod"
                    @click="handleClose(1)"
                  >
                    关闭所有标签页
                  </el-dropdown-item>
                  <el-dropdown-item
                    :disabled="activeIndex == indexPath || tabstore.getTabIndex(activeIndex) == 0"
                    icon="IconCloseBlod"
                    @click="handleClose(2)"
                  >
                    关闭当前标签页左边
                  </el-dropdown-item>
                  <el-dropdown-item
                    :disabled="tabstore.getTabIndex(activeIndex) == tabs.length - 1"
                    icon="IconCloseBlod"
                    @click="handleClose(3)"
                  >
                    关闭当前标签页右边
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </template>
        </el-tab-pane>
      </el-tabs>
      <!-- 二级路由 -->
      <router-view />
    </div>
  </div>
  <AiChatWidget
    v-model:current-agent="currentAgent"
    :agents="aiAgents"
    :agents-loading="aiAgentsLoading"
    :agents-error="aiAgentsError"
    :request="requestAi"
    :initial-messages="aiInitialMessages"
    @open="handleAiChatOpen"
    @retry-load-agents="handleRetryLoadAgents"
  />
</template>

<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, ref } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { storeToRefs } from 'pinia'
import { useUserStore } from '@/stores/user'
import { useTabStore } from '@/stores/tab'
import { useAiChatStore } from '@/stores/ai-chat'
import type { TabPaneName, TabsPaneContext } from 'element-plus'
import { ElMessage, ElMessageBox } from 'element-plus'
import type { ChatImage, ChatMessage } from '@/components/ai-chat/type'
import AiChatWidget from '@/components/ai-chat/AiChatWidget.vue'
import {
  getApiProfile,
  getApiProfileLabel,
  setApiProfile,
  type ApiProfile
} from '@/apis/basicdata/profile'
// 应用名称
const appName = import.meta.env.VITE_APP_TITLE
// 当前用户信息
const ustore = useUserStore()
// 用户信息提示
const { user, getMenus: menus } = storeToRefs(ustore)
const userInfo = computed(() => `欢迎用户：${user.value?.username ?? '游客'}`)

const apiProfile = ref<ApiProfile>(getApiProfile())
const apiProfileLabel = computed(() => getApiProfileLabel(apiProfile.value))
const route = useRoute()
const showApiProfileSwitch = computed(() => route.path.startsWith('/basicdata/'))

async function handleApiProfileCommand(command: string | number | object) {
  if (command !== 'c6' && command !== 'j4') return
  if (command === apiProfile.value) return
  const targetLabel = getApiProfileLabel(command)
  try {
    await ElMessageBox.confirm(
      `切换到 ${targetLabel} 后将刷新页面，并清空当前页面的临时数据。是否继续？`,
      '切换接口来源',
      {
        confirmButtonText: '切换并刷新',
        cancelButtonText: '取消',
        type: 'warning'
      }
    )
  } catch {
    return
  }
  setApiProfile(command)
  window.location.reload()
}

// 用户设置点击事件
const handleUserSettings = async (command: string | number | object) => {
  if (command == 'profile') {
    await router.push({ name: 'MyProfile' })
  } else if (command == 'logout') {
    try {
      await ustore.logoutSession()
    } finally {
      tabstore.reset()
      await router.push({ name: 'Login' })
    }
  }
}

// 菜单是是否折叠
const isCollapse = ref(false)
let isCompactViewport = false

function syncResponsiveLayout() {
  const compact = window.innerWidth <= 900
  if (compact === isCompactViewport) return
  isCompactViewport = compact
  isCollapse.value = compact
}

onMounted(() => {
  syncResponsiveLayout()
  window.addEventListener('resize', syncResponsiveLayout)
})

onBeforeUnmount(() => window.removeEventListener('resize', syncResponsiveLayout))
// 路由数据
const router = useRouter()
// 标签页数据
const tabstore = useTabStore()
const { tabs, activeIndex, indexPath } = storeToRefs(tabstore)

/** 标签页点击事件 */
const tabClick = (pane: TabsPaneContext) => {
  // 如果点击的是操作标签页
  if (pane.paneName == 'tab-operation') return
  // 设置激活标签页
  tabstore.setActiveIndex(pane.paneName as string)
  // 进行路由跳转
  router.push({ path: activeIndex.value })
}
/** 标签页关闭事件 */
const tabColse = (name: TabPaneName) => {
  // 如果删除的是当前标签
  if (activeIndex.value == name) {
    // 重新设置当前激活标签页为它相邻的标签页
    const idx = tabstore.getTabIndex(name as string) - 1
    if (idx >= 0 && tabs.value[idx]) activeIndex.value = tabs.value[idx].path
    else activeIndex.value = indexPath.value
    // 进行路由跳转
    router.push({ path: activeIndex.value })
  }
  // 删除标签
  tabstore.remTab(name as string)
}
/** 标签页切换事件 */
const beforeLeave = (activeName: TabPaneName) => {
  // 操作标签不做激活操作
  if (activeName == 'tab-operation') return false
  return true
}

/** 管理标签页关闭 */
function handleClose(type: number) {
  switch (type) {
    case 1:
      // 重置标签页数据
      tabstore.reset()
      // 跳转到首页
      router.push({ path: activeIndex.value })
      break
    case 2:
      // 关闭当前标签页左边
      tabstore.remBeforeTab(activeIndex.value)
      break
    case 3:
      // 关闭当前标签页右边
      tabstore.remAfterTab(activeIndex.value)
      break
    default:
      break
  }
}

// AI 对话：统一由 Pinia 管理智能体列表、当前选中智能体，以及请求实现
const aiChatStore = useAiChatStore()
// 响应式解构：currentAgent / agents / initialMessages 在 UI 中会随 store 更新而自动刷新
const {
  currentAgent,
  agents: aiAgents,
  agentsLoading: aiAgentsLoading,
  agentsLoadError: aiAgentsError,
  initialMessages: aiInitialMessages
} = storeToRefs(aiChatStore)
const handleAiChatOpen = () => aiChatStore.loadAgents()
const handleRetryLoadAgents = () => aiChatStore.loadAgents(true)
// 对话请求：按组件签名转发到 store
const requestAi = (text: string, history: ChatMessage[], images?: ChatImage[]) =>
  aiChatStore.requestAi(text, history, images)
</script>

<style>
:root {
  --home-header-height: 60px;
  --home-menu-width: 220px;
}

.main .el-tabs {
  .el-tabs__header {
    padding: 0;
    margin-bottom: 0;
  }

  .el-tabs__content {
    padding-top: 0px;
    padding-bottom: 0;
  }

  #tab-tab-operation {
    padding: 0;
  }
}
</style>

<style scoped>
.header-row {
  height: var(--home-header-height);
  background-color: #6c777f;
  display: flex;
  align-items: center;
  justify-content: space-between;

  .header-title {
    width: var(--home-menu-width);
    padding-left: 15px;
    display: flex;
    align-items: center;
    justify-content: space-between;

    .app-icon {
      width: 30px;
      border-radius: 5px;
    }

    .app-name {
      color: white;
      font-size: 16px;
    }

    .collapse-btn {
      color: white;
      font-size: 24px;
    }

    &.is-collapsed {
      width: calc(var(--el-menu-icon-width) + var(--el-menu-base-level-padding) * 2);
      padding-left: 0;
      justify-content: center;

      .collapse-btn {
        margin: 0;
      }
    }
  }

  .header-nav {
    display: flex;
    justify-content: flex-end;
    align-items: center;
    gap: 10px;
    padding-right: 15px;
    color: #f8f8f8;
    text-align: right;

    .api-profile-button {
      border-color: rgba(255, 255, 255, 0.72);
      background: rgba(255, 255, 255, 0.12);
      color: #fff;
      font-weight: 600;
    }
  }
}

.content-row {
  height: calc(100vh - var(--home-header-height));
  display: flex;
  flex-direction: row;

  .el-menu {
    border: 0;
    width: var(--home-menu-width);
    height: 100%;
    overflow: auto;
  }

  .el-menu--collapse {
    width: calc(var(--el-menu-icon-width) + var(--el-menu-base-level-padding) * 2);
  }

  .main {
    flex: 1;
    min-width: 0;
    background-color: rgba(237, 236, 236, 0);
    width: auto;
    height: 100%;
    overflow: auto;

    .operation-icon {
      padding: calc(var(--el-tabs-header-height) / 2 - 12px) 20px;
    }
  }
}

@media (max-width: 900px) {
  .header-row .header-nav {
    min-width: 0;
    gap: 6px;
    padding-right: 10px;
    white-space: nowrap;
  }
}
</style>
