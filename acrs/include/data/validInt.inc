<?php
/**
 * Validate integer field to maximum length and numbers only
 * Crop field to maxLength, enforce non-empty if required,
 * enforce only digits [0-9]
 * postData associative array
 * fieldName name of data field in postData
 * fieldDescr description for validation messages
 * maxLength maximum number of digits
 * required true if length cannot be zero
 * returns validation message, empty if valid
 */
function validInt(& $postData, $fieldName, $fieldDescr, $maxLength, $required)
{
    $fail = '';
    $postData[$fieldName] = crop($postData[$fieldName], $maxLength);
    if (strlen($postData[$fieldName]) == 0)
    {
        if ($required)
            $fail = 'Provide ' . $fieldName;
    }
    else
    {
        $intValue = preg_replace('/[^0-9]/', '', $postData[$fieldName]);
        if (strlen($postData[$fieldName]) != strlen($intValue))
        {
            $fail = $fieldName . ' must contain only numbers.';
        }
    }
    return $fail;
}
/*
   Copyright 2008 International Aerobatic Club, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
?>
