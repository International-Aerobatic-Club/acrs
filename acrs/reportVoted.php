<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("disclosures.php");
require_once ("query/userQueries.inc");
require_once ("data/encodeHTML.inc");
require_once ("useful.inc");

function checkoff($record, $label, $fieldName)
{
    echo '<td class="reportText">' . $label . ': ';
    if (boolChecked($record, $fieldName))
    {
        echo 'checked';
    }
    else
    {
        echo 'NOT checked';
    }
    echo "</td>\n";
}

function showRegistrationData($record)
{
    echo '<table class="reportText"><tbody class="reportText">' . "\n";
    echo '<tr>' . "\n";

    echo '<td colspan="2" class="reportHead">World team aspirants</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td colspan="2" class="reportText">' . strhtml($record["givenName"]) . ' ' . strhtml($record["familyName"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td colspan="2" class="reportText">' . strhtml($record["address"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td class="reportText">' . strhtml($record["city"]) . ', ' . strhtml($record["state"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td class="reportText">' . strhtml($record["country"]) . '</td>' . "\n";
    echo '<td class="reportText">' . strhtml($record["postalCode"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td colspan="2" class="reportText">Email address: ' . strhtml($record["email"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td colspan="2" class="reportText">Cell phone number: ' . strhtml($record["contactPhone"]) . '</td>' . "\n";
    echo '</tr><tr>' . "\n";
    echo '<td class="reportText">EAA Number: ' . strhtml($record["eaaID"]) . '</td>' . "\n";
    echo '<td class="reportText">IAC Number: ' . strhtml($record["iacID"]) . '</td>' . "\n";
    echo "</tr><tr>\n";
    if ($record && isSelected($record, "compType", "competitor"))
    {
        echo '<td class="reportText" colspan="2">Competing as: ' . strhtml($record['compClass']) . ' ' .
        strhtml($record['compCat']);
        if ($record['chapter'] != '')
        {
            echo ', chapter ' . strhtml($record['chapter']);
        }
        echo "</td>\n</tr><tr>\n";
        echo '<td class="reportText" colspan="2">Certificate: ' . strhtml($record['certType']) . ' ' .
        strhtml($record['certNumber']) . '</td>' . "\n";
        echo "</tr><tr>\n";
        if (boolChecked($record, "fourMinFree"))
        {
            echo '<td class="reportText" colspan="2">' . 'Competing in the four minute freestyle event' .
            "</td>\n";
            echo "</tr><tr>\n";
        }
        if (boolChecked($record, "teamAspirant"))
        {
            echo '<td class="reportText" colspan="2">Competing for the world team, FAI ID ' .
            strhtml($record['faiID']) . "</td>\n";
            echo "</tr><tr>\n";
        }
        if (boolChecked($record, "isFirstTime"))
        {
            echo '<td class="reportText" colspan="2">First time competitor</td>' . "\n";
            echo "</tr><tr>\n";
        }
        if (boolChecked($record, "isStudent"))
        {
            echo '<td class="reportText">Student registration</td><td class="reportText">' .
            strhtml($record['university']) . "</td>\n";
            echo "</tr><tr>\n";
            echo '<td class="reportText" colspan="2">' . strhtml($record['program']) . '</td>' . "\n";
            echo "</tr><tr>\n";
        }
        echo '<td colspan="2" class="reportHead">Flying Information</td>' . "\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText" colspan="2">Flying airplane registration ' . strhtml($record['airplaneRegID']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText">Make: ' . strhtml($record['airplaneMake']) .
        '</td><td class="reportText">Model: ' . strhtml($record['airplaneModel']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText">Colors: ' . strhtml($record['airplaneColors']) . "</td>\n";
        echo '<td class="reportText">Airworthiness: ' . strhtml($record['airworthiness']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText">Engine make: ' . strhtml($record['engineMake']) . "</td>\n";
        echo '<td class="reportText">model: ' . strhtml($record['engineModel']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText" colspan="2">Engine horsepower: ' . strhtml($record['engineHP']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td colspan="2" class="reportHead">Owner Information</td>' . "\n";
        echo "</tr><tr>\n";
        echo '<td class="reportText" colspan="2">Owned by ' . strhtml($record['ownerName']) . "</td>\n";
        echo "</tr><tr>\n";
        echo '<td colspan="2" class="reportText">' . strhtml($record["ownerAddress"]) . '</td>' . "\n";
        echo '</tr><tr>' . "\n";
        echo '<td class="reportText">' . strhtml($record["ownerCity"]) . ', ' . strhtml($record["ownerState"]) . '</td>' . "\n";
        echo '</tr><tr>' . "\n";
        echo '<td class="reportText">' . strhtml($record["ownerCountry"]) . '</td>' . "\n";
        echo '<td class="reportText">' . strhtml($record["ownerPostal"]) . '</td>' . "\n";
        echo '</tr><tr>' . "\n";
        echo '<td colspan="2" class="reportText">Phone number: ' . strhtml($record["ownerPhone"]) . '</td>' . "\n";
        echo '</tr><tr>' . "\n";
        if ($record['safety'] != '')
        {
            echo '<td colspan="2" class="reportText">Safety pilot: ' . strhtml($record["safety"]) . '</td>' . "\n";
            echo '</tr><tr>' . "\n";
        }
        echo '<td colspan="2" class="reportHead">Checks</td>' . "\n";
        echo "</tr><tr>\n";
        checkoff($record, '1M Liability', 'liabilityAmt');
        checkoff($record, '100K Injury', 'injuryAmt');
        echo "</tr><tr>\n";
        echo '<td class="reportText">Insurance company: ' . strhtml($record["insCompany"]) . "</td>\n";
        echo '<td class="reportText">expires: ' . strhtml($record["insExpires"]) . "</td>\n";
        echo '</tr><tr>' . "\n";
        checkoff($record, 'Current medical', 'currMedical');
        checkoff($record, 'Current bi-annual', 'currBiAnn');
        echo '</tr><tr>' . "\n";
        checkoff($record, 'Condition Inspection', 'currInspection');
        checkoff($record, 'Current parachute pack', 'currPacked');
        echo '</tr><tr>' . "\n";
    }
    $feePaid = $record['paidAmt'];
    echo '<td class="reportText">Fees paid: $' . $feePaid . '</td>';
    $fee = computeRegistrationFee($record);
    if ($fee < $feePaid)
    {
        echo '<td class="reportText">$' . ($feePaid - $fee) . ' overpayment.</td>' . "\n";
    }
    else
        if ($feePaid < $fee)
        {
            echo '<td class="reportText">$' . $fee - $feePaid . ' registration payment due.</td>' . "\n";
        }
        else
        {
            echo '<td class="reportText">registration paid in full.</td>' . "\n";
        }
    echo '</tr></tbody></table>' . "\n";
}

function showVolunteerData($db_conn, $record)
{
    $fail = '';
    $userID = $record['userID'];
    $ctstID = $_SESSION['ctstID'];
    // retrieve volunteer data (unique to user id)
    $query = 'select category, volunteer from volunteer';
    $query .= ' where userID=' . $userID;
    $query .= ' and ctstID=' . intSQL($ctstID);
    $query .= ' order by category';
    //debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        notifyError($fail, 'printReg');
    }
    else
    {
        echo "<h4 class='reportHead'>Volunteer</h4>\n";
        $cat = '';
        $startedRow = false;
        while ($row = dbFetchRow($result))
        {
            if ($row[0] != $cat)
            {
                $cat = $row[0];
                if ($cat != $record['compCat'])
                {
                    if ($startedRow)
                    {
                        echo '</p>';
                        $startedRow = false;
                    }
                    echo '<p class="volunteer-cat">' . $cat . "</p>\n";
                }
            }
            if ($cat != $record['compCat'])
            {
                if (!$startedRow)
                {
                    echo '<p class="volunteer-list">' . $row[1];
                    $startedRow = true;
                }
                else
                {
                    echo ', ' . $row[1];
                }
            }
        }
        if ($startedRow)
        {
            echo '</p>';
        }
    }
}

$corrMsg = '';
$userID = $_SESSION["userID"];
$registrant = $_POST;
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "register.php");
    $corrMsg = "<it>Internal: failed access to contest database</it>";
    $readRecord = FALSE;
}
else
{
    startHead("Registration");
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
    echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
    startContent();
    echo '<h1 class="noprint">Registration Information</h1>';
    echo '<p class="noprint"><input style="margin-right:20px" type="button" onClick="window.print()" value="Print This Page"/>';
    echo '<a href="index.php">Return to registration</a></p>';
    if ($corrMsg != '')
    {
        echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
    }
    else
    {
        $query = 'select givenName, familyName, email, hasVotedJudge' .
        		' from registrant a, registration b' .
        		' where a.userID = b.userID and compCat = \'advanced\'' .
        				' and compType = \'competitor\' and paidAmt >= 350'.
        				' and ctstID = '.$_SESSION['ctstID'].
        				' order by hasVotedJudge, familyName, givenName';
        $record = dbQuery($db_conn, $query);
        if (dbErrorNumber() != 0)
        {
            $fail = 'error '.dbErrorText().' on voted list.';
        }
        // TODO and so forth
        if ($fail == '')
        {
            showRegistrationData($record);
            showVolunteerData($db_conn, $record);
		    if ($record && isSelected($record, "compType", "competitor"))
    		{
    		    writeDisclosures();
    		}
        }
        else
        {
            notifyError($fail, "reportVoted");
            echo '<p style="color:red; font-weight:bold">' . $fail . '</p>';
        }
    }
    echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
    endContent();
}
dbClose($db_conn);
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
