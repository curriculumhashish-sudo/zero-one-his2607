#pragma
#ifndef _WARDSERVICE_H_
#define _WARDSERVICE_H_
#include<string>
#include "../../domain/vo/ward/WardNameVO.h"
class WardService
{
public:
	oatpp::List<WardNameVO::Wrapper>listNames(
		const std::string& organizationId

	);

};

#endif // !
