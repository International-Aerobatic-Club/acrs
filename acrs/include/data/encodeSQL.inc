<?php


/**
Functions convert POST data into safe and suitable values for injection into
SQL queries.
*/

/**
Trims input of any leading and trailing space.
Crops input to given maximum length
*/
function crop($input, $maxlength)
{
    $input = trim($input);
    $input = substr($input, 0, $maxlength);
    return $input;
}

/**
Convert string into sql sanitized and sized data string within single quotes
string: to convert
maxlen: character field length
return quoted, escaped string, with maximum length observed.
return the string, "null" if the provided string is zero length after trimming white space.
*/
function strSQL($string, $maxlen)
{
    $strval = crop($string, $maxlen);
    if (strlen($strval) == 0)
    {
        $strval = "null";
    }
    else
    {
        $strval = "'" . addslashes($strval) . "'";
    }
    return $strval;
}


function strSQLTest($fieldName, $string, $maxlen)
{
  $test = $fieldName;
  $strVal = strSQL($string, $maxlen);
  if ($strVal == 'null')
  {
    $test .= ' is null';
  }
  else
  {
    $test .= ' = ' . $strVal;
  }
  return $test;
}

/**
Convert selection of a set of posted values into comma-separated list
postData: associative array in post data format
dataFieldName: name of data field/column of the set
setMembers: array of string values that may be checked in the postData
returns data string of comma separated items from setMembers where
$postData[dataFieldName_item] == 'on'
Note: the returned string does not have added slashes to make it sql safe.
*/
function selectionSet($postData, $dataFieldName, $setMembers)
{
    $sqlset = '';
    $first = true;
    foreach ($setMembers as $item)
    {
        $postField = $dataFieldName . '_' . $item;
        if (isSet ($postData[$postField]) && strcmp(strtolower($postData[$postField]), "on") == 0)
        {
            if (!$first)
                $sqlset .= ",";
            else
                $first = false;
            $sqlset .= addslashes($item);
        }
    }
    return $sqlset;
}

/**
Convert selection of a set of value into sql form
postData: associative array in post data format
dataFieldName: name of data field/column of the set
setMembers: array of string values that may be checked in the postData
returns sql data string (goes between the '=' and the ',')
the string consists of comma separated items from setMembers where
$postData[dataFieldName.item] == 'On'
*/
function selectionSQL($postData, $dataFieldName, $setMembers)
{
    //debug('encodeSQL.selectionSQL check data field name ' . $dataFieldName);
    //debugArr('for members ',$setMembers);
    return "'" . selectionSet($postData, $dataFieldName, $setMembers) . "'";
}

/**
Convert string into an unsigned integer by stripping any non-digit characters.
value: a string or integer
Return a string representation of the integer, or "null" if there are no numerals.
*/
function intSQL($value)
{
    $sqlint = strval($value);
    $sqlint = preg_replace("/[^0-9]/", "", $sqlint);
    if (strlen($sqlint) == 0)
    {
        $sqlint = "null";
    }
    return $sqlint;
}

/**
Encode enumerated value as safe string.
enum: value from a radio button
enumSet: list of valid enum values
return quoted, escaped string
*/
function enumSQL($enum, $enumSet)
{
    $value = "null";
    $prop = trim($enum);
    if (strlen($prop) > 0)
    {
        foreach ($enumSet as $item)
            if (strcmp($item, $prop) == 0)
                $value = "'" . addslashes($prop) . "'";
    }
    return $value;
}

/**
Encode checked value as 'y' or 'n' enum value
return 'y' or 'n'
*/
function boolSQL($postData, $fieldName)
{
    $boolV = "'n'";
    if (isSet ($postData[$fieldName]) && (strcmp(strtolower($postData[$fieldName]), "on") == 0 || strcmp(strtolower($postData[$fieldName]), "yes") == 0))
    {
        $boolV = "'y'";
    }
    return $boolV;
}

/**
Encode date entered mm/dd/yyyy as sql date.
Will work for some other GNU date formats as well.
See strtotime($dateTime);
*/
function dateSQL($postDate)
{
    if (!isSet ($postDate) || strlen(trim($postDate)) < 6)
    {
        $dateStr = 'null';
    }
    else
    {
        $ts = strtotime($postDate);
        $da = getdate($ts);
        $dateStr = "'" . $da['year'] . '-' . $da['mon'] . '-' . $da['mday'] . "'";
    }
    return $dateStr;
}

/**
Encode time entered hh:mm as sql time.
Will work for some other GNU time formats as well.
See strtotime($dateTime);
*/
function timeSQL($postTime)
{
    if (!isSet ($postTime) || strlen(trim($postTime)) < 5)
    {
        $timeStr = 'null';
    }
    else
    {
        $ts = strtotime($postTime);
        $timeStr = "'" . strftime('%H:%M:%S', $ts) . "'";
    }
    return $timeStr;
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
