#ifndef _SAMPLE_SERVICE_
#define _SAMPLE_SERVICE_
#include "domain/vo/sample/SampleVO.h"
#include "domain/query/sample/SampleQuery.h"
#include "domain/dto/sample/SampleDTO.h"

/**
 * 示例服务实现，演示基础的示例服务实现
 */
class DiagnoseService
{
public:
	
	// 保存数据
	std::string saveData(const SampleAddDTO::Wrapper& dto);
    std::string savecategory(const SampleAddDTO::Wrapper& dto);
	// 批量保存数据
	bool saveDataBatch(const oatpp::List<SampleAddDTO::Wrapper>& datas);
	// 修改数据
	bool updateData(const SampleDTO::Wrapper& dto);
	// 通过ID删除数据
	bool removeData(const oatpp::List<oatpp::String>& ids);
};

#endif // !_SAMPLE_SERVICE_

