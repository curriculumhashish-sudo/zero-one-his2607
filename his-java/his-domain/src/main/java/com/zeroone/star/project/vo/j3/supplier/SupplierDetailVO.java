package com.zeroone.star.project.vo.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：供应商详情完整VO（表单回填、详情弹窗，包含全量业务字段，不含系统基础字段）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商详情完整视图对象")
public class SupplierDetailVO implements Serializable {

    @Schema(description = "主键", example = "1234567890123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编号", example = "SUP20250001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型枚举值", example = "ENTERPRISE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "统一社会信用代码", example = "91110000100000XXXX", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creditCode;

    @Schema(description = "法定代表人", example = "张三")
    private String legalRepresentative;

    @Schema(description = "地址", example = "北京市朝阳区XX路XX号")
    private String address;

    @Schema(description = "拼音码", example = "HRYY")
    private String pyStr;

    @Schema(description = "五笔码", example = "WXDG")
    private String wbStr;

    @Schema(description = "联系电话", example = "010-88888888")
    private String phone;

    @Schema(description = "邮箱", example = "contact@huarun.com")
    private String email;

    @Schema(description = "开户行", example = "中国工商银行北京分行")
    private String bankName;

    @Schema(description = "银行账户", example = "6222000000000000000")
    private String bankAccount;

    @Schema(description = "机构标识", example = "ORG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String orgId;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "状态枚举值（ENABLED-启用/DISABLED-停用）", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "备注说明", example = "优质供应商，年合作额超百万")
    private String note;
}
