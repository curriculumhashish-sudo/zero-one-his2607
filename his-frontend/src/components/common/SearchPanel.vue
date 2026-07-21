<template>
  <div class="query-shell" @keydown.enter="submitOnEnter ? handleSearch($event) : undefined">
    <!-- ==================== Header：查询条件区 ==================== -->
    <div class="query-shell__fields" :class="{ 'is-collapsed': isCollapsed }">
      <!-- 内置：关键词输入框 -->
      <div class="query-shell__field">
        <label class="query-shell__label">{{ keywordLabel }}</label>
        <el-input
          :model-value="keyword"
          :placeholder="keywordPlaceholder"
          clearable
          size="default"
          @update:model-value="emit('update:keyword', $event)"
          @clear="emit('update:keyword', '')"
        />
      </div>

      <!-- 内置：下拉菜单 -->
      <div v-if="dropdownOptions?.length" class="query-shell__field">
        <label class="query-shell__label">{{ dropdownLabel }}</label>
        <el-select
          :model-value="dropdownValue"
          :placeholder="dropdownPlaceholder"
          clearable
          size="default"
          style="width: 100%"
          @update:model-value="emit('update:dropdownValue', $event)"
          @clear="emit('update:dropdownValue', '')"
        >
          <el-option
            v-for="opt in dropdownOptions"
            :key="opt.value"
            :label="opt.label"
            :value="opt.value"
          />
        </el-select>
      </div>

      <!-- 插槽：额外查询字段 -->
      <slot name="header" />
    </div>

    <!-- 展开/收起 -->
    <div v-if="isCollapsible" class="query-shell__collapse-bar">
      <el-button text size="small" @click="toggleCollapse">
        <span>{{ isCollapsed ? '展开更多' : '收起' }}</span>
        <el-icon :class="{ 'is-reverse': !isCollapsed }">
          <ArrowDown />
        </el-icon>
      </el-button>
    </div>

    <!-- ==================== Body：操作按钮区 ==================== -->
    <div class="query-shell__body">
      <slot name="body">
        <el-button
          v-if="showSearch"
          type="primary"
          size="default"
          :loading="loading"
          @click="handleSearch"
        >
          <el-icon><Search /></el-icon>
          查询
        </el-button>
        <el-button
          v-if="showReset"
          size="default"
          @click="handleReset"
        >
          <el-icon><Refresh /></el-icon>
          重置
        </el-button>
      </slot>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, watch, onMounted, nextTick } from 'vue'
import { Search, Refresh, ArrowDown } from '@element-plus/icons-vue'

// ==================== Props ====================

const props = withDefaults(
  defineProps<{
    /** 关键词字段标签 */
    keywordLabel?: string
    /** 关键词 v-model */
    keyword?: string
    /** 关键词占位文本 */
    keywordPlaceholder?: string
    /** 下拉字段标签 */
    dropdownLabel?: string
    /** 下拉选项 */
    dropdownOptions?: { label: string; value: string | number }[]
    /** 下拉选中值 v-model */
    dropdownValue?: string | number | null
    /** 下拉占位文本 */
    dropdownPlaceholder?: string
    /** 查询按钮 loading */
    loading?: boolean
    /** 是否显示查询按钮 */
    showSearch?: boolean
    /** 是否显示重置按钮 */
    showReset?: boolean
    /** 是否启用回车提交 */
    submitOnEnter?: boolean
    /** #header 插槽中额外的字段数量（用于自动判断展开/收起） */
    fieldCount?: number
  }>(),
  {
    keywordLabel: '关键词',
    keyword: '',
    keywordPlaceholder: '请输入关键词',
    dropdownLabel: '类别',
    dropdownOptions: () => [],
    dropdownValue: null,
    dropdownPlaceholder: '请选择',
    loading: false,
    showSearch: true,
    showReset: true,
    submitOnEnter: true,
    fieldCount: 0
  }
)

// ==================== Emits ====================

const emit = defineEmits<{
  'update:keyword': [value: string]
  'update:dropdownValue': [value: string | number | null]
  search: []
  reset: []
}>()

// ==================== 折叠逻辑 ====================

const TOTAL_HEADER_ITEMS = computed(() => {
  let count = 1 // 关键词输入框始终存在
  if (props.dropdownOptions?.length) count++ // 下拉菜单
  count += props.fieldCount // #header 插槽中的额外字段
  return count
})

/** 4 个及以上字段时支持展开/收起 */
const isCollapsible = computed(() => TOTAL_HEADER_ITEMS.value >= 4)

const isCollapsed = ref(true)

watch(isCollapsible, (val) => {
  if (!val) isCollapsed.value = false
})

function toggleCollapse() {
  isCollapsed.value = !isCollapsed.value
}

// ==================== 溢出遮罩 ====================

const hasOverflow = ref(false)

onMounted(() => {
  nextTick(() => checkOverflow())
})

function checkOverflow() {
  if (!isCollapsible.value || !isCollapsed.value) {
    hasOverflow.value = false
    return
  }
  const container = document.querySelector('.query-shell__fields.is-collapsed')
  if (container) {
    hasOverflow.value = container.scrollHeight > container.clientHeight
  }
}

// ==================== 按钮事件 ====================

function handleSearch(event?: Event) {
  if (event) event.preventDefault()
  emit('search')
}

function handleReset() {
  emit('reset')
}
</script>

<style scoped>
/* ==================== 根容器 ==================== */
.query-shell {
  background: var(--el-bg-color);
  border-radius: var(--el-border-radius-base);
  padding: 16px;
}

/* ==================== Header：查询条件网格 ==================== */
.query-shell__fields {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 12px;
  overflow: hidden;
  transition: max-height 0.35s ease;
}

.query-shell__fields.is-collapsed {
  max-height: 48px;
}


/* ==================== 展开/收起 ==================== */
.query-shell__collapse-bar {
  display: flex;
  justify-content: center;
  padding-top: 8px;
}

.query-shell__collapse-bar .el-icon {
  font-size: 14px;
  transition: transform 0.3s ease;
}

.query-shell__collapse-bar .el-icon.is-reverse {
  transform: rotate(180deg);
}

/* ==================== Body：操作按钮 ==================== */
.query-shell__body {
  display: flex;
  align-items: center;
  gap: 12px;
  padding-top: 12px;
  margin-top: 12px;
  border-top: 1px solid var(--el-border-color-lighter);
}

/* ==================== 响应式 ==================== */
@media (max-width: 768px) {
  .query-shell__fields {
    grid-template-columns: repeat(2, 1fr);
  }

  .query-shell__body {
    flex-wrap: wrap;
    gap: 8px;
  }
}
</style>

<style>
/* 非 scoped：供 #header 插槽中的自定义字段复用布局样式 */
.query-shell__field {
  display: flex;
  align-items: center;
  gap: 8px;
  min-width: 0;
}

.query-shell__label {
  flex-shrink: 0;
  font-size: 13px;
  font-weight: 500;
  color: var(--el-text-color-regular);
  white-space: nowrap;
}
</style>
