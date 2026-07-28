package j4.pharmacy.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import java.util.List;

@Schema(description = "门诊批量发药结果")
public class OutpatientBatchDispenseVO {

    @Schema(description = "提交总数", example = "5", defaultValue = "0")
    private int totalCount;

    @Schema(description = "发药成功数", example = "4", defaultValue = "0")
    private int successCount;

    @Schema(description = "发药失败数", example = "1", defaultValue = "0")
    private int failCount;

    @Schema(description = "失败明细", defaultValue = "[]")
    private List<DispenseError> errors;

    @Schema(description = "发药失败项")
    public static class DispenseError {
        @Schema(description = "发药记录ID", example = "322663105157926913", defaultValue = "322663105157926913")
        private String dispenseId;
        @Schema(description = "失败原因", example = "库存不足: 需要10.00, 当前5.00", defaultValue = "未知错误")
        private String reason;

        public String getDispenseId() { return dispenseId; }
        public void setDispenseId(String dispenseId) { this.dispenseId = dispenseId; }
        public String getReason() { return reason; }
        public void setReason(String reason) { this.reason = reason; }
    }

    public int getTotalCount() { return totalCount; }
    public void setTotalCount(int totalCount) { this.totalCount = totalCount; }
    public int getSuccessCount() { return successCount; }
    public void setSuccessCount(int successCount) { this.successCount = successCount; }
    public int getFailCount() { return failCount; }
    public void setFailCount(int failCount) { this.failCount = failCount; }
    public List<DispenseError> getErrors() { return errors; }
    public void setErrors(List<DispenseError> errors) { this.errors = errors; }
}
