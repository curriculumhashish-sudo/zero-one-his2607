# SectionTitle

统一页面分区标题。用于表单分区、详情区域、表格分组和工作站区域标题。

## 基础用法

```vue
<SectionTitle title="患者基本信息" description="展示本次就诊患者的基础资料" />
```

## 紧凑标题和操作区

```vue
<SectionTitle title="单据明细" dense bordered>
  <template #actions>
    <el-button type="primary">新增明细</el-button>
    <el-button>批量删除</el-button>
  </template>
</SectionTitle>
```

## Props

| 属性 | 类型 | 默认值 | 说明 |
|---|---|---|---|
| `title` | `string` | 必填 | 标题 |
| `description` | `string` | 空字符串 | 说明文字 |
| `dense` | `boolean` | `false` | 紧凑模式 |
| `bordered` | `boolean` | `false` | 是否显示底部分隔线 |
| `level` | `2 \| 3 \| 4 \| 5 \| 6` | `3` | 标题语义层级 |

## Slots

| 插槽 | 说明 |
|---|---|
| `default` | 覆盖标题内容 |
| `icon` | 标题前图标 |
| `description` | 覆盖说明内容 |
| `actions` | 右侧操作区域 |

## 边界说明

- 组件只负责标题结构和样式，不处理按钮权限及业务操作。
- 长标题允许换行，右侧操作区保持可见。
- 页面应根据标题层级设置 `level`，避免所有区域都使用同一语义标题。

