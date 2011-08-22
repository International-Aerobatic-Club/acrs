<?php
set_include_path('./include');
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("query/userQueries.inc");
require_once ("data/encodeHTML.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");
require_once ("ui/make_form2.inc");
require_once ('ui/make_VolformXlate.inc');
require_once ('ui/cform2.inc');

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "reportVolIAC.php");
   $corrMsg = "<li>Internal: failed access to contest database</li>";
} else
{
   if (!isContestAdmin())
   {
      $corrMsg = '<li>Restricted to contest officials.</li>';
   }
}
if ($corrMsg == '')
{
   $fail = generateIACVolunteerForms($db_conn, $_SESSION['ctstID'], $_SESSION['userID'], false);
   if ($fail != '')
   {
      $corrMsg = '<li>' . $fail . '</li>';
   }
}
if ($db_conn)
{
   dbClose($db_conn);
}
if ($corrMsg != '')
{
   startHead("Volunteers");
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
   echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
   startContent();
   echo '<ul class="error">' . $corrMsg . '</ul>';
   echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
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
