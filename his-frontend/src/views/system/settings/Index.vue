<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ElMessage } from 'element-plus'
import type { SettingDefinition } from '@/apis/system/settings'
import SearchPanel from '@/components/common/SearchPanel.vue'
import DataTablePanel from '@/components/common/DataTablePanel.vue'
import ConfigDialog from './components/config-dialog.vue'
import { initSettingDefinitions } from '@/apis/system/settings'

defineOptions({ name: 'SettingsManagement' })

const allDefinitions = ref<SettingDefinition[]>([])
const tableData = ref<SettingDefinition[]>([])
const loading = ref(false)
const total = ref(0)
const keyword = ref('')
const page = ref(1)
const pageSize = ref(10)

async function handleQuery() {
  loading.value = true
  try {
    const kw = (keyword.value || '').trim().toLowerCase()
    const filtered = kw
      ? allDefinitions.value.filter(
          (x) =>
            (x.name || '').toLowerCase().includes(kw) ||
            (x.title || '').toLowerCase().includes(kw)
        )
      : allDefinitions.value
    total.value = filtered.length
    const start = (page.value - 1) * pageSize.value
    tableData.value = filtered.slice(start, start + pageSize.value)
  } catch {
    ElMessage.error('加载数据失败')
    tableData.value = []
    total.value = 0
  } finally {
    loading.value = false
  }
}

async function loadInit() {
  loading.value = true
  try {
    allDefinitions.value = await initSettingDefinitions()
  } catch {
    allDefinitions.value = []
  } finally {
    loading.value = false
  }
}

function handleReset() {
  keyword.value = ''
  page.value = 1
  handleQuery()
}

function handleSearch() {
  page.value = 1
  handleQuery()
}

const dialogVisible = ref(false)
const currentDefinition = ref<SettingDefinition | null>(null)

function handleConfig(row: SettingDefinition) {
  currentDefinition.value = { ...row }
  dialogVisible.value = true
}

onMounted(async () => {
  await loadInit()
  handleQuery()
})
</script>

<template>
  <div class="settings-page">
    <SearchPanel
      v-model:keyword="keyword"
      :keyword-label="'关键字'"
      :keyword-placeholder="'请输入名称或标题'"
      :loading="loading"
      @search="handleSearch"
      @reset="handleReset"
    />

    <DataTablePanel
      :data="tableData"
      :loading="loading"
      :total="total"
      v-model:page="page"
      v-model:page-size="pageSize"
      show-index
    >
      <el-table-column prop="title" label="标题" min-width="200" show-overflow-tooltip />
      <el-table-column prop="name" label="名称" min-width="200" show-overflow-tooltip />
      <el-table-column prop="description" label="描述" min-width="200" show-overflow-tooltip />
      <el-table-column label="操作" width="100" fixed="right" align="center">
        <template #default="{ row }">
          <el-button type="primary" link @click.stop="handleConfig(row as SettingDefinition)">配置</el-button>
        </template>
      </el-table-column>
    </DataTablePanel>

    <ConfigDialog v-model:visible="dialogVisible" :definition="currentDefinition" />
  </div>
</template>

<style scoped>
.settings-page {
  display: flex;
  flex-direction: column;
  gap: 12px;
  padding: 16px;
}
</style>
