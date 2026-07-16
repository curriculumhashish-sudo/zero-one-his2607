import os

_PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
DDL_FILE = os.path.join(_PROJ, 'public_mysql_ddl.sql')

with open(DDL_FILE, 'r', encoding='utf-8') as f:
    lines = f.readlines()
filtered = [l for l in lines if not l.startswith('DROP TABLE IF EXISTS')]
with open(DDL_FILE, 'w', encoding='utf-8') as f:
    f.writelines(filtered)
print(f'OK: {len(lines)} -> {len(filtered)} lines, removed {len(lines) - len(filtered)} DROP TABLE')
