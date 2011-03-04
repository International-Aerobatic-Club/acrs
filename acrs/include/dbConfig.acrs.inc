<?php
$dbprefix='wbreezec_';
$dbhost = 'localhost';
$dbuser = 'sitePHP';
$dbpasswd = 'goN7l0xmRoTa/v2aAa0E';
$dbname = 'iacreg';
$dbtype = 'mysql';
$table_prefix = '';

//$debug=true;
// debug action can be logfile name, or "echo"
//$debug_action="mail";
// email action can be logfile name, or "echo" or "mail"
$email_action="mail";

$admin_email = "admin@iacusn.org";
//registrationURL is the url to the root directory of the registration system.
//end the url with a slash.
$registrationURL = "http://www.iacusn.org/acrs/";

//The following makes transactions with the sandbox:
//$paypalServerName = 'www.sandbox.paypal.com';
//The following makes LIVE transactions
$paypalServerName = 'www.paypal.com';
$paypalServerURL = 'https://'.$paypalServerName.'/cgi-bin/webscr';

//URL for user return after payment
$paypalReturnURL = 'http://www.iacusn.org/acrs/ppreturn.php';
//URL for user return after cancel payment
$paypalCancelURL = 'http://www.iacusn.org/acrs/index.php';
//URL for PayPal notification of payment
$paypalNotifyURL = 'http://www.iacusn.org/acrs/ppnotifytxn.php';
?>
