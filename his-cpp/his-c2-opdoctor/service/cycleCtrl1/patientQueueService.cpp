#include "stdafx.h"

#include "patientQueueService.h"
#include <iostream> // for logging in catch

oatpp::Vector<PatientQueueDTO::Wrapper> 
	PatientQueueService::getPatientQueueList(const PatientQueueQuery::Wrapper& query)
{
	//调用dao（增删改查实现逻辑）,查询数据库拿到do（数据库字段）
	std::list<PtrBkgWaitlistEntryDO> entryList = dao.getPatientQueueList(query);

	//vector容器，存放最终dto
	auto resultVec = oatpp::Vector<PatientQueueDTO::Wrapper>::createShared();

	try
	{
		//遍历出每一条do,想让do字段转到dto
		for (auto& entry : entryList)
		{
			// 新建一条dto（用户想要拿到的数据）
			auto dto = PatientQueueDTO::createShared();

			// 1、取出患者的基本信息（可能为空）
			auto patientDO = entry->getPatientDO();
			if (patientDO)
			{
				dto->patientName = patientDO->getName();
				dto->gender = patientDO->getGenderEnum();
				dto->visitNum = patientDO->getBusNo();

				// 还要患者的年龄，guard 空字符串
				std::string bd = patientDO->getBirthDate();
				if (!bd.empty())
					dto->age = calcAge(bd);
				else
					dto->age = 0;
			}
			else
			{
				// 没有联表到患者信息，填默认值以保证序列化安全
				dto->patientName = "";
				dto->gender = "";
				dto->visitNum = "";
				dto->age = 0;
			}

			// 2、取出挂号类型（可能为空）
			auto serviceDO = entry->getHealthcareServiceDO();
			if (serviceDO)
			{
				dto->payType = serviceDO->getName();
			}
			else
			{
				dto->payType = "";
			}

			// 3.诊断/挂号时间（可能为空）
			dto->visitTime = entry->getCreatedAt().empty() ? "" : entry->getCreatedAt();

			// 4、状态（safe）
			std::string statusCode = entry->getStatus();
			if (statusCode == "WAITING")
			{
				dto->visitStatus = "待诊";
			}
			else if (statusCode == "TREAT")
			{
				dto->visitStatus = "在诊";
			}
			else if (statusCode == "FINISH")
			{
				dto->visitStatus = "完诊";
			}
			else
			{
				dto->visitStatus = "";
			}

			// 组装完成的dto放入数组
			resultVec->push_back(dto);
		}
	}
	catch (const std::exception& ex)
	{
		// 打印到控制台 / 日志，方便排查真实异常来源
		std::cerr << "PatientQueueService::getPatientQueueList exception: " << ex.what() << std::endl;
		// 如果需要，可以重新抛出以让上层统一错误处理；这里返回当前已构建的部分结果（或空列表）
	}

	return resultVec;
}

inline int PatientQueueService::calcAge(const std::string& birthDate)
{
	// TODO: 实现出生日期到年龄的计算，当前返回0以保证安全
	(void)birthDate;
	return 0;
}
