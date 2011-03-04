<?php
session_start();
require_once ("dbConfig.inc");
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodeHTML.inc");
require_once ("securimage.inc");
require_once ("useful.inc");

function validateNewAccount($registrant)
{
   $accountName = crop($registrant['accountName'], 32);
   if (strlen($accountName) == 0)
   {
      $corrMsg .= "<li>Provide an account name.</li>";
   }
   $givenName = crop($registrant["givenName"], 80);
   if (strlen($givenName) == 0)
   {
      $corrMsg .= "<li>Provide a given (first) name.</li>";
   }
   $familyName = crop($registrant["familyName"], 80);
   if (strlen($familyName) == 0)
   {
      $corrMsg .= "<li>Provide a family (last) name.</li>";
   }
   $email = crop($registrant["email"], 320);
   if (strlen($email) == 0)
   {
      $corrMsg .= "<li>Provide an email address.</li>";
   }
   else
   if (!validEmail($email))
   {
      $corrMsg .= "<li>The format of the email address is not valid.</li>";
   }
   return $corrMsg;
}

function validateUniqueAccountName($registrant, $db_conn)
{
   $accountName = $registrant['accountName'];
   $query = 'select userID from registrant where accountName = ' .
   strSQL($accountName, 32) . ';';
   //debug('validateUniqueAccountName: '.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $corrMsg = '<li>' . notifyError(dbErrorText(), 'validateNewAccount.php') . '</li>';
   }
   else
   if (dbCountResult($result) != 0)
   {
      $corrMsg = '<li>The account name is already registered.'.
      '<a href="reset.php?email='.strhtml($registrant['email']).
      '"> Send password reset </a></li>';
   }
   return $corrMsg;
}

function isUniqueEmail($registrant, $db_conn)
{
   $isUnique = isset($registrant['useDuplicateEmail']);
   if (!$isUnique)
   {
      $query = 'select userID from registrant where eMail = ' .
      strSQL($registrant['email'],320) . ';';
      //debug('isUniqueEmail: '.$query);
      $result = dbQuery($db_conn, $query);
      if ($result === false)
      {
         notifyError(dbErrorText(), 'isUniqueEmail');
      }
      else
      {
         $isUnique = (dbCountResult($result) == 0);
      }
   }
   return $isUnique;
}
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
