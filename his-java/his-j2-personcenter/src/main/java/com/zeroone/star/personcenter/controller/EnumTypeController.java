package com.zeroone.star.personcenter.controller;

import com.zeroone.star.project.dto.j2.sys.EnumTypeDeleteDTO;
import com.zeroone.star.project.j2.sys.enumTypeApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "枚举类型管理相关接口")
public class EnumTypeController implements enumTypeApis {
    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum-type")
    public JsonVO<String> deleteEnumType(@RequestBody EnumTypeDeleteDTO dto) {
        return null;
    }
}
