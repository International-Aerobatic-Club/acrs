<?php
set_include_path('include'); 
require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");

/*
Functions for reading and writing volunteer data with SQL
*/

// PHP 5 defines this
//function array_combine($arr1, $arr2)
//{
//    $out = array ();
//    foreach ($arr1 as $key1 => $value1)
//    {
//        $out[$value1] = $arr2[$key1];
//    }
//    return $out;
//}

function getJudgeQualificationValues()
{
    return array('none','regional','national');
}

/**
 * return array of volunteer table volunteer column set values
 */
function getVolunteerRoles()
{
    return array (
        'judge',
        'assistJudge',
        'recorder',
        'boundary',
        'runner',
        'deadline',
        'timer',
        'assistChief',
    );
}

/**
 * return array of descriptions for volunteer column set values
 */
function getRoleDescriptions()
{
    return array (
        'As grading judge',
        'As assistant judge or caller',
        'As recorder or scribe',
        'As boundary judge',
        'As runner or driver',
        'As deadline judge',
        'As timer or boundary recorder',
        'As assistant to the Chief Judge'
    );
}

/**
Delete volunteer record from the database for the given category and user
db_conn: database connection handle
userID: safe integer ID of volunteer
catID: category identifier
volSQL: sql safe set value for volunteer field of volunteer table
returns empty string on success, failure message on failure
*/
function deleteVolunteerRecord($db_conn, $userID, $catID, $volSQL)
{
    $fail = '';
    $ctstID = $_SESSION['ctstID'];
    $update = 'delete from volunteer';
    $update .= " where userID = " . $userID;
    $update .= " and catID = " . intSQL($catID);
    //debug($update);
    return dbExec($db_conn, $update);
}

/**
Insert volunteer record into the database for the given category and user
db_conn: database connection handle
userID: safe integer ID of volunteer
cat: category
volSQL: sql safe set value for volunteer field of volunteer table
returns empty string on success, failure message on failure
*/
function insertVolunteerRecord($db_conn, $userID, $catID, $volSQL)
{
    $_enumSet_posts = getVolunteerRoles();
    $fail = '';
    $ctstID = $_SESSION['ctstID'];
    $update = "insert into volunteer (userID, catID, volunteer) values (" .
    intSQL($userID) . ',' .
    intSQL($catID) . ',' .
    $volSQL . ')';
    // debug('insertVolunteerRecord: '.$update);
    return dbExec($db_conn, $update);
}

/**
Update volunteer record for category in the volunteer table
db_conn: database connection handle
userID: safe integer ID of volunteer
cat: category
volSQL: sql safe set value for volunteer field of volunteer table
returns empty string on success, failure message on failure
*/
function updateVolunteerRecord($db_conn, $userID, $catID, $volSQL)
{
    $_enumSet_posts = getVolunteerRoles();
    $update = "update volunteer set ";
    $update .= 'volunteer= ' . $volSQL;
    $update .= " where userID = " . $userID;
    $update .= " and catID = " . intSQL($catID);
    // debug('updateVolunteerRecord:'.$update);
    return dbExec($db_conn, $update);
}

/**
check whether volunteer setting exists for the current year, category, and user.
The current year is contained in the global variable, $_SESSION['ctstID'].
db_conn database connection handle
userID: safe integer ID of registrant
cat: category
return true if record exists, else false
*/
function checkExistsCurrentVolunteer($db_conn, $userID, $catID)
{
    $haveSetting = false;
    $query = 'select volunteer from volunteer';
    $query .= " where userID = " . $userID;
    $query .= " and catID = " . intSQL($catID);
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        notifyError(dbErrorText(), 'checkExistsCurentVolunteer');
    } else
    {
        $haveSetting = (dbCountResult($result) != 0);
    }
    return $haveSetting;
}

function updateJudgeQualification($db_conn, $userID, $qual)
{
    $_enumSet_qual = getJudgeQualificationValues();
    $query = 'update registrant set judgeQualification = ' . enumSQL($qual,$_enumSet_qual) .
    ' where userID = ' . intSQL($userID);
    return dbExec($db_conn, $query);
}

function updateVolunteer($db_conn, $volunteer, $userID)
{
    $_enumSet_posts = getVolunteerRoles();

    // debug("updateVolunteer user id = " . $userID);
    // debugArr('volunteer data', $volunteer);
    $fail = '';
    $ctstCatList = explode('::', $volunteer['compCatIDList']);
    // debugArr('contest category id\'s', $ctstCatList);
    foreach ($ctstCatList as $key => $catID)
    {
        $volSQL = selectionSQL($volunteer, 'volunteer_' . $catID, $_enumSet_posts);
        // debug('update Volunteer category ' . $catID . ' selects ' . $volSQL);
        if (checkExistsCurrentVolunteer($db_conn, $userID, $catID))
        {
            if ($volSQL != '\'\'')
                $fail .= updateVolunteerRecord($db_conn, $userID, $catID, $volSQL);
            else
                $fail .= deleteVolunteerRecord($db_conn, $userID, $catID);
        } else
        {
            if ($volSQL != '\'\'')
                $fail .= insertVolunteerRecord($db_conn, $userID, $catID, $volSQL);
        }
    }
    $fail .= updateJudgeQualification($db_conn, $userID, $volunteer['judgeQualification']);
    if ($fail != '')
    {
        notifyError($fail, "updateVolunteer");
    }
    return $fail;
}

/**
 * Fill the volunteer['compCatIDList'] and volunteer['compCatNameList'] fields
 * given the contest ID.
 * Return error string, empty if no error.
 */
function fillCategoryList($db_conn, $ctstID, & $volunteer)
{
    $fail = '';
    $query = 'select catID, name from ctst_cat';
    $query .= ' where ctstID=' . intSQL($ctstID);
    $query .= ' order by catID';
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    } else
    {
        $compCatIDList = array ();
        while ($row = dbFetchRow($result))
        {
            $compCatIDList[] = $row[0];
            $compCatNameList[] = $row[1];
        }
        $volunteer['compCatIDList'] = implode('::', $compCatIDList);
        $volunteer['compCatNameList'] = implode('::', $compCatNameList);
    }
    return $fail;
}

/**
 * Fill the volunteer['compCatID'] and volunteer['compCatName'] with the identifier
 * and name of any category in which the person with given user ID is registered
 * to compete in the contest with given contest ID. 
 * Return error string, empty if no error.
 */
function fillCompetitionCategory($db_conn, $userID, $ctstID, & $volunteer)
{
    $volunteer['compCatID'] = null;
    $volunteer['compCatName'] = null;
    $fail = '';
    $query = 'select b.catID, b.name, c.compType' .
    ' from registration a, ctst_cat b, reg_type c';
    $query .= ' where c.userID=' . $userID;
    $query .= ' and c.ctstID=' . intSQL($ctstID);
    $query .= ' and a.regID = c.regID';
    $query .= ' and a.catID = b.catID';
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        $fail = dbErrorText();
    } else
        if ($row = dbFetchRow($result))
        {
            $volunteer['compType'] = $row[2];
            if (strcmp($volunteer['compType'], 'competitor') == 0)
            {
                $volunteer['compCatID'] = $row[0];
                $volunteer['compCatName'] = $row[1];
            }
        }
    return $fail;
}

/**
 * Fill the volunteer['givenName'] and volunteer['familyName'] with the
 * registered name for the given user ID. 
 * Return error string, empty if no error.
 */
function fillVolunteerName($db_conn, $userID, & $volunteer)
{
    $fail = '';
    $query = 'select givenName, familyName, judgeQualification from registrant';
    $query .= ' where userID=' . $userID;
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    } else
        if ($row = dbFetchRow($result))
        {
            $volunteer['givenName'] = stripslashes($row[0]);
            $volunteer['familyName'] = stripslashes($row[1]);
            $volunteer['judgeQualification'] = $row[2];
        }
    return $fail;
}

/**
 * Fill the volunteer_{catID}_{role} = on for the given user ID and contest ID. 
 * Return error string, empty if no error.
 */
function fillVolunteerSelections($db_conn, $userID, $ctstID, & $volunteer)
{
    $fail = '';
    $query = 'select a.catID, a.name, volunteer from ctst_cat a, volunteer b';
    $query .= ' where userID=' . $userID;
    $query .= ' and a.ctstID=' . intSQL($ctstID);
    $query .= ' and a.catID = b.catID';
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    } else
    {
        while ($row = dbFetchRow($result))
        {
            sqlSetValueToPostData($row[2], 'volunteer_' . $row[0], $volunteer);
        }
    }
    return $fail;
}

/**
Retrieve volunteer data from the registration database
db_conn database connection handle
userID: safe integer ID of volunteer
volunteer: reference to registrant data in an associative array
retrieves volunteer information for user, current contest year
*/
function retrieveVolunteer($db_conn, & $volunteer, $userID)
{
    //debug("retrieveVolunteer user id = " . $userID);
    $ctstID = $_SESSION['ctstID'];
    // retrieve volunteer data (unique to user id)
    $fail = fillVolunteerSelections($db_conn, $userID, $ctstID, $volunteer);
    $fail .= fillVolunteerName($db_conn, $userID, $volunteer);
    $fail .= fillCompetitionCategory($db_conn, $userID, $ctstID, $volunteer);
    $fail .= fillCategoryList($db_conn, $ctstID, $volunteer);
    //debugArr('volQueries:volunteer:',$volunteer);
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
