import { createRouter, createWebHistory } from 'vue-router'
import { ElMessage } from 'element-plus'
import { useUserStore } from '@/stores/user'
import { useTabStore } from '@/stores/tab'
import type { RouteRecordRawArray } from '@/types/Route'

const routes = []
routes.push(
  {
    path: '/:pathMatch(.*)*',
    name: 'NotFound',
    meta: {
      anonymous: true
    },
    component: () => import('../views/status/404.vue')
  },
  {
    path: '/forbidden',
    name: 'Forbidden',
    meta: {
      anonymous: true
    },
    component: () => import('../views/status/403.vue')
  },
  {
    path: '/error',
    name: 'Error',
    meta: {
      anonymous: true
    },
    component: () => import('../views/status/500.vue')
  },
  {
    path: '/signin-callback',
    name: 'SigninCallback',
    meta: {
      anonymous: true
    },
    component: () => import('@/components/auth/SigninCallback.vue')
  }
)

// 读取login模块路由
const loginRouter = import.meta.glob<RouteRecordRawArray>('./login/index.ts', { eager: true })
for (const path in loginRouter) {
  const module = loginRouter[path]
  if (module && module.default) {
    routes.push(...module.default)
  }
}

// 读取main模块路由
const mainRouter = import.meta.glob<RouteRecordRawArray>('./main/index.ts', { eager: true })
for (const path in mainRouter) {
  const module = mainRouter[path]
  if (module && module.default) {
    routes.push(...module.default)
  }
}

// 读取示例演示模块路由
if (import.meta.env.DEV) {
  const sampleRouter = import.meta.glob<RouteRecordRawArray>('./sample/index.ts', { eager: true })
  for (const path in sampleRouter) {
    const module = sampleRouter[path]
    if (module && module.default) {
      routes.push(...module.default)
    }
  }
}

// 定义一个路由对象
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes
})

// 添加一个路由的全局前置守卫
router.beforeEach(async function (to, _from) {
  // 放行可匿名访问的页面
  if (to.meta.anonymous) {
    return true
  }

  // 放行示例模块与基础数据模块访问（开发环境）
  if (import.meta.env.DEV) {
    if (to.path.indexOf('sample') !== -1 || to.path.indexOf('basicdata') !== -1) {
      return true
    }
  }

  // 判断本地是否记录token值
  const store = useUserStore()
  const token = store.getToken
  // 如果有token
  if (token) {
    // 判断是否已经加载数据
    const isLoaded = store.isLoaded
    // 如果没有加载
    if (!isLoaded) {
      try {
        // 加载用户信息
        await store.loadUser()
        // 加载菜单资源
        await store.loadMenus()
        // 设置加载完毕
        store.setLoaded(true)
      } catch {
        store.resetSaveData()
        useTabStore().reset()
        ElMessage.warning('登录状态已失效，请重新登录')
        return { name: 'Login', replace: true }
      }
    }

    // #region 处理标签页数据
    const tabstore = useTabStore()
    // 首页处理
    if (to.path == tabstore.indexPath) {
      tabstore.setActiveIndex(to.path)
    }
    // 设置了标签数据的路由
    else if (to.meta.label) {
      const idx = tabstore.getTabIndex(to.path)
      // 如果标签页已经打开了
      if (idx !== -1) {
        // 设置当前标签页
        tabstore.setActiveIndex(to.path)
      } else {
        // 添加标签页
        tabstore.addTab({
          label: to.meta.label,
          path: to.path
        })
        // 设置当前标签页
        tabstore.setActiveIndex(to.path)
      }
    }
    // #endregion

    // 允许跳转
    return true
  }
  // 如果没有token值，直接进入登录
  else {
    ElMessage.warning('在未登录时，禁止访问其他页面！')
    return { name: 'Login' }
  }
})

export default router
