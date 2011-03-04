<?php
/*  reset.php, acrs, dlco, 10/23/2010
 *  account password reset
 *
 *  Changes:
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.
 */

session_start();
set_include_path('./include');
require_once ('dbConfig.inc');
require_once ('dbCommand.inc');
require_once ('data/encodeSQL.inc');
require_once ('data/encodeHTML.inc');
require_once ('securimage.inc');
require_once ('useful.inc');
require_once ('ui/siteLayout.inc');
require_once ('ui/emailNotices.inc');
require_once ('data/password.inc');

startHead('Account Password Reset');
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
startContent();
$doForm = TRUE;
$failRobo = FALSE;
if (isset ($_POST["submit"]))
{
   $member["email"] = $_POST["email"];
   $antirobo = new securimage();
   if (!$antirobo->check($_POST["antiRobot"]))
   {
      $failRobo = TRUE;
   }
   else
   {
      $fail = dbConnect($db_conn);
      if ($fail == '')
      {
         $email = crop($_POST["email"], 320);
         $registrant['email'] = $email;
         $query = "select userID, accountName, givenName, familyName from registrant where rtrim(email)=" . strSQL($email,320) . ';';
         $result = dbQuery($db_conn, $query);
         if ($result !== false)
         while ($row = dbFetchAssoc($result))
         {
            foreach ($row as $key => $value)
            {
               $registrant[$key] = stripslashes($value);
            }
            $newPwd = generatePassword();
            $pwd = encodePWD($newPwd);
            $update = "update registrant set password = " . 
               strSQL($pwd, 40) . 
               " where userID = " . inthtml($registrant['userID']);
            $fail = dbExec($db_conn, $update);
            if ($fail == '')
            {
               $fail = sendAccountResetEmail($registrant, $newPwd);
            }
         }
         $doForm = FALSE;
      }
      dbClose($db_conn);
   }
   // post reset interface
   if ($fail != '')
   {
      echo "<p>" . notifyError($fail, "reset.php") . "</p>";
   }
}
if (!$doForm)
{
   echo "<p>Thank you.  If your address, '" . $email .
    "' was on record with us, then we have mailed your new password " .
    "to that address.</p>";
}
else
{
   if (isset ($_GET["email"]))
   {
      $member["email"] = $_GET["email"];
   }
   echo '<p>Enter your email address, copy the anti-robot text, then press the '.
'"Reset" button. If the email address matches an email address on record, '.
'we will mail your new password to that address. Note: If you share this '.
'email address with other account holders, then those accounts will be '.
'reset as well. If you do not know the email address we have on record '.
'for you, if that address is no longer valid, or this otherwise does not '.
'work for you, please ';
   echo '<a href="mailto:' . ADMIN_EMAIL .
    '?subject=contest%20registration%20system%20login%20difficulty">' .
    'email the administrator</a>' . "\n";
   echo '</p>';
   echo '<form method = "post">';
   echo '<table>';
   echo '<tr>';
   echo '<td align="right">email</td>';
   echo '<td><input type="text" size="40" maxlength="80" name="email" value="' . $member["email"] . '"/></td>';
   echo '</tr><tr>';
   echo '<td><img src="antiRoboImage.php"/></td>';
   echo '<td><input class="antiRobot" type="text" size="8" maxlength="8" name="antiRobot"/></td>';
   echo '</tr></table>';
   echo '<input type="submit" name="submit" value="Reset"/>';
   echo '</form>';
   
   if ($failRobo)
   {
      echo '<p style="color: red; font-weight: bold">Copy the text of the image into '.
'the text entry field next to the image. Both upper and lower-case '.
'letters will match. <a href="mailto:'.
      ADMIN_EMAIL.
	'?subject=anti-robot difficulty>Email '.
    'the member administrator</a> if you have difficulty reading the image.</p>';
   }
}
echo '<p><a href="login.php">member login</a></p>';
endContent();
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