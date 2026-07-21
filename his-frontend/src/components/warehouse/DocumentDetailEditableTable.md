#### 用途

用于入库、出库、调拨、盘点等单据的可编辑表格明细的公共组件



#### 实现要求

- 组件分header区和body区，header区设置两个按钮：“添加行”和“删除行”，用于控制body区表格项的操作，由父组件控制其是否显示
- body区为表格，可直接复用现有公共组件(路径：src\components\common\DataTablePanel.vue)，默认为空状态，表格列项应包含可选框，供header区的“删除行”按钮进行批量删除操作
- 点击“添加行”按钮时，表单添加一行数据为空或默认字段的表单项，然后由用户直接点击表单各项进行编辑，可支持输入框、数字、下拉等基础编辑类型，下拉项选择等通过 slot 接入
- 表格明细 rows 受控，新增、修改、删除通过事件回传，组件不保存后台
- 每行具有稳定的 rowKey，新建未保存行使用不会冲突的临时 key
- 支持行级和整表校验，错误能够定位到具体行列
- 支持只读、编辑、loading、空明细和禁用行
- 删除前是否确认由配置或页面决定，删除只触发事件
- 支持行金额/数量变化事件，但税额、金额等业务计算由外部函数完成



#### 推荐接口参数

- Props：modelValue、rowKey、columns、readonly、loading、disabledRow、rules、showAdd、showDelete、maxRows
- Emits：update:modelValue、rowAdd、rowChange、rowDelete、 cellChange、validationError、selectionChange
- Slots：toolbar、cell-{prop}、editor-{prop}、empty、summary、 actions
- 暴露方法：validate()、clearValidate()、focusCell(rowKey, prop)



#### 暂不实现

- 不直接请求商品、批号或库存接口
- 不在组件内部规定税率、售价、库存扣减和审核规则