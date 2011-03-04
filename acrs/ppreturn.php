<?php
/*  ppreturn.php, acrs, dlco, 10/23/2010
 *  return form from PayPal
 *
 *  Changes:
 *    10/23/2010 jim_ward       use ADMIN_EMAIL.
 */

set_include_path('./include');
require_once ('dbConfig.inc');
require_once('useful.inc');
require_once('ui/siteLayout.inc');

   startHead("Registration Payment Return");
   startContent();
   echo "<h1>Registration Payment</h1>";
   echo "<p>Thank you for your registration payment.".
        "  Please retain your email confirmation from PayPal.".
        "  If the registration system still requests a payment, " .
        "please give it a minute to process your transaction notification, ".
        "then refresh your browser.  It may take as long as <b>several hours</b> " .
        "when the PayPal server is experiencing high transaction volumes.  ".
        "If after <b>several hours</b> the system still asks for payment, ".
        "forward your PayPal payment confirmation email ".
        "to the <a href='mailto:".ADMIN_EMAIL."'>the site admin</a>, who " .
   		"will straighten it out.</p>";
   echo "<p><a href='index.php'>return to registration</a></p>";
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
