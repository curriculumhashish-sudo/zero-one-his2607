import { UserManager, type UserManagerSettings, type User } from 'oidc-client-ts'
import { type Plugin, getCurrentInstance, inject, ref } from 'vue'

/** 插件标识符 */
const PROVIDE_KEY = Symbol('oidc-provider')

/**
 * 认证管理器
 */
class AuthManager {
  /**
   * token
   */
  accessToken = ref('')
  /**
   * 当前用户信息
   */
  user = ref<User>()
  /**
   * oidc 客户端
   */
  private oidc: UserManager

  /**
   * 构造函数
   * @param settings oidc 客户端配置
   */
  constructor(settings: UserManagerSettings) {
    this.oidc = new UserManager(settings)
    // 当用户登录时，更新token
    this.oidc.events.addUserLoaded((user) => {
      this.accessToken.value = user.access_token
      this.user.value = user
      // 开启静默刷新，清除过期状态
      this.oidc.startSilentRenew()
      this.oidc.clearStaleState()
    })
    // 当更新token失败时，退出登录
    this.oidc.events.addSilentRenewError(() => {
      this.logout()
    })
    // 当token过期时，退出登录
    this.oidc.events.addAccessTokenExpired(() => {
      this.logout()
    })
    // 初始化时加载用户信息
    this.loadUser()
  }
  /**
   * 加载用户信息
   */
  async loadUser() {
    const user = await this.oidc.getUser()
    // 如果能加载出来则将信息放到Ref里
    if (user) {
      this.accessToken.value = user.access_token
      this.user.value = user
      // 开启静默刷新，清除过期状态
      this.oidc.startSilentRenew()
      this.oidc.clearStaleState()
    }
  }
  /**
   * 登录
   */
  login() {
    return this.oidc.signinRedirect()
  }
  /**
   * 检查是否已登录
   * @returns 是否已登录
   */
  async checkLogin(): Promise<boolean> {
    const user = await this.oidc.getUser()
    return user != null && !user.expired
  }
  /**
   * 退出登录
   */
  logout() {
    this.oidc.stopSilentRenew()
    this.accessToken.value = ''
    this.user.value = undefined
    return this.oidc.signoutRedirect()
  }
  /**
   * 刷新token
   * @param force是否强制刷新
   */
  async refresh(force?: boolean) {
    // 如果不是强制刷新，则先检查用户可用，如果用户可用则不刷新
    if (!force) {
      const user = await this.oidc.getUser()
      if (user != null && !user.expired) {
        return user
      }
    }
    return this.oidc.signinSilent()
  }
  /**
   * 登录回调
   */
  loginCallback() {
    return this.oidc.signinCallback()
  }
}

/** 全局实例 */
let globalAuth: AuthManager | null = null

/**
 * 认证插件
 */
const authPlugin: Plugin<UserManagerSettings> = {
  install: (app, options) => {
    const auth = new AuthManager(options)
    app.provide(PROVIDE_KEY, auth)
    // 保存到全局变量
    globalAuth = auth
    console.log('Auth plugin installed.')
  }
}

/**
 * 使用认证管理器
 * @returns 认证管理器
 */
const useAuthManager = () => {
  // 尝试获取当前组件实例
  const instance = getCurrentInstance()
  if (instance) {
    // 在组件上下文中，使用注入
    const plugin = inject<AuthManager>(PROVIDE_KEY)
    if (plugin) return plugin
  }

  // 不在组件上下文中，使用全局实例
  if (globalAuth) {
    return globalAuth
  }

  // 没有获取到插件实例，抛出异常
  throw new Error('Auth plugin is not installed, Please use app.use(authPlugin) first.')
}

export { authPlugin, useAuthManager }
