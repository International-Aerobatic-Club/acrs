<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("ui/emailNotices.inc");
require_once ("useful.inc");
require_once ("redirect.inc");
require_once ("query/userQueries.inc");
require_once ('data/timecheck.inc');
require_once ('practice.php');

function testAvailableSlot($catID, $class, $curDes)
{
   parseKey($curDes, $desType, $desValue);
   if (($desType == 'category' && $catID != $desValue) || ($desType == 'class' && $class != $desValue))
      {
         $key = 'reserved';
      }
   else
   {
      $key = 'designated';
   }
   return $key;
}

/**
 * Compute type of each slot as status = open, current, or reserved.
 * Populate array $slotType as slot key -> status
*/
function encodeSlotTypes($userID, $catID, $class, $sessID, $start, $end, $interval, $curDes, $curRsvd, &$slotType)
{
   $slotCount = computeSlotCount($start, $end, $interval);
   for ($curIndex = 0; $curIndex < $slotCount; ++$curIndex)
   {
      $key = makeKey($sessID, $curIndex);
      // three cases
      $uidRsvd = $curRsvd[$key];
      if ($uidRsvd == null)
      {
         // the slot is unreserved
         if ($curDes[$key] != null)
         {
            $slotType[$key] = testAvailableSlot($catID, $class, $curDes[$key]);
         }else{
            $slotType[$key] = 'open';
         }
         
      } else
         if ($uidRsvd == $userID)
         {
            // the slot is reserved by the current competitor
            $slotType[$key] = 'current';
         } else
         {
            // the slot is reserved by another competitor
            $slotType[$key] = 'reserved';
         }
    }
    return $slotType;
}

/**
Output a schedule of practice slots for one date
*/
function practiceDay($db_conn, $submitURL, $userID, $catID, $class, $sessID, $dateStr, $start, $end, $interval, $maxCt)
{
    $curDes = array(); // slot key => designation key
    $curRsvd = array (); // slot key => userID
    $rsvdName = array (); // slot key => label 
    $slotType = array(); // slot key => type (open, current, reserved)
    findCurrentDesignations($db_conn, $sessID, $curDes, $rsvdName);
    findCurrentReservations($db_conn, $sessID, $curRsvd, $rsvdName);
    encodeSlotTypes($userID, $catID, $class, $sessID, $start, $end, $interval, $curDes, $curRsvd, $slotType);
    writePracticeDayTable($userID, $sessID, $dateStr, $start, $end, $interval, $maxCt, $rsvdName, $slotType, $curDes);
}

/**
 * Reserve slot for user, return non-empty string on error.
 */
function reserveSlot($db_conn, $sessID, $slotIdx, $userID, $label)
{
    $corrmsg = '';
    $query = 'insert into practice_slot (sessID, slotIndex, userID)' .
    ' values (' . intSQL($sessID) . ',' . intSQL($slotIdx) . ',' . intSQL($userID) . ')';
    //debug('add slot, ' . $label . ':' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        $corrmsg = 'Another person reserved ' . $label . ' while you were making your selection.';
    }
    return $corrmsg;
}

/**
 * Free slot for user, return non-empty string on error.
 */
function freeSlot($db_conn, $sessID, $slotIdx, $userID, $label)
{
    $corrmsg = '';
    $query = 'delete from practice_slot ' .
    'where sessID = ' . intSQL($sessID) .
    ' and slotIndex = ' . intSQL($slotIdx) .
    ' and userID = ' . $userID;
    //debug('free slot, ' . $label . ':' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        notifyError($fail, 'practiceSlot.freeSlot');
        $corrmsg = 'Failed to release ' . $label;
    }
    return $corrmsg;
}

/*
Validate count of selections, reserve slots.
db_conn: database connection token
slots: post data with checked slots
userID: id of voter
slotList: return string containing the list of reserved slots
Return error string if error, else empty string
*/
function reserveSelectedSlots($db_conn, $slots, $userID, $practiceDate, $practiceStart, $practiceInterval, & $slotList)
{
    //debugArr("post data: ", $slots);
    $corrMsg = '';
    $slotList = '';
    foreach ($slots as $key => $value)
    {
        if (strncmp($key, 'slot:', 5) == 0)
        {
            $fail = '';
            $slot = substr($key, 5);
            $sessID = $slotIdx = null;
            parsekey($slot, $sessID, $slotIdx);
            $label = makeDescription($practiceDate[$sessID], $practiceStart[$sessID], $slotIdx, $practiceInterval[$sessID]);
            if (!isset ($slots['preselect:' . $slot]))
            {
                $fail = reserveSlot($db_conn, $sessID, $slotIdx, $userID, $label);
                if ($fail != '')
                {
                    $corrMsg .= '<li>' . $fail . '</li>';
                }
            }
            if ($fail == '')
            {
                if ($slotList != '') $slotList .= "\n";
                $slotList .= $label;
            }
        }
        if (strncmp($key, 'preselect:', 10) == 0)
        {
            $slot = substr($key, 10);
            if (!isset ($slots['slot:' . $slot]))
            {
                $sessID = $slotIdx = null;
                $sessID = $slotIdx = null;
                parsekey($slot, $sessID, $slotIdx);
                $label = makeDescription($practiceDate[$sessID], $practiceStart[$sessID], $slotIdx, $practiceInterval[$sessID]);
                $fail = freeSlot($db_conn, $sessID, $slotIdx, $userID, $label);
                if ($fail != '')
                {
                    $corrMsg .= '<li>' . $fail . '</li>';
                }
            }
        }
    }
    return $corrMsg;
}

function doPracticeSlots()
{
    $wasUpdated = false;
    $isRegistered = false;
    $corrMsg = '';
    $slotList = ''; // initialized by reserveSelectedSlots() if $wasUpdated == true
    $ctstID = $_SESSION['ctstID'];
    $userID = $_SESSION['userID'];
    $contest = $_SESSION['contest'];
    $slots = $_POST;
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "register.php");
        $corrMsg = "<it>Internal: failed access to contest database</it>";
    } else
    {
        retrieveExistingRegData($db_conn, $userID, $ctstID, & $registrant);
        $isRegistered = $registrant && isSelected($registrant, "compType", "competitor");
        $isRegistered &= havePracticeRegistration($contest, $registrant);
        $isRegistered &= (!sqlIsTrue($contest['reqPmtForPracticeReg']) || checkPaidInFull($reg));
    }
    $practiceDate = array ();
    $practiceStart = array ();
    $practiceEnd = array ();
    $practiceInterval = array ();
    $maxSlotsPer = array ();
    $haveSession = false;
    if ($isRegistered)
    {
        $query = 'select * from session ';
        $query .= 'where ctstID = ' . intSQL($ctstID);
        $query .= ' order by practiceDate, startTime';
        //debug('practiceSlots ' . $query);
        $result = dbQuery($db_conn, $query);
        if ($result === false)
        {
            $fail = notifyError(dbErrorText(), 'practiceSlot.php');
        } else
        {
            while ($row = dbFetchAssoc($result))
            {
                $haveSession = true;
                $sessID = $row['sessID'];
                //debugArr('practice session', $row);
                $practiceDate[$sessID] = $row['practiceDate'];
                $practiceStart[$sessID] = $row['startTime'];
                $practiceEnd[$sessID] = $row['endTime'];
                $practiceInterval[$sessID] = $row['minutesPer'];
                $maxSlotsPer[$sessID] = $row['maxSlotsPer'];
            }
        }
    }
    if ($haveSession && isset ($slots["submit"]))
    {
        // begin form processing
        if ($corrMsg == '')
        {
            // have valid data
            $corrMsg = reserveSelectedSlots($db_conn, $slots, $userID, $practiceDate, $practiceStart, $practiceInterval, $slotList);
            if ($corrMsg == '')
            {
                $wasUpdated = true;
            }
        }
    }
    startHead("Practice slot reservation");
    echo '<link href = "regform.css" type = "text/css" rel = "stylesheet"/>' . "\n";
    if ($haveSession)
    {
        // show reservation form
        $currentCount = 0;
        $sessionCounts = array ();
        foreach ($maxSlotsPer as $sessID => $maxSlots)
        {
            $query = 'select count(*) from practice_slot' .
            ' where sessID = ' . $sessID .
            ' and userID = ' . intSQL($userID);
            //debug('practiceSlot.php ' . $sessID . ' counts:' . $query);
            $result = dbQuery($db_conn, $query);
            if ($result === fail)
            {
                $fail = notifyError(dbErrorText(), 'practiceSlot.php');
            } else
            {
                if ($row = dbFetchRow($result))
                {
                    $sessionCounts[$sessID] = $row[0];
                    $currentCount += intval($row[0]);
                }
            }
        }

        // $corrMsg has HTML content
        echo '<script type="text/javascript" src="practiceSlot.js"></script>' . "\n";
        echo '<script type="text/javascript">' . "\n";
        echo 'function pageLoaded(){';
        echo 'initReservationTotal(' . $contest['maxPracticeSlots'] . ', ' . $currentCount . ');';
        foreach ($sessionCounts as $sessID => $sessionCount)
        {
            echo 'initReservationSession(' . $sessID . ', ' . $maxSlotsPer[$sessID] . ', ' . $sessionCount . ');';
        }
        echo 'checkEnabledSubmit();}</script>';
        startContent("onload='pageLoaded()'");
        echo "<h1>Practice Slot Reservation</h1>";
        verificationHeader("Competitor,");
        if ($corrMsg != '')
        {
            echo '<ul class="error">' . $corrMsg . '</ul>';
        }
        if ($wasUpdated)
        {
            sendPracticeReservationEmail($registrant, $slotList);
            echo '<p>Your reservation is updated as:'."<ul><li>".
            str_replace("\n","</li><li>",$slotList)."</li></ul>".
            'You will receive email confirmation.</p>';
            echo '<div class="returnButton"><a href = "index.php">Return to registration</a ></div>';
        }
        
        echo '<p>Please avoid scheduling a slot within forty minutes of';
        echo ' another pilot using the same airplane. The form does';
        echo ' <b>not</b> (currently) enforce this. The contest director';
        echo ' or designee may rearrange slots with discretion.';
        echo ' You will receive notification of any changes.';
        echo ' The form shows current reservations.</p>';
        echo "<form class=\"recordForm\" action=\"practiceSlot.php\" method=\"post\">\n";
        foreach ($practiceDate as $sessID => $pDate)
        {
            $start = strtotime($practiceStart[$sessID]);
            $end = strtotime($practiceEnd[$sessID]);
            $maxCt = $maxSlotsPer[$sessID];
            $dateStamp = strtotime($pDate);
            $dateHead = date('l, F j', $dateStamp);
            echo '<h4>' . $dateHead . '</h4>';
            echo '<p>Select no more than '. $maxCt .' slot';
            if ($maxCt > 1) echo 's';
            echo ' in this practice session.</p>';
            practiceDay($db_conn, 'practiceSlot.php', $userID, $registrant['catID'], $registrant['class'], $sessID, $pDate, $start, $end, $practiceInterval[$sessID], $maxCt);
        }
        echo '<p>All unassigned time slots will become available for assignment on-site' .
        ' according to the contest schedule.  The practice coordinator will assign open' .
        ' slots on a first-come basis, in person, to the pilot flying.  He or she will not' .
        ' accept telephone or proxy reservations.</p>';
        echo '<div class="error" id="ttlWarning"><p>Select no more than '.$contest['maxPracticeSlots'];
        echo ($contest['maxPracticeSlots'] > 1 ? ' slots.' : ' slot.'); 
        echo '</p></div>'."\n";
        echo '<div class="error" id="ssnCtWarning"><p>You have made too many selections for one session.</p></div>'."\n";
        echo '<input class = "submit" name = "submit" type = "submit" value = "Update your selection"/>' . "\n";
        echo "</form>\n";
    } else
    {
        startContent();
        echo "<h1>Practice Slot Reservation</h1>";
        if (!$isRegistered)
        {
            echo '<p class = "error"> Only competitors with paid registrations ';
            echo 'may reserve practice slots.</p>';
        } else
        {
            echo '<p class = "error"> There are no practice sessions ' .
            'scheduled for this contest.</p>';
        }
    }
    echo '<div class="returnButton"><a href = "index.php">Return to registration</a ></div>';
    endContent();
    dbClose($db_conn);
}

if (isRegOpen())
{
    doPracticeSlots();
} else
{
    startHead("Practice Slot Reservation");
    echo '<link href = "regform.css" type = "text/css" rel = "stylesheet"/>' . "\n";
    startContent();
    echo '<p class="error">On-line registration is closed.  On-site registration will open before the contest.</p>';
    echo '<div class="returnButton"><a href="index.php">List of registrants</a></div>';
    endContent();
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
