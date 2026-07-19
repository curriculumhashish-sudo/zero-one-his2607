# Test translation function
import re

_COMMENT_TRANS = [
    ('episode of care encounter', '诊疗周期就诊'),
    ('episode of care', '诊疗周期'),
    ('charge item definition', '收费项目定义'),
    ('charge item', '收费项目'),
    ('device definition', '设备定义'),
    ('encounter diagnosis', '就诊诊断'),
    ('encounter location', '就诊地点'),
    ('encounter participant', '就诊参与者'),
    ('encounter reason', '就诊原因'),
    ('healthcare service', '医疗服务'),
    ('patient identifier', '患者标识'),
    ('patient student', '患者学员'),
    ('pr assign schedule', 'PR分配排班'),
    ('pr assign service', 'PR分配服务'),
    ('pr assign', 'PR分配'),
    ('specimen definition', '标本定义'),
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
]

def _translate_comment(text):
    if not text:
        return text
    for eng, chn in _COMMENT_TRANS:
        text = text.replace(eng, chn)
    return text

# Test cases
tests = [
    ('管理域 account', '管理域 账号'),
    ('管理域 charge item', '管理域 收费项目'),
    ('管理域 charge item definition', '管理域 收费项目定义'),
    ('管理域 device definition', '管理域 设备定义'),
    ('管理域 encounter', '管理域 就诊'),
    ('管理域 encounter diagnosis', '管理域 就诊诊断'),
    ('管理域 encounter location', '管理域 就诊地点'),
    ('管理域 encounter participant', '管理域 就诊参与者'),
    ('管理域 encounter reason', '管理域 就诊原因'),
    ('管理域 episode of care', '管理域 诊疗周期'),
    ('管理域 episode of care encounter', '管理域 诊疗周期就诊'),
    ('管理域 healthcare service', '管理域 医疗服务'),
    ('管理域 invoice', '管理域 发票'),
    ('管理域 location', '管理域 地点'),
    ('管理域 organization', '管理域 机构'),
    ('管理域 patient', '管理域 患者'),
    ('管理域 patient identifier', '管理域 患者标识'),
    ('管理域 patient student', '管理域 患者学员'),
    ('管理域 pr assign', '管理域 PR分配'),
    ('管理域 pr assign schedule', '管理域 PR分配排班'),
    ('管理域 pr assign service', '管理域 PR分配服务'),
    ('管理域 specimen definition', '管理域 标本定义'),
    ('encounter 标识', '就诊 标识'),
    ('关联 Appointment，不占用 Slot', '关联 预约，不占用 时段'),
    ('default 标志位', '默认 标志位'),
    ('contract 编号', '合同 编号'),
]

all_pass = True
for input_str, expected in tests:
    result = _translate_comment(input_str)
    status = 'PASS' if result == expected else 'FAIL'
    if result != expected:
        all_pass = False
    print(f'{status}: "{input_str}" -> "{result}"  (expected: "{expected}")')

print()
print('ALL PASS!' if all_pass else 'SOME TESTS FAILED!')
