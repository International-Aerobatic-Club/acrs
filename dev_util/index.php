<?php
set_include_path('../acrs/include');
require_once("dbCommand.inc");
require_once("data/encodeSQL.inc");
require_once("data/encodeHTML.inc");
require_once("query/userQueries.inc");
require_once("useful.inc");
require("ui/siteLayout.inc");

// page processing

$corrMsg = '';
$fail = dbConnect($db_conn);
if ($fail != '')
{
  $corrMsg = "<li>".strhtml($fail)."</li>";
}

   startHead("Admin Functions");
?>   
   <style>
   div.userSubmit img {vertical-align:middle;}
   </style>
<?php
   startContent();
   echo '<h1>Admin Functions</h1>';
   // $corrMsg has HTML content
   if ($corrMsg != '')
   {
      echo '<ul style="color:red; font-weight:bold">'.$corrMsg.'</ul>';
   }
   endContent();
?>
