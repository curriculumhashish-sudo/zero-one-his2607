#ifndef _chinapres3CONTROLLER_H_
#define _chinapres3CONTROLLER_H_

// ����VO��DTO���ͷ�ļ�
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/chinapres3/chinapres3IssueDTO.h"
#include "../../domain/vo/chinapres3/chinapres3IssueResultVO.h"
// ---- revoke-related includes (temporarily disabled for debugging) ----
//#include "../../domain/dto/chinapres3/chinapres3RevokeDTO.h"
//#include "../../domain/vo/chinapres3/chinapres3RevokeResultVO.h"
#include "../../service/chinapres3/chinapres3Service.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// �ӿڷ����ǩ������Swagger�ӿ��ĵ�չʾ
#define API_TAG ZH_WORDS_GETTER("chinapres3.tags.tag")

/**
 * ҽ����������������
 *
 * �ṩ�����������������ض���API�ӿ�
 * ����ǰ�˴�������������
 * ���� chinapres3Service ִ��ҵ���߼�
 * ͳһ��VO��ʽ���ؽӿ���Ӧ����
 */
class chinapres3Controller : public oatpp::web::server::api::ApiController
{
	// �����ӿڼ�Ȩ��ȫ������У��JWT��¼ƾ֤��Ϣ
	API_ACCESS_DECLARE(chinapres3Controller);

public:
	// ���忪�������ӿ��ĵ���Ϣ����Ȩ��ʶ�������塢�������飨Swaggerչʾ��
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chinapres3.issue.summary"),
		issuechinapres3,                              // �ӿ��ڲ���ʶ����
		chinapres3IssueResultVO::Wrapper,              // �ӿڷ�������VO����
		API_TAG);

	// �ӿ�·�ɶ��壺POST /chinapres3/issue ���������ӿ�
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,                                   // ����ʽ POST
		"/chinapres3/issue",                          // �ӿڷ���·�ɵ�ַ
		issuechinapres3,                              // �ӿڱ�ʶ�����Ϸ��ĵ������Ӧ
		BODY_DTO(chinapres3IssueDTO::Wrapper, dto),   // ����ǰ��JSON�����壬��Ϊdto����
		execIssuechinapres3(dto, authObject->getPayload()));

	// ---- revoke-related endpoint (temporarily disabled for debugging) ----
	/*
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chinapres3.recall.summary"),
		revokechinapres3,
		chinapres3RevokeResultVO::Wrapper,
		API_TAG);

	// Route: POST /chinapres3/revoke
	API_HAND

private:
	// ע��ҽ����������ҵ����񣬳��ؿ��������غ���ҵ���߼�
	chinapres3Service service;

	/**
	 * ���������ӿ�ִ�к���
	 *
	 * ����ǰ���ύ�Ŀ�������DTO����¼�û�JWT�غ���Ϣ
	 * ����service.issue()��ɴ������������´���״̬�����ɷ�ҩ���շѼ�¼
	 *
	 * @param dto �������������������������ID������ID��ǩ��Ŀ�괦���б�
	 * @param payload JWT�غɣ��洢��ǰ������¼���˺���Ϣ
	 * @return chinapres3IssueResultVO �����������������ִ��״̬����ʾ�İ���ͳ������
	 */
	chinapres3IssueResultVO::Wrapper execIssuechinapres3(
		const chinapres3IssueDTO::Wrapper& dto,
		const PayloadDTO& payload);

	// ---- ��������ִ�к��� ----
	/**
	 * �������ؽӿ�ִ�к���
	 *
	 * ת��������service.revoke() ִ�д�������ȫ�����߼�
	 *
	 * @param dto �������������Я�������ش�����������
	 * @param payload JWT�غɣ���ŵ�ǰ�����û���Ϣ
	 * @return Revoke���VO�������ɹ�����ͳ��������ʧ�ܴ�����ϸ
	 */
	chinapres3RevokeResultVO::Wrapper execRevokechinapres3(
		const chinapres3RevokeDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_chinapres3CONTROLLER_H_