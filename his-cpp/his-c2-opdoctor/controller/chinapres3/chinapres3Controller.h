#ifndef _chinapres3CONTROLLER_H_
#define _chinapres3CONTROLLER_H_

// VO��DTO ���ݽṹͷ�ļ�����
#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/chinapres3/chinapres3IssueDTO.h"
#include "../../domain/vo/chinapres3/chinapres3IssueResultVO.h"
// ���ش�����ؽṹ��ͷ�ļ�
#include "../../domain/dto/chinapres3/chinapres3RevokeDTO.h"
#include "../../domain/vo/chinapres3/chinapres3RevokeResultVO.h"
#include "../../service/chinapres3/chinapres3Service.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

// Swagger�ӿڷ����ǩ�����Դ���
#define API_TAG ZH_WORDS_GETTER("chinapres3.tags.tag")

/**
 * ��ҽ����������
 *
 * �ṩ��ҽ����ǩ�������ض���API�ӿ�
 * ���нӿھ�Я��JWT���ݼ�Ȩ
 * �ڲ����� chinapres3Service ����ҵ���߼�
 * ���DTO������VOͳһ��װ�����뷵������
 */
class chinapres3Controller : public oatpp::web::server::api::ApiController
{
	// ����JWTȨ��������
	API_ACCESS_DECLARE(chinapres3Controller);

public:
	// ����ǩ���ӿ�Swagger�ĵ����壨����Ȩ��
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chinapres3.issue.summary"),
		issuechinapres3,                              // �ӿ��ڲ�������ʶ
		chinapres3IssueResultVO::Wrapper,              // �ӿڷ���VO��װ����
		API_TAG);

	// POST /chinapres3/issue ����ǩ��·�ɰ�
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,                                   // ���󷽷� POST
		"/c2-opdoctor/chinapres3/issue",               // �ӿ�·��
		issuechinapres3,                              // �����ĵ������ʶ
		BODY_DTO(chinapres3IssueDTO::Wrapper, dto),   // ����JSON�����岢��dto����
		execIssuechinapres3(dto, authObject->getPayload()));

	// ---- �������ؽӿ��ĵ����� ----
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chinapres3.recall.summary"),
		revokechinapres3,
		chinapres3RevokeResultVO::Wrapper,
		API_TAG);

	// POST /chinapres3/revoke ��������·�ɰ�
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/c2-opdoctor/chinapres3/revoke",
		revokechinapres3,
		BODY_DTO(chinapres3RevokeDTO::Wrapper, dto),
		execRevokechinapres3(dto, authObject->getPayload()));

private:
	// ����ҵ�����ʵ��
	chinapres3Service service;

	/**
	 * ����ǩ��ִ�к���
	 *
	 * ����ǰ�˴����ǩ��DTO��JWT�����غ�
	 * ����service.issue��ɴ���ǩ��ȫҵ������
	 *
	 * @param dto ǩ������DTO����������ID������ID����ǩ�𴦷��б�
	 * @param payload JWT�غɣ��洢��ǰ����ҽ�������ҡ�ҽԺ������Ϣ
	 * @return chinapres3IssueResultVO ǩ�������װ���󣬷��سɹ�/ʧ��״̬���ִ
	 */
	chinapres3IssueResultVO::Wrapper execIssuechinapres3(
		const chinapres3IssueDTO::Wrapper& dto,
		const PayloadDTO& payload);

	// ---- ��������ִ�к��� ----
	/**
	 * ��������ִ�к���
	 *
	 * ����service.revokeִ�д������ء����ݿ�״̬����߼�
	 *
	 * @param dto ��������DTO��������Ҫ�����Ĵ����������
	 * @param payload JWT�����غɣ�����У�����Ȩ��
	 * @return chinapres3RevokeResultVO ���ؽ��VO�����ÿ����������ִ�н��
	 */
	chinapres3RevokeResultVO::Wrapper execRevokechinapres3(
		const chinapres3RevokeDTO::Wrapper& dto,
		const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_chinapres3CONTROLLER_H_