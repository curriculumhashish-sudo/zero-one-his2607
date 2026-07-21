package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.configuration_management.medical_record_template.EmrDocumentTypeApis;
import com.zeroone.star.project.query.j5.configuration_management.medical_record_template.EmrDocumentTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeDetailVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeNameVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.apache.ibatis.annotations.Mapper;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：获取文书类型控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */

@RestController
@Tag(name = "获取文书类型")
public class EmrDocumentTypeController implements EmrDocumentTypeApis {

    /**
     * 获取文书类型下拉字典（仅启用）
     * 返回：集合List
     */
    @GetMapping("/name")
    @Operation(summary = "获取文书类型下拉选项")
    @Override
    public JsonVO<List<EmrDocumentTypeNameVO>> getEmrDocumentTypeName() {
        return null;
    }

    /**
     * 分页查询文书类型列表
     * 双层泛型：外层JsonVO，内层分页PageDTO，分页内单条EmrDocumentTypeVO
     */
    @GetMapping("/page")
    @Operation(summary = "分页查询全部文书类型")
    @Override
    public JsonVO<PageDTO<EmrDocumentTypeVO>> queryEmrDocumentType(EmrDocumentTypeQuery query) {
        return null;
    }

    /**
     * 根据ID查询单条详情
     */
    @GetMapping("/{id}")
    @Operation(summary = "根据主键ID查询文书类型详情")
    @Override
    public JsonVO<EmrDocumentTypeDetailVO> getEmrDocumentTypeDetail(String id) {
        return null;
    }
}
