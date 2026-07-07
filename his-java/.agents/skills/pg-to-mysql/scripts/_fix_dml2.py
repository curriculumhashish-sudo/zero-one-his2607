import re
import os

_PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
DML_FILE = os.path.join(_PROJ, 'public_mysql_dml.sql')

dml = open(DML_FILE, 'r', encoding='utf-8').read()

# Pattern 1: ...,'329891962016501760' (stray quote before trailing ID)
# Match: digit,quote,15-20digits,quote → remove opening quote
# '...,729094784','329891962016501760' → '...,729094784,329891962016501760'
dml = re.sub(r"(\d),'(\d{15,20}')", r"\1,\2", dml)

# Pattern 2: 'id1','id2' (both individually quoted) → 'id1,id2'
dml = re.sub(r"'(\d{15,20})','(\d{15,20})'", r"'\1,\2'", dml)

# Pattern 3: 'id1,'id2' → 'id1,id2'
dml = re.sub(r"'(\d{15,20}),'(\d{15,20})'", r"'\1,\2'", dml)

open(DML_FILE, 'w', encoding='utf-8').write(dml)
print('Fixed DML')
