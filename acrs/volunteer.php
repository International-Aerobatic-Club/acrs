<?php
set_include_path('./include');
require("ui/validate.inc");
require ("data/validCtst.inc");
require_once("dbConfig.inc");
require_once("dbCommand.inc");
require_once("data/encodeSQL.inc");
require_once("form/volform.inc");
require_once("query/volQueries.inc");
require_once("ui/emailNotices.inc");
require_once("useful.inc");
require_once('data/timecheck.inc');
require_once("redirect.inc");

function doVolunteer()
{
$wasUpdated = FALSE;
$readRecord = TRUE;
$corrMsg = '';
$userID = $_SESSION['userID'];
$email = $_SESSION['email'];
$volunteer = $_POST;
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
   // debugArr('post', $volunteer);
   // debug("user is ".$userID);
   // begin form processing
   $fail = updateVolunteer($db_conn, $volunteer, $userID);
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
if ($readRecord)
{
  // not POST
  $fail = retrieveVolunteer($db_conn, $volunteer, $userID);
  //debugArr('volnteer.php retrieveVolunteer found:', $volunteer);
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
     $nextURL = "flying.php";
   }
   else
   {
     $nextURL = "index.php";
     sendUpdateEmail($email, $volunteer);
   }
   getNextPage($nextURL);
}
else
{
   // volunteer information form
   // $corrMsg has HTML content
   // $volunteer has POST content
   startHead("Volunteer Information");
   volunteerFormHeader();
   startContent();
   echo "<h1>Volunteer Information</h1>";
   verificationHeader("For");
   if ($corrMsg != '')
   {
      echo '<ul class="error">'.$corrMsg.'</ul>';
   }
   $roles = array_combine(getVolunteerRoles(), getRoleDescriptions());
   volunteerForm($volunteer, "volunteer.php", $roles);
   echo '<div class="returnButton"><a href="index.php">Return without saving</a></div>';
   endContent();
}
}

if (isRegOpen())
{
    doVolunteer();
}
else
{
   startHead("Volunteer Information");
   volunteerFormHeader();
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
