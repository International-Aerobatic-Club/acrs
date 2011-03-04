<?php
set_include_path('./include');
require ('ui/validate.inc');
require_once ('data/encodeHTML.inc');
require_once ('data/encodeSQL.inc');
require_once ('dbConfig.inc');
require_once ('dbCommand.inc');
require_once ('ui/siteLayout.inc');
require_once ('useful.inc');

function writeAccountUser($record)
{
   echo '<tr> ' . "\n";
   echo '<td> ' . strhtml($record["email"]) . '</td> ' . "\n";
   echo '<td> ' . inthtml($record["userID"]) . '</td> ' . "\n";
   echo '<td> ' . strhtml($record["accountName"]) . '</td> ' . "\n";
   echo '<td> ' . strhtml($record["givenName"]) . '</td> ' . "\n";
   echo '<td> ' . strhtml($record["familyName"]) . '</td> ' . "\n";
   echo '<td> ' . strhtml($record["contactPhone"]) . '</td> ' . "\n";
   echo '<td> ' . datehtml($record["updated"]) . '</td> ' . "\n";
   echo '</tr> ' . "\n";
}

function writeRegistration($record)
{
   echo '<tr> ' . "\n";
   echo '<td/> '. "\n";
   echo '<td/> '. "\n";
   echo '<td> ' . inthtml($record["regYear"]) . '</td> ' . "\n";
   echo '<td colspan="3"> ' . strhtml($record["name"]) . '</td> ' . "\n";
   echo '<td> ' . datehtml($record["endDate"]) . '</td> ' . "\n";
   echo '</tr> ' . "\n";
}

function writeAccounts($result)
{
   echo '<div class = "break-after"> ' . "\n";
   echo '<p><table class="pilot-rpt">';
   echo '<thead><tr>';
   echo '<th>email</th> ' . "\n";
   echo '<th>userID</th> ' . "\n";
   echo '<th>accountName</th> ' . "\n";
   echo '<th>givenName</th> ' . "\n";
   echo '<th>familyName</th> ' . "\n";
   echo '<th>contactPhone</th> ' . "\n";
   echo '<th>updated</th> ' . "\n";
   echo '</tr><tr><th/><th/>';
   echo '<th>regYear</th> ' . "\n";
   echo '<th colspan="3">name</th> ' . "\n";
   echo '<th>endDate</th> ' . "\n";
   echo '<tbody> ' . "\n";
   $lastUID = '';
   $curRcd = dbFetchAssoc($result);
   while ($curRcd)
   {
      if ($lastUID != $curRcd['userID'])
      {
         writeAccountUser($curRcd);
         $lastUID = $curRcd['userID'];
      }
      writeRegistration($curRcd);
      $curRcd = dbFetchAssoc($result);
   }
   echo '</tbody></table></p>' . "\n";
   echo '</div> ' . "\n";
   return '';
}

function reportAccounts($db_conn)
{
   $query = 'select a.userID, a.accountName, a.givenName, a.familyName, a.email, a.contactPhone, a.updated, '.
    ' c.regYear, c.name, c.endDate' . 
    ' from registrant a left outer join reg_type b'.
    ' on b.userID = a.userID, contest c' .
    ' where c.ctstID = b.ctstID' .
    ' order by a.email, a.userID desc, c.ctstID desc;';
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      $fail = writeAccounts($result);
   }
   return $fail;
}

function writeChangeForm()
{
   echo '<form method="post">'.
   '<p>Merge registration entries from id '.
   '<input type="text" name="from"/> into those for id '.
   '<input type="text" name="to"/>'.
   '<input type="submit" name="doMerge" value="Merge and delete"/></p>';
}

function reallyDeleteRecords($db_conn, $userID, $ctstID, $regID)
{
   $fail = '';
   $query = 'delete from registration, reg_type where regID = '.$regID.';';
   debug('accountAdmin.reallyDeleteRecords query '.$query);
   $fail = dbExec($db_conn, $query);
   if ($fail == '')
   {
      $query = 'delete from volunteer where userID = '.$userID.' and catID in ('.
      'select catID from ctst_cat where ctstID = '.$ctstID.');';
      debug('accountAdmin.reallyDeleteRecords query '.$query);
      $fail = dbExec($db_conn, $query);
   }
   if ($fail == '')
   {
      $query = 'delete from practice_slot where userID = '.$userID.' and sessID in ('.
      'select sessID from session where ctstID = '.$ctstID.');';
      debug('accountAdmin.deleteRecords query '.$query);
      $fail = dbExec($db_conn, $query);
   }
   return $fail;
}

function provisionallyDeleteRecords($db_conn, $userID, $ctstID, $regID)
{
   $fail = '';
   $query = 'select from pptxn where regID = '.$regID.';';
   debug('accountAdmin.provisionallyDeleteRecords query '.$query);
   $result = dbQuery($db_conn, $query);
   if ($result !== false && 0 < dbCountResult($result))
   {
      echo '<p>User '.$userID.' has payments for contest '.$ctstID.'</p>';
   }
   else
   {
      $fail = reallyDeleteRecords($db_conn, $userID, $ctstID, $regID);
   }
   return $fail;
}

function executeMerge($db_conn, $from, $to)
{
   $fail = dbBegin($db_conn);
   if ($fail == '')
   {
      // find duplicate contests and delete the 'from' side of those.
      $query = 'select regID, ctstID from reg_type where userID = '.$from.' and ctstID in ( '.
      'select ctstID from reg_type where userID = ' . $to . ');';
      debug('accountAdmin.executeMerge query '.$query);
      $result = dbQuery($db_conn, $query);
      if ($result === false)
      {
         $fail = dbErrorText();
      }
      else
      {
         $curRcd = dbFetchAssoc($result);
         while ($curRcd && $fail == '')
         {
            $dupCtst = $curRcd['ctstID'];
            $dupReg = $curRcd['regID'];
            echo '<p>Contest ctstID ' . $dupCtst . ', regID ' . $dupReg . ' is a duplicate.</p>';
            $fail = provisionallyDeleteRecords($db_conn, $from, $dupCtst, $dupReg);
            $curRcd = dbFetchAssoc($result);
         }
      }
      if ($fail == '')
      $fail = dbCommit($db_conn);
      else
      $fail=dbRollback($db_conn);
   }
   return $fail;
}

function processPost($db_conn, $post)
{
   $from = intSQL($post['from']);
   $to = intSQL($post['to']);
   if ($from != 'null' && $to != 'null' && $from != $to)
   {
      $fail = executeMerge($db_conn, $from, $to);
      echo '<p>Merged from id ' . strhtml($post['from']) .
   ' to ' . strhtml($post['to']) . '</p>';
   }
   else
   {
      $fail = '<li>Provide two different integer id\'s</li>';
   }
   return $fail;
}

$corrMsg = '';
startHead("Account Administration");
startContent();
verificationHeader('Welcome');
echo '<h1 class="noprint">Account Administration</h1>';
if (!isAdministrator())
{
   $corrMsg = '<li>Restricted to system administrator.</li>';
}
if ($corrMsg == '')
{
   $fail = dbConnect($db_conn);
   if ($fail != '')
   {
      notifyError($fail, "accountAdmin.php");
      $corrMsg = "<it>Internal: failed access to contest database</it>";
   }
   else if ($_POST['doMerge'])
   {
      $corrMsg = processPost($db_conn, $_POST);
      debug('accountAdmin.postresult:', $corrMsg);
   }
   if ($fail == '')
   {
      $fail = reportAccounts($db_conn);
   }
   if ($db_conn)
   dbClose($db_conn);
   if ($fail != '')
   {
      $eMsg = notifyError('Database connect', 'accountAdmin.php: ' . $fail);
   }
   else
   {
      writeChangeForm();
   }
}
if ($corrMsg != '')
{
   echo '<ul class="error">' . $corrMsg . '</ul>';
}
echo '<p><a href="index.php">Return to registration</a></p>';
endContent();

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
