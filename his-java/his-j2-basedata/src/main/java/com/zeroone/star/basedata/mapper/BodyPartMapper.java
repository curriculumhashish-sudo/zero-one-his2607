package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.doo.BodyPartDO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

@Mapper
public interface BodyPartMapper extends BaseMapper<BodyPartDO> {

    List<BodyPartTreeVO> selectTree();

    Long selectListCount(@Param("query") BodyPartPageQuery query);

    List<BodyPartListVO> selectList(
            @Param("query") BodyPartPageQuery query,
            @Param("offset") int offset,
            @Param("limit") int limit
    );
}