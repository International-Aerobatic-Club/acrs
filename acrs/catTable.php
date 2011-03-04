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
function categoryTablePageHeader()
{
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

function startTable($contest)
{
    echo "<table class=\"category\">\n";
    echo "<thead>\n";
    echo "<tr>\n";
    echo "<th>Category</th>\n";
    echo "<th>Edit</th>\n";
    echo "<th>Delete</th>\n";
    echo "<th>Reg amt</th>\n";
    echo "<th>Has Student</th>\n";
    echo "<th>Student amt</th>\n";
    echo "<th>Has Team</th>\n";
    echo "<th>Team amt</th>\n";
    echo "<th>Has 4Min</th>\n";
    echo "<th>4Min add amt</th>\n";
    if (sqlIsTrue($contest['hasVoteJudge']))
    {
        echo "<th>Judge Voting</th>\n";
        echo "<th>Max Votes</th>\n";
        echo "<th>Vote Team only</th>\n";
        echo "<th>Vote Region</th>\n";
        echo "<th>Max per Region</th>\n";
        echo "<th>Voting deadline</th>\n";
    }
    echo "</tr>\n";
    echo "</thead>\n";
    echo "<tbody>\n";
}

function endTable()
{
    echo '</tbody></table>';
}

function categoryTable($db_conn, $ctstID, $contest)
{
    //debugArr('catTable contest', $contest);
    startTable($contest);
    $result = dbQuery($db_conn, "select * from ctst_cat where ctstID = " . intsql($ctstID));

    if (dbErrorNumber() != 0)
    {
        $fail = notifyError(strhtml(dbErrorText()) . " on select from ctst_cat.", "catTable.php");
    } else
        while ($row = dbFetchAssoc($result))
        {
            echo '<tr><td class="name">' . strhtml($row['name']) . "</td>\n";
            echo '<td class="cmd"><a href="catForm.php?catID=' . $row['catID'] . '">edit</a></td>';
            echo '<td class="cmd"><a href="catTable.php?delete=' . $row['catID'] . '"/>delete</a></td>';
            echo '<td class="amt">' . inthtml($row['regAmt']) . "</td>\n";
            sqlBoolValueToPostData($row['hasStudentReg'], 'hasStudentReg', $row);
            echo '<td class="bool">' . strhtml($row['hasStudentReg']) . "</td>\n";
            echo '<td class="amt">' . inthtml($row['studentRegAmt']) . "</td>\n";
            sqlBoolValueToPostData($row['hasTeamReg'], 'hasTeamReg', $row);
            echo '<td class="bool">' . strhtml($row['hasTeamReg']) . "</td>\n";
            echo '<td class="amt">' . inthtml($row['teamRegAmt']) . "</td>\n";
            sqlBoolValueToPostData($row['hasFourMinute'], 'hasFourMinute', $row);
            echo '<td class="bool">' . strhtml($row['hasFourMinute']) . "</td>\n";
            echo '<td class="amt">' . inthtml($row['fourMinRegAmt']) . "</td>\n";
            if (sqlIsTrue($contest['hasVoteJudge']))
            {
                sqlBoolValueToPostData($row['hasVoteJudge'], 'hasVoteJudge', $row);
                echo '<td class="bool">' . strhtml($row['hasVoteJudge']) . "</td>\n";
                echo '<td class="ct">' . inthtml($row['maxVotes']) . "</td>\n";
                sqlBoolValueToPostData($row['voteTeamOnly'], 'voteTeamOnly', $row);
                echo '<td class="bool">' . strhtml($row['voteTeamOnly']) . "</td>\n";
                sqlBoolValueToPostData($row['voteByRegion'], 'voteByRegion', $row);
                echo '<td class="bool">' . strhtml($row['voteByRegion']) . "</td>\n";
                echo '<td class="ct">' . inthtml($row['maxRegion']) . "</td>\n";
                echo '<td class="date">' . datehtml($row['voteDeadline']) . "</td>\n";
            }
            echo "</tr>\n";
            //debugArr('catTable row ', $row);
        }
    endTable();
}

function verifyDeleteForm($db_conn, $catID)
{
    $result = dbQuery($db_conn, "select name from ctst_cat where catID = " . intsql($catID));

    if (dbErrorNumber() != 0)
    {
        echo '<p class="error">' .
        notifyError(strhtml(dbErrorText()) .
        " on select from ctst_cat.", 'catTable.php') . '</p>';
    } else
        if ($row = dbFetchRow($result))
        {
            echo '<p>Delete category ' . strhtml($row[0]) . "?</p>\n";
            echo '<form action="catTable.php" method="post">';
            echo '<input type="hidden" name="catID" value="' . $catID . '"/>';
            echo '<input type="submit" name="delete" value="Yes, delete the category"/>';
            echo '</form>';
        } else
        {
            echo '<p>' .
            notifyError('The category record ' . $catID . ' is already gone.', 'catTable.php') . '</p>';
        }
}

function doDeleteCategory($db_conn, $catID)
{
    $fail = '';
    //TODO delete records that refer to category
    $query = 'delete from ctst_cat where catID = ' . intsql($catID);
    dbExec($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = '<li>' .
        notifyError(dbErrorText() . ' on ' . $query, 'catTable.php') .
        '</li>';
    }
    return $fail;
}

function processForm($catInfo)
{
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "catForm.php");
        $corrMsg = "<li>Internal: failed access to contest database</li>";
    } else
    {
        if (isset ($catInfo['delete']))
        {
            $corrMsg = doDeleteCategory($db_conn, $catInfo['catID']);
        }
        startHead("Contest Categories");
        categoryTablePageHeader();
        startContent();
        echo "<h1>Contest Categories</h1>";
        verificationHeader("Contest official,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        if (isset ($_GET['delete']))
        {
            verifyDeleteForm($db_conn, $_GET['delete']);
            echo '<div class="returnButton"><a href="catTable.php">Return without changes</a></div>';
        } else
        {
            categoryTable($db_conn, $_SESSION['ctstID'], $_SESSION['contest']);
            $link = 'catForm.php?ctstID=' . $_SESSION['ctstID'] . '&year=' . $_SESSION['ctst_year'];
            echo '<p><a href="catForm.php?' . strhtml($link) . '">Add Category</input></p>';
            echo "<p><a href='categoryWizard.php'>Run contest category wizard.</a></p>\n";
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
