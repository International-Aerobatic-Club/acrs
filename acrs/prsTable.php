<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once ("data/encodeHTML.inc");
require_once ('data/validMMDD.inc');
require_once ("useful.inc");
require_once ("redirect.inc");

/**
Provides style settings and script for the form. Call from the header
*/
function sessionTablePageHeader()
{
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

function startTable()
{
    echo "<table class=\"session\">\n";
    echo "<thead>\n";
    echo "<tr>\n";
    echo "<th>Edit</th>\n";
    echo "<th>Delete</th>\n";
    echo "<th>Date</th>\n";
    echo "<th>Start time</th>\n";
    echo "<th>End time</th>\n";
    echo "<th>Minutes</th>\n";
    echo "<th>Max per pilot</th>\n";
    echo "</tr>\n";
    echo "</thead>\n";
    echo "<tbody>\n";
}

function endTable()
{
    echo '</tbody></table>';
}

function sessionTable($db_conn, $ctstID)
{
    startTable();
    $query = "select * from session " .
    		"where ctstID = " . intsql($ctstID).
    		' order by practiceDate, startTime';
    $result = dbQuery($db_conn, $query);

    if (dbErrorNumber() != 0)
    {
        $fail = notifyError(strhtml(dbErrorText()) . " on select from session.", "prsTable.php");
    } else
        while ($row = dbFetchAssoc($result))
        {
            echo "<tr>\n";
            echo '<td><a href="prsForm.php?sessID=' . $row['sessID'] . '">edit</a></td>';
            echo '<td><a href="prsTable.php?delete=' . $row['sessID'] . '"/>delete</a></td>';
            echo '<td>' . datehtml($row['practiceDate']) . "</td>\n";
            echo '<td>' . timehtml($row['startTime']) . "</td>\n";
            echo '<td>' . timehtml($row['endTime']) . "</td>\n";
            echo '<td>' . inthtml($row['minutesPer']) . "</td>\n";
            echo '<td>' . inthtml($row['maxSlotsPer']) . "</td>\n";
            echo "</tr>\n";
        }
    endTable();
}

function verifyDeleteForm($db_conn, $sessID)
{
    $result = dbQuery($db_conn, "select practiceDate, startTime, endTime from session where sessID = " . intsql($sessID));

    if (dbErrorNumber() != 0)
    {
        echo '<p class="error">' .
        notifyError(strhtml(dbErrorText()) .
        " on select from session.", 'prsTable.php') . '</p>';
    } else
        if ($row = dbFetchRow($result))
        {
            echo '<p>Delete practice session on ' . datehtml($row[0]) . " from ".timehtml($row[1])." to ".timehtml($row[2])."?</p>\n";
            echo '<form action="prsTable.php" method="post">';
            echo '<input type="hidden" name="sessID" value="' . $sessID . '"/>';
            echo '<input type="submit" name="delete" value="Yes, delete the session"/>';
            echo '</form>';
        } else
        {
            echo '<p>' .
            notifyError('The session record ' . $sessID . ' is already gone.', 'prsTable.php') . '</p>';
        }
}

function doDeleteSession($db_conn, $sessID)
{
    $fail = '';
    //TODO delete records that refer to session
    $query = 'delete from session where sessID = ' . intsql($sessID);
    dbExec($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = '<li>' .
        notifyError(dbErrorText() . ' on ' . $query, 'prsTable.php') .
        '</li>';
    }
    return $fail;
}

function processForm($sessInfo)
{
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "prsForm.php");
        $corrMsg = "<li>Internal: failed access to contest database</li>";
    } else
    {
        if (isset ($sessInfo['delete']))
        {
            $corrMsg = doDeleteSession($db_conn, $sessInfo['sessID']);
        }
        startHead("Practice Sessions");
        sessionTablePageHeader();
        startContent();
        echo "<h1>Practice Sessions</h1>";
        verificationHeader("Contest official,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        if (isset ($_GET['delete']))
        {
            verifyDeleteForm($db_conn, $_GET['delete']);
            echo '<div class="returnButton"><a href="prsTable.php">Return without changes</a></div>';
        } else
        {
            sessionTable($db_conn, $_SESSION['ctstID']);
            $link = 'prsForm.php?ctstID=' . $_SESSION['ctstID'] . '&year=' . $_SESSION['ctst_year'];
            echo '<p><a href="prsForm.php?' . strhtml($link) . '">Add Session</input></p>';
            echo '<p><a href="index.php">Return to registration</a></div>';
        }
        endContent();
        dbClose($db_conn);
    }
}

if (isContestAdmin())
{
    processForm($_POST);
} else
{
    getNextPage('index.html');
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
