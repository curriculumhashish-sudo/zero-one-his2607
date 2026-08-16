#ifndef _GET_SEE_DO_
#define _GET_SEE_DO_
#include "../DoInclude.h"

class GetSeeDO : public BaseDO
{
	// 编号
	MYSQL_SYNTHESIZE(string, id, Id);
	// 就诊号
	MYSQL_SYNTHESIZE(string, bus_no, Bus_no);
	// 患者名称
	MYSQL_SYNTHESIZE(string, name, Name);
	// 性别
	MYSQL_SYNTHESIZE(string, gender_enum, Gender_enum);
	// 电话
	MYSQL_SYNTHESIZE(string, phone, Phone);
	// 科室
	MYSQL_SYNTHESIZE(string, org_name, Org_name);
	// 医生
	MYSQL_SYNTHESIZE(string, doctor, Doctor);
	// 费用性质
	MYSQL_SYNTHESIZE(string, cost_type, Cost_type);
	//参保地
	MYSQL_SYNTHESIZE(string,addr,Addr);
	//挂号类型
	MYSQL_SYNTHESIZE(string, register_type, Register_type);
	//挂号费
	MYSQL_SYNTHESIZE(double, cost, Cost);
	//挂号时间
	MYSQL_SYNTHESIZE(string, time, Time);

	MYSQL_SYNTHESIZE(string, serviceTypeId, ServiceTypeId);

	MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);

public:
	GetSeeDO() : BaseDO("GetSee"){
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("gender_enum", "s", gender_enum);
		MYSQL_ADD_FIELD("phone", "s", phone);
		MYSQL_ADD_FIELD("org_name", "s", org_name);
		MYSQL_ADD_FIELD("doctor", "s", doctor);
		MYSQL_ADD_FIELD("cost_type", "s", cost_type);
		MYSQL_ADD_FIELD("addr", "s", addr);
		MYSQL_ADD_FIELD("register_type", "s", register_type);
		MYSQL_ADD_FIELD("cost", "d", cost);
		MYSQL_ADD_FIELD("time", "dt", time);
		MYSQL_ADD_FIELD("serviceTypeId", "s", serviceTypeId);
		MYSQL_ADD_FIELD("statusEnum", "s", statusEnum);

	}
};

#endif // !_SAMPLE_DO_