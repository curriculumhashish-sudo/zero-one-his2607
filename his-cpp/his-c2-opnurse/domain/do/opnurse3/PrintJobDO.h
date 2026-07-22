#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_DO_
#define _PRINT_JOB_DO_

#include "../DoInclude.h"

class PrintJobDO : public BaseDO
{
	// id
	MYSQL_SYNTHESIZE(string, id, Id);
	// doc type
	MYSQL_SYNTHESIZE(string, docType, DocType);
	// patient name
	MYSQL_SYNTHESIZE(string, patientName, PatientName);
	// treatment item
	MYSQL_SYNTHESIZE(string, treatmentItem, TreatmentItem);
	// exec department
	MYSQL_SYNTHESIZE(string, execDept, ExecDept);
	// doctor name
	MYSQL_SYNTHESIZE(string, doctorName, DoctorName);
	// order time
	MYSQL_SYNTHESIZE(string, orderTime, OrderTime);
	// exec time
	MYSQL_SYNTHESIZE(string, execTime, ExecTime);
	// executor
	MYSQL_SYNTHESIZE(string, executor, Executor);
	// status
	MYSQL_SYNTHESIZE(int, status, Status);
	// print count
	MYSQL_SYNTHESIZE(int, printCount, PrintCount);
	// remark
	MYSQL_SYNTHESIZE(string, remark, Remark);
	// create by
	MYSQL_SYNTHESIZE(string, createBy, CreateBy);
	// create time
	MYSQL_SYNTHESIZE(string, createTime, CreateTime);
	// update by
	MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
	// update time
	MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);

public:
	PrintJobDO() : BaseDO("print_job")
	{
		MYSQL_ADD_FIELD_PK("id", "s", id);
		MYSQL_ADD_FIELD("doc_type", "s", docType);
		MYSQL_ADD_FIELD("patient_name", "s", patientName);
		MYSQL_ADD_FIELD("treatment_item", "s", treatmentItem);
		MYSQL_ADD_FIELD("exec_dept", "s", execDept);
		MYSQL_ADD_FIELD("doctor_name", "s", doctorName);
		MYSQL_ADD_FIELD("order_time", "dt", orderTime);
		MYSQL_ADD_FIELD("exec_time", "dt", execTime);
		MYSQL_ADD_FIELD("executor", "s", executor);
		MYSQL_ADD_FIELD("status", "i", status);
		MYSQL_ADD_FIELD("print_count", "i", printCount);
		MYSQL_ADD_FIELD("remark", "s", remark);
		MYSQL_ADD_FIELD("create_by", "s", createBy);
		MYSQL_ADD_FIELD("create_time", "dt", createTime);
		MYSQL_ADD_FIELD("update_by", "s", updateBy);
		MYSQL_ADD_FIELD("update_time", "dt", updateTime);
	}
};

typedef std::shared_ptr<PrintJobDO> PtrPrintJobDO;
#endif // !_PRINT_JOB_DO_
