<?php
/*  setContest.inc, acrs/include/post, dclo, 10/24/2010
 *  set, update and clear contest-related data
 *
 *  Changes:
 *    10/24/2010 jim_ward       correct result test from dbQuery() function value in setRoles().
 */

require_once ('dbCommand.inc');
require_once ('useful.inc');

function setContest($ctstID)
{
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "data/validCtst.inc");
    } else
    {
        $fail = setContestL($db_conn, $ctstID);
        dbClose($db_conn);
    }
    return $fail;
}

function clearContest()
{
    //debug('enter clearContest');
    unset ($_SESSION['ctstID']);
    unset ($_SESSION['ctst_name']);
    unset ($_SESSION['ctst_year']);
    unset ($_SESSION['ctst_descr']);
    unset ($_SESSION['ctst_question']);
    unset ($_SESSION['contest']);
    $roles = explode(",", 'admin,cd,registrar,vc');
    foreach ($roles as $key)
    {
        unset ($_SESSION['ctst_' . $key]);
    }
    //debug('exit clearContest');
}

function setRoles($db_conn, $ctstID)
{
    $fail = '';
    $query = "select roles from ctst_admin where ctstID = " . $ctstID .
    " and userID = " . $_SESSION['userID'];
    $result = dbQuery($db_conn, $query);
    //debug('setRoles:' . $query);

    if ($result === false)
        $fail = notifyError('Fail query, "' . $query . '" is ' . dbErrorText(), 'form/setContest.inc');

    else
    {
        if (($row = dbFetchRow($result)))   /* assignment intended */
        {
            //debugArr('setRoles query result:', $row);
            $roles = explode(",", $row[0]);
            //debugArr('setRoles roles:', $roles);
            foreach ($roles as $key => $value)
                $_SESSION['ctst_' . $value] = true;
        }   /* query returned a row */
    }   /* query succeeded */
    return $fail;
}

function setCtstVars($db_conn, $ctstID)
{
    $fail = '';
    $query = "select * from contest where ctstID = " . $ctstID;
    $result = dbQuery($db_conn, $query);
    //debug('setCtstVars:' . $query);
    if (dbErrorNumber() != 0)
    {
        $fail = notifyError('Fail query, "' . $query . '" is ' . dbErrorText(), 'form/setContest.inc');
    } else
    {
        $ctst = array ();
        $row = dbFetchAssoc($result);
        foreach ($row as $key => $value)
        {
            $ctst[$key] = stripslashes($value);
        }
        //debugArr('setCtstVars query result:', $row);
        $_SESSION['ctst_name'] = $ctst['name'];
        $_SESSION['ctst_year'] = $ctst['regYear'];
        $_SESSION['ctst_descr'] = $ctst['regYear'] . ' ' . $ctst['name'];
        $_SESSION['ctst_question'] = $ctst['question'];
        $_SESSION['contest'] = $ctst;
    }
    return $fail;
}

function setContestL($db_conn, $ctstID)
{
    //debug('setContestL: ctstID = ' . $ctstID);
    $_SESSION['ctstID'] = $ctstID;
    $fail = setRoles($db_conn, $ctstID);
    if ($fail == '')
        $fail = setCtstVars($db_conn, $ctstID);
    return $fail;
}

/*
   Copyright 2008, 2010 International Aerobatic Club, Inc.

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
