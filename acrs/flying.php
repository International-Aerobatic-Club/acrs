<?php
set_include_path('./include');
require("ui/validate.inc");
require ("data/validCtst.inc");
require_once("dbConfig.inc");
require_once("dbCommand.inc");
require_once("data/encodeSQL.inc");
require_once("query/userQueries.inc");
require_once("form/flyingform.inc");
require_once("useful.inc");
require_once("ui/emailNotices.inc");
require_once('data/timecheck.inc');
require_once("redirect.inc");

function doFlyingInfo()
{
$wasUpdated = FALSE;
$readRecord = TRUE;
$corrMsg = '';
$userID = $_SESSION['userID'];
$email = $_SESSION['email'];
$registrant = $_POST;
$db_conn = false;
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "register.php");
   $corrMsg = "<it>Internal: failed access to contest database</it>";
   $readRecord = FALSE;
}
else if (isset($_POST["submit"]) || isset($_POST["save"]))
{
   $readRecord = FALSE;
   // begin form processing
   $certNumber = crop($registrant["certNumber"], 16);
   if (strlen($certNumber) == 0)
   {
       $corrMsg .= "<li>Provide a pilot certificate number.</li>";
   }
   $regID = crop($registrant["airplaneRegID"], 16);
   if (strlen($regID) == 0)
   {
       $corrMsg .= "<li>Provide airplane registration ID.</li>";
   }
   $insCo = crop($registrant["insCompany"], 24);
   if (strlen($insCo) == 0)
   {
       $corrMsg .= "<li>Provide name of insurance company.</li>";
   }
   if ($corrMsg == '')
   {
      // have valid data
      $fail = updateFlying($db_conn, $registrant, $userID);
      if ($fail == '')
      {
          $wasUpdated = TRUE;
      } 
      else
      {
          notifyError($fail, "register.php");
          $corrMsg = "<it>Internal: failed data update.</it>";
      }
   }
}
if ($readRecord)
{
  // not POST
  $fail = retrieveRegistrant($db_conn, $registrant, $userID);
  if ($fail != '')
  {
     notifyError($fail, "register.php");
     $corrMsg ="<it>Internal: failed access to registration record of ".$userID."</it>";
  }
}
dbClose($db_conn);

if ($wasUpdated)
{
   // post change interface
   if (isset($_POST["submit"]))
   {
     $nextURL = "payRegFee.php";
   }
   else
   {
     $nextURL = "index.php";
   }
   sendUpdateEmail($email, $registrant);
   getNextPage($nextURL);
}
else
{
   // flying information form
   // $corrMsg has HTML content
   // $registrant has POST content
   startHead("Nationals Flying Information");
   flyingFormHeader();
   startContent("onload='checkOwnerPilot()'");
   echo "<h1>Flying Information</h1>";
   verificationHeader("For");
   if ($corrMsg != '')
   {
      echo '<ul class="error">'.$corrMsg.'</ul>';
   }
   flyingForm($registrant, "flying.php");
   echo '<div class="returnButton"><a href="index.php">Return without saving</a></div>';
   endContent();
}
}

if (isRegOpen())
{
    doFlyingInfo();
}
else
{
   startHead("Nationals Flying Information");
   flyingFormHeader();
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
