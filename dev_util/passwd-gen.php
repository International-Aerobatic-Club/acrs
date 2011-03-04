<?php
if ($_POST[password] != "" AND $_POST[user] != "" ) {
  $usr = $_POST[user];
  $pass = $_POST[password];
  $ht_pass = crypt($pass);
}
print "<html><head><title>Password Encryption</title></head><body>
<form method=post action='passwd-gen.php'>
<font size=5><b>.htpasswd File Password Encryption</b></font>
<br><br>Enter Username<br>
<input name=user value='$usr' size=20>
<br><br>Enter Password<br>
<input name=password value='$pass' size=20>
<br><br><input type=submit name=submit value='Encrypt Now'>
";
if ($_POST[password] != "" AND $_POST[user] != "" ) {
  print "<br><br>.htpasswd File Code<br>$usr:$ht_pass";
}
print "</form></body></html>";
?> 
