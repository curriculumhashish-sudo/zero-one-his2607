<script setup lang="ts">
import EmptyState from '@/components/common/EmptyState.vue'
</script>

<template>
  <main class="page-shell">
    <h2>库存明细</h2>
    <EmptyState title="库存明细页面" description="页面入口已配置，业务交互待继续完善" />
  </main>
</template>

<style scoped>
.page-shell {
  box-sizing: border-box;
  min-height: 100%;
  padding: 20px;
  background: #fff;
}

.page-shell h2 {
  margin: 0;
  font-size: 18px;
}
</style>
