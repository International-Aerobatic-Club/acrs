<?php
set_include_path('./include');
require_once ('ui/validate.inc');
require_once ('dbConfig.inc');
require_once ('dbCommand.inc');
require_once ('data/encodeSQL.inc');
require_once ('data/password.inc');
require_once ('useful.inc');
require_once ('ui/siteLayout.inc');
require_once ('ui/emailNotices.inc');

$doForm = TRUE;
$corrMsg = '';
$curPwd = '';
if (isset ($_GET["pwd"]))
{
    $curPwd = urldecode($_GET["pwd"]);
}
if (isset ($_POST["submit"]))
{
    $curPwd = crop($_POST["curPwd"], 32);
    $newPwd = crop($_POST["newPwd"], 32);
    $newPwdV = crop($_POST["newPwdV"], 32);
    if (strlen($newPwd) < 6)
    {
        $corrMsg = "The new password must have at least six characters.\n";
    }
    if (strcmp($newPwd, $newPwdV) != 0)
    {
        $corrMsg .= "The new password and repeat password do not match.\n";
    }
    if ($corrMsg == '')
    {
        $fail = dbConnect($db_conn);
        if ($fail == '')
        {
            $pwdOK = FALSE;
            $userID = $_SESSION['userID'];
            $query = "select password from registrant where userID = " . $userID . ";";
            $result = dbQuery($db_conn, $query);
            if ($result === false)
            {
                $fail = "Failed password lookup.";
            }
            else
            {
                $resultRow = dbFetchRow($result);
                $pwd = $resultRow[0];
                if ($pwd != encodePWD($curPwd))
                {
                    $corrMsg = "Current password does not match your password.";
                }
                else
                {
                    $pwdOK = TRUE;
                }
            }
            if ($pwdOK)
            {
                $update = "update registrant " .
                "set updated = now(), password = " . strSQL(encodePWD($newPwd), 40) .
                " where userID = " . $userID . ";";
                $fail = dbExec($db_conn, $update);
                if ($fail == '')
                {
                    $doForm = FALSE;
                    sendPasswordChangeEmail($_SESSION["email"], $_SESSION['givenName'] . ' ' . $_SESSION['familyName'], $_SESSION['accountName']);
                }
            }
        }
        if ($fail != '')
        {
            $corrMsg = notifyError($fail, "changePWD.php");
        }
        dbClose($db_conn);
    }
}
startHead('Password Change');
startContent();
if (!$doForm)
{
    // post change interface
    echo "<p>Your password has been changed.  Use your new password" .
    " the next time you log-in.</p>\n";
}
else
{
    // password change interface
?>
<p>
Enter your current and new password,
then press the "Update" button.
Passwords must have at least six characters.
They may have as many as thirty-two characters.
All characters are valid in a password.
Upper and lower-case alphabetic characters are distinct-- case matters.
</p><p>
<form action="changePWD.php" method = "post">
<table>
<tr>
<td align="right">current password</td>
<?php
echo '<td><input type="password" size="32" maxlength="32" name="curPwd"';
echo ' value="'.strhtml($curPwd).'"/>';
?>
</td>
</tr><tr>
<td align="right">new password</td>
<td><input type="password" size="32" maxlength="32" name="newPwd"/></td>
</tr><tr>
<td align="right">repeat new password</td>
<td><input type="password" size="32" maxlength="32" name="newPwdV"/></td>
</tr>
</table>
<input type="submit" name="submit" value="Update"/>
</form>
</p>
<?php

    if ($corrMsg != '')
    {
        echo '<p style="color:red; font-weight:bold">' . $corrMsg . '</p>';
    }
}
echo '<p><a href="index.php">Return to registration</a></p>';
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
