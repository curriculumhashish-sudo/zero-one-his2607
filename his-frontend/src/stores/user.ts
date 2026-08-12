import { defineStore } from 'pinia'
import type {
  LoginResult,
  Menu,
  Oauth2TokenDTO,
  RouteMenuRecord,
  UserInfo
} from '@/apis/login/type'
import { loadCurrentMenus, loadCurrentUser, logout } from '@/apis/login'
import { loadMenusForNavigation } from '@/apis/workbench'
import { DataUpType, useHttp } from '@/plugins/http'
import { useAuthManager } from '@/plugins/auth-manager'
import { createFrontendMenus, filterFrontendMenus } from '@/config/frontend-menus'

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

function toMenu(route: RouteMenuRecord, parentId?: string): Menu | null {
  const title = route.meta?.title
  if (!title) return null
  const id = route.name || route.path
  const children = route.children?.length
    ? route.children.map((item) => toMenu(item, id)).filter((m): m is Menu => m !== null)
    : undefined
  return {
    id,
    pid: parentId,
    text: title,
    icon: route.meta?.icon ?? 'IconMenu',
    href: children ? undefined : route.path,
    children
  }
}

export const useUserStore = defineStore('user', {
  state: () => ({
    token: null as string | null,
    // token:
    //   'eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX25hbWUiOiJhZG1pbiIsInNjb3BlIjpbImFsbCJdLCJpZCI6MSwiZXhwIjoxNjAwNzU4NjEzLCJhdXRob3JpdGllcyI6WyJBRE1JTiJdLCJqdGkiOiI2ZWY3MTkzNS04Yjk0LTQ0YzQtOGRhNC0wODhhMTVmOGQ5NzkiLCJjbGllbnRfaWQiOiJoZWFsdGgtY2xpZW50In0.RY6Eu9O35SRbtxOKrP0Db-kl7A9QK8XBr7KCP2Qnx1bGCoHm9v3OnXbOgZ95QMuo117wS3t8KyCAHIhclrDykl5kf_sV2_FrnrzZWYb8kSDB6pPKSSJLTWTud1eN9W07K2OKh9DHtDEW3CdnHXeV9I3yGJhIYuSaWcmq4pqOSd3UFktEcvUIQewcbdppsgT_CWngaXn88wNCmHjClvBuFuXL9r67_QwvWqJr1iCMrF9tOo993PzxP1P2zDkRwnINy9lRxly7lIkJ4Um5w7v4eeJmYo5e3VliagACKM-MOKqB6cy8nqUMkLW5r-t_74h_yxcjKdan0jRcjcpCXcHsrA',
    refreshToken: null as string | null,
    // refreshToken:
    //   'eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX25hbWUiOiJhZG1pbiIsInNjb3BlIjpbImFsbCJdLCJhdGkiOiI2ZWY3MTkzNS04Yjk0LTQ0YzQtOGRhNC0wODhhMTVmOGQ5NzkiLCJpZCI6MSwiZXhwIjoxNjAwODQxNDEzLCJhdXRob3JpdGllcyI6WyJBRE1JTiJdLCJqdGkiOiJlMjU3ZDFjNi02Mzk0LTRkODItYmIxOC1mY2I4ZDBkOTczNzciLCJjbGllbnRfaWQiOiJoZWFsdGgtY2xpZW50In0.I2kxj_sOJZ2Ui2Hd8aPCPzCE-vP8kq4L6WXjBgrFUXahismN2ipRuqaMTzxC_sWBPaSjTSsElmYiN5q95ktm_QwLZbQkPb0wi2l9CggVKWSOpoz6QorIfpCh63Tc_GR3vPT6W9M2tdBiDtX477O5ddAmgAoXf62foOL-AjcCxSryTLrdICGbBq2ZArOGvKhF5NInF7BHX2LYTJF5Yt9B0y5YMdKdmIoMJwQLgPWwxktfHOjx2s-DordJEKjKSmLWkq9qa6MgxgYJQM7Ex-8obEtoxkEDDloC3SAZK_53YffnO14kY4025cWLH4N0p_RbrGPPz9bBthVO8YOog9nfGA',
    // tokenHead: 'Bearer ',
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
    getMenus: (state) => state.menus
  },
  actions: {
    enableDevBypass() {
      this.loaded = true
      this.user = {
        id: 'dev-user',
        userId: 'dev-user',
        username: 'dev-user',
        realName: '开发测试用户',
        roles: ['dev'],
        avatar: '',
        desc: '仅用于本地接口联调',
        homePath: '/home',
        token: ''
      }
      this.menus = createFrontendMenus()
    },
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
        console.log('[menus] /auth/menus 返回路由数:', routes.length)
        const serverMenus = routes.map((route) => toMenu(route)).filter((m): m is Menu => m !== null)
        console.log('[menus] 转换后菜单数:', serverMenus.length)
        const authorizedMenus = filterFrontendMenus(serverMenus)
        if (authorizedMenus.length > 0) {
          this.menus = authorizedMenus
          console.log('[menus] 使用后端授权菜单，过滤后分组数:', this.menus.length)
        } else {
          // 后端返回空，从 J2 工作台数据构建（已按角色过滤）
          console.warn('[menus] 后端菜单为空，尝试从工作台数据构建导航')
          this.menus = await loadMenusForNavigation()
        }
      } catch (error) {
        console.warn('[menus] 加载权限菜单失败，尝试从工作台数据构建导航:', error)
        try {
          this.menus = await loadMenusForNavigation()
        } catch {
          console.warn('[menus] 工作台数据也失败，使用前端全量菜单兜底')
          this.menus = createFrontendMenus()
        }
      }
    },
    loadRefreshToken() {
      if (!this.refreshToken) this.refreshToken = localStorage.getItem('refreshToken')
    },
    async reloadToken() {
      this.loadRefreshToken()
      if (!this.refreshToken) return
      const data = await useHttp().post<Oauth2TokenDTO>(
        '/login/refresh-token',
        { refreshToken: this.refreshToken, token: this.token },
        { upType: DataUpType.form }
      )
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
      if ('tokenHead' in data && data.tokenHead) localStorage.setItem('tokenHead', data.tokenHead.trim())
    },
    setLoginContext(context: {
      tenantId?: string | number | null
      hospitalId?: string | number | null
      departmentId?: string | null
    }) {
      if ('tenantId' in context) this.tenantId = context.tenantId ?? null
      if ('hospitalId' in context) this.hospitalId = context.hospitalId ?? null
      if ('departmentId' in context) this.departmentId = context.departmentId ?? null
      localStorage.setItem(
        LOGIN_CONTEXT_KEY,
        JSON.stringify({
          tenantId: this.tenantId,
          hospitalId: this.hospitalId,
          departmentId: this.departmentId
        })
      )
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
      localStorage.removeItem('tokenHead')
      localStorage.removeItem(LOGIN_CONTEXT_KEY)
    }
  }
})
