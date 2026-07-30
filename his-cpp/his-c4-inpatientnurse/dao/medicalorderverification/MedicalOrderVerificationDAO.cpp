/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "MedicalOrderVerificationDAO.h"
#include "MedicalOrderVerificationMapper.h"

std::list<MedicalOrderVerificationDetailDTO::Wrapper> MedicalOrderVerificationDAO::selectItems(const MedicalVerificationQuery::Wrapper& q)
{
	std::string sql="SELECT * FROM (SELECT mr.id request_id,'MED_REQUEST' order_type,p.name patient_name,IF(mr.status_enum='VERIFIED','true','false') is_passed,COALESCE(md.name,mr.content_json,mr.remark,'') content,COALESCE(mr.therapy_enum,'') medical_type,mr.status_enum FROM med_medication_request mr JOIN adm_patient p ON p.id=mr.patient_id AND p.is_deleted=0 LEFT JOIN med_medication_definition md ON md.id=mr.medication_id AND md.is_deleted=0 WHERE mr.is_deleted=0 UNION ALL SELECT sr.id,'SERVICE_REQUEST',p.name,IF(sr.status_enum='VERIFIED','true','false'),COALESCE(ad.name,sr.entrust_content,sr.remark,''),COALESCE(sr.category_enum,''),sr.status_enum FROM wkf_service_request sr JOIN adm_patient p ON p.id=sr.patient_id AND p.is_deleted=0 LEFT JOIN wkf_activity_definition ad ON ad.id=sr.activity_id AND ad.is_deleted=0 WHERE sr.is_deleted=0) x WHERE 1=1";
	SqlParams p;
	if(q->name&&!q->name->empty()){sql+=" AND x.patient_name LIKE CONCAT('%',?,'%')";SQLPARAMS_PUSH(p,"s",std::string,q->name.getValue(""));}
	if(q->isPassed&&!q->isPassed->empty()){sql+=q->isPassed.getValue("")=="true"?" AND x.status_enum='VERIFIED'":" AND x.status_enum='ACTIVE'";}
	if(q->medicalType&&!q->medicalType->empty()){sql+=" AND (x.order_type=? OR x.medical_type=?)";SQLPARAMS_PUSH(p,"s",std::string,q->medicalType.getValue(""));SQLPARAMS_PUSH(p,"s",std::string,q->medicalType.getValue(""));}
	if(q->medicalPeriod&&!q->medicalPeriod->empty()){sql+=" AND x.medical_type=?";SQLPARAMS_PUSH(p,"s",std::string,q->medicalPeriod.getValue(""));}
	sql+=" ORDER BY x.patient_name,x.request_id";
	return sqlSession->executeQuery<MedicalOrderVerificationDetailDTO::Wrapper>(sql,MedicalOrderVerificationMapper(),p);
}

int MedicalOrderVerificationDAO::verify(const std::string& type,const std::string& id,const std::string& op)
{
	std::string table=type=="MED_REQUEST"?"med_medication_request":(type=="SERVICE_REQUEST"?"wkf_service_request":""); if(table.empty())return 0;
	SqlParams p; SQLPARAMS_PUSH(p,"s",std::string,op);SQLPARAMS_PUSH(p,"s",std::string,id);
	return sqlSession->executeUpdate("UPDATE "+table+" SET status_enum='VERIFIED',performer_check_id=?,check_time=NOW(),updated_at=NOW(),version=version+1 WHERE id=? AND is_deleted=0 AND status_enum='ACTIVE'",p);
}
int MedicalOrderVerificationDAO::reject(const std::string& type,const std::string& id,const std::string& reason,const std::string& op)
{
	std::string table=type=="MED_REQUEST"?"med_medication_request":(type=="SERVICE_REQUEST"?"wkf_service_request":""); if(table.empty())return 0;
	std::string reasonCol=type=="MED_REQUEST"?"remark":"reason_text"; SqlParams p; SQLPARAMS_PUSH(p,"s",std::string,reason);SQLPARAMS_PUSH(p,"s",std::string,op);SQLPARAMS_PUSH(p,"s",std::string,id);
	return sqlSession->executeUpdate("UPDATE "+table+" SET status_enum='REJECTED',"+reasonCol+"=?,performer_check_id=?,check_time=NOW(),updated_at=NOW(),version=version+1 WHERE id=? AND is_deleted=0 AND status_enum='ACTIVE'",p);
}
