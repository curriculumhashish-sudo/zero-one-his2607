package j4.pharmacy.dto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import java.util.List;

@Schema(description = "门诊批量发药命令")
public class OutpatientBatchDispenseCommand {

    @NotEmpty(message = "待发药记录不能为空")
    @Schema(description = "待发药记录ID列表，对应 med_medication_dispense.id",
            example = "[\"322663105157926912\", \"322663105157926913\"]",
            defaultValue = "[\"322663105157926912\"]")
    private List<@NotBlank String> dispenseIds;

    @Schema(description = "就诊ID列表，对应 adm_encounter.id（可选）",
            example = "[\"322663000000000001\"]", defaultValue = "null")
    private List<String> encounterIds;

    @NotBlank(message = "调配药师不能为空")
    @Schema(description = "调配药师ID，对应 adm_practitioner.id",
            example = "320000000000000001", defaultValue = "320000000000000001")
    private String preparerId;

    @NotBlank(message = "发放库房不能为空")
    @Schema(description = "发放库房/药房ID，对应 adm_location.id",
            example = "320000000000000100", defaultValue = "320000000000000100")
    private String locationId;

    @Schema(description = "批号选择列表，不传则按效期FIFO自动选", defaultValue = "[]")
    private List<LotSelection> lotSelections;

    @Schema(description = "追溯码列表", defaultValue = "[]")
    private List<TraceCode> traceCodes;

    @Schema(description = "批号选择")
    public static class LotSelection {
        @NotBlank
        @Schema(description = "发药记录ID", example = "322663105157926912", defaultValue = "322663105157926912")
        private String dispenseId;
        @NotBlank
        @Schema(description = "产品批号，对应 wkf_inventory_item.lot_number", example = "20260715-001", defaultValue = "20260715-001")
        private String lotNumber;
        @Schema(description = "库存项ID（可选），对应 wkf_inventory_item.id", example = "320000000000000200", defaultValue = "")
        private String inventoryId;

        public String getDispenseId() { return dispenseId; }
        public void setDispenseId(String dispenseId) { this.dispenseId = dispenseId; }
        public String getLotNumber() { return lotNumber; }
        public void setLotNumber(String lotNumber) { this.lotNumber = lotNumber; }
        public String getInventoryId() { return inventoryId; }
        public void setInventoryId(String inventoryId) { this.inventoryId = inventoryId; }
    }

    @Schema(description = "追溯码")
    public static class TraceCode {
        @NotBlank
        @Schema(description = "发药记录ID", example = "322663105157926912", defaultValue = "322663105157926912")
        private String dispenseId;
        @NotBlank
        @Schema(description = "国家药品追溯码", example = "81234567890123456789", defaultValue = "81234567890123456789")
        private String traceNo;

        public String getDispenseId() { return dispenseId; }
        public void setDispenseId(String dispenseId) { this.dispenseId = dispenseId; }
        public String getTraceNo() { return traceNo; }
        public void setTraceNo(String traceNo) { this.traceNo = traceNo; }
    }

    // getters/setters
    public List<String> getDispenseIds() { return dispenseIds; }
    public void setDispenseIds(List<String> dispenseIds) { this.dispenseIds = dispenseIds; }
    public List<String> getEncounterIds() { return encounterIds; }
    public void setEncounterIds(List<String> encounterIds) { this.encounterIds = encounterIds; }
    public String getPreparerId() { return preparerId; }
    public void setPreparerId(String preparerId) { this.preparerId = preparerId; }
    public String getLocationId() { return locationId; }
    public void setLocationId(String locationId) { this.locationId = locationId; }
    public List<LotSelection> getLotSelections() { return lotSelections; }
    public void setLotSelections(List<LotSelection> lotSelections) { this.lotSelections = lotSelections; }
    public List<TraceCode> getTraceCodes() { return traceCodes; }
    public void setTraceCodes(List<TraceCode> traceCodes) { this.traceCodes = traceCodes; }
}
