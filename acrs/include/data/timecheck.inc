<?php
require_once ('dbConfig.inc');

/*
 * $dateStr date and time in format accepted by strtotime
 * return true if current time is before given time
 */
function isBeforeDate($dateStr)
{
	$dateStamp = strtotime($dateStr);
	return time() < $dateStamp;
}

/*
 * $dateStr date and time in format accepted by strtotime
 * return true if current time is after given time
 */
function isAfterDate($dateStr)
{
	$dateStamp = strtotime($dateStr);
	return $dateStamp < time();
}

function dateAfterRegOpen($contest)
{
   return !isBeforeDate($contest['regOpen']);
}

function dateBeforeRegClosed($contest)
{
   return !isAfterDate($contest['regDeadline']);
}

function isRegOpen()
{
    $contest = $_SESSION['contest'];
    return dateAfterRegOpen($contest) && dateBeforeRegClosed($contest);
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
