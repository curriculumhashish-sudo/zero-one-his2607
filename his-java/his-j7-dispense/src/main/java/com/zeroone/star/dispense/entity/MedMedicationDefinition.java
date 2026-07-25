package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品定义（med_medication_definition）
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("med_medication_definition")
public class MedMedicationDefinition implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String busNo;

    private String name;

    private String domainEnum;

    private String medVersion;

    private String nameEn;

    private String pyStr;

    private String wbStr;

    private String categoryCode;

    private String merchandiseName;

    private String merchandisePyStr;

    private String merchandiseWbStr;

    private String purchaseUnitCode;

    private String splitUnitCode;

    private String comprisedText;

    private String ingredient;

    private Integer doseFrom;

    private String approvalNumber;

    private Boolean ybMatchFlag;

    private String ybNo;

    private String pharmacologyCategoryCode;

    private Boolean skinTestFlag;

    private Boolean injectFlag;

    private String manufacturerId;

    private String manufacturerText;

    private String supplyId;

    private Boolean restrictedFlag;

    private String restrictedScope;

    private Boolean activeFlag;

    private Boolean childrenFlag;

    private Integer characteristic;

    private BigDecimal itemMinQuantity;

    private BigDecimal itemMaxQuantity;

    private String defEncounterUnitCode;

    private String defInhospitalUnitCode;

    private String nationalDrugCode;

    private String partAttributeEnum;

    private String antibioticCode;

    private String restrictedEnum;

    private Boolean selfFlag;

    private Boolean antibioticFlag;

    private Boolean basicFlag;

    private String thoPartAttributeEnum;

    private Integer chrgitmLv;

    private Boolean rxFlag;

    private String dosageInstruction;

    private String drug69Code;

    private String totalVolume;

    private String doseFormCode;

    private String doseUnitCode;

    private String packageUnitCode;

    private String stockUnitCode;

    private String methodCode;

    private String rateCode;

    private BigDecimal dose;

    private BigDecimal maxUnit;

    private BigDecimal usageLimit;

    private String locationId;

    private String statusEnum;

    private String orgId;

    private String ingredientItem;

    private String medicationdefinition;

    private String dddCode;

    private String dddUnitCode;

    private String minRateCode;

    private String maxRateCode;

    private BigDecimal doseUnitRatio;

    private BigDecimal purchaseUnitRatio;

    private BigDecimal stockUnitRatio;

    private BigDecimal packageUnitRatio;
}
