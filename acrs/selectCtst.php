<?php
/*  selectCtst.php, acrs, dclo, 10/24/2010
 *  contest table query and update
 *
 *  Changes:
 *    10/24/2010 jim_ward       don't reference undefined array indices.  Remove unused args from showMenu.
 *                              There's no need to call getBasicRegistrationData() from the main code, so we
 *                              no longer do so.  Also, remove all args from show_menu(); none were used.
 */

set_include_path('./include'); 
require ("ui/validate.inc");
require_once ('post/setContest.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ('data/timecheck.inc');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("query/userQueries.inc");

function showMenu()
{
    echo "<ul class=\"basicReg\">\n";
    echo "<li><a href='addContest.php'>Add a new contest.</a></li>\n";
    echo "</ul>\n";
}

function showRegistrations($db_conn)
{
    $query = 'select a.ctstID, a.regYear, a.name as ctstName, a.startDate, b.name as catName'.
    ' from reg_type c, contest a, ctst_cat b, registration d' . 
    ' where c.userID = '.$_SESSION['userID'].
    ' and c.compType = \'competitor\'' .
    ' and d.regID = c.regID' .
    ' and b.catID = d.catID' .
    ' and a.ctstID = c.ctstID' .
    ' and now() <= a.endDate' .
    ' order by a.startDate;';
    // debug('selectCtst:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p class="error">' . notifyError(dbErrorText(),'selectCtst') . '</p>';
    }
    else
    {
        if (0 < dbCountResult($result))
        {
            echo '<h3>You are registered to compete and volunteer:</h3>';
            echo '<table class="contestList">';
            echo '<thead class="contestList">';
            echo '<tr><th>Start</th><th>Contest</th><th>Category</th></tr>';
            echo '</thead><tbody class="contestList">';
            while ($curRcd = dbFetchAssoc($result))
            {
                echo '<tr><td>' .
                datehtml($curRcd['startDate']) . '</td>'.
                '<td><a href="index.php?ctstID=' .
                inthtml($curRcd['ctstID']) . '">' .
                inthtml($curRcd['regYear']) . ' ' .
                strhtml(stripslashes($curRcd['ctstName'])) . 
                '</a></td>' . "\n<td>".
                strhtml($curRcd['catName']) . '</td></tr>' . "\n";
            }
            echo '</tbody></table>';
        }
    }
}

function showVolunteer($db_conn)
{
    $query = 'select a.ctstID, a.regYear, a.name, a.startDate'.
    ' from reg_type c, contest a' . 
    ' where c.userID = '.$_SESSION['userID'].
    ' and c.compType = \'volunteer\'' .
    ' and a.ctstID = c.ctstID' .
    ' and now() <= a.endDate' .
    ' order by a.startDate;';
    //debug('selectCtst:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p class="error">' . notifyError(dbErrorText(),'selectCtst') . '</p>';
    }
    else
    {
        if (0 < dbCountResult($result))
        {
            echo '<h3>Your are registered to volunteer:</h3>';
            echo '<table class="contestList">';
            echo '<thead class="contestList">';
            echo '<tr><th>Start</th><th>Contest</th></tr>';
            echo '</thead><tbody class="contestList">';
            while ($curRcd = dbFetchAssoc($result))
            {
                echo '<tr><td>' .
                datehtml($curRcd['startDate']) . '</td>'.
                '<td><a href="index.php?ctstID=' .
                inthtml($curRcd['ctstID']) . '">' .
                inthtml($curRcd['regYear']) . ' ' .
                strhtml(stripslashes($curRcd['name'])) . 
                '</a></td></tr>' . "\n";
            }
            echo '</tbody></table>';
        }
    }
}

function showOfficial($db_conn)
{
    $query = 'select a.ctstID, a.regYear, a.name as ctstName, a.startDate'.
    ' from ctst_admin c, contest a' . 
    ' where c.userID = '.$_SESSION['userID'].
    ' and a.ctstID = c.ctstID' .
    ' order by a.startDate desc;';
    // debug('selectCtst:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p class="error">' . notifyError(dbErrorText(),'selectCtst') . '</p>';
    }
    else
    {
        if (0 < dbCountResult($result))
        {
            echo '<h3>Your are or were a contest official:</h3>';
            echo '<table class="contestList">';
            echo '<thead class="contestList">';
            echo '<tr><th>Start</th><th>Contest</th></tr>';
            echo '</thead><tbody class="contestList">';
            while ($curRcd = dbFetchAssoc($result))
            {
                echo '<tr><td>' .
                datehtml($curRcd['startDate']) . '</td>'.
                '<td><a href="index.php?ctstID=' .
                inthtml($curRcd['ctstID']) . '">' .
                inthtml($curRcd['regYear']) . ' ' .
                strhtml(stripslashes($curRcd['ctstName'])) . 
                '</a></td>' . '</tr>' . "\n";
            }
            echo '</tbody></table>';
        }
    }
}

function showContests($db_conn)
{
    $query = 'select ctstID, regYear, name, startDate, endDate, regDeadline'.
    ' from contest' . 
    ' where now() <= endDate' .
    ' and regOpen <= now()' .
    ' order by startDate;';
    //debug('selectCtst:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p class="error">' . notifyError(dbErrorText(),'selectCtst') . '</p>';
    }
    else
    {
        if (0 == dbCountResult($result))
            echo '<p>There are no contests open for registration.</p>';
        else
        {
            echo '<h3>Available Contests</h3>';
            echo '<table class="contestList">';
            echo '<thead class="contestList">';
            echo '<tr><th>Contest</th><th>Start</th><th>End</th><th>Reg closes</th></tr>';
            echo '</thead><tbody class="contestList">';
            while ($curRcd = dbFetchAssoc($result))
            {
                echo '<tr><td><a href="index.php?ctstID=' .
                inthtml($curRcd['ctstID']) . '">' .
                inthtml($curRcd['regYear']) . ' ' .
                strhtml(stripslashes($curRcd['name'])) . '</a></td><td> ' .
                datehtml($curRcd['startDate']) . '</td>' . "\n<td>".
                datehtml($curRcd['endDate']) . '</td>' . "\n<td>".
                datehtml($curRcd['regDeadline']) . '</td>' . "\n";
            }
            echo '</tbody></table>';
        }
    }
}

clearContest();
startHead("Select a Contest");
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>' . "\n";
startContent();
echo "<h1>Select a Contest</h1>";
$fail = dbConnect($db_conn);
$userID = $_SESSION['userID'];
if (isset ($_SESSION['ctstID']))
    $ctstID = $_SESSION['ctstID'];
else
    unset ($ctstID);

if ($fail != '')
{
    echo "<p>" . $fail . "</p>";
}
else
{
    verificationHeader("Welcome,");
    showContests($db_conn);
    showMenu();
    showRegistrations($db_conn);
    showVolunteer($db_conn);
    showOfficial($db_conn);
    dbClose($db_conn);
}
endContent();
?>
<?php
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
