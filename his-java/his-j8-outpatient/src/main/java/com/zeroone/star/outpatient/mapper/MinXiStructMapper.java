package com.zeroone.star.outpatient.mapper;

import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import com.zeroone.star.project.dto.j8.mzfymx.MinXiExportDTO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Named;
import org.mapstruct.factory.Mappers;

import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细 MapStruct 转换器
 *      核心转换：MinXiDTO（查询列表用，含内部ID和序号）→ MinXiExportDTO（导出Excel用，不含内部字段）
 *      特殊处理：tradeDirection 英文编码 → 中文展示（CHARGE→收费，REFUND→退费）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Zzen
 * @version 1.0.0
 */
@Mapper(componentModel = "default")
public interface MinXiStructMapper {

    MinXiStructMapper INSTANCE = Mappers.getMapper(MinXiStructMapper.class);

    /**
     * 单条转换：MinXiDTO → MinXiExportDTO
     * ⚠️ tradeDirection 使用 qualifiedByName 自定义翻译（英文→中文）
     * ⚠️ 其他同名同类型字段（outpatientNo/patientName/deptName/.../8个金额）自动映射
     */
    @Mapping(source = "tradeDirection", target = "tradeDirection", qualifiedByName = "translateTradeDirection")
    MinXiExportDTO toExportDTO(MinXiDTO dto);

    /**
     * 批量转换（调用单条循环）
     */
    List<MinXiExportDTO> toExportDTOList(List<MinXiDTO> list);

    /**
     * 收退方向翻译：英文编码 → 中文展示
     *  CHARGE → 收费
     *  REFUND → 退费
     *  其他（null/空/未知）→ 原字符串返回（兜底，避免 NPE）
     */
    @Named("translateTradeDirection")
    default String translateTradeDirection(String code) {
        if (code == null) {
            return null;
        }
        switch (code) {
            case "CHARGE":
                return "收费";
            case "REFUND":
                return "退费";
            default:
                return code;
        }
    }
}