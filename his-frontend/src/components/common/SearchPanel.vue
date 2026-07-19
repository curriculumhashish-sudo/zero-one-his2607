<!-- 查询区外壳 -->
<template>
  <div
    class="query-shell"
    :class="{
      'is-collapsible': collapsible,
      'is-collapsed': collapsible && collapsed
    }"
    @keydown.enter="submitOnEnter ? handleSearch($event) : undefined"
  >
    <!-- ====== 表单主体区域 ====== -->
    <el-form
      ref="formRef"
      class="query-shell__form"
      :label-width="labelWidth"
      :model="props.model"
      @submit.prevent
    >
      <!-- 插槽内容（所有表单项） -->
      <div class="query-shell__fields">
        <slot :collapsed="collapsible ? collapsed : false" :toggleCollapse="toggleCollapse">
          <!-- 默认无插槽时显示提示 -->
          <div class="query-shell__placeholder">
            <el-text type="info">请通过默认插槽传入查询字段</el-text>
          </div>
        </slot>

        <!-- 折叠渐变遮罩（仅 collapsible 且 collapsed 时显示） -->
        <div
          v-if="collapsible && collapsed && hasOverflow"
          class="query-shell__mask"
          @click="toggleCollapse"
        >
          <span class="query-shell__mask-text">
            展开更多
            <el-icon><ArrowDown /></el-icon>
          </span>
        </div>
      </div>

      <!-- ====== 按钮操作区 ====== -->
      <div class="query-shell__actions">
        <!-- 查询按钮 -->
        <el-button
          type="primary"
          :loading="loading"
          class="query-shell__btn query-shell__btn--search"
          @click="handleSearch"
        >
          <el-icon><Search /></el-icon>
          查询
        </el-button>

        <!-- 重置按钮 -->
        <el-button class="query-shell__btn query-shell__btn--reset" @click="handleReset">
          <el-icon><Refresh /></el-icon>
          重置
        </el-button>

        <!-- 展开/收起按钮 -->
        <el-button
          v-if="collapsible"
          text
          class="query-shell__btn query-shell__btn--collapse"
          @click="toggleCollapse"
        >
          <span>{{ collapsed ? '收起' : '展开' }}</span>
          <el-icon :class="{ 'is-reverse': !collapsed }">
            <ArrowUp />
          </el-icon>
        </el-button>
      </div>
    </el-form>
  </div>
</template>

<script setup lang="ts">
import { ref, watch, onMounted, nextTick, useSlots } from 'vue'
import { ElForm, ElButton, ElIcon, ElText } from 'element-plus'
import { Search, Refresh, ArrowDown, ArrowUp } from '@element-plus/icons-vue'

// ============================================================
// props 定义
// ============================================================
const props = withDefaults(
  defineProps<{
    // 查询按钮 loading 状态
    loading?: boolean
    // 是否支持展开/收起
    collapsible?: boolean
    // 当前折叠状态
    collapsed?: boolean
    // el-form label 宽度
    labelWidth?: string
    // 是否启用回车提交
    submitOnEnter?: boolean
    // 表单数据模型（传入以在查询时收集表单数据）
    model?: Record<string, any>
  }>(),
  {
    loading: false,
    collapsible: false,
    collapsed: false,
    labelWidth: '120px',
    submitOnEnter: true,
    model: () => ({})
  }
)

// ============================================================
// emits 定义
// ============================================================
const emit = defineEmits<{
  // 点击查询或回车提交时触发
  search: [values: Record<string, any>]
  // 点击重置时触发（仅触发事件，不清空表单）
  reset: []
  // 展开/收起状态变化
  'update:collapsed': [value: boolean]
}>()

// ============================================================
// 内部状态
// ============================================================
const formRef = ref<InstanceType<typeof ElForm> | null>(null)
const slots = useSlots()

// 内容是否超出容器（用于控制折叠遮罩的显示）
const hasOverflow = ref(false)

// 内容容器引用，用于检测是否溢出
const fieldsRef = ref<HTMLDivElement | null>(null)

// 监听 collapsed 变化，重新检测溢出
watch(
  () => props.collapsed,
  () => {
    nextTick(() => checkOverflow())
  }
)

// 组件挂载后检测溢出
onMounted(() => {
  nextTick(() => checkOverflow())
})

// ============================================================
// 方法
// ============================================================

// 检测内容是否溢出
function checkOverflow() {
  // 通过 DOM 检测内容高度是否超过限制
  // 由于我们使用 CSS 类的形式控制折叠，这里通过判断是否有滚动或高度超限
  const container = document.querySelector('.query-shell.is-collapsed .query-shell__fields')
  if (container) {
    const content = container.querySelector('.query-shell__fields > :first-child')
    if (content) {
      hasOverflow.value = (content as HTMLElement).scrollHeight > container.clientHeight
    } else {
      hasOverflow.value = false
    }
  } else {
    hasOverflow.value = false
  }
}

// 触发查询事件
function handleSearch(event?: Event) {
  // 防止表单默认提交行为
  if (event) {
    event.preventDefault()
  }
  // 使用 props.model 传递表单数据
  emit('search', props.model)
}

// 触发重置事件（仅触发，不清空表单）
function handleReset() {
  emit('reset')
}

// 切换展开/收起
function toggleCollapse() {
  const newVal = !props.collapsed
  emit('update:collapsed', newVal)
}

// ============================================================
// 暴露给父组件的属性和方法（通过 ref 访问）
// ============================================================
defineExpose({
  // el-form 引用
  formRef,
  // 触发查询
  search: handleSearch,
  // 触发重置
  reset: handleReset,
  // 切换折叠
  toggleCollapse
})
</script>

<style scoped>
.query-shell {
  position: relative;
  background: #fff;
  border-radius: 8px;
  padding: 20px 24px 16px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
  transition: box-shadow 0.3s ease;
}

.query-shell:hover {
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
}

/* ---------- 表单区域 ---------- */
.query-shell__form {
  width: 100%;
}

.query-shell__fields {
  position: relative;
  transition: max-height 0.35s cubic-bezier(0.4, 0, 0.2, 1);
  overflow: hidden;
}

/* 折叠状态 */
.query-shell.is-collapsed .query-shell__fields {
  max-height: 120px;
  /* 默认显示约 2 行，可根据实际调整 */
}

/* 展开状态（不限制高度） */
.query-shell:not(.is-collapsed) .query-shell__fields {
  max-height: none;
}

/* ---------- 折叠遮罩 ---------- */
.query-shell__mask {
  position: absolute;
  bottom: 0;
  left: 0;
  right: 0;
  height: 64px;
  background: linear-gradient(
    to bottom,
    rgba(255, 255, 255, 0),
    rgba(255, 255, 255, 0.95) 60%,
    #fff
  );
  display: flex;
  align-items: flex-end;
  justify-content: center;
  cursor: pointer;
  padding-bottom: 8px;
  pointer-events: auto;
  transition: opacity 0.3s ease;
}

.query-shell__mask:hover {
  opacity: 0.9;
}

.query-shell__mask-text {
  font-size: 13px;
  color: var(--el-color-primary);
  display: flex;
  align-items: center;
  gap: 4px;
  font-weight: 500;
  background: rgba(255, 255, 255, 0.8);
  padding: 2px 12px;
  border-radius: 12px;
  box-shadow: 0 1px 6px rgba(0, 0, 0, 0.06);
  transition: all 0.2s ease;
}

.query-shell__mask-text:hover {
  background: #fff;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

.query-shell__mask-text .el-icon {
  font-size: 14px;
  animation: bounce-down 1.2s ease-in-out infinite;
}

@keyframes bounce-down {
  0%,
  100% {
    transform: translateY(0);
  }
  50% {
    transform: translateY(4px);
  }
}

/* ---------- 按钮操作区 ---------- */
.query-shell__actions {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-top: 16px;
  padding-top: 14px;
  border-top: 1px solid var(--el-border-color-light, #f0f0f0);
  flex-wrap: wrap;
}

/* 按钮基础样式 */
.query-shell__btn {
  display: inline-flex;
  align-items: center;
  gap: 6px;
  font-size: 14px;
  transition: all 0.25s ease;
}

/* 查询按钮 */
.query-shell__btn--search {
  min-width: 90px;
  font-weight: 500;
  box-shadow: 0 2px 8px rgba(64, 158, 255, 0.25);
}

.query-shell__btn--search:hover {
  box-shadow: 0 4px 16px rgba(64, 158, 255, 0.35);
  transform: translateY(-1px);
}

/* 重置按钮 */
.query-shell__btn--reset {
  color: var(--el-text-color-secondary, #909399);
  border-color: var(--el-border-color, #dcdfe6);
}

.query-shell__btn--reset:hover {
  color: var(--el-text-color-primary, #303133);
  border-color: var(--el-border-color-hover, #c0c4cc);
  background: var(--el-fill-color-light, #f5f7fa);
}

/* 展开/收起按钮 */
.query-shell__btn--collapse {
  margin-left: auto;
  color: var(--el-color-primary);
  font-weight: 500;
  padding: 4px 12px;
  border-radius: 6px;
  transition: background 0.2s ease;
}

.query-shell__btn--collapse:hover {
  background: var(--el-color-primary-light-9, #ecf5ff);
}

.query-shell__btn--collapse .el-icon {
  font-size: 14px;
  transition: transform 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

.query-shell__btn--collapse .el-icon.is-reverse {
  transform: rotate(180deg);
}

/* ---------- 占位提示 ---------- */
.query-shell__placeholder {
  padding: 24px 0;
  text-align: center;
  color: var(--el-text-color-placeholder, #c0c4cc);
  font-size: 14px;
}

/* ---------- 响应式适配 ---------- */
@media (max-width: 768px) {
  .query-shell {
    padding: 16px 16px 12px;
  }

  .query-shell__actions {
    gap: 8px;
    flex-wrap: wrap;
  }

  .query-shell__btn--collapse {
    margin-left: 0;
    width: 100%;
    justify-content: center;
    order: 3;
  }

  .query-shell.is-collapsed .query-shell__fields {
    max-height: 100px;
  }
}

/* 当 collapsible 为 false 时，不显示遮罩相关样式 */
.query-shell:not(.is-collapsible) .query-shell__mask {
  display: none;
}

/* ---------- 适配 el-form 内部布局 ---------- */
.query-shell :deep(.el-form-item) {
  margin-bottom: 18px;
}

.query-shell :deep(.el-form-item__label) {
  font-weight: 500;
  color: var(--el-text-color-regular, #606266);
}

.query-shell :deep(.el-form-item__content) {
  line-height: 32px;
}

/* 当使用 el-row / el-col 布局时，保持一致性 */
.query-shell :deep(.el-row) {
  margin-left: 0 !important;
  margin-right: 0 !important;
}

.query-shell :deep(.el-row .el-col) {
  padding-left: 0 !important;
  padding-right: 0 !important;
}

/* 多个 el-row 之间增加间距 */
.query-shell :deep(.el-row + .el-row) {
  margin-top: 0;
}

/* ---------- 暗色模式适配 ---------- */
@media (prefers-color-scheme: dark) {
  .query-shell {
    background: var(--el-bg-color, #1d1e1f);
    box-shadow: 0 2px 12px rgba(0, 0, 0, 0.2);
  }

  .query-shell:hover {
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3);
  }

  .query-shell__mask {
    background: linear-gradient(
      to bottom,
      rgba(29, 30, 31, 0),
      rgba(29, 30, 31, 0.95) 60%,
      #1d1e1f
    );
  }

  .query-shell__mask-text {
    background: rgba(29, 30, 31, 0.8);
    color: var(--el-color-primary-light-3, #79bbff);
  }

  .query-shell__btn--collapse:hover {
    background: var(--el-color-primary-dark-2, #337ecc);
  }

  .query-shell__actions {
    border-top-color: var(--el-border-color-base, #4c4d4f);
  }
}
</style>
