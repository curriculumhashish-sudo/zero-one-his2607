import { defineStore } from 'pinia'
import type { LoginResult, Menu, Oauth2TokenDTO, RouteMenuRecord, UserInfo } from '@/apis/login/type'
import { loadCurrentMenus, loadCurrentUser, logout } from '@/apis/login'
import { DataUpType, useHttp } from '@/plugins/http'
import { useAuthManager } from '@/plugins/auth-manager'
import { createFrontendMenus, mergeFrontendMenus } from '@/config/frontend-menus'

type StoredUser = Partial<UserInfo> & { id: string; username: string }

const LOGIN_CONTEXT_KEY = 'loginContext'

function readLoginContext() {
  try {
    return JSON.parse(localStorage.getItem(LOGIN_CONTEXT_KEY) ?? '{}') as {
      tenantId?: string | number | null
      hospitalId?: string | number | null
      departmentId?: string | null
    }
  } catch {
    return {}
  }
}

function toMenu(route: RouteMenuRecord, parentId?: string): Menu {
  const id = route.name || route.path
  return {
    id,
    pid: parentId,
    text: route.meta.title,
    icon: route.meta.icon ?? 'IconMenu',
    href: route.children?.length ? undefined : route.path,
    children: route.children?.map((item) => toMenu(item, id))
  }
}

export const useUserStore = defineStore('user', {
  state: () => ({
    token: null as string | null,
    refreshToken: null as string | null,
    loaded: false,
    user: null as StoredUser | null,
    menus: createFrontendMenus(),
    tenantId: null as string | number | null,
    hospitalId: null as string | number | null,
    departmentId: null as string | null
  }),
  getters: {
    getToken: (state) => {
      if (state.token) return state.token
      const localToken = localStorage.getItem('token')
      if (localToken) return localToken
      try {
        return useAuthManager().accessToken.value
      } catch {
        return ''
      }
    },
    isLoaded: (state) => state.loaded,
    getUser: (state) => state.user,
    getMenus: (state) => mergeFrontendMenus(state.menus)
  },
  actions: {
    async loadUser() {
      if (!this.token) this.token = localStorage.getItem('token')
      this.loadRefreshToken()
      const user = await loadCurrentUser()
      this.user = { ...user, id: user.userId }
      this.setLoginContext(readLoginContext())
    },
    async loadMenus() {
      try {
        const routes = await loadCurrentMenus()
        this.menus = mergeFrontendMenus(routes.map((route) => toMenu(route)))
      } catch (error) {
        // 主框架的业务菜单由前端维护；菜单服务暂不可用时不应阻止已验证的用户进入系统。
        this.menus = createFrontendMenus()
        console.warn('加载权限菜单失败，已使用前端业务菜单。', error)
      }
    },
    loadRefreshToken() {
      if (!this.refreshToken) this.refreshToken = localStorage.getItem('refreshToken')
    },
    async reloadToken() {
      this.loadRefreshToken()
      if (!this.refreshToken) return
      const data = await useHttp().post<Oauth2TokenDTO>('/login/refresh-token', { refreshToken: this.refreshToken, token: this.token }, { upType: DataUpType.form })
      this.setToken(data.data)
    },
    setLoaded(loaded: boolean) {
      this.loaded = loaded
    },
    setToken(data: LoginResult | Oauth2TokenDTO | null | undefined) {
      if (!data) return
      this.token = 'accessToken' in data ? data.accessToken : data.token
      this.refreshToken = data.refreshToken
      if (this.token) localStorage.setItem('token', this.token)
      if (this.refreshToken) localStorage.setItem('refreshToken', this.refreshToken)
    },
    setLoginContext(context: { tenantId?: string | number | null; hospitalId?: string | number | null; departmentId?: string | null }) {
      if ('tenantId' in context) this.tenantId = context.tenantId ?? null
      if ('hospitalId' in context) this.hospitalId = context.hospitalId ?? null
      if ('departmentId' in context) this.departmentId = context.departmentId ?? null
      localStorage.setItem(LOGIN_CONTEXT_KEY, JSON.stringify({
        tenantId: this.tenantId,
        hospitalId: this.hospitalId,
        departmentId: this.departmentId
      }))
    },
    async logoutSession() {
      try {
        await logout()
      } finally {
        this.resetSaveData()
      }
    },
    resetSaveData() {
      this.loaded = false
      this.user = null
      this.token = null
      this.refreshToken = null
      this.menus = createFrontendMenus()
      this.tenantId = null
      this.hospitalId = null
      this.departmentId = null
      localStorage.removeItem('token')
      localStorage.removeItem('refreshToken')
      localStorage.removeItem(LOGIN_CONTEXT_KEY)
    }
  }
})
