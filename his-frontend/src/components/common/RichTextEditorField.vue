<template>
  <div class="rich-text-field" :style="{ minHeight: minHeight }">
    <!-- 工具栏：只读模式下隐藏 -->
    <Toolbar
      v-if="!readonly"
      style="border-bottom: 1px solid #e5e7eb"
      :editor="editorRef"
      :defaultConfig="toolbarConfig"
      :mode="mode"
    />
    <!-- 编辑器 -->
    <Editor
      :key="editorKey"
      :style="{ height: editorHeight, overflowY: 'hidden' }"
      v-model="valueHtml"
      :defaultConfig="editorConfig"
      :mode="mode"
      @onCreated="handleCreated"
      @onChange="handleChange"
      @onFocus="handleFocus"
      @onBlur="handleBlur"
    />
    <!-- 错误状态 -->
    <div v-if="error" class="rich-text-field__error">
      <span>{{ error.message }}</span>
      <button @click="handleRetry">重试</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onBeforeUnmount, ref, shallowRef, watch, computed } from 'vue'
import { Editor, Toolbar } from '@wangeditor/editor-for-vue'

// 样式：使用 @import 避免 TypeScript 报错
// 在 <style> 中已通过 @import 引入

const props = withDefaults(
  defineProps<{
    /** 受控 HTML 内容 */
    modelValue?: string
    /** 只读模式 */
    readonly?: boolean
    /** 占位文字 */
    placeholder?: string
    /** 编辑器最小高度（CSS 长度） */
    minHeight?: string
    /** 保留的工具栏按钮 */
    toolbarKeys?: string[]
    /** 排除的工具栏按钮 */
    excludeKeys?: string[]
    /** 编辑器模式：default | simple */
    mode?: 'default' | 'simple'
    /** 图片上传函数：返回图片 URL */
    uploadImage?: (file: File) => Promise<string>
  }>(),
  {
    modelValue: '',
    readonly: false,
    placeholder: '请输入内容...',
    minHeight: '300px',
    toolbarKeys: () => [],
    excludeKeys: () => ['uploadImage', 'uploadVideo'],
    mode: 'default',
    uploadImage: undefined,
  }
)

const emit = defineEmits<{
  (e: 'update:modelValue', value: string): void
  (e: 'change', value: string): void
  (e: 'focus', editor: any): void
  (e: 'blur', editor: any): void
  (e: 'ready', editor: any): void
  (e: 'error', err: Error): void
  (e: 'retry'): void
}>()

// ---------- 状态 ----------
const editorRef = shallowRef<any>(null)
const valueHtml = ref(props.modelValue)
const error = ref<Error | null>(null)
const isReady = ref(false)
const editorKey = ref(0)

// ---------- 编辑器高度计算 ----------
const editorHeight = computed(() => {
  const h = props.minHeight || '300px'
  // 如果有工具栏，额外增加工具栏高度（约 40px），避免内容被遮挡
  return props.readonly ? h : `calc(${h} + 40px)`
})

// ---------- 工具栏配置 ----------
const toolbarConfig = computed(() => {
  const config: Record<string, any> = {}
  if (props.toolbarKeys.length > 0) {
    config.toolbarKeys = props.toolbarKeys
  }
  if (props.excludeKeys.length > 0) {
    config.excludeKeys = props.excludeKeys
  }
  return config
})

// ---------- 编辑器配置 ----------
const editorConfig = computed(() => {
  const config: Record<string, any> = {
    placeholder: props.placeholder || '请输入内容...',
    readOnly: props.readonly,
  }

  // 图片上传配置（仅当传入 uploadImage 函数时启用）
  if (props.uploadImage) {
    config.MENU_CONF = {
      uploadImage: {
        // 自定义上传逻辑：不写死 URL，使用传入的函数
        customUpload: async (file: File, insertFn: (url: string, alt?: string, href?: string) => void) => {
          try {
            const url = await props.uploadImage!(file)
            insertFn(url, file.name, '')
          } catch (err) {
            console.error('图片上传失败:', err)
            const uploadError = err instanceof Error ? err : new Error(String(err))
            error.value = uploadError
            emit('error', uploadError)
          }
        },
      },
    }
  }

  return config
})

// ---------- 事件处理 ----------
const handleCreated = (editor: any) => {
  editorRef.value = editor
  isReady.value = true
  error.value = null
  emit('ready', editor)

  // 如果初始内容为空，设置空段落
  if (!valueHtml.value) {
    editor.setHtml('<p><br></p>')
  }
}

const handleChange = (editor: any) => {
  const html = editor.getHtml()
  valueHtml.value = html
  emit('update:modelValue', html)
  emit('change', html)
}

const handleFocus = (editor: any) => {
  emit('focus', editor)
}

const handleBlur = (editor: any) => {
  emit('blur', editor)
}

const handleRetry = () => {
  error.value = null
  emit('retry')
  // 重新初始化编辑器
  if (editorRef.value) {
    editorRef.value.destroy()
    editorRef.value = null
  }
  // 触发重新创建
  isReady.value = false
  editorKey.value += 1
}

// ---------- 外部值同步 ----------
watch(
  () => props.modelValue,
  (newVal: string) => {
    // 防止循环更新
    if (newVal !== valueHtml.value && editorRef.value) {
      valueHtml.value = newVal
      editorRef.value.setHtml(newVal || '<p><br></p>')
    }
  }
)

// ---------- 只读模式动态切换 ----------
watch(
  () => props.readonly,
  (val: boolean) => {
    const editor = editorRef.value
    if (editor) {
      if (val) {
        editor.disable()
      } else {
        editor.enable()
      }
    }
  }
)

// ---------- 组件卸载：销毁编辑器，清理资源 ----------
onBeforeUnmount(() => {
  const editor = editorRef.value
  if (editor) {
    editor.destroy()
    editorRef.value = null
  }
})
</script>

<style>
@import '@wangeditor/editor/dist/css/style.css';
</style>

<style scoped>
.rich-text-field {
  border: 1px solid var(--el-border-color, #d1d5db);
  border-radius: 6px;
  overflow: hidden;
  background: #ffffff;
  transition: border-color 0.2s;
}

.rich-text-field:focus-within {
  border-color: var(--el-color-primary, #409eff);
  box-shadow: 0 0 0 2px rgba(64, 158, 255, 0.2);
}

.rich-text-field__error {
  padding: 12px 16px;
  color: #f56c6c;
  background: #fef0f0;
  border-top: 1px solid #fde2e2;
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 14px;
}

.rich-text-field__error button {
  padding: 4px 16px;
  background: #f56c6c;
  color: #fff;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 13px;
}

.rich-text-field__error button:hover {
  background: #e74c4c;
}

/* 覆盖 wangEditor 默认样式，适配项目 */
.rich-text-field :deep(.w-e-toolbar) {
  border-bottom: 1px solid #e5e7eb !important;
  background: #fafafa !important;
  padding: 4px 8px !important;
}

.rich-text-field :deep(.w-e-text-container) {
  border: none !important;
}

.rich-text-field :deep(.w-e-text-container .w-e-text) {
  padding: 16px !important;
  min-height: 200px;
}

/* 只读模式样式 */
.rich-text-field--readonly :deep(.w-e-text-container .w-e-text) {
  background: #fafafa;
  cursor: default;
}
</style>
