package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "Stocktake approval flow payload")
public class StocktakeApprovalDTO {
    @Schema(description = "Stocktake order business number", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "busNo cannot be blank")
    private String busNo;

    @Schema(description = "Approval action: SUBMIT, WITHDRAW, AGREE, REJECT")
    private String action;

    @Schema(description = "Reject or change reason code")
    private String changeReasonCode;

    @Schema(description = "Reject or change reason")
    private String changeReason;

    @Schema(description = "Contract business number")
    private String contractBusNo;
}
