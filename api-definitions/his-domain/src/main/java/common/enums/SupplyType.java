package common.enums;

public enum SupplyType {

    PURCHASE_INBOUND("PURCHASE_INBOUND", "采购入库"),
    TRANSFER_INBOUND("TRANSFER_INBOUND", "调拨入库"),
    STOCK_IN("STOCK_IN", "药房入库"),
    STOCK_OUT("STOCK_OUT", "药库出库"),
    STOCKTAKE("STOCKTAKE", "盘点"),
    PURCHASE_ORDER("PURCHASE_ORDER", "订货"),
    RETURN_TO_SUPPLIER("RETURN_TO_SUPPLIER", "退货"),
    GAIN_LOSS("GAIN_LOSS", "损益");

    private final String code;
    private final String display;

    SupplyType(String code, String display) { this.code = code; this.display = display; }
    public String getCode() { return code; }
    public String getDisplay() { return display; }
}
