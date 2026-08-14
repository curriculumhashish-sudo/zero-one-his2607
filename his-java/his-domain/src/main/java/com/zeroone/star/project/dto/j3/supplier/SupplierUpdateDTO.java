package com.zeroone.star.project.dto.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商数据库更新DTO")
public class SupplierUpdateDTO {


    // ==================== 基础字段 ====================

    @Schema(description = "主键", example = "4100000000000000002")
    private String id;

    // ==================== 业务字段 ====================

    @Schema(description = "名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型枚举值（ENTERPRISE-企业/INDIVIDUAL-个体）", example = "ENTERPRISE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "统一社会信用代码", example = "91110000100000XXXX")
    private String creditCode;

    @Schema(description = "法定代表人", example = "张三")
    private String legalRepresentative;

    @Schema(description = "地址", example = "北京市朝阳区XX路XX号")
    private String address;

    @Schema(description = "拼音码", example = "HRYY", requiredMode = Schema.RequiredMode.REQUIRED)
    private String pyStr;

    @Schema(description = "五笔码", example = "WXDG", requiredMode = Schema.RequiredMode.REQUIRED)
    private String wbStr;

    @Schema(description = "联系电话", example = "010-88888888")
    private String phone;

    @Schema(description = "邮箱", example = "contact@huarun.com")
    private String email;

    @Schema(description = "开户行", example = "中国工商银行北京分行")
    private String bankName;

    @Schema(description = "银行账户", example = "6222000000000000000")
    private String bankAccount;

    @Schema(description = "机构标识，不传保留原机构", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String orgId;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "状态枚举值（ACTIVE-启用/INACTIVE-停用）", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "备注说明", example = "优质供应商，年合作额超百万")
    private String note;
}
