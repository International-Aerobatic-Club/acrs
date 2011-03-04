<?php
/*  contestQueries.inc, acrs/include/query, dclo, 10/24/2010
 *  contest table query functions
 *
 *  Changes:
 *    10/24/2010 jim_ward       sense isset() of $_SESSION['ctst_admin'] and friends before referencing
 *                              them, to suppress PHP notices.
 */

require_once ('data/encodePOST.inc');
require_once ('data/encodeSQL.inc');
require_once ('post/setContest.inc');

function retrieveContest($db_conn, & $ctstInfo, $ctstID)
{
    $fail = '';
    $query = "select * from contest where ctstID = " . $ctstID;
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
                $ctstInfo[$key] = stripslashes($value);
            }
        }
        else
        {
            $fail = "no entry for " . $ctstID;
        }
    $regD = strtotime($ctstInfo['regOpen']);
    $ctstInfo['regOpen'] = strftime('%m/%d', $regD);
    $regD = strtotime($ctstInfo['regDeadline']);
    $ctstInfo['regDeadline'] = strftime('%m/%d', $regD);
    $regD = strtotime($ctstInfo['startDate']);
    $ctstInfo['startDate'] = strftime('%m/%d', $regD);
    $regD = strtotime($ctstInfo['endDate']);
    $ctstInfo['endDate'] = strftime('%m/%d', $regD);
    sqlBoolValueToPostData($ctstInfo['hasVoteJudge'], 'hasVoteJudge', $ctstInfo);
    sqlBoolValueToPostData($ctstInfo['hasPayPal'], 'hasPayPal', $ctstInfo);
    sqlBoolValueToPostData($ctstInfo['hasPracticeReg'], 'hasPracticeReg', $ctstInfo);
    sqlBoolValueToPostData($ctstInfo['reqPmtForVoteJudge'], 'reqPmtForVoteJudge', $ctstInfo);
    sqlBoolValueToPostData($ctstInfo['reqPmtForPracticeReg'], 'reqPmtForPracticeReg', $ctstInfo);
    return $fail;
}

function doUpdateContest($db_conn, $ctstInfo, $ctstID)
{
    $query = "update contest set ";
    $query .= 'regYear= ' . intSQL($ctstInfo['regYear']) . ',';
    $query .= 'name = ' . strSQL($ctstInfo['name'], 72) . ',';
    $query .= 'location = ' . strSQL($ctstInfo['location'], 72) . ',';
    $query .= 'chapter = ' . intSQL($ctstInfo['chapter'], 4) . ',';
    $query .= 'regOpen = ' . dateSQL($ctstInfo['regOpen'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= 'regDeadline = ' . dateSQL($ctstInfo['regDeadline'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= 'startDate = ' . dateSQL($ctstInfo['startDate'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= 'endDate = ' . dateSQL($ctstInfo['endDate'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= 'homeURL = ' . strSQL($ctstInfo['homeURL'], 320) . ',';
    $query .= 'regEmail = ' . strSQL($ctstInfo['regEmail'], 320) . ',';
    $query .= 'hasVoteJudge= ' . boolSQL($ctstInfo, 'hasVoteJudge') . ',';
    $query .= 'voteEmail = ' . strSQL($ctstInfo['voteEmail'], 320) . ',';
    $query .= 'hasPayPal= ' . boolSQL($ctstInfo, 'hasPayPal') . ',';
    $query .= 'payEmail = ' . strSQL($ctstInfo['payEmail'], 320) . ',';
    $query .= 'reqPmtForVoteJudge= ' . boolSQL($ctstInfo, 'reqPmtForVoteJudge') . ',';
    $query .= 'reqPmtForPracticeReg= ' . boolSQL($ctstInfo, 'reqPmtForPracticeReg') . ',';
    $query .= 'hasPracticeReg= ' . boolSQL($ctstInfo, 'hasPracticeReg') . ',';
    $query .= 'maxPracticeSlots= ' . intSQL($ctstInfo['maxPracticeSlots']);
    $query .= " where ctstID = " . $ctstID . ";";
    //debug('doUpdateContest:'.$query);
    $fail = dbExec($db_conn, $query);
}

function doAddContest($db_conn, $ctstInfo, & $ctstID)
{
    $query .= 'reqPmtForVoteJudge= ' . boolSQL($ctstInfo, 'reqPmtForVoteJudge') . ',';
    $query .= 'reqPmtForPracticeReg= ' . boolSQL($ctstInfo, 'reqPmtForPracticeReg') . ',';
    $query = "insert into contest (regYear, name, location, chapter, regOpen, regDeadline," .
    		" startDate, endDate, homeURL, regEmail," .
            " hasVoteJudge, voteEmail, hasPayPal, payEmail, hasPracticeReg, maxPracticeSlots," .
            " reqPmtForVoteJudge, reqPmtForPracticeReg" .
            ") values (";
    $query .= intSQL($ctstInfo['regYear']) . ',';
    $query .= strSQL($ctstInfo['name'], 72) . ',';
    $query .= strSQL($ctstInfo['location'], 72) . ',';
    $query .= intSQL($ctstInfo['chapter']) . ',';
    $query .= dateSQL($ctstInfo['regOpen'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= dateSQL($ctstInfo['regDeadline'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= dateSQL($ctstInfo['startDate'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= dateSQL($ctstInfo['endDate'] . '/' . $ctstInfo['regYear']) . ',';
    $query .= strSQL($ctstInfo['homeURL'], 320) . ',';
    $query .= strSQL($ctstInfo['regEmail'], 320) . ',';
    $query .= boolSQL($ctstInfo, 'hasVoteJudge') . ',';
    $query .= strSQL($ctstInfo['voteEmail'], 320) . ',';
    $query .= boolSQL($ctstInfo, 'hasPayPal') . ',';
    $query .= strSQL($ctstInfo['payEmail'], 320) . ',';
    $query .= boolSQL($ctstInfo, 'hasPracticeReg') . ',';
    $query .= intSQL($ctstInfo['maxPracticeSlots']) . ',';
    $query .= boolSQL($ctstInfo, 'reqPmtForVoteJudge') . ',';
    $query .= boolSQL($ctstInfo, 'reqPmtForPracticeReg');
    $query .= ');';
    debug('doAddContest:'.$query);
    $fail = dbExec($db_conn, $query);
    if ($fail == '')
    {
        $ctstID = dbLastID();
    }
    return $fail;
}

function setAdminUser($db_conn, $ctstID, $userID)
{
    $fail = '';
    //debug('setAdminUser:$_SESSION["ctst_admin"] == ' . $_SESSION['ctst_admin']);
    if (!isset ($_SESSION['ctst_admin']) || !$_SESSION['ctst_admin'])
    {
        $query = "update ctst_admin set roles = ('admin'";
        if (isset ($_SESSION['ctst_cd']) && $_SESSION['ctst_cd'])
            $query .= ",'cd'";
        if (isset ($_SESSION['ctst_registrar']) && $_SESSION['ctst_registrar'])
            $query .= ",'registrar'";
        if (isset ($_SESSION['ctst_vc']) && $_SESSION['ctst_vc'])
            $query .= ",'vc'";
        $query .= ") where ctstId = " . intSQL($ctstID) . " and userID = " . intSQL($userID);
        //debug('setAdminUser:' . $query);
        $fail = dbExec($db_conn, $query);
        if ($fail == '')
        {
            $_SESSION['ctst_admin'] = true;
        }
    }
    return $fail;
}

function addAdminUser($db_conn, $ctstID, $userID)
{
    $fail = '';
    $query = "insert into ctst_admin (ctstID, userID, roles) values (";
    $query .= intSQL($ctstID) . ',';
    $query .= intSQL($userID) . ',\'admin\')';
    //debug('setAdminUser:' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail == '')
    {
        $_SESSION['ctst_admin'] = true;
    }
    return $fail;
}

function updateContest($db_conn, $ctstInfo, & $ctstID, $userID)
{
    $fail = dbBegin($db_conn);
    if ($fail == '')
    {
        if (isSet ($ctstID))
        {
            $fail = doUpdateContest($db_conn, $ctstInfo, $ctstID);
        }
        else
        {
            $fail = doAddContest($db_conn, $ctstInfo, $ctstID);
            if ($fail == '')
                $fail = addAdminUser($db_conn, $ctstID, $userID);
        }
        if ($fail == '')
        {
            $fail = dbCommit($db_conn);
        }
        else
        {
            $fail .= dbRollback($db_conn);
        }
    }
    //debug('on exit updateContest:' . $fail);
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
