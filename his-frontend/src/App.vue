<template>
  <!-- 全屏加载动画 -->
  <section v-loading.fullscreen.lock="false"></section>
  <ErrorBoundary>
    <router-view />
  </ErrorBoundary>
</template>

<script setup lang="ts">
import ErrorBoundary from '@/components/common/ErrorBoundary.vue'
</script>
