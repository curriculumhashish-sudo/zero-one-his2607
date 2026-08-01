<script setup lang="ts">
import { computed, ref, watch } from 'vue'
import { ElMessage } from 'element-plus'
import {
  createSetting,
  deleteSetting,
  getProviderKeys,
  initSetting,
  pageSettings,
  updateSetting,
  type ProviderKeyOption,
  type SettingDefinition,
  type SettingRecord
} from '@/apis/system/settings'

interface Props {
  visible?: boolean
  definition?: null | SettingDefinition
}

const props = withDefaults(defineProps<Props>(), {
  visible: false,
  definition: null
})

const emit = defineEmits<{
  'update:visible': [value: boolean]
}>()

const providers = ref<Array<{ label: string; value: string }>>([])
const providerKeyOptions = ref<ProviderKeyOption[]>([])
const providerName = ref('G')
const settings = ref<SettingRecord[]>([])
const loading = ref(false)
const editingId = ref<number | null>(null)
const editingValue = ref('')
const submitting = ref(false)

const titleText = computed(() => props.definition?.title || '配置')
const nameText = computed(() => props.definition?.name || '')

const formData = ref({
  providerName: 'G',
  providerKey: '',
  value: ''
})

async function loadProviders() {
  const res = await initSetting()
  providers.value = res.providers || []
}

async function loadProviderKeys(p: string) {
  providerKeyOptions.value = p && p !== 'G' ? await getProviderKeys(p) : []
}

async function loadSettings() {
  if (!nameText.value) return
  loading.value = true
  try {
    settings.value = await pageSettings(nameText.value)
  } finally {
    loading.value = false
  }
}

async function handleSave() {
  submitting.value = true
  try {
    await createSetting({
      name: nameText.value,
      description: titleText.value,
      value: formData.value.value || '',
      providerName: formData.value.providerName,
      providerKey: formData.value.providerName === 'G' ? '' : formData.value.providerKey
    })
    ElMessage.success('保存成功')
    formData.value.value = ''
    formData.value.providerKey = ''
    await loadSettings()
  } catch (e: any) {
    ElMessage.error(e.message || '保存失败')
  } finally {
    submitting.value = false
  }
}

function startEdit(row: SettingRecord) {
  editingId.value = row.id
  editingValue.value = row.value || ''
}

function cancelEdit() {
  editingId.value = null
  editingValue.value = ''
}

async function saveEdit(row: SettingRecord) {
  try {
    await updateSetting(row.id, {
      description: titleText.value,
      value: editingValue.value || ''
    })
    ElMessage.success('更新成功')
    cancelEdit()
    await loadSettings()
  } catch (e: any) {
    ElMessage.error(e.message || '更新失败')
  }
}

async function handleDelete(row: SettingRecord) {
  try {
    await deleteSetting(row.id)
    ElMessage.success('删除成功')
    await loadSettings()
  } catch (e: any) {
    ElMessage.error(e.message || '删除失败')
  }
}

function handleProviderNameChange(val: string) {
  formData.value.providerName = val
  formData.value.providerKey = ''
  loadProviderKeys(val)
}

watch(
  () => props.visible,
  async (v) => {
    if (v) {
      formData.value.providerName = 'G'
      formData.value.providerKey = ''
      formData.value.value = ''
      providerName.value = 'G'
      await loadProviders()
      await loadProviderKeys('G')
      await loadSettings()
    }
  }
)
</script>

<template>
  <el-dialog
    :model-value="visible"
    :title="`配置：${titleText}`"
    width="820px"
    :close-on-click-modal="false"
    @update:model-value="(v: boolean) => emit('update:visible', v)"
  >
    <div class="config-layout">
      <div class="config-layout__form">
        <div class="config-tip">
          取值优先级：U > R > T > G
        </div>
        <el-form :model="formData" label-width="100px" size="default">
          <el-form-item label="标题">
            <el-input :model-value="titleText" disabled />
          </el-form-item>
          <el-form-item label="名称">
            <el-input :model-value="nameText" disabled />
          </el-form-item>
          <el-form-item label="提供者" required>
            <el-select
              v-model="formData.providerName"
              placeholder="请选择提供者"
              style="width: 100%"
              @change="handleProviderNameChange"
            >
              <el-option
                v-for="p in providers"
                :key="p.value"
                :label="p.label"
                :value="p.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item v-if="formData.providerName !== 'G'" label="提供者键" required>
            <el-select
              v-model="formData.providerKey"
              filterable
              placeholder="请选择提供者键"
              style="width: 100%"
            >
              <el-option
                v-for="k in providerKeyOptions"
                :key="k.key"
                :label="k.label"
                :value="k.key"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="值" required>
            <el-input v-model="formData.value" placeholder="请输入值" />
          </el-form-item>
        </el-form>
        <div class="config-actions">
          <el-button type="primary" :loading="submitting" @click="handleSave">保存</el-button>
        </div>
      </div>

      <div class="config-layout__list">
        <div class="config-list-title">已配置项</div>
        <div v-loading="loading" class="config-list">
          <div
            v-for="row in settings"
            :key="row.id"
            class="config-item"
          >
            <div class="config-item__info">
              <div class="config-item__provider">
                {{ row.providerName }}
                <span v-if="row.providerKey">:{{ row.providerKey }}</span>
              </div>
              <div v-if="editingId !== row.id" class="config-item__value">
                {{ row.value }}
              </div>
              <div v-else class="config-item__edit">
                <el-input v-model="editingValue" placeholder="请输入值" size="small" />
                <el-button type="primary" size="small" @click="saveEdit(row)">保存</el-button>
                <el-button size="small" @click="cancelEdit">取消</el-button>
              </div>
            </div>
            <div class="config-item__actions">
              <el-button
                v-if="editingId !== row.id"
                type="primary"
                link
                size="small"
                @click="startEdit(row)"
              >
                更新
              </el-button>
              <el-button type="danger" link size="small" @click="handleDelete(row)">删除</el-button>
            </div>
          </div>
          <div v-if="settings.length === 0" class="config-empty">暂无配置</div>
        </div>
      </div>
    </div>
  </el-dialog>
</template>

<style scoped>
.config-layout {
  display: flex;
  gap: 20px;
}

.config-layout__form {
  flex: 1;
  min-width: 0;
}

.config-layout__list {
  flex: 1;
  min-width: 300px;
  max-width: 420px;
}

.config-tip {
  font-size: 12px;
  color: #909399;
  margin-bottom: 12px;
}

.config-actions {
  display: flex;
  justify-content: flex-end;
  margin-top: 12px;
}

.config-list-title {
  font-size: 14px;
  font-weight: 600;
  margin-bottom: 8px;
  color: var(--el-text-color-primary);
}

.config-list {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.config-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  border: 1px solid #ebeef5;
  border-radius: 4px;
  padding: 8px 12px;
  gap: 12px;
}

.config-item__info {
  flex: 1;
  min-width: 0;
}

.config-item__provider {
  font-size: 13px;
  font-weight: 500;
  color: var(--el-text-color-primary);
}

.config-item__value {
  font-size: 12px;
  color: #909399;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.config-item__edit {
  display: flex;
  align-items: center;
  gap: 6px;
  margin-top: 4px;
}

.config-item__actions {
  flex-shrink: 0;
  display: flex;
  gap: 4px;
}

.config-empty {
  font-size: 12px;
  color: #909399;
  padding: 20px;
  text-align: center;
}
</style>
