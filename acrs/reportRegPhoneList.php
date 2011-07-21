<?php
/*  reportRegPhoneList.php, acrs, dlco, 10/26/2010
 *  show registrant list for a contest with phone #s
 *
 *  Changes:
 *      10/26/2010 jim_ward     fix ref to uninit var.
 */

set_include_path('./include');
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeHTML.inc");
require_once ("data/encodeCSV.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

function showRegistrant($record)
{
   //debugArr('reportRegPhoneList', $record);
   echo strCSV($record["givenName"]) . ',' .
   strCSV($record["familyName"]) . ',' .
   strCSV($record['category']) . ',' .
   strCSV($record['email']) . ',' .
   strCSV($record["contactPhone"]) . "\n";
}

function showPhoneList($db_conn)
{
   $query = 'select givenName, familyName, email,' .
    ' category, contactPhone' .
    ' from registrant a, registration b, reg_type d, ctst_cat e' .
    ' where d.ctstID = ' . $_SESSION['ctstID'] .
    ' and d.compType = \'competitor\'' .
    ' and b.regID = d.regID ' .
    ' and a.userID = d.userID ' .
    ' and e.catID = b.catID ' .
    ' order by e.category, a.familyName, a.givenName';
   $result = dbQuery($db_conn, $query);
   if (dbErrorNumber() != 0)
      $fail = 'error ' . dbErrorText() . ' on reportRegPhoneList.';
   else
      $fail = '';
   if ($fail == '')
   {
      header('Content-type: text/csv');
      header('Content-Disposition: attachment; filename="contacts.csv"');
      echo '"givenName","familyName","category","email","contactPhone"' . "\n";
      while ($record = dbFetchAssoc($result))
      {
         showRegistrant($record);
      }
   } else
   {
      notifyError($fail, "reportRegPhoneList:showPhoneList()");
      echo '<p style="color:red; font-weight:bold">Failed database query.</p>';
   }
}

$corrMsg = '';
$userID = $_SESSION["userID"];
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "reportRegPhoneList.php");
   $corrMsg = "<li>Internal: failed access to contest database</li>";
} else
{
   if (!isRegistrar())
   {
      $corrMsg = '<li>Restricted to contest officials.</li>';
   }
}
if ($corrMsg == '') {
   $corrMsge =   showPhoneList($db_conn);
}
if ($corrMsg != '') {
   startHead("Contact list export error");
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>' . "\n";
   startContent();
   echo '<h1>Contact list export error</h1>';
   echo '<ul class="error">' . $corrMsg . '</ul>';
   echo '<p><a href="index.php">Return to registration</a></p>';
   endContent();
}
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
