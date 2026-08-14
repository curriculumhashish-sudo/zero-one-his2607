const routes = [
  {
    path: '/login',
    // 跳过登录
    // alias: '/',
    name: 'Login',
    meta: {
      anonymous: true
    },
    component: () => import('@/views/LoginView.vue')
  }
]

export default routes
