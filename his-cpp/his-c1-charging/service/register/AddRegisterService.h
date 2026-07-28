#ifndef __ADD_REGISTER_SERVICE__
#define __ADD_REGISTER_SERVICE__
#include "domain/dto/register/AddRegisterDTO.h"
#include"../../dao/register/AddRegisterDAO.h"
#include "id/UuidFacade.h"

class AddRegisterService
{
public:
	std::string Addregister(const AddRegisterDTO::Wrapper& dto)
	{
		UuidFacade uf;
		dto->id = uf.genUuid();
		AddRegisterDAO dao;
		return dao.InsertRegister(dto) > 0 ? dto->id:"";
	}

};

#endif