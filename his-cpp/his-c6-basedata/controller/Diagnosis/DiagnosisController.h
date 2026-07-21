#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/20 11:00:34
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
#ifndef _DIAGNOSISCONTROLLER_H_
#define _DIAGNOSISCONTROLLER_H_

//#include "domain/vo/BaseJsonVO.h"
//#include "GlobalInclude.h"
#include "domain/dto/Diagnosis/DiagnoseDTO.h"


/*
//// �������ײ�ѯQuery
//#include "../../domain/query/Diagnosis/DiagnosisSelectQuery.h"
//#include "domain/query/Diagnosis/DiagnosisPageQuery.h"
//// �������׷�ҳ/���鷵��VO
//#include "domain/vo/diagnosis/DiagnosisSelectJsonVO.h"
//#include "domain/vo/diagnosis/DiagnosisPageJsonVO.h"
//#include "domain/vo/diagnosis/DiagnosisDetailJsonVO.h"
这是你之前的头文件，暂时注释了
*/
// 查询Query
#include "domain/query/Diagnosis/DiagnosisSelectQuery.h"
#include "domain/query/Diagnosis/DiagnosisPageQuery.h"

// 业务DTO
#include "domain/dto/Diagnosis/DiagnosisAddDTO.h"
#include "domain/dto/Diagnosis/DiagnosisPageDTO.h"
#include "domain/dto/Diagnosis/DiagnosisDetailDTO.h"
#include "domain/dto/Diagnosis/DiagnosisDeleteDTO.h"

// 返回JsonVO
#include "domain/vo/Diagnosis/DiagnosisSelectJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisPageJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisDetailJsonVO.h"


using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnose.tags.t1")

class DiagnosisController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisController);
/*

public:

    1. ����ѡ����Ϸ�ҳ�ӿ� /diagnosis/select-page
	 ����ҽ��վ��סԺ�Ǽǵ���ɸѡ����չʾ�������ݣ�����ҽ/��ҽ����ɸѡ��
	// 
	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.select.summary"), listSelectDiagnosis, DiagnosisSelectJsonVO::Wrapper, API_TAG);
	//API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/select-page", listSelectDiagnosis, DiagnosisSelectQuery, executeListSelectDiagnosis(query));

	//
	//  2. ��Ϲ�����̨��ҳ�ӿ� /diagnosis/page
	//  ��̨���б���֧��Ŀ¼�����롢���ơ�״̬ɸѡ��֧��������ͣ
	//
	//API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.page.summary"), listDiagnosis, DiagnosisPageJsonVO::Wrapper, API_TAG);
	//API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/page", listDiagnosis, DiagnosisPageQuery, executeListDiagnosis(query));

	//
	// 3. ��ȡ��ϵ�������ӿ� /diagnosis/detail
	//  �������鵯�����༭��������
	// 
	//API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.detail.summary"), getDiagnosisDetail, DiagnosisDetailJsonVO::Wrapper, API_TAG,
	//	API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("diagnosis.field.id"), "", true);
	//);
	//API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/diagnosis/detail", getDiagnosisDetail, QUERY(String, id), executeGetDiagnosisDetail(id));
	// 1. 诊断弹窗选择分页
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.select.summary"), listSelectDiagnosis, DiagnosisSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/select-page", listSelectDiagnosis, DiagnosisSelectQuery, executeListSelectDiagnosis(query));

	// 2. 诊断管理后台分页
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.page.summary"), listDiagnosis, DiagnosisPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/page", listDiagnosis, DiagnosisPageQuery, executeListDiagnosis(query));

	// 3. 诊断单条详情
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.detail.summary"), getDiagnosisDetail, DiagnosisDetailJsonVO::Wrapper, API_TAG,
	   API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("diagnosis.field.id"), "", true);
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/diagnosis/detail", getDiagnosisDetail, QUERY(String, id), executeGetDiagnosisDetail(id));


	// 4. 新增诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.post.summary"), addDiagnosis, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnosis", addDiagnosis, BODY_DTO(DiagnosisAddDTO::Wrapper, dto), execAddDiagnosis(dto, authObject->getPayload()));

	// 5. 修改诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.put.summary"), modifyDiagnosis, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnosis", modifyDiagnosis, BODY_DTO(DiagnosisPageDTO::Wrapper, dto), execModifyDiagnosis(dto, authObject->getPayload()));

	// 6. 批量删除诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.delete.summary"), removeDiagnosis, ListJsonVO<String>::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnosis", removeDiagnosis, BODY_DTO(List<String>, ids), execRemoveDiagnosis(ids));

private:
	// ����ѡ���ҳҵ��ʵ��
	//DiagnosisSelectJsonVO::Wrapper executeListSelectDiagnosis(const DiagnosisSelectQuery::Wrapper& query);
	//// ������̨��ҳҵ��ʵ��
	//DiagnosisPageJsonVO::Wrapper executeListDiagnosis(const DiagnosisPageQuery::Wrapper& query);
	//// ����ID��ѯ��������ʵ��
	//DiagnosisDetailJsonVO::Wrapper executeGetDiagnosisDetail(const String& id);
	这是你的代码，暂时注释了
	
*/

public:
	// 3.1 定义新增诊断表接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.post.summary"), addDiagnose, StringJsonVO::Wrapper, API_TAG);

// 3.2 定义新增诊断表接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose", addDiagnose, BODY_DTO(DiagnoseAddDTO::Wrapper, dto), execAddDiagnose(dto, authObject->getPayload()));

//// 3.1 定义新增诊断目录接口描述
//API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.post.summary_category"), addDiagcategory, StringJsonVO::Wrapper, API_TAG);
//// 3.2 定义新增诊断目录接口处理
//API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose/category", addDiagcategory, BODY_DTO(DiagcategoryAddDTO::Wrapper, dto), execAddDiagcategory(dto, authObject->getPayload()));
//

	//  定义修改接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.put.summary"), modifyDiagnose, StringJsonVO::Wrapper, API_TAG);
// 定义修改接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnose", modifyDiagnose, BODY_DTO(DiagnoseDTO::Wrapper, dto), execModifyDiagnose(dto, authObject->getPayload()));

//  定义删除接口描述
API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summary"), removeDiagnose, ListJsonVO<String>::Wrapper, API_TAG);
//  定义删除接口处理
API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnose", removeDiagnose, BODY_DTO(List<String>, ids), execRemoveDiagnose(ids));

private:

// 新增诊断表数据
StringJsonVO::Wrapper execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
//  新增诊断目录（分类）数据
//StringJsonVO::Wrapper execAddDiagcategory(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
// 演示修改数据
StringJsonVO::Wrapper execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload);
// 演示删除数据
ListJsonVO<String>::Wrapper execRemoveDiagnose(const List<String>& ids);

	DiagnosisSelectJsonVO::Wrapper executeListSelectDiagnosis(const DiagnosisSelectQuery::Wrapper& query);
	DiagnosisPageJsonVO::Wrapper executeListDiagnosis(const DiagnosisPageQuery::Wrapper& query);
	DiagnosisDetailJsonVO::Wrapper executeGetDiagnosisDetail(const String& id);
	/*StringJsonVO::Wrapper execAddDiagnosis(const DiagnosisAddDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execModifyDiagnosis(const DiagnosisPageDTO::Wrapper& dto, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemoveDiagnosis(const List<String>& ids);*/
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _DIAGNOSISCONTROLLER_H_