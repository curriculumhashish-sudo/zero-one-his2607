---
name: "crud-page-generator"
description: "根据数据模型自动生成 CRUD 页面代码，包括搜索表单、数据表格、表单对话框以及路由注册。当用户需要新增一个管理页面、增删改查页面或列表页面时调用。"
---

# CRUD 页面生成器

根据用户提供的数据模型，自动生成符合项目规范的完整 CRUD 页面代码。

## 适用场景

- 用户需要新增一个后台管理页面
- 用户需要创建一个带搜索、表格、表单对话框的列表页
- 用户描述了一个数据模型，希望快速生成增删改查页面

## 项目技术栈与规范

- **框架**: Vue 3 + TypeScript + Element Plus
- **状态管理**: Pinia
- **HTTP 客户端**: `useHttp()` 从 `@/plugins/http` 导入
- **路径别名**: `@` 映射到 `src/`

## 生成文件清单

对于模块名 `{module}`，需要生成以下文件：

| 文件 | 路径 |
|------|------|
| API 类型定义 | `src/apis/{module}/type.ts` |
| API 接口函数 | `src/apis/{module}/index.ts` |
| CRUD 页面组件 | `src/views/{module}/Index.vue` |
| 路由注册（新建） | `src/router/main/{module}/index.ts` |
| 路由注册（修改） | `src/router/main/index.ts`（追加模块路由） |

## 核心组件说明

项目封装了以下通用组件，CRUD 页面必须使用：

### MySearch — 搜索表单
- 组件路径: `@/components/mysearch/MySearch.vue`
- 关键类型: `MySearchProps<T>`, `MyFormItemAttr`, `MyFormAttr`
- 类型路径: `@/components/mysearch/type.ts`, `@/components/myform/type.ts`

### MyTable — 数据表格
- 组件路径: `@/components/mytable/MyTable.vue`
- 关键类型: `MyTableColumn`, `MyTableOperationsColumn`, `MyTableOperationsBtn`, `PageDTO`, `MyTableAttr`
- 类型路径: `@/components/mytable/type.ts`
- 关键工厂函数: `createPageDTO()`, `createMyTableOperationsColumn()`

### MyFormDialog — 表单对话框
- 组件路径: `@/components/mydialog/MyFormDialog.vue`
- 关键类型: `MyFormDialogProps`
- 类型路径: `@/components/mydialog/type.ts`

## 生成规范

### 1. API 类型定义 (`src/apis/{module}/type.ts`)

```typescript
import type { PageQuery } from '../type'

/**
 * {模块中文名}数据模型
 */
export interface {Module}VO {
  /** 主键 */
  id?: number
  // ... 根据用户提供的字段生成，每个字段必须包含 JSDoc 注释
}

/**
 * {模块中文名}查询参数
 */
export interface {Module}Query extends PageQuery {
  // ... 可用于搜索的字段
}
```

### 2. API 接口函数 (`src/apis/{module}/index.ts`)

遵循项目回调模式，分页查询使用 `success`/`fail` 回调：

```typescript
import { useHttp, type RequestCallback } from '@/plugins/http'
import type { PageDTO } from '../type'
import type { {Module}VO, {Module}Query } from './type'

const baseUrl = '/{module-path}'

/**
 * 分页查询{模块中文名}
 */
export const list{Module} = async (
  data: {Module}Query,
  success: RequestCallback,
  fail: RequestCallback
) => {
  try {
    const http = useHttp()
    const res = await http.get<PageDTO<{Module}VO>>(baseUrl + '/query-all', data, {
      showLoading: true
    })
    if (res.data) {
      success(res.data)
      return
    }
    fail(res)
  } catch (err) {
    fail(err)
  }
}

/**
 * 新增{模块中文名}
 */
export const add{Module} = async (
  data: {Module}VO,
  success: RequestCallback,
  fail: RequestCallback
) => {
  try {
    const http = useHttp()
    const res = await http.post<{Module}VO>(baseUrl + '/add', data, {
      showLoading: true
    })
    if (res.data) {
      success(res.data)
      return
    }
    fail(res)
  } catch (err) {
    fail(err)
  }
}

/**
 * 更新{模块中文名}
 */
export const update{Module} = async (
  data: {Module}VO,
  success: RequestCallback,
  fail: RequestCallback
) => {
  try {
    const http = useHttp()
    const res = await http.put<{Module}VO>(baseUrl + '/update', data, {
      showLoading: true
    })
    if (res.data) {
      success(res.data)
      return
    }
    fail(res)
  } catch (err) {
    fail(err)
  }
}

/**
 * 删除{模块中文名}
 */
export const delete{Module} = async (
  id: number,
  success: RequestCallback,
  fail: RequestCallback
) => {
  try {
    const http = useHttp()
    const res = await http.delete(baseUrl + `/delete/${id}`, undefined, {
      showLoading: true
    })
    if (res.data) {
      success(res.data)
      return
    }
    fail(res)
  } catch (err) {
    fail(err)
  }
}
```

> 注意：对于简单的获取类接口（如获取下拉选项），可使用 `async/await` 直接返回 Promise，无需回调。参考 `src/apis/sample/index.ts` 中的 `getAddress` 函数。

### 3. CRUD 页面组件 (`src/views/{module}/Index.vue`)

页面结构必须包含三部分：

#### Template 结构

```vue
<template>
  <section class="root-container">
    <!-- 搜索表单 -->
    <my-search
      :model="searchData"
      :items="searchItems"
      :formattr="searchAttr"
      :rules="searchRules"
      @model-change="onSearchModelChange"
      @do-search="doSearch"
    />

    <!-- 数据表格 -->
    <my-table
      tabtitle="{模块中文名}列表"
      :tabattr="tableAttr"
      :tabdatacolumns="tableColumns"
      :taboperbtns="tableOperBtns"
      :tabdata="tableData"
      @taboper-click="handleOperation"
      @page-change="handlePageChange"
    >
      <template #header>
        <el-button type="primary" icon="IconPlus" @click="openAddDialog">新增</el-button>
      </template>
    </my-table>

    <!-- 查看/编辑表单对话框 -->
    <my-form-dialog ref="formDialogRef" :="formDialogProps" @confirm="onFormSubmit" />
  </section>
</template>
```

#### Script 结构

```typescript
<script setup lang="ts">
import { ref, reactive, onMounted } from 'vue'
import { ElMessageBox, ElMessage } from 'element-plus'
import { cloneDeep } from 'lodash-es'

// API
import { list{Module}, add{Module}, update{Module}, delete{Module} } from '@/apis/{module}/index'
import type { {Module}VO, {Module}Query } from '@/apis/{module}/type'

// 表格组件
import MyTable from '@/components/mytable/MyTable.vue'
import {
  createPageDTO,
  createMyTableOperationsColumn,
  type PageDTO,
  type MyTableAttr,
  type MyTableColumn,
  type MyTableOperationsColumn,
  type MyTableOperationsBtn
} from '@/components/mytable/type'

// 搜索组件
import MySearch from '@/components/mysearch/MySearch.vue'
import type { MySearchProps } from '@/components/mysearch/type'
import type {
  MyFormItemAttr,
  MyFormAttr,
  MyFormInputProps,
  MyFormSelectProps,
  MyFormDatePickerProps,
  MyFormInputNumberProps
} from '@/components/myform/type'

// 表单对话框
import MyFormDialog from '@/components/mydialog/MyFormDialog.vue'
import type { MyFormDialogProps } from '@/components/mydialog/type'

// ====== 搜索表单配置 ======
const searchData = reactive<{Module}Query>({})
const searchItems = reactive<MyFormItemAttr[]>([
  // 根据可搜索字段生成
])
const searchAttr: MyFormAttr = {
  'label-suffix': ':',
  'show-count': 3
}

// ====== 表格配置 ======
const tableAttr: MyTableAttr = {
  'row-key': 'id'
}
const tableColumns: MyTableColumn[] = [
  // 根据数据模型字段生成
]
const tableOperBtns: MyTableOperationsBtn[] = [
  { evtname: 'view', text: '查看', attr: { type: 'primary' } },
  { evtname: 'edit', text: '编辑', attr: { type: 'success', icon: 'icon-edit' } },
  { evtname: 'delete', text: '删除', attr: { type: 'danger', icon: 'icon-delete' } }
]
const tableData = ref<PageDTO<{Module}VO>>(createPageDTO())

// ====== 表单对话框配置 ======
const formDialogRef = ref()
const currentEditId = ref<number>(-1)
const formDialogProps = reactive<MyFormDialogProps<{Module}VO>>({
  title: '编辑数据',
  submitText: '保存',
  cancelText: '取消',
  data: reactive<{Module}VO>({}),
  formattr: { 'label-width': '80px', disabled: false },
  formitemdata: reactive<MyFormItemAttr[]>([/* 与字段对应 */])
})

// ====== 数据加载 ======
function loadData() {
  list{Module}(
    { ...searchData, pageIndex: tableData.value.pageIndex, pageSize: tableData.value.pageSize },
    (data) => { tableData.value = data },
    (err) => { console.error(err) }
  )
}
function doSearch() { tableData.value.pageIndex = 1; loadData() }
function handlePageChange(data: PageDTO<{Module}VO>) {
  tableData.value.pageIndex = data.pageIndex
  tableData.value.pageSize = data.pageSize
  loadData()
}

// ====== CRUD 操作 ======
function openAddDialog() {
  currentEditId.value = -1
  resetFormData(formDialogProps.data as {Module}VO)
  formDialogProps.title = '新增数据'
  formDialogRef.value.openDialog(true)
}

function handleOperation(index: number, row: {Module}VO, evtname: string) {
  switch (evtname) {
    case 'view':
      formDialogProps.title = '查看详情'
      formDialogProps.formattr = { ...formDialogProps.formattr, disabled: true }
      Object.assign(formDialogProps.data, row)
      formDialogRef.value.openDialog(true)
      break
    case 'edit':
      currentEditId.value = index
      formDialogProps.title = '编辑数据'
      formDialogProps.formattr = { ...formDialogProps.formattr, disabled: false }
      Object.assign(formDialogProps.data, cloneDeep(row))
      formDialogRef.value.openDialog(true)
      break
    case 'delete':
      ElMessageBox.confirm('确认删除该数据？', '提示', {
        confirmButtonText: '确认删除',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        delete{Module}(
          row.id!,
          () => { ElMessage.success('删除成功'); loadData() },
          (err) => { console.error(err) }
        )
      }).catch(() => {})
      break
  }
}

function onFormSubmit(data: {Module}VO) {
  if (currentEditId.value >= 0) {
    update{Module}(data,
      () => { ElMessage.success('更新成功'); formDialogRef.value.closeDialog(); loadData() },
      (err) => { console.error(err) }
    )
  } else {
    add{Module}(data,
      () => { ElMessage.success('新增成功'); formDialogRef.value.closeDialog(); loadData() },
      (err) => { console.error(err) }
    )
  }
}

function resetFormData(target: {Module}VO) {
  Object.keys(target).forEach(key => delete (target as any)[key])
}

function onSearchModelChange(prop: string, val: any) {
  (searchData as any)[prop] = val
}

onMounted(() => { loadData() })
</script>

<style scoped>
.root-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 15px;
}
</style>
```

### 4. 路由注册

新建文件 `src/router/main/{module}/index.ts`：

```typescript
const routes = [
  {
    path: '/{module}',
    name: '{Module}',
    redirect: { path: '/{module}/index' },
    component: () => import('@/views/{module}/Index.vue'),
    children: [
      {
        name: '{module}List',
        path: '/{module}/index',
        component: () => import('@/views/{module}/Index.vue')
      }
    ]
  }
]

export default routes
```

然后在 `src/router/main/index.ts` 中追加导入和路由注册。

## 执行流程

当用户请求生成 CRUD 页面时，按以下步骤执行：

1. **收集信息**：确认模块名、数据字段（字段名、中文名、类型、是否可搜索、是否在表格显示）
2. **生成 API 层**：先创建 `src/apis/{module}/type.ts` 和 `src/apis/{module}/index.ts`
3. **生成页面组件**：创建 `src/views/{module}/Index.vue`
4. **注册路由**：创建或修改路由文件
5. **验证**：确认所有导入路径正确、类型引用一致

## 类型字段 → 表单组件映射

| TypeScript 类型 | 表单 type | Element Plus 组件 |
|----------------|-----------|-------------------|
| `string` | `'input'` | `el-input` |
| `number` | `'number'` | `el-input-number` |
| `string` (枚举) | `'select'` | `el-select` |
| `string` (日期) | `'date'` | `el-date-picker` |
| `boolean` | `'radio-group'` | `el-radio-group` |
| `string[]` | `'checkbox-group'` | `el-checkbox-group` |
| `string` (级联) | `'cascader'` | `el-cascader` |

## 注意事项

- 表格操作列必须使用 `createMyTableOperationsColumn()` 创建并与其他列合并
- 查看模式下必须设置 `formattr.disabled = true`
- 表单对话框使用 `reactive` 包装数据，通过 `Object.assign` 更新
- 编辑/新增切换时注意重置表单数据
- 分页组件改变时需同步更新 `tableData` 的 `pageIndex` 和 `pageSize`
- 所有导入使用 `@/` 别名路径
