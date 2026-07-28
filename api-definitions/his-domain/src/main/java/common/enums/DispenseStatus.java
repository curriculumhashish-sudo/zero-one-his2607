package common.enums;

public enum DispenseStatus {

    PENDING("PENDING", "待发药"),
    PREPARED("PREPARED", "已配药"),
    DISPENSED("DISPENSED", "已发药"),
    CANCELLED("CANCELLED", "已取消");

    private final String code;
    private final String display;

    DispenseStatus(String code, String display) { this.code = code; this.display = display; }
    public String getCode() { return code; }
    public String getDisplay() { return display; }
}
