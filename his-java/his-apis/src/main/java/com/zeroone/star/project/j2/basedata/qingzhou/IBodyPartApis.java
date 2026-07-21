package com.zeroone.star.project.j2.basedata.qingzhou;


import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.basedata.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.BodyPartTreeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;


import java.util.List;

/**
 * 部位管理 API 接口定义
 */
@Tag(name = "部位管理")
public interface IBodyPartApis {

    @Operation(summary = "获取部位名称树")
    JsonVO<List<BodyPartTreeVO>> getTree();

    @Operation(summary = "获取部位列表（条件+分页）")
    JsonVO<List<BodyPartListVO>> getList(@Valid BodyPartPageQuery query);

    @Operation(summary = "添加部位")
    JsonVO<Long> add(@Valid @RequestBody BodyPartAddDTO dto);
}