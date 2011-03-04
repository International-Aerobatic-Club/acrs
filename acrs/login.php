<?php
/*  login.php, acrs, dclo, 10/24/2010
 *  publish and process login form
 *
 *  Changes:
 *    02/17/2011 dcl        move password reset instruction up above the form.
 *    10/24/2010 jim_ward	don't reference undefined array indices.
 */

set_include_path('./include');
require_once ('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodeHTML.inc");
require_once ("data/encodePOST.inc");
require_once ("data/password.inc");
require_once ("query/userQueries.inc");
require_once ("redirect.inc");
require_once ("ui/emailNotices.inc");
require_once ("useful.inc");
require ("ui/siteLayout.inc");
session_start();

function setSessionVariables($registrant)
{
    $_SESSION['userID'] = $registrant['userID'];
    $_SESSION['givenName'] = strhtml($registrant['givenName']);
    $_SESSION['familyName'] = strhtml($registrant['familyName']);
    $_SESSION['email'] = $registrant['email'];
    $_SESSION['isAdmin'] = sqlIsTrue($registrant['isAdmin']);
    $_SESSION['accountName'] = $registrant['accountName']; 
}

function writeLoginCookie($userID, $code)
{
    $time = time() + 60 * 60 * 24 * 360; //one year
    setcookie('iac_reg[id]', $userID, $time);
    setcookie('iac_reg[code]', $code, $time);
    //debug('Setting the cookie id=' . $userID . ', code=' . $code . "\n");
}

function clearLoginCookie()
{
    setcookie('iac_reg[id]', '');
    setcookie('iac_reg[code]', '');
}

function getAccountData($db_conn, $accountName, $pwd, & $registrant)
{
    $fail = '';
    $query = 'select userID, givenName, familyName, email, admin' .
    ' from registrant ' .
    'where rtrim(accountName)=' . strSQL($accountName, 32) .
    ' and rtrim(password)=' . strSQL($pwd,40);
    //debug('login.getAccountData query is '.$query);
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = notifyerror(dbErrorText(), "login.php");
    }
    else
    {
        if (dbCountResult($result) == 1)
        {
            $row = dbFetchRow($result);
            $registrant['userID'] = $row[0];
            $registrant['givenName'] = stripslashes($row[1]);
            $registrant['familyName'] = stripslashes($row[2]);
            $registrant['email'] = stripslashes($row[3]);
            $registrant['isAdmin'] = ($row[4] == 'y');
            $registrant['accountName'] = $accountName;
        }
        else
        {
            $fail = "Sorry, your account, \"" . strhtml($accountName) . "\" is not on file or the password does not match.";
        }
    }
    return $fail;
}

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
    $corrMsg = "<li>" . strhtml($fail) . "</li>";
}

$doForm = true;
$registrant = $_POST;
$password = '';
// get url or default
if (isset ($_GET["url"]))
{
    $url = urldecode($_GET["url"]);
}
else
{
    $url = "index.php";
}
// get ctstID
$haveQuery = false;
if (isset ($_GET["ctstID"]))
{
    $url .= '?ctstID=' . $_GET['ctstID'];
    $haveQuery = true;
}
if (isset ($_GET["uid"]))
{
    $registrant['accountName'] = urldecode($_GET["uid"]);
    $url .= $haveQuery ? '&' : '?';
    $url .= 'uid='.$_GET['uid'];
    $haveQuery = true;
}
if (isset ($_GET["pwd"]))
{
    $password = urldecode($_GET["pwd"]);
    $url .= $haveQuery ? '&' : '?';
    $url .= 'pwd='.$_GET['pwd'];
    $haveQuery = true;
}

if ($corrMsg == '' && isset ($registrant["submit"]))
{
    // save values for possible form redisplay
    $accountName = crop($registrant["accountName"], 32);
    $password = crop($registrant['pass'],32);
    $url = crop($registrant["url"], 255);

    if (strlen($accountName) == 0)
    {
        $corrMsg .= "<li>Provide an account name.</li>";
    }

    if (strlen($password) == 0)
    {
        $corrMsg .= "<li>Provide a password.</li>";
    }

    $pwd = encodePWD($password);
    if ($corrMsg == '')
    {
        //debug("Have valid login credentials\n");
        // all fields valid
       $corrMsg = getAccountData($db_conn, $accountName, $pwd, $registrant);
    }
    if ($corrMsg == '')
    {
        $doForm = FALSE;
        setSessionVariables($registrant);
        if (isset ($registrant['writeCookie']))
        {
            writeLoginCookie($accountName, $pwd);
        }
    }
}
else
{
    if ($corrMsg == '' && isset ($_COOKIE['iac_reg']))
    {
        $cookie = $_COOKIE['iac_reg'];
        $accountName = $cookie['id'];
        $pwd = $cookie['code'];
        $corrMsg = getAccountData($db_conn, $accountName, $pwd, $registrant);
        if ($corrMsg == '')
        {
            $doForm = FALSE;
            setSessionVariables($registrant);
            writeLoginCookie($accountName, $pwd);
        }
        else
        {
            clearLoginCookie();
        }
    }
}
dbClose($db_conn);

if (!$doForm)
{
    // will redirect if _SESSION["userID"] is established.
    if (redirect_session($url))
    {
        exit ();
    }
    else
    {
        $corrMsg = notifyError("Failed redirect to " . strhtml($url), "login.php");
    }
}

startHead("Registration Login");
startContent();
echo '<h1>Registration Login</h1>';
// registration form
// $corrMsg has HTML content
// $registrant has validated POST content
if (isset ($corrMsg) && $corrMsg != '')
{
    echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
}
echo '<p>This is the Aerobatic Contest Registration System (ACRS).  ';
echo 'You can get an overview of ACRS capabilities and features by ';
echo 'referring to <a href="docs/ACRS.pdf">this PDF document.</a></p>';
echo '<p style="font-weight:bold">If you need an account, please visit us here: ';
echo '<a href="loginAccount.php">Create an account</a>.</p>';
echo '<p style="font-weight:bold">If you have an account but ' .
'do not have your password, use this link: ' .
'<a href="reset.php">Need password reset.</a>' .
'</p>' . "\n";
echo '<p>' .
'<form name="login" class="userForm" action="login.php" method = "post">' .
'<table>' .
'<tbody>' . "\n";
echo '<tr><td class="requiredInput">Account name:</td>' .
'<td><input type="text" name="accountName" size="32" maxlength="32"';
echo ' value = "'. (isset ($registrant['accountName'])? strhtml($registrant['accountName']) : '') .'"';
echo '/></td>';
echo '</tr><tr>' .
'<td>Password:</td>' .
'<td class="requiredInput">' .
'<input type="password" name="pass" size="32" maxlength="32"';
echo ' value = "'.strhtml($password).'"/>' .
'</td>' .
'</tr>' .
'<td/>';
echo '<td><input type="checkbox" name="writeCookie"' .
		boolChecked($registrant, 'writeCookie').'/>' .
'this is my private, trusted computer; remember me.</td>' .
'</tr>';
echo '</tbody></table>' . "\n";
echo "<input type=\"hidden\" name=\"url\" value=\"" . strhtml($url) . "\"/>";
echo "<div class=\"userSubmit\">\n";
echo '<input type="submit" name="submit" value="Login"/>' .
'</div></form>' . "\n";

endContent();
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
