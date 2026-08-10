<template>
  <div class="table-panel" :style="{ minHeight }">
    <!-- ==================== 表格区域 ==================== -->
    <div class="table-panel__body">
      <el-table
        ref="tableRef"
        :data="data"
        :loading="loading"
        border
        stripe
        style="width: 100%"
        @selection-change="handleSelectionChange"
        v-bind="$attrs"
      >
        <!-- 序号列 -->
        <el-table-column
          v-if="showIndex"
          type="index"
          label="序号"
          width="55"
          align="center"
          fixed="left"
        />

        <!-- 选择列 -->
        <el-table-column
          v-if="selectable"
          type="selection"
          width="50"
          align="center"
          fixed="left"
          :selectable="rowSelectable"
        />

        <!-- 空状态 -->
        <template #empty>
          <el-empty :image-size="120" description="暂无数据" />
        </template>

        <!--
          默认插槽：由父组件声明所有业务列、可编辑单元格、操作列等
          示例：
            <el-table-column prop="name" label="姓名" />
            <el-table-column prop="age" label="年龄">
              <template #default="{ row }">
                <el-input v-model="row.age" />
              </template>
            </el-table-column>
            <el-table-column label="操作" width="180">
              <template #default="{ row }">
                <el-button @click="handleEdit(row)">编辑</el-button>
              </template>
            </el-table-column>
        -->
        <slot />
      </el-table>
    </div>

    <!-- ==================== 分页区域 ==================== -->
    <div v-if="total > 0" class="table-panel__pagination">
      <el-pagination
        :current-page="page"
        :page-size="pageSize"
        :page-sizes="[10, 20, 50, 100]"
        :total="total"
        layout="total, sizes, prev, pager, next, jumper"
        background
        @update:current-page="handlePageChange"
        @update:page-size="handlePageSizeChange"
      />
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

// ============================================================
// Props
// ============================================================
interface DataTablePanelProps {
  /** 表格数据源 */
  data?: any[]
  /** 加载状态 */
  loading?: boolean
  /** 总记录数（用于分页） */
  total?: number
  /** 当前页码 */
  page?: number
  /** 每页条数 */
  pageSize?: number
  /** 容器最小高度 */
  minHeight?: string
  /** 是否显示行选择列 */
  selectable?: boolean
  /** 行是否允许被勾选 */
  rowSelectable?: (row: any, index: number) => boolean
  /** 是否显示序号列 */
  showIndex?: boolean
}

withDefaults(defineProps<DataTablePanelProps>(), {
  data: () => [],
  loading: false,
  total: 0,
  page: 1,
  pageSize: 10,
  minHeight: '400px',
  selectable: false,
  rowSelectable: undefined,
  showIndex: false
})

// ============================================================
// Emits（事件名与 props 一一对应）
// ============================================================
const emit = defineEmits<{
  /** 页码变化 */
  'update:page': [page: number]
  /** 每页条数变化 */
  'update:pageSize': [pageSize: number]
  /** 行选择变化 */
  selectable: [selection: any[]]
}>()

// ============================================================
// 模板引用
// ============================================================
const tableRef = ref()

// ============================================================
// 事件处理
// ============================================================
const handleSelectionChange = (selection: any[]) => {
  emit('selectable', selection)
}

const handlePageChange = (page: number) => {
  emit('update:page', page)
}

const handlePageSizeChange = (pageSize: number) => {
  emit('update:pageSize', pageSize)
}

// ============================================================
// 暴露给父组件
// ============================================================
defineExpose({
  /** el-table 实例，可用于 toggleRowSelection 等操作 */
  tableRef
})
</script>

<style scoped>
/* ==================== 容器 ==================== */
.table-panel {
  display: flex;
  flex-direction: column;
  background: #fff;
  border-radius: 8px;
  padding: 16px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.06);
  transition: box-shadow 0.3s ease;
}

.table-panel:hover {
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.08);
}

/* ==================== 表格主体 ==================== */
.table-panel__body {
  flex: 1;
  overflow-x: auto;
}

/* ==================== 分页 ==================== */
.table-panel__pagination {
  display: flex;
  justify-content: flex-end;
  align-items: center;
  padding-top: 14px;
  margin-top: 14px;
  border-top: 1px solid var(--el-border-color-lighter, #ebeef5);
}

/* ==================== 响应式 ==================== */
@media (max-width: 768px) {
  .table-panel {
    padding: 12px;
  }

  .table-panel__pagination {
    justify-content: center;
    flex-wrap: wrap;
  }

  .table-panel__pagination :deep(.el-pagination) {
    flex-wrap: wrap;
    justify-content: center;
    gap: 4px;
  }
}

/* ==================== 暗色模式 ==================== */
@media (prefers-color-scheme: dark) {
  .table-panel {
    background: var(--el-bg-color, #1d1e1f);
    box-shadow: 0 2px 12px rgba(0, 0, 0, 0.2);
  }

  .table-panel:hover {
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3);
  }

  .table-panel__pagination {
    border-top-color: var(--el-border-color-base, #4c4d4f);
  }
}
</style>
