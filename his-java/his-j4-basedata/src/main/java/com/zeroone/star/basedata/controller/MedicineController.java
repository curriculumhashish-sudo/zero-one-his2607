package com.zeroone.star.basedata.controller;


import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineUpdateDTO;
import com.zeroone.star.project.j4.basedata.MedicineApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineChangeStatusVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDeleteVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.extern.slf4j.Slf4j;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：药品管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/medicine")
@Tag(name = "basedata-medicine", description = "药品管理接口")
@Validated
@Slf4j
public class MedicineController implements MedicineApis {

    @PutMapping("/update-medicine")
    @Operation(summary = "修改药品")
    public JsonVO<String> updateMedicine(@Validated @RequestBody MedicineUpdateDTO dto){
        return JsonVO.success("修改成功");
    }


    @DeleteMapping("/delete-medicine")
    @Operation(summary = "删除药品（支持批量）")
    public JsonVO<MedicineDeleteVO> deleteMedicine(@Validated @RequestBody MedicineDeleteDTO dto){
        List<String> list = new ArrayList<>();
        list.add("1");
        return JsonVO.success(new MedicineDeleteVO(list));
    }

    @PutMapping("/change-status/{status}")
    @Operation(summary = "启/停用药品(支持批量)，status为1表示启用，为0表示停用")
    public JsonVO<MedicineChangeStatusVO> updateEnableMedicineStatus(
            @PathVariable int status,
            @Validated @RequestBody MedicineChangeStatusDTO dto
    ){
        List<String> list = dto.getIds();
        //判断状态进行启用或者提示已启用，无需重复启用
        if(status == 0) {
            //要启用药品
            log.debug("启用药品成功");
            return JsonVO.success(new MedicineChangeStatusVO(list));
        }else if (status == 1){
            //要停用药品
            log.error("停用药品成功");
            return JsonVO.success(new MedicineChangeStatusVO(list));
        }
            return JsonVO.fail("status值有误");
    }

}
