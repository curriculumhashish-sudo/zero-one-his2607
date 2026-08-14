import type { UploadRawFile } from 'element-plus'

export type FileUploadStatus = 'ready' | 'uploading' | 'success' | 'fail'

export interface FileUploadValue<TResponse = unknown> {
  uid: number
  name: string
  size?: number
  url?: string
  status?: FileUploadStatus
  percentage?: number
  response?: TResponse
  errorMessage?: string
  raw?: UploadRawFile
}

export interface FileUploadContext {
  signal: AbortSignal
  onProgress: (percentage: number) => void
}

export type FileUploadHandler<TResponse = unknown> = (
  file: UploadRawFile,
  context: FileUploadContext
) => Promise<FileUploadValue<TResponse>>

export type FileRemoveHandler<TResponse = unknown> = (
  file: FileUploadValue<TResponse>
) => Promise<void> | void

