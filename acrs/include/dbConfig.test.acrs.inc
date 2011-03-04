<?php
$dbprefix='wbreezec_';
$dbhost = 'localhost';
$dbuser = 'sitePHP';
$dbpasswd = 'goN7l0xmRoTa/v2aAa0E';
$dbname = 'iacregTest';
$dbtype = 'mysql';
$table_prefix = '';

$debug=true;
// debug action can be logfile name, or "echo"
$debug_action="/home/wbreezec/log/test.acrs.debug.log";
// email action can be logfile name, or "echo" or "mail"
//$email_action="/home/dcl/Documents/ws.iacusn/acrs/log/email.log";
$email_action="mail";

$admin_email = "doug@wbreeze.com";
//registrationURL is the url to the root directory of the registration system.
//end the url with a slash.
$registrationURL = "http://www.iacusn.org/test/acrs/";

//The following makes transactions with the sandbox:
$paypalServerName = 'www.sandbox.paypal.com';
//The following makes LIVE transactions
//$paypalServerName = 'www.paypal.com';

//$paypalServerURL = 'http://www.iacusn.org/test/acrs/acrs_admin/paymentbypass.php';
$paypalServerURL = 'https://'.$paypalServerName.'/cgi-bin/webscr';
$paypalReturnURL = 'http://www.iacusn.org/test/acrs/ppreturn.php';
$paypalCancelURL = 'http://www.iacusn.org/test/acrs/index.php';
$paypalNotifyURL = 'http://www.iacusn.org/test/acrs/testNotify.php';
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
