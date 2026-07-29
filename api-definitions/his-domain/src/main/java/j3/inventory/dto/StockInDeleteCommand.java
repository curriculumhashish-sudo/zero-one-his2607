package j3.inventory.dto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

@Schema(description = "入库单删除命令")
public class StockInDeleteCommand {

    @NotBlank(message = "单据号不能为空")
    @Size(max = 64)
    @Schema(description = "单据号，对应 wkf_supply_request.bus_no", example = "SI202607190001", defaultValue = "SI202607190001")
    private String busNo;

    @Size(max = 500)
    @Schema(description = "删除原因", example = "录入错误", defaultValue = "录入错误")
    private String reason;

    public String getBusNo() { return busNo; }
    public void setBusNo(String busNo) { this.busNo = busNo; }
    public String getReason() { return reason; }
    public void setReason(String reason) { this.reason = reason; }
}
