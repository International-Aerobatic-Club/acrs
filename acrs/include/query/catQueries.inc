<?php
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");

function doUpdateCat($db_conn, $record, $catID)
{
    $update = "update ctst_cat set ";
    $update .= 'ctstID= ' . intSQL($record['ctstID']) . ',';
    $update .= 'name = ' . strSQL($record['name'], 72) . ',';
    $_enumSet_class = array (
        'power',
        'glider',
        'other'
    );
    $update .= 'class = ' . enumSQL($record['class'], $_enumSet_class) . ',';
    $_enumSet_category = array (
        'primary',
        'sportsman',
        'intermediate',
        'advanced',
        'unlimited',
        '4min',
        'other'
    );
    $update .= 'category=' . enumSQL($record['category'], $_enumSet_category) . ',';
    $update .= 'regAmt=' . intSQL($record['regAmt']) . ',';
    $update .= 'hasStudentReg=' . boolSQL($record, 'hasStudentReg') . ',';
    $update .= 'studentRegAmt=' . intSQL($record['studentRegAmt']) . ',';
    $update .= 'hasTeamReg=' . boolSQL($record, 'hasTeamReg') . ',';
    $update .= 'teamRegAmt=' . intSQL($record['teamRegAmt']) . ',';
    $update .= 'hasFourMinute=' . boolSQL($record, 'hasFourMinute') . ',';
    $update .= 'fourMinRegAmt=' . intSQL($record['fourMinRegAmt']) . ',';
    $update .= 'hasVoteJudge=' . boolSQL($record, 'hasVoteJudge') . ',';
    $update .= 'voteTeamOnly=' . boolSQL($record, 'voteTeamOnly') . ',';
    $update .= 'maxVotes=' . intSQL($record['maxVotes']) . ',';
    $update .= 'voteByRegion=' . boolSQL($record, 'voteByRegion') . ',';
    $update .= 'maxRegion=' . intSQL($record['maxRegion']) . ',';
    $update .= 'voteDeadline=' . dateSQL($record['voteDeadline'] . '/' . $record['year']);
    $update .= " where catID=" . $catID . ";";
    //debug($update);
    $fail = dbExec($db_conn, $update);
}

function doAddCat($db_conn, $record, & $catID)
{
    $update = "insert into ctst_cat (ctstID, name, class, category, regAmt, " .
    "hasStudentReg, studentRegAmt, hasTeamReg, teamRegAmt, hasFourMinute, fourMinRegAmt, hasVoteJudge, " .
    "maxVotes, voteTeamOnly, voteByRegion, maxRegion, voteDeadline" .
    ") values (";
    $update .= intSQL($record['ctstID']) . ',';
    $update .= strSQL($record['name'], 72) . ',';
    $_enumSet_class = array (
        'power',
        'glider',
        'other'
    );
    $update .= enumSQL($record['class'], $_enumSet_class) . ',';
    $_enumSet_category = array (
        'primary',
        'sportsman',
        'intermediate',
        'advanced',
        'unlimited',
        '4min',
        'other'
    );
    $update .= enumSQL($record['category'], $_enumSet_category) . ',';
    $update .= intSQL($record['regAmt']) . ',';
    $update .= boolSQL($record, 'hasStudentReg') . ',';
    $update .= intSQL($record['studentRegAmt']) . ',';
    $update .= boolSQL($record, 'hasTeamReg') . ',';
    $update .= intSQL($record['teamRegAmt']) . ',';
    $update .= boolSQL($record, 'hasFourMinute') . ',';
    $update .= intSQL($record['fourMinRegAmt']) . ',';
    $update .= boolSQL($record, 'hasVoteJudge') . ',';
    $update .= intSQL($record['maxVotes']) . ',';
    $update .= boolSQL($record, 'voteTeamOnly') . ',';
    $update .= boolSQL($record, 'voteByRegion') . ',';
    $update .= intSQL($record['maxRegion']) . ',';
    $update .= dateSQL($record['voteDeadline'] . '/' . $record['year']);
    $update .= ');';
    //debug($update);
    $fail = dbExec($db_conn, $update);
    if ($fail == '')
    {
        $catID = dbLastID();
    }
    return $fail;
}

function insertOrUpdateCategory($db_conn, $record, & $catID)
{
    if (isSet ($catID) && $catID != null && $catID != '')
    {
        $fail = doUpdateCat($db_conn, $record, $catID);
    }
    else
    {
        $fail = doAddCat($db_conn, $record, $catID);
    }
    return $fail;
}

function doRetrieveCategory($db_conn, & $record, $query)
{
    $fail = '';
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail .= "<it>" . dbErrorText() . "<\it>";
    }
    else
        if (dbCountResult($result) != 0)
        {
            $row = dbFetchAssoc($result);
            foreach ($row as $key => $value)
            {
                $record[$key] = stripslashes($value);
            }
        }
        else
        {
            $fail = "no entry for " . $query;
        }
    $regD = strtotime($record['voteDeadline']);
    $record['voteDeadline'] = strftime('%m/%d', $regD);
    sqlSetValueToPostData($record['class'], 'class', $record);
    sqlSetValueToPostData($record['category'], 'category', $record);
    sqlBoolValueToPostData($record['hasStudentReg'], 'hasStudentReg', $record);
    sqlBoolValueToPostData($record['hasTeamReg'], 'hasTeamReg', $record);
    sqlBoolValueToPostData($record['hasFourMinute'], 'hasFourMinute', $record);
    sqlBoolValueToPostData($record['hasVoteJudge'], 'hasVoteJudge', $record);
    sqlBoolValueToPostData($record['voteTeamOnly'], 'voteTeamOnly', $record);
    sqlBoolValueToPostData($record['voteByRegion'], 'voteByRegion', $record);

    return $fail;
}

function retrieveCategory($db_conn, & $record, $catID)
{
    $query = "select * from ctst_cat where catID = " . intSQL($catID);
    return doRetrieveCategory($db_conn, $record, $query);
}
    
function retrieveCatForCtstClassCat($db_conn, & $record, $ctstID, $class, $cat)
{
    $query = "select * from ctst_cat where ctstID = " . intSQL($ctstID) .
     " and class = '" . addslashes($class) .
     "' and category = '" . addslashes($cat) . "'";
    return doRetrieveCategory($db_conn, $record, $query);
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
