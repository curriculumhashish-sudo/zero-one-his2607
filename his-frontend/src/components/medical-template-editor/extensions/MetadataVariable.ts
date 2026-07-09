import { Node, mergeAttributes } from '@tiptap/core'
import type { MedicalMetadataItem } from '../type'

export interface MetadataVariableOptions {
  metadata: MedicalMetadataItem[]
}

declare module '@tiptap/core' {
  interface Commands<ReturnType> {
    insertMetadataVariable: {
      insertMetadataVariable: (key: string) => ReturnType
    }
  }
}

export const MetadataVariable = Node.create<MetadataVariableOptions>({
  name: 'metadataVariable',
  group: 'inline',
  inline: true,
  atom: true,

  addOptions() {
    return {
      metadata: []
    }
  },

  addAttributes() {
    return {
      key: {
        default: '',
        parseHTML: (element) => element.getAttribute('data-key') || '',
        renderHTML: (attributes) => ({ 'data-key': attributes.key })
      },
      label: {
        default: '',
        parseHTML: (element) => element.getAttribute('data-label') || '',
        renderHTML: (attributes) => ({ 'data-label': attributes.label })
      },
      readOnly: {
        default: false,
        parseHTML: (element) => element.getAttribute('data-readonly') === 'true',
        renderHTML: (attributes) => ({ 'data-readonly': String(attributes.readOnly) })
      }
    }
  },

  parseHTML() {
    return [
      {
        tag: 'span[data-metadata-variable]'
      }
    ]
  },

  renderHTML({ node }) {
    const { key, label, readOnly } = node.attrs
    return [
      'span',
      mergeAttributes({
        'data-metadata-variable': '',
        'data-key': key,
        'data-label': label,
        'data-readonly': String(readOnly),
        contenteditable: readOnly ? 'false' : 'true',
        class: `metadata-variable ${readOnly ? 'readonly' : ''}`
      }),
      `[${label}]`
    ]
  },

  addCommands() {
    return {
      insertMetadataVariable:
        (key) =>
        ({ chain }) => {
          const meta = this.options.metadata.find((m: MedicalMetadataItem) => m.key === key)
          if (!meta) return false

          return chain().focus().insertContent({
            type: this.name,
            attrs: {
              key: meta.key,
              label: meta.label,
              readOnly: meta.readOnly
            }
          }).run()
        }
    }
  }
})