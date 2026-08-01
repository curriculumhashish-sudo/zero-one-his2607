#### 页面结构

左右布局：

- 左侧顶部为搜索框，用于关键词查询，下方为标签页，可切换“字典系统”和“枚举系统”，标签页页面的顶部为”新增“按钮（独占一行），下方显示字典类型或枚举类型的类型项
- 右侧顶部为查询区（共占一行）：
  - 关键词：（普通输入框，placeholder="请输入关键词"）
  - 名称：（普通输入框，placeholder="请输入名称"）
  - 查询/重置按钮
- 查询区下部为”新增“按钮（独占一行），再下方为表格，表格列项包括：序号、名称、编码、排序、描述、操作（固定为编辑、删除链接），表格项内容则为左侧所选类型项的字典项或枚举项



#### 新增逻辑

##### 左侧新增：

点击左侧”字典系统“标签页下的”新增“按钮时，显示类型项的新增表单弹框：

- 表单标题：新增字典
- 名称：（普通输入框，placeholder="请输入名称"）
- 标题：（普通输入框，placeholder="请输入标题"）
- 版本：（普通输入框，placeholder="请输入版本"）
- 描述：（type="textarea"，placeholder="请输入描述"）

其中名称、标题、版本不能为空（定义规则）

点击确定后，下方新增一行类型项，文本为：”标题“+”-“+”名称“（例如名称为”vital_signs_code“，标题为”生命体特征“，则该行文本为：生命体特征-vital_signs_code），右侧同步显示空状态

“枚举系统”同理



##### 右侧新增：

点击右侧的”新增“按钮时，显示表格项的新增表单弹框：

- 表单标题：新增字典编码
- 编码：（普通输入框，placeholder="请输入名称"）
- 名称：（普通输入框，placeholder="请输入标题"）
- 描述：（普通输入框，placeholder="请输入描述"）
- 排序：（数字输入框，要求direction="vertical" controls-position="right"，且数字必须≥0，placeholder="请输入排序号（越小越靠前）"）

其中编码、名称、描述不能为空

点击确定后，表单新增一行表单项，并渲染对应数据

点击操作栏中的”编辑“时，显示编辑表单弹框（除标题为”编辑字典编码“，且编码项不可操作外，其余与新增弹框类似），同样点击确定后更改的数据会渲染至对应表单项；点击”删除“时，经过消息弹出框组件的二次确认后可删除当前行的表格项

“枚举系统”同理



#### 查询逻辑：

##### 左侧查询：

左侧顶部的搜索框采用节流思想，根据输入的关键词筛选下方文本对应的类型项，切换页签时，搜索框自动清空

##### 右侧查询：

关键词查询框可对应：”名称“、”编码“和”描述“的任意文本内容，而名称查询框只可对应”名称“，点击搜索按钮后下方显示对应的表单项



#### 核心逻辑：

左侧加载类型项和右侧加载表格项均须添加loading效果

进入页面时左侧默认选中”字典系统“的第一项，同时右侧表格显示对应的字典项

左侧选择发生变化时，右侧须先清空旧的对应项，再加载新的

以上组件均使用 `Element Plus` 库对应的组件实现



#### 模拟数据生成与接口管理：

”字典系统“和”枚举系统“分别生成10组模拟数据，要求在接口目录：`src\apis\basic\code\` 下实现，其中：

- `index.ts` ：定义和导出接口调用函数，并作为出口在 `<script>` 中调用
- `mock.ts` ：生成随机/模拟数据，定义 Mock 规则
- `type.ts` ：数据类型定义

##### 可用的接口名称：

字典类型

- `pageDictionaryTypes(query)`：查询字典类型。
- `createDictionaryType(data)`：新增字典类型。
- `updateDictionaryType(id, data)`：修改字典类型。
- `removeDictionaryType(id)`：删除字典类型。

字典管理

- `loadDictionaryOptions(keyword)`：其他页面获取字典名称选项。
- `pageDictionaryItems(query)`：查询当前类型下字典项。
- `createDictionaryItem(data)`：新增字典项。
- `updateDictionaryItem(id, data)`：修改字典项。
- `removeDictionaryItem(id)`：删除字典项。

枚举类型

- `pageEnumTypes(query)`、`createEnumType(data)`、`updateEnumType(id, data)`、`removeEnumType(id)`。

枚举管理

- `loadEnumOptions(keyword)`、`pageEnumItems(query)`、`createEnumItem(data)`、`updateEnumItem(id, data)`、`removeEnumItem(id)`。