#include "Prescription3Controller.h"

/**
 * 删除医嘱，支持批量
 *
 * 当前只返回接口文档测试数据，不执行数据库操作。
 */
Prescription3DeleteJsonVO::Wrapper
Prescription3Controller::execBatchDeletePrescription3(
    const Prescription3BatchDeleteDTO::Wrapper& dto
)
{
    auto result =
        Prescription3DeleteJsonVO::createShared();

    result->data =
        Prescription3DeleteResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}

/**
 * 并组医嘱
 *
 * 当前只返回接口文档测试数据，不执行数据库操作。
 */
Prescription3MergeJsonVO::Wrapper
Prescription3Controller::execMergePrescription3Group(
    const Prescription3MergeGroupDTO::Wrapper& dto
)
{
    auto result =
        Prescription3MergeJsonVO::createShared();

    result->data =
        Prescription3MergeResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}

/**
 * 拆组医嘱
 *
 * 当前只返回接口文档测试数据，不执行数据库操作。
 */
Prescription3SplitJsonVO::Wrapper
Prescription3Controller::execSplitPrescription3Group(
    const Prescription3SplitGroupDTO::Wrapper& dto
)
{
    auto result =
        Prescription3SplitJsonVO::createShared();

    result->data =
        Prescription3SplitResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}