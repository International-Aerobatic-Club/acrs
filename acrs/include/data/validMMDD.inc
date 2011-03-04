<?php
/**
 * Validate string containing MM/DD month and day for given year.
 * Field description appears in any error message.
 * Returns error message, empty if no error.
 * Normalizes the mmddStr
 */
function validMMDD(&$mmddStr, $year, $fieldDescr)
{
    $fail = '';
    $mmddStr = crop($mmddStr, 5);
    if (strlen($mmddStr) != 5)
    {
        $fail = 'Provide '.$fieldDescr.'(MM/DD)';
        $mmddStr = '';
    }
    else
    {
        $slash = strpos($mmddStr, "/");
        if ($slash === false)
        {
            $fail = 'Please use MM/DD format for the '.$fieldDescr;
            $mmddStr = '';
        }
        else
        {
            $mm = substr($mmddStr,0,$slash);
            $dd = substr($mmddStr,$slash+1);
            $mm = intval($mm);
            $dd = intval($dd);
            if (!checkdate($mm, $dd, intval($year)))
            {
                $fail = 'The '.$fieldDescr.' is not a valid date in '.$year;
                $mmddStr = '';
            }
            else
            {
                $mmddStr = $mm . '/' . $dd;
            }
        }
    }
    return $fail;
}

/**
 * Validate string containing MM/DD month and day for given year.
 * Ensure the date is in the future
 * Field description appears in any error message.
 * Returns error message, empty if no error.
 * Normalizes the mmddStr
 */
 function futureMMDD(&$mmddStr, $year, $fieldDescr)
 {
     $fail = validMMDD($mmddStr, $year, $fieldDescr);
     if ($fail == '')
     {
         $date = strtotime($mmddStr . '/' . $year);
         if ($date < time())
         {
             $fail = 'The '.$fieldDescr.' is in the past.';
         }
     }
 }

/**
 * Validate string containing HH:MM hour and minute.
 * Field description appears in any error message.
 * Returns error message, empty if no error.
 * Normalizes the hhmmStr
 */
function validHHMM(&$hhmmStr, $fieldDescr)
{
    $fail = '';
    $hhmmStr = crop($hhmmStr, 5);
    if (strlen($hhmmStr) != 5)
    {
        $fail = 'Provide '.$fieldDescr.'(HH:MM)';
    }
    else
    {
        $colon = strpos($hhmmStr, ":");
        if ($colon === false)
        {
            $fail = 'Please use HH:MM format for the '.$fieldDescr;
        }
        else
        {
            $hh = substr($hhmmStr,0,$colon);
            $mm = substr($hhmmStr,$colon+1);
            $hh = intval($hh);
            $mm = intval($mm);
            if (0 <= $hh && $hh < 24 && 0 <= $mm && $mm < 60)
            {
                $hhmmStr = sprintf('%02d:%02d',$hh, $mm);
            }
            else
            {
                $fail = 'The '.$fieldDescr.' is not a valid time.';
            }
        }
    }
    return $fail;
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
