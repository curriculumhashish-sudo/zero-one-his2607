package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;

import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

/**
 * 部位管理 Mapper
 */
@Mapper
public interface BodyPartMapper extends BaseMapper<Object> {

    List<BodyPartTreeVO> selectTree();

    List<BodyPartListVO> selectList(BodyPartPageQuery query);
}