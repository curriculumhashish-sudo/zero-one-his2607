<script setup lang="ts">
import { onBeforeUnmount, ref, watch } from 'vue'
import CharacterCount from '@tiptap/extension-character-count'
import TextAlign from '@tiptap/extension-text-align'
import StarterKit from '@tiptap/starter-kit'
import type { Editor, JSONContent } from '@tiptap/core'
import { EditorContent, useEditor } from '@tiptap/vue-3'

import type { MedicalRecordJsonDocument } from './type'
import { createEmptyMedicalRecordDocument, isMedicalRecordJsonDocument } from './type'

defineOptions({
  name: 'JsonMedicalRecordEditor'
})

const props = withDefaults(
  defineProps<{
    /** 受控 JSON 文档对象，不接收 HTML 字符串。 */
    modelValue?: MedicalRecordJsonDocument
    /** 是否只读。 */
    readonly?: boolean
    /** 编辑区最小高度。 */
    minHeight?: string
    /** 空文档提示文字。 */
    placeholder?: string
    /** 是否显示底部字数统计。 */
    showCharacterCount?: boolean
  }>(),
  {
    modelValue: () => createEmptyMedicalRecordDocument(),
    readonly: false,
    minHeight: '360px',
    placeholder: '请填写病历正文',
    showCharacterCount: true
  }
)

const emit = defineEmits<{
  'update:modelValue': [value: MedicalRecordJsonDocument]
  change: [value: MedicalRecordJsonDocument]
  focus: [editor: Editor]
  blur: [editor: Editor]
  ready: [editor: Editor]
}>()

function cloneDocument(document: MedicalRecordJsonDocument): MedicalRecordJsonDocument {
  return JSON.parse(JSON.stringify(document)) as MedicalRecordJsonDocument
}

function normalizeDocument(value: unknown): MedicalRecordJsonDocument {
  return isMedicalRecordJsonDocument(value)
    ? cloneDocument(value)
    : createEmptyMedicalRecordDocument()
}

function documentsEqual(left: MedicalRecordJsonDocument, right: MedicalRecordJsonDocument) {
  return JSON.stringify(left) === JSON.stringify(right)
}

function countDocumentCharacters(document: MedicalRecordJsonDocument): number {
  const countNode = (node: JSONContent): number => {
    if (typeof node.text === 'string') return node.text.length
    return node.content?.reduce((total, child) => total + countNode(child), 0) ?? 0
  }

  return document.content?.reduce((total, node) => total + countNode(node), 0) ?? 0
}

const characterCount = ref(countDocumentCharacters(normalizeDocument(props.modelValue)))

const editor = useEditor({
  content: normalizeDocument(props.modelValue),
  editable: !props.readonly,
  extensions: [
    StarterKit,
    CharacterCount,
    TextAlign.configure({ types: ['heading', 'paragraph'] })
  ],
  editorProps: {
    attributes: {
      class: 'json-medical-record-editor__content',
      role: 'textbox',
      'aria-label': '病历正文',
      'aria-multiline': 'true'
    }
  },
  onCreate: ({ editor }) => {
    characterCount.value = editor.storage.characterCount.characters()
    emit('ready', editor)
  },
  onUpdate: ({ editor }) => {
    const document = cloneDocument(editor.getJSON() as MedicalRecordJsonDocument)
    characterCount.value = editor.storage.characterCount.characters()
    emit('update:modelValue', document)
    emit('change', document)
  },
  onFocus: ({ editor }) => emit('focus', editor),
  onBlur: ({ editor }) => emit('blur', editor)
})

watch(
  () => props.modelValue,
  (value) => {
    if (!editor.value) return

    const nextDocument = normalizeDocument(value)
    characterCount.value = countDocumentCharacters(nextDocument)
    const currentDocument = editor.value.getJSON() as MedicalRecordJsonDocument
    if (!documentsEqual(currentDocument, nextDocument)) {
      editor.value.commands.setContent(nextDocument, { emitUpdate: false })
    }
  },
  { deep: true }
)

watch(
  () => props.readonly,
  (readonly) => editor.value?.setEditable(!readonly)
)

function runCommand(command: (editor: Editor) => void) {
  if (!editor.value || props.readonly) return
  command(editor.value)
}

function clearContent() {
  runCommand((instance) => instance.commands.setContent(createEmptyMedicalRecordDocument()))
}

defineExpose({
  editor,
  focus: () => editor.value?.commands.focus(),
  clearContent,
  getJSON: () => normalizeDocument(editor.value?.getJSON())
})

onBeforeUnmount(() => editor.value?.destroy())
</script>

<template>
  <div
    class="json-medical-record-editor"
    :class="{ 'json-medical-record-editor--readonly': readonly }"
  >
    <div
      v-if="!readonly"
      class="json-medical-record-editor__toolbar"
      role="toolbar"
      aria-label="病历编辑工具栏"
    >
      <el-button-group>
        <el-button
          title="撤销"
          aria-label="撤销"
          :disabled="!editor?.can().chain().focus().undo().run()"
          @click="runCommand((instance) => instance.chain().focus().undo().run())"
        >
          撤销
        </el-button>
        <el-button
          title="重做"
          aria-label="重做"
          :disabled="!editor?.can().chain().focus().redo().run()"
          @click="runCommand((instance) => instance.chain().focus().redo().run())"
        >
          重做
        </el-button>
      </el-button-group>

      <el-button-group>
        <el-button
          :type="editor?.isActive('bold') ? 'primary' : 'default'"
          aria-label="加粗"
          @click="runCommand((instance) => instance.chain().focus().toggleBold().run())"
        >
          <strong>B</strong>
        </el-button>
        <el-button
          :type="editor?.isActive('italic') ? 'primary' : 'default'"
          aria-label="斜体"
          @click="runCommand((instance) => instance.chain().focus().toggleItalic().run())"
        >
          <em>I</em>
        </el-button>
        <el-button
          :type="editor?.isActive('underline') ? 'primary' : 'default'"
          aria-label="下划线"
          @click="runCommand((instance) => instance.chain().focus().toggleUnderline().run())"
        >
          <u>U</u>
        </el-button>
      </el-button-group>

      <el-button-group>
        <el-button
          :type="editor?.isActive('heading', { level: 2 }) ? 'primary' : 'default'"
          @click="
            runCommand((instance) => instance.chain().focus().toggleHeading({ level: 2 }).run())
          "
        >
          标题
        </el-button>
        <el-button
          :type="editor?.isActive('bulletList') ? 'primary' : 'default'"
          @click="runCommand((instance) => instance.chain().focus().toggleBulletList().run())"
        >
          项目列表
        </el-button>
        <el-button
          :type="editor?.isActive('orderedList') ? 'primary' : 'default'"
          @click="runCommand((instance) => instance.chain().focus().toggleOrderedList().run())"
        >
          编号列表
        </el-button>
      </el-button-group>

      <el-button-group>
        <el-button
          @click="runCommand((instance) => instance.chain().focus().setTextAlign('left').run())"
        >
          左对齐
        </el-button>
        <el-button
          @click="runCommand((instance) => instance.chain().focus().setTextAlign('center').run())"
        >
          居中
        </el-button>
      </el-button-group>
    </div>

    <div
      class="json-medical-record-editor__paper"
      :style="{ minHeight }"
      @click="editor?.commands.focus()"
    >
      <EditorContent v-if="editor" :editor="editor" />
      <span
        v-if="editor?.isEmpty && !readonly"
        class="json-medical-record-editor__placeholder"
        aria-hidden="true"
      >
        {{ placeholder }}
      </span>
    </div>

    <footer v-if="showCharacterCount" class="json-medical-record-editor__footer">
      {{ characterCount }} 字
      <span class="json-medical-record-editor__format">JSON 文档</span>
    </footer>
  </div>
</template>

<style scoped>
.json-medical-record-editor {
  overflow: hidden;
  border: 1px solid var(--el-border-color);
  border-radius: 6px;
  background: var(--el-bg-color);
}

.json-medical-record-editor:focus-within {
  border-color: var(--el-color-primary);
  box-shadow: 0 0 0 2px var(--el-color-primary-light-8);
}

.json-medical-record-editor__toolbar {
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  gap: 8px;
  padding: 8px;
  border-bottom: 1px solid var(--el-border-color-lighter);
  background: var(--el-fill-color-light);
}

.json-medical-record-editor__paper {
  position: relative;
  box-sizing: border-box;
  padding: 20px 24px;
  cursor: text;
}

.json-medical-record-editor__placeholder {
  position: absolute;
  top: 20px;
  left: 24px;
  color: var(--el-text-color-placeholder);
  pointer-events: none;
}

.json-medical-record-editor__footer {
  display: flex;
  justify-content: flex-end;
  gap: 12px;
  padding: 6px 12px;
  border-top: 1px solid var(--el-border-color-lighter);
  color: var(--el-text-color-secondary);
  font-size: 12px;
}

.json-medical-record-editor__format {
  color: var(--el-color-success);
}

.json-medical-record-editor--readonly .json-medical-record-editor__paper {
  cursor: default;
  background: var(--el-fill-color-lighter);
}

.json-medical-record-editor :deep(.json-medical-record-editor__content) {
  min-height: inherit;
  outline: none;
  color: var(--el-text-color-primary);
  line-height: 1.8;
}

.json-medical-record-editor :deep(.json-medical-record-editor__content p) {
  margin: 0 0 10px;
}

.json-medical-record-editor :deep(.json-medical-record-editor__content h2) {
  margin: 16px 0 10px;
  font-size: 18px;
}

.json-medical-record-editor :deep(.json-medical-record-editor__content ul),
.json-medical-record-editor :deep(.json-medical-record-editor__content ol) {
  padding-left: 24px;
}
</style>
