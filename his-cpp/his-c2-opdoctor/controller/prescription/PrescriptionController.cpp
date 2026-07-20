#include "PrescriptionController.h"

PrescriptionDeleteJsonVO::Wrapper
PrescriptionController::execBatchDeletePrescription(
    const PrescriptionBatchDeleteDTO::Wrapper& dto
)
{
    auto result = PrescriptionDeleteJsonVO::createShared();

    result->data =
        PrescriptionDeleteResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}

PrescriptionMergeJsonVO::Wrapper
PrescriptionController::execMergePrescriptionGroup(
    const PrescriptionMergeGroupDTO::Wrapper& dto
)
{
    auto result = PrescriptionMergeJsonVO::createShared();

    result->data =
        PrescriptionMergeResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}

PrescriptionSplitJsonVO::Wrapper
PrescriptionController::execSplitPrescriptionGroup(
    const PrescriptionSplitGroupDTO::Wrapper& dto
)
{
    auto result = PrescriptionSplitJsonVO::createShared();

    result->data =
        PrescriptionSplitResultDTO::createShared();

    if (dto && dto->prescriptionIds)
    {
        result->data->affectedCount =
            static_cast<v_int32>(
                dto->prescriptionIds->size()
                );
    }

    return result;
}