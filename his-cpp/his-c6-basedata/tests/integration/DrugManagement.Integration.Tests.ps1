[CmdletBinding()]
param(
    [string]$BaseUrl = 'http://127.0.0.1:8092',
    [string]$DbHost = '127.0.0.1',
    [int]$DbPort = 3306,
    [string]$DbName = 'zo_his',
    [string]$DbUser = 'root',
    [string]$DbPassword = $env:ZO_HIS_DB_PASSWORD,
    [string]$MySqlExe = 'D:\xtCode\DB\MySQL\mysql-9.7.1-winx64\bin\mysql.exe',
    [string]$JwtExe = '',
    [switch]$KeepFinalState,
    [switch]$ResetOnly
)

$ErrorActionPreference = 'Stop'
$script:Passed = 0
$script:Failed = 0
$script:Results = [System.Collections.Generic.List[object]]::new()

$moduleRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$cppRoot = (Resolve-Path (Join-Path $moduleRoot '..')).Path
if ([string]::IsNullOrWhiteSpace($JwtExe)) {
    $JwtExe = Join-Path $cppRoot 'out\build\x64-Debug\arch-tests\bin\test-jwt.exe'
}

$fixtureIds = @(
    '990000000000000001',
    '990000000000000003',
    '990000000000000004',
    '990000000000000007',
    '990000000000000008',
    '990000000000000009'
)
$sourceDrugId = '322994171216400384'
$modifyId = $fixtureIds[0]
$stateIds = @($fixtureIds[1], $fixtureIds[2])
$deleteIds = @($fixtureIds[3], $fixtureIds[4])
$crossTenantId = $fixtureIds[5]
$fixtureIdSql = ($fixtureIds | ForEach-Object { "'$_'" }) -join ','

function Assert-Condition {
    param(
        [string]$Name,
        [bool]$Condition,
        [string]$Detail
    )

    if ($Condition) {
        $script:Passed++
        $script:Results.Add([pscustomobject]@{ name = $Name; passed = $true; detail = $Detail })
        Write-Host "[PASS] $Name - $Detail" -ForegroundColor Green
    }
    else {
        $script:Failed++
        $script:Results.Add([pscustomobject]@{ name = $Name; passed = $false; detail = $Detail })
        Write-Host "[FAIL] $Name - $Detail" -ForegroundColor Red
    }
}

function Invoke-Database {
    param([string]$Sql)

    $previousPassword = $env:MYSQL_PWD
    try {
        $env:MYSQL_PWD = $DbPassword
        $output = & $MySqlExe `
            "--host=$DbHost" `
            "--port=$DbPort" `
            "--user=$DbUser" `
            "--database=$DbName" `
            '--batch' `
            '--raw' `
            '--skip-column-names' `
            '--execute' $Sql 2>&1
        if ($LASTEXITCODE -ne 0) {
            throw "MySQL command failed: $($output -join [Environment]::NewLine)"
        }
        return @($output)
    }
    finally {
        if ($null -eq $previousPassword) {
            Remove-Item Env:MYSQL_PWD -ErrorAction SilentlyContinue
        }
        else {
            $env:MYSQL_PWD = $previousPassword
        }
    }
}

function Get-DatabaseScalar {
    param([string]$Sql)

    $rows = @(Invoke-Database $Sql)
    if ($rows.Count -eq 0) {
        return ''
    }
    return [string]$rows[0]
}

function Get-TestToken {
    if (-not (Test-Path -LiteralPath $JwtExe)) {
        throw "JWT test utility was not found: $JwtExe"
    }
    $output = (& $JwtExe 2>&1) -join [Environment]::NewLine
    $match = [regex]::Match($output, 'eyJ[A-Za-z0-9_-]+\.[A-Za-z0-9_-]+\.[A-Za-z0-9_-]+')
    if (-not $match.Success) {
        throw 'No RSA JWT was found in test-jwt output.'
    }
    return $match.Value
}

function Invoke-DrugApi {
    param(
        [ValidateSet('PUT', 'DELETE')]
        [string]$Method,
        [string]$Path,
        [object]$Body,
        [string]$Token
    )

    $json = $Body | ConvertTo-Json -Depth 20 -Compress
    return Invoke-RestMethod `
        -Method $Method `
        -Uri "$BaseUrl$Path" `
        -Headers @{ Authorization = "Bearer $Token" } `
        -ContentType 'application/json; charset=utf-8' `
        -Body ([System.Text.Encoding]::UTF8.GetBytes($json)) `
        -TimeoutSec 15
}

function Copy-Object {
    param([object]$Value)
    return $Value | ConvertTo-Json -Depth 20 | ConvertFrom-Json
}

function Reset-Fixtures {
    $sourceCount = [int](Get-DatabaseScalar "SELECT COUNT(*) FROM med_medication_definition WHERE id='$sourceDrugId' AND is_deleted=0;")
    if ($sourceCount -ne 1) {
        throw "Fixture source drug $sourceDrugId does not exist in $DbName."
    }

    $drugColumns = Get-DatabaseScalar @"
SET SESSION group_concat_max_len=65535;
SELECT GROUP_CONCAT(COLUMN_NAME ORDER BY ORDINAL_POSITION SEPARATOR ',')
FROM information_schema.columns
WHERE table_schema=DATABASE() AND table_name='med_medication_definition' AND column_name<>'id';
"@
    $drugAssignments = Get-DatabaseScalar @"
SET SESSION group_concat_max_len=65535;
SELECT GROUP_CONCAT(CONCAT('t.',COLUMN_NAME,'=s.',COLUMN_NAME) ORDER BY ORDINAL_POSITION SEPARATOR ',')
FROM information_schema.columns
WHERE table_schema=DATABASE() AND table_name='med_medication_definition' AND column_name<>'id';
"@
    $priceColumns = Get-DatabaseScalar @"
SET SESSION group_concat_max_len=65535;
SELECT GROUP_CONCAT(COLUMN_NAME ORDER BY ORDINAL_POSITION SEPARATOR ',')
FROM information_schema.columns
WHERE table_schema=DATABASE() AND table_name='adm_charge_item_definition'
  AND column_name NOT IN ('id','instance_id');
"@

    foreach ($id in $fixtureIds) {
        Invoke-Database @"
INSERT INTO med_medication_definition (id,$drugColumns)
SELECT '$id',$drugColumns
FROM med_medication_definition
WHERE id='$sourceDrugId'
  AND NOT EXISTS (SELECT 1 FROM med_medication_definition WHERE id='$id');
UPDATE med_medication_definition t
JOIN med_medication_definition s ON s.id='$sourceDrugId'
SET $drugAssignments
WHERE t.id='$id';
"@ | Out-Null
    }

    Invoke-Database @"
UPDATE med_medication_definition
SET tenant_id='1',name='UT-MODIFY',status_enum='ACTIVE',active_flag=1,is_deleted=0,
    deleted_at=NULL,deleted_by=NULL,version=0
WHERE id='$modifyId';
UPDATE med_medication_definition
SET tenant_id='1',name=CONCAT('UT-STATE-',RIGHT(id,1)),status_enum='ACTIVE',active_flag=1,is_deleted=0,
    deleted_at=NULL,deleted_by=NULL,version=0
WHERE id IN ('$($stateIds[0])','$($stateIds[1])');
UPDATE med_medication_definition
SET tenant_id='1',name=CONCAT('UT-DELETE-',RIGHT(id,1)),status_enum='INACTIVE',active_flag=0,is_deleted=0,
    deleted_at=NULL,deleted_by=NULL,version=0
WHERE id IN ('$($deleteIds[0])','$($deleteIds[1])');
UPDATE med_medication_definition
SET tenant_id='2',name='UT-CROSS-TENANT',status_enum='ACTIVE',active_flag=1,is_deleted=0,
    deleted_at=NULL,deleted_by=NULL,version=0
WHERE id='$crossTenantId';
DELETE FROM adm_charge_item_definition WHERE instance_id IN ($fixtureIdSql);
"@ | Out-Null

    foreach ($id in $fixtureIds) {
        Invoke-Database @"
INSERT INTO adm_charge_item_definition (id,instance_id,$priceColumns)
SELECT REPLACE(UUID(),'-',''),'$id',$priceColumns
FROM adm_charge_item_definition
WHERE instance_table='med_medication_definition' AND instance_id='$sourceDrugId' AND is_deleted=0;
UPDATE adm_charge_item_definition p
JOIN med_medication_definition d ON d.id=p.instance_id
SET p.tenant_id=d.tenant_id
WHERE p.instance_id='$id';
"@ | Out-Null
    }

    Invoke-Database @"
DELETE FROM med_medication_request WHERE medication_id IN ($fixtureIdSql);
DELETE FROM med_medication_dispense WHERE medication_id IN ($fixtureIdSql);
DELETE FROM cln_allergy_intolerance WHERE medication_id IN ($fixtureIdSql);
DELETE FROM cus_order_template_line WHERE medication_definition_id IN ($fixtureIdSql);
DELETE FROM wkf_inventory_item
WHERE item_table='med_medication_definition' AND item_id IN ($fixtureIdSql);
"@ | Out-Null
}

function Write-Results {
    $runId = Get-Date -Format 'yyyyMMdd-HHmmss'
    $resultDir = Join-Path $env:TEMP "zero-one-his2607\drug-management-it\$runId"
    New-Item -ItemType Directory -Path $resultDir -Force | Out-Null
    $resultFile = Join-Path $resultDir 'results.json'
    [pscustomobject]@{
        branch = (git -C (Resolve-Path (Join-Path $cppRoot '..')).Path branch --show-current)
        head = (git -C (Resolve-Path (Join-Path $cppRoot '..')).Path rev-parse HEAD)
        database = $DbName
        baseUrl = $BaseUrl
        passed = $script:Passed
        failed = $script:Failed
        total = $script:Passed + $script:Failed
        cases = $script:Results
    } | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath $resultFile -Encoding UTF8
    Write-Host "Result file: $resultFile"
}

if ([string]::IsNullOrWhiteSpace($DbPassword)) {
    throw 'Set ZO_HIS_DB_PASSWORD or pass -DbPassword before running the integration test.'
}
if (-not (Test-Path -LiteralPath $MySqlExe)) {
    throw "mysql.exe was not found: $MySqlExe"
}
if ($DbName -ne 'zo_his') {
    throw "This fixture script is guarded for the local development database zo_his; actual value: $DbName"
}

$token = ''
$fixturesPrepared = $false
try {
    $databaseName = Get-DatabaseScalar 'SELECT DATABASE();'
    Assert-Condition 'Database selection' ($databaseName -eq 'zo_his') "database=$databaseName"

    Reset-Fixtures
    $fixturesPrepared = $true
    if ($ResetOnly) {
        Write-Host 'Fixture reset completed.'
        return
    }

    $openApi = Invoke-RestMethod -Uri "$BaseUrl/api-docs/oas-3.0.0.json" -TimeoutSec 10
    Assert-Condition 'OpenAPI reachable' ($null -ne $openApi.paths) "$BaseUrl/api-docs/oas-3.0.0.json"
    Assert-Condition 'Modify method and operationId' `
        ($openApi.paths.'/drug/modify-drug'.put.operationId -eq 'modifyDrug') 'PUT modifyDrug'
    Assert-Condition 'Remove method and operationId' `
        ($openApi.paths.'/drug/remove-drug'.delete.operationId -eq 'removeDrug') 'DELETE removeDrug'
    Assert-Condition 'Enable-disable method and operationId' `
        ($openApi.paths.'/drug/enable-disable-drug'.put.operationId -eq 'enableDisableDrug') 'PUT enableDisableDrug'

    $modifySchema = $openApi.components.schemas.DrugModifyDTO
    $resultSchema = $openApi.components.schemas.DrugModifyResultDTO
    Assert-Condition 'DrugModifyDTO schema' `
        ((@($modifySchema.properties.psobject.Properties).Count -eq 71) -and (@($modifySchema.required).Count -eq 23)) `
        '71 properties / 23 required'
    Assert-Condition 'DrugModifyResultDTO schema' `
        ((@($resultSchema.properties.psobject.Properties).Count -eq 88) -and (@($resultSchema.required).Count -eq 25)) `
        '88 properties / 25 required'

    $token = Get-TestToken
    $modifyExample = $openApi.paths.'/drug/modify-drug'.put.requestBody.content.'application/json'.examples.default.value

    $invalidModify = Copy-Object $modifyExample
    $invalidModify.id = $modifyId
    $invalidModify.retailPrice = 10
    $invalidModify.maximumRetailPrice = 9
    $nameBefore = Get-DatabaseScalar "SELECT name FROM med_medication_definition WHERE id='$modifyId';"
    $invalidResponse = Invoke-DrugApi PUT '/drug/modify-drug' $invalidModify $token
    $nameAfter = Get-DatabaseScalar "SELECT name FROM med_medication_definition WHERE id='$modifyId';"
    Assert-Condition 'Reject invalid maximum retail price' `
        (($invalidResponse.code -ne 10000) -and ($nameBefore -eq $nameAfter)) "code=$($invalidResponse.code), row unchanged"

    $validModify = Copy-Object $modifyExample
    $validModify.id = $modifyId
    $validModify.name = 'UT-MODIFIED'
    $validModify.purchasePrice = 7.25
    $validModify.retailPrice = 8.50
    $validModify.maximumRetailPrice = 9.00
    $lotPriceCountBefore = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM adm_charge_item_definition
WHERE instance_id='$modifyId' AND lot_no IS NOT NULL AND is_deleted=0;
"@)
    $modifyResponse = Invoke-DrugApi PUT '/drug/modify-drug' $validModify $token
    Assert-Condition 'Modify API success' `
        (($modifyResponse.code -eq 10000) -and ($modifyResponse.data.id -eq $modifyId)) "code=$($modifyResponse.code)"

    $modifiedRow = Get-DatabaseScalar @"
SELECT CONCAT(name,'|',version) FROM med_medication_definition WHERE id='$modifyId';
"@
    Assert-Condition 'Modify main row persisted' ($modifiedRow -eq 'UT-MODIFIED|1') $modifiedRow

    $basePriceCheck = Get-DatabaseScalar @"
SELECT CONCAT(
  SUM(price_type_code='PURCHASE' AND price_value=7.25),',',
  SUM(price_type_code='SALE' AND price_value=8.50),',',
  SUM(price_type_code='LIMIT' AND price_value=9.00))
FROM adm_charge_item_definition
WHERE instance_id='$modifyId' AND lot_no IS NULL AND is_deleted=0;
"@
    Assert-Condition 'Modify base prices persisted' ($basePriceCheck -eq '1,1,1') $basePriceCheck

    $lotPriceCountAfter = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM adm_charge_item_definition
WHERE instance_id='$modifyId' AND lot_no IS NOT NULL AND is_deleted=0;
"@)
    Assert-Condition 'Modify preserves lot prices' ($lotPriceCountAfter -eq $lotPriceCountBefore) `
        "before=$lotPriceCountBefore after=$lotPriceCountAfter"

    $disableBody = [pscustomobject]@{ ids = $stateIds; activeFlag = $false }
    $disableResponse = Invoke-DrugApi PUT '/drug/enable-disable-drug' $disableBody $token
    $disabledCount = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM med_medication_definition
WHERE id IN ('$($stateIds[0])','$($stateIds[1])') AND status_enum='INACTIVE' AND active_flag=0 AND is_deleted=0;
"@)
    Assert-Condition 'Batch disable' (($disableResponse.code -eq 10000) -and ($disabledCount -eq 2)) `
        "code=$($disableResponse.code), rows=$disabledCount"

    $repeatDisable = Invoke-DrugApi PUT '/drug/enable-disable-drug' $disableBody $token
    Assert-Condition 'Reject repeated disable' ($repeatDisable.code -ne 10000) "code=$($repeatDisable.code)"

    $enableBody = [pscustomobject]@{ ids = $stateIds; activeFlag = $true }
    $enableResponse = Invoke-DrugApi PUT '/drug/enable-disable-drug' $enableBody $token
    $enabledCount = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM med_medication_definition
WHERE id IN ('$($stateIds[0])','$($stateIds[1])') AND status_enum='ACTIVE' AND active_flag=1 AND is_deleted=0;
"@)
    Assert-Condition 'Batch enable' (($enableResponse.code -eq 10000) -and ($enabledCount -eq 2)) `
        "code=$($enableResponse.code), rows=$enabledCount"

    $missingMixedBody = [pscustomobject]@{ ids = @($stateIds[0], '990000000000009999'); activeFlag = $false }
    $missingMixedResponse = Invoke-DrugApi PUT '/drug/enable-disable-drug' $missingMixedBody $token
    $firstState = Get-DatabaseScalar "SELECT CONCAT(status_enum,'|',active_flag) FROM med_medication_definition WHERE id='$($stateIds[0])';"
    Assert-Condition 'Batch state rollback on missing ID' `
        (($missingMixedResponse.code -ne 10000) -and ($firstState -eq 'ACTIVE|1')) `
        "code=$($missingMixedResponse.code), state=$firstState"

    $crossTenantBody = [pscustomobject]@{ ids = @($stateIds[0], $crossTenantId); activeFlag = $false }
    $crossTenantResponse = Invoke-DrugApi PUT '/drug/enable-disable-drug' $crossTenantBody $token
    $crossTenantState = Get-DatabaseScalar @"
SELECT GROUP_CONCAT(CONCAT(id,':',status_enum,':',active_flag) ORDER BY id SEPARATOR '|')
FROM med_medication_definition WHERE id IN ('$($stateIds[0])','$crossTenantId');
"@
    Assert-Condition 'Reject cross-tenant batch' `
        (($crossTenantResponse.code -ne 10000) -and ($crossTenantState -like '*ACTIVE:1*')) `
        "code=$($crossTenantResponse.code), $crossTenantState"

    $tooManyIds = 1..101 | ForEach-Object { 'T' + $_.ToString('D3') }
    $tooManyResponse = Invoke-DrugApi PUT '/drug/enable-disable-drug' `
        ([pscustomobject]@{ ids = $tooManyIds; activeFlag = $false }) $token
    Assert-Condition 'Reject batch larger than 100' ($tooManyResponse.code -ne 10000) "code=$($tooManyResponse.code)"

    $activeDeleteResponse = Invoke-DrugApi DELETE '/drug/remove-drug' `
        ([pscustomobject]@{ ids = @($modifyId) }) $token
    $activeDeleteState = Get-DatabaseScalar "SELECT CONCAT(status_enum,'|',is_deleted) FROM med_medication_definition WHERE id='$modifyId';"
    Assert-Condition 'Reject deleting active drug' `
        (($activeDeleteResponse.code -ne 10000) -and ($activeDeleteState -eq 'ACTIVE|0')) `
        "code=$($activeDeleteResponse.code), state=$activeDeleteState"

    $deleteResponse = Invoke-DrugApi DELETE '/drug/remove-drug' `
        ([pscustomobject]@{ ids = $deleteIds }) $token
    $deletedCount = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM med_medication_definition
WHERE id IN ('$($deleteIds[0])','$($deleteIds[1])')
  AND status_enum='DISCARDED' AND active_flag=0 AND is_deleted=1 AND version=1;
"@)
    Assert-Condition 'Batch delete' (($deleteResponse.code -eq 10000) -and ($deletedCount -eq 2)) `
        "code=$($deleteResponse.code), rows=$deletedCount"

    $activeDeletedPriceCount = [int](Get-DatabaseScalar @"
SELECT COUNT(*) FROM adm_charge_item_definition
WHERE instance_id IN ('$($deleteIds[0])','$($deleteIds[1])') AND is_deleted=0;
"@)
    Assert-Condition 'Batch delete soft-deletes all prices' ($activeDeletedPriceCount -eq 0) `
        "active prices=$activeDeletedPriceCount"

    $repeatDeleteResponse = Invoke-DrugApi DELETE '/drug/remove-drug' `
        ([pscustomobject]@{ ids = $deleteIds }) $token
    Assert-Condition 'Reject repeated delete' ($repeatDeleteResponse.code -ne 10000) `
        "code=$($repeatDeleteResponse.code)"

    $emptyDeleteResponse = Invoke-DrugApi DELETE '/drug/remove-drug' `
        ([pscustomobject]@{ ids = @() }) $token
    Assert-Condition 'Reject empty delete list' ($emptyDeleteResponse.code -ne 10000) `
        "code=$($emptyDeleteResponse.code)"
}
finally {
    if ($fixturesPrepared -and -not $KeepFinalState -and -not $ResetOnly) {
        Reset-Fixtures
        Write-Host 'Fixtures were restored for the next run.'
    }
    if (-not $ResetOnly) {
        Write-Results
        Write-Host "RESULT passed=$script:Passed failed=$script:Failed total=$($script:Passed + $script:Failed)"
    }
}

if ($script:Failed -gt 0) {
    exit 1
}
