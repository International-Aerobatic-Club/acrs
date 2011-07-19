<?php
/*  ppnotifytxn.php, acrs, dlco, 10/26/2010
 *  receive notification of a transaction from PayPal
 *
 *  Changes:
 *      10/26/2010 jim_ward     use new PAYPAL_SERVER_NAME constant.
 *      07/19/2011 dclo         remove extra validation of account email
 */

set_include_path('./include');
require_once ('dbConfig.inc');
require_once ('dbCommand.inc');
require_once ('useful.inc');
require_once ('ui/siteLayout.inc');
require_once ('query/userQueries.inc');
require_once ('post/paypal.inc');

/**
 * Get registration info for the registrant id
 * return empty string on sucess, or failure message
 */
function getRegInfo($db_conn, $regID, & $regInfo)
{
    $fail = '';
    $query = 'select a.paidAmt, b.payEmail from registration a, contest b, reg_type c ' .
    		'where c.regID =' . intSQL($regID) . ' and '.
    		'a.regID = c.regID and '.
    		'b.ctstID = c.ctstID';
    //debug('ppnotifytxn.getRegInfo() query is:'.$query);
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        $fail = notifyError(dbErrorText() . ' for registration ' . $regID, 'ppnotifytxn.php');
    } else
    {
        //debug('ppnotifytxn.getRegInfo() has result:'.strval($result));
        if (dbCountResult($result) != 1)
        {
            //debug('ppnotifytxn.getRegInfo() count result:'.dbCountResult($result));
            $fail = notifyError('Missing data for registration ' . $regID, 'ppnotifytxn.php');
        } else
        {
            $row = dbFetchAssoc($result);
            //debug('ppnotifytxn.getRegInfo() has row:'.strval($row));
            foreach ($row as $key => $value)
            {
                //debug('ppnotifytxn.getRegInfo() has key "'.$key.'", value "'.$value.'"');
                $regInfo[$key] = stripslashes($value);
            }
        }
    }
    //debugArr('ppnotifytxn.getRegInfo() result :',$regInfo);
    return $fail;
}

/**
Receive notification of a payment or refund from PayPal.
Validate the notification with PayPal.
Record the payment in the database.
*/
function postPayment($db_conn, $txnData)
{
    $regID = $txnData['custom'];
    $regInfo = array();
    $fail = dbBegin($db_conn);
    $haveTxn = $fail == '';

    $payment_currency = $txnData['mc_currency'];
    if (strpos(trim($payment_currency), 'USD') != 0)
    {
        $fail .= 'Cannot process currency, ' . $payment_currency;
    }

    if ($fail == '')
    {
        $fail = getRegInfo($db_conn, $regID, $regInfo);
    }

    if ($fail == '')
    {
        $fail = dbExec($db_conn, "start transaction");
    }

    // record the payment
    if ($fail == '')
    {
        $amt = intval($txnData['mc_gross']);
        $curPaid = intSQL($regInfo['paidAmt']);
        $update = 'update registration set paidAmt = ';
        if ($curPaid == 'null')
        {
            $update .= intSQL($amt);
        } else
        {
            $update .= $curPaid . '+' . intSQL($amt);
        }
        $update .= ' where regID = ' . intSQL($regID);
        debug('ppnotifytxn.postPayment():'.$update);               
        $fail = dbExec($db_conn, $update);
    }

    // record the transaction
    if ($fail == '')
    {
        $update = 'insert into pptxn (txn_id, regID, ' .
        'pay_date, item_name, pay_amt, currency, ' .
        'payer_email, first_name, last_name)' .
        'values (' . strSQL($txnData['txn_id'], 17) . ',' . intSQL($regID) . ',' .
        strSQL($txnData['payment_date'], 28) . ',' . strSQL($txnData['item_name'], 127) . ',' .
        strSQL($txnData['mc_gross'], 10) . ',' . strSQL($txnData['mc_currency'], 3) . ',' .
        strSQL($txnData['payer_email'], 127) . ',' . strSQL($txnData['first_name'], 64) . ',' .
        strSQL($txnData['last_name'], 64) . ')';
        debug('ppnotifytxn.postPayment():'.$update);
        $fail = dbExec($db_conn, $update);
    }

    if ($haveTxn)
    {
        if ($fail == '')
        {
            $fail = dbCommit($db_conn);
        } else
        {
            $fail .= dbRollback($db_conn);
        }
    }
    debug('ppnotifytxn.postPayment():'.$fail);
    return $fail;
}

/**
 * return true if transaction exists
 */
function isDupTransaction($db_conn, $txnID)
{
    $query = 'select regID from pptxn where txn_id =' . strSQL($txnID, 17);
    //debug('ppnotifytxn.isDupTransaction():'.$query);
    $result = dbQuery($db_conn, $query);
    return $result !== false && dbCountResult($result) != 0;
}

/***
 * POST processing
 */
//debug('Here ppnotifytxn.php');
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
$fail = '';
$txnData = $_POST;
$txn_id = $txnData['txn_id'];
debugArr('ppnotifytxn.php has post', $txnData);

// post back to PayPal system to validate
$req = 'cmd=_notify-validate';
foreach ($txnData as $key => $value)
{
    $value = urlencode(stripslashes($value));
    $req .= "&" . $key . "=" . $value;
}
$header .= "POST /cgi-bin/webscr HTTP/1.0\r\n";
$header .= "Content-Type: application/x-www-form-urlencoded\r\n";
$header .= "Content-Length: " . strlen($req) . "\r\n\r\n";
$fp = fsockopen(PAYPAL_SERVER_NAME, 80, $errno, $errstr, 30);
$valid = false;
$dbRes = '';
if (!$fp)
{
    $fail = "failed open connection to " . PAYPAL_SERVER_NAME . ": " . $errstr;
} else
{
    fputs($fp, $header . $req);
    while (!$valid && !feof($fp))
    {
        $res = fgets($fp, 1024);
        $dbRes .= $res;
        $valid = (strcmp($res, 'VERIFIED') == 0);
    }
    fclose($fp);
    if (!$valid)
    {
        $fail = "response from PayPal indicates INVALID notification post, data: " . $header . $req .
        " paypal verification return: " . $dbRes;
    }
}

if ($fail == '')
{
    $fail = dbConnect($db_conn);
}

if ($fail == '' && isDupTransaction($db_conn, $txnID))
{
    $fail = 'Duplicate transaction';
}

if ($fail == '')
{
    // check the payment_status is Completed
    // check that receiver_email is your Primary PayPal email
    if (strcmp($txnData['payment_status'], 'Completed') == 0)
    {
        // process payment
        debug('ppnotifytxn.php attempting postPayment');
        $fail = postPayment($db_conn, $txnData);
    }
}

if ($db_conn !== false)
    dbClose($db_conn);

if (isDebug())
{
$log = fopen('ppnotifytxn.log', 'a');
fwrite($log, "************************\nLog date, time:".date('Y-m-d:H:i:s') . "\n");
fwrite($log, "Post data:\n" . print_r($txnData, true) . "\n");
fwrite($log, "\nVerify query:\n" . $header . $req . "\n");
fwrite($log, "\nVerify response:\n" . $dbRes . "\n");
fwrite($log, "\nvalid:" . ($valid ? 'true' : 'false') . "\n");
fwrite($log, "payment_status:" . $txnData['payment_status'] . "\n");
fwrite($log, "result:" . (($fail == '')?'success':$fail). "\n");
fwrite($log, "************************\n");
fclose($log);
}

if ($fail != '')
{
    notifyError("PayPal transaction notification processing failure is:\n" . $fail .
    "\n for post data: \n" . $req, 'ppnotifytxn.php');
}
}
else
{
  // not a POST
  if (isDebug())
  {
  $log = fopen('ppnotifytxn.log', 'a');
  fwrite($log, "************************\nLog date, time:".date('Y-m-d:H:i:s') . "\n");
  fwrite($log, 'Access '.$_SERVER['REQUEST_METHOD'].' from '.$_SERVER['REMOTE_ADDR'].' as '.$_SERVER['REMOTE_HOST']);
  fwrite($log, "\n************************\n");
  fclose($log);
  }
  header('HTTP/1.0 403 Forbidden');
}
//debug('ppnotifytxn.php complete');
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
