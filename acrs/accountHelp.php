<?php
set_include_path('./include');
require ("ui/validate.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("query/userQueries.inc");
require_once ("form/queryUserForm.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

// debugArr('accountHelp session variables:', $_SESSION);

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "accountHelp.php");
   $corrMsg = "<li>Internal: failed access to contest database</li>";
} else
{
   if (!isAdministrator())
   {
      $corrMsg = '<li>Restricted to ACRS Administrator.</li>';
   }
}
if ($db_conn)
{
   dbClose($db_conn);
}

startHead("Account Help");
queryUserFormHeader();
startContent();
if ($corrMsg != '')
{
   echo '<ul class="error">' . $corrMsg . '</ul>';
}
else
{
  queryUserForm('userList.php');
}
echo '<p><a href="/index.php">Return to registration</a></p>';
endContent();


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
