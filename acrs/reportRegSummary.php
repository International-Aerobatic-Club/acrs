<?php
/*  reportRegSummary.php, acrs, dclo, 10/24/2010
 *  display, optionally print, summary of all registrants for the current contest
 *
 *  Changes:
 *    10/24/2010 jim_ward	include non-flying volunteers.
 */

set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("query/userQueries.inc");
require_once ("data/encodeHTML.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

function showRegistrant($record)
{
    echo '<tr class="reportHead">' . "\n";
    echo '<td colspan="2" class="reportHead">' . strhtml($record["givenName"]) . ' ' .
    strhtml($record["familyName"]) . '</td>' . "\n";
    echo '<td class="reportText">' . strhtml($record["iacID"]) . '</td>' . "\n";
    echo '<td class="reportText">' . (isset ($record['catName'])? strhtml ($record['catName']) : '') . '</td>' . "\n";
    if (boolChecked($record, "fourMinFree"))
    {
        echo '<td class="reportText">4 min free' . "</td>\n";
    }
    if (boolChecked($record, "teamAspirant"))
    {
        echo '<td class="reportText">Team' . "</td>\n";
    }
    if (boolChecked($record, "isFirstTime"))
    {
        echo '<td class="reportText">First time</td>' . "\n";
    }
    if (boolChecked($record, "isStudent"))
    {
        echo '<td class="reportText">Student</td>' . "\n";
    }
    echo "</tr><tr>\n";
    echo '<td colspan="2" class="reportText">' . strhtml($record["email"]) . '</td>' . "\n";
    echo '<td class="reportText">' . (isset ($record["contactPhone"])? strhtml($record["contactPhone"]) : "") . '</td>' . "\n";
    if (boolChecked($record, "hasPayPal") && isset ($record["paidAmt"]))
    {
        $feePaid = intVal($record['paidAmt']);
        echo '<td class="reportText">$' . $feePaid . '</td>';
        $fee = computeRegistrationFee($record);
        if ($fee < $feePaid)
        {
            echo '<td colspan="4" class="reportText">$' . ($feePaid - $fee) . ' overpayment.</td>' . "\n";
        } else
        {
            if ($feePaid < $fee)
            {
                echo '<td colspan="4" class="reportText">$' . ($fee - $feePaid) . ' due.</td>' . "\n";
            } else
            {
                echo '<td colspan="4" class="reportText">paid in full.</td>' . "\n";
            }
        }
    }
    echo '<td class="reportText">Size '. strhtml($record['shirtsize']) . '</td>' . "\n";
    echo '</tr>' . "\n";
}

function doSummaryReport($db_conn)
{
    $fail   = '';
    $ctstID = $_SESSION['ctstID'];
    $query  = 'select  ' .
              'a.teamAspirant, a.fourMinFree, a.airplaneMake, a.insCompany, ' .
              'a.isStudent, a.paidAmt, a.hasVotedJudge, ' .
              'b.name, b.catID, b.class, b.category, b.regAmt, b.hasStudentReg, b.studentRegAmt, ' .
              'b.hasTeamReg, b.teamRegAmt, b.hasVoteJudge, ' .
              'b.voteTeamOnly, b.hasFourMinute, b.fourMinRegAmt, ' . 
              'c.compType, ' . 
              'd.givenName, d.familyName, d.email, d.iacID, d.shirtsize, ' .
              'e.hasPayPal';
    $query .= ' from registration a, ctst_cat b, reg_type c, registrant d, contest e where';
    $query .= ' c.ctstID = ' . $ctstID;
    $query .= " and c.compType = 'competitor'";
    $query .= ' and e.ctstID = c.ctstID';
    $query .= ' and d.userID = c.userID';
    $query .= ' and a.regID = c.regID';
    $query .= ' and b.catID = a.catID';
    $query .= ' order by catID, category, class, familyName, givenName';
    debug($query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = 'error ' . dbErrorText() . ' on registrant query.';
    }

    $curCat = -1;
    $total = 0;
    $volTotal = 0;

    if ($fail == '')
    {
        $catTotal = 0;
        $record = dbFetchAssoc($result);
        while ($record)
        {
            sqlBoolValueToPostData($record['teamAspirant'], 'teamAspirant', $record);
            sqlBoolValueToPostData($record['fourMinFree'], 'fourMinFree', $record);
            sqlBoolValueToPostData($record['hasFourMinute'], 'hasFourMinute', $record);
            sqlBoolValueToPostData($record['isStudent'], 'isStudent', $record);
            sqlBoolValueToPostData($record['hasPayPal'], 'hasPayPal', $record);
            sqlBoolValueToPostData($record['voteTeamOnly'], 'voteTeamOnly', $record);
            sqlBoolValueToPostData($record['hasVoteJudge'], 'hasVoteJudge', $record);
            sqlBoolValueToPostData($record['hasVotedJudge'], 'hasVotedJudge', $record);
            sqlBoolValueToPostData($record['hasTeamReg'], 'hasTeamReg', $record);
            sqlBoolValueToPostData($record['hasStudentReg'], 'hasStudentReg', $record);
            if ($curCat != $record['catID'])
            {
                if ($curCat != -1)
                {
                    echo '</tbody></table>' . "\n";
                    echo '<p>' . $catTotal . ' registrant' . ($catTotal==1? "" : "s") . ' in category.' . "</p>\n";
                }
                $catTotal = 0;
                $curCat = $record['catID'];
                echo '<h4 class="registrant">' . $record['name'] . "</h4>\n";
                echo '<table class="report"><tbody>' . "\n";
            }
            showRegistrant($record);
            ++$total;
            ++$catTotal;
            $record = dbFetchAssoc($result);
        }

        $query =  'select  ' .
                  'c.compType, ' . 
                  'd.givenName, d.familyName, d.email, d.iacID, d.shirtsize, ' .
                  'e.hasPayPal';
        $query .= ' from reg_type c, registrant d, contest e where';
        $query .= ' c.ctstID = ' . $ctstID;
        $query .= " and c.compType = 'volunteer'";
        $query .= ' and e.ctstID = c.ctstID';
        $query .= ' and d.userID = c.userID';
        $query .= ' order by familyName, givenName';
        debug($query);
        $result = dbQuery($db_conn, $query);
        if (dbErrorNumber() != 0)
            $fail = 'error ' . dbErrorText() . ' on registrant (volunteer) query.';
    }

    if ($fail == '')
    {
        while ($record = dbFetchAssoc($result))
        {
            if ($volTotal == 0)
            {
                if ($curCat != -1)
                {
                    echo '</tbody></table>' . "\n";
                    echo '<p>' . $catTotal . ' registrant' . ($catTotal==1? "" : "s") . ' in category.' . "</p>\n";
                }
                $catTotal = 0;
                $curCat = -1;
                echo '<h4 class="registrant">Non-flying volunteers' . "</h4>\n";
                echo '<table class="report"><tbody>' . "\n";
            }
            showRegistrant($record);
            ++$total;
            ++$volTotal;
        }   /* while */
    }   /* no query failure */

    if ($fail == '')
    {
        echo '</tbody></table>' . "\n";
        if ($volTotal == 0)
            echo '<p>' . $catTotal . ' registrant' . ($catTotal==1? "" : "s") . ' in category.' . "</p>\n";
        else
            echo '<p>' . $volTotal . ' non-flying volunteer' . ($volTotal == 1? "" : "s") . ".</p>\n";
        echo '<p>' . $total . ' total registrant' . ($total == 1? "" : "s") . ".</p>\n";
    } else
    {
        notifyError($fail, "reportRegSummary:retrieveRegistrant()");
        echo '<p style="color:red; font-weight:bold">' . $fail . '</p>';
    }
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "reportRegSummary.php");
    $corrMsg = "<it>Internal: failed access to contest database</it>";
    $readRecord = FALSE;
} else
{
    startHead("Registration Summary");
    echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
    echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
    startContent();
    echo '<h1 class="noprint">Registration Summary</h1>';
    echo '<p class="noprint"><input style="margin-right:20px" type="button" onClick="window.print()" ' .
    'value="Print This Page"/><a href="index.php">Return to registration</a></p>';
    if ($corrMsg != '')
    {
        echo '<ul class="error">' . $corrMsg . '</ul>';
    } else
    {
        if (isContestOfficial())
        {
            doSummaryReport($db_conn);
        } else
        {
            echo '<p class="error">Restricted to contest officials.</p>';
        }

    }
    echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
    endContent();
}
dbClose($db_conn);
?>
<?php
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
