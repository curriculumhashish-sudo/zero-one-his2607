<script setup lang="ts">
import type {
  FileRemoveHandler,
  FileUploadHandler,
  FileUploadStatus,
  FileUploadValue
} from './upload-type'

import { onBeforeUnmount, ref, shallowRef, watch } from 'vue'

import { ElMessage } from 'element-plus'
import type { UploadFile, UploadFiles, UploadInstance, UploadRawFile } from 'element-plus'

defineOptions({
  name: 'FileUploadField'
})

interface FileUploadFieldProps {
  modelValue?: FileUploadValue[]
  upload: FileUploadHandler
  remove?: FileRemoveHandler
  accept?: string
  maxSizeMB?: number
  limit?: number
  multiple?: boolean
  readonly?: boolean
  disabled?: boolean
  autoUpload?: boolean
  allowDuplicateName?: boolean
  downloadable?: boolean
  tip?: string
}

const props = withDefaults(defineProps<FileUploadFieldProps>(), {
  modelValue: () => [],
  remove: undefined,
  accept: '',
  maxSizeMB: 20,
  limit: 5,
  multiple: true,
  readonly: false,
  disabled: false,
  autoUpload: true,
  allowDuplicateName: false,
  downloadable: true,
  tip: ''
})

const emit = defineEmits<{
  'update:modelValue': [files: FileUploadValue[]]
  change: [files: FileUploadValue[]]
  success: [file: FileUploadValue]
  error: [error: unknown, file: FileUploadValue]
  remove: [file: FileUploadValue]
  download: [file: FileUploadValue]
  exceed: [files: File[]]
}>()

const uploadRef = ref<UploadInstance>()
const fileList = shallowRef<FileUploadValue[]>([])
const controllers = new Map<number, AbortController>()

watch(
  () => props.modelValue,
  (files) => {
    fileList.value = files.map((file) => ({ ...file }))
  },
  { immediate: true }
)

function emitFiles() {
  const files = fileList.value.map((file) => ({ ...file }))
  emit('update:modelValue', files)
  emit('change', files)
}

function updateFile(uid: number, updates: Partial<FileUploadValue>) {
  fileList.value = fileList.value.map((file) =>
    file.uid === uid ? { ...file, ...updates } : file
  )
  emitFiles()
}

function matchesAccept(file: UploadRawFile) {
  const acceptRules = props.accept
    .split(',')
    .map((rule) => rule.trim().toLocaleLowerCase())
    .filter(Boolean)

  if (acceptRules.length === 0) return true

  const fileName = file.name.toLocaleLowerCase()
  const mimeType = file.type.toLocaleLowerCase()

  return acceptRules.some((rule) => {
    if (rule.startsWith('.')) return fileName.endsWith(rule)
    if (rule.endsWith('/*')) return mimeType.startsWith(rule.slice(0, -1))
    return mimeType === rule
  })
}

function validateFile(file: UploadRawFile, uid: number) {
  if (!matchesAccept(file)) return `文件“${file.name}”的类型不符合要求`

  const maxBytes = Math.max(0, props.maxSizeMB) * 1024 * 1024
  if (maxBytes > 0 && file.size > maxBytes) {
    return `文件“${file.name}”不能超过 ${props.maxSizeMB} MB`
  }

  if (
    !props.allowDuplicateName &&
    fileList.value.some((item) => item.uid !== uid && item.name === file.name)
  ) {
    return `文件“${file.name}”已存在`
  }

  return ''
}

function normalizeUploadFile(file: UploadFile): FileUploadValue {
  return {
    uid: file.uid,
    name: file.name,
    size: file.size,
    url: file.url,
    status: file.status as FileUploadStatus,
    percentage: file.percentage,
    response: file.response,
    raw: file.raw
  }
}

async function startUpload(file: FileUploadValue) {
  if (!file.raw || props.readonly || props.disabled || file.status === 'uploading') return

  const validationMessage = validateFile(file.raw, file.uid)
  if (validationMessage) {
    updateFile(file.uid, { status: 'fail', errorMessage: validationMessage })
    ElMessage.warning(validationMessage)
    return
  }

  controllers.get(file.uid)?.abort()
  const controller = new AbortController()
  controllers.set(file.uid, controller)
  updateFile(file.uid, {
    status: 'uploading',
    percentage: 0,
    errorMessage: ''
  })

  try {
    const result = await props.upload(file.raw, {
      signal: controller.signal,
      onProgress(percentage) {
        if (controller.signal.aborted) return
        const normalized = Math.min(100, Math.max(0, Math.round(percentage)))
        updateFile(file.uid, { percentage: normalized })
      }
    })

    if (controller.signal.aborted) return

    const uploadedFile: FileUploadValue = {
      ...file,
      ...result,
      uid: file.uid,
      name: result.name || file.name,
      size: result.size ?? file.size,
      raw: file.raw,
      status: 'success',
      percentage: 100,
      errorMessage: ''
    }

    updateFile(file.uid, uploadedFile)
    emit('success', uploadedFile)
  } catch (error) {
    if (controller.signal.aborted) return

    const failedFile: FileUploadValue = {
      ...file,
      status: 'fail',
      errorMessage: '上传失败，请重试'
    }
    updateFile(file.uid, failedFile)
    emit('error', error, failedFile)
  } finally {
    if (controllers.get(file.uid) === controller) controllers.delete(file.uid)
  }
}

function handleChange(uploadFile: UploadFile, uploadFiles: UploadFiles) {
  const normalizedFile = normalizeUploadFile(uploadFile)
  const existing = fileList.value.find((file) => file.uid === uploadFile.uid)

  if (!existing) {
    fileList.value = [
      ...fileList.value,
      {
        ...normalizedFile,
        status: 'ready',
        percentage: 0
      }
    ]
  } else {
    fileList.value = fileList.value.map((file) =>
      file.uid === uploadFile.uid ? { ...file, ...normalizedFile } : file
    )
  }

  const currentFile = fileList.value.find((file) => file.uid === uploadFile.uid)
  if (!currentFile?.raw || uploadFiles.length > props.limit) return

  const validationMessage = validateFile(currentFile.raw, currentFile.uid)
  if (validationMessage) {
    fileList.value = fileList.value.filter((file) => file.uid !== currentFile.uid)
    emitFiles()
    ElMessage.warning(validationMessage)
    return
  }

  emitFiles()
  if (props.autoUpload && currentFile.status === 'ready') void startUpload(currentFile)
}

async function beforeRemove(uploadFile: UploadFile) {
  if (props.readonly || props.disabled) return false

  const file = fileList.value.find((item) => item.uid === uploadFile.uid)
  if (!file) return true

  controllers.get(file.uid)?.abort()
  controllers.delete(file.uid)

  if (file.status === 'success' && props.remove) {
    try {
      await props.remove(file)
    } catch (error) {
      ElMessage.error('文件删除失败，请重试')
      emit('error', error, file)
      return false
    }
  }

  return true
}

function handleRemove(uploadFile: UploadFile) {
  const file = fileList.value.find((item) => item.uid === uploadFile.uid)
  fileList.value = fileList.value.filter((item) => item.uid !== uploadFile.uid)
  emitFiles()
  if (file) emit('remove', file)
}

function handleExceed(files: File[]) {
  ElMessage.warning(`最多只能上传 ${props.limit} 个文件`)
  emit('exceed', files)
}

function retry(file: FileUploadValue) {
  void startUpload(file)
}

function submit() {
  for (const file of fileList.value) {
    if ((file.status === 'ready' || file.status === 'fail') && file.raw) void startUpload(file)
  }
}

function abort(uid?: number) {
  if (uid !== undefined) {
    controllers.get(uid)?.abort()
    controllers.delete(uid)
    updateFile(uid, { status: 'ready', percentage: 0 })
    return
  }

  for (const [fileUid, controller] of controllers) {
    controller.abort()
    updateFile(fileUid, { status: 'ready', percentage: 0 })
  }
  controllers.clear()
}

function removeFile(file: FileUploadValue) {
  const uploadFile = file as UploadFile
  uploadRef.value?.handleRemove(uploadFile)
}

function downloadFile(file: FileUploadValue) {
  emit('download', file)
  if (file.url) window.open(file.url, '_blank', 'noopener,noreferrer')
}

function statusText(status?: FileUploadStatus) {
  if (status === 'uploading') return '上传中'
  if (status === 'success') return '上传成功'
  if (status === 'fail') return '上传失败'
  return '等待上传'
}

function getManagedFile(uid: number) {
  return fileList.value.find((file) => file.uid === uid)
}

function retryByUid(uid: number) {
  const file = getManagedFile(uid)
  if (file) retry(file)
}

function removeByUid(uid: number) {
  const file = getManagedFile(uid)
  if (file) removeFile(file)
}

function downloadByUid(uid: number) {
  const file = getManagedFile(uid)
  if (file) downloadFile(file)
}

onBeforeUnmount(() => {
  for (const controller of controllers.values()) controller.abort()
  controllers.clear()
})

defineExpose({ submit, abort, retry })
</script>

<template>
  <div class="file-upload-field">
    <el-upload
      ref="uploadRef"
      :file-list="fileList"
      :accept="accept"
      :limit="limit"
      :multiple="multiple"
      :disabled="disabled || readonly"
      :auto-upload="false"
      :before-remove="beforeRemove"
      :on-change="handleChange"
      :on-remove="handleRemove"
      :on-exceed="handleExceed"
    >
      <template v-if="!readonly" #trigger>
        <slot name="trigger">
          <el-button type="primary" :disabled="disabled">选择文件</el-button>
        </slot>
      </template>

      <template #tip>
        <slot name="tip">
          <div class="file-upload-field__tip">
            {{ tip || `最多 ${limit} 个文件，单个文件不超过 ${maxSizeMB} MB` }}
          </div>
        </slot>
      </template>

      <template #file="{ file }">
        <slot
          name="file"
          :file="getManagedFile(file.uid)"
          :retry="retry"
          :remove="removeFile"
          :download="downloadFile"
        >
          <div v-if="fileList.find((item) => item.uid === file.uid)" class="file-upload-field__file">
            <div class="file-upload-field__file-main">
              <span class="file-upload-field__name" :title="file.name">{{ file.name }}</span>
              <span
                class="file-upload-field__status"
                :class="`is-${file.status || 'ready'}`"
              >
                {{ statusText(file.status as FileUploadStatus) }}
              </span>
              <span v-if="file.status === 'uploading'" class="file-upload-field__progress">
                {{ Math.round(file.percentage || 0) }}%
              </span>
            </div>

            <div class="file-upload-field__actions">
              <el-button
                v-if="downloadable && file.url"
                type="primary"
                link
                @click.stop="downloadByUid(file.uid)"
              >
                下载
              </el-button>
              <el-button
                v-if="file.status === 'fail' && !readonly && !disabled"
                type="primary"
                link
                @click.stop="retryByUid(file.uid)"
              >
                重试
              </el-button>
              <el-button
                v-if="!readonly && !disabled"
                type="danger"
                link
                @click.stop="removeByUid(file.uid)"
              >
                删除
              </el-button>
            </div>

            <div
              v-if="getManagedFile(file.uid)?.errorMessage"
              class="file-upload-field__error"
              role="alert"
            >
              {{ getManagedFile(file.uid)?.errorMessage }}
            </div>
          </div>
        </slot>
      </template>
    </el-upload>

    <el-button
      v-if="!autoUpload && !readonly"
      class="file-upload-field__submit"
      :disabled="disabled || !fileList.some((file) => file.status === 'ready' || file.status === 'fail')"
      @click="submit"
    >
      开始上传
    </el-button>
  </div>
</template>

<style scoped>
.file-upload-field {
  width: 100%;
  min-width: 0;
}

.file-upload-field :deep(.el-upload-list) {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.file-upload-field :deep(.el-upload-list__item) {
  height: auto;
  margin: 0;
  padding: 0;
  transition: none;
}

.file-upload-field__tip {
  margin-top: 6px;
  color: var(--el-text-color-secondary);
  font-size: 12px;
  line-height: 18px;
}

.file-upload-field__file {
  display: grid;
  grid-template-columns: minmax(0, 1fr) auto;
  gap: 4px 12px;
  padding: 8px 10px;
  border: 1px solid var(--el-border-color-lighter);
  border-radius: var(--el-border-radius-base);
  background: var(--el-fill-color-blank);
}

.file-upload-field__file-main,
.file-upload-field__actions {
  display: flex;
  min-width: 0;
  align-items: center;
  gap: 8px;
}

.file-upload-field__name {
  overflow: hidden;
  color: var(--el-text-color-primary);
  text-overflow: ellipsis;
  white-space: nowrap;
}

.file-upload-field__status,
.file-upload-field__progress {
  flex: none;
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.file-upload-field__status.is-success {
  color: var(--el-color-success);
}

.file-upload-field__status.is-fail,
.file-upload-field__error {
  color: var(--el-color-danger);
}

.file-upload-field__error {
  grid-column: 1 / -1;
  font-size: 12px;
}

.file-upload-field__submit {
  margin-top: 10px;
}
</style>
