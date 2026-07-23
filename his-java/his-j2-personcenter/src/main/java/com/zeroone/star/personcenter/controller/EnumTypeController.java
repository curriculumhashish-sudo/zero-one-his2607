package com.zeroone.star.personcenter.controller;

import com.zeroone.star.project.j2.sys.enumTypeApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "枚举类型管理相关接口")
public class EnumTypeController implements enumTypeApis {
    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum-type/{enumKey}")
    public JsonVO<String> deleteEnumType(@PathVariable(value = "enumKey", required = true) String enumKey) {
        return null;
    }
}
