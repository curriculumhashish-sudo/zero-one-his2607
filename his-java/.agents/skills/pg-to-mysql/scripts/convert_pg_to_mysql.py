#!/usr/bin/env python3
"""
PostgreSQL → MySQL 8 转换脚本 (v3 - final)
- 主键 int8 → VARCHAR(64)
- 外键 int8（_id/_by 结尾） → VARCHAR(64)  
- 其他 int8 → BIGINT
- 建表语句和数据语句分成两个文件
"""

import os
import re
import sys
import json as _json

# Resolve paths relative to project root (five levels up from this script)
_PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))

# Determine input file name: use command-line arg if provided, otherwise default to public.sql
_input_filename = sys.argv[1] if len(sys.argv) > 1 else 'public.sql'
_prefix = os.path.splitext(_input_filename)[0]  # e.g., 'public' or 'custom'

INPUT_FILE = os.path.join(_PROJ, _input_filename)
DDL_FILE   = os.path.join(_PROJ, f'{_prefix}_mysql_ddl.sql')
DML_FILE   = os.path.join(_PROJ, f'{_prefix}_mysql_dml.sql')
IDX_FILE   = os.path.join(_PROJ, f'{_prefix}_mysql_idx.sql')

def read_file(path):
    with open(path, 'r', encoding='utf-8') as f:
        return f.read()

def write_file(path, content):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)


def convert_column_type(col_name_raw, type_rest):
    """Convert a column's type definition from PG to MySQL."""
    col_name = col_name_raw.strip('"').lower()
    is_pk = (col_name == 'id')
    is_fk = col_name.endswith('_id') or col_name.endswith('_by')
    
    result = type_rest
    
    # Remove COLLATE clause completely (handles "pg_catalog"."default")
    result = re.sub(r'\s*COLLATE\s+"[^"]*"(?:\."default")?', '', result)
    
    # Remove leftover ."default" fragments from broken COLLATE removal
    result = re.sub(r'\.\s*"[^"]*"', '', result)
    
    # Remove ::type casts
    result = re.sub(r'::character\s+varying', '', result)
    result = re.sub(r'::varchar\(\d+\)', '', result)
    result = re.sub(r'::text', '', result)
    result = re.sub(r'::int[248]', '', result)
    result = re.sub(r'::bool', '', result)
    result = re.sub(r'::numeric\(\d+,\d+\)', '', result)
    
    # Type mappings
    if is_pk and re.search(r'\bint[48]\b', result):
        result = re.sub(r'\bint8\b', 'VARCHAR(64)', result)
        result = re.sub(r'\bint4\b', 'VARCHAR(64)', result)
    elif is_fk and re.search(r'\bint8\b', result):
        result = re.sub(r'\bint8\b', 'VARCHAR(64)', result)
    else:
        result = re.sub(r'\btimestamptz\(\d+\)', 'DATETIME(6)', result)
        result = re.sub(r'\btimestamp\(\d+\)', 'DATETIME(6)', result)
        result = re.sub(r'\bbool\b', 'TINYINT(1)', result)
        result = re.sub(r'\bint8\b', 'BIGINT', result)
        result = re.sub(r'\bint4\b', 'INT', result)
        result = re.sub(r'\bint2\b', 'SMALLINT', result)
        result = re.sub(r'\bfloat4\b', 'FLOAT', result)
        result = re.sub(r'\bfloat8\b', 'DOUBLE', result)
        result = re.sub(r'\bnumeric\((\d+),(\d+)\)', r'DECIMAL(\1,\2)', result)
        result = re.sub(r'\bvarchar\b', 'VARCHAR', result)
        result = re.sub(r'\btext\b', 'TEXT', result)
        result = re.sub(r'\bdate\b', 'DATE', result)
        result = re.sub(r'\bjsonb?\b', 'JSON', result)
        result = re.sub(r'\bbytea\b', 'LONGBLOB', result)
        result = re.sub(r'\bserial[48]\b', 'BIGINT AUTO_INCREMENT', result)
        result = re.sub(r'\bbigserial\b', 'BIGINT AUTO_INCREMENT', result)
        result = re.sub(r'\buuid\b', 'CHAR(36)', result)
    
    # MySQL VARCHAR max is 16383; beyond that use TEXT
    result = re.sub(r'VARCHAR\((\d+)\)', lambda m: 'TEXT' if int(m.group(1)) > 16383 else m.group(0), result)
    
    # MySQL strict mode: TEXT/BLOB/JSON columns cannot have DEFAULT values
    if re.search(r'\b(?:TEXT|LONGBLOB|JSON)\b', result):
        result = re.sub(r"\s+DEFAULT\s+'[^']*'", '', result)
        
    # Change DEFAULT false/true
    result = re.sub(r'DEFAULT\s+false\b', 'DEFAULT 0', result)
    result = re.sub(r'DEFAULT\s+true\b', 'DEFAULT 1', result)
    # Convert PG now() → MySQL CURRENT_TIMESTAMP
    result = re.sub(r'DEFAULT\s+now\(\)', 'DEFAULT CURRENT_TIMESTAMP', result, flags=re.IGNORECASE)
    # Match CURRENT_TIMESTAMP precision to DATETIME(fsp)
    result = re.sub(r'(DATETIME)\((\d+)\)(.+?\bCURRENT_TIMESTAMP)\b(?!\()', r'\1(\2)\3(\2)', result)
    
    # Clean up extra spaces
    result = re.sub(r'\s+', ' ', result).strip()
    
    # Backtick column name
    result = f'  `{col_name_raw}` {result}'
    
    return result


def convert_data_line(line):
    """Convert data line: fix bool, wrap bigint IDs, fix schema."""
    # Replace "public"."tablename" → `tablename`
    line = re.sub(r'"public"\."([^"]+)"', r'`\1`', line)
    
    # Replace boolean literals as standalone values
    line = re.sub(r"(?<=[,\s(])'t'(?=\s*[,)])", '1', line)
    line = re.sub(r"(?<=[,\s(])'f'(?=\s*[,)])", '0', line)
    
    # Strip timezone offsets from datetime values: '2026-01-01 12:00:00.123+08' → '2026-01-01 12:00:00.123'
    line = re.sub(r"'(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}(?:\.\d+)?)[+-]\d{2}(?::\d{2})?'", r"'\1'", line)
    
    # Wrap large int IDs (15-20 digits) in quotes for VARCHAR columns
    # Only match standalone IDs (preceded by '(' or space), skip comma-separated field internals
    line = re.sub(r"(?<=[( ])(\d{15,20})(?=\s*[,)])", r"'\1'", line)
    
    # Sanitize JSON values: parse and re-serialize to strip PG-specific encoding quirks
    def _clean_json(m):
        raw = m.group(1).replace("''", "'")
        try:
            obj = _json.loads(raw)
            clean = _json.dumps(obj, ensure_ascii=False, separators=(',', ':'))
            clean = _json.dumps(obj, ensure_ascii=False).replace('\x00', '')
            return "'" + clean.replace("'", "''") + "'"
        except Exception:
            return m.group(0)
    line = re.sub(r"'([\[{][^']*?[\]}])'", _clean_json, line)
    
    return line


def convert_index_line(line):
    """Convert CREATE INDEX from PG to MySQL format."""
    # Replace "public"."tablename" → `tablename`
    line = re.sub(r'"public"\."([^"]+)"', r'`\1`', line)
    # Replace remaining double-quoted identifiers
    line = re.sub(r'"([^"]+)"', r'`\1`', line)
    # Remove USING btree
    line = re.sub(r'\s*USING\s+btree\s*', ' ', line)
    # Remove pg_catalog.xxx_ops patterns
    line = re.sub(r'`pg_catalog`\.`[^`]+`', '', line)
    line = re.sub(r'"pg_catalog"\."[^"]+"', '', line)
    # Remove ASC/DESC NULLS FIRST/LAST
    line = re.sub(r'\s+(ASC|DESC)\s+NULLS\s+(FIRST|LAST)', '', line, flags=re.IGNORECASE)
    # Remove COLLATE clauses in index
    line = re.sub(r'\s*COLLATE\s+`[^`]*`(?:\.`[^`]*`)?', '', line)
    # Remove leftover COLLATE (when collation name already stripped)
    line = re.sub(r'\s*COLLATE\s*(?=[,)])', '', line)
    # Remove ::type casts from index expressions (same as column conversion)
    line = re.sub(r'::character\s+varying', '', line)
    line = re.sub(r'::varchar\(\d+\)', '', line)
    line = re.sub(r'::text\b', '', line)
    line = re.sub(r'::int[248]\b', '', line)
    line = re.sub(r'::bool\b', '', line)
    line = re.sub(r'::numeric\(\d+,\d+\)', '', line)
    # Simplify PG function expressions in indexes to plain column references
    line = re.sub(r'COALESCE\s*\(\s*`?(\w+)`?\s*,\s*\'[^\']*\'\)', r'`\1`', line, flags=re.IGNORECASE)
    line = re.sub(r'(?:LOWER|UPPER)\s*\(\s*`?(\w+)`?\s*\)', r'`\1`', line, flags=re.IGNORECASE)
    # Remove WHERE clause (PG partial indexes not supported in MySQL)
    m = re.search(r'\s+WHERE\s+(.+?);?\s*$', line)
    where_clause = ''
    if m:
        where_clause = m.group(1).strip()
        line = re.sub(r'\s+WHERE\s+.+?;?\s*$', '', line)
    # Clean up extra spaces
    line = re.sub(r'\s+', ' ', line).strip()
    # Fix parenthesis spacing
    line = re.sub(r'\(\s+', '(', line)
    line = re.sub(r'\s+\)', ')', line)
    line = re.sub(r'\s*,\s*', ', ', line)
    # Remove empty ()
    line = re.sub(r'\(\s*\)', '', line)
    if not line.endswith(';'):
        line += ';'
    if where_clause:
        line += f' -- WHERE {where_clause}'
    return line


def convert_alter_table_line(line):
    """Convert ALTER TABLE ... ADD CONSTRAINT from PG to MySQL format."""
    # Replace "public"."tablename" → `tablename`
    line = re.sub(r'"public"\."([^"]+)"', r'`\1`', line)
    # Replace remaining double-quoted identifiers with backticks
    line = re.sub(r'"([^"]+)"', r'`\1`', line)
    return line


# Translation map for English terms in table comments
_TABLE_COMMENT_TRANS = {
    # Multi-word phrases first (longest match)
    'episode of care encounter': '诊疗周期就诊',
    'episode of care': '诊疗周期',
    'charge item definition': '收费项目定义',
    'charge item auto gen': '收费项目自动生成',
    'charge item': '收费项目',
    'device definition': '设备定义',
    'encounter diagnosis': '就诊诊断',
    'encounter location': '就诊地点',
    'encounter participant': '就诊参与者',
    'encounter reason': '就诊原因',
    'encounter history': '就诊历史',
    'healthcare service': '医疗服务',
    'patient identifier': '患者标识',
    'patient student': '患者学员',
    'pr assign schedule': 'PR分配排班',
    'pr assign service': 'PR分配服务',
    'pr assign': 'PR分配',
    'specimen definition': '标本定义',
    'allergy intolerance': '过敏不耐受',
    'condition definition': '病情定义',
    'condition template': '病情模板',
    'procedure performer': '操作执行者',
    'activity execution location': '活动执行地点',
    'order template line': '医嘱模板行',
    'order template': '医嘱模板',
    'service device template': '服务设备模板',
    'service location template': '服务地点模板',
    'terminology code mapping': '术语编码映射',
    'trace number record': '追踪编号记录',
    'body structure': '身体结构',
    # Single words
    'account': '账号',
    'encounter': '就诊',
    'invoice': '发票',
    'location': '地点',
    'organization': '机构',
    'patient': '患者',
    'assign': '分配',
    'definition': '定义',
    'diagnosis': '诊断',
    'participant': '参与者',
    'specimen': '标本',
    'condition': '病情',
    'procedure': '手术/操作',
    'frequency': '频次',
    'supplier': '供应商',
    'slot': '时段',
    'weekday': '星期',
    'catalog upload record': '目录上传记录',
    'clear apply': '清算申请',
    'encounter platform map': '就诊平台映射',
    'inpatient fee upload': '住院费用上传',
    'inpatient pre settle': '住院预结算',
    'inv upload trace': '发票上传追踪',
    'inv upload': '发票上传',
    'national catalog item': '国家目录项',
    'person cache': '人员缓存',
    'pub sign': '公示签章',
    'recon daily': '日对账',
    'recon detail': '对账明细',
    'settlement record': '结算记录',
}

def translate_table_comment(comment):
    """Translate English terms in table comment to Chinese."""
    if not comment:
        return comment
    result = comment
    # Sort by key length descending so longer phrases match first
    for en, zh in sorted(_TABLE_COMMENT_TRANS.items(), key=lambda x: -len(x[0])):
        result = result.replace(en, zh)
    return result


# Translation map for English terms embedded in Chinese comments
# Multi-word phrases first (longest match wins)
_COMMENT_TRANS = [
    # --- Multi-word phrases ---
    ('episode of care encounter', '诊疗周期就诊'),
    ('episode of care', '诊疗周期'),
    ('charge item definition', '收费项目定义'),
    ('charge item auto gen', '收费项目自动生成'),
    ('charge item', '收费项目'),
    ('device definition', '设备定义'),
    ('encounter diagnosis', '就诊诊断'),
    ('encounter location', '就诊地点'),
    ('encounter participant', '就诊参与者'),
    ('encounter reason', '就诊原因'),
    ('encounter history', '就诊历史'),
    ('healthcare service', '医疗服务'),
    ('patient identifier', '患者标识'),
    ('patient student', '患者学员'),
    ('pr assign schedule', 'PR分配排班'),
    ('pr assign service', 'PR分配服务'),
    ('pr assign', 'PR分配'),
    ('specimen definition', '标本定义'),
    ('allergy intolerance', '过敏不耐受'),
    ('condition definition', '病情定义'),
    ('condition template', '病情模板'),
    ('procedure performer', '操作执行者'),
    ('activity execution location', '活动执行地点'),
    ('order template line', '医嘱模板行'),
    ('order template', '医嘱模板'),
    ('service device template', '服务设备模板'),
    ('service location template', '服务地点模板'),
    ('terminology code mapping', '术语编码映射'),
    ('trace number record', '追踪编号记录'),
    ('body structure', '身体结构'),
    # --- Single words ---
    ('account', '账号'),
    ('encounter', '就诊'),
    ('invoice', '发票'),
    ('location', '地点'),
    ('organization', '机构'),
    ('patient', '患者'),
    ('appointment', '预约'),
    ('Appointment', '预约'),
    ('schedule', '排班'),
    ('Schedule', '排班'),
    ('slot', '时段'),
    ('Slot', '时段'),
    ('contract', '合同'),
    ('definition', '定义'),
    ('default', '默认'),
    ('condition', '病情'),
    ('procedure', '手术/操作'),
    ('frequency', '频次'),
    ('supplier', '供应商'),
    ('weekday', '星期'),
    ('catalog upload record', '目录上传记录'),
    ('clear apply', '清算申请'),
    ('encounter platform map', '就诊平台映射'),
    ('inpatient fee upload', '住院费用上传'),
    ('inpatient pre settle', '住院预结算'),
    ('inv upload trace', '发票上传追踪'),
    ('inv upload', '发票上传'),
    ('national catalog item', '国家目录项'),
    ('person cache', '人员缓存'),
    ('pub sign', '公示签章'),
    ('recon daily', '日对账'),
    ('recon detail', '对账明细'),
    ('settlement record', '结算记录'),
]


def _translate_comment(text):
    """Translate embedded English terms in a comment string to Chinese."""
    if not text:
        return text
    for eng, chn in _COMMENT_TRANS:
        text = text.replace(eng, chn)
    return text


def _flush_dml(dml_lines, table_name, values_list):
    """Flush buffered DML rows into a single INSERT with multiple VALUES tuples."""
    if not values_list:
        return
    dml_lines.append(f"INSERT INTO `{table_name}` VALUES")
    for idx, vals in enumerate(values_list):
        comma = ',' if idx < len(values_list) - 1 else ';'
        dml_lines.append(f"  {vals}{comma}")


def _flush_table(ddl_lines, col_buffer, col_comments, table_comment, column_types=None, table_name=None):
    """Flush buffered columns to DDL with inline COMMENTs."""
    # Collect column type info for index prefix fixing
    if column_types is not None and table_name:
        col_info = {}
        for col_name, type_str, has_comma in col_buffer:
            if col_name.startswith('__'):
                continue
            # type_str format: "  `colname` VARCHAR(64) NOT NULL ..."
            m = re.match(r'\s*`[^`]+`\s+(\w+)(?:\((\d+)\))?', type_str)
            if m:
                col_type = m.group(1).upper()
                col_len = int(m.group(2)) if m.group(2) else None
                col_info[col_name] = (col_type, col_len)
        column_types[table_name] = col_info
    
    for idx, (col_name, type_str, has_comma) in enumerate(col_buffer):
        is_last = (idx == len(col_buffer) - 1)
        line = type_str
        # Add inline COMMENT for regular columns
        if col_name != '__constraint__' and col_name != '__raw__' and col_name in col_comments:
            line += f" COMMENT '{col_comments[col_name]}'"
        # Add trailing comma for all but last item
        if not is_last:
            line += ','
        ddl_lines.append(line)
    # Close with ENGINE, optionally adding table comment
    engine_clause = "ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci"
    if table_comment:
        ddl_lines.append(f") {engine_clause} COMMENT='{table_comment}';")
    else:
        ddl_lines.append(f") {engine_clause};")


def _fix_index_prefixes(ddl_lines, column_types):
    """Post-process index/unique-constraint lines: add VARCHAR column prefixes."""
    PREFIX_LEN = 64
    for idx, line in enumerate(ddl_lines):
        # Handle CREATE INDEX / CREATE UNIQUE INDEX
        if line.startswith('CREATE INDEX ') or line.startswith('CREATE UNIQUE INDEX '):
            m = re.match(r'CREATE (?:UNIQUE )?INDEX `[^`]+` ON `([^`]+)` \((.+)\);?( -- .*)?$', line)
            if not m:
                continue
            table_name = m.group(1)
            cols_str = m.group(2)
            suffix = m.group(3) or ''
            line_prefix = line[:m.start(2)]
            line_suffix = ');' + suffix
        # Handle ALTER TABLE ... ADD CONSTRAINT ... UNIQUE (...)
        elif line.startswith('ALTER TABLE ') and 'UNIQUE (' in line:
            m = re.match(r'ALTER TABLE `([^`]+)` ADD CONSTRAINT `[^`]+` UNIQUE \((.+)\);$', line)
            if not m:
                continue
            table_name = m.group(1)
            cols_str = m.group(2)
            line_prefix = line[:m.start(2)]
            line_suffix = ');'
        else:
            continue
        
        if table_name not in column_types:
            continue
        col_info = column_types[table_name]
        cols = [c.strip() for c in cols_str.split(',')]
        new_cols = []
        for col_ref in cols:
            cm = re.match(r'`([^`]+)`(?:\((\d+)\))?', col_ref)
            if not cm:
                new_cols.append(col_ref)
                continue
            col_name = cm.group(1)
            existing_prefix = int(cm.group(2)) if cm.group(2) else None
            if col_name not in col_info or existing_prefix:
                new_cols.append(col_ref)
                continue
            ctype, clen = col_info[col_name]
            if ctype in ('VARCHAR', 'CHAR') and clen is not None and clen > PREFIX_LEN:
                new_cols.append(f'`{col_name}`({PREFIX_LEN})')
            elif ctype in ('TEXT', 'LONGTEXT', 'MEDIUMTEXT', 'BLOB', 'LONGBLOB', 'MEDIUMBLOB'):
                new_cols.append(f'`{col_name}`({PREFIX_LEN})')
            else:
                new_cols.append(col_ref)
        new_line = line_prefix + ', '.join(new_cols) + line_suffix
        ddl_lines[idx] = new_line


def process_sql(content):
    """Process the entire SQL file."""
    lines = content.split('\n')
    
    ddl_lines = []
    dml_lines = []
    idx_lines = []   # index/constraint statements go to separate file
    
    # DDL header
    ddl_lines.append('-- ============================================')
    ddl_lines.append('-- MySQL 8 DDL - 建表语句')
    ddl_lines.append('-- Converted from PostgreSQL by script')
    ddl_lines.append('-- Primary keys & foreign keys: VARCHAR(64)')
    ddl_lines.append('-- ============================================')
    ddl_lines.append('')
    ddl_lines.append("SET NAMES utf8mb4;")
    ddl_lines.append("SET FOREIGN_KEY_CHECKS = 0;")
    ddl_lines.append('')
    
    # DML header
    dml_lines.append('-- ============================================')
    dml_lines.append('-- MySQL 8 DML - 数据插入语句')
    dml_lines.append('-- Converted from PostgreSQL by script')
    dml_lines.append('-- ============================================')
    dml_lines.append('')
    dml_lines.append("SET NAMES utf8mb4;")
    dml_lines.append("SET FOREIGN_KEY_CHECKS = 0;")
    dml_lines.append('')
    
    state = 'init'
    current_table = None
    in_create_table = False
    col_buffer = []       # buffer column defs: (col_name, type_str, has_comma)
    col_comments = {}     # col_name → comment_text
    table_comment = None  # table-level comment
    dml_buffer = []       # buffer DML VALUES per table: list of value strings
    dml_table = None      # current DML table name
    column_types = {}     # table_name → {col_name: (type, length)} for index prefix fixing
    
    # Tables to skip in DML (logs, audits, flyway history)
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
    skip_dml = False      # whether current DML table should be skipped
    
    i = 0
    
    while i < len(lines):
        line = lines[i]
        
        # Detect state changes — flush pending table before switching
        if 'Table structure for' in line:
            # Flush previous table if any
            if col_buffer:
                _flush_table(ddl_lines, col_buffer, col_comments, table_comment, column_types, current_table)
                col_buffer = []
                col_comments = {}
                table_comment = None
            state = 'ddl'
            in_create_table = False
            m = re.search(r'Table structure for (\S+)', line)
            if m:
                current_table = m.group(1)
                ddl_lines.append(f'-- ----------------------------')
                ddl_lines.append(f'-- Table structure for {current_table}')
                ddl_lines.append(f'-- ----------------------------')
            i += 1
            continue
            
        if 'Records of' in line:
            # Flush previous table if any (COMMENT lines come after ); but before Records of)
            if col_buffer:
                _flush_table(ddl_lines, col_buffer, col_comments, table_comment, column_types, current_table)
                col_buffer = []
                col_comments = {}
                table_comment = None
            # Flush previous DML table buffer
            if dml_buffer:
                _flush_dml(dml_lines, dml_table, dml_buffer)
                dml_buffer = []
            state = 'dml'
            in_create_table = False
            m = re.search(r'Records of (\S+)', line)
            if m:
                current_table = m.group(1)
                dml_table = current_table
                skip_dml = dml_table in SKIP_DML_TABLES
                if not skip_dml:
                    dml_lines.append(f'-- ----------------------------')
                    dml_lines.append(f'-- Records of {current_table}')
                    dml_lines.append(f'-- ----------------------------')
            i += 1
            continue
        
        # Detect index/constraint section headers
        if 'Indexes structure for' in line or 'Primary Key structure for' in line or 'Foreign Keys structure for' in line:
            # Flush any remaining table
            if col_buffer:
                _flush_table(ddl_lines, col_buffer, col_comments, table_comment, column_types, current_table)
                col_buffer = []
                col_comments = {}
                table_comment = None
            state = 'indexes'
            in_create_table = False
            idx_lines.append(f'-- {line.strip().lstrip("-").strip()}')
            i += 1
            continue
        
        if state == 'ddl':
            # Handle DROP TABLE - skip (not needed for target DDL)
            if line.strip().startswith('DROP TABLE IF EXISTS'):
                pass
            
            # Handle CREATE TABLE
            elif line.strip().startswith('CREATE TABLE'):
                m = re.search(r'"public"\."([^"]+)"', line)
                if m:
                    ddl_lines.append(f'CREATE TABLE `{m.group(1)}` (')
                else:
                    ddl_lines.append(re.sub(r'"([^"]*)"', r'`\1`', line))
                in_create_table = True
            
            # Handle column definitions
            elif in_create_table and re.match(r'\s+"[^"]+"\s', line):
                m = re.match(r'\s*"([^"]+)"\s+(.+)', line)
                if m:
                    col_name = m.group(1)
                    type_rest = m.group(2).rstrip().rstrip(',')
                    converted = convert_column_type(col_name, type_rest)
                    has_comma = line.rstrip().endswith(',')
                    col_buffer.append((col_name, converted, has_comma))
                else:
                    converted = re.sub(r'"([^"]*)"', r'`\1`', line)
                    ddl_lines.append(converted)
            
            # Handle PRIMARY KEY / CONSTRAINT lines inside CREATE TABLE
            elif in_create_table and line.strip().startswith('"'):
                col_buffer.append(('__constraint__', re.sub(r'"([^"]*)"', r'`\1`', line), line.rstrip().endswith(',')))
            
            # Handle end of CREATE TABLE: ); or just ;
            elif in_create_table and (line.strip() == ');' or line.strip() == ';'):
                # Don't flush now — COMMENT lines come after );
                # Will flush at next section header (Records of / Table structure for / Indexes)
                in_create_table = False
            
            # Handle ) on its own line (could be end of CREATE TABLE or inside)
            elif in_create_table and line.strip() == ')':
                if i + 1 < len(lines) and lines[i + 1].strip() == ';':
                    # Don't flush now — COMMENT lines come after );
                    in_create_table = False
                    i += 1
                else:
                    col_buffer.append(('__raw__', line, False))
            
            # Handle COMMENT ON COLUMN
            elif 'COMMENT ON COLUMN' in line:
                m = re.match(r"\s*COMMENT ON COLUMN\s+\"public\"\.\"([^\"]+)\"\.\"([^\"]+)\"\s+IS\s+'([^']*)'\s*;?", line)
                if m:
                    col = m.group(2)
                    comment = m.group(3).replace("'", "''")
                    col_comments[col] = _translate_comment(comment)
            
            # Handle COMMENT ON TABLE
            elif 'COMMENT ON TABLE' in line:
                m = re.match(r"\s*COMMENT ON TABLE\s+\"public\"\.\"([^\"]+)\"\s+IS\s+'([^']*)'\s*;?", line)
                if m:
                    table_comment = _translate_comment(m.group(2).replace("'", "''"))
            
            # Skip separator lines
            elif line.strip().startswith('-- ---'):
                pass
            
            # Skip empty lines inside tables
            elif line.strip() == '':
                pass
        
        elif state == 'dml':
            if skip_dml:
                pass  # skip log/audit table data
            elif line.strip().startswith('INSERT INTO'):
                # Handle multi-line VALUES (e.g. JSON content spanning lines)
                full_statement = line
                stripped = line.strip()
                if not stripped.endswith(');'):
                    while i + 1 < len(lines) and lines[i + 1].strip() and not lines[i + 1].strip().startswith('-- ---'):
                        next_line = lines[i + 1].strip()
                        if next_line.startswith('INSERT INTO') or next_line.startswith('Records of') or next_line.startswith('Table structure for'):
                            break
                        i += 1
                        full_statement += ' ' + next_line
                        if next_line.endswith(');'):
                            break
                converted = convert_data_line(full_statement)
                m = re.match(r"INSERT INTO\s+`[^`]+`\s+VALUES\s+(.+?);?\s*$", converted)
                if m:
                    dml_buffer.append(m.group(1))
            elif line.strip() and not line.strip().startswith('-- ---'):
                converted = convert_data_line(line)
                m = re.match(r"INSERT INTO\s+`[^`]+`\s+VALUES\s+(.+?);?\s*$", converted)
                if m:
                    dml_buffer.append(m.group(1))
        
        elif state == 'indexes':
            # Handle CREATE INDEX
            if line.strip().upper().startswith('CREATE INDEX') or line.strip().upper().startswith('CREATE UNIQUE INDEX'):
                # Multi-line index - collect full statement
                full_statement = line
                while i + 1 < len(lines) and not lines[i + 1].strip().startswith('-- ---') and lines[i + 1].strip() != '' and not lines[i + 1].strip().startswith('CREATE') and not lines[i + 1].strip().startswith('ALTER'):
                    i += 1
                    full_statement += ' ' + lines[i].strip()
                # Remove ; at end if present (handled by convert function)
                converted = convert_index_line(full_statement.strip())
                idx_lines.append(converted)
            # Handle ALTER TABLE
            elif line.strip().upper().startswith('ALTER TABLE'):
                converted = convert_alter_table_line(line)
                idx_lines.append(converted)
            # Skip empty lines and separators
            elif line.strip() == '' or line.strip().startswith('-- ---'):
                pass
        
        elif state == 'init':
            # Skip header comments except block comments
            if line.strip().startswith('/*') or line.strip().startswith('*/'):
                ddl_lines.append(line)
        
        i += 1
    
    # Flush any remaining table at end of file
    if col_buffer:
        _flush_table(ddl_lines, col_buffer, col_comments, table_comment, column_types, current_table)
    
    # Flush any remaining DML buffer
    if dml_buffer:
        _flush_dml(dml_lines, dml_table, dml_buffer)
    
    # Remove empty table sections (Records of header with no INSERT)
    i = 0
    while i < len(dml_lines):
        if (dml_lines[i].startswith('-- ----------------------------') and
            i + 2 < len(dml_lines) and 'Records of' in dml_lines[i + 1] and
            dml_lines[i + 2].startswith('-- ----------------------------')):
            # Check if any INSERT follows before next section or end
            j = i + 3
            has_insert = False
            while j < len(dml_lines):
                if dml_lines[j].startswith('INSERT INTO'):
                    has_insert = True
                    break
                if dml_lines[j].startswith('-- ----------------------------'):
                    break
                j += 1
            if not has_insert:
                # Remove the 3-line empty header
                del dml_lines[i:i + 3]
                continue
        i += 1
    
    # Footer
    ddl_lines.append('')
    ddl_lines.append('SET FOREIGN_KEY_CHECKS = 1;')
    
    dml_lines.append('')
    dml_lines.append('SET FOREIGN_KEY_CHECKS = 1;')
    
    _fix_index_prefixes(idx_lines, column_types)
    
    # Prepend index file header
    idx_header = []
    idx_header.append('-- ============================================')
    idx_header.append('-- MySQL 8 Indexes & Constraints')
    idx_header.append('-- Converted from PostgreSQL by script')
    idx_header.append('-- ============================================')
    idx_header.append('')
    idx_header.append("SET NAMES utf8mb4;")
    idx_header.append("SET FOREIGN_KEY_CHECKS = 0;")
    idx_header.append('')
    idx_lines = idx_header + idx_lines
    idx_lines.append('')
    idx_lines.append('SET FOREIGN_KEY_CHECKS = 1;')
    
    return '\n'.join(ddl_lines), '\n'.join(dml_lines), '\n'.join(idx_lines)


def main():
    print(f"Input file: {_input_filename}")
    print("Reading input file...")
    content = read_file(INPUT_FILE)
    print(f"Read {len(content)} characters")
    
    print("Converting...")
    ddl, dml, idx = process_sql(content)
    
    print(f"Writing DDL to {DDL_FILE}...")
    write_file(DDL_FILE, ddl)
    print(f"DDL: {len(ddl)} chars, {ddl.count(chr(10))} lines")
    
    print(f"Writing DML to {DML_FILE}...")
    
    # Sanitize JSON values: MySQL JSON is stricter than PG JSONB.
    # Re-serialize JSON strings to strip \u0000 and other problematic chars.
    import json as _json
    def _fix_json(m):
        raw = m.group(1).replace("''", "'")  # PG SQL -> actual string
        try:
            obj = _json.loads(raw)
            fixed = _json.dumps(obj, ensure_ascii=False, separators=(',', ':'))
            return "'" + fixed.replace("'", "''") + "'"
        except Exception:
            return m.group(0)
    dml = re.sub(r"'([\[{][^']*?[\]}])'", _fix_json, dml)
    
    write_file(DML_FILE, dml)
    print(f"DML: {len(dml)} chars, {dml.count(chr(10))} lines")
    
    print(f"Writing indexes to {IDX_FILE}...")
    write_file(IDX_FILE, idx)
    print(f"IDX: {len(idx)} chars, {idx.count(chr(10))} lines")
    
    print("Done!")
    
    # Also write a verification log
    LOG_FILE = os.path.join(_PROJ, f'{_prefix}_convert_log.txt')
    with open(LOG_FILE, 'w', encoding='utf-8') as lf:
        # Extract all table comments
        tcs = re.findall(r"COMMENT='([^']+)'", ddl)
        lf.write(f"Total table comments: {len(tcs)}\n")
        for tc in tcs:
            lf.write(f"  TABLE: {tc}\n")
        # Extract first few column comments with English
        ccs = re.findall(r"COMMENT '([^']+)'", ddl)
        eng_ccs = [c for c in ccs if re.search(r'[a-zA-Z]', c)]
        lf.write(f"\nColumn comments with English: {len(eng_ccs)}\n")
        for cc in eng_ccs[:30]:
            lf.write(f"  COL: {cc}\n")


if __name__ == '__main__':
    main()
