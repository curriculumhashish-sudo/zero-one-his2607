const routes = [
  {
    path: '/login',
    alias: '/',
    name: 'Login',
    meta: {
      anonymous: true
    },
    component: () => import('@/views/LoginView.vue')
  }
]

export default routes
