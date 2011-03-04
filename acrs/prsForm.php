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
require_once ('prsQueries.php');

/**
Provides style settings and script for the form. Call from the header
*/
function sessionFormHeader()
{
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
}

function validatePost(& $sessInfo)
{
    $corrMsg = '';
    $fail = validMMDD($sessInfo['practiceDate'], $sessInfo['year'], 'Session date(MM/DD)');
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    $fail = validHHMM($sessInfo['startTime'], 'Start time(HH:MM)');
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    $fail = validHHMM($sessInfo['endTime'], 'End time(HH:MM)');
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    $fail = validInt($sessInfo, 'maxSlotsPer', 'Max slots per pilot', 2, true);
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    $fail = validInt($sessInfo, 'minutesPer', 'Minutes per slot', 2, true);
    if ($fail != '')
        $corrMsg .= '<li>' . $fail . '</li>';
    return $corrMsg;
}

/**
Creates a form for editing practice session data.
$sessInfo POST format data
$action is html sanitized url for the http post
*/
function sessionForm($sessInfo, $action)
{
    echo "<form class=\"recordForm\" action=\"" . $action . "\" method=\"post\">\n";
    echo "<table><tbody>\n";
    echo "<tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_practiceDate\">Session date(MM/DD):</label><input id=\"in_practiceDate\" name=\"practiceDate\" value=\"" . $sessInfo['practiceDate'] . "\" maxlength=\"5\" size=\"5\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_startTime\">Start time(HH:MM):</label><input id=\"in_startTime\" name=\"startTime\" value=\"" . $sessInfo['startTime'] . "\" maxlength=\"5\" size=\"5\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_endTime\">End time(HH:MM):</label><input id=\"in_endTime\" name=\"endTime\" value=\"" . $sessInfo['endTime'] . "\" maxlength=\"5\" size=\"5\"/></td>\n";
    echo "</tr><tr>\n";
    echo "<td class=\"form_text\"><label for=\"in_maxSlotsPer\">Max slots per pilot:</label><input id=\"in_maxSlotsPer\" name=\"maxSlotsPer\" value=\"" . $sessInfo['maxSlotsPer'] . "\" maxlength=\"2\" size=\"2\"/></td>\n";
    echo "<td class=\"form_text\"><label for=\"in_minutesPer\">Minutes per slot:</label><input id=\"in_minutesPer\" name=\"minutesPer\" value=\"" . $sessInfo['minutesPer'] . "\" maxlength=\"2\" size=\"2\"/></td>\n";
    echo "</tr>\n";
    echo "</tbody></table>\n";
    echo "<div class=\"submit\">\n";
    echo "<input type=\"hidden\" id=\"in_sessID\" name=\"sessID\" value=\"" . $sessInfo['sessID'] . "\"/>\n";
    echo "<input id=\"in_year\" type=\"hidden\" name=\"year\" value=\"" . $sessInfo['year'] . "\"/>\n";
    echo "<input id=\"in_ctstID\" type=\"hidden\" name=\"ctstID\" value=\"" . $sessInfo['ctstID'] . "\"/>\n";
    echo "<input class=\"submit\" name=\"submit\" type=\"submit\" value=\"" . 'Save Session' . "\"/>\n";
    echo "</div>\n";
    echo "</form>\n";
}

function processForm($sessInfo)
{
    $wasUpdated = false;
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "prsForm.php");
        $corrMsg = "<it>Internal: failed access to contest database</it>";
    } else
        if (isset ($sessInfo["submit"]))
        {
            // create or update record with post data
            $corrMsg = validatePost($sessInfo);
            //debugArr("prsForm post data", $sessInfo);
            if ($corrMsg == '')
            {
                // have valid data. update sets sessID

                $sessID = $sessInfo['sessID'];
                $fail = insertOrUpdateSession($db_conn, $sessInfo, $sessID);
                if ($fail == '')
                {
                    $sessInfo['sessID'] = $sessID;
                    $wasUpdated = true;
                } else
                {
                    $corrMsg = "<it>" . $fail . "</it>";
                }
            }
        } else
        {
            if (isset ($_GET['sessID']))
            {
                // read record for edit
                $sessID = $_GET['sessID'];
                $fail = retrieveSession($db_conn, $sessInfo, $sessID);
                if ($fail != '')
                {
                    notifyError($fail, "prsForm.php");
                    $corrMsg = "<it>Internal: failed access to session record of " . $sessID . "</it>";
                }
            } else
            {
                $contest = $_SESSION['contest'];
                $sessInfo['ctstID'] = $contest['ctstID'];
                $date = strtotime($contest['startDate']);
                $sessInfo['practiceDate'] = strftime('%m/%d', $date);
                $time = strtotime('08:00');
                $sessInfo['startTime'] = strftime('%H:%M', $time);
                $time = strtotime('19:00');
                $sessInfo['endTime'] = strftime('%H:%M', $time);
                $sessInfo['maxSlotsPer'] = 1;
                $sessInfo['minutesPer'] = 10;
            }
            //debugArr("prsForm record data", $sessInfo);
            $sessInfo['year'] = $_SESSION['ctst_year'];
            $sessInfo['ctstID'] = $_SESSION['ctstID'];
        }
    dbClose($db_conn);

    if ($wasUpdated)
    {
        getNextPage('prsTable.php');
    } else
    {
        // add contest form
        // $corrMsg has HTML content
        // $sessInfo has POST content
        startHead("Session Settings");
        echo "<link href=\"regform.css\" type=\"text/css\" rel=\"stylesheet\"/>\n";
        startContent();
        echo "<h1>Practice Session</h1>";
        verificationHeader("Contest official,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        echo '<p>Edit the settings for this practice session.</p>';
        sessionForm($sessInfo, "prsForm.php");
        echo '<div class="returnButton"><a href="prsTable.php">Return without saving</a></div>';
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
