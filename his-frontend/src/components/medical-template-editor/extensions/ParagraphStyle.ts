import { Extension } from '@tiptap/core'

declare module '@tiptap/core' {
  interface Commands<ReturnType> {
    paragraphStyle: {
      setParagraphSpacing: (spacing: { before?: string; after?: string }) => ReturnType
      setParagraphIndent: (indent: string) => ReturnType
      unsetParagraphSpacing: () => ReturnType
      unsetParagraphIndent: () => ReturnType
    }
  }
}

export const ParagraphStyle = Extension.create({
  name: 'paragraphStyle',

  addCommands() {
    return {
      setParagraphSpacing:
        ({ before, after }) =>
        ({ commands }) => {
          return commands.updateAttributes('paragraph', {
            spacingBefore: before || null,
            spacingAfter: after || null
          })
        },
      setParagraphIndent:
        (indent) =>
        ({ commands }) => {
          return commands.updateAttributes('paragraph', {
            indent: indent || null
          })
        },
      unsetParagraphSpacing:
        () =>
        ({ commands }) => {
          return commands.updateAttributes('paragraph', {
            spacingBefore: null,
            spacingAfter: null
          })
        },
      unsetParagraphIndent:
        () =>
        ({ commands }) => {
          return commands.updateAttributes('paragraph', {
            indent: null
          })
        }
    }
  }
})
