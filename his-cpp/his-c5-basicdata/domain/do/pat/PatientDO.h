#ifndef _PATIENT_DO_
#define _PATIENT_DO_
#include "../DoInclude.h"

/**
 * 患者数据实体类
 */
class PatientDO : public BaseDO
{
	// 主键
	MYSQL_SYNTHESIZE(string, id, Id);
	// 租户标识
	MYSQL_SYNTHESIZE(string, tenantId, TenantId);

	// 创建时间
	MYSQL_SYNTHESIZE(string, createdAt, CreatedAt);
	// 更新时间
	MYSQL_SYNTHESIZE(string, updatedAt, UpdatedAt);
	// 创建人
	MYSQL_SYNTHESIZE(string, createdBy, CreatedBy);
	// 更新人
	MYSQL_SYNTHESIZE(string, updatedBy, UpdatedBy);

	// 删除标记
	MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
	// 删除时间
	MYSQL_SYNTHESIZE(string, deletedAt, DeletedAt);
	// 删除人
	MYSQL_SYNTHESIZE(string, deletedBy, DeletedBy);

	// 乐观锁版本
	MYSQL_SYNTHESIZE(int, version, Version);

	// 患者业务编号
	MYSQL_SYNTHESIZE(string, busNo, BusNo);
	// 患者姓名
	MYSQL_SYNTHESIZE(string, name, Name);
	// 姓名扩展JSON
	MYSQL_SYNTHESIZE(string, nameJson, NameJson);

	// 性别编码
	MYSQL_SYNTHESIZE(string, gender, Gender);
	// 出生日期
	MYSQL_SYNTHESIZE(string, birthDate, BirthDate);

	// 患者状态
	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

	// 启用标记
	MYSQL_SYNTHESIZE(int, activeFlag, ActiveFlag);
	// 临时建档标记
	MYSQL_SYNTHESIZE(int, tempFlag, TempFlag);

	// 身份证号
	MYSQL_SYNTHESIZE(string, idCard, IdCard);

	// 婚姻状态
	MYSQL_SYNTHESIZE(string, maritalStatusEnum, MaritalStatusEnum);

	// 职业编码
	MYSQL_SYNTHESIZE(string, prfsEnum, PrfsEnum);

	// 联系电话
	MYSQL_SYNTHESIZE(string, phone, Phone);

	// 地址
	MYSQL_SYNTHESIZE(string, address, Address);
	// 省
	MYSQL_SYNTHESIZE(string, addressProvince, AddressProvince);
	// 市
	MYSQL_SYNTHESIZE(string, addressCity, AddressCity);
	// 区
	MYSQL_SYNTHESIZE(string, addressDistrict, AddressDistrict);
	// 街道
	MYSQL_SYNTHESIZE(string, addressStreet, AddressStreet);

	// 地址扩展
	MYSQL_SYNTHESIZE(string, addressExtendJson, AddressExtendJson);

	// 民族编码
	MYSQL_SYNTHESIZE(string, nationalityCode, NationalityCode);

	// 拼音码
	MYSQL_SYNTHESIZE(string, pyStr, PyStr);
	// 五笔码
	MYSQL_SYNTHESIZE(string, wbStr, WbStr);

	// ABO血型
	MYSQL_SYNTHESIZE(int, bloodTypeAbo, BloodTypeAbo);
	// RH血型
	MYSQL_SYNTHESIZE(int, bloodTypeRh, BloodTypeRh);

	// 工作单位
	MYSQL_SYNTHESIZE(string, workCompany, WorkCompany);

	// 籍贯
	MYSQL_SYNTHESIZE(string, nativePlace, NativePlace);

	// 国家地区编码
	MYSQL_SYNTHESIZE(string, countryCode, CountryCode);

	// 联系人
	MYSQL_SYNTHESIZE(string, linkName, LinkName);
	// 联系关系编码
	MYSQL_SYNTHESIZE(int, linkRelationCode, LinkRelationCode);
	// 联系电话
	MYSQL_SYNTHESIZE(string, linkTelcom, LinkTelcom);
	// 联系人JSON
	MYSQL_SYNTHESIZE(string, linkJsons, LinkJsons);

	// 死亡时间
	MYSQL_SYNTHESIZE(string, deceasedDate, DeceasedDate);
	// 死亡原因
	MYSQL_SYNTHESIZE(string, deceasedReason, DeceasedReason);

	// 建档机构
	MYSQL_SYNTHESIZE(string, organizationId, OrganizationId);

	// 合并目标患者
	MYSQL_SYNTHESIZE(string, mergedTargetPatientId, MergedTargetPatientId);
	// 合并时间
	MYSQL_SYNTHESIZE(string, mergedAt, MergedAt);

public:
	PatientDO() : BaseDO("adm_patient")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);

		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);

		MYSQL_ADD_FIELD("created_at", "dt", createdAt);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updatedAt, true);

		MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);

		MYSQL_ADD_FIELD("is_deleted", "i", isDeleted);

		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deletedAt, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);

		MYSQL_ADD_FIELD("version", "i", version);


		MYSQL_ADD_FIELD("bus_no", "s", busNo);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD_NULLABLE("name_json", "s", nameJson, true);

		MYSQL_ADD_FIELD("gender", "s", gender);
		MYSQL_ADD_FIELD_NULLABLE("birth_date", "s", birthDate, true);

		MYSQL_ADD_FIELD("status_enum", "s", statusEnum);

		MYSQL_ADD_FIELD("active_flag", "i", activeFlag);
		MYSQL_ADD_FIELD("temp_flag", "i", tempFlag);

		MYSQL_ADD_FIELD_NULLABLE("id_card", "s", idCard, true);

		MYSQL_ADD_FIELD_NULLABLE("marital_status_enum", "s", maritalStatusEnum, true);
		MYSQL_ADD_FIELD_NULLABLE("prfs_enum", "s", prfsEnum, true);

		MYSQL_ADD_FIELD_NULLABLE("phone", "s", phone, true);

		MYSQL_ADD_FIELD_NULLABLE("address", "s", address, true);

		MYSQL_ADD_FIELD_NULLABLE("address_province", "s", addressProvince, true);
		MYSQL_ADD_FIELD_NULLABLE("address_city", "s", addressCity, true);
		MYSQL_ADD_FIELD_NULLABLE("address_district", "s", addressDistrict, true);
		MYSQL_ADD_FIELD_NULLABLE("address_street", "s", addressStreet, true);

		MYSQL_ADD_FIELD_NULLABLE("address_extend_json", "s", addressExtendJson, true);

		MYSQL_ADD_FIELD_NULLABLE("nationality_code", "s", nationalityCode, true);

		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", pyStr, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wbStr, true);

		MYSQL_ADD_FIELD_NULLABLE("blood_type_abo", "i", bloodTypeAbo, true);
		MYSQL_ADD_FIELD_NULLABLE("blood_type_rh", "i", bloodTypeRh, true);

		MYSQL_ADD_FIELD_NULLABLE("work_company", "s", workCompany, true);

		MYSQL_ADD_FIELD_NULLABLE("native_place", "s", nativePlace, true);

		MYSQL_ADD_FIELD_NULLABLE("country_code", "s", countryCode, true);


		MYSQL_ADD_FIELD_NULLABLE("link_name", "s", linkName, true);
		MYSQL_ADD_FIELD_NULLABLE("link_relation_code", "i", linkRelationCode, true);
		MYSQL_ADD_FIELD_NULLABLE("link_telcom", "s", linkTelcom, true);
		MYSQL_ADD_FIELD_NULLABLE("link_jsons", "s", linkJsons, true);

		MYSQL_ADD_FIELD_NULLABLE("deceased_date", "dt", deceasedDate, true);
		MYSQL_ADD_FIELD_NULLABLE("deceased_reason", "s", deceasedReason, true);

		MYSQL_ADD_FIELD("organization_id", "s", organizationId);

		MYSQL_ADD_FIELD_NULLABLE("merged_target_patient_id", "s", mergedTargetPatientId, true);
		MYSQL_ADD_FIELD_NULLABLE("merged_at", "dt", mergedAt, true);
	}

};

typedef std::shared_ptr<PatientDO> PtrPatientDO;

#endif // !_PATIENT_DO_