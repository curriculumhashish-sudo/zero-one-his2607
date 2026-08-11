#pragma once
#ifndef RECEPTIONPATIENTSERVICE_H_
#define RECEPTIONPATIENTSERVICE_H_

#include "domain/dto/cycleCtrl1/ReceptionPatientDTO.h"
#include "domain/query/cycleCtrl1/ReceptionPatientQuery.h"
#include "dao/cycleCtrl1/ReceptionPatientDAO.h"
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "EnumDictUtil.h"


class ReceptionPatientService
{
public:
	//返回在诊患者
	RecePatientDTO::Wrapper getRecePatient(const RecePatientQuery::Wrapper& query, 
		const string& tenantId, const string& practitionerId);

	// 执行接诊（arrived -> in-progress），返回完整患者上下文
	RecePatientDTO::Wrapper acceptPatientByEncounterId(const std::string& encounterId,
		const std::string& tenantId, const std::string& practitionerId);

	static int calcAge(const std::string& birthDate);

	ReceptionPatientService()
		: m_mongoClient(make_shared<MongoClient>("mongodb://994xyQ10gK1h:2rNNkUEG8C8syEOmJBK66WUnh24g4BKS@47.113.229.85:41242/hisDb"))
	{

	}

private:
	ReceptionPatientDAO m_dao;
	std::shared_ptr<MongoClient> m_mongoClient;

};




#endif // !RECEPTIONPATIENTSERVICE_H_
