#ifndef __GET_CATEGORY_DETAILS_SERVICE__
#define __GET_CATEGORY_DETAILS_SERVICE__

#include "domain/do/settle/GetCategoryDetailsDO.h"
#include "domain/dto/settle/GetCategoryDetailsDTO.h"
#include "domain/query/settle/GetCategoryDetailsQuery.h"
#include "../../dao/settle/GetCategoryDetailsDAO.h"

#include <oatpp/core/data/mapping/type/Vector.hpp>
#include <list>

class GetCategoryDetailsService
{
public:
    
    oatpp::Vector<GetCategoryDetailsDTO::Wrapper> listAll(const GetCategoryDetailsQuery::Wrapper& query)
    {
        GetCategoryDetailsDAO dao;

        
        std::list<GetCategoryDetailsDO> doList = dao.selectWithPage(query);

        // ´´½¨ Oat++ Vector ÈÝÆ÷
        auto dtoList = oatpp::Vector<GetCategoryDetailsDTO::Wrapper>::createShared();

        for (const auto& sub : doList)
        {
            auto dto = GetCategoryDetailsDTO::createShared();

           
            dto->sort_num = (v_uint32)sub.getSort_num();

          
            dto->charge_type_name = sub.getCharge_type_name();

          
            dto->type_total_fee = (v_float64)sub.getType_total_fee();

            
            dtoList->push_back(dto);
        }

        return dtoList;
    }
};

#endif // __GET_CATEGORY_DETAILS_SERVICE__

