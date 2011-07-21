<?php
set_include_path('./include');
require_once ('dbConfig.inc');
require("ui/validate.inc");
require ("data/validCtst.inc");
require_once("dbCommand.inc");
require_once("data/encodeSQL.inc");
require_once("ui/emailNotices.inc");
require_once("useful.inc");
require_once("redirect.inc");
require_once('data/timecheck.inc');
require_once("query/userQueries.inc");

/**
Write one candidate checkbox form entry.
*/
function writeCandidate($row)
{
   echo "<td><input class=\"form_check\" type=\"checkbox\" id=\"in_judge_".
         $row['iacID']."\" name=\"".$row['iacID'].
         "\" onclick=\"checkJudge(this, '".$row['region']."')\">".
         $row['givenName'].' '.$row['familyName'].
         "</input></td>\n";
}

/**
Get required information to determine judge voting status.
db_conn: database connection
userID: current registrant
ctstID: current contest
voteData: returned associative array of voting status data
Return empty string on success, else error string.
*/
function getVotingData($db_conn, $userID, $ctstID, &$voteData)
{
   $fail = '';
   $query = "select a.userID, a.givenName, a.familyName, a.email, a.iacID,".
     " b.regID, b.compType,".
     " c.teamAspirant, c.fourMinFree, c.hasVotedJudge, c.paidAmt, c.isStudent, ".
     " d.reqPmtForVoteJudge, d.voteEmail, ".
     " e.regAmt, e.hasStudentReg, e.studentRegAmt, e.hasTeamReg, e.teamRegAmt, e.hasVoteJudge, e.maxVotes, ".
     " e.voteTeamOnly, e.voteByRegion, e.maxRegion, e.voteDeadline, e.hasFourMinute, e.fourMinRegAmt".
     " from registrant a, reg_type b, registration c, contest d, ctst_cat e".
     " where " . 
     " b.userID = ".$userID.
     " and b.ctstID = ".$ctstID.
     " and a.userID = b.userID".
     " and d.ctstID = b.ctstID".
     " and c.regID = b.regID".
     " and e.catID = c.catID";
   //debug('votejudge.getVotingData:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else if (dbCountResult($result) != 1)
   {
      $fail = "not registered for contest";
   } else
   {
     $row = dbFetchAssoc($result);
     foreach ($row as $key => $value)
     {
         $voteData[$key] = stripslashes($value);
     }
     sqlBoolValueToPostData($row['teamAspirant'], 'teamAspirant', $voteData);
     sqlBoolValueToPostData($row['hasVotedJudge'], 'hasVotedJudge', $voteData);
     sqlBoolValueToPostData($row['hasVoteJudge'], 'hasVoteJudge', $voteData);
     sqlBoolValueToPostData($row['isStudent'], 'isStudent', $voteData);
     sqlBoolValueToPostData($row['hasStudentReg'], 'hasStudentReg', $voteData);
     sqlBoolValueToPostData($row['hasTeamReg'], 'hasTeamReg', $voteData);
     sqlBoolValueToPostData($row['voteTeamOnly'], 'voteTeamOnly', $voteData);
     sqlBoolValueToPostData($row['voteByRegion'], 'voteByRegion', $voteData);
     sqlBoolValueToPostData($row['hasFourMinute'], 'hasFourMinute', $voteData);
   }
   return $fail;
}

/**
Output a ballot form for judge selection
*/
function judgeBallot($db_conn, $submitURL)
{
   $fail = '';
   $query = "select givenName, familyName, iacID, region".
     " from judge".
     " where ctstID = ".$_SESSION['ctstID'].
     " order by region, familyName, givenName";
   //debug($query);
   $result = dbQuery($db_conn, $query);
   $region = '';
   $colCount = 0;
   $maxCols = 4;
   echo "<form class=\"recordForm\" action=\"".$submitURL."\" method=\"post\">\n";
   echo '<table class="judgeBallot"><tbody>';
   $row = false;
   if ($result)
   {
     $row = dbFetchAssoc($result);  
   }
   else
   {
      $fail = dbErrorText(); 
      notifyError('failed query judges, '.$fail, 'votejudge.judgeBallot()');
   }
   while ($row)
   {
     if ($region != $row['region'])
     {
       if ($colCount > 0) echo '</tr>';
       $region = $row['region'];
       echo '<tr><th class="regionHeader" colspan="'.$maxCols.'">'.$region."</th></tr>\n";
       $colCount = 0;
     }
     if ($colCount == 0) echo "<tr>";
     writeCandidate($row);
     $colCount += 1;
     if ($colCount == $maxCols)
     {
        echo "</tr>";
        $colCount = 0;
     }
     $row = dbFetchAssoc($result);
   }
   echo "</tbody></table>\n";
   // TODO make numbers depend on data
   echo '<div class="error" id="ttlWarning" style="display:none">You have selected more than 7 judges.</div>';
   echo '<div class="error" id="rgnCtWarning" style="display:none">You have selected more than 2 judges from one region.</div>';
   echo '<input class="submit" name="submit" type="submit" value="Register your vote."/>'."\n";
   echo "</form>\n";
   return $fail;
}

/*
Validate votes, update judges' vote counts, update users voting status.
db_conn: database connection token
votes: post data with judge id's
regID: id of registration
jlist: return string containing the names and regions of voted judges.
Return error string if error, else empty string
*/
function tallyJudgeScore($db_conn, $votes, $regID, &$jlist)
{
   global $contest;
   //debugArr("votejudge.tallyJudgeScore() judges: ", $votes);
   $vta = array();
   $dscr = array();
   $jlist = '';
   $corrmsg = '';
   $ctt = 0;
   $query = "select iacID, votecount, region, givenName, familyName from judge".
       " where iacID in (";
   $first = true;
   foreach ($votes as $id => $on)
   {
      if (strcmp($id,'submit') != 0)
      {
         if (!$first)
         {
            $query .= ",";
         }
         $first = false;
         $query .= strSQL($id,12);
         $ctt += 1;
      }
   }
   // TODO make this a data driven check
   if ($ctt > 7) 
   {
      $corrmsg .= '<it>vote contains too many judges</it>';
   }
   $query .= ") and ctstID = ".$_SESSION['ctstID'];
   $query .= " order by region";
   //debug($query);
   $result = dbQuery($db_conn, $query);
   $ctr = 0;
   if ($result)
   {
     $row = dbFetchAssoc($result);  
   }
   $region = '';
   while ($row)
   {
     if ($region != $row['region'])
     {
        // TODO have to make this a data driven check
//       if ($ctr > 2)
//       {
//          $corrmsg .= '<it>vote has too many judges in region, '.$region.'</it>';
//       }
       $ctr = 0;
       $region = $row['region'];
     }
     $vta[$row['iacID']] = $row['votecount'] + 1;
     $dscr[$row['iacID']] = $row['givenName'] . ' ' . $row['familyName'] . ' from the ' . $region . " region";
     ++$ctr;
     $row = dbFetchAssoc($result);  
   }
   //debugArr("counts: ", $vta);
   $query = "start transaction";
   $fail = dbExec($db_conn, $query);
   if ($fail)
   {
      $corrmsg .= notifyError('<it>failed start transaction</it>', $fail);
   }
   $first = true;
   if (isset($vta))
   foreach ($vta as $id => $count)
   if ($corrmsg == '')
   {
      $query = "update judge set votecount = ".$count." where iacID = ".$id;
      $query .= " and ctstID = ".$_SESSION['ctstID'];
      //debug($query);
      $fail = dbExec($db_conn, $query);
      if ($fail)
      {
        $corrmsg .= notifyError('<it>failed update counts for '.$id.'</it>', $fail);
      }
      else
      {
        if (!$first)
        {
          $jlist .= "\n";
        }
        $jlist .= $dscr[$id];
        $first = false;
      }
   }
   if ($corrmsg == '')
   {
      $query = "update registration set hasVotedJudge = 'y' where regID=".$regID;
      //debug($query);
      $fail = dbExec($db_conn, $query);
      if ($fail)
      {
        $corrmsg .= notifyError('<it>failed record voted judge on registration '.$regID.'</it>', $fail);
      }
   }
   if ($corrmsg == '')
   {
      $query = "commit";
   }
   else
   {
      $query = "rollback";
   }
   $fail = dbExec($db_conn, $query);
   if ($fail)
   {
      $corrmsg .= notifyError('<it>failed end transaction with '.$query.'</it>', $fail);
   }
   return $corrmsg;
}

// Main
$wasUpdated = false;
$validVoter = false;
$openVoting = false;
$corrMsg = '';
$jlist = '';
$userID = $_SESSION['userID'];
$ctstID = $_SESSION['ctstID'];
$votes = $_POST;
$voteData = array();
$db_conn = false;
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "votejudge.php");
   $corrMsg = "<it>Internal: failed access to contest database</it>";
}
else
{
   $fail = getVotingData($db_conn, $userID, $ctstID, $voteData);
   if ($fail != '') 
   {
      notifyError($fail, "votejudge.php");
      $corrMsg = "<it>Internal: failed voter query</it>";
   }
   else
   {
     debugArr('voteJudge.voteData:',$voteData);
     $validVoter = (
            !boolChecked($voteData, 'hasTeamReg')
         || !boolChecked($voteData,'voteTeamOnly') 
         || boolChecked($voteData, 'teamAspirant')
       ) && (
            !boolChecked($voteData, 'reqPmtForVoteJudge')
         || checkPaidInFull($voteData)
       );
     $openVoting = (!isAfterDate($voteData['voteDeadline']));
   }
}
if ($openVoting && $validVoter && isset($votes["submit"]))
{
   // begin form processing
   if ($corrMsg == '')
   {
      // have valid data
      //debugArr('votejudge post data',$votes);
      $fail = tallyJudgeScore($db_conn, $votes, $voteData['regID'], $jlist);
      if ($fail == '')
      {
          $wasUpdated = true;
      } 
      else
      {
          notifyError($fail, "votejudge.php");
          $corrMsg = "<it>Internal: failed data update.</it>";
      }
   }
}

startHead("Judge Ballot");
if ($wasUpdated)
{
   startContent();
   echo "<h1>Judge Ballot</h1>";
   verificationHeader("Competitor,");
   echo '<p>Your votes were recorded for '."<ul><li>".
        str_replace("\n","</li><li>",$jlist)."</li></ul>".
        'Please retain your email confirmation.</p>';
   sendVotesEmail($voteData, $jlist);
   echo '<div class="returnButton"><a href="index.php">Return to registration</a></div>';
}
else if ($openVoting && $validVoter)
{
   // show voting form
   // $corrMsg has HTML content
   echo '<script type="text/javascript" src="votejudge.js"></script>';
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
   startContent("onload='initVoting()'");
   echo "<h1>Judge Ballot</h1>";
   verificationHeader("For");
   if ($corrMsg != '')
   {
      echo '<ul class="error">'.$corrMsg.'</ul>';
   }
   // TODO make content depend on configured voting rules
   echo '<p>Vote your choice of judges.  ';
   echo 'Attend to the voting limits outlined in the registration information.  ';
   echo 'You will receive an email confirmation of your vote.  ';
   //echo 'The system will count your vote and record that you have ';
   //echo 'voted.  It will not identify who voted for which judges.  ';
   echo 'The system will not identify who voted for which judges.  ';
   echo 'The judge selection chair will receive a copy of your vote.  ';
   //echo 'You will have no opportunity to change your vote after ';
   //echo 'you have registered your vote.</p>';
   echo '</p>';
   $fail = judgeBallot($db_conn, "votejudge.php");
   if ($fail != '')
   {
      echo '<p class="error">'.$fail.'</p>';
   }
   echo '<div class="returnButton"><a href="index.php">Return without voting</a></div>';
}
else if (!$validVoter)
{
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
   startContent();
   echo "<h1>Judge Voting</h1>";
   // TODO based on configured rules
   echo '<p class="error">Only team aspirants who have paid registrations,';
   echo ' who have not yet voted may vote for grading judges.</p>';
   echo '<div class="returnButton"><a href="index.php">Registration index</a></div>';
}
else if (!$openVoting)
{
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
   startContent();
   echo '<p class="error">Judge voting is closed.</p>';
   echo '<div class="returnButton"><a href="index.php">Registration index</a></div>';
}
endContent();
dbClose($db_conn);

/*
   Copyright 2008, 2010 International Aerobatic Club, Inc.

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
