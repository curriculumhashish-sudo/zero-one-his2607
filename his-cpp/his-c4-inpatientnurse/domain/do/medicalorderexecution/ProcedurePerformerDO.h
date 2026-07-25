#pragma once
#ifndef _PROCEDUREPERFORMERDO_
#define _PROCEDUREPERFORMERDO_

#include "../DoInclude.h"

class ProcedurePerformerDO : public BaseDO
{
public:
    ProcedurePerformerDO() : BaseDO("cln_procedure_performer") {}
};

typedef std::shared_ptr<ProcedurePerformerDO> PtrProcedurePerformerDO;

#endif
