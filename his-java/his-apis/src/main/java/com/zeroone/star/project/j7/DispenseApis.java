package com.zeroone.star.project.j7;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientRefundConfirmDTO;
import com.zeroone.star.project.query.j7.dispense.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.*;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.List;

public interface DispenseApis {




    /**
     * <p>
     * 描述：住院发药管理 API 接口
     * </p>
     * <p>版权：&copy;01星球</p>
     * <p>地址：01星球总部</p>
     * @author zg
     * @version 1.0.0
     */


        /**
         * 获取汇总单队列（支持条件筛选与分页）
         * @param query 查询条件
         * @return 分页的汇总单列表
         */
        JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(DispensingSummaryQuery query);

        /**
         * 获取患者药品项目（点击汇总单后查看明细）
         * @param summaryNo 汇总单编号
         * @return 患者药品项目列表
         */
        JsonVO<List<PatientMedicationVO>> queryPatientMedications(String summaryNo);

        /**
         * 核对发药（确认发药并扣减库存）
         * @param verifyDTO 核对发药请求
         * @return 核对发药结果
         */
        JsonVO<String> verifyDispense(DispenseVerifyDTO verifyDTO);








    @Tag(name = "住院发退药记录")
    @RequestMapping("/pharmacy/dispense-record/inpatient")

        @Operation(summary = "分页查询住院发退药记录")
        @PostMapping("/page")
        JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(
                @Parameter(description = "分页及筛选条件", required = true)
                @Valid @RequestBody InpatientDispenseRecordQuery query
        );

        @Operation(summary = "按当前筛选条件导出住院发退药记录")
        @GetMapping("/export")
        void export(
                @Parameter(description = "筛选条件，不使用分页参数")
                @Valid @ModelAttribute InpatientDispenseRecordQuery query,
                HttpServletResponse response
        ) throws IOException;



    /**
     * <p>
     * 描述：住院退药接口（对齐原型：初始化、患者队列、退药明细、确认退药）
     * </p>
     * <p>版权：&copy;01星球</p>
     * <p>地址：01星球总部</p>
     * @author jacobzp
     * @version 1.0.0
     */

        /** 加载页面初始化选项 */
        JsonVO<InpatientRefundInitVO> loadInit();

        /** 分页查询退药患者队列 */
        JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(InpatientRefundPatientQuery query);

        /** 获取患者退药明细 */
        JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(InpatientRefundDetailQuery query);

        /** 确认退药（当前阶段假数据） */
        JsonVO<InpatientRefundConfirmVO> confirmReturn(InpatientRefundConfirmDTO dto);










    @Tag(name = "门诊发退药记录")


        @Operation(summary = "分页查询发退药记录（按处方分组）")
        JsonVO<PageDTO<DispenseGroupVO>> queryPage(
                @Parameter(description = "查询参数", required = true) DispenseQuery query
        );

        @Operation(summary = "导出发退药记录（平铺格式）")
        void export(
                @Parameter(description = "查询参数") DispenseQuery query,
                HttpServletResponse response
        ) throws IOException;













    /**
     * 门诊发药模块接口
     */
    @Tag(name = "门诊发药", description = "门诊发药相关接口（患者队列、处方药品查询）")
    @Validated


        /**
         * 分页查询患者队列
         */
        @Operation(
                summary = "分页查询患者队列",
                description = "根据姓名、状态、机构等条件分页查询候诊/发药患者队列"
        )
        @GetMapping("/api/j7/outpharm/patient-queue")
        JsonVO<IPage<PatientQueueVO>> pagePatientQueue(
                @Parameter(hidden = true)
                PatientQueueQuery query
        );

        /**
         * 分页查询患者药品项目（处方明细 + 药品信息）
         */
        @Operation(
                summary = "分页查询患者药品项目",
                description = "根据患者ID、就诊ID、处方状态等条件查询患者处方药品明细"
        )
        @GetMapping("/api/j7/outpharm/patient-drug-items")
        JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(
                @Parameter(hidden = true)
                PatientDrugItemQuery query
        );







}
