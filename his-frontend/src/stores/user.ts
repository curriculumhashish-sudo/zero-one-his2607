import { defineStore } from 'pinia'
import type {
  LoginResult,
  Menu,
  Oauth2TokenDTO,
  RouteMenuRecord,
  UserInfo
} from '@/apis/login/type'
import { loadCurrentMenus, loadCurrentUser, logout } from '@/apis/login'
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
        this.menus = filterFrontendMenus(routes.map((route) => toMenu(route)))
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
