<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("useful.inc");
require_once ("redirect.inc");
require_once ("query/userQueries.inc");
require_once ('data/timecheck.inc');
require_once ('practice.php');

/**
 * Compute type of each slot as status = open, available, or reserved.
 * Populate array $slotType as slot key -> status
*/
function encodeDesTypes($sessID, $start, $end, $interval, $curRsvd, $curDes, &$slotType)
{
   $slotCount = computeSlotCount($start, $end, $interval);
   for ($curIndex = 0; $curIndex < $slotCount; ++$curIndex)
   {
      $key = makeKey($sessID, $curIndex);
      if ($curRsvd[$key] == null)
      {
         // the slot is unreserved
         if ($curDes[$key] == null)
         {
            // undesignated
            $slotType[$key] = 'open';
         }
         else
         {
            // designated, may be redesignated
            $slotType[$key] = 'available';
         }
      } else
         {
            // the slot is reserved by a competitor
            $slotType[$key] = 'reserved';
         }
    }
    return $slotType;
}

/**
Output a schedule of practice slots for one date
*/
function practiceDay($db_conn, $submitURL, $userID, $sessID, $dateStr, $start, $end, $interval, $maxCt)
{
    $curRsvd = array (); // slot key => userID
    $curDes = array(); // slot key => designation key
    $rsvdName = array (); // slot key => label
    $slotType = array(); // slot key => type (open, current, reserved)
    findCurrentDesignations($db_conn, $sessID, $curDes, $rsvdName);
    findCurrentReservations($db_conn, $sessID, $curRsvd, $rsvdName);
    encodeDesTypes($sessID, $start, $end, $interval, $curRsvd, $curDes, $slotType);
    writePracticeDayTable($userID, $sessID, $dateStr, $start, $end, $interval, $maxCt, $rsvdName, $slotType, $curDes);
}

function isDesCat($catID, $catDes)
{
  $select = '';
  if ($catID == $catDes)
  {
     $select='checked="on"';
  }
  // debug('practiceSlotDes.isDesCat catID=' . $catID . ', catDes=' . $catDes . ', select='.$select);
  return $select;
}

function isDesClass($class, $classDes)
{
  $select = '';
  if ($class == $classDes)
  {
     $select='checked="on"';
  }
  return $select;
}

function designationOptions($catList, $class, $catID)
{
    // debug('designationOptions: class='.$class.', catID='.$catID);
    echo '<span class="form_select">'.
         '<label for="compCat">Designation:</label>'.
         '<fieldset id="compCat" legend="Designation">' . "\n";
    $col = 0;
    echo '<table><tbody><tr>';
    foreach ($catList as $cat)
    {
        if ($col != 0 && ($col % 5) == 0) echo '</tr><tr>';
        echo '<td><input class="form_select" type="radio" name="designation" value="' . 
        makeKey('category', $cat['catID']) .'" '. isDesCat($cat['catID'],$catID) . '>' . $cat['name'] . 
        '</input></td>' . "\n";
        ++$col;
    }
    echo '</tr><tr>';
    echo '<td><input class="form_select" type="radio" name="designation" value="' . 
         makeKey('class', 'power') . '" ' .isDesClass('power', $class) . '>' . 'power' . '</input></td>' . "\n";
    echo '<td><input class="form_select" type="radio" name="designation" value="' . 
         makeKey('class', 'glider') . '" ' . isDesClass('glider', $class) . '>' . 'glider' . '</input></td>' . "\n";
    echo '</tr></tbody></table>';
    echo '</fieldset></span>' . "\n";
}

/**
 * designate slot for type, class, catID, return non-empty string on error.
 */
function designateSlot($db_conn, $sessID, $slotIdx, $type, $class, $catID)
{
    $corrmsg = '';
    $query = 'insert into slot_restriction (sessID, slotIndex, restrictionType, class, catID)' .
    ' values (' . intSQL($sessID) . ',' . intSQL($slotIdx) . ',' . strSQL($type,8) .
    ','. strSQL($class,6) .','.intSQL($catID). ')';
    // debug('designate slot, ' . $label . ':' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        $corrmsg = 'Data error; slot ' . $label . ' designated before insert?';
    }
    return $corrmsg;
}

/**
 * redesignate slot for type, class, catID, return non-empty string on error.
 */
function redesignateSlot($db_conn, $sessID, $slotIdx, $type, $class, $catID)
{
    $corrmsg = '';
    $query = 'update slot_restriction set '. 
    ' restrictionType = ' .  strSQL($type,8) . 
    ' , class = ' . strSQL($class,6) .
    ' , catID = ' . intSQL($catID) .
    ' where sessID = ' . intSQL($sessID) .
    ' and slotIndex = ' . intSQL($slotIdx);
    // debug('redesignate slot, ' . $label . ':' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        $corrmsg = 'Data error; slot ' . $label . ' may have been freed before update?';
    }
    return $corrmsg;
}

/**
 * Free slot for type, class, catID, return non-empty string on error.
 */
function freeSlot($db_conn, $sessID, $slotIdx, $label)
{
    $corrmsg = '';
    $query = 'delete from slot_restriction ' .
    'where sessID = ' . intSQL($sessID) .
    ' and slotIndex = ' . intSQL($slotIdx);
    // debug('free slot, ' . $label . ':' . $query);
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        notifyError($fail, 'practiceSlotDes.freeSlot');
        $corrmsg = 'Failed to release ' . $label;
    }
    return $corrmsg;
}

/*
 * Determine whether slot preselected for class or category is no longer selected for same.
 * slots: posted array
 * slot: current slot key (sessID+index)
 * selType: type preselected for slot
 * selValue: category or class preselected for slot
 * type: type of slot now posting (class or category)
 * class: class of slot now posting 
 * category: category of slot now posting
 * return true if the slot was preselected for the type, class, category of the slot now posting
 */
function deselectedForThis($slots, $slot, $selType, $selValue, $type, $class, $catID)
{
   $deselected = !isset ($slots['slot:' . $slot]);
   if ($deselected)
   {
     $deselected = $selType == $type;
     if ($deselected)
     {
       if ($type == 'category')
       {
          $deselected = $selValue == $catID;
       }
       else 
       {
          $deselected = $selValue == $class;
       }
     }
   }
   return $deselected; 
}

/*
Validate count of selections, designate slots.
db_conn: database connection token
slots: post data with checked slots
userID: id of voter
slotList: return string containing the list of designated slots
Return error string if error, else empty string
*/
function designateSelectedSlots($db_conn, $slots, $userID, $practiceDate, $practiceStart, $practiceInterval, $type, $class, $catID, & $slotList)
{
    // debugArr("practiceSlotDes post data: ", $slots);
    $corrMsg = '';
    $slotList = '';
    
    foreach ($slots as $key => $value)
    {
        if (strncmp($key, 'slot:', 5) == 0)
        {
                $slot = substr($key, 5);
                parsekey($slot, $sessID, $slotIdx);
                $label = makeDescription($practiceDate[$sessID], $practiceStart[$sessID], $slotIdx, $practiceInterval[$sessID]);
                if (isset ($slots['preselect:' . $slot]))
                {
                   $fail = redesignateSlot($db_conn, $sessID, $slotIdx, $type, $class, $catID);
                }
                else
                {
                   $fail = designateSlot($db_conn, $sessID, $slotIdx, $type, $class, $catID);
                }
                if ($fail != '')
                {
                    $corrMsg .= '<li>' . $fail . '</li>';
                }
        }
        if (strncmp($key, 'preselect:', 10) == 0)
        {
            $slot = substr($key, 10);
            parsekey($value, $selType, $selValue);
// DCL todo make value contain selected type key
            if (deselectedForThis($slots, $slot, $selType, $selValue, $type, $class, $catID))
            {
                parsekey($slot, $sessID, $slotIdx);
                $label = makeDescription($practiceDate[$sessID], $practiceStart[$sessID], $slotIdx, $practiceInterval[$sessID]);
                $fail = freeSlot($db_conn, $sessID, $slotIdx, $label);
                if ($fail != '')
                {
                    $corrMsg .= '<li>' . $fail . '</li>';
                }
            }
        }
    }
    return $corrMsg;
}

/**
 * Query practice sessions for contest.
 * ctstID - identifier of contest
 * practiceDate array indexed by session ID records practiceDate
 * practiceStart array indexed by session ID records startTime
 * practiceEnd array indexed by session ID records endTime
 * practiceInterval array indexed by session ID records minutesPer
 * maxSlotsPer array indexed by session ID records maxSlotsPer
 * return failure string, empty if successful
*/
function querySessionsForContest($db_conn, $ctstID, &$practiceDate, &$practiceStart, &$practiceEnd, &$practiceInterval, &$maxSlotsPer)
{
    $fail = '';
        $query = 'select * from session ';
        $query .= 'where ctstID = ' . intSQL($ctstID);
        $query .= ' order by practiceDate, startTime';
        //debug('practiceSlots ' . $query);
        $result = dbQuery($db_conn, $query);
        if ($result === fail)
        {
            $fail = notifyError(dbErrorText(), 'practiceSlot.php');
        } else
        {
            while ($row = dbFetchAssoc($result))
            {
                $sessID = $row['sessID'];
                //debugArr('practice session', $row);
                $practiceDate[$sessID] = $row['practiceDate'];
                $practiceStart[$sessID] = $row['startTime'];
                $practiceEnd[$sessID] = $row['endTime'];
                $practiceInterval[$sessID] = $row['minutesPer'];
                $maxSlotsPer[$sessID] = $row['maxSlotsPer'];
            }
        }
    return $fail;
}

function doSlotDesignation()
{
    $wasUpdated = false;
    $isRegistered = false;
    $corrMsg = '';
    $slotList = ''; // initialized by designateSelectedSlots() if $wasUpdated == true
    $ctstID = $_SESSION['ctstID'];
    $userID = $_SESSION['userID'];
    $contest = $_SESSION['contest'];
    $slots = $_POST;
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "practiceSlotDes.php");
        $corrMsg = "<it>Internal: failed access to contest database</it>";
    } else
    {
        retrieveExistingRegData($db_conn, $userID, $ctstID, & $registrant);
    }
    $practiceDate = array ();
    $practiceStart = array ();
    $practiceEnd = array ();
    $practiceInterval = array ();
    $maxSlotsPer = array ();
    $fail = querySessionsForContest($db_conn, $ctstID, $practiceDate, $practiceStart, $practiceEnd, $practiceInterval, $maxSlotsPer);
    $type = 'class';
    $class = 'glider';
    $catID = null;    
    if ($fail == '' && isset ($slots["submit"]))
    {
        // begin form processing
    $designation = $slots['designation'];
    parsekey($designation, $type, $des);
    if ($type == 'category')
    {
       $catID = $des;
       $class = null;
    }
    else
    {
       $catID = null;
       $class = $des;
    }
    // debug("practiceSlotDes designation ".$designation.', type '.$type.', class '.$class.', catID '.$catID);
    if ($corrMsg == '')
        {
            // have valid data
            $corrMsg = designateSelectedSlots($db_conn, $slots, $userID, $practiceDate, $practiceStart, $practiceInterval, $type, $class, $catID, $slotList);
            if ($corrMsg == '')
            {
                $wasUpdated = true;
            }
        }
    }
    startHead("Designate practice slots");
    echo '<link href = "regform.css" type = "text/css" rel = "stylesheet"/>' . "\n";
    if ($fail == '')
    {
        // show reservation form
        $currentCount = 0;
        $sessionCounts = array ();

        // $corrMsg has HTML content
        echo '<script type="text/javascript" src="practiceDes.js"></script>' . "\n";
        startContent("onload='checkEnabledSubmit()'");
        echo "<h1>Designate practice slots</h1>";
        verificationHeader("Contest Director,");
        if ($corrMsg != '')
        {
            echo '<ul class="error">' . $corrMsg . '</ul>';
        }

        $catList = null;
        $fail = getCategoryList($db_conn, $ctstID, $catList);
        if ($fail != '')
        {
            notifyError($fail, "register.php");
            $corrMsg .= "<it>Internal: failed access to category records of " . $ctstID . "</it>";
        }
        
        echo '<p>Select a category or class.  Select practice slots ';
        echo 'to designate for that category or class.';
        echo ' The form shows current reservations and designations.';
        echo ' Remove checks from current designations to clear them.</p>';
        echo "<form class=\"recordForm\" action=\"practiceSlotDes.php\" method=\"post\">\n";
        designationOptions($catList, $class, $catID);
        foreach ($practiceDate as $sessID => $pDate)
        {
            $start = strtotime($practiceStart[$sessID]);
            $end = strtotime($practiceEnd[$sessID]);
            $maxCt = $maxSlotsPer[$sessID];
            $dateStamp = strtotime($pDate);
            $dateHead = date('l, F j', $dateStamp);
            echo '<h4>' . $dateHead . '</h4>';
            practiceDay($db_conn, 'practiceSlotDes.php', $userID, $sessID, $pDate, $start, $end, $practiceInterval[$sessID], $maxCt);
        }
        echo '<input class = "submit" name = "submit" type = "submit" value = "Designate slots"/>' . "\n";
        echo "</form>\n";
    } else
    {
        startContent();
        echo "<h1>Designate practice slots</h1>";
        if ($fail != '')
        {
            echo '<p class = "error">'.$fail.'</p>';
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

if (isContestAdmin())
{
    doSlotDesignation();
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
