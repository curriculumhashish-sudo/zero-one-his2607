# TianaiCaptcha 验证码组件

基于天爱验证码（Tianai Captcha）的 Vue3 封装组件，提供滑块、旋转、拼接、文字点选等多种验证方式。

组件官方地址：https://gitee.com/dromara/tianai-captcha

## 📋 功能特性

- ✅ 支持两种显示模式：内置模式和弹窗模式
- ✅ 支持多种验证码类型：滑块、旋转、拼接、文字点选
- ✅ 自动管理验证码生命周期（初始化/销毁）
- ✅ TypeScript 类型支持
- ✅ 灵活的事件回调机制
- ✅ 支持自定义配置参数传递

## 🚀 快速开始

### 1. 环境准备

确保项目中已配置以下环境变量（`.env` 文件）：

```env
VITE_CAPTCHA_URL=http://your-captcha-server.com
VITE_CAPTCHA_PREFIX=/captcha
```

### 2. 静态资源部署

确保 `public/static/tac` 目录包含天爱验证码的静态资源文件。

### 3. Logo 图片

在 `public` 目录下放置 `logo.jpg` 文件，或修改组件中的 `style.logoUrl` 配置。

## 📖 API 文档

### Props 属性

| 属性名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| mode | `string` | `'default'` | 显示模式：`'default'`（内置）或 `'popup'`（弹窗） |
| modelValue | `boolean` | `false` | 弹窗模式下的显隐控制（v-model 双向绑定） |
| options | `object` | `{}` | 传递给验证码的额外配置参数 |
| type | `string` | `''` | 验证码类型：`SLIDER` \| `ROTATE` \| `CONCAT` \| `WORD_IMAGE_CLICK` |

### Events 事件

| 事件名 | 参数 | 说明 |
|--------|------|------|
| update:modelValue | `(value: boolean)` | 弹窗关闭时触发，用于 v-model 双向绑定 |
| success | `(captchaId: string)` | 验证成功时触发，返回验证码 ID |
| error | - | 初始化或验证失败时触发 |

### Exposed Methods 暴露方法

| 方法名 | 参数 | 返回值 | 说明 |
|--------|------|--------|------|
| reloadCaptcha | - | - | 重新加载验证码 |

## 💡 使用示例

### 示例 1：内置模式（默认）

直接在页面中嵌入验证码，适合登录页等场景。

```vue
<template>
  <div class="login-form">
    <!-- 其他表单内容 -->
    <TianaiCaptcha 
      mode="default"
      type="SLIDER"
      @success="handleSuccess"
      @error="handleError"
    />
  </div>
</template>

<script setup lang="ts">
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'

const handleSuccess = (captchaId: string) => {
  console.log('验证成功，验证码ID:', captchaId)
  // 携带 captchaId 调用登录接口
}

const handleError = () => {
  console.error('验证失败')
}
</script>
```

### 示例 2：弹窗模式

点击按钮后弹出验证码弹窗，适合需要二次验证的场景。

```vue
<template>
  <div>
    <el-button @click="showCaptcha">进行安全验证</el-button>
    
    <TianaiCaptcha 
      v-model="captchaVisible"
      mode="popup"
      type="ROTATE"
      :options="{ customParam: 'value' }"
      @success="handleSuccess"
      @error="handleError"
    />
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'

const captchaVisible = ref(false)

const showCaptcha = () => {
  captchaVisible.value = true
}

const handleSuccess = (captchaId: string) => {
  console.log('验证成功，验证码ID:', captchaId)
  // 执行后续业务逻辑
}

const handleError = () => {
  console.error('验证失败')
  ElMessage.error('验证失败，请重试')
}
</script>
```

### 示例 3：通过 ref 调用方法

手动控制验证码刷新。

```vue
<template>
  <div>
    <TianaiCaptcha 
      ref="captchaRef"
      mode="default"
      type="SLIDER"
      @success="handleSuccess"
    />
    <el-button @click="refreshCaptcha">刷新验证码</el-button>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'

const captchaRef = ref()

const refreshCaptcha = () => {
  captchaRef.value?.reloadCaptcha()
}

const handleSuccess = (captchaId: string) => {
  console.log('验证成功', captchaId)
}
</script>
```

### 示例 4：不同验证码类型

```vue
<template>
  <div>
    <!-- 滑块验证 -->
    <TianaiCaptcha type="SLIDER" @success="handleSuccess" />
    
    <!-- 旋转验证 -->
    <TianaiCaptcha type="ROTATE" @success="handleSuccess" />
    
    <!-- 拼接验证 -->
    <TianaiCaptcha type="CONCAT" @success="handleSuccess" />
    
    <!-- 文字点选验证 -->
    <TianaiCaptcha type="WORD_IMAGE_CLICK" @success="handleSuccess" />
  </div>
</template>

<script setup lang="ts">
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'

const handleSuccess = (captchaId: string) => {
  console.log('验证成功', captchaId)
}
</script>
```

## ⚙️ 高级配置

### 自定义验证码配置

通过 `options` 属性传递额外的配置参数：

```vue
<TianaiCaptcha 
  mode="popup"
  :options="{
    requestCaptchaDataUrl: "/gen",
    validCaptchaUrl: "/check",
  }"
  @success="handleSuccess"
/>
```

### 完整登录流程示例

```vue
<template>
  <el-form :model="loginForm" @submit.prevent="handleLogin">
    <el-form-item label="用户名">
      <el-input v-model="loginForm.username" />
    </el-form-item>
    
    <el-form-item label="密码">
      <el-input v-model="loginForm.password" type="password" />
    </el-form-item>
    
    <TianaiCaptcha 
      v-model="captchaVisible"
      mode="popup"
      type="SLIDER"
      @success="handleCaptchaSuccess"
    />
    
    <el-button @click="showCaptcha">登录</el-button>
  </el-form>
</template>

<script setup lang="ts">
import { ref, reactive } from 'vue'
import { ElMessage } from 'element-plus'
import TianaiCaptcha from '@/components/captcha/TianaiCaptcha.vue'
import { loginApi } from '@/apis/auth'

const loginForm = reactive({
  username: '',
  password: ''
})

const captchaVisible = ref(false)
let captchaId = ''

const showCaptcha = () => {
  if (!loginForm.username || !loginForm.password) {
    ElMessage.warning('请输入用户名和密码')
    return
  }
  captchaVisible.value = true
}

const handleCaptchaSuccess = (id: string) => {
  captchaId = id
  submitLogin()
}

const submitLogin = async () => {
  try {
    const res = await loginApi({
      ...loginForm,
      captchaId
    })
    ElMessage.success('登录成功')
    // 处理登录成功逻辑
  } catch (error) {
    ElMessage.error('登录失败')
  }
}
</script>
```

## 🔧 注意事项

1. **类型声明**：确保 `env.d.ts` 文件中已声明 `window.initTAC` 类型
2. **静态资源**：`public/static/tac` 目录必须包含天爱验证码的完整静态资源
3. **环境变量**：`VITE_CAPTCHA_URL` 和 `VITE_CAPTCHA_PREFIX` 必须正确配置
4. **Logo 图片**：默认使用 `/logo.jpg`，如不存在请修改组件内的 `style.logoUrl`
5. **容器 ID**：组件内部使用 `#captcha-div` 作为容器 ID，确保页面中不冲突
6. **生命周期**：组件会自动管理验证码的初始化和销毁，无需手动处理

## 🐛 常见问题

### Q1: TypeScript 报错 "window.initTAC 不存在"

**解决方案**：检查 `env.d.ts` 文件是否包含以下声明：

```typescript
declare global {
  interface Window {
    initTAC: (path: string, config: any, style: any) => Promise<any>
  }
}

export {}
```

### Q2: 验证码无法显示

**检查项**：
- 确认 `public/static/tac` 目录存在且包含完整资源
- 确认 `VITE_CAPTCHA_URL` 配置正确且服务可访问
- 检查浏览器控制台是否有网络请求错误

### Q3: 弹窗模式下关闭按钮无效

**解决方案**：确保 `btnCloseFun` 回调正确配置，组件已正确处理关闭逻辑。

### Q4: 如何自定义验证码样式？

**解决方案**：通过 `options` 属性传递样式配置，或修改组件内部的 `style` 对象。

## 📝 更新日志

### v1.0.0
- ✨ 初始版本发布
- ✅ 支持内置和弹窗两种模式
- ✅ 支持四种验证码类型
- ✅ TypeScript 类型支持
- ✅ 完整的生命周期管理

## 📄 License

MIT

---

**技术支持**：如有问题，请联系前端团队或查阅天爱验证码官方文档。
