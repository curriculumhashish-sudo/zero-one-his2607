package j3.inventory.vo;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "单据删除结果")
public class SupplyDocumentDeleteVO {

    @Schema(description = "单据号", example = "SI202607190001", defaultValue = "SI202607190001")
    private String busNo;

    @Schema(description = "删除行数", example = "3", defaultValue = "0")
    private int deletedLines;

    @Schema(description = "解锁库存锁记录数", example = "3", defaultValue = "0")
    private int unlockedLocks;

    public String getBusNo() { return busNo; }
    public void setBusNo(String busNo) { this.busNo = busNo; }
    public int getDeletedLines() { return deletedLines; }
    public void setDeletedLines(int deletedLines) { this.deletedLines = deletedLines; }
    public int getUnlockedLocks() { return unlockedLocks; }
    public void setUnlockedLocks(int unlockedLocks) { this.unlockedLocks = unlockedLocks; }
}
