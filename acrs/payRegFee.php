<?php
/*  payRegFee.php, acrs, dlco, 10/23/2010
 *  pay registration fee
 *
 *  Changes:
 *    10/25/2010 jim_ward       use getRegistrantBasicData(), rather than getRegistrationBasicData(), as
 *                              iacID, givenName and familyName are sent to paypal (these are not reported
 *                              by getRegistrationBasicData()).
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.
 *    07/20/2011 dclo           use retrieveRegistrant
 */

set_include_path('./include');
require ('ui/validate.inc');
require ("data/validCtst.inc");
require_once ('dbConfig.inc');
require_once ('dbCommand.inc');
require_once ('ui/siteLayout.inc');
require_once ('useful.inc');
require_once ('query/userQueries.inc');
require ('post/paypal.inc');
require ('ui/disclosures.inc');

$corrMsg = '';
$userID = $_SESSION['userID'];
$ctstID = $_SESSION['ctstID'];
$registrant = array();
$contest = array();
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "payRegFee.php");
    $corrMsg = '<it>Internal: failed access to contest database</it>';
}
if ($fail == '')
{
    $fail = retrieveRegistrant($db_conn, $userID, $ctstID, $registrant);
    if ($fail != '')
    {
        notifyError($fail, "payRegFee.php");
        $corrMsg = '<li>Internal: failed access to registration record.</li>';
    }
}
if ($fail == '')
{
    $fail = getContestData($db_conn, $ctstID, $contest);
    if ($fail != '')
    {
        notifyError($fail, "payRegFee.php");
        $corrMsg = '<li>Internal: failed access to contest record.</li>';
    }
}
if ($fail == '' && !sqlIsTrue($contest['hasPayPal']))
{
    $corrMsg .= "<li>This contest does not accept on-line registration payments.</li>";
}

startHead("Registration Payment");
echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
startContent();
echo "<h1>Registration Payment</h1>";
verificationHeader("For");

if ($corrMsg == '')
{
    $fee = computeRegistrationFee($registrant);
    $pmtDue = $fee - $registrant['paidAmt'];

    if (0 < $pmtDue)
    {
        // payment stuff
        echo "<p>If you believe you have paid and the system still asks for payment, " .
        "please give it a minute to process your transaction notification, " .
        "then refresh your browser. If more than a few <b>hours</b> have elapsed " .
        "since you paid, " .
        "forward your payment confirmation email from PayPal " .
        "to the <a href='mailto:" . ADMIN_EMAIL . "'>the site admin</a>, who " .
        "will straighten it out.</p>";
        echo "<p>Review the fine print, then press the registration payment button to go to PayPal.  If you would like to pay by check or by phone please ";
        echo "<a href='mailto:" . $contest['regEmail'] . "'>contact the registrar</a>.</p>";
        writeDisclosures();
        regularRegistrationButton($pmtDue, $registrant, $contest);
    }
    else
    {
        echo "<p>Your registration is paid in full.</p>";
    }
}
else
{
    echo '<ul class="error">' . $corrMsg . '</ul>';
}
echo '<p><a href="index.php">Return to the registration page</a></p>';
endContent();
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
