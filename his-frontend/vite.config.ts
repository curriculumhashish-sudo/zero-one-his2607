import { fileURLToPath, URL } from 'node:url'
import { defineConfig, loadEnv } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import { ElementPlusResolver } from 'unplugin-vue-components/resolvers'

// 本地https支持插件，插件启动的时候需要从Github下载文件，所以没有梯子可能无法正常工作
// import mkcert from 'vite-plugin-mkcert'

// https://vite.dev/config/
const env = loadEnv(process.env.NODE_ENV || 'development', process.cwd(), '')
const gatewayUrl = env.VITE_GATEWAY_URL || 'http://124.223.72.139:10001'
const loginServerUrl = env.VITE_LOGIN_SERVER_URL || 'http://47.116.98.7:10500'
const c7ConfigurationServerUrl =
  env.VITE_C7_CONFIGURATION_SERVER_URL || 'http://localhost:8090'

export default defineConfig({
  server: {
    host: '0.0.0.0',
    port: 3000,
    proxy: {
      '/api': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:使用Apifox云MOCK,联调阶段需要修改成网关地址
        target: gatewayUrl,
        rewrite: (path) => path.replace(/^\/api/, '')
      },
      '/captcha': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:联调阶段需要修改成网关地址
        target: 'http://localhost:10010',
        rewrite: (path) => path.replace(/^\/captcha/, '')
      },
      '/login-api': {
        changeOrigin: true,
        target: loginServerUrl,
        rewrite: (path) => path.replace(/^\/login-api/, '/j1-login')
      },
      '/c7-configuration-api': {
        changeOrigin: true,
        target: c7ConfigurationServerUrl,
        rewrite: (path) => path.replace(/^\/c7-configuration-api/, '')
      },
      '/ai': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:正式环境直接走网关地址，不走这里了
        target: 'http://localhost:10250',
        rewrite: (path) => path.replace(/^\/ai/, '')
      }
    }
  },
  build: {
    assetsDir: 'static',
    chunkSizeWarningLimit: 2500,
    rollupOptions: {
      output: {
        manualChunks(id) {
          if (id.includes('node_modules')) {
            return id.toString().split('node_modules/')[1].split('/')[0].toString()
          }
        }
      },
      external: new RegExp('views/sample/.*')
    }
  },
  plugins: [
    //mkcert(),
    vue(),
    vueDevTools(),
    AutoImport({
      resolvers: [ElementPlusResolver()]
    }),
    Components({
      resolvers: [ElementPlusResolver()]
    })
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  }
})
