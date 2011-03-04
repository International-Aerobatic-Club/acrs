<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("query/userQueries.inc");
require_once ("data/encodeHTML.inc");
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ('practice.php');

function writeReservationLine($label, $record)
{
    echo '<tr class="practiceSlotPilot">' . "\n";
    echo '<td class="practiceSlotLabel">' . strhtml($label) . "</td>\n";
    echo '<td class="practiceSlotPilot">' . strhtml($record["givenName"]) .
    ' ' . strhtml($record["familyName"]) . ', ' . strhtml($record['compCat']) . "</td>\n";
    echo '<td class="practiceSlotPhone">' . strhtml($record['contactPhone']) . "</td>\n";
    echo "</tr>\n";
    echo '<tr class="practiceSlotPlane">' . "\n";
    echo '<td class="practiceSlotReg">' . strhtml($record['compClass']) . ' ' .
    strhtml($record['airplaneRegID']) . "</td>\n";
    echo '<td class="practiceSlotMnM">' . strhtml($record['airplaneMake']) .
    ' ' . strhtml($record['airplaneModel']) . "</td>\n";
    echo '<td class="practiceSlotColors">' . strhtml($record['airplaneColors']) . "</td>\n";
    echo "</tr>\n";
}

function writeBlankLine($slotLabel)
{
    echo '<tr class="practiceSlotPilot">' . "\n";
    echo '<td class="practiceSlotLabel">' . strhtml($slotLabel) . "</td>\n";
    echo '<td class="practiceSlotBlank" colspan="2"/>' . "\n";
    echo '</tr><tr class="practiceSlotBlank"><td colspan="3" class="practiceSlotBlank"/></tr>' . "\n";
}

/*
 * Write report of practice slots
 * $rsvdPilot array of resrvation data indexed by slot key (see makeKey)
 */
function writePracticeDay($rsvdPilot, $sessID, $practiceDate, $start, $end, $interval)
{
    $interval *= 60; // seconds per minute
    //debug('start:'.$start.'; end:'.$end.'; interval:'.$interval);
    $fullInterval = $end - $start;
    $slotCount = intval($fullInterval / $interval);
    //debug('fullInterval:'.$fullInterval.'; slotCount:'.$slotCount);
    $curTime = $start;
    echo '<table class="practiceSlot"><thead><tr class="practiceDate"><th colspan="3">' .
    $practiceDate . "<tbody>\n";
    for ($curSlot = 0; $curSlot < $slotCount; ++ $curSlot)
    {
        $curKey = makeKey($sessID, $curSlot);
        //debug($curKey);
        $slotLabel = makeLabel($curTime, $interval);
        if ($rsvdPilot[$curKey])
        {
            writeReservationLine($slotLabel, $rsvdPilot[$curKey]);
        } else
        {
            writeBlankLine($slotLabel);
        }
        $curTime += $interval;
    }
    echo '</tbody></table>' . "\n";
}

/*
 * Write report of practice slots
 * $rsvdPilot array of resrvation data indexed by slot key (see makeKey)
 */
function writeReport($db_conn, $rsvdPilot)
{
    $fail = '';
    $query = 'select * from session where ctstId = ' . $_SESSION['ctstID'] .
    ' order by practiceDate, startTime';
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        $fail = notifyError(dbErrorText(), 'reportPracticeSlots.php');
    } else
    {
        while ($row = dbFetchAssoc($result))
        {
            $sessID = $row['sessID'];
            $start = strtotime($row['startTime']);
            $end = strtotime($row['endTime']);
            $interval = $row['minutesPer'];
            $practiceDate = $row['practiceDate'];
            writePracticeDay($rsvdPilot, $sessID, $practiceDate, $start, $end, $interval);
        }
    }
    return $fail;
}

function reportPractice($db_conn)
{
    $query = 'select givenName, familyName, contactPhone, d.sessID, slotIndex, airplaneRegID,' .
    ' airplaneMake, airplaneModel, airplaneColors, category, class' .
    ' from registrant a, practice_slot b, registration c, session d, ctst_cat e, reg_type f' .
    ' where a.userID = b.userID ' .
    ' and d.ctstID = ' . $_SESSION['ctstID'] .
    ' and b.sessID = d.sessID' .
    ' and f.ctstID = d.ctstID' .
    ' and f.userID = b.userID' .
    ' and c.regID = f.regID' .
    ' and e.catID = c.catID';
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        $fail = notifyError(dbErrorText(),'reportPracticeSlots.php');
    }
    if ($fail == '')
    {
        $rsvdPilot = array (); // slot key => data record
        while ($record = dbFetchAssoc($result))
        {
            $key = makeKey($record['sessID'], $record['slotIndex']);
            $rsvdPilot[$key] = $record;
        }
        writeReport($db_conn, $rsvdPilot);
    } else
    {
        notifyError($fail, "reportPracticeSlots.php");
        echo '<p style="color:red; font-weight:bold">' . $fail . '</p>';
    }
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "reportPracticeSlots.php");
    $corrMsg = "<it>Internal: failed access to contest database</it>";
    $readRecord = FALSE;
} else
{
    startHead("Practice Slot Reservations");
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
    echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
    startContent();
    echo '<h1 class="noprint">Practice Slot Reservations</h1>';
    echo '<p class="noprint"><input style="margin-right:20px" type="button" onClick="window.print()" ' .
    'value="Print This Page"/><a href="index.php">Return to registration</a></p>';
    if ($corrMsg != '')
    {
        echo '<ul class="error">' . $corrMsg . '</ul>';
    } else
    {
        if (isContestOfficial())
        {
            reportPractice($db_conn);
        } else
        {
            echo '<p class="error">Restricted to contest officials.</p>';
        }
    }
    echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
    endContent();
}
dbClose($db_conn);
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
