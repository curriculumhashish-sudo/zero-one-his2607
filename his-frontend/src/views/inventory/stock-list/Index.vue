<template>
  <div class="stock-list-page">
    <!-- ==================== 查询区 ==================== -->
    <el-form :model="queryParams" label-width="auto">
      <el-row :gutter="20">
        <el-col v-for="item in visibleFormItems" :key="item.prop" :span="6">
          <el-form-item :label="item.label + '：'">
            <el-input
              v-if="item.type === 'input'"
              v-model="queryParams[item.prop]"
              :placeholder="item.placeholder"
              :clearable="item.clearable ?? true"
            />
            <el-select
              v-else
              v-model="queryParams[item.prop]"
              :placeholder="item.placeholder"
              :multiple="item.multiple"
              :clearable="item.clearable ?? true"
              :collapse-tags="item.collapseTags"
              :filterable="item.filterable"
            >
              <el-option
                v-for="opt in item.options"
                :key="opt.value"
                :label="opt.label"
                :value="opt.value"
              />
            </el-select>
          </el-form-item>
        </el-col>
      </el-row>

      <el-row justify="end" style="margin-top: 8px">
        <el-button type="primary" :icon="Search" @click="handleSearch">搜索</el-button>
        <el-button :icon="RefreshRight" @click="handleReset">重置</el-button>
        <el-button v-if="expanded" link type="primary" @click="toggleExpand">
          收起<el-icon><ArrowUp /></el-icon>
        </el-button>
        <el-button v-else link type="primary" @click="toggleExpand">
          展开<el-icon><ArrowDown /></el-icon>
        </el-button>
      </el-row>

      <el-row style="margin: 10px 0">
        <el-button type="danger" plain :icon="Download" @click="handleExport">导出Excel</el-button>
      </el-row>
    </el-form>

    <!-- ==================== 表单区 ==================== -->
    <el-table
      :data="tableData"
      :loading="loading"
      border
      stripe
      scrollbar-always-on
      max-height="calc(100vh - 400px)"
      style="width: 100%"
    >
      <el-table-column
        v-for="col in tableColumnConfigs"
        :key="col.prop || col.label"
        v-bind="col"
      />
      <!-- 操作列 -->
      <el-table-column label="操作" width="180" fixed="right" align="center" headerAlign="center">
        <template #default="{ row }">
          <el-button link type="primary" @click="handleTrace(row)">追溯码</el-button>
          <el-button
            v-if="row.inventoryStatusEnum !== 'STOP_SUPPLY'"
            link
            type="danger"
            @click="handleRetire(row)"
          >
            停供
          </el-button>
          <el-button v-else link type="primary" @click="handleResume(row)"> 取消停供 </el-button>
        </template>
      </el-table-column>
    </el-table>

    <!-- ==================== 合计 + 分页 ==================== -->
    <div class="footer">
      <div class="sum-row">
        <span
          >进价合计：<strong>{{ fmtSum(sumLineCostTotal(tableData)) }}</strong></span
        >
        <span
          >售价合计：<strong>{{ fmtSum(sumLinePriceTotal(tableData)) }}</strong></span
        >
      </div>
      <el-pagination
        :current-page="pagination.pageNo"
        :page-size="pagination.pageSize"
        :page-sizes="[10, 20, 50, 100]"
        :total="total"
        background
        layout="total, sizes, prev, pager, next, jumper"
        @current-change="handlePageChange"
        @size-change="handleSizeChange"
      />
    </div>

    <!-- 追溯码弹窗 -->
    <el-dialog v-model="traceVisible" title="追溯码" width="420px" :close-on-click-modal="true">
      <el-input :model-value="traceText" readonly type="textarea" :rows="4" />
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { computed, onMounted, reactive, ref } from 'vue'
import { Search, RefreshRight, Download, ArrowUp, ArrowDown } from '@element-plus/icons-vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import type { StockListQuery } from '@/apis/inventory/stock-list'
import { queryStockList, retireItem, resumeItem, exportStockList } from '@/apis/inventory/stock-list'
import { formFieldConfigs, tableColumnConfigs, sumLineCostTotal, sumLinePriceTotal } from './type'

defineOptions({ name: 'InventoryStockListPage' })

// ==================== 查询参数 ====================
const queryParams = reactive<Record<string, any>>(
  formFieldConfigs.reduce(
    (acc, f) => {
      acc[f.prop] = f.multiple ? [] : ''
      return acc
    },
    {} as Record<string, any>
  )
)

// ==================== 展开/收起 ====================
const VISIBLE_COUNT = 8
const expanded = ref(false)
const visibleFormItems = computed(() =>
  expanded.value ? formFieldConfigs : formFieldConfigs.slice(0, VISIBLE_COUNT)
)
function toggleExpand() {
  expanded.value = !expanded.value
}

// ==================== 表格数据 & 列 ====================
const tableData = ref<any[]>([])
const total = ref(0)
const loading = ref(false)

// ==================== 分页 ====================
const pagination = reactive({ pageNo: 1, pageSize: 10 })

// ==================== 构建查询 ====================
function buildQuery(): StockListQuery {
  const ed = queryParams.expireWithinDays?.trim()
  return {
    pageNo: pagination.pageNo,
    pageSize: pagination.pageSize,
    keyword: queryParams.keyword?.trim() || undefined,
    warehouseIds: emptyToUndef(queryParams.warehouseIds),
    supplierIds: emptyToUndef(queryParams.supplierIds),
    medicationCategoryCodes: emptyToUndef(queryParams.medicationCategoryCodes),
    deviceCategoryCodes: emptyToUndef(queryParams.deviceCategoryCodes),
    inventoryStatusEnums: emptyToUndef(queryParams.inventoryStatusEnums),
    chrgitmLvList: emptyToUndef(queryParams.chrgitmLvList),
    expireWithinDays: ed && /^\d+$/.test(ed) ? Number(ed) : undefined,
    zeroStockOnly:
      queryParams.zeroStockFilter === '数量等于0'
        ? true
        : queryParams.zeroStockFilter === '数量大于0'
          ? false
          : undefined
  }
}

// ==================== 数据获取 ====================
async function fetchData() {
  loading.value = true
  try {
    const res = await queryStockList(buildQuery())
    tableData.value = res.rows
    total.value = res.total
  } finally {
    loading.value = false
  }
}

// ==================== 事件处理 ====================
function handleSearch() {
  pagination.pageNo = 1
  fetchData()
}

function handleReset() {
  formFieldConfigs.forEach((f) => {
    queryParams[f.prop] = f.multiple ? [] : ''
  })
  pagination.pageNo = 1
  fetchData()
}

function handlePageChange(page: number) {
  pagination.pageNo = page
  fetchData()
}

function handleSizeChange(size: number) {
  pagination.pageSize = size
  pagination.pageNo = 1
  fetchData()
}

async function handleExport() {
  try {
    ElMessage.info('正在生成导出文件...')
    const blob = await exportStockList(buildQuery())
    if (!blob || blob.size === 0) {
      ElMessage.warning('暂无数据可导出')
      return
    }
    const url = URL.createObjectURL(blob)
    const link = document.createElement('a')
    link.href = url
    link.download = `库存明细_${new Date().toISOString().slice(0, 10)}.xlsx`
    link.click()
    URL.revokeObjectURL(url)
    ElMessage.success('导出成功')
  } catch (e) {
    console.error('export error:', e)
    ElMessage.error('导出失败')
  }
}

// ==================== 追溯码 ====================
const traceVisible = ref(false)
const traceText = ref('')

function handleTrace(row: any) {
  if (!row.traceNo?.trim()) {
    ElMessage.info('当前行无追溯码')
    return
  }
  traceText.value = row.traceNo
  traceVisible.value = true
}

// ==================== 停供 / 取消停供 ====================
function toggleRowStatus(id: string, status: 'STOP_SUPPLY' | 'NORMAL') {
  const idx = tableData.value.findIndex((r: any) => r.id === id)
  if (idx === -1) return
  tableData.value.splice(idx, 1, {
    ...tableData.value[idx],
    inventoryStatusEnum: status,
    inventoryStatusLabel: status === 'STOP_SUPPLY' ? '停供' : '正常'
  })
}

async function handleRetire(row: any) {
  try {
    await ElMessageBox.confirm(`确认对【${row.name}】停供？`, '停供', {
      type: 'warning',
      confirmButtonText: '确认停供',
      cancelButtonText: '取消'
    })
    await retireItem(row.id)
    toggleRowStatus(row.id, 'STOP_SUPPLY')
    ElMessage.success('停供成功')
  } catch {
    /* 取消或报错由框架处理 */
  }
}

async function handleResume(row: any) {
  try {
    await ElMessageBox.confirm(`确认恢复【${row.name}】供应？`, '取消停供', {
      type: 'info',
      confirmButtonText: '确认恢复',
      cancelButtonText: '取消'
    })
    await resumeItem(row.id)
    toggleRowStatus(row.id, 'NORMAL')
    ElMessage.success('已恢复供应')
  } catch {
    /* 取消或报错由框架处理 */
  }
}

// ==================== 工具函数 ====================
function fmtSum(val: number): string {
  return val.toFixed(4)
}
function emptyToUndef(arr?: string[]): string[] | undefined {
  return arr?.length ? arr : undefined
}

// ==================== 初始化 ====================
onMounted(() => {
  fetchData()
})
</script>

<style scoped>
.stock-list-page {
  padding: 16px;
  height: 100%;
  display: flex;
  flex-direction: column;
}

.footer {
  display: flex;
  flex-direction: column;
  align-items: flex-end;
  margin-top: 12px;
  gap: 8px;
}

.sum-row {
  display: flex;
  gap: 24px;
  font-size: 13px;
  color: var(--el-text-color-regular);
}
.sum-row strong {
  color: var(--el-text-color-primary);
}
</style>
