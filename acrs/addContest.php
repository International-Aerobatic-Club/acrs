<?php
set_include_path('./include');
require ("ui/validate.inc");
require ('data/validMMDD.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("form/addContestForm.inc");
require_once ("query/contestQueries.inc");
require_once ("ui/emailNotices.inc");
require_once ("useful.inc");
require_once ("data/timecheck.inc");
require_once ("redirect.inc");

function validatePost($record)
{
   $corrMsg = '';
   $record['regYear'] = crop($record['regYear'], 4);
   if (strlen($record['regYear']) != 4)
   {
      $corrMsg .= '<li>Provide Year (YYYY)</li>';
   } else
   {
      $intValue = preg_replace('/[^0-9]/', '', $record['regYear']);
      if (strlen($record['regYear']) != 4)
      {
         $corrMsg .= '<li>Year (YYYY) must contain only numbers.</li>';
      }
   }
   $record['name'] = crop($record['name'], 72);
   if (strlen($record['name']) == 0)
   {
      $corrMsg .= '<li>Provide Name</li>';
   }

   $startDate = null;
   $dateCheck = futureMMDD($record['startDate'], $record['regYear'], 'start date');
   if ($dateCheck != '')
   $corrMsg .= '<li>' . $dateCheck . '</li>';
   else
   $startDate = strtotime($record['startDate'] . '/' . $record['regYear']);

   $endDate = null;
   $dateCheck = futureMMDD($record['endDate'], $record['regYear'], 'end date');
   if ($dateCheck != '')
   $corrMsg .= '<li>' . $dateCheck . '</li>';
   else
   $endDate = strtotime($record['endDate'] . '/' . $record['regYear']);

   if ($startDate != null && $endDate != null && $endDate < $startDate)
   {
      $corrMsg .= '<li>The contest ends before it starts.</li>';
   }

   $regClose = null;
   $dateCheck = futureMMDD($record['regDeadline'], $record['regYear'], 'last day to register');
   if ($dateCheck != '')
   $corrMsg .= '<li>' . $dateCheck . '</li>';
   else
   $regClose = strtotime($record['regDeadline'] . '/' . $record['regYear']);

   $regOpen = null;
   $dateCheck = futureMMDD($record['regOpen'], $record['regYear'], 'first day to register');
   if ($dateCheck != '')
   $corrMsg .= '<li>' . $dateCheck . '</li>';
   else
   $regOpen = strtotime($record['regOpen'] . '/' . $record['regYear']);

   if ($regOpen != null && $regClose != null && $regClose < $regOpen)
   {
      $corrMsg .= '<li>Registration closes before it opens.</li>';
   }

   $record['homeURL'] = crop($record['homeURL'], 320);
   if (strlen($record['homeURL']) == 0)
   {
      $corrMsg .= '<li>Provide an announcement web address</li>';
   }
   $record['regEmail'] = crop($record['regEmail'], 320);
   if (strlen($record['regEmail']) == 0)
   {
      $corrMsg .= '<li>Provide Registration email contact</li>';
   } else
   if (!validEmail($record['regEmail']))
   {
      $corrMsg .= '<li>Provide valid Registration email address</li>';
   }
   if (boolChecked($record, 'hasVoteJudge'))
   {
      $record['voteEmail'] = crop($record['voteEmail'], 320);
      if (strlen($record['voteEmail']) == 0)
      {
         $corrMsg .= '<li>Provide Voting email contact</li>';
      } else
      if (!validEmail($record['voteEmail']))
      {
         $corrMsg .= '<li>Provide valid Voting email address</li>';
      }
   }
   if (boolChecked($record, 'hasPayPal'))
   {
      $record['payEmail'] = crop($record['payEmail'], 320);
      if (strlen($record['payEmail']) == 0)
      {
         $corrMsg .= '<li>Provide PayPal account identifier</li>';
      }
   }
   if (boolChecked($record, 'hasPracticeReg'))
   {
      $record['maxPracticeSlots'] = crop($record['maxPracticeSlots'], 2);
      if (strlen($record['maxPracticeSlots']) == 0)
      {
         $corrMsg .= '<li>Provide Practice slot reservation limit</li>';
      } else
      {
         $intValue = preg_replace('/[^0-9]/', '', $record['maxPracticeSlots']);
         if (strlen($record['maxPracticeSlots']) != strlen($intValue))
         {
            $corrMsg .= '<li>Practice slot reservation limit must contain only numbers.</li>';
         }
      }
   }
   return $corrMsg;
}

//debug('start addContest.php');
$wasUpdated = FALSE;
$readRecord = isset ($_SESSION['ctstID']);
$initRecord = !$readRecord;
$ctstInfo = $_POST;
$corrMsg = '';
$userID = $_SESSION['userID'];
$ctstID = $_SESSION['ctstID'];
$db_conn = false;
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "addContest.php");
   $corrMsg = "<it>Internal: failed access to contest database</it>";
   $readRecord = FALSE;
} else
if (isset ($ctstInfo["save"]))
{
   $readRecord = false;
   $initRecord = false;
   //debugArr("addContest post data", $ctstInfo);
   // begin form processing

   $corrMsg = validatePost($ctstInfo);
   if ($corrMsg == '')
   {
      // have valid data. update sets ctstID
      $fail = updateContest($db_conn, $ctstInfo, $ctstID, $userID);
      if ($fail == '')
      {
         $wasUpdated = true;
         setContestL($db_conn, $ctstID);
      } else
      {
         $corrMsg = "<it>" . notifyerror($fail, 'addContest.php') . "</it>";
      }
   }
}
if ($readRecord)
{
   // not POST
   $fail = retrieveContest($db_conn, $ctstInfo, $ctstID);
   if ($fail != '')
   {
      notifyError($fail, "addContest.php");
      $corrMsg = "<it>Internal: failed access to contest record of " . $ctstID . "</it>";
   }
   $ctstInfo['isEdit'] = isset($_GET['edit']);
} else
if ($initRecord)
{
   sqlBoolValueToPostData('n', 'hasVoteJudge', $ctstInfo);
   sqlBoolValueToPostData('n', 'hasPayPal', $ctstInfo);
   sqlBoolValueToPostData('n', 'hasPracticeReg', $ctstInfo);
   sqlBoolValueToPostData('y', 'reqPmtForVoteJudge', $ctstInfo);
   sqlBoolValueToPostData('y', 'reqPmtForPracticeReg', $ctstInfo);
   $ctstInfo['isEdit'] = isset($_GET['edit']);
}
dbClose($db_conn);

if ($wasUpdated)
{
   if ($ctstInfo['isEdit'])
   {
      $nextURL = 'index.php';
   }
   else
   {
      $nextURL = 'categoryWizard.php';
   }
   getNextPage($nextURL);
} else
{
   // add contest form
   // $corrMsg has HTML content
   // $ctstInfo has POST content
   startHead("Add a contest");
   addContestFormHeader();
   startContent();
   echo "<h1>Contest Information</h1>";
   verificationHeader("Contest official,");
   if ($corrMsg != '')
   {
      echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
   }
   echo '<p>This form lets an IAC Chapter add a contest that it will sponsor.  Please don\'t use this form to ' .
    'add a contest you think you know about that you would like to attend.  Pass this url along to the ' .
    'contest CD instead. The system will log you as the administrator for this contest entry.  Thank you.</p>';
   addContestForm($ctstInfo, "addContest.php");
   echo '<div class="returnButton"><a href="index.php">Return without saving</a></div>';
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