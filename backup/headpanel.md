```vue
<!--
  库房单据头：标题 + 表单槽位 + 底部审核横排（订货申请 / 采购退货等共用）。
-->
<template>
  <section
    class="warehouse-doc-order-head-panel shrink-0 rounded-md border border-[var(--el-border-color-extra-light)] bg-[var(--el-fill-color-blank)] px-2 py-1"
  >
    <header class="warehouse-doc-order-head-panel__header">
      <div dense class="section-title text-base font-semibold">
        {{ title }}
      </div>
    </header>
    <div class="warehouse-doc-order-head-panel__form">
      <slot />
    </div>
    <!--
      库房单据下方横排审核摘要：审核人 / 审核日期 / 审核状态（标签展示）。
      文案与展示形态由父组件计算后传入。
    -->
    <template>
      <div
        v-if="showAuditInline"
        class="warehouse-doc-audit-inline mt-0.5 flex flex-wrap items-center gap-x-1 gap-y-0.5 border-t border-dashed border-[var(--el-border-color-extra-light)] pt-1"
      >
        <span class="inline-flex min-w-0 max-w-full items-center gap-1">
          <span class="shrink-0 text-[12px] leading-tight text-[var(--el-text-color-secondary)]">
            审核人
          </span>
          <el-tag
            size="small"
            type="warning"
            plain
            class="!my-0 max-w-[min(40vw,180px)] truncate"
            :title="approverLabel"
          >
            {{ approverLabel }}
          </el-tag>
        </span>
        <span
          class="mx-1 hidden h-3 w-px shrink-0 bg-[var(--el-border-color-lighter)] sm:inline"
          aria-hidden="true"
        ></span>
        <span class="inline-flex min-w-0 max-w-full items-center gap-1">
          <span class="shrink-0 text-[12px] leading-tight text-[var(--el-text-color-secondary)]">
            审核日期
          </span>
          <el-tag
            size="small"
            type="primary"
            plain
            class="!my-0 max-w-[min(48vw,220px)] truncate"
            :title="approvalTimeText"
          >
            {{ approvalTimeText }}
          </el-tag>
        </span>
        <span
          class="mx-1 hidden h-3 w-px shrink-0 bg-[var(--el-border-color-lighter)] sm:inline"
          aria-hidden="true"
        ></span>
        <span class="inline-flex min-w-0 max-w-full items-center gap-1">
          <span class="shrink-0 text-[12px] leading-tight text-[var(--el-text-color-secondary)]">
            审核状态
          </span>
          <el-tag size="small" :type="statusTagType" plain class="!my-0">
            {{ statusText }}
          </el-tag>
        </span>
      </div>
    </template>
  </section>
</template>

<script lang="ts" setup>
defineOptions({
  name: 'WarehouseDocOrderHeadPanel'
})

withDefaults(
  defineProps<{
    approvalTimeText?: string
    approverLabel?: string
    statusTagType?: 'danger' | 'info' | 'success'
    statusText?: string
    /** 为 false 时隐藏底部审核人/时间/状态横排 */
    showAuditInline?: boolean
    title?: string
  }>(),
  {
    approvalTimeText: '暂无',
    approverLabel: '暂无',
    statusTagType: 'info',
    statusText: '暂无',
    showAuditInline: true,
    title: '单据信息'
  }
)
</script>

<style scoped>
.warehouse-doc-order-head-panel__header {
  margin-bottom: 2px;
}

.warehouse-doc-order-head-panel__header :deep(.section-title--dense) {
  font-size: 12px;
  line-height: 1.15;
}

.warehouse-doc-order-head-panel__header :deep(.section-title--dense::before) {
  height: 10px;
}
.warehouse-doc-order-head-panel__header .section-title[dense] {
  position: relative;
  padding-left: 10px;
  color: #020817;
}

.warehouse-doc-order-head-panel__header .section-title[dense]::before {
  position: absolute;
  top: 50%;
  left: 0;
  width: 3px;
  height: 14px;
  background: var(--el-color-primary);
  border-radius: 2px;
  content: '';
  transform: translateY(-50%);
}
</style>

```

