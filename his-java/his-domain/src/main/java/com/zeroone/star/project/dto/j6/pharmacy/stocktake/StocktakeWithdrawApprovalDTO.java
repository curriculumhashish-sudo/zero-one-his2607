package com.zeroone.star.project.dto.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "Withdraw warehouse stocktake approval request")
public class StocktakeWithdrawApprovalDTO {
    @Schema(description = "Stocktake order business number", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "busNo cannot be blank")
    private String busNo;
}
