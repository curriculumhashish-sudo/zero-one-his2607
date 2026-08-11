#pragma once
#ifndef _MEDICATIONREQUESTDETAILDAO1_H_
#define _MEDICATIONREQUESTDETAILDAO1_H_

#include "BaseDAO.h"
#include "domain/do/chinapres1/MedicationRequestDetailDO1.h"

class MedicationRequestDetailDAO1 : public BaseDAO
{
public:
    // 根据主表ID查药材明细
    std::list<PtrMedicationRequestDetailDO> selectByRequestId(const string& requestId);
};

#endif // !_MEDICATIONREQUESTDETAILDAO1_H_