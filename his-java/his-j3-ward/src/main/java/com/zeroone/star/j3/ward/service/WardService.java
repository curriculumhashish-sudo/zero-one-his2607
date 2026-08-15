package com.zeroone.star.j3.ward.service;

import com.zeroone.star.project.dto.j3.ward.BedAddDTO;
import com.zeroone.star.project.dto.j3.ward.BedDTO;
import com.zeroone.star.project.dto.j3.ward.RoomAddDTO;
import com.zeroone.star.project.dto.j3.ward.RoomDTO;
import com.zeroone.star.project.dto.j3.ward.WardAddDTO;
import com.zeroone.star.project.dto.j3.ward.WardDTO;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.j3.ward.BedOptionVO;
import com.zeroone.star.project.vo.j3.ward.OptionVO;
import com.zeroone.star.project.vo.j3.ward.WardTreeVO;

import java.util.List;

/**
 * <p>
 * 描述：病区病房管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
public interface WardService {

    /**
     * 获取病区-病房-床位树形列表
     *
     * @param query 树形查询条件
     * @return 树形结构数据
     */
    List<WardTreeVO> getWardTree(WardTreeQuery query);

    /**
     * 获取病区列表（用于下拉框）
     *
     * @return 病区下拉选项列表
     */
    List<OptionVO> getWardList();

    /**
     * 添加病区
     *
     * @param dto 病区添加数据
     */
    void addWard(WardAddDTO dto);

    /**
     * 修改病区
     *
     * @param id  病区ID
     * @param dto 病区数据
     */
    void updateWard(String id, WardDTO dto);

    /**
     * 删除病区
     *
     * @param id 病区ID
     */
    void deleteWard(String id);

    /**
     * 根据病区获取病房列表（用于下拉框）
     *
     * @param wardId 病区ID
     * @return 病房下拉选项列表
     */
    List<OptionVO> getRoomListByWard(String wardId);

    /**
     * 添加病房
     *
     * @param dto 病房添加数据
     */
    void addRoom(RoomAddDTO dto);

    /**
     * 修改病房
     *
     * @param id  病房ID
     * @param dto 病房数据
     */
    void updateRoom(String id, RoomDTO dto);

    /**
     * 删除病房
     *
     * @param id 病房ID
     */
    void deleteRoom(String id);

    /**
     * 根据病房获取病床列表
     *
     * @param roomId 病房ID
     * @return 病床列表
     */
    List<BedOptionVO> getBedListByRoom(String roomId);

    /**
     * 添加床位
     *
     * @param dto 床位添加数据
     */
    void addBed(BedAddDTO dto);

    /**
     * 修改床位
     *
     * @param id  床位ID
     * @param dto 床位数据
     */
    void updateBed(String id, BedDTO dto);

    /**
     * 删除床位
     *
     * @param id 床位ID
     */
    void deleteBed(String id);
}
