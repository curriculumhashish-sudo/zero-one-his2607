package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "Reject warehouse stocktake request")
public class StocktakeRejectApprovalDTO {
    @Schema(description = "Stocktake order business number", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "busNo cannot be blank")
    private String busNo;

    @Schema(description = "Reject reason code")
    private String changeReasonCode;

    @Schema(description = "Reject reason", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "changeReason cannot be blank when rejecting approval")
    private String changeReason;
}
