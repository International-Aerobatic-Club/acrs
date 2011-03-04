<?php

define( 'SEND_BUT', "Send" );

$temp = $_POST['action'];

if( $temp == SEND_BUT )
{
	// get values for address, subject, and message
	$address = $_POST['address'];
	$subject = $_POST['subject'];
	$message = $_POST['message'];
	echo("Addressee: $address<br>");
	echo("Subject: $subject<p>");
	echo("Message text: $message<br>");
	echo("<br><br>");
	echo("Attempting to send now!");
	$x = mail($address, $subject, $message);
	echo("Sent(?) with return value $x .<br>");
	echo("</body></html>");
}
else 
{
	// output form
	$formtext = <<< end_of_form
<html><body>
<form action= "mailtest.php" method="POST">
Enter e-mail address, subject, and message to send:<br><br>
e-mail address:  <input type="text" name="address" /> <br>
subject: <input type="text" name="subject" /> <br>
message: <input type="text" name="message" /> <br>
<input type="submit" name="action" value="Send" />
</form></body></html>
end_of_form;

	echo $formtext;
}
?>