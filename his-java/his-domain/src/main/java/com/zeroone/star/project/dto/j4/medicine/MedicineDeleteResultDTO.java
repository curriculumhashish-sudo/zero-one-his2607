package com.zeroone.star.project.dto.j4.medicine;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class MedicineDeleteResultDTO {
    private int successCount;
    private int failCount;
    private List<String> failIds;
}
