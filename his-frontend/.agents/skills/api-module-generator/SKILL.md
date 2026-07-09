---
name: "api-module-generator"
description: "根据数据模型或接口文档，自动生成符合项目规范的 API 模块代码（type.ts 和 index.ts）。当用户需要新增接口对接、创建 API 模块或添加后端接口调用代码时调用。"
---

# API 模块生成器

根据用户提供的数据模型或接口描述，自动生成符合项目规范的 API 模块代码。

## 适用场景

- 用户需要对接新的后端接口模块
- 用户描述了一组 CRUD 接口需要生成调用代码
- 用户提供了 Swagger / 接口文档需要生成对应的 API 层
- 用户新增了一个业务模块需要创建对应的 API 文件夹

## 项目技术栈

- **HTTP 客户端**: `useHttp()` 从 `@/plugins/http` 导入
- **请求基类**: `HttpClient`（位于 `src/apis/http.ts`）
- **通用类型**: 从 `src/apis/type.ts` 导入（`PageQuery`, `PageDTO`, `SelectItem`, `JsonVO`）
- **回调类型**: `RequestCallback` 从 `@/plugins/http` 导入
- **数据上传类型**: `DataUpType` 从 `@/plugins/http` 导入

## 生成文件清单

对于模块名 `{module}`，生成以下文件：

| 文件 | 路径 | 说明 |
|------|------|------|
| 类型定义 | `src/apis/{module}/type.ts` | 请求/响应数据类型 |
| API 函数 | `src/apis/{module}/index.ts` | 接口调用函数 |

## 类型定义规范 (`type.ts`)

### 基本结构

```typescript
import type { PageQuery } from '../type'

/**
 * {模块中文名}数据模型（后端返回的 VO 对象）
 */
export interface {Module}VO {
  /** 主键ID */
  id?: number
  // ... 其他字段，每个字段必须有 JSDoc 注释
}

/**
 * {模块中文名}查询参数
 */
export interface {Module}Query extends PageQuery {
  // ... 搜索条件字段
}

/**
 * {模块中文名}新增/更新参数
 */
export interface {Module}DTO {
  // ... 表单提交字段（通常与 VO 类似但不含只读字段）
}
```

### 命名约定

| 后缀 | 含义 | 示例 |
|------|------|------|
| `VO` | 后端返回的视图对象 | `SampleFormData`, `AiAgentVO` |
| `DTO` | 前端提交的数据传输对象 | `LoginDTO`, `AiChatRequestDTO` |
| `Query` | 分页查询参数，必须 extends `PageQuery` | `SampleFormQuery` |

### 字段注释规范

```typescript
export interface SampleFormData extends Record<string, any> {
  /** 数据唯一标识 */
  id?: number
  /** 姓名 */
  name?: string
  /** 国家 */
  country?: string
  /** 省份 */
  state?: string
  /** 城市 */
  city?: string
  /** 地址 */
  address?: string
  /** 邮编 */
  zip?: number
  /** 生日 */
  date?: string
  /** 标签 */
  tags?: string
}
```

## API 函数规范 (`index.ts`)

### 文件头部

```typescript
import { useHttp, type RequestCallback } from '@/plugins/http'
import type { PageDTO } from '../type'
import type { {Module}VO, {Module}Query, {Module}DTO } from './type'

// 定义一个功能模块基础url，方便替换
const baseUrl = '/{module-path}'
```

### 两种调用模式

项目支持两种 API 调用模式，根据场景选用：

#### 模式一：回调模式（推荐用于增删改查操作）

适用于需要 `showLoading`、后续处理复杂的场景：

```typescript
/**
 * 分页查询
 * @param data 查询条件
 * @param success 成功回调
 * @param fail 失败回调
 */
export const listAll = async (
  data: {Module}Query,
  success: RequestCallback,
  fail: RequestCallback
) => {
  try {
    const http = useHttp()
    const res = await http.get<PageDTO<{Module}VO>>(baseUrl + '/query-all', data, {
      showLoading: true
    })
    if (res.data) {
      success(res.data)
      return
    }
    fail(res)
  } catch (err) {
    fail(err)
  }
}
```

**回调模式要点**：
- 函数参数顺序：`data, success, fail`
- 内部使用 `try/catch` 包裹
- 成功时检查 `res.data` 是否存在
- 失败时调用 `fail` 回调
- 分页查询返回类型为 `PageDTO<T>`

#### 模式二：async/await 模式（推荐用于简单获取类接口）

适用于获取下拉选项、获取详情等简单场景：

```typescript
/**
 * 获取字典数据
 * @param type 数据类型
 * @returns 异步数据对象
 */
export const getOptions = (type: string) => {
  const http = useHttp()
  return http.get<Array<SelectItem>>(baseUrl + '/options', { code: type })
}
```

或更简洁的写法：

```typescript
/**
 * 获取智能体列表
 */
export const getAgents = async () => {
  const http = useHttp()
  const res = await http.get<AiAgentVO[]>(baseUrl + '/agents', undefined, {
    baseURL: '/ai',
    timeout: 100000
  })
  return Array.isArray(res?.data) ? res.data : []
}
```

**async/await 模式要点**：
- 函数返回 `Promise<T>` 或直接返回 `http.get/post/...` 的结果
- 适合调用方自行处理错误
- 无需 `RequestCallback` 类型

### HTTP 方法参考

基于 `src/apis/http.ts` 中 `HttpClient` 类的方法：

| 方法 | 用法 | 说明 |
|------|------|------|
| `http.get<T>(url, params, config)` | GET 请求 | params 自动序列化到 URL |
| `http.post<T>(url, data, config)` | POST 请求 | 默认 JSON 格式 |
| `http.put<T>(url, data, config)` | PUT 请求 | 默认 JSON 格式 |
| `http.delete<T>(url, data, config)` | DELETE 请求 | 默认 JSON 格式 |
| `http.postWithFile<T>(url, data, config)` | 文件上传 | data 自动转 FormData |
| `http.postFileStream<T>(url, file, success, fail, config)` | 二进制流上传 | 通过 FileReader 读取 |
| `http.getFile(url, params, config)` | 下载文件 | 返回完整响应对象 |

### 常用配置项 (`CustomAxiosRequestConfig`)

```typescript
{
  showLoading: true,           // 显示全局 loading
  upType: DataUpType.form,     // 表单提交（application/x-www-form-urlencoded）
  upType: DataUpType.json,     // JSON 提交（application/json）—— 默认
  upType: DataUpType.file,     // 文件上传（multipart/form-data）
  upType: DataUpType.stream,   // 二进制流
  baseURL: '/custom-base',     // 覆盖默认 baseURL
  timeout: 100000              // 覆盖默认超时时间（毫秒）
}
```

### 完整 CRUD 示例

```typescript
import { useHttp, type RequestCallback } from '@/plugins/http'
import type { PageDTO } from '../type'
import type { UserVO, UserQuery, UserDTO } from './type'

const baseUrl = '/sys/user'

/** 分页查询用户 */
export const listUser = async (data: UserQuery, success: RequestCallback, fail: RequestCallback) => {
  try {
    const http = useHttp()
    const res = await http.get<PageDTO<UserVO>>(baseUrl + '/query-all', data, { showLoading: true })
    if (res.data) { success(res.data); return }
    fail(res)
  } catch (err) { fail(err) }
}

/** 新增用户 */
export const addUser = async (data: UserDTO, success: RequestCallback, fail: RequestCallback) => {
  try {
    const http = useHttp()
    const res = await http.post<UserVO>(baseUrl + '/add', data, { showLoading: true })
    if (res.data) { success(res.data); return }
    fail(res)
  } catch (err) { fail(err) }
}

/** 更新用户 */
export const updateUser = async (data: UserDTO, success: RequestCallback, fail: RequestCallback) => {
  try {
    const http = useHttp()
    const res = await http.put<UserVO>(baseUrl + '/update', data, { showLoading: true })
    if (res.data) { success(res.data); return }
    fail(res)
  } catch (err) { fail(err) }
}

/** 删除用户 */
export const deleteUser = async (id: number, success: RequestCallback, fail: RequestCallback) => {
  try {
    const http = useHttp()
    const res = await http.delete(baseUrl + `/delete/${id}`, undefined, { showLoading: true })
    if (res.data) { success(res.data); return }
    fail(res)
  } catch (err) { fail(err) }
}

/** 获取用户详情 */
export const getUserById = (id: number) => {
  const http = useHttp()
  return http.get<UserVO>(baseUrl + `/detail/${id}`)
}

/** 获取用户角色选项（用于下拉框） */
export const getUserRoles = () => {
  const http = useHttp()
  return http.get<SelectItem[]>(baseUrl + '/roles')
}
```

## 执行流程

当用户请求生成 API 模块时：

1. **收集信息**：
   - 模块名称（英文路径）
   - 模块中文名
   - 数据字段列表（字段名、类型、中文描述）
   - 需要的接口类型（分页查询、新增、更新、删除、详情、导出等）
   - 特殊配置（如自定义 baseURL、超时时间等）
2. **生成 `type.ts`**：根据字段列表生成类型定义，注意 `Query` 需继承 `PageQuery`
3. **生成 `index.ts`**：根据接口需求生成函数，选择合适的调用模式
4. **验证**：确认导入路径正确、类型引用一致、`baseUrl` 变量名统一

## 注意事项

- `PageQuery` 接口已定义在 `src/apis/type.ts`，包含 `pageIndex` 和 `pageSize`
- `PageDTO<T>` 和 `createPageDTO()` 在 `src/apis/type.ts` 中重新导出
- `RequestCallback` 类型为 `(res: any) => void`
- `useHttp()` 可在组件外（如 API 文件中）调用，它通过全局实例获取
- 每个模块使用独立的 `baseUrl` 变量，方便后续替换
- 异步函数内部必须使用 `try/catch` 包裹
- 如果需要自定义 Content-Type，使用 `DataUpType` 枚举
- 文件上传使用 `postWithFile` 方法，该方法会自动将 JS 对象转为 FormData
