<template>
  <section class="code-page">
    <!-- ==================== 左侧：类型管理 ==================== -->
    <aside class="code-page__left">
      <el-input
        v-model="leftKeyword"
        placeholder="请输入关键词"
        clearable
        @input="onLeftSearchInput"
      />

      <el-tabs v-model="activeSystem" class="code-page__tabs" @tab-change="onSystemChange">
        <el-tab-pane label="字典系统" name="dict" />
        <el-tab-pane label="枚举系统" name="enum" />
      </el-tabs>

      <el-button type="primary" style="width: 100%" @click="openTypeDialog('create')">
        新增
      </el-button>

      <ul v-loading="typeLoading" class="type-list">
        <li
          v-for="item in filteredTypeList"
          :key="item.id"
          class="type-list__item"
          :class="{ 'type-list__item--active': item.id === selectedTypeId }"
          @click="selectType(item.id)"
        >
          <span class="type-list__text">{{ item.title }}-{{ item.name }}</span>
        </li>
        <li v-if="!typeLoading && filteredTypeList.length === 0" class="type-list__empty">
          暂无数据
        </li>
      </ul>
    </aside>

    <!-- ==================== 右侧：编码项管理 ==================== -->
    <main class="code-page__right">
      <div class="query-bar">
        <el-input
          v-model="rightQuery.keyword"
          placeholder="请输入关键词"
          clearable
          style="width: 200px"
        />
        <el-input
          v-model="rightQuery.name"
          placeholder="请输入名称"
          clearable
          style="width: 200px"
        />
        <el-button type="primary" @click="searchItems">查询</el-button>
        <el-button @click="resetQuery">重置</el-button>
      </div>

      <el-button type="primary" :disabled="!selectedTypeId" style="align-self: flex-start" @click="openItemDialog('create')">
        新增
      </el-button>

      <el-table
        v-loading="itemLoading"
        :data="itemList"
        border
        stripe
        style="width: 100%"
        :empty-text="selectedTypeId ? '暂无数据' : '请先在左侧选择类型'"
      >
        <el-table-column label="序号" type="index" width="60" align="center" />
        <el-table-column prop="name" label="名称" min-width="120" />
        <el-table-column prop="code" label="编码" min-width="120" />
        <el-table-column prop="sort" label="排序" width="80" align="center" />
        <el-table-column prop="description" label="描述" min-width="160" show-overflow-tooltip />
        <el-table-column label="操作" width="140" fixed="right" align="center">
          <template #default="{ row }">
            <el-button type="primary" link @click="openItemDialog('edit', row)">编辑</el-button>
            <el-button type="danger" link @click="deleteItem(row)">删除</el-button>
          </template>
        </el-table-column>
      </el-table>
    </main>

    <!-- ==================== 类型表单弹框 ==================== -->
    <MyDialog
      ref="typeDialogRef"
      :data="typeForm"
      :title="typeDialogTitle"
      width="520px"
      :close-on-click-modal="false"
    >
      <template #default>
        <el-form
          ref="typeFormRef"
          :model="typeForm"
          :rules="typeFormRules"
          label-width="70px"
          status-icon
          @submit.prevent
        >
          <el-form-item label="名称" prop="name">
            <el-input v-model="typeForm.name" placeholder="请输入名称" />
          </el-form-item>
          <el-form-item label="标题" prop="title">
            <el-input v-model="typeForm.title" placeholder="请输入标题" />
          </el-form-item>
          <el-form-item label="版本" prop="version">
            <el-input v-model="typeForm.version" placeholder="请输入版本" />
          </el-form-item>
          <el-form-item label="描述" prop="description">
            <el-input
              v-model="typeForm.description"
              type="textarea"
              placeholder="请输入描述"
              :rows="3"
            />
          </el-form-item>
        </el-form>
      </template>

      <template #footer>
        <el-button :disabled="typeSaving" @click="closeTypeDialog">取消</el-button>
        <el-button type="primary" :loading="typeSaving" @click="submitTypeForm">确定</el-button>
      </template>
    </MyDialog>

    <!-- ==================== 编码项表单弹框 ==================== -->
    <MyDialog
      ref="itemDialogRef"
      :data="itemForm"
      :title="itemDialogTitle"
      width="520px"
      :close-on-click-modal="false"
    >
      <template #default>
        <el-form
          ref="itemFormRef"
          :model="itemForm"
          :rules="itemFormRules"
          label-width="70px"
          status-icon
          @submit.prevent
        >
          <el-form-item label="编码" prop="code">
            <el-input
              v-model="itemForm.code"
              placeholder="请输入编码"
              :disabled="itemFormMode === 'edit'"
            />
          </el-form-item>
          <el-form-item label="名称" prop="name">
            <el-input v-model="itemForm.name" placeholder="请输入名称" />
          </el-form-item>
          <el-form-item label="描述" prop="description">
            <el-input v-model="itemForm.description" placeholder="请输入描述" />
          </el-form-item>
          <el-form-item label="排序" prop="sort">
            <el-input-number
              v-model="itemForm.sort"
              :min="0"
              controls-position="right"
              placeholder="请输入排序号（越小越靠前）"
              style="width: 100%"
            />
          </el-form-item>
        </el-form>
      </template>

      <template #footer>
        <el-button :disabled="itemSaving" @click="closeItemDialog">取消</el-button>
        <el-button type="primary" :loading="itemSaving" @click="submitItemForm">确定</el-button>
      </template>
    </MyDialog>
  </section>
</template>

<script setup lang="ts">
import { computed, nextTick, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import type { FormInstance, FormRules } from 'element-plus'
import MyDialog from '@/components/mydialog/MyDialog.vue'
import {
  pageDictionaryTypes,
  pageDictionaryItems,
  createDictionaryType,
  createDictionaryItem,
  updateDictionaryItem,
  removeDictionaryItem,
  pageEnumTypes,
  pageEnumItems,
  createEnumType,
  createEnumItem,
  updateEnumItem,
  removeEnumItem
} from '@/apis/basic/code'
import type { CodeItemRecord, CodeTypeRecord } from '@/apis/basic/code'
import {
  createEmptyItemForm,
  createEmptyTypeForm
} from './type'
import type { CodeSystem, ItemFormMode, ItemFormModel, TypeFormMode, TypeFormModel } from './type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

// ==================== 响应式状态 ====================

const activeSystem = ref<CodeSystem>('dict')
const typeList = ref<CodeTypeRecord[]>([])
const selectedTypeId = ref<string | null>(null)
const typeLoading = ref(false)
const itemList = ref<CodeItemRecord[]>([])
const itemLoading = ref(false)

// 左侧搜索（节流相关）
const leftKeyword = ref('')
const throttledKeyword = ref('')
let throttleTimer: ReturnType<typeof setTimeout> | null = null

// 右侧查询
const rightQuery = reactive({ keyword: '', name: '' })
let requestSequence = 0

// 类型弹框
const typeDialogRef = ref<DialogExpose | null>(null)
const typeFormRef = ref<FormInstance>()
const typeFormMode = ref<TypeFormMode>('create')
const typeForm = reactive<TypeFormModel>(createEmptyTypeForm())
const typeSaving = ref(false)

// 编码项弹框
const itemDialogRef = ref<DialogExpose | null>(null)
const itemFormRef = ref<FormInstance>()
const itemFormMode = ref<ItemFormMode>('create')
const itemForm = reactive<ItemFormModel>(createEmptyItemForm())
const itemSaving = ref(false)

// ==================== 计算属性 ====================

const filteredTypeList = computed(() => {
  const kw = throttledKeyword.value.trim().toLowerCase()
  if (!kw) return typeList.value
  return typeList.value.filter(
    (t) => t.name.toLowerCase().includes(kw) || t.title.toLowerCase().includes(kw)
  )
})

const typeDialogTitle = computed(() => {
  const prefix = activeSystem.value === 'dict' ? '字典' : '枚举'
  return typeFormMode.value === 'create' ? `新增${prefix}` : `编辑${prefix}`
})

const itemDialogTitle = computed(() => {
  const prefix = activeSystem.value === 'dict' ? '字典编码' : '枚举编码'
  return itemFormMode.value === 'create' ? `新增${prefix}` : `编辑${prefix}`
})

const typeFormRules: FormRules<TypeFormModel> = {
  name: [{ required: true, message: '请输入名称', trigger: 'blur' }],
  title: [{ required: true, message: '请输入标题', trigger: 'blur' }],
  version: [{ required: true, message: '请输入版本', trigger: 'blur' }]
}

const itemFormRules: FormRules<ItemFormModel> = {
  code: [{ required: true, message: '请输入编码', trigger: 'blur' }],
  name: [{ required: true, message: '请输入名称', trigger: 'blur' }],
  description: [{ required: true, message: '请输入描述', trigger: 'blur' }]
}

// ==================== 左侧：类型加载与选择 ====================

async function loadTypes() {
  typeLoading.value = true
  try {
    const fetcher = activeSystem.value === 'dict' ? pageDictionaryTypes : pageEnumTypes
    typeList.value = await fetcher({})
  } catch (error) {
    typeList.value = []
    ElMessage.error(error instanceof Error ? error.message : '类型列表加载失败')
  } finally {
    typeLoading.value = false
  }
}

function selectType(id: string) {
  if (selectedTypeId.value === id) return
  selectedTypeId.value = id
  itemList.value = []
  void loadItems()
}

function onLeftSearchInput() {
  if (throttleTimer) clearTimeout(throttleTimer)
  throttleTimer = setTimeout(() => {
    throttledKeyword.value = leftKeyword.value
    throttleTimer = null
  }, 300)
}

function onSystemChange() {
  leftKeyword.value = ''
  throttledKeyword.value = ''
  selectedTypeId.value = null
  itemList.value = []
  void loadTypes()
}

// ==================== 左侧：类型弹框 ====================

function openTypeDialog(mode: TypeFormMode) {
  typeFormMode.value = mode
  Object.assign(typeForm, createEmptyTypeForm())
  typeDialogRef.value?.openDialog()
  nextTick(() => typeFormRef.value?.clearValidate())
}

function closeTypeDialog() {
  if (!typeSaving.value) typeDialogRef.value?.closeDialog()
}

async function submitTypeForm() {
  if (typeSaving.value) return
  const valid = await typeFormRef.value?.validate().catch(() => false)
  if (!valid) return

  typeSaving.value = true
  try {
    const payload = {
      name: typeForm.name,
      title: typeForm.title,
      version: typeForm.version,
      description: typeForm.description
    }
    const creator =
      activeSystem.value === 'dict' ? createDictionaryType : createEnumType
    await creator(payload)
    ElMessage.success('类型创建成功')
    typeDialogRef.value?.closeDialog()
    await loadTypes()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '类型保存失败')
  } finally {
    typeSaving.value = false
  }
}

// ==================== 右侧：编码项加载与查询 ====================

async function loadItems() {
  if (!selectedTypeId.value) return
  const seq = ++requestSequence
  itemLoading.value = true
  try {
    const fetcher = activeSystem.value === 'dict' ? pageDictionaryItems : pageEnumItems
    itemList.value = await fetcher({
      typeId: selectedTypeId.value,
      keyword: rightQuery.keyword,
      name: rightQuery.name
    })
  } catch (error) {
    if (seq !== requestSequence) return
    itemList.value = []
    ElMessage.error(error instanceof Error ? error.message : '编码项列表加载失败')
  } finally {
    if (seq === requestSequence) itemLoading.value = false
  }
}

function searchItems() {
  void loadItems()
}

function resetQuery() {
  rightQuery.keyword = ''
  rightQuery.name = ''
  void loadItems()
}

// ==================== 右侧：编码项弹框 ====================

function openItemDialog(mode: ItemFormMode, source?: unknown) {
  const row = source as CodeItemRecord | undefined
  itemFormMode.value = mode
  if (mode === 'edit' && row) {
    Object.assign(itemForm, {
      id: row.id,
      code: row.code,
      name: row.name,
      description: row.description,
      sort: row.sort,
      typeId: row.typeId
    })
  } else {
    Object.assign(itemForm, createEmptyItemForm(selectedTypeId.value ?? undefined))
  }
  itemDialogRef.value?.openDialog()
  nextTick(() => itemFormRef.value?.clearValidate())
}

function closeItemDialog() {
  if (!itemSaving.value) itemDialogRef.value?.closeDialog()
}

async function submitItemForm() {
  if (itemSaving.value) return
  const valid = await itemFormRef.value?.validate().catch(() => false)
  if (!valid) return

  itemSaving.value = true
  try {
    const payload = {
      code: itemForm.code,
      name: itemForm.name,
      description: itemForm.description,
      sort: itemForm.sort
    }
    if (itemFormMode.value === 'edit') {
      const updater =
        activeSystem.value === 'dict' ? updateDictionaryItem : updateEnumItem
      await updater(itemForm.id, payload)
      ElMessage.success('编码项编辑成功')
    } else {
      const creator =
        activeSystem.value === 'dict' ? createDictionaryItem : createEnumItem
      await creator(selectedTypeId.value!, payload)
      ElMessage.success('编码项创建成功')
    }
    itemDialogRef.value?.closeDialog()
    await loadItems()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '编码项保存失败')
  } finally {
    itemSaving.value = false
  }
}

async function deleteItem(source: unknown) {
  const row = source as CodeItemRecord
  try {
    await ElMessageBox.confirm('确认删除该编码项？', '提示', {
      confirmButtonText: '确定',
      cancelButtonText: '取消',
      type: 'warning'
    })
  } catch {
    return
  }
  try {
    const remover =
      activeSystem.value === 'dict' ? removeDictionaryItem : removeEnumItem
    await remover(row.id)
    ElMessage.success('编码项已删除')
    await loadItems()
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '编码项删除失败')
  }
}

// ==================== 初始化 ====================

onMounted(async () => {
  await loadTypes()
  const firstType = typeList.value[0]
  if (firstType) selectType(firstType.id)
})
</script>

<style scoped>
.code-page {
  display: flex;
  height: 100%;
  box-sizing: border-box;
}

/* ===== 左侧面板 ===== */
.code-page__left {
  display: flex;
  flex-direction: column;
  width: 260px;
  min-width: 260px;
  border-right: 1px solid #e4e7ed;
  padding: 14px;
  gap: 12px;
  box-sizing: border-box;
  background: #fafafa;
}

.code-page__tabs {
  --el-tabs-header-height: 32px;
}

.code-page__tabs :deep(.el-tabs__header) {
  margin-bottom: 0;
}

/* ===== 类型列表 ===== */
.type-list {
  flex: 1;
  list-style: none;
  margin: 0;
  padding: 0;
  overflow-y: auto;
  min-height: 0;
}

.type-list__item {
  padding: 10px 12px;
  border-radius: 6px;
  cursor: pointer;
  font-size: 13px;
  transition: background 0.15s;
  margin-bottom: 2px;
  word-break: break-all;
}

.type-list__item:hover {
  background: #e8f0fe;
}

.type-list__item--active {
  background: #d9ecff;
  color: #409eff;
  font-weight: 500;
}

.type-list__text {
  display: block;
  line-height: 1.4;
}

.type-list__empty {
  padding: 24px 0;
  text-align: center;
  color: #c0c4cc;
  font-size: 13px;
}

/* ===== 右侧面板 ===== */
.code-page__right {
  flex: 1;
  display: flex;
  flex-direction: column;
  padding: 14px 18px;
  gap: 12px;
  min-width: 0;
  box-sizing: border-box;
}

/* ===== 查询栏 ===== */
.query-bar {
  display: flex;
  align-items: center;
  gap: 10px;
  flex-wrap: wrap;
}
</style>
