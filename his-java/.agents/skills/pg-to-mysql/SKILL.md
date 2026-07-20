---
name: pg-to-mysql
description: 将 PostgreSQL 数据库导出文件（public.sql）转换为 MySQL 8 兼容的 DDL、DML 和索引文件。当用户需要迁移 PG 数据到 MySQL、执行数据库转换、或处理 public.sql 文件时使用此技能。
---

# PG → MySQL 转换

## 文件结构

```
.agents/skills/pg-to-mysql/
├── SKILL.md                 ← 本文件
├── reference.md             ← 详细技术参考
└── scripts/
    ├── convert_pg_to_mysql.py   ← 主转换脚本
    ├── _fix_dml2.py             ← DML 引号修补
    ├── extract_comments.py      ← 提取 PG 表注释
    ├── remove_drop_table.py     ← 移除 DROP TABLE 语句
    └── test_translate.py        ← 注释翻译测试
```

## 快速使用

```powershell
# 默认转换 public.sql（一步到位）
python .agents/skills/pg-to-mysql/scripts/convert_pg_to_mysql.py

# 指定其他输入文件
python .agents/skills/pg-to-mysql/scripts/convert_pg_to_mysql.py custom.sql

# 辅助脚本（按需使用）
python .agents/skills/pg-to-mysql/scripts/remove_drop_table.py   # 从 DDL 中移除 DROP TABLE
python .agents/skills/pg-to-mysql/scripts/_fix_dml2.py            # 修补 DML 大整数 ID 引号
python .agents/skills/pg-to-mysql/scripts/extract_comments.py     # 查看表注释
python .agents/skills/pg-to-mysql/scripts/test_translate.py       # 测试注释翻译
```

## 输出文件

输出文件前缀由输入文件名决定（不含 `.sql` 扩展名）。例如 `public.sql` → 前缀 `public`，`custom.sql` → 前缀 `custom`。

| 文件 | 内容 |
|------|------|
| `{prefix}_mysql_ddl.sql` | 建表语句（ENGINE=InnoDB, utf8mb4） |
| `{prefix}_mysql_dml.sql` | 数据插入语句（压缩格式，一条数据一行） |
| `{prefix}_mysql_idx.sql` | 索引和约束 |
| `{prefix}_convert_log.txt` | 转换日志 |

## 核心转换规则

| PG 类型 | MySQL 类型 |
|---------|-----------|
| `int8` (PK) / `int8` (FK `_id`/`_by`) | `VARCHAR(64)` |
| 其他 `int8` | `BIGINT` |
| `int4` | `INT` |
| `timestamptz(N)` / `timestamp(N)` | `DATETIME(N)` |
| `bool` | `TINYINT(1)` |
| `jsonb` | `JSON` |
| `numeric(p,s)` | `DECIMAL(p,s)` |
| `varchar(N)` (N>16383) | `TEXT` |
| `serial` | `BIGINT AUTO_INCREMENT` |

## 跳过的日志表

DML 输出自动排除以下日志/审计/迁移历史表（不生成 INSERT 数据）：

`flyway_schema_history`、`flyway_schema_history_booking/chs/elep/emr`、`whale_audit_logs`、`whale_qrtz_*`、`whale_security_logs`、`whale_user_login_session`、`yb_inv_upload`

> 如需调整跳过列表，修改 `process_sql()` 中的 `SKIP_DML_TABLES` 集合。

## 高级功能

### JSON 清洗
自动检测 JSON 列并执行 `json.loads` → `json.dumps` 标准化，清除 PG jsonb 中可能存在但 MySQL JSON 不接受的字符（如 `\u0000`）。

### 索引前缀
自动为超过 64 字符的 VARCHAR 列添加 `(64)` 索引前缀，防止 utf8mb4 下键长超过 3072 字节的 1071 错误。

### 注释翻译
自动将 PG 英文表注释和列注释翻译为中文。

## 辅助脚本说明

| 脚本 | 用途 | 使用时机 |
|------|------|---------|
| `convert_pg_to_mysql.py` | 主转换：public.sql → DDL + DML + IDX | 每次导表后运行 |
| `remove_drop_table.py` | 清除 DDL 中的 `DROP TABLE IF EXISTS` | DDL 生成后（避免误删已有表） |
| `_fix_dml2.py` | 修复 DML 中大整数 ID 的引号错位 | DML 生成后，出现 1074 错误时 |
| `extract_comments.py` | 从 public.sql 提取表 COMMENT ON TABLE | 检查 PG 表注释时 |
| `test_translate.py` | 测试注释翻译映射是否正确 | 修改翻译表后验证 |

## 配置参数

输入文件名通过命令行参数指定，路径自动基于脚本所在位置解析（项目根目录）。详细参考 [reference.md](reference.md)。

```powershell
# 使用默认文件 public.sql
python .agents/skills/pg-to-mysql/scripts/convert_pg_to_mysql.py

# 指定自定义输入文件
python .agents/skills/pg-to-mysql/scripts/convert_pg_to_mysql.py 自定义.sql
```
