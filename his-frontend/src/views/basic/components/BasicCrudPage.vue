<template>
  <section class="basic-crud-page" v-loading="loading">
    <MySearch
      :formtitle="`${title}查询`"
      :model="query"
      :items="searchItems"
      :formattr="{ 'label-suffix': ':', 'show-count': 3 }"
      @model-change="handleSearchModelChange"
      @do-search="handleSearch"
    />

    <MyTable
      :tabtitle="`${title}列表`"
      :istabseq="true"
      :tabattr="tableAttr"
      :tabdatacolumns="tableColumnsWithActions"
      :taboperbtns="operationButtons"
      :tabdata="tableData"
      @taboper-click="handleOperation"
      @page-change="handlePageChange"
      @row-dbclick="openView"
    >
      <template #header>
        <el-text type="info">{{ description }}</el-text>
        <el-button type="primary" icon="IconPlus" @click="openCreate">新增</el-button>
        <el-button icon="IconRefresh" @click="loadData">刷新</el-button>
      </template>
    </MyTable>

    <MyFormDialog ref="dialogRef" :="dialogProps" @confirm="handleSubmit" />
  </section>
</template>

<script setup lang="ts">
import { computed, nextTick, onMounted, reactive, ref } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import MySearch from '@/components/mysearch/MySearch.vue'
import MyTable from '@/components/mytable/MyTable.vue'
import MyFormDialog from '@/components/mydialog/MyFormDialog.vue'
import type { MyFormDialogProps } from '@/components/mydialog/type'
import type { MyFormItemAttr } from '@/components/myform/type'
import {
  createMyTableOperationsColumn,
  createPageDTO,
  type MyTableAttr,
  type MyTableColumn,
  type MyTableOperationsBtn,
  type PageDTO
} from '@/components/mytable/type'
import type { BasicCrudRecord, BasicCrudService } from './basic-crud'

const props = withDefaults(
  defineProps<{
    title: string
    description?: string
    service: BasicCrudService
    columns: MyTableColumn[]
    formItems: MyFormItemAttr[]
    searchItems?: MyFormItemAttr[]
    defaultForm: Record<string, unknown>
    resolveEnabled?: (row: BasicCrudRecord) => boolean
    dialogWidth?: string
  }>(),
  {
    description: '',
    searchItems: () => [
      {
        type: 'input',
        prop: 'keyword',
        label: '关键词',
        fprops: { placeholder: '请输入名称或编码', clearable: true }
      }
    ],
    resolveEnabled: (row: BasicCrudRecord) => {
      if (typeof row.enabled === 'boolean') return row.enabled
      if (row.activeFlag !== undefined) return row.activeFlag === 1 || row.activeFlag === true
      return row.statusEnum === 'ACTIVE'
    },
    dialogWidth: '720px'
  }
)

const query = reactive({ keyword: '', pageIndex: 1, pageSize: 10 })
const tableData = ref<PageDTO<BasicCrudRecord>>(createPageDTO())
const loading = ref(false)
const dialogRef = ref<{ openDialog: (reload?: boolean) => void; closeDialog: () => void } | null>(null)
const editingId = ref('')
const dialogMode = ref<'create' | 'edit' | 'view'>('create')
let requestSequence = 0

const tableAttr: MyTableAttr = {
  border: true,
  stripe: true,
  'row-key': 'id',
  'max-height': 620
}

const tableColumnsWithActions = computed<MyTableColumn[]>(() => [
  ...props.columns,
  createMyTableOperationsColumn({ 'min-width': props.service.setEnabled ? 300 : 230 })
])

const operationButtons = computed<MyTableOperationsBtn[]>(() => {
  const buttons: MyTableOperationsBtn[] = [
    { evtname: 'view', text: '查看', attr: { type: 'primary', link: true } },
    { evtname: 'edit', text: '编辑', attr: { type: 'success', link: true } }
  ]
  if (props.service.setEnabled) {
    buttons.push({ evtname: 'toggle', text: '启/停', attr: { type: 'warning', link: true } })
  }
  buttons.push({ evtname: 'delete', text: '删除', attr: { type: 'danger', link: true } })
  return buttons
})

const dialogProps = reactive<MyFormDialogProps<BasicCrudRecord>>({
  title: `新增${props.title}`,
  width: props.dialogWidth,
  submitText: '保存',
  cancelText: '取消',
  reset: true,
  data: reactive<BasicCrudRecord>({ id: '' }),
  formattr: { 'label-width': '120px', 'label-suffix': ':', disabled: false },
  formitemdata: props.formItems
})

function errorMessage(error: unknown, fallback: string) {
  return error instanceof Error ? error.message : fallback
}

async function loadData() {
  const sequence = ++requestSequence
  loading.value = true
  try {
    const result = await props.service.page({ ...query })
    if (sequence !== requestSequence) return
    tableData.value = {
      pageIndex: result.pageIndex,
      pageSize: result.pageSize,
      total: result.total,
      pages: result.pages,
      rows: result.rows
    }
  } catch (error) {
    if (sequence !== requestSequence) return
    tableData.value = createPageDTO({ pageIndex: query.pageIndex, pageSize: query.pageSize })
    ElMessage.error(errorMessage(error, `${props.title}列表加载失败`))
  } finally {
    if (sequence === requestSequence) loading.value = false
  }
}

function handleSearchModelChange(prop: string, value: unknown) {
  if (prop === 'keyword') query.keyword = String(value ?? '')
  else (query as Record<string, unknown>)[prop] = value
}

function handleSearch() {
  query.pageIndex = 1
  void loadData()
}

function handlePageChange(page: PageDTO<BasicCrudRecord>) {
  query.pageIndex = page.pageIndex
  query.pageSize = page.pageSize
  void loadData()
}

async function showDialog(mode: 'create' | 'edit' | 'view', row?: BasicCrudRecord) {
  dialogMode.value = mode
  editingId.value = row?.id ?? ''
  dialogProps.title = `${mode === 'create' ? '新增' : mode === 'edit' ? '编辑' : '查看'}${props.title}`
  dialogProps.formattr = { ...dialogProps.formattr, disabled: mode === 'view' }
  const data = (mode === 'create' ? { ...props.defaultForm, id: '' } : row) as Record<
    string,
    unknown
  >
  const dialogData = dialogProps.data as Record<string, unknown>
  Object.assign(dialogData, data)
  for (const key of Object.keys(dialogData)) {
    if (!(key in data)) delete dialogData[key]
  }
  await nextTick()
  dialogRef.value?.openDialog(true)
}

function openCreate() {
  void showDialog('create')
}

async function loadDetail(row: BasicCrudRecord) {
  return props.service.detail ? await props.service.detail(row.id) : row
}

async function openView(row: BasicCrudRecord) {
  try {
    await showDialog('view', await loadDetail(row))
  } catch (error) {
    ElMessage.error(errorMessage(error, `${props.title}详情加载失败`))
  }
}

async function handleOperation(_index: number, row: BasicCrudRecord, action: string) {
  if (action === 'view') return openView(row)
  if (action === 'edit') {
    try {
      await showDialog('edit', await loadDetail(row))
    } catch (error) {
      ElMessage.error(errorMessage(error, `${props.title}详情加载失败`))
    }
    return
  }
  if (action === 'toggle' && props.service.setEnabled) {
    const enabled = !props.resolveEnabled(row)
    try {
      await ElMessageBox.confirm(`确认${enabled ? '启用' : '停用'}该${props.title}？`, '状态确认', {
        type: 'warning'
      })
      await props.service.setEnabled(row.id, enabled)
      ElMessage.success(`${props.title}已${enabled ? '启用' : '停用'}`)
      await loadData()
    } catch (error) {
      if (error !== 'cancel') ElMessage.error(errorMessage(error, `${props.title}状态修改失败`))
    }
    return
  }
  if (action === 'delete') {
    try {
      await ElMessageBox.confirm(`确认删除该${props.title}？删除后无法恢复。`, '删除确认', {
        type: 'warning'
      })
      await props.service.remove(row.id)
      ElMessage.success(`${props.title}已删除`)
      if ((tableData.value.rows?.length ?? 0) === 1 && query.pageIndex > 1) query.pageIndex -= 1
      await loadData()
    } catch (error) {
      if (error !== 'cancel') ElMessage.error(errorMessage(error, `${props.title}删除失败`))
    }
  }
}

async function handleSubmit(data: BasicCrudRecord) {
  loading.value = true
  try {
    const payload: Record<string, unknown> = { ...data }
    delete payload.id
    if (dialogMode.value === 'edit' && editingId.value) {
      await props.service.update(editingId.value, payload)
      ElMessage.success(`${props.title}修改成功`)
    } else {
      await props.service.create(payload)
      ElMessage.success(`${props.title}新增成功`)
    }
    dialogRef.value?.closeDialog()
    await loadData()
  } catch (error) {
    ElMessage.error(errorMessage(error, `${props.title}保存失败`))
  } finally {
    loading.value = false
  }
}

onMounted(() => void loadData())
</script>

<style scoped>
.basic-crud-page {
  box-sizing: border-box;
  display: flex;
  min-height: 100%;
  padding: 16px;
  flex-direction: column;
  gap: 14px;
}
</style>
