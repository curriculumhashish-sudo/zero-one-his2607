<script setup lang="ts">
import { ref } from 'vue'
import { Document, FolderOpened } from '@element-plus/icons-vue'

import type {
  DoctorMedicalRecordTemplateNodeVO,
  DoctorPatientContextVO
} from '@/apis/outpatient/doctor-station'
import TreeFilterPanel, { type TreeNodeData } from '@/components/common/TreeFilterPanel.vue'

import PatientContextPanel from './PatientContextPanel.vue'

defineOptions({
  name: 'DoctorStationAssistantPanel'
})

const props = withDefaults(
  defineProps<{
    patientSelected?: boolean
    context?: DoctorPatientContextVO | null
    contextLoading?: boolean
    contextError?: string
    templates?: DoctorMedicalRecordTemplateNodeVO[]
    templatesLoading?: boolean
    templatesError?: string
    applyingTemplate?: boolean
    canApplyTemplate?: boolean
  }>(),
  {
    patientSelected: false,
    context: null,
    contextLoading: false,
    contextError: '',
    templates: () => [],
    templatesLoading: false,
    templatesError: '',
    applyingTemplate: false,
    canApplyTemplate: false
  }
)

const emit = defineEmits<{
  retryContext: []
  retryTemplates: []
  applyTemplate: [template: DoctorMedicalRecordTemplateNodeVO]
}>()

// 右侧默认打开模板，医生选中患者后可以直接套用；就诊信息仍保留在第二个页签。
const activePane = ref<'templates' | 'context'>('templates')
const filterText = ref('')
const currentTemplateKey = ref<string | number | null>(null)

// 告诉公共树组件：节点文字在 name 字段，子节点在 children 字段。
const templateTreeProps = {
  label: 'name',
  children: 'children',
  isLeaf: 'isLeaf'
}

const templateError = () => (props.templatesError ? new Error(props.templatesError) : null)

function selectTemplate(data: TreeNodeData) {
  const template = data as DoctorMedicalRecordTemplateNodeVO

  // CATEGORY 是目录，只负责展开；只有 TEMPLATE 叶子节点会触发套用。
  if (template.type === 'TEMPLATE' && props.canApplyTemplate) {
    emit('applyTemplate', template)
  }
}
</script>

<template>
  <section class="doctor-station-assistant">
    <el-tabs v-model="activePane" class="doctor-station-assistant__tabs" stretch>
      <el-tab-pane label="病历模板" name="templates">
        <!--
          加载模板时使用骨架屏，不给整棵树设置透明度。
          透明加载层在部分 Chrome 环境中会让右栏所有文字看起来发白。
        -->
        <div v-if="templatesLoading" class="doctor-station-assistant__skeleton">
          <el-skeleton :rows="8" animated />
        </div>

        <TreeFilterPanel
          v-else
          v-model:current-key="currentTemplateKey"
          v-model:filter-text="filterText"
          :data="templates"
          node-key="id"
          :props="templateTreeProps"
          :default-expanded-keys="['category-common', 'category-cardiology']"
          :error="templateError()"
          search-placeholder="搜索病历模板"
          empty-text="暂无病历模板"
          class="doctor-station-assistant__tree"
          @select="selectTemplate"
          @retry="emit('retryTemplates')"
        >
          <template #node="{ data }">
            <span
              class="doctor-station-assistant__node"
              :class="{ 'is-disabled': data.type === 'TEMPLATE' && !canApplyTemplate }"
            >
              <el-icon>
                <FolderOpened v-if="data.type === 'CATEGORY'" />
                <Document v-else />
              </el-icon>
              <span>{{ data.name }}</span>
            </span>
          </template>

          <template #footer>
            <span class="doctor-station-assistant__tip">
              {{
                applyingTemplate
                  ? '正在套用模板，请稍候……'
                  : canApplyTemplate
                    ? '点击模板即可套用，套用后仍需手动保存'
                    : '接诊后才能把模板套用到病历'
              }}
            </span>
          </template>
        </TreeFilterPanel>
      </el-tab-pane>

      <el-tab-pane label="就诊信息" name="context">
        <PatientContextPanel
          :patient-selected="patientSelected"
          :context="context"
          :loading="contextLoading"
          :error="contextError"
          @retry="emit('retryContext')"
        />
      </el-tab-pane>
    </el-tabs>
  </section>
</template>

<style scoped>
.doctor-station-assistant {
  box-sizing: border-box;
  height: 100%;
  min-height: 0;
  padding: 0 8px 8px;
  color: #303133;
  background: var(--el-bg-color);
}

.doctor-station-assistant__tabs {
  display: flex;
  height: 100%;
  min-height: 0;
  flex-direction: column;
}

.doctor-station-assistant__tabs :deep(.el-tabs__header) {
  flex-shrink: 0;
  margin: 0 0 8px;
}

.doctor-station-assistant__tabs :deep(.el-tabs__item) {
  color: #606266;
}

.doctor-station-assistant__tabs :deep(.el-tabs__item.is-active) {
  color: var(--el-color-primary, #409eff);
  font-weight: 600;
}

.doctor-station-assistant__tabs :deep(.el-tabs__content),
.doctor-station-assistant__tabs :deep(.el-tab-pane) {
  min-height: 0;
  flex: 1;
  overflow: hidden;
}

.doctor-station-assistant__tabs :deep(.el-tabs__content) {
  display: flex;
  flex-direction: column;
}

.doctor-station-assistant__skeleton {
  box-sizing: border-box;
  height: 100%;
  padding: 16px 8px;
  background: #ffffff;
}

.doctor-station-assistant__tree {
  border: 0;
  border-radius: 0;
}

.doctor-station-assistant__node {
  display: inline-flex;
  min-width: 0;
  align-items: center;
  gap: 6px;
  color: #303133;
}

/*
 * “尚未接诊”只限制点击套用，不再用 placeholder 浅色降低可读性。
 * 用户仍可清楚看到有哪些模板，底部提示负责说明为什么当前不能套用。
 */
.doctor-station-assistant__node.is-disabled {
  color: #606266;
  cursor: not-allowed;
}

.doctor-station-assistant__tip {
  color: #606266;
  font-size: 12px;
  line-height: 1.5;
}
</style>
