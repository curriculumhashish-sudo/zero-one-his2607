#pragma once 
#ifndef _WARDTREENODE_VO_H_
#define _WARDTREENODE_VO_H_
#include<iostream>
#include"../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class WardTreeNodeVO :public oatpp::DTO {
	DTO_INIT(WardTreeNodeVO,DTO);
	DTO_FIELD(String, id);
	DTO_FIELD(String, name);
	DTO_FIELD(String, formEnum);
	DTO_FIELD(String, parentId);
	DTO_FIELD(String, statusEnum);
	DTO_FIELD(String, operationalEnum);
	DTO_FIELD(Int32, displayOrder);
	DTO_FIELD(
		List<WardTreeNodeVO::Wrapper>,
		children
	);


};

#include OATPP_CODEGEN_END(DTO)
#endif