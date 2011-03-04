<?php
/*  broadcast.php, acrs, dlco, 10/23/2010
 *  broadcast email to contest participants
 *
 *  Changes:
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.
 */

set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ('data/timecheck.inc');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("query/userQueries.inc");

function getMailingList($db_conn,&$toList)
{
    $fail = '';
    $toList = '';
    $query = 'select a.givenName, a.familyName, a.email ' .
    ' from registrant a, reg_type f' .
    ' where a.userID = f.userID ' .
    ' and f.ctstID = ' . $_SESSION['ctstID'] .
    " and f.compType in ('competitor', 'volunteer')";
    // todo add volunteer
    debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    } else
    {
        $first = true;
        $curRcd = dbFetchAssoc($result);
        while ($curRcd)
        {
            if (!$first)
            {
              $toList .= ', ';
            }
            else
            {
               $first = false;
            }
            $toList .= $curRcd['givenName'] . ' ' . $curRcd['familyName'] . ' <' . $curRcd['email'] . '>';
            $curRcd = dbFetchAssoc($result);
        }
    }
    return $fail;
}

function displayToList($msg, $toList)
{
      $toListDisplay = strhtml($toList);
      echo '<p>'.$msg.'<ul><li>'.
        str_replace(",","</li>\n<li>",$toListDisplay)."</li></ul></p>\n";
}

function messageForm($action, $subject, $message)
{
    echo '<form id="messageForm" class="regForm" action="' . $action . '" method="post">' . "\n";

    echo '<p class="regItem"><label for="input1">Subject:</label><input id="input1" name="subject" maxlength="80" size="48" value="'.strhtml($subject).'"/></p>' . "\n";
    echo '<p class="regItem"><label for="input2">Message:</label><textarea id="input2" name="message" maxlength="1024" rows="12" cols="80">'.strhtml($message).'</textarea></p>' . "\n";

    // Submission
    echo '<div class="regSubmit">' . "\n";
    echo '<input class="submit" name="send" type="submit" value="Send the message"/>' . "\n";
    echo '</div>'. "\n";
    echo '</form>';
}

function broadcast($db_conn)
{
   $toList = '';
   $fail = getMailingList($db_conn, $toList);
   if ($fail == '')
   {
   $message = $_POST;
   $subject = '';
   $msg = '';
   $wasMailed = false;
   debugArr('broadcast post data:', $message);
   $corrMsg = '';
   if (isset($message['send']))
   {
      $subject = trim($message['subject']);
      if (strlen($subject) == 0)
      {
         $corrMsg = '<li>Provide a subject.</li>';
      }
      $msg = trim($message['message']);
      if (strlen($msg) == 0)
      {
         $corrMsg .= '<li>Provide a message.</li>';
      }
      if ($corrMsg == '')
      {
        $headers = 'From: ' . $_SESSION['email'] . "\r\n"; 
        $headers .= 'Bcc: ' . ADMIN_EMAIL . "\r\n"; 
        do_email($toList, $subject, $msg, $headers);
        $wasMailed = true;
      }
   }
   if ($wasMailed)
   {
      displayToList('Your message went to', $toList);
   }
   else
   {
      if ($corrMsg != '')
      {
          echo '<ul class="error">' . $corrMsg . '</ul>';
      }
      messageForm('broadcast.php', $subject, $msg);
      displayToList('Your message will go to', $toList);
   }
   }
   else
   {
      notifyerror($fail, 'broadcast.php');
      echo '<p class="error"> Failed to read the list of registrants.</p>';
   }
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "broadcast.php");
    $corrMsg = "<it>Internal: failed access to contest database</it>";
    $readRecord = FALSE;
}
startHead("Broadcast Message");
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
startContent();
echo '<h1 class="noprint">Write to contest registrants</h1>';
if ($corrMsg != '')
{
    echo '<ul class="error">' . $corrMsg . '</ul>';
} else
{
    if (isContestOfficial())
    {
        broadcast($db_conn);
    } else
    {
        echo '<p class="error">Restricted to contest officials.</p>';
    }
}
if ($fail != '')
{
    $eMsg = notifyError('Data temporarily unavailable.', $fail);
    echo '<p>' . $eMsg . '</p>';
}
echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
endContent();
dbClose($db_conn);
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
