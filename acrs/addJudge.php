<?php
/*  addJudge.php, acrs, dlco, 10/23/2010
 *  add a judge to the ballot
 *
 *  Changes:
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.
 */

set_include_path('./include'); 
require_once ("ui/validate.inc");
require_once ("data/validCtst.inc");
require_once ('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodeHTML.inc");
require_once ("query/userQueries.inc");
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");

$_enumSet_region = array (
    'northeast',
    'southeast',
    'midamerica',
    'southcentral',
    'northwest',
    'southwest'
);

function judgeExists($db_conn, $judge)
{
    $haveJudge = false;
    $query = "select region from judge where iacID = " . intSQL($judge['iacID']) . " and ctstID = " . intSQL($_SESSION['ctstID']);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        notifyError(dbErrorText(), 'judgeExists');
    } else
    {
        $haveJudge = (dbCountResult($result) != 0);
    }
    return $haveJudge;
}

/*
Add a judge record to the list of national judges.
*/
function insertOrUpdateJudge($db_conn, $judge)
{
    global $_enumSet_region;
    //debugArr('addJudge.insertOrUpdateJudge, data is ', $judge);
    if (judgeExists($db_conn, $judge))
    {
        $query = 'update judge set '; 
        $query .= ' region = ' . enumSQL($judge['region'], $_enumSet_region) . ',';
        $query .= ' givenName = ' . strSQL($judge['givenName'],72). ',';
        $query .= ' familyName = ' . strSQL($judge['familyName'],72). ',';
        $query .= ' contactPhone = ' . strSQL($judge['contactPhone'],16). ',';
        $query .= ' availableDate = ' . dateSQL($judge['availableDate']);
        $query .= ' where iacID = ' . strSQL($judge['iacID'],12);
        $query .= ' and ctstID = ' . intSQL($_SESSION['ctstID']);
    } else
    {
        $query = 'insert into judge(iacID, ctstID, region, givenName, familyName, contactPhone, availableDate) values';
        $query .= '(' . strSQL($judge['iacID'],12) . ',' . intSQL($_SESSION['ctstID']) . ',';
        $query .= enumSQL($judge['region'], $_enumSet_region) . ',';
        $query .= strSQL($judge['givenName'], 72) . ',';
        $query .= strSQL($judge['familyName'], 72) . ',';
        $query .= strSQL($judge['contactPhone'], 16) . ',';
        $query .= dateSQL($judge['availableDate']) . ')';
    }
    //debug($query);
    return dbExec($db_conn, $query);
}

/*
Remove a judge record from the list of national judges.
*/
function removeJudge($db_conn, $judge)
{
    $fail = '';
    if (judgeExists($db_conn, $judge))
    {
        $query = 'delete from judge ' .
        $query .= ' where iacID = ' . strSQL($judge['iacID'],12);
        $query .= ' and ctstID = ' . intSQL($_SESSION['ctstID']);
        //debug($query);
        $fail = dbExec($db_conn, $query);
    }
    return $fail;
}

function validateForAdd($db_conn, &$judge)
{
    $givenName = crop($judge['givenName'], 80);
    $familyName = crop($judge['familyName'], 80);
    $contactPhone = crop($judge['contactPhone'], 320);
    $availableDate = crop($judge['availableDate'], 10);
    $corrMsg = '';
    // all fields valid, check name against registrants using iacID
    $query = 'select givenName, familyName from registrant where iacID = ' . strSQL($judge['iacID'], 12);
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $corrMsg .= '<it>' . dbErrorText() . '<\it>';
    } else
        if (dbCountResult($result) != 0)
        {
            // have a record with this iacID
            $row = dbFetchRow($result);
            //debug('existing uid is '.$judge['iacID']);
            if (strtolower(addslashes($givenName)) != strtolower($row[0]) || 
                strtolower(addslashes($familyName)) != strtolower($row[1]))
            {
                $corrMsg .= '<it>The judge name did not match the information on file for this IAC member number.' .
                ' If the information now shown is correct, resubmit as shown;' .
                " otherwise, <a href='mailto:" . ADMIN_EMAIL . "'?subject=" . urlencode('subject=id conflict') . "'>write to the administrator</a>.</it>";
                $judge['givenName'] = strhtml($row[0]);
                $judge['familyName'] = strhtml($row[1]);
            }
        } else
        {
            // do not have a record with this iacID
            // check family name
            if (strlen($familyName) == 0)
            {
                $corrMsg .= "<li>Provide a family name.</li>";
            }
            // check given name
            if (strlen($givenName) == 0)
            {
                $corrMsg .= "<li>Provide a given name.</li>";
            }
            // check contactPhone
            if (strlen($contactPhone) == 0)
            {
                $corrMsg .= "<li>Provide a contact phone number.</li>";
            }
            // check available date
            if (strlen($availableDate) == 0)
            {
                $corrMsg .= "<li>Provide an available date.</li>";
            }
        }
    if (strlen($availableDate) < 6)
    {
       $judge['availableDate'] = $availableDate . '/' . $_SESSION['ctst_year'];
    }
        
    return $corrMsg;
}

function doPost($db_conn, &$judge)
{
  //debugArr("judge post data", $judge);
  //debugArr("session data", $_SESSION);
  
  $corrMsg = '';
  $iacID = crop($judge["iacID"], 12);
  if (strlen($iacID) == 0)
  {
      $corrMsg = "<li>Provide an IAC member number.</li>";
  }

  $fail = '';
  if ($corrMsg == '' && isset($judge['add']))
  {
      $event = 'Added judge';
      $corrMsg = validateForAdd($db_conn, $judge);
      if ($corrMsg == '')
      {
      $fail = insertOrUpdateJudge($db_conn, $judge);
      }
  }
  else if ($corrMsg == '')
      {
          $event = 'Removed judge';
          $fail = removeJudge($db_conn, $judge);
      }
  if ($fail != '')
          {
          $corrMsg .= "<it>DB error: " . $fail . " processing judge.</it>";
          }
  if ($corrMsg == '')
      {
          $corrMsg = '<it>' . $event . ' ' . $judge['givenName'] . ' ' . $judge['familyName'] . ' in ' . $judge['region'] . ' region.</it>';
          $judge['givenName'] = null;
          $judge['familyName'] = null;
          $judge['contactPhone'] = null;
          $judge['iacID'] = null;
          $judge['availableDate'] = null;
      }
   return $corrMsg;
}

function displayJudgeForm($judge)
{
        echo "<form name=\"addJudge\" method=\"post\" class=\"userForm\">";
        echo "<table> <tbody> <tr> \n";
        echo "<tr><td class=\"requiredInput\">Given Name:</td><td><input tabindex=\"1\" type=\"text\" name=\"givenName\" value=\"" . $judge["givenName"] . "\"/></td></tr>\n";
        echo "<tr><td class=\"requiredInput\">Family Name:</td><td><input tabindex = \"1\" type=\"text\" name=\"familyName\" value=\"" . $judge["familyName"] . "\"/></td></tr>\n";
        echo "<td class=\"requiredInput\">Contact Phone:</td><td><input tabindex = \"1\" type=\"text\" name=\"contactPhone\" size=\"16\" value=\"" . $judge["contactPhone"] . "\"/></td></tr>\n";
        echo "<td class=\"requiredInput\">IAC Member #:</td><td><input tabindex = \"1\" type=\"text\" name=\"iacID\" size=\"12\" value=\"" . $judge["iacID"] . "\"/></td></tr>\n";
        echo "<td class=\"requiredInput\">Available Date:</td><td><input tabindex = \"1\" type=\"text\" name=\"availableDate\" size=\"10\" value=\"" . $judge["availableDate"] . "\"/>(mm/dd or mm/dd/yyyy)</td></tr>\n";
        echo "<tr><td/><td>";

        // Region (region)
        echo '<span class="form_select"><label for="in_region">Region:</label><fieldset id="in_region" legend="Register as">' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="northwest" ' . isSelected($judge, "region", "northwest") . '>northwest</input>' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="southwest" ' . isSelected($judge, "region", "southwest") . '>southwest</input>' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="midamerica" ' . isSelected($judge, "region", "midamerica") . '>midamerica</input>' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="southcentral" ' . isSelected($judge, "region", "southcentral") . '>southcentral</input>' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="northeast" ' . isSelected($judge, "region", "northeast") . '>northeast</input>' . "\n";
        echo '<input tabindex="1" class="form_select" type="radio" name="region" value="southeast" ' . isSelected($judge, "region", "southeast") . '>southeast</input>' . "\n";
        echo '</fieldset></span>' . "\n";

        echo "</td></tr>";

        echo "</tbody></table>\n";
        echo "<input tabindex = \"1\" class=\"submit\" type=\"submit\" name=\"add\" value=\"Add judge\"/>\n";
        echo "<input tabindex = \"1\" class=\"submit\" type=\"submit\" name=\"remove\" value=\"Remove judge\"/>\n";
        echo "</form>\n";
}

function displayCurrentBallot($db_conn)
{
    $fail = '';
    $query = "select *" .
    " from judge where " .
    " ctstID = " . intSQL($_SESSION['ctstID']) .
    " order by region, familyName, givenName";
    debug("displayCurrentBallot query = " . $query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    } else
    {
        $haveTable = false;
        $curRegion = '';
        $curRcd = dbFetchAssoc($result);
        if ($curRcd)
        {
            $haveTable = true;
            echo "<h3>Ballot of Judges</h3>\n";
            echo "<table class='attJudge'><tbody>\n";
        }
        while ($curRcd)
        {
            debugArr("displayCurrentBallot query result row", $curRcd);
            if ($curRcd['region'] != $curRegion)
            {
                $curRegion = $curRcd['region'];
                echo '<tr><th class="attJudgeRegion" colspan="3">' . $curRegion . '</th></tr>';
            }
            echo '<tr class="attJudge"><td class="attJudge">' . strhtml($curRcd["givenName"]) . ' ' . strhtml($curRcd["familyName"]) . '</td><td>' . $curRcd['iacID'] . '</td><td>' . strhtml($curRcd['contactPhone'],16) . '</td><td>' . datehtml($curRcd['availableDate']) . "</td></tr>\n";
            $curRcd = dbFetchAssoc($result);
        }
        if ($haveTable)
        {
            echo "</tbody></table>\n";
        }
    }
    return $fail;
}

// page processing
function processForm($postData)
{
    $corrMsg = '';
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        $corrMsg = "<li>" . strhtml($fail) . "</li>";
    }
    $judge = null;
    $doForm = true;
    if ($corrMsg == '' && (isset ($postData["add"]) || isset ($postData["remove"])))
    {
       $judge = $postData;
       $corrMsg = doPost($db_conn, $judge);
    }
    else
    {
      $judge['region'] = 'southwest';
      $judge['givenName'] = '';
      $judge['familyName'] = '';
      $judge['contactPhone'] = '';
      $judge['iacID'] = '';
      $judge['availableDate'] = '';
    }

      startHead("Judge Ballot");
?>   
   <style>
   div.userSubmit img {vertical-align:middle;}
   </style>
<?php

        startContent();
        echo '<h1>Judge Ballot</h1>';
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        
        displayJudgeForm($judge);
        echo '<div class="returnButton"><a href="index.php">Return to index</a></div>';
        $corrMsg = displayCurrentBallot($db_conn);

        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }

    endContent();
    dbClose($db_conn);
}

if (isContestAdmin())
{
    processForm($_POST);
} else
{
    getNextPage('index.html');
}
?>
<?php

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
