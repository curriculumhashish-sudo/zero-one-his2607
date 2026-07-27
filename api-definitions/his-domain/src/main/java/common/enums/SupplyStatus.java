package common.enums;

public enum SupplyStatus {

    DRAFT("DRAFT", "草稿"),
    PENDING_APPROVAL("PENDING_APPROVAL", "待审核"),
    APPROVED("APPROVED", "已审核"),
    REJECTED("REJECTED", "已驳回"),
    WITHDRAWN("WITHDRAWN", "已撤回");

    private final String code;
    private final String display;

    SupplyStatus(String code, String display) { this.code = code; this.display = display; }
    public String getCode() { return code; }
    public String getDisplay() { return display; }
}
