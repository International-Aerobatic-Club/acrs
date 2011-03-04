<?php
session_start();
set_include_path('./include');
require_once ("dbConfig.inc");
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodeHTML.inc");
require_once ('data/password.inc');
require_once ("securimage.inc");
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ('ui/emailNotices.inc');
require_once ('post/validateNewAccount.inc');
require_once ('form/newAccountForm.inc');

/**
 Insert new member record into the database
 db_conn database connection handle
 record contains accountName, givenName, familyName, email
 newPwd output, new password valid if success
 returns empty string on success, failure message on failure
 */
function insertNewMemberRecord($db_conn, $record, & $newPwd)
{
   $newPwd = generatePassword();
   $password = encodePWD($newPwd);
   $update = "insert into registrant " .
    "(accountName, password, email, givenName, familyName) " .
    "values (" .
   strsql($record['accountName'], 32) . ',' .
   strsql($password, 40) . ',' .
   strsql($record['email'], 320) . ',' .
   strsql($record['givenName'], 72) . ',' .
   strsql($record['familyName'], 72) . ')';
   return dbExec($db_conn, $update);
}

$doRegForm = TRUE;
$doVerifyForm = FALSE;
$failRobo = FALSE;
$corrMsg = '';
$registrant = $_POST;
if (isset($registrant['changeEmail']))
{
   unset($registrant['email']);
}
else if (isset ($registrant["submit"]))
{
   // begin registration form processing
   $corrMsg = '';
   $db_conn = '';
   $antirobo = new securimage();
   if (!$antirobo->check($registrant["antiRobot"]))
   {
      $failRobo = TRUE;
      $corrMsg = "<li>Copy the anti-robot image text.</li>";
   }
   if ($corrMsg == '')
   {
      //debug('loginAccount.php: validating');
      $corrMsg = validateNewAccount($registrant);
   }
   if ($corrMsg == '')
   {
      //debug('loginAccount.php: dbConnect');
      $fail = dbConnect($db_conn);
      if ($fail != '')
      {
         $corrMsg = "<li>" . strhtml($fail) . "</li>";
      }
   }
   if ($corrMsg == '')
   {
      //debug('loginAccount.php: checkAccountName');
      $corrMsg = validateUniqueAccountName($registrant, $db_conn);
   }
   if ($corrMsg == '')
   {
      //debug('loginAccount.php: unique email');
      if (!isUniqueEmail($registrant, $db_conn))
      {
         $doVerifyForm = true;
         $doRegForm = false;
      }
      else
      {
         //debug('loginAccount.php: insert record');
         $newPwd = '';
         $fail = insertNewMemberRecord($db_conn, $registrant, $newPwd);
         if ($fail == '')
         {
            $doRegForm = FALSE;
            sendAccountEstablishedEmail($registrant, $newPwd);
         }
         else
         {
            $corrMsg = "<li>Internal: " . strhtml($fail) . "</li>";
            notifyError($fail, "loginAccount.php");
         }
      }
   }
   if ($db_conn)
   dbClose($db_conn);
}
startHead('Account Registration');
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
startContent();
if ($doRegForm)
{
   displayNewAccountForm($registrant, $corrMsg);
}
else if ($doVerifyForm)
{
   displayVerifyForm($registrant);
}
else {
   // post change interface
   echo "<p>We have mailed your account credentials to" .
    " the email address you supplied.</p>\n";
   echo "<p>Your account password is in the email message</p>\n";
   echo '<p><a href="index.php">Return to registration</a></p>';
}
endContent();
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
