<?php

set_include_path('./include'); 

DEFINE ("DEBUG",             true);                             // true to generate debug output, false to suppress
DEFINE ("DEBUG_ACTION",      "/home/dcl/Documents/ws.iacusn/acrs/log/debug.log"); // "echo" to send debug output to stdout/web page, else log file name
DEFINE ("EMAIL_ACTION",      "/home/dcl/Documents/ws.iacusn/acrs/log/email.log"); // "echo" to send email output to stdout/web page, "mail" to mail it, else log file name
DEFINE ("ADMIN_NAME",        "Douglas Lovell");                 // name of the person responsible for the site
DEFINE ("ADMIN_EMAIL",       "doug@wbreeze.com");               // responsible person's email address
DEFINE ("REGISTRATION_URL",  "http://acrs.localhost/");         // URL of the acrs registration web site root directory, must be end with "/"
DEFINE ("ADMIN_URL",         "http://acrs_admin.localhost/");   // URL of the acrs admin web site root directory, must end with "/"
DEFINE ("TEST_EMAIL_SERVER", "wbreeze.com");                    // domain name of email server where test messages are sent
DEFINE ("DBHOST",            "localhost");                      // database host
DEFINE ("DBPREFIX",          "");                               // database name and user name prefix
DEFINE ("DBNAME",            "acrs");                           // database name
DEFINE ("DBUSER",            "php");                            // database user name
DEFINE ("DBPASSWD",          "ACyeFXKL");                       // database password

/*  Certain PayPal-related constants change depending on whether the code is running in test or production.
 */

DEFINE ("ACRS_PAYPAL_MODE", "local");         // one of local, sandbox, live

/*  For live production:
 */
if (ACRS_PAYPAL_MODE == "live") {
       DEFINE ("PAYPAL_SERVER_NAME","www.paypal.com");
       DEFINE ("PAYPAL_SERVER_URL", "https://" . PAYPAL_SERVER_NAME . "/cgi-bin/webscr");
       DEFINE ("PAYPAL_NOTIFY_URL", REGISTRATION_URL . "ppnotifytxn.php");
}

/*  For testing with the PayPal sandbox:
 */
elseif (ACRS_PAYPAL_MODE == "sandbox") {
    DEFINE ("PAYPAL_SERVER_NAME","www.sandbox.paypal.com");
    DEFINE ("PAYPAL_SERVER_URL", "https://" . PAYPAL_SERVER_NAME . "/cgi-bin/webscr");
    DEFINE ("PAYPAL_NOTIFY_URL", REGISTRATION_URL . "ppnotifytxn.php");
}


/*  Local testing without contacting PayPal.  Note that PAYPAL_SERVER_NAME is not defined; it is not
 *  used in this case.
 */
elseif (ACRS_PAYPAL_MODE == "local") {
    DEFINE ("PAYPAL_SERVER_URL", ADMIN_URL . "paymentbypass.php");
    DEFINE ("PAYPAL_NOTIFY_URL", REGISTRATION_URL . "testNotify.php");
}


/*  Undefined/mis-defined:
 */
else {
    echo "ACRS_PAYPAL_MODE unknown";
    exit;
}

/*  These two DEFINEs remain unchanged, regardless of whether the code is operating in test or production mode.
 */
DEFINE ("PAYPAL_CANCEL_URL", REGISTRATION_URL . "index.php");       // paypal "cancel" button return point
DEFINE ("PAYPAL_RETURN_URL", REGISTRATION_URL . "ppreturn.php");    // paypal return point URL for completed payment


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
