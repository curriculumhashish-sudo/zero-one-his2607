import { defineStore } from 'pinia'
import type { Menu, Oauth2TokenDTO, UserInfo } from '@/apis/login/type'
import { DataUpType, useHttp } from '@/plugins/http'
import { useAuthManager } from '@/plugins/auth-manager'
import { createFrontendMenus, mergeFrontendMenus } from '@/config/frontend-menus'

export const useUserStore = defineStore('user', {
  state: () => ({
    // 记录token
    token: null as string | null,
    // 记录refreshToken
    refreshToken: null as string | null,
    // 保存一个标识信息，指示登陆后需要加载的初始化数据是否完成
    loaded: false,
    // 保存当前用户
    user: null as UserInfo | null,
    // 菜单数据
    menus: createFrontendMenus()
  }),
  getters: {
    // 获取token
    getToken: (state) => {
      // 1. 优先使用当前state的token
      if (state.token) return state.token
      // 2. 开发与测试环境，尝试降级到localStorage方便调试
      if (import.meta.env.DEV || import.meta.env.TEST) {
        const ltoken = localStorage.getItem('token')
        if (ltoken) return ltoken
      }
      // 3. 统一的兜底获取逻辑
      try {
        return useAuthManager().accessToken.value
      } catch (error: any) {
        console.error('获取accessToken失败:', error.message)
        return ''
      }
    },
    // 是否已加载
    isLoaded: (state) => state.loaded,
    // 获取当前用户
    getUser: (state) => state.user,
    // 获取菜单
    // getter 始终补齐前端菜单，当前登录会话无需重新请求菜单也能立即显示。
    getMenus: (state) => mergeFrontendMenus(state.menus)
  },
  actions: {
    // 加载用户
    async loadUser() {
      // 发送获取当前用户信息请求
      const data = await useHttp().get<UserInfo>('/login/current-user')
      if (data.data) this.user = data.data
      if (!this.user?.avatar) {
        this.user = {
          avatar: 'https://cube.elemecdn.com/3/7c/3ea6beec64369c2642b92c6726f1epng.png',
          ...this.user
        } as UserInfo
      }
    },
    // 加载菜单
    async loadMenus() {
      try {
        // 保留后端权限菜单，并补入当前阶段由前端维护的业务页面。
        const data = await useHttp().get<Array<Menu>>('/login/get-menus')
        this.menus = mergeFrontendMenus(data.data ?? [])
      } catch (error) {
        this.menus = createFrontendMenus()
        // 开发阶段允许只使用前端菜单；生产环境仍应由后端菜单接口决定权限。
        if (!import.meta.env.DEV) throw error
      }
    },
    // 加载刷新凭证
    loadRefreshToken() {
      if (!this.refreshToken) this.refreshToken = localStorage.getItem('refreshToken')
    },
    // 刷新token
    async reloadToken() {
      // 先加载刷新凭证
      this.loadRefreshToken()
      // 如果没有刷新凭证，直接返回
      if (!this.refreshToken) return
      // 发送刷新凭证请求
      const data = await useHttp().post<Oauth2TokenDTO>(
        '/login/refresh-token',
        {
          refreshToken: this.refreshToken,
          token: this.token
        },
        {
          upType: DataUpType.form
        }
      )
      //设置Token相关属性
      this.setToken(data.data)
    },
    // 设置是否加载完成
    setLoaded(loaded: boolean) {
      this.loaded = loaded
    },
    // 设置token
    setToken(data: any) {
      this.token = data.token
      this.refreshToken = data.refreshToken
      if (this.token) localStorage.setItem('token', this.token)
      if (this.refreshToken) localStorage.setItem('refreshToken', this.refreshToken)
    },
    // 重置数据
    resetSaveData() {
      this.loaded = false
      this.user = null
      this.token = null
      this.refreshToken = null
      this.menus = createFrontendMenus()
      localStorage.removeItem('token')
      localStorage.removeItem('refreshToken')
    }
  }
})
