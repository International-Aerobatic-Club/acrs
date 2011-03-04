<?php
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");

function doUpdateSession($db_conn, $record, $sessID)
{
    $update = "update session set ";
    $update .= 'practiceDate= ' . dateSQL($record['practiceDate'] . '/' . $record['year']) . ',';
    $update .= 'startTime= ' . timeSQL($record['startTime']) . ',';
    $update .= 'endTime= ' . timeSQL($record['endTime']) . ',';
    $update .= 'maxSlotsPer= ' . intSQL($record['maxSlotsPer']) . ',';
    $update .= 'minutesPer= ' . intSQL($record['minutesPer']);
    $update .= " where sessID=" . $sessID . ";";
    //debug($update);
    $fail = dbExec($db_conn, $update);
}

function doAddSession($db_conn, $record, & $sessID)
{
    $update = 'insert into session (ctstID, practiceDate, startTime, endTime,' .
    'maxSlotsPer, minutesPer' .
    ') values (';
    $update .= intSQL($record['ctstID']) . ',';
    $update .= dateSQL($record['practiceDate'] . '/' . $record['year']) . ',';
    $update .= timeSQL($record['startTime']) . ',';
    $update .= timeSQL($record['endTime']) . ',';
    $update .= intSQL($record['maxSlotsPer']) . ',';
    $update .= intSQL($record['minutesPer']);
    $update .= ');';
    //debug($update);
    $fail = dbExec($db_conn, $update);
    if ($fail == '')
    {
        $sessID = dbLastID();
    }
    return $fail;
}

function insertOrUpdateSession($db_conn, $record, & $sessID)
{
    if (isSet ($sessID) && $sessID != null && $sessID != '')
    {
        $fail = doUpdateSession($db_conn, $record, $sessID);
    } else
    {
        $fail = doAddSession($db_conn, $record, $sessID);
    }
    return $fail;
}

function doRetrieveSession($db_conn, & $record, $query)
{
    $fail = '';
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail .= "<it>" . dbErrorText() . "<\it>";
    } else
        if (dbCountResult($result) != 0)
        {
            $row = dbFetchAssoc($result);
            foreach ($row as $key => $value)
            {
                $record[$key] = stripslashes($value);
            }
            $date = strtotime($record['practiceDate']);
            $record['practiceDate'] = strftime('%m/%d', $date);
            $time = strtotime($record['startTime']);
            $record['startTime'] = strftime('%H:%M', $time);
            $time = strtotime($record['endTime']);
            $record['endTime'] = strftime('%H:%M', $time);
        } else
        {
            $fail = "no entry for " . $query;
        }

    return $fail;
}

function retrieveSession($db_conn, & $record, $sessID)
{
    $query = "select * from session where sessID = " . intSQL($sessID);
    return doRetrieveSession($db_conn, $record, $query);
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
