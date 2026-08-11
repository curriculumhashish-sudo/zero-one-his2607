#ifndef _GET_TODAY_APPOINT_DO_
#define _GET_TODAY_APPOINT_DO_

#include "../DoInclude.h"


class GetTodayAppointDO:public BaseDO
{
	// 编号
	MYSQL_SYNTHESIZE(string, id, Id);
	//号源状态
	MYSQL_SYNTHESIZE(string, status, Status);
	//号源总容量
	MYSQL_SYNTHESIZE(int, capacity, Capacity);
	//已预约计数
	MYSQL_SYNTHESIZE(int, booked_count, Booked_count);
	//号源类型
	MYSQL_SYNTHESIZE(string, register_type, Register_type);
	//科室
	MYSQL_SYNTHESIZE(string, organization, Organization);
	//开始时间
	MYSQL_SYNTHESIZE(string, start_time, Start_time);
	//结束时间
	MYSQL_SYNTHESIZE(string,end_time,End_time)
	//服务类型
	MYSQL_SYNTHESIZE(string, serviceTypeId, ServiceTypeId)
	//挂号费
	MYSQL_SYNTHESIZE(string, cost, Cost)
	//医生id
	MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId)
	//科室id
	MYSQL_SYNTHESIZE(string, org_id, Org_id)

public:
	GetTodayAppointDO() : BaseDO("GetTodauApoint")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("status", "s", status);
		MYSQL_ADD_FIELD("capacity", "i", capacity);
		MYSQL_ADD_FIELD("booked_count", "i", booked_count);
		MYSQL_ADD_FIELD("register_type", "s", register_type);
		MYSQL_ADD_FIELD("organization", "s", organization);
		MYSQL_ADD_FIELD("start_time", "dt", start_time);
		MYSQL_ADD_FIELD("end_time", "dt", end_time);
		MYSQL_ADD_FIELD("serviceTypeId", "s", serviceTypeId);
		MYSQL_ADD_FIELD("cost", "s", cost);
		MYSQL_ADD_FIELD("practitionerId", "s", practitionerId);
		MYSQL_ADD_FIELD("org_id", "s", org_id);
	}
};


#endif
