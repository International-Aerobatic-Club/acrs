<?php
require_once("ui/siteLayout.inc");
require_once("useful.inc");
session_start();
if (!isset($_SESSION['userID'])) {
  header("Location: login.php?url=" . urlencode($_SERVER["SCRIPT_NAME"]));
  exit();
}
else if (!isset($_COOKIE[session_name()])) 
{
  startHead('Cookie Trouble');
  startContent();
?>
<h1>Registration Login</h1>
    <p style="color:red;font-weight:bold;text-align:center">
	Please allow cookies from this site to your browser.
	This application uses a cookie to keep track of your login.
	Your browser will delete the cookie when you close it.
	(Sorry.  I couldn't tell before you tried the login.)
	</p>
	<p style="text-align:center"><a href="login.php">Retry login</a> after enabling cookies.</p>
<?php 
  endContent();
  exit();
}
//debugArr('ui/validate.inc session data:',$_SESSION);

/**
Call only with established session
*/
function verificationHeader($greeting)
{
  $givenName = $_SESSION['givenName'];
  $familyName = $_SESSION['familyName'];
  echo "<h2>".$greeting." ".$givenName." ".$familyName.".</h2>\n";
  echo "<span class='note'>(If you are not ".$givenName." ".$familyName.", ";
  echo "<a href=\"logoutin.php\">please login</a>.)</span>\n";
}

function isAdministrator()
{
   return $_SESSION['isAdmin'];
}
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
