import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'
import router from './router'
import 'element-plus/es/components/message/style/css'
import 'element-plus/es/components/message-box/style/css'
import zhCn from 'element-plus/es/locale/lang/zh-cn'
import './assets/main.css'

// 使用ElementPlus和FcDesigner
import FcDesigner from '@form-create/designer'
import ElementPlus from 'element-plus'

/** 开发联调固定 JWT，用于跳过前端登录页。 */
const app = createApp(App)

app.use(createPinia())
app.use(router)

// 使用ElementPlus和FcDesigner
app.use(ElementPlus, { locale: zhCn })
app.use(FcDesigner)

// 安装http插件
import http from './plugins/http'
app.use(http, { router })

// 安装ElIcon
import icon from './plugins/icon'
app.use(icon)

// 安装authManager
import { authPlugin } from './plugins/auth-manager'
import { WebStorageStateStore, type UserManagerSettings } from 'oidc-client-ts'
app.use(authPlugin, {
  // 认证服务器地址
  authority: import.meta.env.VITE_AUTH_SERVER_URL,
  // 客户端ID
  client_id: import.meta.env.VITE_CLIENT_ID,
  // 登录回调地址
  redirect_uri: `${window.location.origin}/signin-callback`,
  // 登出回调地址
  post_logout_redirect_uri: `${window.location.origin}/`,
  // 授权码模式
  response_type: 'code',
  // 请求的权限范围
  scope: 'openid profile',
  // 使用本地存储持久化用户状态
  userStore: new WebStorageStateStore({ store: window.sessionStorage })
} as UserManagerSettings)

app.mount('#app')
