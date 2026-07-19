import type { Component } from 'vue'

export type CommonComponentKey = string | number

export interface DetailSummaryItem {
  key: CommonComponentKey
  label: string
  value?: string | number | null
  span?: number
}

export interface RecordSidebarItem {
  key: CommonComponentKey
  title: string
  subtitle?: string
  description?: string
  status?: string
  disabled?: boolean
}

export interface SideNavigatorNode {
  key: CommonComponentKey
  label: string
  disabled?: boolean
  children?: SideNavigatorNode[]
}

export interface ActionToolbarItem {
  key: CommonComponentKey
  label: string
  type?: 'primary' | 'success' | 'warning' | 'danger' | 'info' | 'default'
  icon?: Component | string
  disabled?: boolean
  loading?: boolean
  plain?: boolean
  text?: boolean
  confirmText?: string
}

export interface RowActionItem {
  key: CommonComponentKey
  label: string
  type?: 'primary' | 'success' | 'warning' | 'danger' | 'info' | 'default'
  disabled?: boolean
  divided?: boolean
  confirmText?: string
}

export interface StatusDisplayOption {
  value: CommonComponentKey
  label: string
  type?: 'primary' | 'success' | 'warning' | 'danger' | 'info'
  effect?: 'dark' | 'light' | 'plain'
  description?: string
}
