# DetailPanel（C11）

`DetailPanel` 是右侧详情外壳组件，用来统一详情标题、顶部操作、滚动内容、加载状态、空状态、只读/编辑模式和底部操作栏。

组件通过插槽组合业务内容，不绑定具体详情接口，也不处理保存参数转换。

组件路径：

```text
src/components/common/DetailPanel.vue
```

## 1. 基础用法

```vue
<script setup lang="ts">
import { ref } from 'vue'
import DetailPanel from '@/components/common/DetailPanel.vue'

const readonly = ref(true)
const loading = ref(false)
const selectedRecord = ref({ id: 1, name: '张三' })

function save() {
  // 调用业务保存接口。
  readonly.value = true
}

function cancel() {
  // 恢复编辑前的数据。
  readonly.value = true
}
</script>

<template>
  <DetailPanel
    title="患者详情"
    :loading="loading"
    :readonly="readonly"
    :empty="!selectedRecord"
    sticky-footer
    @edit="readonly = false"
    @save="save"
    @cancel="cancel"
  >
    <template #default="{ readonly: panelReadonly }">
      <el-form :disabled="panelReadonly">
        <el-form-item label="患者姓名">
          <el-input v-model="selectedRecord.name" />
        </el-form-item>
      </el-form>
    </template>
  </DetailPanel>
</template>
```

## 2. Props

| 属性 | 类型 | 默认值 | 说明 |
| --- | --- | --- | --- |
| `title` | `string` | `''` | 默认标题；可被 `title` 插槽替换 |
| `loading` | `boolean` | `false` | 是否显示详情加载状态 |
| `readonly` | `boolean` | `true` | 当前是否为只读模式 |
| `empty` | `boolean` | `false` | 是否显示未选择记录的空状态 |
| `emptyDescription` | `string` | `'请选择一条记录'` | 空状态说明 |
| `stickyFooter` | `boolean` | `true` | 底部栏是否吸附在滚动内容底部 |

## 3. 事件

| 事件 | 参数 | 默认触发位置 |
| --- | --- | --- |
| `edit` | 无 | 只读模式下的默认“编辑”按钮 |
| `save` | 无 | 编辑模式下的默认“保存”按钮 |
| `cancel` | 无 | 编辑模式下的默认“取消”按钮 |

组件只发出操作意图，具体接口调用由使用页面处理。

## 4. 插槽

| 插槽 | 插槽参数 | 说明 |
| --- | --- | --- |
| `title` | 无 | 自定义标题 |
| `actions` | `{ readonly }` | 自定义顶部操作区 |
| `default` | `{ readonly }` | 详情主体内容 |
| `footer` | `{ readonly }` | 自定义底部操作区 |

顶部操作示例：

```vue
<DetailPanel title="患者详情" :readonly="readonly">
  <template #actions="{ readonly: panelReadonly }">
    <el-tag>{{ panelReadonly ? '只读' : '编辑中' }}</el-tag>
    <el-button v-if="panelReadonly" @click="readonly = false">
      编辑
    </el-button>
  </template>

  详情内容
</DetailPanel>
```

自定义底部操作示例：

```vue
<DetailPanel :readonly="false">
  <template #footer>
    <el-button @click="saveDraft">暂存</el-button>
    <el-button type="primary" @click="submit">提交</el-button>
  </template>

  详情内容
</DetailPanel>
```

## 5. 默认操作规则

组件未提供对应插槽时：

- `readonly=true` 且 `empty=false`：顶部显示“编辑”按钮。
- `readonly=false` 且 `empty=false`：底部显示“取消”和“保存”按钮。
- `empty=true`：不显示默认编辑、保存和取消按钮。
- 提供 `actions` 或 `footer` 插槽后，对应默认内容会被替换。

## 6. 只读状态说明

`readonly` 只表示详情面板当前所处模式，不会自动禁用插槽中的表单控件。

需要通过默认插槽参数将状态传递给内部表单：

```vue
<DetailPanel :readonly="readonly">
  <template #default="{ readonly: panelReadonly }">
    <el-form :disabled="panelReadonly">
      <!-- 具体业务字段 -->
    </el-form>
  </template>
</DetailPanel>
```

这样可以保持组件与具体业务表单解耦。

## 7. 加载与空状态

- `loading=true` 时显示加载遮罩，暂时不渲染详情内容。
- `loading=false` 且 `empty=true` 时显示 `el-empty`。
- `loading=false` 且 `empty=false` 时渲染默认插槽。

典型用法：

```vue
<DetailPanel
  :loading="detailLoading"
  :empty="!selectedId"
  empty-description="请从左侧选择一条记录"
/>
```

## 8. stickyFooter

当 `stickyFooter=true` 时，底部操作栏使用 `position: sticky` 吸附在内容区底部。详情内容较长时，用户仍可以看到操作按钮。

如果希望底部栏随内容正常滚动：

```vue
<DetailPanel :sticky-footer="false" />
```

## 9. 布局要求

组件使用 `height: 100%`，父容器需要有明确高度：

```css
.right-content {
  display: flex;
  flex: 1;
  height: 100%;
  min-width: 0;
  min-height: 0;
}
```

`min-width: 0` 和 `min-height: 0` 可以避免详情内容把 Flex 布局撑开。

## 10. 职责边界

适合放入：

- 详情标题和通用操作
- 查看/编辑模式切换
- 加载和未选择记录的空状态
- 可滚动详情内容
- 固定底部操作

不应放入组件内部：

- 详情数据请求
- 患者、单号等业务字段
- 表单校验规则
- 保存参数转换
- 路由跳转

