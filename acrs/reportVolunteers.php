<?php
/*  reportVolunteers.php, acrs, dlco, 10/24/2010
 *  contest table query functions
 *
 *  Changes:
 *    10/24/2010 jim_ward	correct reference to unintialized var $fail in reportVolunteers().
 */

set_include_path('./include');
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("query/userQueries.inc");
require_once ("data/encodeHTML.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

/**
 * groom comma separated list of positions
 * posList is the comma separated list of volunteer positions
 * returns content of posList with comma space and leading capitals, false if catList is empty
 */
function groomPos($posList)
{
   $str = false;
   $first = true;
   $tok = trim(strtok($posList, ','));
   while ($tok)
   {
      if (!$first)
      {
         $str .= ', ';
      } else
      {
         $first = false;
         $str = '';
      }
      $str .= $tok;
      $tok = trim(strtok(','));
   }
   return $str;
}

function startCategory($curRcd, $breakPage = true)
{
   echo $breakPage ? '<div class="break-after">' : '<div class="volunteerRecord">';
   echo '<h3 class="volunteerRecord">'.strhtml($curRcd['class']).' '.strhtml($curRcd['category']).' ('.strhtml($curRcd['name']).')</h3>' . "\n";
   echo '<table class="volunteerTable"><thead>';
   echo '<tr><th>Name</th><th>IAC #</th><th>Flying</th><th>Judge</th></tr>';
   //echo '<tr><th>Preference</th></tr>';
   echo '</thead>' . "\n";
   echo '<tbody class="volunteerTable-body">' . "\n";
}

function showVolunteer($curRcd, $isFlying = false)
{
   echo '<tr><td>' . $curRcd["givenName"] . ' ' . $curRcd["familyName"] . '</td>';
   echo '<td>' . $curRcd['iacID'] . '</td>';
   echo '<td>';
   if ($isFlying)
   {
      echo $curRcd['flyingCat'];
   } else
   {
      echo 'Not flying';
   }
   echo "</td>\n";
   echo '<td>';
   if ($curRcd['judgeQualification'] == 'regional')
   echo ' regional';
   else if ($curRcd['judgeQualification'] == 'national')
   echo ' national';
   echo '</td></tr>';
   echo '<tr class="prefs"><td colspan="4">';
   $vols = groomPos($curRcd['volunteer']);
   if ($vols)
   {
      echo strhtml($vols);
   }
   echo '</td>';
   echo "</tr>\n";
}

function endCategory()
{
   echo '</tbody></table></div>' . "\n";
}

function processVolunteerResult($result, $isFlying)
{
   $curRcd = dbFetchAssoc($result);
   while ($curRcd)
   {
      showVolunteer($curRcd, $isFlying);
      $curRcd = dbFetchAssoc($result);
   }
}

function competitorPrefsForCatID($catID)
{
   return 'select a.givenName, a.familyName, a.iacID, a.judgeQualification, ' .
    ' c.volunteer, f.name as flyingCat' .
    ' from registrant a, registration b, volunteer c, reg_type d, ctst_cat e,' .
    ' ctst_cat f' .
 ' where c.catID = ' . $catID .
 ' and a.userID = c.userID' .
 ' and e.catID = ' . $catID .
 ' and d.ctstID = e.ctstID' .
 ' and d.userID = c.userID' .
 " and d.compType = 'competitor'" .
 ' and b.regID = d.regID' .
 ' and f.catID = b.catID' .
 ' and e.catID != f.catID' .
 ' order by flyingCat, a.familyName, a.givenName';
}

function volunteerPrefsForCatID($catID)
{
   return 'select a.givenName, a.familyName, a.iacID, a.judgeQualification, ' .
    ' c.volunteer' .
    ' from registrant a, volunteer c, reg_type d, ctst_cat e' .
 ' where c.catID = ' . $catID .
 ' and a.userID = c.userID' .
 ' and e.catID = ' . $catID .
 ' and d.ctstID = e.ctstID' .
 ' and d.userID = c.userID' .
 " and d.compType = 'volunteer'" .
 ' order by a.familyName, a.givenName';
}

function reportCategory($db_conn, $catID)
{
   $query = competitorPrefsForCatID($catID);
   //debug('reportVolunteers.reportCategory('.$catID.'):'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      processVolunteerResult($result, true);
   }
   $query = volunteerPrefsForCatID($catID);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      processVolunteerResult($result, false);
   }
}

function reportVolunteers($db_conn)
{
   $query = 'SELECT catID, name, class, category FROM ctst_cat WHERE ctstID = '.
   $_SESSION['ctstID'] .
   ' order by category, class';
   $result = dbQuery($db_conn, $query);
   //debug('reportVolunteers:'. $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      $fail = '';
      $first = true;
      $curRcd = dbFetchAssoc($result);
      while ($curRcd && $fail == '')
      {
         if (!$first)
         {
            endCategory();
         }
         else
         {
            $first = false;
         }
         startCategory($curRcd);
         $fail = reportCategory($db_conn, $curRcd['catID']);
         $curRcd = dbFetchAssoc($result);
      }
      if (!$first) endCategory();
   }
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "reportVolunteers.php");
   $corrMsg = "<it>Internal: failed access to contest database</it>";
   $readRecord = FALSE;
}
startHead("Volunteers");
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
startContent();
echo '<h1 class="noprint">Volunteers</h1>';
echo '<p class="noprint"><input style="margin-right:20px" type="button" onClick="window.print()" ' .
'value="Print This Page"/><a href="index.php">Return to registration</a></p>';
if ($corrMsg != '')
{
   echo '<ul class="error">' . $corrMsg . '</ul>';
} else
{
   if (isContestOfficial())
   {
      $fail = reportVolunteers($db_conn);
   } else
   {
      echo '<p class="error">Restricted to contest officials.</p>';
   }
}
if ($fail != '')
{
   $eMsg = notifyError('Volunteer data temporarily unavailable.', $fail);
   echo '<p>' . $eMsg . '</p>';
}
echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
endContent();
dbClose($db_conn);
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
