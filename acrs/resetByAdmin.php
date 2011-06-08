<?php
set_include_path('./include');
require ("ui/validate.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodePOST.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodeHTML.inc");
require_once ("data/password.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "userList.php");
   $corrMsg = "<li>Internal: failed access to contest database</li>";
} else
{
   if (!isAdministrator())
   {
      $corrMsg = '<li>Restricted to ACRS Administrator.</li>';
   }
}
$userID = $_GET['id'];
$newPwd = 'iac';
if (isset ($userID))
{
  $pwd = encodePWD($newPwd);
  $update = 'update registrant set password = '. strSQL($pwd, 40) .
    ' where userID = ' .  intSQL($userID);
  // debug('resetByAdmin update statement is ' . $update);
  $fail = dbExec($db_conn, $update);
  if ($fail != '')
  {
    $corrMsg .= '<li>' . 'Failed to reset password' . '</li>';
  }
}
else
{
  $corrMsg .= '<li>Missing query data</li>';
}

startHead("Admin Reset Password");
startContent();
if ($corrMsg != '')
{
   echo '<ul class="error">' . $corrMsg . '</ul>';
}
else
{
  echo '<p>Password reset to "'. strhtml($newPwd) .'"</p>';
}
echo '<p><a href="accountHelp.php">Try another query</a></p>';
echo '<p><a href="index.php">Return to registration</a></p>';
endContent();

if ($db_conn)
{
   dbClose($db_conn);
}


/*
 Copyright 2011 International Aerobatic Club, Inc.

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
