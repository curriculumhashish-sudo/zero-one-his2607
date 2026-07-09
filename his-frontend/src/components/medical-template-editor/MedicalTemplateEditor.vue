<template>
  <div class="medical-template-editor">
    <div class="editor-layout">
      <div v-if="showTemplateInfo" class="left-panel">
        <div class="panel-header">
          <h3>模板信息</h3>
        </div>
        <div class="form-group">
          <label class="form-label">模板名称</label>
          <input v-model="localTemplateInfo.name" class="form-input" placeholder="请输入模板名称" />
        </div>
        <div class="form-group">
          <label class="form-label">模板编码</label>
          <input v-model="localTemplateInfo.code" class="form-input" placeholder="请输入模板编码" />
        </div>
        <div class="form-group">
          <label class="form-label">文书类型</label>
          <select v-model="localTemplateInfo.documentType" class="form-select">
            <option v-for="opt in DOCUMENT_TYPE_OPTIONS" :key="opt.value" :value="opt.value">
              {{ opt.label }}
            </option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">适用就诊</label>
          <select v-model="localTemplateInfo.visitType" class="form-select">
            <option v-for="opt in VISIT_TYPE_OPTIONS" :key="opt.value" :value="opt.value">
              {{ opt.label }}
            </option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">使用范围</label>
          <select v-model="localTemplateInfo.scope" class="form-select">
            <option v-for="opt in SCOPE_OPTIONS" :key="opt.value" :value="opt.value">
              {{ opt.label }}
            </option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">启用</label>
          <div
            :class="['toggle-switch', { active: localTemplateInfo.enabled }]"
            @click="localTemplateInfo.enabled = !localTemplateInfo.enabled"
          >
            <div :class="['toggle-inner', { active: localTemplateInfo.enabled }]"></div>
          </div>
        </div>
      </div>

      <div v-if="showMetadata" class="middle-panel">
        <div class="panel-header">
          <h3>元数据</h3>
          <span class="panel-hint">拖到正文插入字段；双击可手工修正。</span>
        </div>
        <div class="metadata-list">
          <div
            v-for="item in localMetadata"
            :key="item.key"
            class="metadata-item"
            :class="{ readOnly: item.readOnly }"
            draggable="true"
            @dragstart="handleDragStart($event, item)"
          >
            <span class="metadata-label">{{ item.label }}</span>
            <span class="metadata-key">{{ item.key }}</span>
            <span class="metadata-status">{{ item.readOnly ? '只读' : '可改' }}</span>
          </div>
        </div>
      </div>

      <div class="right-panel">
        <div class="toolbar">
          <div class="toolbar-group">
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().undo().run()"
              title="撤销"
            >
              <span class="toolbar-icon">↩</span>
            </button>
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().redo().run()"
              title="重做"
            >
              <span class="toolbar-icon">↪</span>
            </button>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('bold') }"
              @mousedown.prevent="editor?.chain().focus().toggleBold().run()"
              title="加粗"
            >
              <span class="toolbar-icon">B</span>
            </button>
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('italic') }"
              @mousedown.prevent="editor?.chain().focus().toggleItalic().run()"
              title="斜体"
            >
              <span class="toolbar-icon italic">I</span>
            </button>
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('underline') }"
              @mousedown.prevent="editor?.chain().focus().toggleUnderline().run()"
              title="下划线"
            >
              <span class="toolbar-icon underline">U</span>
            </button>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('heading', { level: 1 }) }"
              @mousedown.prevent="editor?.chain().focus().toggleHeading({ level: 1 }).run()"
              title="标题1"
            >
              H1
            </button>
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('heading', { level: 2 }) }"
              @mousedown.prevent="editor?.chain().focus().toggleHeading({ level: 2 }).run()"
              title="标题2"
            >
              H2
            </button>
            <button
              class="toolbar-btn"
              :class="{ active: editor?.isActive('heading', { level: 3 }) }"
              @mousedown.prevent="editor?.chain().focus().toggleHeading({ level: 3 }).run()"
              title="标题3"
            >
              H3
            </button>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().toggleBulletList().run()"
              title="无序列表"
            >
              <span class="toolbar-icon">•</span>
            </button>
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().toggleOrderedList().run()"
              title="有序列表"
            >
              <span class="toolbar-icon">1.</span>
            </button>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <div
              class="dropdown-wrapper"
              @mouseenter="handleInsertMenuEnter"
              @mouseleave="handleInsertMenuLeave"
            >
              <button class="toolbar-btn dropdown-toggle" title="插入">
                <span class="toolbar-icon">+</span>
                <span class="dropdown-arrow">▼</span>
              </button>
              <div
                class="dropdown-menu"
                :class="{ show: showInsertMenu }"
                :style="
                  showInsertMenu
                    ? { top: insertMenuPosition.top + 'px', left: insertMenuPosition.left + 'px' }
                    : {}
                "
                ref="insertMenuRef"
                @mouseenter="handleDropdownMenuEnter"
                @mouseleave="handleDropdownMenuLeave"
              >
                <button class="dropdown-item" @click="insertHorizontalRule">横线</button>
                <button class="dropdown-item" @click="insertLink">插入链接</button>
                <button class="dropdown-item" @click="insertImage">插入图片</button>
                <div class="dropdown-divider"></div>
                <button class="dropdown-item" @click="insertTable(3, 3)">表格 3×3</button>
                <button class="dropdown-item" @click="insertTable(4, 4)">表格 4×4</button>
                <button class="dropdown-item" @click="insertTable(5, 5)">表格 5×5</button>
              </div>
            </div>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().setTextAlign('left').run()"
              title="左对齐"
            >
              <span class="toolbar-icon">←</span>
            </button>
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().setTextAlign('center').run()"
              title="居中"
            >
              <span class="toolbar-icon">—</span>
            </button>
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().setTextAlign('right').run()"
              title="右对齐"
            >
              <span class="toolbar-icon">→</span>
            </button>
            <button
              class="toolbar-btn"
              @mousedown.prevent="editor?.chain().focus().setTextAlign('justify').run()"
              title="两端对齐"
            >
              <span class="toolbar-icon">↔</span>
            </button>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleFontFamilyChange"
              :value="currentFontFamily"
              title="字体"
            >
              <option value="">默认字体</option>
              <option value="SimSun">宋体</option>
              <option value="SimHei">黑体</option>
              <option value="Microsoft YaHei">微软雅黑</option>
              <option value="KaiTi">楷体</option>
              <option value="Arial">Arial</option>
              <option value="Times New Roman">Times New Roman</option>
            </select>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleFontSizeChange"
              :value="currentFontSize"
              title="字号"
            >
              <option value="">默认字号</option>
              <option value="12px">12px</option>
              <option value="14px">14px</option>
              <option value="16px">16px</option>
              <option value="18px">18px</option>
              <option value="20px">20px</option>
              <option value="24px">24px</option>
              <option value="28px">28px</option>
              <option value="32px">32px</option>
            </select>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleLineHeightChange"
              :value="currentLineHeight"
              title="行间距"
            >
              <option value="">默认间距</option>
              <option value="1.2">1.2倍</option>
              <option value="1.5">1.5倍</option>
              <option value="1.8">1.8倍</option>
              <option value="2.0">2.0倍</option>
              <option value="2.5">2.5倍</option>
              <option value="3.0">3.0倍</option>
            </select>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleSpacingBeforeChange"
              :value="currentSpacingBefore"
              title="段前间距"
            >
              <option value="">段前</option>
              <option value="0px">0</option>
              <option value="4px">4px</option>
              <option value="8px">8px</option>
              <option value="12px">12px</option>
              <option value="16px">16px</option>
              <option value="24px">24px</option>
              <option value="32px">32px</option>
            </select>
          </div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleSpacingAfterChange"
              :value="currentSpacingAfter"
              title="段后间距"
            >
              <option value="">段后</option>
              <option value="0px">0</option>
              <option value="4px">4px</option>
              <option value="8px">8px</option>
              <option value="12px">12px</option>
              <option value="16px">16px</option>
              <option value="24px">24px</option>
              <option value="32px">32px</option>
            </select>
          </div>
          <div class="toolbar-divider"></div>
          <div class="toolbar-group">
            <select
              class="toolbar-select"
              @change="handleIndentChange"
              :value="currentIndent"
              title="首行缩进"
            >
              <option value="">缩进</option>
              <option value="0px">无缩进</option>
              <option value="2em">2字符</option>
              <option value="4em">4字符</option>
              <option value="6em">6字符</option>
              <option value="8em">8字符</option>
            </select>
          </div>
        </div>
        <div class="editor-wrapper" @dragover.prevent @drop="handleDrop">
          <editor-content :editor="editor" class="editor-content" />
        </div>
        <div class="editor-footer">
          <span class="char-count">{{ characterCount || 0 }} 字</span>
        </div>
      </div>
    </div>

    <div class="editor-actions">
      <button class="action-btn secondary" @click="handleCopyTemplate">复制为新模板</button>
      <button class="action-btn secondary" @click="handleCancel">取消</button>
      <button class="action-btn primary" @click="handleSave">保存</button>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, watch, onMounted, onBeforeUnmount } from 'vue'
import { useEditor, EditorContent } from '@tiptap/vue-3'
import StarterKit from '@tiptap/starter-kit'
import { TextStyle, FontSize, LineHeight } from '@tiptap/extension-text-style'
import { FontFamily } from '@tiptap/extension-font-family'
import { TextAlign } from '@tiptap/extension-text-align'
import { Underline } from '@tiptap/extension-underline'
import { CharacterCount } from '@tiptap/extension-character-count'
import { Table } from '@tiptap/extension-table'
import { TableRow } from '@tiptap/extension-table-row'
import { TableCell } from '@tiptap/extension-table-cell'
import { TableHeader } from '@tiptap/extension-table-header'
import { Link } from '@tiptap/extension-link'
import { Image } from '@tiptap/extension-image'
import { MetadataVariable } from './extensions/MetadataVariable'
import { ParagraphStyle } from './extensions/ParagraphStyle'
import {
  type MedicalTemplateInfo,
  type MedicalMetadataItem,
  type MedicalTemplate,
  createMedicalTemplateInfo,
  DOCUMENT_TYPE_OPTIONS,
  VISIT_TYPE_OPTIONS,
  SCOPE_OPTIONS,
  DEFAULT_METADATA_LIST
} from './type'

interface Props {
  /** 模板信息，支持外部传入 */
  templateInfo?: MedicalTemplateInfo
  /** 模板内容，支持外部传入 */
  content?: string
  /** 元数据列表，支持外部传入 */
  metadata?: MedicalMetadataItem[]
  /** 是否显示元数据面板，默认显示 */
  showMetadata?: boolean
  /** 是否显示模板信息面板，默认显示 */
  showTemplateInfo?: boolean
}

const props = withDefaults(defineProps<Props>(), {
  templateInfo: () => createMedicalTemplateInfo({ name: '首次病程记录', code: 'PN_FIRST' }),
  content: `<h1>首次病程记录</h1><p>病例特点：</p><p>初步诊断：</p><p>诊疗计划：</p>`,
  metadata: () => [...DEFAULT_METADATA_LIST],
  showMetadata: true,
  showTemplateInfo: true
})

const emit = defineEmits<{
  /** 保存模板事件 */
  (e: 'save', template: MedicalTemplate): void
  /** 取消编辑事件 */
  (e: 'cancel'): void
  /** 复制模板事件 */
  (e: 'copy', template: MedicalTemplate): void
}>()

const localTemplateInfo = ref<MedicalTemplateInfo>({ ...props.templateInfo })
const localMetadata = ref<MedicalMetadataItem[]>([...props.metadata])
const characterCount = ref(0)
const currentFontFamily = ref('')
const currentFontSize = ref('')
const currentLineHeight = ref('')
const currentIndent = ref('')
const currentSpacingBefore = ref('')
const currentSpacingAfter = ref('')
const showInsertMenu = ref(false)
const insertMenuPosition = ref({ top: 0, left: 0 })
const insertMenuRef = ref<HTMLElement | null>(null)
let menuCloseTimer: ReturnType<typeof setTimeout> | null = null

function handleSpacingBeforeChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const spacing = target.value
  currentSpacingBefore.value = spacing
  if (spacing) {
    editor.value?.chain().focus().setParagraphSpacing({ before: spacing }).run()
  } else {
    editor.value?.chain().focus().unsetParagraphSpacing().run()
  }
}

function handleSpacingAfterChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const spacing = target.value
  currentSpacingAfter.value = spacing
  if (spacing) {
    editor.value?.chain().focus().setParagraphSpacing({ after: spacing }).run()
  } else {
    editor.value?.chain().focus().unsetParagraphSpacing().run()
  }
}

function handleIndentChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const indent = target.value
  currentIndent.value = indent
  if (indent) {
    editor.value?.chain().focus().setParagraphIndent(indent).run()
  } else {
    editor.value?.chain().focus().unsetParagraphIndent().run()
  }
}

function handleInsertMenuEnter(event: MouseEvent) {
  if (menuCloseTimer) {
    clearTimeout(menuCloseTimer)
    menuCloseTimer = null
  }
  const target = event.currentTarget as HTMLElement
  const rect = target.getBoundingClientRect()
  insertMenuPosition.value = {
    top: rect.bottom + 4,
    left: rect.left
  }
  showInsertMenu.value = true
}

function handleInsertMenuLeave() {
  menuCloseTimer = setTimeout(() => {
    showInsertMenu.value = false
    menuCloseTimer = null
  }, 200)
}

function handleDropdownMenuEnter() {
  if (menuCloseTimer) {
    clearTimeout(menuCloseTimer)
    menuCloseTimer = null
  }
  showInsertMenu.value = true
}

function handleDropdownMenuLeave() {
  showInsertMenu.value = false
}

function insertHorizontalRule() {
  showInsertMenu.value = false
  editor.value?.chain().focus().setHorizontalRule().run()
}

function insertTable(rows: number, cols: number) {
  showInsertMenu.value = false
  editor.value?.chain().focus().insertTable({ rows, cols }).run()
}

function insertLink() {
  showInsertMenu.value = false
  if (!editor.value) return
  const url = prompt('请输入链接地址：')
  if (!url) return
  const hasSelection = !editor.value.state.selection.empty
  if (hasSelection) {
    editor.value.chain().focus().setLink({ href: url }).run()
  } else {
    editor.value.chain().focus().insertContent(`<a href="${url}">链接文本</a>`).run()
  }
}

function insertImage() {
  showInsertMenu.value = false
  if (!editor.value) return
  const url = prompt('请输入图片URL：')
  if (!url) return
  editor.value.chain().focus().setImage({ src: url }).run()
}

function handleFontFamilyChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const fontFamily = target.value
  currentFontFamily.value = fontFamily
  editor.value?.chain().focus().setFontFamily(fontFamily).run()
}

function handleFontSizeChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const fontSize = target.value
  currentFontSize.value = fontSize
  if (fontSize) {
    editor.value?.chain().focus().setFontSize(fontSize).run()
  } else {
    editor.value?.chain().focus().unsetFontSize().run()
  }
}

function handleLineHeightChange(event: Event) {
  const target = event.target as HTMLSelectElement
  const lineHeight = target.value
  currentLineHeight.value = lineHeight
  if (lineHeight) {
    editor.value?.chain().focus().setLineHeight(lineHeight).run()
  } else {
    editor.value?.chain().focus().unsetLineHeight().run()
  }
}

const editor = useEditor({
  content: props.content,
  extensions: [
    StarterKit,
    TextStyle,
    FontFamily,
    FontSize,
    LineHeight,
    Underline,
    CharacterCount,
    TextAlign.configure({
      types: ['heading', 'paragraph']
    }),
    Table.configure({
      resizable: true
    }),
    TableRow,
    TableCell,
    TableHeader,
    Link.configure({
      openOnClick: false
    }),
    Image.configure({
      allowBase64: true
    }),
    ParagraphStyle,
    MetadataVariable.configure({
      metadata: localMetadata.value
    })
  ],
  onUpdate: ({ editor }) => {
    characterCount.value = editor.storage.characterCount.characters()
  }
})

watch(
  () => props.templateInfo,
  (newInfo) => {
    if (newInfo) {
      localTemplateInfo.value = { ...newInfo }
    }
  },
  { deep: true }
)

watch(
  () => props.content,
  (newContent) => {
    if (newContent && editor.value) {
      editor.value.commands.setContent(newContent)
    }
  }
)

watch(
  () => props.metadata,
  (newMetadata) => {
    if (newMetadata) {
      localMetadata.value = [...newMetadata]
    }
  },
  { deep: true }
)

const handleDragStart = (event: DragEvent, item: MedicalMetadataItem) => {
  if (event.dataTransfer) {
    event.dataTransfer.setData('text/plain', item.key)
  }
}

const handleDrop = (event: DragEvent) => {
  event.preventDefault()
  const key = event.dataTransfer?.getData('text/plain')
  if (key && editor.value) {
    editor.value.chain().focus().insertMetadataVariable(key).run()
  }
}

const handleSave = () => {
  const template: MedicalTemplate = {
    info: { ...localTemplateInfo.value },
    content: editor.value?.getHTML() || ''
  }
  emit('save', template)
}

const handleCancel = () => {
  emit('cancel')
}

const handleCopyTemplate = () => {
  const template: MedicalTemplate = {
    info: {
      ...localTemplateInfo.value,
      name: `${localTemplateInfo.value.name} (副本)`,
      code: `${localTemplateInfo.value.code}_COPY`
    },
    content: editor.value?.getHTML() || ''
  }
  emit('copy', template)
}

onMounted(() => {
  if (editor.value) {
    characterCount.value = editor.value.storage.characterCount.characters()
  }
})

onBeforeUnmount(() => {
  editor.value?.destroy()
  if (menuCloseTimer) {
    clearTimeout(menuCloseTimer)
    menuCloseTimer = null
  }
})
</script>

<style scoped>
.medical-template-editor {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: #f5f5f5;
  padding: 16px;
  gap: 16px;
}

.editor-layout {
  display: flex;
  flex: 1;
  overflow: hidden;
}

.editor-layout:not(:has(.left-panel)):not(:has(.middle-panel)) {
  gap: 0;
}

.left-panel,
.middle-panel {
  width: 240px;
  background: #fff;
  border-radius: 8px;
  padding: 16px;
  display: flex;
  flex-direction: column;
}

.right-panel {
  flex: 1;
  background: #fff;
  border-radius: 8px;
  display: flex;
  flex-direction: column;
  overflow-y: auto;
}

.panel-header {
  margin-bottom: 16px;
  padding-bottom: 12px;
  border-bottom: 1px solid #eee;
}

.panel-header h3 {
  margin: 0;
  font-size: 14px;
  font-weight: 600;
  color: #333;
}

.panel-hint {
  display: block;
  font-size: 12px;
  color: #999;
  margin-top: 4px;
}

.form-group {
  margin-bottom: 16px;
}

.form-label {
  display: block;
  font-size: 12px;
  color: #666;
  margin-bottom: 8px;
}

.form-input,
.form-select {
  width: 100%;
  padding: 8px 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 13px;
  box-sizing: border-box;
}

.form-input:focus,
.form-select:focus {
  outline: none;
  border-color: #409eff;
}

.toggle-switch {
  width: 48px;
  height: 26px;
  background: #ddd;
  border-radius: 13px;
  cursor: pointer;
  position: relative;
  transition: background 0.3s;
}

.toggle-switch .toggle-inner {
  width: 22px;
  height: 22px;
  background: #fff;
  border-radius: 50%;
  position: absolute;
  top: 2px;
  left: 2px;
  transition: left 0.3s;
}

.toggle-switch .toggle-inner.active {
  left: 24px;
}

.toggle-switch.active {
  background: #409eff;
}

.metadata-list {
  flex: 1;
  overflow-y: auto;
}

.metadata-item {
  padding: 10px 12px;
  background: #f8f9fa;
  border-radius: 4px;
  margin-bottom: 8px;
  cursor: grab;
  border: 1px solid #eee;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.metadata-item:hover {
  background: #e9ecef;
  border-color: #ddd;
}

.metadata-item.readOnly {
  opacity: 0.7;
}

.metadata-label {
  font-size: 13px;
  color: #333;
}

.metadata-key {
  font-size: 11px;
  color: #999;
  font-family: monospace;
}

.metadata-status {
  font-size: 10px;
  color: #fff;
  background: #67c23a;
  padding: 2px 6px;
  border-radius: 10px;
}

.metadata-item.readOnly .metadata-status {
  background: #909399;
}

.toolbar {
  display: flex;
  align-items: center;
  padding: 8px 16px;
  border-bottom: 1px solid #eee;
  gap: 4px;
  overflow-x: auto;
  flex-wrap: nowrap;
  scrollbar-width: thin;
  scrollbar-color: #ddd transparent;
}

.toolbar::-webkit-scrollbar {
  height: 4px;
}

.toolbar::-webkit-scrollbar-track {
  background: transparent;
}

.toolbar::-webkit-scrollbar-thumb {
  background: #ddd;
  border-radius: 2px;
}

.toolbar::-webkit-scrollbar-thumb:hover {
  background: #ccc;
}

.toolbar-group {
  display: flex;
  gap: 2px;
}

.toolbar-btn {
  width: 32px;
  height: 32px;
  border: none;
  background: #f5f5f5;
  border-radius: 4px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 14px;
  transition: all 0.2s;
}

.toolbar-btn:hover {
  background: #e8e8e8;
}

.toolbar-btn.active {
  background: #409eff;
  color: #fff;
}

.toolbar-icon {
  font-weight: bold;
}

.toolbar-icon.italic {
  font-style: italic;
}

.toolbar-icon.underline {
  text-decoration: underline;
}

.toolbar-divider {
  width: 1px;
  height: 24px;
  background: #ddd;
  margin: 0 8px;
}

.toolbar-select {
  height: 28px;
  padding: 0 8px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 13px;
  background: #fff;
  cursor: pointer;
  outline: none;
  min-width: 100px;
}

.toolbar-select:hover {
  border-color: #ccc;
}

.toolbar-select:focus {
  border-color: #409eff;
}

.dropdown-wrapper {
  position: relative;
  z-index: 100;
}

.dropdown-toggle {
  display: flex;
  align-items: center;
  gap: 4px;
  padding: 0 8px;
}

.dropdown-arrow {
  font-size: 8px;
}

.dropdown-menu {
  position: fixed;
  top: auto;
  left: auto;
  margin-top: 4px;
  background: #fff;
  border: 1px solid #ddd;
  border-radius: 4px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
  z-index: 9999;
  min-width: 120px;
  display: none;
}

.dropdown-menu.show {
  display: block;
}

.dropdown-item {
  display: block;
  width: 100%;
  padding: 8px 12px;
  text-align: left;
  border: none;
  background: none;
  cursor: pointer;
  font-size: 13px;
  color: #333;
}

.dropdown-item:hover {
  background: #f5f5f5;
}

.dropdown-divider {
  height: 1px;
  background: #eee;
  margin: 4px 0;
}

.editor-wrapper {
  flex: 1;
  overflow-y: auto;
  padding: 24px;
  background: #fff;
}

.editor-content {
  min-height: 100%;
  outline: none;
  border: none;
}

.editor-content :deep(.ProseMirror) {
  outline: none;
  border: none;
}

.editor-content :deep(h1) {
  font-size: 20px;
  font-weight: bold;
  text-align: center;
  margin-bottom: 24px;
}

.editor-content :deep(h2) {
  font-size: 16px;
  font-weight: bold;
  margin-top: 16px;
  margin-bottom: 8px;
}

.editor-content :deep(h3) {
  font-size: 14px;
  font-weight: bold;
  margin-top: 12px;
  margin-bottom: 6px;
}

.editor-content :deep(p) {
  margin: 8px 0;
  line-height: 1.8;
}

.editor-content :deep(strong) {
  font-weight: bold;
}

.editor-content :deep(em) {
  font-style: italic;
}

.editor-content :deep(u) {
  text-decoration: underline;
}

.editor-content :deep(ul),
.editor-content :deep(ol) {
  padding-left: 24px;
  margin: 8px 0;
}

.editor-footer {
  padding: 8px 16px;
  border-top: 1px solid #eee;
  text-align: right;
}

.char-count {
  font-size: 12px;
  color: #999;
}

.editor-content :deep(.metadata-variable) {
  display: inline-block;
  padding: 2px 8px;
  border-radius: 4px;
  font-family: monospace;
  font-size: 13px;
  cursor: text;
  transition: all 0.2s;
}

.editor-content :deep(.metadata-variable:not(.readonly)) {
  background: #e8f4fd;
  color: #1e88e5;
  border: 1px solid #90caf9;
}

.editor-content :deep(.metadata-variable.readonly) {
  background: #f5f5f5;
  color: #909399;
  border: 1px solid #ddd;
  cursor: not-allowed;
}

.editor-content :deep(table) {
  border-collapse: collapse;
  width: 100%;
  margin: 8px 0;
}

.editor-content :deep(td),
.editor-content :deep(th) {
  border: 1px solid #ddd;
  padding: 8px 12px;
  text-align: left;
}

.editor-content :deep(th) {
  background: #f5f5f5;
  font-weight: bold;
}

.editor-content :deep(tr:hover) {
  background: #f9f9f9;
}

.editor-content :deep(hr) {
  border: none;
  border-top: 1px solid #ddd;
  margin: 16px 0;
}

.editor-content :deep(a) {
  color: #409eff;
  text-decoration: underline;
}

.editor-content :deep(a:hover) {
  color: #66b1ff;
}

.editor-content :deep(img) {
  max-width: 100%;
  height: auto;
  border-radius: 4px;
}

.editor-actions {
  display: flex;
  justify-content: flex-end;
  gap: 12px;
}

.action-btn {
  padding: 10px 24px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  transition: all 0.2s;
}

.action-btn.primary {
  background: #409eff;
  color: #fff;
}

.action-btn.primary:hover {
  background: #66b1ff;
}

.action-btn.secondary {
  background: #fff;
  color: #666;
  border: 1px solid #ddd;
}

.action-btn.secondary:hover {
  background: #f5f5f5;
}
</style>
