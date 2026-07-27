#include "PatService.h"
#include "../../dao/pat/PatientDAO.h"
#include "../../Macros.h"
#include <ctime>

PageDTO<PatSelectDTO>::Wrapper PatService::querySelectPatPage(const PatSelectPageQuery::Wrapper& query)
{
    auto pageData = PageDTO<PatSelectDTO>::createShared();

    int offset = (query->pageIndex - 1) * query->pageSize;
    int limit = query->pageSize;

    PatientDAO patientDao;
    std::vector<PtrPatientDO> patientList = patientDao.selectPatList(query, offset, limit);
    int totalCount = patientDao.countPatList(query);

    // 直接遍历DO，每循环一次就add一条DTO到分页rows，不需要中间dtoList容器
    for (auto& patientDO : patientList)
    {
        PatSelectDTO dto;
        dto.busNo = patientDO->getBusNo();
        dto.name = patientDO->getName();
        dto.gender = patientDO->getGender();
        dto.birthDate = patientDO->getBirthDate();
        dto.phone = patientDO->getPhone();
        dto.idCard = patientDO->getIdCard();

        // 调用PageDTO自带方法塞入列表，类型100%匹配
        pageData->addData(dto);
    }

    // 分页基础参数赋值
    pageData->pageIndex = query->pageIndex;
    pageData->pageSize = query->pageSize;
    pageData->total = totalCount;
    // 自动计算总页数
    pageData->calcPages();

    return pageData;
}

