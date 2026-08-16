
#ifndef _GET_TOTAL_FEES_SERVICE__
#define _GET_TOTAL_FEES_SERVICE__


#include "domain/do/settle/GetTotalFeesDO.h"
#include "domain/dto/settle/GetTotalFeesDTO.h"
#include "domain/query/settle/GetTotalFeesQuery.h"
#include "../../dao/settle/GetTotalFeesDAO.h"

#include <oatpp/core/data/mapping/type/Vector.hpp>
#include <list>

class GetTotalFeesService
{
public:
    oatpp::Vector<GetTotalFeesDTO::Wrapper> listAll(const GetTotalFeesQuery::Wrapper& query)
    {
        GetTotalFeesDAO dao;
        std::list<GetTotalFeesDO> doList = dao.selectWithPage(query);
        auto dtoList = oatpp::Vector<GetTotalFeesDTO::Wrapper>::createShared();

        for (const auto& sub : doList)
        {
            auto dto = GetTotalFeesDTO::createShared();
            // ÊýÖµÓ³Éä
            dto->total_hospital_fee = (v_float64)sub.getTotal_hospital_fee();
            dto->insurance_pay_fee = (v_float64)sub.getInsurance_pay_fee();
            dto->personal_pay_fee = (v_float64)sub.getPersonal_pay_fee();
            dto->prepay_total_fee = (v_float64)sub.getPrepay_total_fee();
            dto->final_balance_fee = (v_float64)sub.getFinal_balance_fee();
            dto->account_balance = (v_float64)sub.getAccount_balance();

            dtoList->push_back(dto);
        }
        return dtoList;
    }
};


#endif // _GET_TOTAL_FEES_SERVICE_



