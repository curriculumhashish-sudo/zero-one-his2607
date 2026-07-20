# PG → MySQL 转换工具 详细参考

## 文件结构

```
convert_pg_to_mysql.py    # 主转换脚本
{prefix}.sql               # 输入：PG 导出（pg_dump --inserts），默认为 public.sql
{prefix}_mysql_ddl.sql     # 输出：建表语句
{prefix}_mysql_dml.sql     # 输出：数据 DML
{prefix}_mysql_idx.sql     # 输出：索引/约束
{prefix}_convert_log.txt   # 输出：转换日志
```

`{prefix}` 由命令行参数指定（如 `python convert_pg_to_mysql.py custom.sql` → prefix=`custom`），未指定时默认为 `public`。

## 配置变量（convert_pg_to_mysql.py 顶部）

输入文件名通过命令行参数传入，脚本自动根据文件名推导前缀和输出路径。

```python
# 默认行为（不传参数）
#   INPUT_FILE = {_PROJ}/public.sql
#   输出：public_mysql_ddl.sql, public_mysql_dml.sql, public_mysql_idx.sql

# 指定文件（如 custom.sql）
#   INPUT_FILE = {_PROJ}/custom.sql
#   输出：custom_mysql_ddl.sql, custom_mysql_dml.sql, custom_mysql_idx.sql
```

## 类型映射表（完整）

| PG 类型 | MySQL 类型 | 备注 |
|---------|-----------|------|
| `int8` (列名=`id`) | `VARCHAR(64)` | 主键 → 字符串 |
| `int8` (列名以 `_id`/`_by` 结尾) | `VARCHAR(64)` | 外键 → 字符串 |
| 其他 `int8` | `BIGINT` | |
| `int4` | `INT` | |
| `int2` | `SMALLINT` | |
| `timestamptz(N)` | `DATETIME(N)` | 去除时区偏移 |
| `timestamp(N)` | `DATETIME(N)` | |
| `bool` | `TINYINT(1)` | `true`→1, `false`→0 |
| `jsonb` / `json` | `JSON` | 含编码清洗 |
| `numeric(p,s)` | `DECIMAL(p,s)` | |
| `varchar(N)` (N ≤ 16383) | `VARCHAR(N)` | |
| `varchar(N)` (N > 16383) | `TEXT` | MySQL 上限 16383 |
| `text` | `TEXT` | |
| `bytea` | `LONGBLOB` | |
| `uuid` | `CHAR(36)` | |
| `float4` | `FLOAT` | |
| `float8` | `DOUBLE` | |
| `date` | `DATE` | |
| `serial4`/`serial8` | `BIGINT AUTO_INCREMENT` | |

## 数据清洗流程

### 1. 布尔值转换
```sql
-- PG: ... VALUES (..., 't', ...)  →  MySQL: ... VALUES (..., 1, ...)
-- PG: ... VALUES (..., 'f', ...)  →  MySQL: ... VALUES (..., 0, ...)
```

### 2. 时区剥离
```sql
-- PG: '2026-06-29 14:44:07.79893+08'  →  MySQL: '2026-06-29 14:44:07.79893'
```

### 3. 大整数 ID 包裹
```sql
-- PG: 329874660332146688  →  MySQL: '329874660332146688'
-- (15-20 位数字，匹配 VARCHAR(64) 列)
```

### 4. JSON 清洗（在 convert_data_line 和 main 中双重执行）
```python
obj = json.loads(raw)                          # 解析
clean = json.dumps(obj, ensure_ascii=False)    # 重新序列化，消除 \u0000 等
```

### 5. DML 日志表跳过
以下表不生成 INSERT 数据：
```python
SKIP_DML_TABLES = {
    'flyway_schema_history', 'flyway_schema_history_booking',
    'flyway_schema_history_chs', 'flyway_schema_history_elep',
    'flyway_schema_history_emr', 'whale_audit_logs',
    'whale_qrtz_job_details', 'whale_qrtz_cron_triggers',
    'whale_qrtz_fired_triggers', 'whale_qrtz_locks',
    'whale_qrtz_scheduler_state', 'whale_qrtz_simple_triggers',
    'whale_qrtz_triggers', 'whale_security_logs',
    'whale_user_login_session', 'yb_inv_upload',
}
```

## 索引处理

- 自动为 VARCHAR 列添加 `(64)` 前缀（仅当列长 > 64）
- 为 TEXT/BLOB 列添加 `(64)` 前缀
- INT 列不添加前缀（会触发 MySQL 1089 错误）
- 移除 PG 的 `USING btree`、`COLLATE`、`ASC/DESC NULLS FIRST/LAST`
- WHERE 子句转为注释（MySQL 不支持部分索引）

## 常见错误处理

| 错误 | 原因 | 解决 |
|------|------|------|
| 1071 (索引键长超限) | VARCHAR 组合索引 > 3072 字节 | 自动添加 `(64)` 前缀 |
| 1074 (VARCHAR 过长) | VARCHAR > 16383 | 自动转 TEXT |
| 1089 (INT 前缀) | INT 列加索引前缀 | 模式匹配排除 `_no` 后缀 |
| 1067 (DATETIME 默认值) | 精度不匹配 | 自动补 `CURRENT_TIMESTAMP(N)` |
| 1101 (TEXT 默认值) | TEXT/BLOB/JSON 不能有 DEFAULT | 自动移除 DEFAULT |
| 3140 (JSON 编码) | PG jsonb 含非法字符 | json.loads→dumps 清洗 |
| `COLLATE` 语法错误 | 孤立 COLLATE 关键字 | 构建时清理 |
