<?php
set_include_path('./include');
require ("ui/validate.inc");
require_once('data/validCtst.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("useful.inc");

function showRegistrant($record)
{
   $iacID = $record['iacID'];
   if (!isset($iacID) || $iacID == '') $iacID = 0;
   $category = ucwords($record['category']);
   echo '<Pilot>';
   echo '<IACNumber>'.strhtml($iacID).'</IACNumber>';
   echo '<Name><Last>'.strhtml($record['familyName']).
   '</Last><First>'.strhtml($record['givenName']).'</First></Name>';
   echo '<Chapter>'.strhtml($record['chapter']).'</Chapter>';
   echo '<Category>' . strhtml($category) . '</Category>';
   echo '<Flights>Known Freestyle Unknown1</Flights>';
   echo '<Aircraft><NNumber>'.strhtml($record['airplaneRegID']).
   '</NNumber><Make>'.strhtml($record['airplaneMake']).
   '</Make><Model>'.strhtml($record['airplaneModel']).
   '</Model></Aircraft>';
   echo '</Pilot>' . "\n";
}

function doExport($db_conn)
{
   $fail = '';
   $ctstID = $_SESSION['ctstID'];
   $query = 'select a.chapter, ' .
            'a.airplaneMake, a.airplaneModel, a.airplaneRegID, ' .
            'b.category, ' .
            'c.compType, ' . 
            'd.givenName, d.familyName, d.iacID';
   $query .= ' from registration a, ctst_cat b, reg_type c, registrant d where';
   $query .= ' c.ctstID = ' . $ctstID;
   $query .= " and c.compType = 'competitor'";
   $query .= ' and d.userID = c.userID';
   $query .= ' and a.regID = c.regID';
   $query .= ' and b.catID = a.catID';
   $query .= ' order by a.catID, d.familyName, d.givenName';
   debug($query);
   $result = dbQuery($db_conn, $query);
   if (dbErrorNumber() != 0)
   {
      $fail = 'error ' . dbErrorText() . ' on registrant query.';
   }
   if ($fail == '')
   {
      $curCat = -1;
      $total = 0;
      $catTotal = 0;
      $record = dbFetchAssoc($result);
      header('Content-type: text/xml');
      header('Content-Disposition: attachment; filename="pilots.xml"');
      echo "<?xml version='1.0'?>\n";
      echo "<Pilots>\n";
      while ($record)
      {
         showRegistrant($record);
         $record = dbFetchAssoc($result);
      }
      echo "\n</Pilots>\n";
   } else
   {
      notifyError($fail, "exportJaSPer:doExport()");
   }
   return $fail;
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
   notifyError($fail, "exportJaSPer.php");
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
   startHead("JaSPer export error");
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>' . "\n";
   startContent();
   echo '<h1>JaSPer export error</h1>';
   echo '<ul class="error">' . $corrMsg . '</ul>';
   echo '<p><a href="index.php">Return to registration</a></p>';
   endContent();
} else

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
