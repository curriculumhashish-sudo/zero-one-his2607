#### 用途

用于显示单据顶部基本信息、审核信息和操作区域



#### 实现要求

- 组件分为三个区域：header、body、footer，每个区域分别对应一个 slot 供父组件使用。
- header区实现显示标题、状态（支持 create/edit/readonly/audited 的表现状态，状态由外部传入，状态显示可直接复用现有组件StatusDisplay(路径：src/components/common/StatusDisplay.vue)）以及查询组件（查询组件可直接复用现有组件，路径：src\components\common\SearchPanel.vue，组件不定义具体字段）
- body区承载不同的单据表单（表单组件可直接复用现有组件，路径：src\components\common\DataTablePanel.vue），组件不定义具体字段
- footer区仅承载保存单据、保存并提交审批、删除单据、提交审批、撤回、审批、驳回的按钮，组件不判断对应业务逻辑



#### 相应参数

Props:title、status、statusMap、approver、approvalTime、showAuditInforeadonly、loading
Emits: action
Slots: title、default、actions、status、auditInfo、extra