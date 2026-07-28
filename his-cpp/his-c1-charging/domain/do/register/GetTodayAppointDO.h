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
	MYSQL_SYNTHESIZE(unsigned long long, start_time, Start_time);
	//结束时间
	MYSQL_SYNTHESIZE(unsigned long long,end_time,End_time)

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
	}
};


#endif
