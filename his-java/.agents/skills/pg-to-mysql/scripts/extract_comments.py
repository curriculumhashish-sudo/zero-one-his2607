import re
import os

_PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
INPUT_FILE = os.path.join(_PROJ, 'public.sql')

content = open(INPUT_FILE, 'r', encoding='utf-8').read()
ms = re.findall(r"COMMENT ON TABLE\s+\"public\"\.\"([^\"]+)\"\s+IS\s+'(.*?)'", content)
for t, cm in ms:
    print(f'{t}: {cm}')
