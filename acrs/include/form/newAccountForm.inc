<?php
/*  newAccountForm.inc, acrs/include/form, dlco, 10/23/2010
 *  display new account form to web user
 *
 *  Changes:
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.  Also, remove (some) references to undefined variables
 *                              and array indices.
 */

require_once ("dbConfig.inc");
require_once ("data/encodeHTML.inc");
require_once ("securimage.inc");
require_once ("useful.inc");

function displayNewAccountForm($registrant, $corrMsg)
{
    // membership form
    // $registrant has HTML content
    // $corrMsg has HTML content
   $submitText = "Create account";
    echo '<p>The system will use your account identity to store and retrieve contest ';
    echo 'registration information.  It will copy your information from an ';
    echo 'earlier contest when you register for a new contest, and allow you to make changes.</p>';
    echo '<p>After you complete this form, check your email.  Look for a welcome message from '.
    ADMIN_EMAIL.' that contains your email address.  We do it this way to confirm your email address.  ';
    echo 'Enter your email address carefully.</p>';
    echo '<p>Login using the link in the email.  It is best to highlight and copy the password from ';
    echo 'the email, then paste into the password field of the login screen.  This way O\'s do not get ';
    echo 'confused with 0\'s, i\'s with l\'s, l\'s with 1\'s, and upper case with lower case. ';
    echo 'If you have trouble using the password you get with the email, try using the ';
    echo '<a href="reset.php">password reset</a> form to mail yourself a new password.</p>';
    echo '<p>If you have difficulty, please try reading the <a href="#xtrahelp">additional instructions</a> ';
    echo 'below the form.</p>';
    if ($corrMsg != '')
    {
        echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
    }
    echo "<form name=\"registration\" method=\"post\" class=\"regForm\">";
    echo "<table> <tbody> <tr> \n";
    echo '<td class="form_text" colspan="2"><label for="in_accountName">Account Name:</label><input id="in_accountName" name="accountName" value="' .
         ($registrant["accountName"]? strhtml($registrant["accountName"]) : "") . '" maxlength="32" size="32"/></td>' . "\n";
    echo '</tr><tr>';
    echo '<td class="form_text"><label for="in_givenName">Given (first) Name:</label><input id="in_givenName" name="givenName" value="' .
         (isset ($registrant["givenName"])? strhtml($registrant["givenName"]) : "") . '" maxlength="72" size="32"/></td>' . "\n";
    echo '<td class="form_text"><label for="in_familyName">Family (last) Name:</label><input id="in_familyName" name="familyName" value="' .
         (isset ($registrant["familyName"])? strhtml($registrant["familyName"]) : "") . '" maxlength="72" size="32"/></td>' . "\n";
    echo '</tr><tr>';
    echo '<td class="form_text" colspan="2"><label for="in_email">eMail:</label><input id="in_email" name="email" value="' .
         (isset ($registrant["email"])? strhtml($registrant["email"]) : "") . '" maxlength="320" size="72"/></td>' . "\n";
    echo "</tr></tbody></table>\n";
    echo "<div class=\"regSubmit\">\n";
    echo "<h4>Copy the letters from the image before pressing, \"" . $submitText . ".\"";
    echo " (Character case does not matter.)</h4>\n";
    echo "<img src=\"antiRoboImage.php\"/>\n";
    echo "<input class=\"antiRobot\" type=\"text\" size=\"8\" maxlength=\"8\" name=\"antiRobot\"/>\n";
    echo "<input class=\"submit\" type=\"submit\" name=\"submit\" value=\"" . $submitText . "\"/>\n";
    echo "</div></form>\n";
    if (isset ($failRobo) && $failRobo)
    {
        echo '<p>';
        echo 'Copy the text of the image into the text entry field ';
        echo 'next to the image. ';
        echo 'Both upper and lower-case letters will match. ';
        echo '<a href="mailto:' . ADMIN_EMAIL . '?subject=iac52%20member%20anti-robot%20difficulty">Email the member administrator</a> ';
        echo 'if you have difficulty reading the image.';
        echo '</p>';
    }
    echo '<div class="returnButton"><a href="login.php">Return to login</a></div>';
    
    echo '<a name="xtrahelp"><h3>Additional Instructions</h3>';
    echo '<p>You may use your IAC member number for your account name, ';
    echo 'or any other unique name.  ';
    echo 'If you use your IAC member number, be sure it is in fact your number.  ';
    echo 'Many people think they know their member number, only it\'s someone else\'s number ';
    echo 'that they know.  Double-check your member card.  If this is a problem, use something ';
    echo 'that works better for you.  There is currently no way to change your account name ';
    echo 'once you have chosen it.</p>';
    echo '<p><b>All of this assumes you are able to type your email address correctly.</b>  ';
    echo 'If you can type your email address correctly and follow a link, you can most likely ';
    echo 'get through this.</p>';
    echo '<p>If you can\'t make this work, write to <a href="mailto:' . ADMIN_EMAIL . '?subject=account help">';
    echo ADMIN_EMAIL . '</a>.';
    echo 'Try to include details about what you have tried and where you think things are breaking-down ';
    echo 'for you.  Know that many of your fellow competitors have made it through this sequence ';
    echo 'without zeros.  Some of them are flying sixty-five horsepower clipped wing cubs.</p>';
}

function displayVerifyForm($registrant)
{
    // account verification form for duplicate email address
    // $registrant has HTML content
    // $corrMsg has HTML content
    echo '<p>The email address you provided, '.strhtml($registrant['email']).
    ' belongs to an account already registered. '.
    'If you share this email with someone else, we\'ll add your additional '.
    'account with the same address.  '.
    'Otherwise, please ask us to send a password reset to you, find your '.
    'existing account information and log-in with that, or use a different '.
    'email address to establish the account.</p>';
    echo '<form name="reset" method="post" action="reset.php">';
    echo '<input type="hidden" name="email" value="' . strhtml($registrant["email"]) . '"/>';
    echo '<input type="hidden" name="antiRobot" value="'.strhtml($registrant["antiRobot"]).'"/>';
    echo '<input class="submit" type="submit" name="submit" value="Get a password reset"/>';
    echo "</form>\n";
    echo '<form name="login" method="post" action="login.php">';
    echo '<input type="hidden" name="accountName" value="' . strhtml($registrant["accountName"]) . '"/>';    
    echo '<input class="submit" type="submit" name="submit" value="Try existing account information"/>';
    echo "</form>\n";
    echo '<form name="registration" method="post">';
    echo '<input type="hidden" name="useDuplicateEmail" value="yes"/>';    
    echo '<input type="hidden" name="accountName" value="' . strhtml($registrant["accountName"]) . '"/>';    
    echo '<input type="hidden" name="givenName" value="' . strhtml($registrant["givenName"]) . '"/>';
    echo '<input type="hidden" name="familyName" value="' . strhtml($registrant["familyName"]) . '"/>';
    echo '<input type="hidden" name="email" value="' . strhtml($registrant["email"]) . '"/>';
    echo '<input type="hidden" name="antiRobot" value="'.strhtml($registrant["antiRobot"]).'"/>';
    echo '<input class="submit" type="submit" name="changeEmail" value="Use a different email address."/>';
    echo '<input class="submit" type="submit" name="submit" value="More than one person shares this email address."/>';
    echo "</form>\n";
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
