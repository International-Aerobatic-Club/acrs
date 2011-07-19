<?php
/*  paymentbypass.php, admin, dclo, 10/24/2010
 *  paypal bypass code for testing
 *
 *  Changes:
 *    10/25/2010 jim_ward       use REGISTRATION_URL constant.  Use set_include_path
 *                              to reach all of the include files needed by this module.
 */

/**
Post a payment.
****************************************************
This file must reside in a protected directory.
****************************************************
A post to this file using the PayPal button format
causes update of the registrant paid status.
Records the payment in the database.
*/

set_include_path ('../acrs/include');
require_once ('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");
require_once ("query/userQueries.inc");

function postPayment($db_conn, $regID, $amt)
{
    $fail = '';
    $query = 'select paidAmt from registration where regId = ' . intsql($regID);
    //debug($query);
    $result = dbQuery($db_conn, $query);
    $curAmt = 0;
    if ($result === false)
    {
        $fail = notifyError($fail, 'paymentbypass.php');
    }
    else
    {
        $row = dbFetchRow($result);
        if ($row === false)
        {
            $fail = notifyError('missing registrant ' . $regID, ' in paymentbypass.php');
        }
        else
        {
            $curAmt = $row[0];
        }
    }

    if ($fail == '')
    {
        $update = 'update registration set paidAmt = ';
        if ($curAmt == null)
        {
            $update .= intSQL($amt);
        }
        else
        {
            $update .= $curAmt . '+' . intSQL($amt);
        }
        $update .= ' where regID = ' . intSQL($regID);
        debug('paymentbypass.postPayment():'.$update);
        $fail = dbExec($db_conn, $update);
    }
    return $fail;
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
    notifyError($fail, "paymentbypass.php");
    $corrMsg = "<it>Internal: failed access to contest database</it>";
}
if ($fail == '')
{
    if ($_POST["submit"])
    {
        debugArr('paymentbypass post data', $_POST);
        $reg = $_POST['item_name'];
        $amt = $_POST['amount'];
        $regID = $_POST['custom'];
        $corrmsg = postPayment($db_conn, $regID, $amt);
    }
    else
    {
        $corrmsg = "not for navigation";
    }
}

// registration form
// $corrMsg has HTML content
// $registrant has POST content
startHead("Registration Payment");
startContent();
echo "<h1>Registration Payment</h1>";
if ($corrMsg != '')
{
    echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
}
else
{
    echo '<p>We have credited your registration payment of $' . $amt . ' dollars';
    echo ' for ' . $reg . '.</p>';
}
echo '<p><a href="'.REGISTRATION_URL.'index.php">Return to registration</a></p>';
endContent();
dbClose($db_conn);
?>
