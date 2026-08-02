#include"AddRegisterContorller.h"
#include"../../service/register/AddRegisterService.h"
StringJsonVO::Wrapper AddRegisterContorller::execQueryAddRegitser(const AddRegisterDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto jvo = StringJsonVO::createShared();

	dto->setPayload(&payload);
	std::string id = AddRegisterService().Addregister(dto);
	if (id != "") {
		jvo->success(id);
	}
	else
	{
		jvo->fail("");
	}
	return jvo;
}