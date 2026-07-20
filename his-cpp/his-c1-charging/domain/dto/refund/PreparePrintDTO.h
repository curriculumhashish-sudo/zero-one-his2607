#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _PREPARE_PRINT_DTO_
#define _PREPARE_PRINT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// ==================== Print Column DTO ====================
// Column definition inside a table-type print element
class PrintColumnDTO : public oatpp::DTO
{
    DTO_INIT(PrintColumnDTO, DTO);

    DTO_FIELD(String, align);
    DTO_FIELD(String, field);
    DTO_FIELD(Boolean, fixed);
    DTO_FIELD(String, title);
    DTO_FIELD(Float64, width);
    DTO_FIELD(Boolean, checked);
    DTO_FIELD(Int32, colspan);
    DTO_FIELD(Int32, rowspan);
    DTO_FIELD(String, columnId);
};

// ==================== Print Element Type DTO ====================
// Descriptor for the type of a print element (text / hline / table)
class PrintElementTypeDTO : public oatpp::DTO
{
    DTO_INIT(PrintElementTypeDTO, DTO);

    DTO_FIELD(String, type);
    DTO_FIELD(String, title);
    // Table-type extra flags
    DTO_FIELD(Boolean, editable);
    DTO_FIELD(Boolean, columnResizable);
    DTO_FIELD(Boolean, isEnableDeleteRow);
    DTO_FIELD(Boolean, isEnableEditField);
    DTO_FIELD(Boolean, isEnableInsertRow);
    DTO_FIELD(Boolean, isEnableMergeCell);
    DTO_FIELD(Boolean, columnAlignEditable);
    DTO_FIELD(Boolean, columnTitleEditable);
    DTO_FIELD(Boolean, isEnableContextMenu);
    DTO_FIELD(Boolean, isEnableDeleteColumn);
    DTO_FIELD(Boolean, isEnableInsertColumn);
    DTO_FIELD(Boolean, columnDisplayEditable);
    DTO_FIELD(Boolean, columnDisplayIndexEditable);
};

// ==================== Print Element Options DTO ====================
// Union of all option fields across text / hline / table element types.
// Unused fields are left unset (null / omitted in JSON) per element type.
class PrintElementOptionsDTO : public oatpp::DTO
{
    DTO_INIT(PrintElementOptionsDTO, DTO);

    // Common layout
    DTO_FIELD(Float64, top);
    DTO_FIELD(Float64, left);
    DTO_FIELD(Float64, width);
    DTO_FIELD(Float64, height);

    // Text-type fields
    DTO_FIELD(String, field);
    DTO_FIELD(String, title);
    DTO_FIELD(Float64, fontSize);
    DTO_FIELD(Boolean, hideTitle);
    DTO_FIELD(String, textAlign);
    DTO_FIELD(String, fontWeight);

    // Hline-type fields
    DTO_FIELD(Float64, borderWidth);

    // Table-type fields
    DTO_FIELD(oatpp::List<oatpp::List<PrintColumnDTO::Wrapper>>, columns);
    DTO_FIELD(Float64, lineHeight);
    DTO_FIELD(String, tableBorder);
    DTO_FIELD(String, tableHeaderBorder);
    DTO_FIELD(String, tableHeaderRepeat);
    DTO_FIELD(String, tableBodyRowBorder);
    DTO_FIELD(Float64, tableBodyRowHeight);
    DTO_FIELD(String, tableBodyCellBorder);
    DTO_FIELD(Float64, tableHeaderRowHeight);
    DTO_FIELD(String, tableHeaderCellBorder);
};

// ==================== Print Element DTO ====================
// A single print element (text / hline / table)
class PrintElementDTO : public oatpp::DTO
{
    DTO_INIT(PrintElementDTO, DTO);

    DTO_FIELD(Object<PrintElementOptionsDTO>, options);
    DTO_FIELD(Object<PrintElementTypeDTO>, printElementType);
};

// ==================== Watermark Options DTO ====================
class WatermarkOptionsDTO : public oatpp::DTO
{
    DTO_INIT(WatermarkOptionsDTO, DTO);

    DTO_FIELD(Float64, width);
    DTO_FIELD(String, format);
    DTO_FIELD(Float64, height);
    DTO_FIELD(Float64, rotate);
    DTO_FIELD(String, content);
    DTO_FIELD(String, fontSize);
    DTO_FIELD(String, fillStyle);
    DTO_FIELD(Boolean, timestamp);
};

// ==================== Panel Layout Options DTO ====================
class PanelLayoutOptionsDTO : public oatpp::DTO
{
    DTO_INIT(PanelLayoutOptionsDTO, DTO);

    DTO_FIELD(String, layoutType);
    DTO_FIELD(String, paperStyle);
    DTO_FIELD(Float64, paperWidth);
    DTO_FIELD(Float64, paperHeight);
    DTO_FIELD(Float64, layoutRowGap);
    DTO_FIELD(Float64, layoutColumnGap);
};

// ==================== Print Panel DTO ====================
// A single panel in the print template
class PrintPanelDTO : public oatpp::DTO
{
    DTO_INIT(PrintPanelDTO, DTO);

    DTO_FIELD(String, name);
    DTO_FIELD(Int32, index);
    DTO_FIELD(Int32, width);
    DTO_FIELD(Int32, height);
    DTO_FIELD(String, paperType);
    DTO_FIELD(Float64, paperFooter);
    DTO_FIELD(Float64, paperHeader);
    DTO_FIELD(oatpp::List<PrintElementDTO::Wrapper>, printElements);
    DTO_FIELD(Float64, paperNumberTop);
    DTO_FIELD(Float64, paperNumberLeft);
    DTO_FIELD(Object<WatermarkOptionsDTO>, watermarkOptions);
    DTO_FIELD(Object<PanelLayoutOptionsDTO>, panelLayoutOptions);
    DTO_FIELD(Boolean, paperNumberContinue);
    DTO_FIELD(Boolean, paperNumberDisabled);
};

// ==================== PrintJson DTO ====================
// Top-level print template
class PrintJsonDTO : public oatpp::DTO
{
    DTO_INIT(PrintJsonDTO, DTO);

    DTO_FIELD(oatpp::List<PrintPanelDTO::Wrapper>, panels);
};

// ==================== PreparePrint Response DTO ====================
// Response data for preparePrint endpoint
class PreparePrintDTO : public oatpp::DTO
{
    DTO_INIT(PreparePrintDTO, DTO);

    DTO_FIELD(String, traceId);
    DTO_FIELD(String, busNo);
    DTO_FIELD(String, scenarioCode);
    DTO_FIELD(Int32, templateVersion);
    DTO_FIELD(String, samplePrintData);
    DTO_FIELD(Object<PrintJsonDTO>, PrintJson);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PREPARE_PRINT_DTO_
