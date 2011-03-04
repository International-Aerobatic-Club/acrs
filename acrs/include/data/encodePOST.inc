<?php

/**
The methods provided here help translate SQL query results into POST data format.
The query does most of the work, provided the POST data field names match the 
database table column names.
*/

/**
Add an dataFieldName.key='on' entry to the POST data for each comma 
separated key in an SQL set type result.
set: set type value from sql query, a string of comma separated keys.
dataFieldName: name of the set type (check boxes) data field
postData: reference to post data
*/
function sqlSetValueToPostData($set, $dataFieldName, & $postData)
{
    $members = explode(",", $set);
    foreach ($members as $key)
    {
        $postData[$dataFieldName . '_' . $key] = 'on';
    }
}

/**
Add an inputFieldName='on' entry to the POST data if dataValue is 'y'.
Also add an inputFieldName_yes='on' or inputFieldName_no='on' entry to
the POST data.
dataValue: 'y' or 'n' boolean value from the SQL query result
inputFieldName: name of the on/off (checkbox) POST data field
postData: reference to post data
*/
function sqlBoolValueToPostData($dataValue, $inputFieldName, & $postData)
{
    if (strcmp($dataValue, 'y') == 0)
    {
        $postData[$inputFieldName] = 'yes';
    }
    else
    {
        $postData[$inputFieldName] = 'no';
    }
}

/**
 * Convert 'y' or 'n' te boolean true or false
 */
function sqlIsTrue($dataValue)
{
    return (strcmp($dataValue,'y') == 0 || strcmp($dataValue, 'yes') == 0);
}

/**
 * Replace 'null' with a default value string
 * dataValue sql data value
 * postData reference to post data
 * fieldName: name of the POST data field
 * default: value with which to replace null
 * returns true if the field was null
 */
function sqlCheckNullDefault($dataValue, $fieldName, & $postData, $default)
{
    $wasNull = false;
    if (!isset ($dataValue) || $dataValue == null || $dataValue == 'null')
    {
        $postData[$fieldName] = $default;
        $wasNull = true;
    }
    else
    {
        $postData[$fieldName] = $dataValue;
    }
    return $wasNull;
}

/**
 * Format date field according to date format, empty if null
 * dataValue sql representation of date
 * inputFieldName name of data field in post data
 * postData to modify fieldName entry
 * format of date according to strftime 
 */
function sqlDateValueToPostData($dataValue, $fieldName, &$postData, $format)
{
    if (!sqlCheckNullDefault($dataValue, $fieldName, $postData, ''))
    {
        $date = strtotime($dataValue);
        $postData[$fieldName] = strftime($format, $date);
    }
}
?>
<?php
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
