<?php
/*
 * Created on Sep 11, 2007
 */

require_once('data/encodeHTML.inc');

/**
 * Format a label for a time slot.
 * $curStart start time of the time slot
 * $interval length of the time slot
 * returns text string representing the slot
 */
function makelabel($curStart, $interval)
{
    $slotText = strftime('%H:%M', $curStart) . '-' .
    strftime('%H:%M', $curStart + intval($interval));
    return $slotText;
}

/**
 * Format a date and time description of the time slot given
 * practiceDate: SQL format text date
 * practiceStart: text start time
 * slotIndex: zero based index
 * interval: minutes per session
 */
function makeDescription($practiceDate, $practiceStart, $slotIndex, $interval)
{
    $d = strtotime($practiceDate);
    $d = strtotime($practiceStart, $d);
    $interval = intval($interval) * 60;
    $d += intval($slotIndex) * $interval;
    return strftime('%A, %B %d ', $d) . makelabel($d, $interval);
}

/*
 * Create a key from two data values by concatenation with a delimiter.
 * See parseKey().
 * partA - first part of key
 * partB - second part of key
 * return parseable, html safe key composed of partA, partB
 */
function makeKey($partA, $partB)
{
    return $partA . '::' . $partB;
}

/*
 * Parse two part key. Inverse of makeKey().
 * key input slot identifier
 * partA return first part of key; null if not a key
 * partB return second part of key; null if not a key
 */
function parseKey($key, & $partA, & $partB)
{
    $partA = $partB = null;
    $i1 = strpos($key, '::');
    $rem = '';
    if ($i1 > 0)
    {
        $partA = substr($key, 0, $i1);
        $partB = substr($key, ($i1 + 2));
    }
    //debug('parseKey: key=' . $key . ', i1=' . $i1 . ', sessID=' . $partA . ', index=' . $partB);
}

/**
Write one practice slot checkbox form entry.
*/
function writeOpenSlot($slotText, $key, $sessID)
{
    echo '<td class="practiceSlot"><div class="resv_time">' .
    '<input class="form_check" type="checkbox" id="' .
    $key . '" name="slot:' . $key;
    echo '" onclick="checkSlot(this, '.$sessID.')"';
    echo '>' . $slotText . '</input></div>';
    echo '<div class="resv_empty"/>';
    echo "</td>\n";
}

/**
Write one practice slot checkbox form entry, designated slot available to competitor.
*/
function writeDesignatedSlot($slotText, $key, $sessID, $rsvdName)
{
    echo '<td class="practiceSlot"><div class="resv_time">' .
    '<input class="form_check" type="checkbox" id="' .
    $key . '" name="slot:' . $key;
    echo '" onclick="checkSlot(this, '.$sessID.')"';
    echo '>' . $slotText . '</input></div>';
    echo '<div class="resv_current">' . $rsvdName . '</div>';
    echo "</td>\n";
}

function writeCurrentReservation($slotText, $key, $sessID, $rsvdName, $preValue='on', $isChecked=true)
{
    echo '<td class="practiceSlot"><div class="resv_time">' .
    '<input class="form_check" type="checkbox" id="' .
    $key . '" name="slot:' . $key;
    echo '" onclick="checkSlot(this, '.$sessID.')"';
    if ($isChecked) echo ' checked="true"';
    echo '>' . $slotText . '</input></div>';
    echo '<div class="resv_current">' . $rsvdName . '</div>';
    echo '<input type="hidden" name="preselect:' . $key . '" value="'.$preValue.'"/>';
    echo "</td>\n";
}

/**
Write one practice slot reservation.
*/
function writeReservation($slotText, $rsvdName)
{
    echo '<td class="practiceSlot"><div class="resv_time">' . $slotText .
    '</div><div class="resv_name">' . $rsvdName . "</div></td>\n";
}

function writeSlot($slotText, $key, $slotType, $sessID, $rsvdName, $selValue='on')
{
                switch ($slotType)
                {
                case 'open': // the slot is unreserved
                    writeOpenSlot($slotText, $key, $sessID);
                    break;
                case 'current':  // the slot is reserved by the current competitor
                    writeCurrentReservation($slotText, $key, $sessID, $rsvdName);
                    break;
                case 'available':  // the slot is designated, may be redesignated
                    writeCurrentReservation($slotText, $key, $sessID, $rsvdName, $selValue, false);
                    break;
                case 'designated':  // the slot is designated, may be reserved
                    writeDesignatedSlot($slotText, $key, $sessID, $rsvdName);
                    break;
                case 'reserved': // the slot is reserved by another competitor
                    writeReservation($slotText, $rsvdName);
                    break;
                }
}

/**
 * Searches practice_slot for current reservations.
 * Updates $curRsvd = array (); // slot key => userID.
 * Updates $rsvdName = array (); // slot key => label 
*/
function findCurrentReservations($db_conn, $sessID, &$curRsvd, &$rsvdName)
{
    // gather data about current reserved slots
    $query = "select a.userID, a.givenName, a.familyName, b.slotIndex" .
    " from registrant a, practice_slot b" .
    ' where b.sessID = ' . $sessID .
    ' and a.userID = b.userID';
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if ($result)
    {
        $row = dbFetchRow($result);
    }
    while ($row)
    {
        $resvUID = $row[0];
        $slotIdx = $row[3];
        $key = makeKey($sessID, $slotIdx);
        $curRsvd[$key] = $resvUID;
        $rsvdName[$key] = strhtml($row[2] . ', ' . substr($row[1], 0, 1) . '.');
        //debug('reservation: ' . $rsvdName[$key] . ' is userID ' . $resvUID . ', sessID ' . $sessID . ', slot ' . $slotIdx);
        $row = dbFetchRow($result);
    }
    //debugArr('reservations:', $curRsvd);
}

/**
 * Searches slot_restriction for current designations.
 * Updates $curDes = array (); // slot key => designation key.
 * Updates $rsvdName = array (); // slot key => label 
*/
function findCurrentDesignations($db_conn, $sessID, &$curDes, &$rsvdName)
{
    $catName = array(); // catID => name
    // gather category names for slots designated to category
    $query = 'select a.catID, a.name from ctst_cat a where ' . 
      'a.catID in (select distinct catID from slot_restriction b where ' .
      'b.sesSID = ' . $sessID . " and b.restrictionType = 'category')";
    // debug('practice.findCurrentDesignations:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result)
    {
        $row = dbFetchRow($result);
    }
    while ($row)
    {
        $catID = $row[0];
        $name = $row[1];
        $catName[$catID] = strhtml($name);
        // debug('practice.findCurrentDesignations category name for ' . $catID . ' is ' . $name);
        $row = dbFetchRow($result);
    }

    // gather data about current designated slots
    $query = 'select slotIndex, restrictionType, class, catID ' .
      'from slot_restriction where sessID = ' . $sessID;
    // debug('practice.findCurrentDesignations ' . $query);
    $result = dbQuery($db_conn, $query);
    if ($result)
    {
        $row = dbFetchRow($result);
    }
    while ($row)
    {
        $slotIdx = $row[0];
        $key = makeKey($sessID, $slotIdx);
        $desType = $row[1];
        $desClass = $row[2];
        $desCat = $row[3];
        if ($desType == 'class')
        {
           $curDes[$key] = makeKey($desType, $desClass);
           $rsvdName[$key] = $desClass;
        }
        else
        {
           $curDes[$key] = makeKey($desType, $desCat);
           $rsvdName[$key] = $catName[$desCat];
        }
        // debug('practice.findCurrentDesignations designation on slot ' . $key . ' is ' . $rsvdName[$key] . ' for ' . $curDes[$key] . ', sessID ' . $sessID . ', slot ' . $slotIdx);
        $row = dbFetchRow($result);
    }
    // debugArr('practice.findCurrentDesignations designations:', $curDes);
}

function computeSlotCount($start, $end, $intervalInSeconds)
{
    //debug('start:'.$start.'; end:'.$end.'; interval:'.$intervalInSeconds);
    $fullInterval = $end - $start;
    $slotCount = intval($fullInterval / $intervalInSeconds);
    //debug('fullInterval:'.$fullInterval.'; slotCount:'.$slotCount);
    return $slotCount;
}

/**
 * output table of reserved slots
 */
function writePracticeDayTable($userID, $sessID, $dateStr, $start, $end, $interval, $maxCt, $rsvdName, $slotType, $curDes)
{
    // need to skip ahead column amounts going across row,
    // interval amount from one row to the next
    $interval *= 60; // seconds per minute
    $columnCount = 5;
    $slotCount = computeSlotCount($start, $end, $interval);
    $columnLength = intval($slotCount / $columnCount);
    if ($slotCount % $columnCount > 0)
        ++ $columnLength;
    $columnInterval = $columnLength * $interval;
    //debug('columnLength:'.$columnLength.'; columnInterval:'.$columnInterval);
    echo '<table class = "practiceDay" > <tbody >';
    for ($tblRow = 0; $tblRow < $columnLength; ++ $tblRow)
    {
        echo '<tr class = "practiceDay" >' . "\n";
        $curStart = $start + $tblRow * $interval;
        $curIndex = $tblRow;
        for ($tblCol = 0; $tblCol < $columnCount; ++ $tblCol)
        {
            if ($curStart < $end)
            {
                $slotText = makeLabel($curStart, $interval);
                //$slotLabels[$curIndex] = $slotText;
                $key = makeKey($sessID, $curIndex);
                $des = $curDes[$key];
                if ($des == null) $des = 'on';
                // debug('practice.writePracticeDayTable has des '.$des);
                writeSlot($slotText, $key, $slotType[$key], $sessID, $rsvdName[$key], $des);
            } else
            {
                // write empty cell
                echo '<td/>';
            }
            $curStart += $columnInterval;
            $curIndex += $columnLength;
        }
        echo "</tr>\n";
    }
    echo "</tbody></table>\n";
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
