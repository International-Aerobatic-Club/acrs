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
require_once ('data/validInt.inc');
require_once ("useful.inc");
require_once ("redirect.inc");
require_once ('query/catQueries.inc');

/**
Provides style settings and script for the form. Call from the header
*/
function categoryFormHeader()
{
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

function validatePost(& $record)
{
    $corrMsg = '';
    $record['name'] = crop($record['name'], 72);
    if (strlen($record['name']) == 0)
    {
        $record['name'] = ucfirst($record['class']) . ' ' . ucfirst($record['category']);
    }
    $fail = validInt($record, 'regAmt', 'regular registration amount', 4, true);
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    if (boolChecked($record, 'hasStudentReg'))
    {
        $fail = validInt($record, 'studentRegAmt', 'student registration amount', 4, true);
        if ($fail != '')
            $corrMsg .= '<li>' . $fail . '</li>';
    }
    if (boolChecked($record, 'hasTeamReg'))
    {
        $fail = validInt($record, 'teamRegAmt', 'team registration amount', 4, true);
        if ($fail != '')
            $corrMsg .= '<li>' . $fail . '</li>';
    }
    if (boolChecked($record, 'hasFourMinute'))
    {
        $fail = validInt($record, 'fourMinRegAmt', 'four minute free additional registration amount', 4, true);
        if ($fail != '')
            $corrMsg .= '<li>' . $fail . '</li>';
    }
    if (boolChecked($record, 'hasVoteJudge'))
    {
        $fail = validInt($record, 'maxVotes', 'maximum total votes', 2, true);
        if ($fail != '')
            $corrMsg .= '<li>' . $fail . '</li>';
        if (boolChecked($record, 'voteByRegion'))
        {
            $fail = validInt($record, 'maxRegion', 'maximum votes per region', 2, true);
            if ($fail != '')
                $corrMsg .= '<li>' . $fail . '</li>';
        }
        $failDate = futureMMDD($record['voteDeadline'], $record['year'], 'Judge voting deadline');
        if ($failDate != '')
            $corrMsg .= '<li>' . $failDate . '</li>';
    } else
    {
        $record['voteDeadline'] = '';
    }
    return $corrMsg;
}

/**
Creates a form for editing category data.
$record POST format registration data
$action is html sanitized url for the http post
*/
function categoryForm($record, $action)
{
    echo "<form class=\"recordForm\" action=\"" . $action . "\" method=\"post\">\n";
    echo "<table><tbody>\n";
    echo "<tr>\n";
    echo "<td colspan=\"2\" class=\"form_select\"><label for=\"in_class\">Class:</label><fieldset id=\"in_class\" legend=\"Class\">\n";
    echo "<input class=\"form_select\" id=\"in_class_power\" type=\"radio\" name=\"class\" value=\"power\" " . isSelected($record, 'class', 'power') . ">power</input>\n";
    echo "<input class=\"form_select\" id=\"in_class_glider\" type=\"radio\" name=\"class\" value=\"glider\" " . isSelected($record, 'class', 'glider') . ">glider</input>\n";
    echo "<input class=\"form_select\" id=\"in_class_other\" type=\"radio\" name=\"class\" value=\"other\" " . isSelected($record, 'class', 'other') . ">other</input>\n";
    echo "</fieldset></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_select\"><label for=\"in_category\">Category:</label><fieldset id=\"in_category\" legend=\"Category\">\n";
    echo "<input class=\"form_select\" id=\"in_category_primary\" type=\"radio\" name=\"category\" value=\"primary\" " . isSelected($record, 'category', 'primary') . ">primary</input>\n";
    echo "<input class=\"form_select\" id=\"in_category_sportsman\" type=\"radio\" name=\"category\" value=\"sportsman\" " . isSelected($record, 'category', 'sportsman') . ">sportsman</input>\n";
    echo "<input class=\"form_select\" id=\"in_category_intermediate\" type=\"radio\" name=\"category\" value=\"intermediate\" " . isSelected($record, 'category', 'intermediate') . ">intermediate</input>\n";
    echo "<input class=\"form_select\" id=\"in_category_advanced\" type=\"radio\" name=\"category\" value=\"advanced\" " . isSelected($record, 'category', 'advanced') . ">advanced</input>\n";
    echo "<input class=\"form_select\" id=\"in_category_unlimited\" type=\"radio\" name=\"category\" value=\"unlimited\" " . isSelected($record, 'category', 'unlimited') . ">unlimited</input>\n";
    echo "<input class=\"form_select\" id=\"in_category_other\" type=\"radio\" name=\"category\" value=\"other\" " . isSelected($record, 'category', 'other') . ">other</input>\n";
    echo "</fieldset></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_name\">Category description:</label><input id=\"in_name\" name=\"name\" value=\"" . $record['name'] . "\" maxlength=\"72\" size=\"48\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_text\"><label for=\"in_regAmt smallint unsigned\">Regular registration amount:</label><input id=\"in_regAmt\" name=\"regAmt\" value=\"" . $record['regAmt'] . "\" maxlength=\"4\" size=\"4\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_hasStudentReg\">Allows student registrations?:</label><fieldset id=\"in_hasStudentReg\" legend=\"Allows student registrations?\">\n";
    echo "<input class=\"form_select\" id=\"in_hasStudentReg_yes\" type=\"radio\" name=\"hasStudentReg\" value=\"yes\" " . isSelected($record, 'hasStudentReg', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_hasStudentReg_no\" type=\"radio\" name=\"hasStudentReg\" value=\"no\" " . isSelected($record, 'hasStudentReg', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "\n";
    echo "<td class=\"form_text\"><label for=\"in_studentRegAmt\">Student registration amount:</label><input id=\"in_studentRegAmt\" name=\"studentRegAmt\" value=\"" . $record['studentRegAmt'] . "\" maxlength=\"4\" size=\"4\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_hasTeamReg\">Has team candidates?:</label><fieldset id=\"in_hasTeamReg\" legend=\"Has team candidates?\">\n";
    echo "<input class=\"form_select\" id=\"in_hasTeamReg_yes\" type=\"radio\" name=\"hasTeamReg\" value=\"yes\" " . isSelected($record, 'hasTeamReg', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_hasTeamReg_no\" type=\"radio\" name=\"hasTeamReg\" value=\"no\" " . isSelected($record, 'hasTeamReg', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "\n";
    echo "<td class=\"form_text\"><label for=\"in_teamRegAmt\">Team registration amount:</label><input id=\"in_teamRegAmt\" name=\"teamRegAmt\" value=\"" . $record['teamRegAmt'] . "\" maxlength=\"4\" size=\"4\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_hasFourMinute\">Has four minute free option?:</label><fieldset id=\"in_hasFourMinute\" legend=\"Has four minute free option?\">\n";
    echo "<input class=\"form_select\" id=\"in_hasFourMinute_yes\" type=\"radio\" name=\"hasFourMinute\" value=\"yes\" " . isSelected($record, 'hasFourMinute', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_hasFourMinute_no\" type=\"radio\" name=\"hasFourMinute\" value=\"no\" " . isSelected($record, 'hasFourMinute', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "\n";
    echo "<td class=\"form_text\"><label for=\"in_teamRegAmt\">Four minute free additional amount:</label><input id=\"in_fourMinRegAmt\" name=\"fourMinRegAmt\" value=\"" . $record['fourMinRegAmt'] . "\" maxlength=\"4\" size=\"4\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_hasVoteJudge\">Judge voting?:</label><fieldset id=\"in_hasVoteJudge\" legend=\"Judge voting?\">\n";
    echo "<input class=\"form_select\" id=\"in_hasVoteJudge_yes\" type=\"radio\" name=\"hasVoteJudge\" value=\"yes\" " . isSelected($record, 'hasVoteJudge', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_hasVoteJudge_no\" type=\"radio\" name=\"hasVoteJudge\" value=\"no\" " . isSelected($record, 'hasVoteJudge', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_maxVotes\">Maximum total votes:</label><input id=\"in_maxVotes\" name=\"maxVotes\" value=\"" . $record['maxVotes'] . "\" maxlength=\"2\" size=\"2\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td colspan=\"2\" class=\"form_select\"><label for=\"in_voteTeamOnly'\">Votes by team candidates only?:</label><fieldset id=\"in_voteTeamOnly\" legend=\"Votes by team candidates only?\">\n";
    echo "<input class=\"form_select\" id=\"in_voteTeamOnly_yes\" type=\"radio\" name=\"voteTeamOnly\" value=\"yes\" " . isSelected($record, 'voteTeamOnly', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_voteTeamOnly_no\" type=\"radio\" name=\"voteTeamOnly\" value=\"no\" " . isSelected($record, 'voteTeamOnly', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_select\"><label for=\"in_voteByRegion\">Votes by Region?:</label><fieldset id=\"in_voteByRegion\" legend=\"Votes by Region?\">\n";
    echo "<input class=\"form_select\" id=\"in_voteByRegion_yes\" type=\"radio\" name=\"voteByRegion\" value=\"yes\" " . isSelected($record, 'voteByRegion', 'yes') . ">yes</input>\n";
    echo "<input class=\"form_select\" id=\"in_voteByRegion_no\" type=\"radio\" name=\"voteByRegion\" value=\"no\" " . isSelected($record, 'voteByRegion', 'no') . ">no</input>\n";
    echo "</fieldset></td>\n";
    echo "\n";
    echo "<td class=\"form_text\"><label for=\"in_maxRegion\">Maximum votes per region:</label><input id=\"in_maxRegion\" name=\"maxRegion\" value=\"" . $record['maxRegion'] . "\" maxlength=\"2\" size=\"2\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_voteDeadline\">Judge voting deadline(MM/DD):</label><input id=\"in_voteDeadline\" name=\"voteDeadline\" value=\"" . $record['voteDeadline'] . "\" maxlength=\"5\" size=\"5\"/></td>\n";
    echo "</tr>\n";
    echo "</tbody></table>\n";
    echo "<div class=\"submit\">\n";
    echo "<input class=\"submit\" name=\"save\" type=\"submit\" value=\"Save and Continue\"/>\n";
    echo "</div>\n";
    echo "<input type=\"hidden\" id=\"in_catID\" name=\"catID\" value=\"" . $record['catID'] . "\"/>\n";
    echo "<input type=\"hidden\" id=\"in_ctstID\" name=\"ctstID\" value=\"" . $record['ctstID'] . "\"/>\n";
    echo "<input type=\"hidden\" id=\"in_year\" name=\"year\" value=\"" . $record['year'] . "\"/>\n";
    echo "</form>\n";
}

function processForm($catInfo)
{
    $wasUpdated = false;
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "catForm.php");
        $corrMsg = "<it>Internal: failed access to contest database</it>";
    } else
        if (isset ($catInfo["save"]))
        {
            // create or update record with post data
            $corrMsg = validatePost($catInfo);
            //debugArr("catForm post data", $catInfo);
            if ($corrMsg == '')
            {
                // have valid data. update sets catID
                $catID = $catInfo['catID'];
                $fail = insertOrUpdateCategory($db_conn, $catInfo, $catID);
                if ($fail == '')
                {
                    $record['catID'] = $catID;
                    $wasUpdated = true;
                } else
                {
                    $corrMsg = "<it>" . $fail . "</it>";
                }
            }
        } else
            if (isset ($_GET['catID']))
            {
                // read record for edit
                $catID = $_GET['catID'];
                $fail = retrieveCategory($db_conn, $catInfo, $catID);
                if ($fail != '')
                {
                    notifyError($fail, "catForm.php");
                    $corrMsg = "<it>Internal: failed access to category record of " . $catID . "</it>";
                }
                $catInfo['year'] = $_SESSION['ctst_year'];
                //debugArr("catForm record data", $catInfo);
            } else
            {
                $catInfo['catID'] = '';
                $catInfo['ctstID'] = $_SESSION['ctstID'];
                $catInfo['year'] = $_SESSION['ctst_year'];
                $catInfo['class'] = 'power';
                $catInfo['category'] = 'primary';
                sqlBoolValueToPostData('n', 'hasStudentReg', $catInfo);
                sqlBoolValueToPostData('n', 'hasTeamReg', $catInfo);
                sqlBoolValueToPostData('n', 'hasVoteJudge', $catInfo);
                sqlBoolValueToPostData('n', 'voteTeamOnly', $catInfo);
                sqlBoolValueToPostData('n', 'voteByRegion', $catInfo);
                //debugArr("catForm initial data", $catInfo);
            }
    dbClose($db_conn);

    if ($wasUpdated)
    {
        getNextPage('catTable.php');
    } else
    {
        // add contest form
        // $corrMsg has HTML content
        // $catInfo has POST content
        startHead("Category Settings");
        categoryFormHeader();
        startContent();
        echo "<h1>Category Settings</h1>";
        verificationHeader("Contest official,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        echo '<p>Edit the settings for this contest category.</p>';
        categoryForm($catInfo, "catForm.php");
        echo '<div class="returnButton"><a href="catTable.php">Return without saving</a></div>';
        endContent();
    }
}

if (isContestAdmin())
{
    processForm($_POST);
} else
{
    getNextPage('index.html');
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
