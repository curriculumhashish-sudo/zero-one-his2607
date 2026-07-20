package com.zeroone.star.project.j2.basedata.nulll;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.*;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;

import java.util.List;


public interface enumApis {
    /**
     * 删除枚举类型
     * @param dto 枚举删除对象DTO
     * @return 枚举类型id
     */
    JsonVO<String> deleteEnumType(EnumDeleteDTO dto);

    /**
     * 获取枚举名称列表
      * @param query 枚举查询对象DTO
      * @return 枚举查询对象DTO
     */
    JsonVO<List<String>> getEnumTypeName(EnumListQuery query);

    /**
     * 获取枚举列表(条件+分页)
     * @param query 枚举查询对象DTO
     * @return
     */
    JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query);


    @GetMapping("/query-all")
    @Operation(summary = "分页查询枚举类型", description = "分页查询枚举类型")
    JsonVO<PageDTO<EnumTypeDTO>> queryAll(EnumTypeQuery query);

    @PostMapping("/add-enum-type")
    @Operation(summary = "添加枚举类型", description = "添加枚举类型")
    JsonVO<String> addEnumType(EnumTypeAddDTO data);

    @PutMapping("/modify-enum-type")
    @Operation(summary = "修改枚举类型", description = "修改枚举类型")
    JsonVO<String> modifyEnumType(EnumTypeModifyDTO data);
}
