<?php
set_include_path('../acrs/include');

$dbprefix='';
$dbhost = 'localhost';
$dbuser = 'php';
$dbpasswd = 'ACyeFXKL';
$dbname = 'acrs';
$table_prefix = '';

$debug=true;
// debug action can be logfile name, or "echo"
$debug_action="/home/dcl/Documents/ws.iacusn/acrs/log/debug.log";
// email action can be logfile name, or "echo" or "mail"
$email_action="/home/dcl/Documents/ws.iacusn/acrs/log/email.log";

$admin_email = "doug@wbreeze.com";
//registrationURL is the url to the root directory of the registration system.
//end the url with a slash.
$registrationURL = "http://reg.localhost/acrs/";

//The following makes transactions with the sandbox:
$paypalServerName = 'www.sandbox.paypal.com';
//The following makes LIVE transactions
//$paypalServerName = 'www.paypal.com';

$paypalServerURL = 'http://reg.localhost/acrs_admin/paymentbypass.php';
//$paypalServerURL = 'https://'.$paypalServerName.'/cgi-bin/webscr';
$paypalReturnURL = 'http://acrs.localhost/ppreturn.php';
$paypalCancelURL = 'http://acrs.localhost/index.php';
$paypalNotifyURL = 'http://acrs.localhost/testNotify.php';
?>
<?php
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
