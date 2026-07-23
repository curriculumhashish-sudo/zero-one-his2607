#pragma once
#ifndef _WARD_DAO_H_
#define _WARD_DAO_H_

#include<iostream>
#include"../../lib-mysql/include/BaseDAO.h"
#include"../../domain/do/ward/WardDO.h"

class WardDAO : public BaseDAO
{
public:
    //std::list<WardDAO> selectNameList(
    //    const std::string& organizationId);
    std::list<WardDO> selectNameList(
        const std::string& organizationId);
};
#endif


