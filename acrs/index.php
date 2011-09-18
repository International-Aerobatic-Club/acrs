<?php
set_include_path('./include');
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ('data/timecheck.inc');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("query/userQueries.inc");
require_once ("practice.php");

function showMenu($db_conn, $regType, $contest, $userID, $ctstID)
{
   //debugArr('index.showMenu has regType:', $regType);
   //debugArr('index.showMenu has contest:', $contest);
   $needVolunteer = false;
   $isVolunteer = !isSelected($regType, "compType", "regrets");
   if ($isVolunteer)
   {
      $query = 'select a.catID from volunteer a, ctst_cat b  ' .
        'where b.ctstID = ' . $ctstID .
        ' and userID = ' . $userID .
        ' and a.catID = b.catID';
      //debug($query);
      $result = dbQuery($db_conn, $query);
      if (dbErrorNumber() != 0)
      {
         notifyError("volunteerQuery:" . dbErrorText(), "index.php");
         echo '<p class="error">Failed access volunteer information</p>';
      } else
      {
         $needVolunteer = (dbCountResult($result) == 0);
      }
   }
   $isCompetitor = isSelected($regType, "compType", "competitor");
   $isTeamCategory = $isCompetitor && boolChecked($regType, "hasTeamReg");
   $isTeam = $isTeamCategory && boolChecked($regType, "teamAspirant");
   $voteCategory = sqlIsTrue($contest['hasVoteJudge']) && boolChecked($regType, "hasVoteJudge");
   $mayVote = $voteCategory && (!boolChecked($regType, 'voteTeamOnly') || $isTeam);
   if ($isCompetitor)
   {
      $needPayment = sqlIsTrue($contest['hasPayPal']) && !checkPaidInFull($regType);
      $mayVote = $mayVote && (!boolChecked($contest, 'reqPmtForVoteJudge') || !$needPayment);
      $needVote = $mayVote && !boolChecked($regType, 'hasVotedJudge') && (!boolChecked($contest, 'reqPmtForVoteJudge') || !$needPayment);
      $needAirplane = !isset ($regType["airplaneRegID"]) || !isset ($regType["insCompany"]) || $regType["airplaneRegID"] == '' || $regType["insCompany"] == '';
      $needPracticeSlot = havePracticeRegistration($contest, $regType);
   } else
   {
      $needPayment = false;
      $needVote = false;
      $needAirplane = false;
      $needPracticeSlot = false;
   }

   $reminders = '';
   if ($needPayment)
   $reminders .= '<li>Your registration requires a payment.</li>';
   if ($needVote)
   $reminders .= '<li>We have not recorded your vote for judges.</li>';
   if ($needAirplane)
   $reminders .= '<li>We do not have your flying information.</li>';
   if ($needVolunteer)
   $reminders .= '<li>We do not have your volunteer preferences.</li>';
   if ($isTeamCategory && !$isTeam)
   $reminders .= '<li>Warning: You are NOT registered as a team aspirant.</li>';

   echo "<ul class=\"basicReg\">\n";
   echo "<li><a href='register.php'>Edit your registration information.</a></li>\n";
   if ($isVolunteer)
   {
      echo "<li><a href='volunteer.php'>Edit your volunteer preferences.</a></li>\n";
   }
   if ($isCompetitor)
   {
      echo "<li><a href='flying.php'>Edit your flying information.</a></li>\n";
   }
   echo "</ul><ul class=\"advancedReg\">\n";
   if ($mayVote)
   {
      echo "<li><a href='votejudge.php'>Vote your choice of judges.</a></li>\n";
   }
   if ($needPayment)
   {
      echo "<li><a href='payRegFee.php'>Pay your registration fee.</a></li>\n";
   }
   if ($needPracticeSlot)
   {
      echo "<li><a href='practiceSlot.php'>Change or select your practice slot.</a></li>\n";
   }
   echo "</ul>\n";
   return $reminders;
}

function showRegistrants($db_conn)
{
   $query = 'select a.givenName, a.familyName, b.chapter, ' .
    'e.category, e.class, e.name, e.catID, e.hasTeamReg, b.teamAspirant ' .
    ' from registrant a, registration b, ctst_cat e, reg_type f' .
    ' where a.userID = f.userID ' .
    ' and f.ctstID = ' . $_SESSION['ctstID'] .
    " and f.compType = 'competitor'" .
    ' and b.regID = f.regID' .
    ' and e.catID = b.catID' .
    ' order by e.catID, a.familyName, a.givenName;';
   //debug($query);
   $result = dbQuery($db_conn, $query);
   if (dbErrorNumber() != 0)
   {
      $fail = dbErrorText();
   } else
   {
      $curCat = '';
      $curRcd = dbFetchAssoc($result);
      if ($curRcd)
      echo "<h3>Registered Contestants</h3\n";
      while ($curRcd)
      {
         if ($curCat != $curRcd['catID'])
         {
            $curCat = $curRcd['catID'];
            echo '<h4 class="registrant">' . $curRcd['name'] . "</h4>\n";
         }
         echo '<p class="registrant">' . strhtml($curRcd["givenName"]) . ' ' . strhtml($curRcd["familyName"]);
         if (isset ($curRcd["chapter"]))
         {
            echo ', Chapter ' . strhtml($curRcd["chapter"]);
         }
         sqlBoolValueToPostData($curRcd['teamAspirant'], 'teamAspirant', $curRcd);
         sqlBoolValueToPostData($curRcd['hasTeamReg'], 'hasTeamReg', $curRcd);
         $isTeamCategory = boolChecked($curRcd, 'hasTeamReg');
         $isTeam = $isTeamCategory && boolChecked($curRcd, 'teamAspirant');
         if ($isTeam)
         {
            echo ', for the Team';
         }
         echo "</p>\n";
         $curRcd = dbFetchAssoc($result);
      }
      $query = 'select a.givenName, a.familyName ' .
        ' from reg_type b, registrant a' .
        ' where b.ctstID = ' . $_SESSION['ctstID'] .
        ' and b.compType = \'volunteer\'' .
        ' and a.userID = b.userID' .
        ' order by a.familyName, a.givenName;';
      //debug($query);
      $result = dbQuery($db_conn, $query);
      if (dbErrorNumber() != 0)
      {
         $fail = dbErrorText();
      } else
      {
         $curRcd = dbFetchAssoc($result);
         if ($curRcd)
         echo "<h3>Registered Volunteers</h3\n";
         while ($curRcd)
         {
            echo '<p class="registrant">' . strhtml($curRcd["givenName"]) . ' ' . strhtml($curRcd["familyName"]) . "</p>\n";
            $curRcd = dbFetchAssoc($result);
         }
      }
   }
}

function showRegPartDownloads($db_conn, $ctstID)
{
  $maxPer = 40;
  $compCount = 0;
  $query = "SELECT count( regID ) FROM reg_type" .
   " WHERE ctstID = $ctstID AND compType = 'competitor'";
  //debug("showRegPartDownloads query is $query");
  $result = dbQuery($db_conn, $query);
  if (dbErrorNumber() == 0)
  {
     $curRcd = dbFetchRow($result);
     $compCount = $curRcd[0];
  }
  //debug("compCount = $compCount");
  $pages = ceil($compCount / $maxPer);
  if (1 < $pages)
  {
     $perPage = $maxPer;
     if ($compCount % $maxPer != 0)
     {
       $perPage += ceil(($compCount % $maxPer) / $pages);
       $pages -= 1;
     }
     $offset = 0;
     echo '<ul>';
     for ($page = 1; $page <= $pages; ++$page)
     {
       echo "<li><a href='reportRegIAC.php?offset=$offset&count=$perPage'>";
       echo "Part $page</a></li>\n";
       $offset += $perPage;
     }
     echo '</ul>';
  }
}

function showAdminFunctions($db_conn, $ctstID)
{
   if (isContestAdmin())
   {
      echo '<p>Contest Director and administrator:</p>';
      echo "<ul class=\"adminReg\">\n";
      echo "<li><a href='addContest.php?edit=true'>Edit contest information.</a></li>\n";
      echo "<li><a href='catTable.php'>Edit contest categories.</a></li>\n";
      echo "<li><a href='prsTable.php'>Edit practice sessions.</a></li>\n";
      echo "<li><a href='practiceSlotDes.php'>Designate practice slots.</a></li>\n";
      echo "<li><a href='addJudge.php'>Edit judging ballot.</a></li>\n";
      echo "<li><a href='authorize.php'>Designate CD, Registrar, VC.</a></li>\n";
      echo '</ul>';
   }
   if (isRegistrar())
   {
      echo '<p>Registrar:</p>';
      echo "<ul class=\"adminReg\">\n";
      echo "<li><a href='reportRegIAC.php'>All IAC registration forms as PDF.</a>\n";
      showRegPartDownloads($db_conn, $ctstID);
      echo "</li>\n";
      echo "<li><a href='reportRegPhoneList.php'>Spreadsheet importable contact list of competitors only.</a></li>\n";
      echo "<li><a href='exportRegistrants.php'>Spreadsheet importable contact list of all registrants.</a></li>\n";
      echo "<li><a href='exportJaSPer.php'>JaSPer importable list of competitors.</a></li>\n";
      echo '</ul>';
   }
   echo '<p>Volunteer Coordinator:</p>';
   echo "<ul class=\"adminReg\">\n";
   echo "<li><a href='reportVolIAC.php'>All IAC volunteer forms as PDF.</a></li>\n";
   echo "<li><a href='reportRegSummary.php'>Summary report of registration information.</a></li>\n";
   echo "<li><a href='broadcast.php'>Broadcast email to all registrants.</a></li>\n";
   echo "<li><a href='reportVolunteers.php'>Report volunteers.</a></li>\n";
   echo "<li><a href='reportPracticeSlots.php'>Report practice slots.</a></li>\n";
   echo '</ul>';
}

function showPracticeSlots($db_conn, $userID, $ctstID)
{
   $query = 'select a.slotIndex, b.practiceDate, b.startTime, b.minutesPer'.
    ' from practice_slot a, session b' . 
    ' where a.userID = '.$userID.
    ' and b.sessID = a.sessID '.
    ' and b.ctstID = ' .$ctstID.
    ' order by a.sessID, a.slotIndex;';
   // debug('selectCtst:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      echo '<p class="error">' . notifyError(dbErrorText(),'selectCtst') . '</p>';
   }
   else
   {
      if (0 < dbCountResult($result))
      {
         echo '<p>Your practice slot ';
         if (1 < dbCountResult($result)) echo 'reservations:'; else echo 'reservation:';
         echo '<ul class="slotList">';
         while ($curRcd = dbFetchAssoc($result))
         {
            $interval = intval($curRcd['minutesPer']);
            $dateStamp = strtotime($curRcd['practiceDate']);
            $startTime = strtotime($curRcd['startTime']);
            echo '<li>' .
            makeDescription($curRcd['practiceDate'], $curRcd['startTime'], $curRcd['slotIndex'], $curRcd['minutesPer'])
            . '</li>';
         }
         echo '</ul></p>';
      }
   }
}

startHead("Registration");
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>' . "\n";
startContent();
echo "<h1>Registration</h1>";
$fail = dbConnect($db_conn);
$userID = $_SESSION['userID'];
$ctstID = $_SESSION['ctstID'];
$regType = array();
$contest = array();
if ($fail == '')
{
  $fail = retrieveExistingRegData($db_conn, $userID, $ctstID, $regType);
}
if ($fail == '')
{
  $fail = retrieveContestData($db_conn, $ctstID, $contest);
}
if ($fail != '')
{
   echo "<p>" . $fail . "</p>";
} else
{
   verificationHeader("Welcome,");

   echo "<table class=\"indexMenu\"><tbody><trow><td class='userMenu'>\n";

   if (isRegOpen($contest))
   {
      $reminders = showMenu($db_conn, $regType, $contest, $userID, $ctstID);
   } else
   {
      if (!dateAfterRegOpen($contest))
      {
         $reminders = '<li>On-line registration will open on ' . $contest['regOpen'] . '</li>';
      }
      else
      {
         $reminders = '<li>On-line registration is closed.  On-site registration will open before the contest.</li>';
      }
   }

   $isCompetitor = isSelected($regType, "compType", "competitor");
   if ($isCompetitor)
   {
      echo '<div class="afterReg">';
      showPracticeSlots($db_conn, $userID, $ctstID);
      echo "<p><a href='printRegIAC.php'>Your IAC contest registration forms as PDF.</a></p>\n";
      echo "</div>\n";
   }

   if ($reminders != '')
   {
      echo "<ul class=\"reminders\">".$reminders."</ul>\n";
   }

   echo "<ul class=\"userControl\">\n";
   echo '<li><a href="selectCtst.php">Select a different contest</a></li>' . "\n";
   echo "<li><a href=\"changePWD.php\">Change your password</a></li>\n";
   if (isset ($_COOKIE['iac_reg']))
   {
      echo "<li><a href=\"logout.php?forget='true'\">Logout and forget your login on this machine.</a></li>\n";
   }
   echo "<li><a href=\"logout.php\">Logout</a></li>\n";
   echo "</ul>\n";

   echo "</td><td class='adminMenu'>\n";

   if (isContestOfficial())
   {
      showAdminFunctions($db_conn, $ctstID);
   }

   echo "</td></trow></tbody></table>\n";

   showRegistrants($db_conn);

   dbClose($db_conn);
}
endContent();
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
