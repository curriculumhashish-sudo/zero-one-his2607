
#ifndef _NURSE_SERVICE2_
#define _NURSE_SERVICE2_

#include "domain/dto/nurse2/NurseCancelDTO2.h"
#include "domain/dto/nurse2/NurseOperateDTO2.h"

/**
 * Nurse2 服务实现
 */
class NurseService2
{
public:

	//执行 患者姓名 状态 医嘱内容 执行次数 执行人 是否执行
	bool NurseOperate2(const NurseOperateDTO2::Wrapper& dto, const std::string& username);

	
	//取消执行
	//患者姓名 状态 医嘱内容 执行次数 执行人 是否执行
	bool NurseCancel2(const NurseCancelDTO2::Wrapper& dto, const std::string& username);
};

#endif // !_NURSE_SERVICE2_
