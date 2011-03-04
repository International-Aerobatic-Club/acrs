<?php
/*  exportRegistrants.php, acrs, dlco, 10/26/2010
 *  export list of all contest registrants for spreadsheet import
 *
 *  Changes:
 *      10/26/2010 jim_ward     fix ref to uninit var.
 */

set_include_path('./include');
require ("ui/validate.inc");
require_once('data/validCtst.inc');
require_once ("data/encodeHTML.inc");
require_once ("data/encodeCSV.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("useful.inc");

function showRegistrant($record)
{
   //debugArr('reportRegPhoneList', $record);
   echo strCSV($record["givenName"]) . ',' .
   strCSV($record["familyName"]) . ',' .
   strCSV($record['email']) . ',' .
   strCSV($record["contactPhone"]) .  ',' .
   strCSV($record["address"]) .  ',' .
   strCSV($record["city"]) .  ',' .
   strCSV($record["state"]) .  ',' .
   strCSV($record["country"]) .  ',' .
   strCSV($record["postalCode"]) . ',' .
   strCSV($record['shirtsize']) .
   "\n";
}

function doExport($db_conn)
{
   $query = 'select givenName, familyName, email,' .
    ' contactPhone, address, city, state, country, postalCode, shirtsize' .
    ' from registrant a, registration b, reg_type d' .
    ' where d.ctstID = ' . $_SESSION['ctstID'] .
    ' and d.compType != \'regrets\'' .
    ' and b.regID = d.regID ' .
    ' and a.userID = d.userID ' .
    ' order by a.familyName, a.givenName';
   $result = dbQuery($db_conn, $query);
   if (dbErrorNumber() != 0)
      $fail = 'error ' . dbErrorText() . ' on exportRegistrants.';
   else
      $fail = '';
   if ($fail == '')
   {
      header('Content-type: text/csv');
      header('Content-Disposition: attachment; filename="registrants.csv"');
      echo '"given name","family name","email","contact phone","address","city","state","country","postal code","shirt size"' . "\n";
      while ($record = dbFetchAssoc($result))
      {
         showRegistrant($record);
      }
   } else
   {
      notifyError($fail, "exportRegistrants:showRegistrants()");
      echo '<p style="color:red; font-weight:bold">Failed database query.</p>';
   }
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "exportRegistrants.php");
   $corrMsg = "<li>Internal: failed access to contest database</li>";
} else
{
   if (!isRegistrar())
   {
      $corrMsg .= '<li>Restricted to contest officials.</li>';
   }
}
if ($corrMsg == '')
{
   $corrMsg = doExport($db_conn);
}
if ($corrMsg != '')
{
   startHead("Registrant export error");
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>' . "\n";
   startContent();
   echo '<h1>Registrant export error</h1>';
   echo '<ul class="error">' . $corrMsg . '</ul>';
   echo '<p><a href="index.php">Return to registration</a></p>';
   endContent();
}
dbClose($db_conn);
/*
 Copyright 2010 International Aerobatic Club, Inc.

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
