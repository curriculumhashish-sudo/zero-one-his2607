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
const c7ConfigurationServerUrl = env.VITE_C7_CONFIGURATION_SERVER_URL || 'http://47.99.54.215:8090'
const c5BasicDataServerUrl = env.VITE_C5_BASICDATA_SERVER_URL || 'http://116.62.31.155:8090'
const j1SysmgrServerUrl = env.VITE_J1_SYSMGR_SERVER_URL || 'http://123.57.144.180:10600'
const j3DepartmentServerUrl = env.VITE_J3_DEPARTMENT_SERVER_URL || 'http://115.29.213.118:10201'
const j3PatientServerUrl = env.VITE_J3_PATIENT_SERVER_URL || 'http://115.29.213.118:10202'
const j3RegisterServerUrl = env.VITE_J3_REGISTER_SERVER_URL || 'http://115.29.213.118:10203'
const j3StorehouseServerUrl = env.VITE_J3_STOREHOUSE_SERVER_URL || 'http://115.29.213.118:10204'
const j3FeeServerUrl = env.VITE_J3_FEE_SERVER_URL || 'http://47.96.2.145:10205'
const j3SupplierServerUrl = env.VITE_J3_SUPPLIER_SERVER_URL || 'http://47.96.2.145:10206'
const j3WardServerUrl = env.VITE_J3_WARD_SERVER_URL || 'http://47.96.2.145:10207'

const j3Proxy = (target: string) => ({
  changeOrigin: true,
  target,
  rewrite: (path: string) => path.replace(/^\/api/, '')
})

export default defineConfig({
  server: {
    host: '0.0.0.0',
    port: 3000,
    proxy: {
      '/api/c5-basicdata-api': {
        changeOrigin: true,
        target: c5BasicDataServerUrl,
        rewrite: (path) => path.replace(/^\/api\/c5-basicdata-api/, '')
      },
      '/api/j1-sysmgr-api': {
        changeOrigin: true,
        target: j1SysmgrServerUrl,
        rewrite: (path) => path.replace(/^\/api\/j1-sysmgr-api/, '/sysmgr')
      },
      '/api/j3-department': j3Proxy(j3DepartmentServerUrl),
      '/api/j3-patient': j3Proxy(j3PatientServerUrl),
      '/api/j3-register': j3Proxy(j3RegisterServerUrl),
      '/api/j3-storehouse': j3Proxy(j3StorehouseServerUrl),
      '/api/j3-fee': j3Proxy(j3FeeServerUrl),
      '/api/j3-supplier': j3Proxy(j3SupplierServerUrl),
      '/api/j3-ward': j3Proxy(j3WardServerUrl),
      '/j1-login': {
        changeOrigin: true,
        // J1 登录服务的本地开发代理；生产环境由网关统一配置。
        target: 'http://47.116.98.7:10500'
      },
      '/j8-calendar': {
        changeOrigin: true,
        // J8 号源日历服务的本地开发代理；生产环境由网关统一配置。
        target: 'http://47.99.167.11:10144'
      },
      '/j8-outpatient': {
        changeOrigin: true,
        // J8 门诊费用明细服务的本地开发代理；生产环境由网关统一配置。
        target: 'http://47.110.92.50:10155'
      },
      '/j8-statdata': {
        changeOrigin: true,
        target: 'http://47.98.154.109:10166'
      },
      '/j8-setroster': {
        changeOrigin: true,
        target: 'http://121.40.180.171:10122'
      },
      '/j8-clinics': {
        changeOrigin: true,
        target: 'http://47.98.165.254:10133'
      },
      '/j6-report': {
        changeOrigin: true,
        target: 'http://39.106.111.242:10235'
      },
      '/c1-report': {
        changeOrigin: true,
        target: 'http://47.109.52.229:8090',
        rewrite: (path) => path.replace(/^\/c1-report/, '')
      },
      '/c1-charge': {
        changeOrigin: true,
        target: 'http://39.105.18.251:8090',
        rewrite: (path) => path.replace(/^\/c1-charge/, '')
      },
      '/c6-api': {
        changeOrigin: true,
        target: 'http://47.102.133.197:8090',
        rewrite: (path) => path.replace(/^\/c6-api/, '')
      },
      '/j4-api': {
        changeOrigin: true,
        target: 'http://8.149.141.110:10300',
        rewrite: (path) => path.replace(/^\/j4-api/, '/j4-basedata')
      },
      '/j1-api': {
        changeOrigin: true,
        target: 'http://47.116.98.7:10500',
        rewrite: (path) => path.replace(/^\/j1-api/, '/j1-login')
      },
      '/api': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:使用Apifox云MOCK,联调阶段需要修改成网关地址
        target: gatewayUrl,
        rewrite: (path) => path.replace(/^\/api/, '')
      },
      '/nurse-station-api': {
        changeOrigin: true,
        // 住院护士站联调专用代理，避免修改其他模块当前使用的 Mock 地址。
        target: 'http://47.116.136.206:8090',
        rewrite: (path) => path.replace(/^\/nurse-station-api/, '')
      },
      '/outpatient-nurse-station-api': {
        changeOrigin: true,
        // 门诊护士站 j5-ons 服务，使用独立前缀避免影响其他模块。
        target: 'http://121.199.54.234:10500',
        rewrite: (path) => path.replace(/^\/outpatient-nurse-station-api/, '/j5-ons')
      },
      '/inpatient-doctor-station-api': {
        changeOrigin: true,
        // 住院医生站联调专用代理，避免修改其他模块当前使用的 Mock 地址。
        target: 'http://8.148.233.234:8070',
        rewrite: (path) => path.replace(/^\/inpatient-doctor-station-api/, '')
      },
      '/captcha': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:联调阶段需要修改成网关地址
        target: 'https://124.223.72.139:10001',
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
      '/j1-sysmgr-api': {
        changeOrigin: true,
        target: j1SysmgrServerUrl,
        rewrite: (path) => path.replace(/^\/j1-sysmgr-api/, '/sysmgr')
      },
      '/ai': {
        changeOrigin: true,
        //FIXME[TEST_CODE]:正式环境直接走网关地址，不走这里了
        target: 'https://124.223.72.139:10001',
        rewrite: (path) => path.replace(/^\/ai/, '')
      },
      '/j2-basedata': {
        // 基础数据-编码管理
        changeOrigin: true,
        target: 'http://118.31.56.25:10300'
      },
      '/j6-pharmacy': {
        // 药库进货/出库/盘点/库存明细
        changeOrigin: true,
        target: 'http://39.106.111.242:10234'
      },
      '/j7-pharmacy': {
        // 药房入库/盘点
        changeOrigin: true,
        target: 'http://121.199.165.157:10500'
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
