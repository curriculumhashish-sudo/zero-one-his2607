#include"AddRegisterContorller.h"
#include"../../service/register/AddRegisterService.h"
AddRegisterJsonVO::Wrapper AddRegisterContorller::execQueryAddRegitser(const AddRegisterDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = AddRegisterJsonVO::createShared();

	dto->setPayload(&payload);
	auto result = AddRegisterService().Addregister(dto);
	if (result!=nullptr) {
		jvo->success(result);
	}
	else
	{
		jvo->fail(nullptr);
	}
	return jvo;
}