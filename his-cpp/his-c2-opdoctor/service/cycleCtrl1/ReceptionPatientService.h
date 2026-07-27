#pragma once
#ifndef RECEPTIONPATIENTSERVICE_H_
#define RECEPTIONPATIENTSERVICE_H_

#include "domain/dto/cycleCtrl1/receptionPatientDTO.h"
#include "domain/query/cycleCtrl1/receptionPatientQuery.h"
#include "dao/cycleCtrl1/ReceptionPatientDAO.h"


class ReceptionPatientService
{
public:
	//∑µªÿ‘⁄’Ôªº’ﬂ
	RecePatientDTO::Wrapper getRecePatient(const RecePatientQuery::Wrapper& query, 
		const string& tenantId, const string& practitionerId);
private:
	ReceptionPatientDAO m_dao;

};




#endif // !RECEPTIONPATIENTSERVICE_H_
